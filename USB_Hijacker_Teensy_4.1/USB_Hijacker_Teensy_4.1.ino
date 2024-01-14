/*  This Project utilizes Teensy® 4.1 Development Board
 *  Teensy® 4.1 Official Site : https://www.pjrc.com/store/teensy41.html
 *  Teensyduino  Installation : https://www.pjrc.com/teensy/td_download.html
 *  
 *  This Project is recommended to
 *  select [USB Type: "Keyboard"] from Tools -> USB Type: menu.
 *  
 *  but if you also want to use Serial communication for debugging,
 *  select [USB Type: "Serial + Keyboard + Mouse + Joystick"]
 *  
 *  in case YOU MUST UNCONDITIONALLY 'OPEN Serial Monitor BEFORE UPLOAD' PLEASE!!
 *  
 *  GOT IT? Let's H.I.J.A.C.K.  (*・_・)ノ*`*`*/





/**⦓   For, HARDWARE   ⦔**/

// Teensy® 4.1's PIN MAP !

//      ____GND____
//      PIN_                00
//      PIN_                01
//      PIN_                02
//      PIN_                03
//      PIN_                04
//      PIN_                05
//      PIN_                06
//      PIN_                07
//      PIN_                08
//      PIN_                09
//      PIN_                10
//      PIN_                11
//      PIN_                12
//      ____3.3____
//      PIN_                A10
//      PIN_                A11
//      PIN_                A12
//      PIN_                A13
//      PIN_                28
//      PIN_                29
//      PIN_                30
//      PIN_                31
//      PIN_                32

//      ____Vin____ (3.6 to 5.5)
//      ____GND____
//      ____3.3____ (250mA max)
#define PIN_BIDIRECTIONAL   A9 // 23
#define PIN_WAKEUP_ESP      A8 // 22
#define PIN_RX_TO_ESP       A7 // 21
#define PIN_TX_TO_ESP       A6 // 20
//      PIN_                A5
//      PIN_                A4
//      PIN_                A3
//      PIN_                A2
//      PIN_                A1
//      PIN_                A0
#define PIN_LED_BUILTIN     13
//      ____GND____
#define PIN_BUZZER          A17 // 41
#define PIN_RANDOMSEED      A16 // 40
//      PIN_                A15
//      PIN_                A14
//      PIN_                37
//      PIN_                36
//      PIN_                35
//      PIN_                34
//      PIN_                33

#ifdef __cplusplus
extern "C"
{
#endif
    struct usb_string_descriptor_struct
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t wString[100];
    };
    usb_string_descriptor_struct usb_string_manufacturer_name = {
    /*  
     *  2 + MANUFACTURER_NAME_LEN * 2,
     *  3,
     *  { MANUFACTURER_NAME }
     */
        2 + 7 * 2,
        3,
        {'$','0','?','H','!','$','+'}
    };
    usb_string_descriptor_struct usb_string_product_name = {
    /*
     *  2 + PRODUCT_NAME_LEN * 2,
     *  3,
     *  { PRODUCT_NAME }
     */
        #if defined(USB_KEYBOARDONLY)   // [USB Type: "Keyboard"]
        2 + 19 * 2,
        3,
        {'I','\'','m',' ','n','o','t',' ','H','i','j','a','c','k','e','r',' ','X','D'}
        #elif defined(USB_SERIAL_HID)   // [USB Type: "Serial + Keyboard + Mouse + Joystick"]
        2 + 28 * 2,
        3,
        {'I','\'','m',' ','n','o','t',' ','H','i','j','a','c','k','e','r',' ','S','E','R','I','A','L',' ','M','O','D','E'}
        #endif
    };
    usb_string_descriptor_struct usb_string_serial_number = {
    /*
     *  2 + SERIAL_NUMBER_LEN * 2,
     *  3,
     *  { SERIAL_NUMBER }
     */
        2 + 0 * 2,
        3,
        {}
    };
    /*
     *  CUSTOMIZE YOUR USB DESCRIPTION !!
     *
     *  And you can also edit VID, PID, etc. from usb_desc.h
     *  Arduino 2.0.x below : YourArduinoFolder\hardware\teensy\avr\cores\teensy4\usb_desc.h
     *  Arduino 2.0.x above : \Appdata\Local\Arduino15\packages\teensy\hardware\avr\1.57.2\cores\teensy4\usb_desc.h
     *
     *  usb_desc.h file's
     *  > #elif defined(USB_KEYBOARDONLY)    ◀ [USB Type: "Keyboard"]
     *  > #elif defined(USB_SERIAL_HID)      ◀ [USB Type: "Serial + Keyboard + Mouse + Joystick"]
     */
