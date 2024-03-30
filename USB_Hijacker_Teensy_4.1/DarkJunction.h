#pragma once

#include <CRC32.h>

#define PIN_DARKJUNCTION_DIGITAL_RX     A9 // 23
#define PIN_DARKJUNCTION_DIGITAL_TX     A8 // 22
//      PIN_DARKJUNCTION_SERIAL_RX      A7 // 21
//      PIN_DARKJUNCTION_SERIAL_TX      A6 // 20
#define PIN_TO_WIFI_ACCOMPLICE_WAKEUP   A5 // 19

#define DECLARE_DARKJUNCTION_PINS()     pinMode(PIN_TO_WIFI_ACCOMPLICE_WAKEUP,OUTPUT); pinMode(PIN_DARKJUNCTION_DIGITAL_RX,INPUT); pinMode(PIN_DARKJUNCTION_DIGITAL_TX,OUTPUT);
#define WIFI_ACCOMPLICE_PUTTOSLEEP()  { digitalWrite(PIN_TO_WIFI_ACCOMPLICE_WAKEUP,HIGH); delay(20); digitalWrite(PIN_TO_WIFI_ACCOMPLICE_WAKEUP,LOW); delay(1); }
#define WIFI_ACCOMPLICE_WAKEUP()      { digitalWrite(PIN_TO_WIFI_ACCOMPLICE_WAKEUP,HIGH); delay(20); digitalWrite(PIN_TO_WIFI_ACCOMPLICE_WAKEUP,LOW); delay(1); DarkJunction::writeHIGHForXXms(1000); }

// This function was designed with the assumption that the master module can control the slave module's power
// slave module can use SoftwareSerial, but master module should be use HardwareSerial
// The reason is that even if control the slave module's power, the master module's Serial must remain operational...
#define S3R14L Serial5 // RX:21 TX:20

namespace DarkJunction // For, Serial Communication with applied Checksum
{
    /***(( some crackling sound echoes... pzzzz... ))***/
    #define DarkJunction_STATE_DISCONNECTED -1
    #define DarkJunction_STATE_STANDBY      1
    #define DarkJunction_STATE_ISREQUESTED  2
    #define DarkJunction_STATE_PROCESSING   3
    int8_t STATE = DarkJunction_STATE_DISCONNECTED;
    uint32_t msLatestFunctioned = 0;

    void BOOT       (void);
    void SHUTDOWN   (void);

    bool isEnableRequestHandler = false;
    void START_REQUEST_DETECTING(void);
    void START_REQUEST_BLOCKING (void);

    #define DarkJunction_REQUEST_ERRORED_NotConfigured  -1
    #define DarkJunction_REQUEST_ERRORED_UnknownRequest -2
    #define DarkJunction_REQUEST_ERRORED_NotArrived     -3
    #define DarkJunction_REQUEST_ERRORED_FailedPolling  -44
    #define DarkJunction_REQUEST_ERRORED_FailedDownload -45
    #define DarkJunction_REQUEST_ERRORED_FailedUpload   -46
    #define DarkJunction_REQUEST_ERRORED_NotExistSD     -80
    #define DarkJunction_REQUEST_ERRORED_NotExistFile   -81
    #define DarkJunction_REQUEST_SUCCESS                1

    using RequestHandler            = int8_t (*)(String);
    RequestHandler PROCESS_REQUEST  = [](String strRequest) -> int8_t { return DarkJunction_REQUEST_ERRORED_NotConfigured; }; // default : dummy function
    void CONFIGURE_REQUEST_HANDLER  (RequestHandler funcProcessRequest);

    void HANDLE_REQUEST     (void);



    /***(( About, Digital RX / TX ))***/
    void writeHIGHForXXms   (uint16_t msWrite){ digitalWrite(PIN_DARKJUNCTION_DIGITAL_TX,HIGH); delay(msWrite); digitalWrite(PIN_DARKJUNCTION_DIGITAL_TX,LOW); delay(1); };
    bool detectHIGHForXXms  (uint16_t msSearch, bool isWaitingForLOW = false);
    // uint16_t measureHIGHForXXms (uint16_t msSearch);



    /***(( About, Serial RX / TX ))***/
    String  strMessage;
    void    appendMessage   (String append) { strMessage += append; }
    String  getMessage      (void)          { return strMessage; }
    void    clearMessage    (void)          { strMessage=""; }
    bool    isLeftInMessage (void)          { return strMessage.length()>0; }

    #define SERIAL_READ_TIMEOUT 9999
    #define CHECKSUM_PATIENCE 7
    #define UPDOWNLOAD_MAX_LENGTH 512

    bool S3R14L_polling     (String* strRead = nullptr);
    bool S3R14L_download    (int8_t countLeftRetry = CHECKSUM_PATIENCE);
    bool S3R14L_upload      (String* strSend, int8_t countLeftRetry = CHECKSUM_PATIENCE);





