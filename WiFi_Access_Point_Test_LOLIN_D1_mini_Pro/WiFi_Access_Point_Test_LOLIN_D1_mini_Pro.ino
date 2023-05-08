/* 
    ESP8266 WiFi Captive Portal
    https://github.com/adamff1/ESP8266-Captive-Portal

    Cyclenerd/AccessPoint.ino
    https://gist.github.com/Cyclenerd/7c9cba13360ec1ec9d2ea36e50c7ff77
*/

#include <EEPROM.h>
#include <list>

#include <ESP8266WiFi.h>
#include <ESP8266LLMNR.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include "ESP8266TimerInterrupt.h"             //https://github.com/khoih-prog/ESP8266TimerInterrupt

IPAddress APIP(172, 0, 0, 1); // Gateway
const byte DNS_PORT = 53;
const byte HTTP_CODE = 200;

DNSServer dnsServer;
ESP8266WebServer webServer(80);




namespace WIFI_CONNECTOR
{
    bool isConnected = false;
    IPAddress NOW_IP;

    #define SAVED_WIFI_NET_MAX_COUNT 10
    struct WIFI_NET {
        char id[33]; // MAX 32 letters + NULL
        char pw[33]; // MAX 32 letters + NULL

        WIFI_NET(){}
        WIFI_NET(String apName, String apPw) {
            strcpy(this->id, apName.c_str());
            strcpy(this->pw, apPw.c_str());
        }
    };
    std::list<WIFI_NET> SAVED_WIFI_NET_LIST;

    void loadWifiList() {
        int addr = 0;
        WIFI_NET wifi;

        for (int i = 0; i < SAVED_WIFI_NET_MAX_COUNT; i++) {
            EEPROM.get(addr, wifi);

            // If any BROKEN character EXISTS, IGNORE.
            for(char c : wifi.id)
                if(c > 127){ strcpy(wifi.id, ""); break; }
            for(char c : wifi.pw)
                if(c > 127){ strcpy(wifi.pw, ""); break; }

            SAVED_WIFI_NET_LIST.push_back(wifi);

            addr += sizeof(WIFI_NET);
        }
    }

    void saveWifiList(String apName, String apPw) {
        // DEL a most old WIFI_NET
        SAVED_WIFI_NET_LIST.pop_back();
        // ADD a new WIFI_NET
        SAVED_WIFI_NET_LIST.push_front(WIFI_NET(apName, apPw));

        int addr = 0;

        for (WIFI_NET wifi : SAVED_WIFI_NET_LIST) {
            EEPROM.put(addr, wifi);

            addr += sizeof(WIFI_NET);
        }
    }

    std::list<String> scanNetList()
    {
        std::list<String> scanedNetList;

        int numNet = WiFi.scanNetworks();
        for(int i=0; i<numNet; i++)
        {
            String ssid = WiFi.SSID(i);

            // If any BROKEN character EXISTS, IGNORE.
            for(char c : ssid){
                if(c > 127){ ssid=""; break; }
            }

            if(ssid.length() <= 0) // valid check
                continue;

            // Add SSID when NOT DUPLICATED
            if (std::find(scanedNetList.begin(), scanedNetList.end(), ssid) == scanedNetList.end())
                scanedNetList.push_back(ssid);
        }

        return scanedNetList;
    }
}





String html;

String htmlIndex = R"(
<!DOCTYPE HTML>
<html>
<head>
	<title>HACK</title>
</head>
<body>
	<h1>This is HACK</h1>
</body>
</html>
)";