#ifdef __cplusplus
}
#endif

bool isReservedReboot = false;
void REBOOT() { SCB_AIRCR = 0x05FA0004; asm volatile ("dsb"); while(true){} }





#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include <USBHost_t36.h>

#include <SD.h>
File textfile;

#include <SoftwareSerial.h>
#include <CRC32.h>

#include "res_Buzzzzer.h"

#include "res_layouts.h"
#include "res_utility.h"
#include "res_pitches.h"

// Programmer can determine whether to use Serial&SD or not.
// false : if you DO NOT WANT TO USE IT any circumstances
bool isSerial = true, isExistSD = true;

IntervalTimer IntervalTimer_per1ms;





/**⦓ For, SNATCH FROM SLAVE KEYBOARD ⦔**/

volatile uint8_t rawKeycode;

volatile uint8_t numDN = 0;

volatile bool isExistWaitingEvent_Press   = false;
volatile bool isExistWaitingEvent_Release = false;

USBHost USBHostOnTeensy;
USBHub Hub_0(USBHostOnTeensy), Hub_1(USBHostOnTeensy), Hub_2(USBHostOnTeensy), Hub_3(USBHostOnTeensy), Hub_4(USBHostOnTeensy), Hub_5(USBHostOnTeensy), Hub_6(USBHostOnTeensy), Hub_7(USBHostOnTeensy);

class KeyboardParser : public KeyboardController
{
/*********************************** SINGLETON ***********************************/
private:
    // SET private Constructor & Destructor
    KeyboardParser(USBHost usbhost) : KeyboardController(usbhost) {}
    ~KeyboardParser() {}
    // Delete copy constructor to prevent copying
    KeyboardParser(const KeyboardParser& ref) = delete;
    // Delete copy assignment operator to prevent copying
    KeyboardParser& operator=(const KeyboardParser& ref) = delete;

public:
    // STATIC SINGLETON INSTANCE !!
    static KeyboardParser& getInstance()
    {
        static KeyboardParser instance(USBHostOnTeensy);
        return instance;
    }
/*********************************** SINGLETON ***********************************/

public:
    #define KEYLOGGER_LEN_MAX 10
    struct DoublyLinkedStackKeyLogger
    {
        struct nodeKeycode
        {
            uint8_t keycode;
            uint32_t millis;

            nodeKeycode* next;
            nodeKeycode* prev;
        } *top, *bottom;
        uint32_t len;

        DoublyLinkedStackKeyLogger() // Initialize
        {
            top = bottom = NULL;
            len = 0;
        }

        bool size()         { return len; }
        bool isEmpty()      { return (len==0); }
        bool isOverflow()   { return (len > KEYLOGGER_LEN_MAX); }

