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

#define PIN_LED_BUILTIN     13
//      PIN_                A0
//      PIN_                A1
//      PIN_                A2
//      PIN_                A3
//      PIN_                A4
//      PIN_                A5
//      PIN_                A6
//      PIN_                A7
//      PIN_                A8
//      PIN_                A9

//      PIN_                A10
//      PIN_                A11
//      PIN_                A12
//      PIN_                A13
//      PIN_                28
//      PIN_                29
//      PIN_                30
//      PIN_                31
//      PIN_                32

//      PIN_                33
//      PIN_                34
//      PIN_                35
//      PIN_                36
//      PIN_                37
//      PIN_                A14
//      PIN_                A15
#define PIN_RANDOMSEED      A16
#define PIN_BUZZER          A17

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

void REBOOT() { SCB_AIRCR = 0x05FA0004; asm volatile ("dsb"); while(true){} }





#include <map>
#include <list>

#include <USBHost_t36.h>

#include <SD.h>
File textfile;

#include "res_Buzzzzer.h"

#include "res_layouts.h"
#include "res_utility.h"
#include "res_pitches.h"





/**⦓ For, SLAVE KEYBOARD ⦔**/

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

        DoublyLinkedStackKeyLogger() //Initialize
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

            //if overflowed, delete oldest node
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

        void PrintAll() //For, Debugging
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

    void begin(auto OnRawPress, auto OnRawRelease)
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
    void setLogicalState                        (int32_t key, bool state) { stateLogical[TeensyLayout_To_keycode(key)] = state; }
    bool getLogicalState                        (int32_t key) { return stateLogical[TeensyLayout_To_keycode(key)]; }
    void printKeyInfo                           (uint8_t keycode);

    void switchStateCapsLockToggle()            { stateCapsLockToggle=!stateCapsLockToggle; }
    void switchStateScrollLockToggle()          { stateScrollLockToggle=!stateScrollLockToggle; }
    void switchStateNumLockToggle()             { stateNumLockToggle=!stateNumLockToggle; }
    bool getStateCapsLockToggle()               { return stateCapsLockToggle; }
    bool getStateScrollLockToggle()             { return stateScrollLockToggle; }
    bool getStateNumLockToggle()                { return stateNumLockToggle; }
    bool isReservedSyncToggleKeyStates          = false;
    void reserveSyncToggleKeyStates             (void) { isReservedSyncToggleKeyStates = true; }
    void syncToggleKeyStates                    (void);

    bool isExistHoldingDownKey                  (void) { return (numBeingHoldDownKey>0); }
    void releaseAllBeingHoldDownKey             (void);

    void pressandreleaseKey                     (int32_t key);
    void pressandreleaseKeys                    (String str);
    void pressandreleaseKeys                    (std::initializer_list<int32_t> keys);
    void pressandreleaseMultiKey                (std::initializer_list<int32_t> keys); // ex) ctrl+c, gui+r, ctrl+shift+esc
    void pressandreleaseKeys                    (int32_t* keys, int32_t len);
    void pressandreleaseMultiKey                (int32_t* keys, int32_t len); // ex) ctrl+c, gui+r, ctrl+shift+esc

    void randomDelayChanger                     (int32_t based, int32_t extraMax) { msBasedDelay = based; msExtraDelayMax = extraMax; }
    void randomDelayGenerator                   (void) { delay(msBasedDelay); if(msExtraDelayMax > 0) delay(random(msExtraDelayMax+1)); }
    void randomDelayGenerator_Manually          (int32_t based, int32_t extraMax) { delay(based); if(extraMax > 0) delay(random(extraMax+1)); }
    void pressandreleaseKey_LikeHuman           (int32_t key);
    void pressandreleaseKeys_LikeHuman          (String str);
    void pressandreleaseKeys_LikeHuman          (std::initializer_list<int32_t> keys);
    void pressandreleaseMultiKey_LikeHuman      (std::initializer_list<int32_t> keys); // ex) ctrl+c, gui+r, ctrl+shift+esc

    // THE C.O.R.E. of HIJACKER
    void TRANSMIT_AFTER_HIJACK                  (void);

    void MODULE_KOREAN_KEYPAD_EVOLUTION         (void);

    void MODULE_MACRO_START_PLAYER_OR_RECORDER  (const char* fname);
    void MODULE_MACRO_REC_PRESSED               (uint8_t keycode, uint32_t delayed);
    void MODULE_MACRO_REC_RELEASED              (uint8_t keycode, uint32_t delayed);
    void MODULE_MACRO_END_RECORDER              (const char* filename);
    void MODULE_MACRO_PLAY_ONGOING              (void);
    void MODULE_MACRO_END_PLAYER                (void);
    void MODULE_MACRO_BLOCK_EVENTS_WHEN_JUST_STARTED (volatile bool *flag);
    void MODULE_MACRO_CHECK_FOR_SHUTDOWN_PLAYER (void);
    void MODULE_MACRO_MANUAL_EVENT_DETECTED     (void);
    void MODULE_MACRO_PRINT                     (const char* filename);

    void MODULE_KEYMAPPER_INITIALIZE            (void);
    void MODULE_KEYMAPPER_HIJACK                (void);

    void begin()
    {
        MODULE_KEYMAPPER_INITIALIZE();

        reserveSyncToggleKeyStates();
    }
}
&KBD_HIJACKER = KeyboardHijacker::getInstance(); // refers to the SINGLETON





