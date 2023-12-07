#define RESET_EEPROM_PIN        2
#define RX_TO_HIJACKER_PIN      12  // D6
#define TX_TO_HIJACKER_PIN      13  // D7
#define BIDIRECTIONAL_PIN       15  // D8




//     // Deciding whether to wake up
//     unsigned long msDeciding = millis();

//     if(BIDIRECTIONAL_PIN_readForXXms(3333) == false)
//     {
//         Serial.println("\nAtashi! Neru!\n");
//         ESP.deepSleep(0);
//         return;
//     }





#include <SoftwareSerial.h>
#include <CRC32.h>



namespace DarkJunction
{
    //              S3r14l (PIN_RX_TO_ESP,      PIN_TX_TO_ESP);         // 21, 20
    SoftwareSerial  S3r14l (RX_TO_HIJACKER_PIN, TX_TO_HIJACKER_PIN);    // 12, 13



    /***(( About, Digital Communication ))***/
    bool BIDIRECTIONAL_PIN_readForXXms(uint16_t msSearch)
    {
        pinMode(BIDIRECTIONAL_PIN,INPUT);

        int8_t msValidate = msSearch>msValidate ? 10 : msSearch*1;
        int8_t isValid = 0-msValidate;

        for(uint8_t i=0; i<msSearch; i++)
        {   delay(1); if((isValid = ((digitalRead(BIDIRECTIONAL_PIN)==HIGH) ? isValid+1 : 0-msValidate)) > 0) return true;   }

        return false;
    }
    void BIDIRECTIONAL_PIN_writeHIGHForXXms(uint16_t msWrite)
    {
        pinMode(BIDIRECTIONAL_PIN,OUTPUT);

        digitalWrite(BIDIRECTIONAL_PIN,HIGH); delay(msWrite);
        digitalWrite(BIDIRECTIONAL_PIN,LOW);
    }
    #define BIDIRECTIONAL_PIN_MODE_WRITE    { pinMode(BIDIRECTIONAL_PIN,OUTPUT); digitalWrite(BIDIRECTIONAL_PIN,LOW); }
    #define BIDIRECTIONAL_PIN_MODE_READ     { pinMode(BIDIRECTIONAL_PIN,INPUT); }



    /***(( About, RX / TX ))***/
    String strMessages;
    void clearMessages() { strMessages=""; }
    String getMessages() { return strMessages; }

    bool download(int countLeftRetry = 5)
    {
        if(countLeftRetry == 0)
            return false;


        BIDIRECTIONAL_PIN_MODE_WRITE;


        while(S3r14l.available() == 0){ delay(1); }
        String strMerged = S3r14l.readStringUntil('\0');


        // if invalidate, redownload
        int indexSeparator;
        if((indexSeparator=strMerged.indexOf('|')) < 0)
        {
            BIDIRECTIONAL_PIN_writeHIGHForXXms(5);
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
            BIDIRECTIONAL_PIN_writeHIGHForXXms(5);
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


        BIDIRECTIONAL_PIN_MODE_READ;


        uint32_t crcCalculated = CRC32::calculate(strSend.c_str(),strSend.length());
        S3r14l.println( strSend + "|" + String(crcCalculated) );
        bool hasChecksumError = BIDIRECTIONAL_PIN_readForXXms(10);


        Serial.println("dataToSend Data : " + strSend);
        Serial.println("Calculated CRC  : " + String(crcCalculated));


        if(hasChecksumError)
            upload(strSend);


        return true;
    }
}



void setup()
{
    // Serial begin
    Serial.begin(115200); while(!Serial){}
    delay(999); Serial.println("\nSERIAL IS ONLINE\n");

    // SerialToHIJACKER begin
    DarkJunction::S3r14l.begin(115200);
}

void loop()
{
    DarkJunction::clearMessages();

    Serial.println("\n-----------------------------------");

    DarkJunction::download();

    Serial.println("-----------------------------------");
    Serial.println("result = " + DarkJunction::getMessages());
    Serial.println("-----------------------------------\n");


    delay(3);
}