        void push(uint8_t keycode)
        {
            nodeKeycode* newNode = (nodeKeycode*)malloc(sizeof(nodeKeycode));
            newNode->keycode = keycode;
            newNode->millis = millis();
            newNode->prev = NULL;

            if(isEmpty()){
                newNode->next = NULL;
                top = bottom = newNode;
            }
            else{
                newNode->next = top;
                top->prev = newNode;
                top = newNode;
            }
            len++;

            // if overflowed, delete oldest node
            if(isOverflow()){
                nodeKeycode* delNode = bottom;
                bottom = bottom->prev;
                bottom->next = NULL;
                free(delNode);
                len--;
            }
        }
        void pop()
        {
            nodeKeycode* delNode = top;
            top = top->next;
            top->prev = NULL;
            free(delNode);
            len--;
        }
        uint8_t peek_keycode(uint32_t num)
        {
            nodeKeycode* readNode = top;
            for(uint32_t i=0; i<num; i++)
                readNode = readNode->next;

            return (readNode->keycode);
        }
        int32_t peek_key(uint32_t num)
        {
            nodeKeycode* readNode = top;
            for(uint32_t i=0; i<num; i++)
                readNode = readNode->next;

            return keycode_To_TeensyLayout(readNode->keycode);
        }
        uint32_t peek_millis(uint32_t num)
        {
            nodeKeycode* readNode = top;
            for(uint32_t i=0; i<num; i++)
                readNode = readNode->next;

            return (readNode->millis);
        }

        void PrintAll() // For, Debugging
        {
            uint32_t num=0;
            Serial.println();
            while(num<len)
            {
                nodeKeycode* readNode = top;
                
                for(uint32_t i=0; i<num; i++)
                    readNode = readNode->next;
                    
                Serial.print("node "); Serial.println(num);
                Serial.print("keycode : "); Serial.println((int)readNode->keycode);
                Serial.print("millis  : "); Serial.println((int)readNode->millis);
                Serial.print("*prev : ");
                (readNode->prev==NULL) ? Serial.println("NULL") : Serial.println((int)readNode->prev);
                Serial.print(" this : "); Serial.println((int)readNode);
                Serial.print("*next : ");
                (readNode->next==NULL) ? Serial.println("NULL") : Serial.println((int)readNode->next);
                Serial.println();
                
                num++;
            }
        }
    } KeyLogger;

    void begin(auto OnRawPress, auto OnRawRelease) // Initialize
    {
        this->attachRawPress(OnRawPress);
        this->attachRawRelease(OnRawRelease);
    }
}
&KBD_PARSER = KeyboardParser::getInstance(); // refers to the SINGLETON





/**⦓ For, TRANSMIT TO HOST(OS) ⦔**/

volatile int32_t key;
volatile bool event;
volatile bool isActivatedKeyEvent;

volatile uint32_t msLatestEventCame = 0, msLatestEventPressed = 0;
#define MILLIS_SINCE_LATEST_EVENT           millis()-msLatestEventCame
#define MILLIS_FROM_PRESSED_UNTIL_RELEASE   millis()-msLatestEventPressed

class KeyboardHijacker
{
/*********************************** SINGLETON ***********************************/
private:
    // SET private Constructor & Destructor
    KeyboardHijacker() {}
    ~KeyboardHijacker() {}
    // Delete copy constructor to prevent copying
    KeyboardHijacker(const KeyboardHijacker& ref) = delete;
    // Delete copy assignment operator to prevent copying
    KeyboardHijacker& operator=(const KeyboardHijacker& ref) = delete;

public:
    // STATIC SINGLETON INSTANCE !!
    static KeyboardHijacker& getInstance()
    {
        static KeyboardHijacker instance;
        return instance;
    }
/*********************************** SINGLETON ***********************************/

private:
    bool stateCapsLockToggle    = false;
    bool stateScrollLockToggle  = false;
    bool stateNumLockToggle     = false;

    bool stateLogical[255]      = {false};
    uint8_t numBeingHoldDownKey = 0;

    uint32_t msBasedDelay       = 30;
    uint32_t msExtraDelayMax    = +50;

public:
    bool isExistHoldingDownKey                  (void) { return (numBeingHoldDownKey>0); }
    void setLogicalState                        (int32_t key, bool state) { stateLogical[TeensyLayout_To_keycode(key)] = state; }
    bool getLogicalState                        (int32_t key) { return stateLogical[TeensyLayout_To_keycode(key)]; }
    void printKeyInfo                           (uint8_t keycode);

