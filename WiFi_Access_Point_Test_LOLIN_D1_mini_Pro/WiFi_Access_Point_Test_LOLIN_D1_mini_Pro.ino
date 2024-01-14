/* 
    ESP8266 WiFi Captive Portal
    https://github.com/adamff1/ESP8266-Captive-Portal

    Cyclenerd/AccessPoint.ino
    https://gist.github.com/Cyclenerd/7c9cba13360ec1ec9d2ea36e50c7ff77
*/

#include <algorithm>
#include <list>

#include <SoftwareSerial.h>
#include <CRC32.h>

#include <ESP8266TimerInterrupt.h>  // https://github.com/khoih-prog/ESP8266TimerInterrupt
#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <ESP8266LLMNR.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <lwip/napt.h>
#include <lwip/dns.h>

#include "res_html_INDEX.h"
#include "res_html_SELECT_WIFI.h"

#define DISABLE_INTERRUPTS cli()
#define ENABLE_INTERRUPTS sei()





#define RESET_EEPROM_PIN         4

#define PIN_TO_HIJACKER_SERIAL_RX   15  // D8
#define PIN_TO_HIJACKER_SERIAL_TX   13  // D7
#define PIN_TO_HIJACKER_DIGITAL_RX  12  // D6
#define PIN_TO_HIJACKER_DIGITAL_TX  14  // D5





DNSServer dnsServer;
ESP8266WebServer webServer(80);

ESP8266Timer TimerInterrupt;





namespace WIFI_CONNECTOR
{
    bool isConnected = false;
    IPAddress NOW_IP;

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
        DISABLE_INTERRUPTS;
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
        ENABLE_INTERRUPTS;

