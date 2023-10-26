/* 
    ESP8266 WiFi Captive Portal
    https://github.com/adamff1/ESP8266-Captive-Portal

    Cyclenerd/AccessPoint.ino
    https://gist.github.com/Cyclenerd/7c9cba13360ec1ec9d2ea36e50c7ff77
*/

#include <EEPROM.h>

#define IS_CLEARING 0       // 0:MAIN   1:CLEAR EEPROM
#if (IS_CLEARING == 1)

void setup(){ EEPROM.begin(4096); for(int i=0;i<4096;i++){ EEPROM.write(i,0); } EEPROM.end(); } void loop(){}

#else

#include <algorithm>
#include <list>

#include <ESP8266WiFi.h>
#include <ESP8266LLMNR.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <lwip/napt.h>
#include <lwip/dns.h>

#include <ESP8266TimerInterrupt.h>  // https://github.com/khoih-prog/ESP8266TimerInterrupt

#include "res_html_INDEX.h"
#include "res_html_SELECT_WIFI.h"

#define DISABLE_INTERRUPTS() cli()
#define ENABLE_INTERRUPTS() sei()





#define STATUS_COMMON           0x00
#define STATUS_WIFI_PROVIDED    0x01

static byte NOW_MODULE_STATUS = STATUS_COMMON;





namespace WIFI_CONNECTOR
{
    bool isConnected = false;
    IPAddress NOW_IP;
    String NOW_WIFI_ID;
    String NOW_WIFI_PW;

    #define SAVED_WIFI_NET_MAX_COUNT 10
    struct WIFI_NET
    {
        char id[33]; // MAX 32 letters + NULL
        char pw[33]; // MAX 32 letters + NULL

        WIFI_NET(){}
        WIFI_NET(String apName, String apPw)
        {
            strcpy(this->id, apName.c_str());
            strcpy(this->pw, apPw.c_str());
        }
    };
    std::list<WIFI_NET> SAVED_WIFI_NET_LIST;

    void loadWifiList()
    {
        DISABLE_INTERRUPTS();
        // DISABLE INTERRUPT while using EEPROM
        {
            int addr = 0;
            WIFI_NET wifi;

            for (int i = 0; i < SAVED_WIFI_NET_MAX_COUNT; i++) {
                EEPROM.get(addr, wifi);
                SAVED_WIFI_NET_LIST.push_back(wifi);

                addr += sizeof(WIFI_NET);
            }
        }
        ENABLE_INTERRUPTS();
    }

    void saveWifiList()
    {
        // If already EXISTS, FUCK OFF.
        bool isAlreadyExist = false;
        for (WIFI_NET wifi : SAVED_WIFI_NET_LIST) {
            if(NOW_WIFI_ID.equals(wifi.id) && NOW_WIFI_PW.equals(wifi.pw))
                isAlreadyExist = true;
        }
        if(isAlreadyExist)
            return;


        // DEL a most old WIFI_NET
        SAVED_WIFI_NET_LIST.pop_back();
        // ADD a new WIFI_NET
        SAVED_WIFI_NET_LIST.push_front( WIFI_NET(NOW_WIFI_ID, NOW_WIFI_PW) );


        DISABLE_INTERRUPTS();
        // DISABLE INTERRUPT while using EEPROM
        {
            int addr = 0;

            for (WIFI_NET wifi : SAVED_WIFI_NET_LIST) {
                EEPROM.put(addr, wifi);

                addr += sizeof(WIFI_NET);
            }

            if (EEPROM.commit())
                Serial.print("SAVE......... COMPLETE!");
            else
                Serial.println("SAVE......... ERROR!?");
        }
        ENABLE_INTERRUPTS();
    }

    std::list<String> scanNetList()
    {
        struct SCANNED_NET {
            String ssid;
            int32_t rssi;
            
            SCANNED_NET(String ssid, int32_t rssi) : ssid(ssid), rssi(rssi) {}
        };

        std::vector<SCANNED_NET> scanedNetList;

        int numNet = WiFi.scanNetworks();
        for(int i=0; i<numNet; i++){
            String ssid = WiFi.SSID(i);
            int32_t rssi = WiFi.RSSI(i);

            // If INVALID
            if(ssid.length() <= 0)
                continue;

            // If DUPLICATED
            bool isDuplicated = false;
            for(SCANNED_NET net : scanedNetList){
                if(net.ssid.equals(ssid)){ isDuplicated = true; break; }
            }
            if(isDuplicated)
                continue;

            scanedNetList.push_back( SCANNED_NET(ssid, rssi) );
        }

        // SORT !
        std::sort   (   scanedNetList.begin(), scanedNetList.end(),
                        [](const SCANNED_NET& a, const SCANNED_NET& b)
                        {
                            return a.rssi > b.rssi; // ORDER BY DESC
                        }
                    );

        std::list<String> result;
        for(SCANNED_NET net : scanedNetList)
            result.push_back( net.ssid );

        return result;
    }
}