    void switchStateCapsLockToggle()            { stateCapsLockToggle=!stateCapsLockToggle; }
    void switchStateScrollLockToggle()          { stateScrollLockToggle=!stateScrollLockToggle; }
    void switchStateNumLockToggle()             { stateNumLockToggle=!stateNumLockToggle; }
    bool getStateCapsLockToggle()               { return stateCapsLockToggle; }
    bool getStateScrollLockToggle()             { return stateScrollLockToggle; }
    bool getStateNumLockToggle()                { return stateNumLockToggle; }

    void syncToggleKeyStates                    (void);
    bool isReservedSyncToggleKeyStates          = false;
    void reserveSyncToggleKeyStates             (void) { isReservedSyncToggleKeyStates = true; }

    void releaseAllBeingHoldDownKey             (void);
    bool isReservedReleaseAllBeingHoldDownKey   = false;
    void reserveReleaseAllBeingHoldDownKey      (void) { isReservedReleaseAllBeingHoldDownKey = true; }

    void pressandreleaseKey                     (int32_t key);
    void pressandreleaseKeys                    (String str);
    void pressandreleaseKeys                    (std::initializer_list<int32_t> keys);
    void pressandreleaseKeys                    (int32_t* keys, int32_t len);
    void pressandreleaseMultiKey                (std::initializer_list<int32_t> keys); // ex) ctrl+c, gui+r, ctrl+shift+esc
    void pressandreleaseMultiKey                (int32_t* keys, int32_t len); // ex) ctrl+c, gui+r, ctrl+shift+esc

    void randomDelayChanger                     (int32_t based, int32_t extraMax) { msBasedDelay = based; msExtraDelayMax = extraMax; }
    void randomDelayGenerator                   (void) { delay(msBasedDelay); if(msExtraDelayMax > 0) delay(random(msExtraDelayMax+1)); }
    void randomDelayGenerator_Manually          (int32_t based, int32_t extraMax) { delay(based); if(extraMax > 0) delay(random(extraMax+1)); }
    void pressandreleaseKey_LikeHuman           (int32_t key);
    void pressandreleaseKeys_LikeHuman          (String str);
    void pressandreleaseKeys_LikeHuman          (std::initializer_list<int32_t> keys);

    // THE C.O.R.E. of HIJACKER
    void TRANSMIT_AFTER_HIJACK                  (void);

    // MODULE_KORPAD.ino
    void MODULE_KOREAN_KEYPAD_EVOLUTION         (void);

    // MODULE_KEYMAPPER.ino
    void MODULE_KEYMAPPER_INITIALIZE            (void);
    void MODULE_KEYMAPPER_HIJACK                (void);
    void MODULE_KEYMAPPER_RAPIDFIRE             (void);

    // MODULE_MACRO.ino
    void MODULE_MACRO_START_PLAYER_OR_RECORDER  (const char* fname);
    void MODULE_MACRO_BLOCK_SEVERAL_EVENTS_WHEN_READONLY_STARTED (volatile bool *flag);
    void MODULE_MACRO_PROCEED_RECORDER          (uint8_t keycode, uint32_t delayed, bool isPressed);
    void MODULE_MACRO_END_RECORDER              (const char* filename);
    void MODULE_MACRO_PROCEED_PLAYER            (void);
    void MODULE_MACRO_END_PLAYER                (void);
    void MODULE_MACRO_CHECK_FOR_SHUTDOWN_PLAYER (void);
    void MODULE_MACRO_MANUAL_EVENT_DETECTED     (void);

    void begin()
    {
        if(isExistSD)
            MODULE_KEYMAPPER_INITIALIZE();

        reserveSyncToggleKeyStates();
    }
}
&KBD_HIJACKER = KeyboardHijacker::getInstance(); // refers to the SINGLETON