        /*
        // SERIAL scan test
        {
            int i=0;
            for (WIFI_NET wifi : SAVED_WIFI_NET_LIST)
            {
                Serial.print("Wifi "); Serial.print(i++); Serial.print(" : ");
                Serial.print(wifi.id); Serial.print(" / "); Serial.println(wifi.pw);
            }
        }
        */
    }

    void saveWifiList(String apName, String apPw)
    {
        // If already EXISTS, FUCK OFF.
        bool isAlreadyExist = false;
        for (WIFI_NET wifi : SAVED_WIFI_NET_LIST) {
            if(apName.equals(wifi.id) && apPw.equals(wifi.pw))
                isAlreadyExist = true;
        }
        if(isAlreadyExist)
            return;


        // DEL a most old WIFI_NET
        SAVED_WIFI_NET_LIST.pop_back();
        // ADD a new WIFI_NET
        SAVED_WIFI_NET_LIST.push_front( WIFI_NET(apName, apPw) );


        DISABLE_INTERRUPTS; // DISABLE INTERRUPT while using EEPROM
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
        ENABLE_INTERRUPTS;
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





namespace DarkJunction
{
    //              S3r14l(PIN_TO_ESP_SERIAL_RX,        PIN_TO_ESP_SERIAL_TX);
    SoftwareSerial  S3r14l(PIN_TO_HIJACKER_SERIAL_RX,   PIN_TO_HIJACKER_SERIAL_TX);



    /***(( About, Digital Communication ))***/
    bool readDigitalForXXms(uint16_t msSearch)
    {
        pinMode(PIN_BIDIRECTIONAL,INPUT);

        int8_t msValidate = 10;
        msValidate = msSearch>msValidate ? msValidate : msSearch*1;

        int8_t isValid = 0-msValidate;
        for(uint8_t i=0; i<msSearch; i++)
        {   delay(1); if((isValid=(digitalRead(PIN_BIDIRECTIONAL)==HIGH ? isValid+1 : 0-msValidate)) > 0) return true;   }

        return false;
    }
    void writeHIGHForXXms(uint16_t msWrite)
    {
        pinMode(PIN_BIDIRECTIONAL,OUTPUT);

        digitalWrite(PIN_BIDIRECTIONAL,HIGH); delay(msWrite);
        digitalWrite(PIN_BIDIRECTIONAL,LOW);
    }
    void PIN_BIDIRECTIONAL_writeLOWForXXms(uint16_t msWrite)
    {
        pinMode(PIN_BIDIRECTIONAL,OUTPUT);

        digitalWrite(PIN_BIDIRECTIONAL,LOW); delay(msWrite);
        digitalWrite(PIN_BIDIRECTIONAL,HIGH);
    }



    /***(( About, RX / TX ))***/
    String strMessages;
    void clearMessages() { strMessages=""; }
    String getMessages() { return strMessages; }

    bool download(int countLeftRetry = 5)
    {
        if(countLeftRetry == 0)
            return false;


        while(S3r14l.available() == 0){ delay(1); }
        String strMerged = S3r14l.readStringUntil('\0');


        // if invalidate, redownload
        int indexSeparator;
        if((indexSeparator=strMerged.indexOf('|')) < 0)
        {
            writeHIGHForXXms(5);
            download(countLeftRetry - 1);
        }


        String strReceived = strMerged.substring(0, indexSeparator);

        uint32_t crcReceived = atoll( strMerged.substring(indexSeparator+1).c_str() );
        uint32_t crcCalculated = CRC32::calculate( strReceived.c_str(), strReceived.length() );


                Serial.println("Received Data  : " + strReceived);
                Serial.println("Received   CRC : " + String(crcReceived));
                Serial.println("Calculated CRC : " + String(crcCalculated));


        // if invalidate, redownload
        if(crcReceived != crcCalculated)
        {
            writeHIGHForXXms(5);
            download(countLeftRetry - 1);
        }
        else
        {
            strMessages += strReceived;
        }


        return true;
    }

    bool upload(String strSend, int countLeftRetry = 5)
    {
        if(countLeftRetry == 0)
            return false;


        uint32_t crcCalculated = CRC32::calculate(strSend.c_str(),strSend.length());
        S3r14l.println( strSend + "|" + String(crcCalculated) );
        bool hasChecksumError = readDigitalForXXms(10);


        Serial.println("dataToSend Data : " + strSend);
        Serial.println("Calculated CRC  : " + String(crcCalculated));


        if(hasChecksumError)
            upload(strSend);


        return true;
    }
}





void setup()
{
    // LED
    pinMode(LED_BUILTIN, OUTPUT); digitalWrite(LED_BUILTIN, HIGH);


    // To HIJACKER, wake me up when you need me...
    {
        if(DarkJunction::readDigitalForXXms(2222) == false)
        {   ESP.deepSleep(0); return;   }
        else
        {   digitalWrite(LED_BUILTIN, LOW);   }
    }


    // Serial begin, for Debugging
    {
        Serial.begin(115200); while(!Serial){}
        delay(999); Serial.println("\nSERIAL IS ONLINE\n");
    }


    // EEPROM begin, if RESET_EEPROM_PIN grounded, reset EEPROM
    {
        EEPROM.begin(4096);

        pinMode(RESET_EEPROM_PIN,INPUT_PULLUP);
        for(uint8_t i=0; i<101; i++)
        {
            delay(1);

            if(digitalRead(RESET_EEPROM_PIN)!=LOW) break;
            if(i==100){ Serial.println(F("EEPROM RESET")); for(int i=0;i<4096;i++) EEPROM.write(i,0); EEPROM.commit(); }
        }
    }
    // Try to CONNECT WIFI (from SAVED_WIFI_NET_LIST)
    {
        WIFI_CONNECTOR::loadWifiList();

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
                    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(200); if(i++ > 40) break; }

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


    // Set AP MODE
    {
        // if WIFI is Connected, Activate mDNS(LLMNR)
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
        #define HTTP_CODE 200

        webServer.on        (   "/",
                                []
                                {
                                    String html = String(rawhtml_INDEX);
                                    webServer.send(HTTP_CODE, "text/html", html);
                                }
                            );
        webServer.on        (   "/connectToAP",
                                HTTP_POST,
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
                                    while (WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(200); if(i++ > 40) break; }


                                    // CONNECT SUCCEX !
                                    if(WiFi.status() == WL_CONNECTED)
                                    {
                                        html.replace("<h1>This is HACK</h1>", String("<h1>Connected SUCCEX!!<br>Now IP : " + (WiFi.localIP()).toString() + " </h1>"));
                                        webServer.send(HTTP_CODE, "text/html", html);
                                        WIFI_CONNECTOR::saveWifiList(apName, apPw);

                                        delay(4444); dnsServer.stop();

                                        // RESTART MODULE !
                                        ESP.restart();
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
        webServer.on        (   "/connectToAP",
                                HTTP_GET,
                                []
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
        webServer.onNotFound(   []
                                {
                                    // redirect to "/connectToAP"
                                    webServer.sendHeader("Location", "/connectToAP", true);
                                    webServer.send(302, "text/plain", "");
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
                                                        if( !(millis() % (WIFI_CONNECTOR::isConnected?1500:150)) )
                                                        {
                                                            static bool stateLED = false;
                                                            digitalWrite(LED_BUILTIN, stateLED^=1 ? HIGH : LOW);
                                                        }
                                                    }
                                                );
    }


    // To HIJACKER, im all ready...
    if(WIFI_CONNECTOR::isConnected)
        DarkJunction::writeHIGHForXXms(100);
    // else
    //     DarkJunction::PIN_BIDIRECTIONAL_writeLOWForXXms(100);
    Serial.println(F("ACCOMPLICE ALL READY, SIR!\n\n\n\n"));
}

void loop()
{
    delay(1);

    if(!WIFI_CONNECTOR::isConnected)
        dnsServer.processNextRequest();

    webServer.handleClient();









}