const char rawhtml_SELECT_WIFI[] PROGMEM = R"(
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="Content-Type" Content="text/html; charset=utf-8">
    <meta name='viewport' content='width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no'>

    <title>WIFI 목록</title>

    <style>
    body
    {font-size:1.2rem;}

    .a:hover
    {cursor:pointer;}

    #spinner
    {z-index:9;position:fixed;top:0;left:0;right:0;bottom:0;background-color:rgba(0, 0, 0, 0.7);padding:0 auto;display:none;}
    #spinner .spinner-around
    {z-index:99;position:absolute;top:50%;left:50%;margin:-25px 0 0 -25px;width:50px;height:50px;border-radius:50%;border:7px solid rgba(0, 0, 0, 0.1);animation:none;}
    #spinner .spinner-inner
    {z-index:999;position:absolute;top:50%;left:50%;margin:-25px 0 0 -25px;width:50px;height:50px;border-radius:50%;border:7px solid transparent;border-top-color:#3498db;}
    </style>
</head>
<body>
    <h1>WiFi List</h1>
    인증가능한 WiFi를 선택하세요. <br>

    <div id='wifiList'>#{strWiFiList}</div>

    <div id='divForm'>
        <form method='post' action='/connectToAP' onsubmit='return whenSubmit();'>
            SELECTED SSID : <br>
            <input type='text' name='apName' readonly> <br>
            PASSWORD(CAN BE NONE) : <br>
            <input type='password' name='apPw'> <br>
            <input type='submit' value='CONNECT'>
        </form>
    </div>

    <div id='spinner'>
        <span class='spinner-around'></span>
        <span class='spinner-inner'></span>
    </div>
</body>
<script>
    const divForm = document.getElementById('divForm');
    divForm.style.visibility = 'hidden';

    const arrLiApName = document.querySelectorAll('.liApName');
    const aSummary = document.querySelector('#aSummary');
    if(arrLiApName.length > 10)
    {
        aSummary.style.display='block';
        aSummary.addEventListener("click", () => { for (liApName of arrLiApName) liApName.style.display=''; aSummary.style.display='none'; });
    }
    let i = 1;
    for (liApName of arrLiApName) {
        const aTag = liApName.querySelector('a');
        aTag.addEventListener("click", () => { select(aTag.name); });

        if(i++>10) liApName.style.display='none';
    }
    function select(apName){
        for (liApName of arrLiApName) {
            if(liApName.querySelector('a').name === apName)
                liApName.style.color = 'navy';
            else
                liApName.style.color = 'gray';
        }

        divForm.style.visibility = 'visible';
        divForm.querySelector('[name="apName"]').value = apName;
        divForm.querySelector('[name="apPw"]').value = ''
    }
</script>
<script>
    //spinner by ChatGPT
    const spinnerInner = spinner.querySelector('.spinner-inner');
    let degree = 0;
    
    const animateSpinner = () => {
        degree = (degree+5) % 360;

        spinnerInner.style.transform = `rotate(${degree}deg)`;
        spinnerInner.style.borderLeftColor = '#3498db';

        setTimeout(() => { spinnerInner.style.borderLeftColor = 'transparent'; animateSpinner(); }, 20);
    };
    animateSpinner();

    function whenSubmit(){
        document.querySelector('#spinner').style.display = 'block';
        return true;
    }
</script>
</html>
)";

String html_SELECT_WIFI()
{
    String result = String(rawhtml_SELECT_WIFI);


    // MAKE strWiFiList
    Serial.println("WIFI SCANNING START...");
    String strWiFiList ="";
    {
        strWiFiList += "<ul>";
        
        for (String ssid : WIFI_CONNECTOR::scanNetList())
        {
            if(ssid.length() <= 0) // valid check
                continue;

            strWiFiList += "<li class='liApName'><a name='" + ssid + "'>" + ssid + "</a> </li>";
        }

        strWiFiList += "<a id='aSummary' style='display:none;text-decoration:underline';>...</a>";
        strWiFiList += "</ul>";
    }
    Serial.println("WIFI SCANNING END!");


    // INSERT encoded strWiFiList
    result.replace("#{strWiFiList}", strWiFiList);


    return result;
}