/**⦓ For, UPLOAD AND DOWNLOAD FILES ⦔**/

namespace DarkJunction
{
    SoftwareSerial  S3r14l (PIN_RX_TO_ESP,      PIN_TX_TO_ESP);         // 21, 20
    //              S3r14l (RX_TO_HIJACKER_PIN, TX_TO_HIJACKER_PIN);    // 12, 13

    #define RESET_ACCOMPLICE()      digitalWrite(PIN_WAKEUP_ESP,HIGH); delay(100); digitalWrite(PIN_WAKEUP_ESP,LOW); delay(100);


    /***(( About, Digital Communication ))***/
    bool PIN_BIDIRECTIONAL_readForXXms(uint16_t msSearch)
    {
        pinMode(PIN_BIDIRECTIONAL,INPUT);

        int8_t msValidate = 10;
        msValidate = msSearch>msValidate ? msValidate : msSearch*1;

        int8_t isValid = 0-msValidate;
        for(uint16_t i=0; i<msSearch; i++)
        {   delay(1); if((isValid=(digitalRead(PIN_BIDIRECTIONAL)==HIGH ? isValid+1 : 0-msValidate)) > 0) return true;   }

        return false;
    }
    void PIN_BIDIRECTIONAL_writeHIGHForXXms(uint16_t msWrite)
    {
        pinMode(PIN_BIDIRECTIONAL,OUTPUT);

        digitalWrite(PIN_BIDIRECTIONAL,HIGH); delay(msWrite);
        digitalWrite(PIN_BIDIRECTIONAL,LOW); delay(1);        
    }
    #define PIN_BIDIRECTIONAL_MODE_WRITE    { pinMode(PIN_BIDIRECTIONAL,OUTPUT); digitalWrite(PIN_BIDIRECTIONAL,LOW); }
    #define PIN_BIDIRECTIONAL_MODE_READ     { pinMode(PIN_BIDIRECTIONAL,INPUT); }



    /***(( About, RX / TX ))***/
    String strMessages;
    void clearMessages() { strMessages=""; }
    String getMessages() { return strMessages; }