    void BOOT()
    {
        S3R14L.begin(19200); // 9600 19200 38400 57600 115200
        START_REQUEST_DETECTING();

        STATE = DarkJunction_STATE_STANDBY;
    }
    void SHUTDOWN()
    {
        S3R14L.flush();

        START_REQUEST_BLOCKING();
        S3R14L.end();

        STATE = DarkJunction_STATE_DISCONNECTED;
    }

    void CONFIGURE_REQUEST_HANDLER(RequestHandler funcProcessRequest)
    {
        if (funcProcessRequest == nullptr)
            return;

        isEnableRequestHandler = true;
        PROCESS_REQUEST = funcProcessRequest;
    }



    #if defined(ESP8266) || defined(ESP32)
    void ICACHE_RAM_ATTR interruptAlarmRequest(){ DarkJunction::STATE = DarkJunction_STATE_ISREQUESTED; DarkJunction::msLatestFunctioned = millis(); }
    void START_REQUEST_DETECTING()
    {
        STATE = DarkJunction_STATE_STANDBY;
        msLatestFunctioned = millis();

        // Interrupt works only REQUEST HANDLER On.
        if (isEnableRequestHandler)
            attachInterrupt(digitalPinToInterrupt(PIN_DARKJUNCTION_DIGITAL_RX), interruptAlarmRequest, RISING);
    }
    #else
    void START_REQUEST_DETECTING()
    {
        STATE = DarkJunction_STATE_STANDBY;
        msLatestFunctioned = millis();

        // Interrupt works only REQUEST HANDLER On.
        if (isEnableRequestHandler)
            attachInterrupt(digitalPinToInterrupt(PIN_DARKJUNCTION_DIGITAL_RX), [](){ DarkJunction::STATE = DarkJunction_STATE_ISREQUESTED; DarkJunction::msLatestFunctioned = millis(); }, RISING);
    }
    #endif
    void START_REQUEST_BLOCKING()
    {
        STATE = DarkJunction_STATE_PROCESSING;
        msLatestFunctioned = millis();

        // Interrupt works only REQUEST HANDLER On.
        if (isEnableRequestHandler)
            detachInterrupt(digitalPinToInterrupt(PIN_DARKJUNCTION_DIGITAL_RX));
    }

    void HANDLE_REQUEST()
    {
        if (STATE != DarkJunction_STATE_ISREQUESTED)
            return;


        START_REQUEST_BLOCKING(); // DarkJunction_STATE_PROCESSING, detach Interrupt


        int8_t result;

        if  (!S3R14L_download())
            result = DarkJunction_REQUEST_ERRORED_NotArrived;
        else
            result = PROCESS_REQUEST(getMessage());

        clearMessage();


        if (isDEBUG)
        {
            Serial.print("\n\nREQUEST RESULT : ");
            Serial.println  (   result==DarkJunction_REQUEST_ERRORED_NotConfigured  ?   "DarkJunction_REQUEST_ERRORED_NotConfigured" :
                                result==DarkJunction_REQUEST_ERRORED_UnknownRequest ?   "DarkJunction_REQUEST_ERRORED_UnknownRequest" :
                                result==DarkJunction_REQUEST_ERRORED_NotArrived     ?   "DarkJunction_REQUEST_ERRORED_NotArrived" :
                                result==DarkJunction_REQUEST_ERRORED_FailedUpload   ?   "DarkJunction_REQUEST_ERRORED_FailedUpload" :
                                result==DarkJunction_REQUEST_ERRORED_FailedDownload ?   "DarkJunction_REQUEST_ERRORED_FailedDownload" :
                                result==DarkJunction_REQUEST_ERRORED_FailedPolling  ?   "DarkJunction_REQUEST_ERRORED_FailedPolling" :
                                result==DarkJunction_REQUEST_ERRORED_NotExistSD     ?   "DarkJunction_REQUEST_ERRORED_NotExistSD" :
                                result==DarkJunction_REQUEST_ERRORED_NotExistFile   ?   "DarkJunction_REQUEST_ERRORED_NotExistFile" :
                                                                                        "DarkJunction_REQUEST_SUCCESS"   );
            Serial.println();
        }


        START_REQUEST_DETECTING(); // DarkJunction_STATE_STANDBY, reattach Interrupt
    }



    bool detectHIGHForXXms(uint16_t msSearch, bool isWaitingForLOW)
    {
        uint8_t msValidate = 3;
        uint8_t isValid = 0;

        for(uint16_t i=0; i<msSearch; i++)
        {
            delay(1);
            if ((isValid=(digitalRead(PIN_DARKJUNCTION_DIGITAL_RX)==HIGH ? isValid+1 : 0)) >= msValidate)
            {
                if (!isWaitingForLOW)
                    return true;
                else
                {
                    while(digitalRead(PIN_DARKJUNCTION_DIGITAL_RX)==HIGH)
                    {   delay(1);   }

                    return true;
                }
            }
        }

        return false;
    }
    /* uint16_t measureHIGHForXXms(uint16_t msSearch)
        {
            uint16_t measured=0;

            for(uint16_t i=0; i<msSearch; i++)
            {
                delay(1);
                measured = (digitalRead(PIN_DARKJUNCTION_DIGITAL_RX)==HIGH) ? measured+1 : measured;
            }

            return measured;
        }
    */