ESP8266Timer TimerInterrupt;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Serial begin
    Serial.begin(115200);
    Serial.println("\nSERIAL IS ONLINE\n");



    // Wifi settings
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP( F("I AM NOT WIFI") );

    // DNS spoofing (Only for HTTP)
    // if DNSServer is started with "*" for domain name, it will reply with
    // provided IP to all DNS request
    dnsServer.start(DNS_PORT, "*", APIP);

    // Start LLMNR responder
    LLMNR.begin("esp8266");





    // Try to Connect from the SAVED_WIFI_NET_LIST
    {
        WIFI_CONNECTOR::loadWifiList();



        //test!!!!!!!!!!!!!!!!!!!
        //test!!!!!!!!!!!!!!!!!!!
        //test!!!!!!!!!!!!!!!!!!!
        {
            // WIFI_CONNECTOR::SAVED_WIFI_NET_LIST.pop_back();
            // WIFI_CONNECTOR::SAVED_WIFI_NET_LIST.push_front(  WIFI_CONNECTOR::WIFI_NET("Sophist-Router", "sophist1306")  );
            
            // SERIAL scan test
            int i=0;
            for (WIFI_CONNECTOR::WIFI_NET wifi : WIFI_CONNECTOR::SAVED_WIFI_NET_LIST) {
                Serial.print("Wifi ");
                Serial.print(i++);
                Serial.print(" : "); Serial.print(wifi.id);
                Serial.print(" / "); Serial.println(wifi.pw);
            }
        }
        //test!!!!!!!!!!!!!!!!!!!
        //test!!!!!!!!!!!!!!!!!!!
        //test!!!!!!!!!!!!!!!!!!!



        for (String ssid : WIFI_CONNECTOR::scanNetList())
        {
            if(ssid.length() <= 0) // valid check
                continue;

            for (const auto& wifiNet : WIFI_CONNECTOR::SAVED_WIFI_NET_LIST)
            {
                if (ssid.equals(wifiNet.id))
                {
                    // CONNECT TO NET !
                    WiFi.begin(wifiNet.id, wifiNet.pw);
                    Serial.println( "\nTry to connect to. [ " + String(wifiNet.id) + " / " + String(wifiNet.pw) + " ]" );

                    // Try to CONNECT, up to 5 seconds.
                    int i=0;
                    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); if(i++ > 10) break; }
                }
            }
        }
    }

    if(WiFi.status() == WL_CONNECTED)
    {
        WIFI_CONNECTOR::isConnected = true;
        WIFI_CONNECTOR::NOW_IP = WiFi.localIP();

        Serial.println(" SUCCEX!!");
        Serial.print("Now Wifi Connected with IP Address : ");
        Serial.println(WIFI_CONNECTOR::NOW_IP);
        Serial.println();
    }










    // WebServer requests
    // webServer.on("/post",[]() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
    // webServer.on("/ssid",[]() { webServer.send(HTTP_CODE, "text/html", ssid()); });
    // webServer.on("/postSSID",[]() { webServer.send(HTTP_CODE, "text/html", postedSSID()); });
    // webServer.on("/pass",[]() { webServer.send(HTTP_CODE, "text/html", pass()); });
    // webServer.on("/clear",[]() { webServer.send(HTTP_CODE, "text/html", clear()); });

    webServer.on        (   "/test",
                            []
                            {
                                html = htmlIndex;
                                webServer.send(HTTP_CODE, "text/html", html);
                            }
                        );

    webServer.on        (   "/connectToAP",
                            []
                            {
                                html = htmlIndex;


                                // args from client
                                String apName;
                                String apPw;

                                for (uint8_t i=0; i<webServer.args(); i++){
                                    if(webServer.argName(i).equals("apName"))
                                        apName = webServer.arg(i);
                                    if(webServer.argName(i).equals("apPw"))
                                        apPw = webServer.arg(i);
                                }


                                // CONNECT TO NET !
                                WiFi.begin(apName, apPw);
                                Serial.println( "\nTry to connect to. [ " + apName + " / " + apPw + " ]" );

                                // Try to CONNECT, up to 5 seconds.
                                int i=0;
                                while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); if(i++ > 10) break; }


                                // CONNECT SUCCEX !
                                if(WiFi.status() == WL_CONNECTED)
                                {
                                    WIFI_CONNECTOR::isConnected = true;
                                    WIFI_CONNECTOR::NOW_IP = WiFi.localIP();


                                    //
                                    // 리스트에 ID PW 모두 매칭되는게 없을때 <<
                                    // 세이브 하는거 만들어야함!!!!!
                                    // WIFI_CONNECTOR::saveWifiList(String apName, String apPw)
                                    //


                                    Serial.println(" SUCCEX!!");
                                    Serial.print("Now Wifi Connected with IP Address : ");
                                    Serial.println(WIFI_CONNECTOR::NOW_IP);
                                    Serial.println();


                                    html.replace("<h1>This is HACK</h1>", String("<h1>Connected SUCCEX!!<br>Now IP : " + WIFI_CONNECTOR::NOW_IP.toString() + " </h1>"));
                                }
                                // CONNECT DENIED..
                                else
                                {
                                    Serial.println(" DENIED..");
                                    Serial.print("Now Wifi Connected with IP Address : ");
                                    Serial.println(WIFI_CONNECTOR::NOW_IP);
                                    Serial.println();


                                    html.replace("<h1>This is HACK</h1>", "<h1>NOOOOOOOOO</h1>");
                                }


                                webServer.send(HTTP_CODE, "text/html", html);
                            }
                        );
    webServer.onNotFound(
                            []
                            { 
                                html = html_SELECT_WIFI();
                                webServer.send(HTTP_CODE, "text/html", html);
                            } 
                        );
    webServer.begin();

    Serial.println("\n");
    Serial.println("SERVER STARTED!!");
    Serial.println("TimerInterrupt START!!");

    // ALL READY! LED BLLLLLINK!!
    TimerInterrupt.attachInterruptInterval  (1000,  [] 
                                                    {
                                                        //per1ms!
                                                        {
                                                            
                                                        }

                                                        //per500ms!
                                                        if( !(millis()%500) )
                                                        {
                                                            static bool stateLED = false;
                                                            digitalWrite(LED_BUILTIN, stateLED^=1 ? HIGH : LOW);
                                                        }
                                                    }
                                            );
}

void loop()
{
    delay(1);

    dnsServer.processNextRequest();
    webServer.handleClient();
}