/**⦓   For, COMMON   ⦔**/

bool isSerial = true, isExistSD = true;  // false : if you DO NOT WANT TO USE any circumstances

IntervalTimer IntervalTimer_per1ms;

void setup()
{
    /* -------------------------------------- Initializing of Teensy® 4.1's Physical Modules -------------------------------------- */
    {
        pinMode(PIN_LED_BUILTIN, OUTPUT);
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
                            [](uint8_t keycode)
                            {
                                KBD_PARSER.KeyLogger.push(keycode);

                                rawKeycode = keycode;

                                isExistWaitingEvent_Press = true;
                                numDN++;

                                //MODULE_MACRO
                                {
                                    //if MACRO is PLAYING
                                    KBD_HIJACKER.MODULE_MACRO_MANUAL_EVENT_DETECTED();

                                    //if MACRO is RECORDING
                                    KBD_HIJACKER.MODULE_MACRO_REC_PRESSED(keycode, MILLIS_SINCE_LATEST_EVENT);

                                    //BLOCK several events when MACRO JUST STARTED
                                    KBD_HIJACKER.MODULE_MACRO_BLOCK_EVENTS_WHEN_JUST_STARTED(&isExistWaitingEvent_Press);
                                }

                                if(isSerial)
                                {
                                    MEASURE_FREE_MEMORY();
                                    
                                    Serial.print(F("\n(Before Hijack) DN ")); KBD_HIJACKER.printKeyInfo(keycode);
                                    Serial.println();
                                }
                            },
                            [](uint8_t keycode)
                            {
                                rawKeycode = keycode;

                                isExistWaitingEvent_Release = true;
                                numDN--; //if(numDN) numDN-=1;

                                //MODULE_MACRO
                                {
                                    //if MACRO is PLAYING
                                    KBD_HIJACKER.MODULE_MACRO_MANUAL_EVENT_DETECTED();

                                    //if MACRO is RECORDING
                                    KBD_HIJACKER.MODULE_MACRO_REC_RELEASED(keycode, MILLIS_SINCE_LATEST_EVENT);

                                    //BLOCK several events when MACRO JUST STARTED
                                    KBD_HIJACKER.MODULE_MACRO_BLOCK_EVENTS_WHEN_JUST_STARTED(&isExistWaitingEvent_Release);
                                }

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
                                            
                                            //per1ms!
                                            {
                                                Buzzzzer::playBuzz();
                                                
                                                KBD_HIJACKER.MODULE_MACRO_PLAY_ONGOING();

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

                                            /*
                                            //per100ms!
                                            if( !(millis()%100) ) {}
                                            //per1000ms!
                                            if( !(millis()%1000) ) {}
                                            */

                                            //per10000ms!
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