DNSServer dnsServer;
ESP8266WebServer webServer(80);

ESP8266Timer TimerInterrupt;





void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);


    // Serial begin
    Serial.begin(115200);
    delay(4444);
    Serial.println("\nSERIAL IS ONLINE\n");


    // EEPROM begin
    EEPROM.begin(4096);


    // Try to CONNECT from the SAVED_WIFI_NET_LIST
    {
        WIFI_CONNECTOR::loadWifiList();

        /*
        // SERIAL scan test
        {
            int i=0;
            for (WIFI_CONNECTOR::WIFI_NET wifi : WIFI_CONNECTOR::SAVED_WIFI_NET_LIST)
            {
                Serial.print("Wifi "); Serial.print(i++); Serial.print(" : ");
                Serial.print(wifi.id); Serial.print(" / "); Serial.println(wifi.pw);
            }
        }
        */

        for (String ssid : WIFI_CONNECTOR::scanNetList())
        {
            if(WIFI_CONNECTOR::isConnected) // already Connected
                break;

            if(ssid.length() <= 0) // str validation
                continue;

            for (WIFI_CONNECTOR::WIFI_NET wifiNet : WIFI_CONNECTOR::SAVED_WIFI_NET_LIST)
            {
                if (ssid.equals(wifiNet.id))
                {
                    // Try to CONNECT TO NET !
                    WiFi.begin(wifiNet.id, wifiNet.pw);
                    Serial.println( "\nTry to connect to. [ " + String(wifiNet.id) + " / " + String(wifiNet.pw) + " ]" );

                    // Up to 10 seconds...
                    int i=0;
                    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); if(i++ > 20) break; }

                    // CONNECT SUCCEX !
                    if(WiFi.status() == WL_CONNECTED)
                    {
                        WIFI_CONNECTOR::isConnected = true;
                        WIFI_CONNECTOR::NOW_IP = WiFi.localIP();

                        Serial.println(" SUCCEX!!");
                        Serial.print("Now Wifi Connected with IP Address : ");
                        Serial.println(WIFI_CONNECTOR::NOW_IP);
                        Serial.println();

                        break;
                    }
                    // CONNECT DENIED..
                    else
                    {
                        Serial.println(" TIMEOUT..");
                        Serial.print("Now Wifi Connected with IP Address : ");
                        Serial.println(WIFI_CONNECTOR::NOW_IP);
                        Serial.println();
                    }
                }
            }
        }
    }


    // ABOUT AP MODE
    {
        if(WIFI_CONNECTOR::isConnected)
        {
            // SET WIFI
            WiFi.mode(WIFI_AP_STA);
            WiFi.softAPConfig( IPAddress(192, 168, 69, 1), IPAddress(192, 168, 69, 1), IPAddress(255, 255, 255, 0) );
            WiFi.softAPDhcpServer().setDns( WiFi.dnsIP(0) );
            WiFi.softAP( F("I AM WIFI") );

            // Start LLMNR responder
            LLMNR.begin("accomplice");
        }
        else
        {
            IPAddress APIP(192, 0, 0, 1); // It doubles as GATEWAY
            byte DNS_PORT = 53;

            // SET WIFI
            WiFi.mode(WIFI_AP);
            WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
            WiFi.softAP( F("I AM NOT WIFI") );

            // DNS spoofing (Only for HTTP)
            // if DNSServer is started with "*" for domain name, it will reply with
            // provided IP to all DNS request
            dnsServer.start(DNS_PORT, "*", APIP);
        }

        // WiFi INFO will not persist after reboot
        WiFi.persistent(false);

        // NAPT allows multiple devices in a private network to share a single public IP
        #define NAPT 1000
        #define NAPT_PORT 10
        err_t ret = ip_napt_init(NAPT, NAPT_PORT);
        if (ret == ERR_OK) { ret = ip_napt_enable_no(SOFTAP_IF, 1); }
    }













    #define HTTP_CODE 200

    // WebServer requests
    // webServer.on("/post",[]() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
    // webServer.on("/ssid",[]() { webServer.send(HTTP_CODE, "text/html", ssid()); });
    // webServer.on("/postSSID",[]() { webServer.send(HTTP_CODE, "text/html", postedSSID()); });
    // webServer.on("/pass",[]() { webServer.send(HTTP_CODE, "text/html", pass()); });
    // webServer.on("/clear",[]() { webServer.send(HTTP_CODE, "text/html", clear()); });

    webServer.on        (   "/test",
                            []
                            {
                                String html = String(rawhtml_INDEX);
                                webServer.send(HTTP_CODE, "text/html", html);
                            }
                        );
    webServer.on        (   "/connectToAP",
                            []
                            {
                                String html = String(rawhtml_INDEX);


                                // INIT args from client
                                String apName;
                                String apPw;
                                for (uint8_t i=0; i<webServer.args(); i++)
                                {
                                    if(webServer.argName(i).equals("apName"))
                                        apName = webServer.arg(i);
                                    if(webServer.argName(i).equals("apPw"))
                                        apPw = webServer.arg(i);
                                }


                                // CONNECT TO NET !
                                WiFi.begin(apName, apPw);
                                Serial.println( "\nTry to connect to. [ " + apName + " / " + apPw + " ]" );
                                
                                // Up to 10 seconds...
                                int i=0;
                                while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(500); if(i++ > 20) break; }


                                // CONNECT SUCCEX !
                                if(WiFi.status() == WL_CONNECTED)
                                {
                                    WIFI_CONNECTOR::isConnected = true;
                                    WIFI_CONNECTOR::NOW_IP = WiFi.localIP();
                                    WIFI_CONNECTOR::NOW_WIFI_ID = apName;
                                    WIFI_CONNECTOR::NOW_WIFI_PW = apPw;

                                    html.replace("<h1>This is HACK</h1>", String("<h1>Connected SUCCEX!!<br>Now IP : " + WIFI_CONNECTOR::NOW_IP.toString() + " </h1>"));
                                    webServer.send(HTTP_CODE, "text/html", html);

                                    // Serial.println(" SUCCEX!!");
                                    // Serial.print("Now Wifi Connected with IP Address : ");
                                    // Serial.println(WIFI_CONNECTOR::NOW_IP);
                                    // Serial.println();

                                    // SWITCH MODULE_STATUS
                                    NOW_MODULE_STATUS = STATUS_WIFI_PROVIDED;
                                }
                                // CONNECT DENIED..
                                else
                                {
                                    html.replace("<h1>This is HACK</h1>", "<h1>NOOOOOOOOO</h1>");
                                    webServer.send(HTTP_CODE, "text/html", html);
                                    
                                    // Serial.println(" TIMEOUT..");
                                    // Serial.print("Now Wifi Connected with IP Address : ");
                                    // Serial.println(WIFI_CONNECTOR::NOW_IP);
                                    // Serial.println();
                                }
                            }
                        );
    webServer.onNotFound(   []
                            {
                                String html = String(rawhtml_SELECT_WIFI);

                                // MAKE strWiFiList
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

                                // INSERT encoded strWiFiList
                                html.replace("#{strWiFiList}", strWiFiList);

                                webServer.send(HTTP_CODE, "text/html", html);
                            }
                        );
    webServer.begin();

    Serial.println("\n");
    Serial.println("SERVER STARTED!!");
    Serial.println("TimerInterrupt START!!");

    // ALL READY! LED BLLLLLINK!!
    TimerInterrupt.attachInterruptInterval  (   1000,
                                                [] 
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

    switch(NOW_MODULE_STATUS)
    {
        case STATUS_COMMON:
        {
            if(!WIFI_CONNECTOR::isConnected)
                dnsServer.processNextRequest();

            webServer.handleClient();
        }
        break;
        case STATUS_WIFI_PROVIDED:
        {
            delay(8282); dnsServer.stop();

            // MODIFY WIFI
            WiFi.mode(WIFI_AP_STA);
            WiFi.softAPConfig( IPAddress(192, 168, 69, 1), IPAddress(192, 168, 69, 1), IPAddress(255, 255, 255, 0) );
            WiFi.softAPDhcpServer().setDns( WiFi.dnsIP(0) );
            WiFi.softAP( F("I AM WIFI") );

            // Start LLMNR responder
            LLMNR.begin("accomplice");

            WIFI_CONNECTOR::saveWifiList();

            NOW_MODULE_STATUS = STATUS_COMMON;
        }
    }
}

#endif