    bool S3R14L_polling(String* strRead)
    {
        // now detecting...
        uint32_t millisDetectingStarted = millis();
        while(S3R14L.available() == 0)
        {
            delay(1);

            if (millis()-millisDetectingStarted > SERIAL_READ_TIMEOUT)
            {
                if (isDEBUG) Serial.println("S3R14L_polling TIMEOUT !!");
                return false;
            }
        } //delay(10);


        // get Message
        if (strRead != nullptr)
        {
            *strRead = S3R14L.readStringUntil('\0');

            while(!isContains(*strRead, '\a'))
            {
                delay(3);
                if (S3R14L.available())
                    (*strRead).concat(S3R14L.readStringUntil('\0'));
            }
        }
        else
            S3R14L.readStringUntil('\0');


        // notice polling success
        writeHIGHForXXms(5);


        return true;
    }
    bool S3R14L_download(int8_t countLeftRetry /* = CHECKSUM_PATIENCE */)
    {
        if (isDEBUG)
        {   Serial.println("***************************************\n  DOWNLOAD TRY LEFT : " + String(countLeftRetry));   }


        clearMessage();


        // if failed to polling, Abondon
        String strMerged = "";
        if (!S3R14L_polling(&strMerged))
            return false;

        if (isDEBUG)
        {   Serial.print("  strMerged : ["); Serial.print(strMerged); Serial.println("]\n***************************************");   }


        // if separator does not exist, redownload
        int indexSeparator = strMerged.lastIndexOf('|');
        int indexEndIndicator = strMerged.lastIndexOf('\a');
        if (indexSeparator < 0)
        {
            writeHIGHForXXms(5); // redownload signal

            // redownload
            if (countLeftRetry > 0)
                return S3R14L_download(countLeftRetry-1);

            // several counts are invalid, Abondon
            else
                return false;
        }


        String strReceived = strMerged.substring(0, indexSeparator);
        uint32_t crcReceived = atoll( strMerged.substring(indexSeparator+1, indexEndIndicator).c_str() );
        uint32_t crcCalculated = CRC32::calculate( strReceived.c_str(), strReceived.length() );

        if (isDEBUG)
        {
            Serial.println("Received Data  : " + strReceived);
            Serial.println("Received   CRC : " + String(crcReceived));
            Serial.println("Calculated CRC : " + String(crcCalculated));
        }


        // if checksum does not match, redownload or abondon
        if (crcReceived != crcCalculated)
        {
            writeHIGHForXXms(5); // redownload signal

            // redownload
            if (countLeftRetry > 0)
                return S3R14L_download(countLeftRetry-1);

            // several counts are invalid, Abondon
            else
                return false;
        }


        delay(20); // skip upload's checksum check


        // prevent duplicated checksum checker error
        if (strReceived.lastIndexOf('\a') > -1 && strReceived.lastIndexOf('|') > -1)
            strReceived = strReceived.substring(0, strReceived.lastIndexOf('|'));


        appendMessage(strReceived); // message received!


        return true;
    }
    bool S3R14L_upload(String* strSend, int8_t countLeftRetry /* = CHECKSUM_PATIENCE */)
    {
        if (isDEBUG)
        {   Serial.println("***************************************\n  UPLOAD TRY LEFT : " + String(countLeftRetry));   }


        // prevent duplicated checksum checker error
        if (strSend->lastIndexOf('\a') > -1 && strSend->lastIndexOf('|') > -1)
            *strSend = strSend->substring(0, strSend->lastIndexOf('|'));


        uint32_t crcCalculated = CRC32::calculate(strSend->c_str(),strSend->length());
        strSend->concat('|');
        strSend->concat(String(crcCalculated));
        strSend->concat('\a');
        S3R14L.print(strSend->c_str());

        if (isDEBUG)
        {   Serial.print("  strSend : ["); Serial.print(strSend->c_str()); Serial.println("]\n***************************************");   }


        // is polling success?
        bool isUploadSuccessful = detectHIGHForXXms(SERIAL_READ_TIMEOUT*2, true);
        if (!isUploadSuccessful) // if signal not detected, Abondon
            return false;


        bool hasChecksumError = detectHIGHForXXms(15, true);


        // if checksum does not match, reupload or abondon
        if (hasChecksumError)
        {
            // redownload
            if (countLeftRetry > 0)
                return S3R14L_upload(strSend, countLeftRetry-1);

            // several counts are invalid, Abondon
            else
                return false;
        }


        return true;
    }
}

const char TRANSMISSION_COMPLETED[] PROGMEM = "7R4N5M15510N_C0MPL373D";
const char ERRORED[] PROGMEM = "3RR0R3D";