    bool download(int countLeftRetry = 5)
    {
        if(countLeftRetry == 0)
            return false;


        PIN_BIDIRECTIONAL_MODE_WRITE;


        while(S3r14l.available() == 0){ delay(1); }
        String strMerged = S3r14l.readStringUntil('\0');


        // if invalidate, redownload
        int indexSeparator;
        if((indexSeparator=strMerged.indexOf('|')) < 0)
        {
            PIN_BIDIRECTIONAL_writeHIGHForXXms(5);
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
            PIN_BIDIRECTIONAL_writeHIGHForXXms(5);
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


        PIN_BIDIRECTIONAL_MODE_READ;


        uint32_t crcCalculated = CRC32::calculate(strSend.c_str(),strSend.length());
        S3r14l.println( strSend + "|" + String(crcCalculated) );
        bool hasChecksumError = PIN_BIDIRECTIONAL_readForXXms(10);


        Serial.println("dataToSend Data : " + strSend);
        Serial.println("Calculated CRC  : " + String(crcCalculated));


        if(hasChecksumError)
            upload(strSend);


        return true;
    }






}





void setup()
{
    /* -------------------------------------- Initializing of Teensy® 4.1's Physical Modules -------------------------------------- */
    {
        pinMode(PIN_LED_BUILTIN, OUTPUT);
        pinMode(PIN_WAKEUP_ESP,OUTPUT);
        randomSeed(analogRead(PIN_RANDOMSEED));


        // SERIAL CHECK
        if(isSerial)
        {
            Serial.begin(115200);
            delay(5252);
            
            if(!Serial) { isSerial = false; Serial.end(); }
        }
        if(isSerial) Serial.println(F("SERIAL IS ONLINE\n"));


        // USB HOST on Teensy® 4.1
        USBHostOnTeensy.begin();
        if(isSerial) Serial.println(F("USB HOST IS ONLINE\n"));


        // SD CARD
        if(isExistSD)
        {
            if (SD.begin(BUILTIN_SDCARD))
            { isExistSD = true; if(isSerial) Serial.println(F("SD CARD IS AVAILABLE :)\n")); }
            else
            { isExistSD = false; if(isSerial) Serial.println(F("SD CARD IS NOT AVAILABLE :(\n")); }
        }


        // ESP8266 a.k.a. ACCOMPLICE
        RESET_ACCOMPLICE();


        // PIEZO BUZZER
        Buzzzzer::reserveBuzz   ( { NOTE_B6,0,  NOTE_E7,0   }
                                , { 111,11,     444,111     } );
        if(isSerial) Serial.println(F("BUZZER PLAYS STARTUP SOUND ♬\n"));

        if(isSerial) Serial.println(F("ALL PHYSICAL MODULES ARE INITIALIZED !!\n\n\n"));
    }
    /* -------------------------------------- Initializing of Teensy® 4.1's Physical Modules -------------------------------------- */



    /* -------------------------------------- Initializing of Objects for Keyboard Hijacking -------------------------------------- */
    {
        // ATTACH Lamda Functions that calls when PRESS / RELEASE interrupt occurs
        KBD_PARSER.begin(
                            /*  function OnRawPress  */ [](uint8_t keycode)
                            {
                                isExistWaitingEvent_Press = true;

                                rawKeycode = keycode;
                                numDN++;


                                // LOG physical keycode
                                KBD_PARSER.KeyLogger.push(keycode);


                                // REBOOT COMMAND [ KEYPAD_SLASH + KEYPAD_0 + KEYPAD_8 ] or [ KEYPAD_0 + KEYPAD_SLASH + KEYPAD_8 ]
                                if(keycode==KEYCODE_KEYPAD_8 && numDN==3)
                                {
                                    if( (KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_SLASH && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_0) ||
                                        (KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_0     && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_SLASH) )
                                    {   isReservedReboot=true; isExistWaitingEvent_Press=false; return;   }
                                }


                                // MODULE_MACRO's
                                if(isExistSD)
                                {
                                    // if MACRO is PLAYING
                                    KBD_HIJACKER.MODULE_MACRO_MANUAL_EVENT_DETECTED();

                                    // if MACRO is RECORDING
                                    KBD_HIJACKER.MODULE_MACRO_PROCEED_RECORDER(keycode, MILLIS_SINCE_LATEST_EVENT, true);

                                    // block several events when READONLY MACRO just started
                                    KBD_HIJACKER.MODULE_MACRO_BLOCK_SEVERAL_EVENTS_WHEN_READONLY_STARTED(&isExistWaitingEvent_Press);
                                }


                                // For, Debugging
                                if(isSerial)
                                {
                                    MEASURE_FREE_MEMORY();
                                    
                                    Serial.print(F("\n(Before Hijack) DN ")); KBD_HIJACKER.printKeyInfo(keycode);
                                    Serial.println();
                                }
                            },
                            /* function OnRawRelease */ [](uint8_t keycode)
                            {
                                if(numDN==0) // invalid Event
                                    return;


                                isExistWaitingEvent_Release = true;

                                rawKeycode = keycode;
                                numDN--;


                                // MODULE_MACRO's
                                if(isExistSD)
                                {
                                    // if MACRO is PLAYING
                                    KBD_HIJACKER.MODULE_MACRO_MANUAL_EVENT_DETECTED();

                                    // if MACRO is RECORDING
                                    KBD_HIJACKER.MODULE_MACRO_PROCEED_RECORDER(keycode, MILLIS_SINCE_LATEST_EVENT, false);

                                    // block several events when READONLY MACRO just started
                                    KBD_HIJACKER.MODULE_MACRO_BLOCK_SEVERAL_EVENTS_WHEN_READONLY_STARTED(&isExistWaitingEvent_Release);
                                }


                                // For, Debugging
                                if(isSerial)
                                {
                                    MEASURE_FREE_MEMORY();
                                    
                                    Serial.print(F("\n(Before Hijack) UP ")); KBD_HIJACKER.printKeyInfo(keycode);
                                    Serial.print(F("         Pressed Time : ")); Serial.println(MILLIS_FROM_PRESSED_UNTIL_RELEASE);
                                }
                            }
                        );
        if(isSerial) Serial.println(F("KEYBOARD PARSER's INTERRUPTS HAVE BEEN CONFIGURED\n"));


        // GET READY FOR HIJACKING
        KBD_HIJACKER.begin();
        if(isSerial) Serial.println(F("KEYBOARD HIJACKER IS ON STANDBY\n"));


        // CONFIGURE 1ms INTERVAL TIMER INTERRUPT
        IntervalTimer_per1ms.begin  (
                                        []
                                        {
                                            static bool isDormancy = false;


                                            // per1ms!
                                            {
                                                Buzzzzer::playBuzz();


                                                if(isExistSD)
                                                    KBD_HIJACKER.MODULE_MACRO_PROCEED_PLAYER();


                                                static int8_t countdownSyncTKS = -1;
                                                if(countdownSyncTKS>-1)
                                                {
                                                    countdownSyncTKS--;
                                                    if(countdownSyncTKS==0) KBD_HIJACKER.syncToggleKeyStates();
                                                }
                                                if(KBD_HIJACKER.isReservedSyncToggleKeyStates)
                                                {
                                                    countdownSyncTKS=77;
                                                    KBD_HIJACKER.isReservedSyncToggleKeyStates = false;
                                                }
                                            }


                                            // per10ms!
                                            if( !(millis()%10) )
                                            {
                                                KBD_HIJACKER.MODULE_KEYMAPPER_RAPIDFIRE();
                                            }


                                            // per10000ms!
                                            if( !(millis()%10000) )
                                            {
                                                isDormancy = (MILLIS_SINCE_LATEST_EVENT)>33333 ? true : false;

                                                if(isDormancy) KBD_HIJACKER.syncToggleKeyStates();
                                            }
                                        },
                                        1000
                                    );
        if(isSerial) Serial.println(F("1ms INTERVAL TIMER INTERRUPT HAS BEEN CONFIGURED\n"));


        if(isSerial) Serial.println(F("ALL OBJECTS ARE INITIALIZED !!\n\n\n"));
    }
    /* -------------------------------------- Initializing of Objects for Keyboard Hijacking -------------------------------------- */



    if(isSerial) Serial.println(F("OPERATION COMPLETE, SIR ! XD\n\n\n\n"));
}

void loop()
{
    USBHostOnTeensy.Task();


    if(isReservedReboot)
    {
        KBD_HIJACKER.releaseAllBeingHoldDownKey();
        while(numDN){ delay(1); }
        REBOOT();
    }


    if(isExistWaitingEvent_Press)
    {
        key   = keycode_To_TeensyLayout( rawKeycode );
        event = true;
        msLatestEventCame = msLatestEventPressed = millis();

        KBD_HIJACKER.TRANSMIT_AFTER_HIJACK();
        
        isExistWaitingEvent_Press   = false;
    }


    if(isExistWaitingEvent_Release)
    {
        key   = keycode_To_TeensyLayout( rawKeycode );
        event = false;
        msLatestEventCame = millis();

        KBD_HIJACKER.TRANSMIT_AFTER_HIJACK();
        
        isExistWaitingEvent_Release = false;
    }


    numDN>0 ? digitalWrite(PIN_LED_BUILTIN, HIGH) : digitalWrite(PIN_LED_BUILTIN, LOW);
}
