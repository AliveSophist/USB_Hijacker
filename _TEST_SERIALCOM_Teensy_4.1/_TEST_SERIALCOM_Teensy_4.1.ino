
/**⦓   For, HARDWARE   ⦔**/

// Teensy® 4.1's PIN MAP !
//      PIN_                00      //RX1
//      PIN_                01      //TX1
//      PIN_                02
//      PIN_                03
//      PIN_                04
//      PIN_                05
//      PIN_                06
//      PIN_                07      //RX2
//      PIN_                08      //TX2
//      PIN_                09
//      PIN_                10
//      PIN_                11
//      PIN_                12

#define PIN_LED_BUILTIN     13
//      PIN_                A0      //TX3
//      PIN_                A1      //RX3
//      PIN_                A2      //RX4
//      PIN_                A3      //TX4
//      PIN_                A4
//      PIN_                A5
#define PIN_TX_TO_ESP       A6      //TX5
#define PIN_RX_TO_ESP       A7      //RX5
#define WAKEUP_ESP_PIN      A8
#define BIDIRECTIONAL_PIN   A9

//      PIN_                A10     //TX6
//      PIN_                A11     //RX6
//      PIN_                A12
//      PIN_                A13
//      PIN_                28      //RX7
//      PIN_                29      //TX7
//      PIN_                30
//      PIN_                31
//      PIN_                32

//      PIN_                33
//      PIN_                34      //RX8
//      PIN_                35      //TX8
//      PIN_                36
//      PIN_                37
//      PIN_                A14
//      PIN_                A15
#define PIN_RANDOMSEED      A16
#define PIN_BUZZER          A17



#include <SoftwareSerial.h>
#include <CRC32.h>



// void setup()
// {
//     pinMode(WAKEUP_ESP_PIN, OUTPUT);
//     pinMode(BIDIRECTIONAL_RW_PIN, OUTPUT);


//     // Serial begin
//     Serial.begin(115200); while(!Serial){}
//     delay(999); Serial.println("\nSERIAL IS ONLINE\n");

//     // SerialToESP begin
//     S3r14l.begin(115200);
// }
// void loop()
// {
//     // 데이터 송신
//     S3r14l.println("Hello, Receiver!");

//     // 잠시 대기
//     delay(500);
// }



namespace DarkJunction
{
    SoftwareSerial  S3r14l (PIN_RX_TO_ESP,      PIN_TX_TO_ESP);         // 21, 20
    //              S3r14l (RX_TO_HIJACKER_PIN, TX_TO_HIJACKER_PIN);    // 12, 13



    /***(( About, Digital Communication ))***/
    void WAKEUP_ESP_PIN_execute()
    {
        pinMode(WAKEUP_ESP_PIN,OUTPUT);

        digitalWrite(WAKEUP_ESP_PIN,HIGH); delay(100);
        digitalWrite(WAKEUP_ESP_PIN,LOW);
    }
    bool BIDIRECTIONAL_PIN_readForXXms(uint16_t msSearch)
    {
        pinMode(BIDIRECTIONAL_PIN,INPUT);

        int8_t isValid = -10;
        for(uint8_t i=0; i<msSearch; i++)
        {   delay(1); if((isValid = ((digitalRead(BIDIRECTIONAL_PIN)==HIGH) ? isValid+1 : -10)) > 0) return true;   }

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



    /***(( About, RX / TX Communication ))***/
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





#include <CRC32.h>

void setup()
{
    pinMode(WAKEUP_ESP_PIN, OUTPUT);


    // Serial begin
    Serial.begin(115200); while(!Serial){}
    delay(999); Serial.println("\nSERIAL IS ONLINE\n");

    // SerialToESP begin
    DarkJunction::S3r14l.begin(115200);
}

void loop()
{
    Serial.println("\n-----------------------------------");

    DarkJunction::upload("Hello, CRC32!");

    Serial.println("-----------------------------------\n");






    delay(1000);
}
