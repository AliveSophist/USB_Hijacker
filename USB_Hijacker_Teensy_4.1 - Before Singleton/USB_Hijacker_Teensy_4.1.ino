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

#include "res_layouts.h"
#include "res_utility.h"
#include "res_pitches.h"










/**⦓ For, SLAVE KEYBOARD ⦔**/

volatile uint8_t numDN = 0;

volatile bool isExistWaitingEvent_Press   = false;
volatile bool isExistWaitingEvent_Release = false;

USBHost USBHostOnTeensy;
USBHub Hub_0(USBHostOnTeensy), Hub_1(USBHostOnTeensy), Hub_2(USBHostOnTeensy), Hub_3(USBHostOnTeensy), Hub_4(USBHostOnTeensy), Hub_5(USBHostOnTeensy), Hub_6(USBHostOnTeensy), Hub_7(USBHostOnTeensy);

class KeyboardParser : public KeyboardController
{
private:
    static void OnRawPress          (uint8_t keycode);
    static void OnRawRelease        (uint8_t keycode);

    volatile uint8_t rawKeycode;

public:
    KeyboardParser(USBHost usbhost) : KeyboardController(usbhost)
    {
        this->attachRawPress        (OnRawPress);
        this->attachRawRelease      (OnRawRelease);
    }

    uint8_t getRawKeycode           () { return this->rawKeycode; }
    
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

            if(isOverflow()){
                nodeKeycode* delNode = bottom;
                bottom = bottom->prev;
                bottom->next = NULL;
                free(delNode);
                len--;
            }
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

} KBD_Parser(USBHostOnTeensy);










/**⦓ For, TRANSMIT TO HOST(OS) ⦔**/

volatile int32_t key;
volatile bool event;
volatile bool isActivateKeyEvent;

volatile uint32_t msLatestEventCame = 0, msLatestEventPressed = 0;
#define MILLIS_SINCE_LATEST_EVENT           millis()-msLatestEventCame
#define MILLIS_FROM_PRESSED_UNTIL_RELEASE   millis()-msLatestEventPressed

class KeyboardHijacker
{
private:
    bool stateCapsLockToggle    = false;
    bool stateScrollLockToggle  = false;
    bool stateNumLockToggle     = false;
    bool stateLogical[255]      = {false};
    
    uint8_t numBeingHoldDownKey = 0;

    uint32_t msBasedDelay       = 30;
    uint32_t msExtraDelayMax    = +50;

public:
    // THE C.O.R.E. of HIJACKER
    void TRANSMIT_AFTER_HIJACK          (void);

    void setLogicalState                (int32_t key, bool state) { stateLogical[TeensyLayout_To_keycode(key)] = state; }
    bool getLogicalState                (int32_t key) { return stateLogical[TeensyLayout_To_keycode(key)]; }
    void printKeyInfo                   (uint8_t keycode);

    void switchStateCapsLockToggle()    { stateCapsLockToggle=!stateCapsLockToggle; }
    void switchStateScrollLockToggle()  { stateScrollLockToggle=!stateScrollLockToggle; }
    void switchStateNumLockToggle()     { stateNumLockToggle=!stateNumLockToggle; }
    bool getStateCapsLockToggle()       { return stateCapsLockToggle; }
    bool getStateScrollLockToggle()     { return stateScrollLockToggle; }
    bool getStateNumLockToggle()        { return stateNumLockToggle; }
    void syncToggleKeyStates            (void);
    bool reserveSyncTKS = false;

    bool isExistHoldingDownKey          (void) { return (numBeingHoldDownKey>0); }
    void releaseAllBeingHoldDownKey     (void);

    static void pressandreleaseKey             (int32_t key);
    static void pressandreleaseKeys            (String str);
    static void pressandreleaseKeys            (std::initializer_list<int32_t> keys);
    static void pressandreleaseShortcutKey     (std::initializer_list<int32_t> keys); // ex) ctrl+c, gui+r, ctrl+shift+esc
    static void pressandreleaseKeys            (int32_t* keys, int32_t len);
    static void pressandreleaseShortcutKey     (int32_t* keys, int32_t len); // ex) ctrl+c, gui+r, ctrl+shift+esc

        // About Auto Delay functions
        void randomDelayChanger                     (int32_t based, int32_t extraMax) { msBasedDelay = based; msExtraDelayMax = extraMax; }
        void randomDelayGenerator                   (void) { delay(msBasedDelay); if(msExtraDelayMax > 0) delay(random(msExtraDelayMax+1)); }
        void randomDelayGenerator_Manually          (int32_t based, int32_t extraMax) { delay(based); if(extraMax > 0) delay(random(extraMax+1)); }
        void pressandreleaseKey_LikeHuman           (int32_t key);
        void pressandreleaseKeys_LikeHuman          (String str);
        void pressandreleaseKeys_LikeHuman          (std::initializer_list<int32_t> keys);
        void pressandreleaseShortcutKey_LikeHuman   (std::initializer_list<int32_t> keys); // ex) ctrl+c, gui+r, ctrl+shift+esc

} KBD_Hijacker;










/**⦓   For, COMMON   ⦔**/

bool isSerial = true, isExistSD = true;  // false : if you DO NOT WANT TO USE any circumstances

IntervalTimer IntervalTimer_per1ms;

namespace Buzzzzer
{
    uint16_t melody[128] = {0};
    uint16_t rhythm[128] = {0};
    uint16_t len = 0;
    uint16_t m_proc,r_proc;

    /*
        reserveBuzz Example)

        Buzzzzer::reserveBuzz(   { NOTE_DS7,0, NOTE_DS6,0, NOTE_B6,0,  NOTE_A6,0,  NOTE_DS6,0, NOTE_DS7,0, NOTE_B6 }
                            ,   { 220,20,     130,20,     270,30,     230,20,     170,20,     270,20,     400     }   );

        It works! PO『std::initializer_list』WER
    */
    void reserveBuzz(std::initializer_list<uint16_t> m, std::initializer_list<uint16_t> r)
    {
        len = m.size();

        auto iteratorM = m.begin();
        auto iteratorR = r.begin();

        for (uint16_t i=0; i<len; i++) {
            melody[i] = *iteratorM++;
            rhythm[i] = *iteratorR++;
        }

        m_proc=r_proc=0;
    }
    void playBuzz()
    {
        if(len==0)
            return;
        
        if(m_proc<len)
            if(r_proc<rhythm[m_proc]){
                if(r_proc==0){
                    if(melody[m_proc]!=0)
                        tone(PIN_BUZZER,melody[m_proc],rhythm[m_proc]);
                    else
                        noTone(PIN_BUZZER);
                }
                r_proc++;
            }
            else{
                m_proc++;
                r_proc=0;
            }
        else
            len = 0;
    }
    void replayBuzz()
    {
        len = m_proc+1;

        m_proc=r_proc=0;
    }
}

void setup()
{
    /* ------------------------------------------------ Initializing of Teensy® 4.1 ------------------------------------------------ */
    {
        pinMode(PIN_LED_BUILTIN, OUTPUT);
        randomSeed(analogRead(PIN_RANDOMSEED));
        
        if(isSerial)
        {
            Serial.begin(115200);
            delay(5252);
            
            if(!Serial) { isSerial = false; Serial.end(); }
        }
        if(isSerial) Serial.println(F("SERIAL IS ONLINE\n"));
        
        // INTERRUPT TIMER on Teensy® 4.1     // by C++ Lambda!
        IntervalTimer_per1ms.begin  (   []
                                        {
                                            static bool isDormancy = false;
                                            
                                            //per1ms!
                                            {
                                                Buzzzzer::playBuzz();
                                                
                                                MODULE_MACRO_PLAYER_ONGOING();

                                                static int8_t countdownSyncTKS = -1;
                                                if(countdownSyncTKS>-1)
                                                {
                                                    countdownSyncTKS--;
                                                    if(countdownSyncTKS==0) KBD_Hijacker.syncToggleKeyStates();
                                                }
                                                if(KBD_Hijacker.reserveSyncTKS)
                                                {
                                                    countdownSyncTKS=77;
                                                    KBD_Hijacker.reserveSyncTKS=false;
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

                                                if(isDormancy) KBD_Hijacker.syncToggleKeyStates();
                                            }
                                        }
                                        , 1000   );

        if(isSerial) Serial.println(F("TIMER INTERRUPT STARTED\n"));
    }
    /* ------------------------------------------------ Initializing of Teensy® 4.1 ------------------------------------------------ */



    /* ------------------------------------------ Initializing of USB Host on Teensy® 4.1 ------------------------------------------ */
    {
        USBHostOnTeensy.begin();
        delay(100);
        
        KBD_Hijacker.syncToggleKeyStates();
        delay(100);
        
        if(isSerial) Serial.println(F("SLAVE KEYBOARD IS ATTACHED\n"));
    }
    /* ------------------------------------------ Initializing of USB Host on Teensy® 4.1 ------------------------------------------ */



    /* ------------------------------------------ Initializing of miscellaneous modules ------------------------------------------ */
    {
        // PIEZO BUZZER
        Buzzzzer::reserveBuzz(   { NOTE_B6,0,  NOTE_E7,    0   }
                            ,   { 111,11,     444,        111 }   );
        if(isSerial) Serial.println(F("BUZZER PLAYS STARTUP SOUND ♬"));

        // SD CARD
        if(isExistSD)
        {
            if (SD.begin(BUILTIN_SDCARD))
            { isExistSD = true; if(isSerial) Serial.println(F("SD CARD IS AVAILABLE :)")); }
            else
            { isExistSD = false; if(isSerial) Serial.println(F("SD CARD IS NOT AVAILABLE :(")); }
        }
        
        if(isSerial) Serial.println(F("ALL MODULES ARE INITIALIZED\n"));
    }
    /* ------------------------------------------ Initializing of miscellaneous modules ------------------------------------------ */



    if(isSerial) Serial.println(F("OPERATION COMPLETE, SIR! XD"));

    MODULE_KEYMAPPER_INITIALIZE();
}

void loop()
{
    USBHostOnTeensy.Task();

    if(isExistWaitingEvent_Press)
    {
        key   = keycode_To_TeensyLayout( KBD_Parser.getRawKeycode() );
        event = true;
        msLatestEventCame = msLatestEventPressed = millis();

        KBD_Hijacker.TRANSMIT_AFTER_HIJACK();
        
        isExistWaitingEvent_Press   = false;
    }

    if(isExistWaitingEvent_Release)
    {
        key   = keycode_To_TeensyLayout( KBD_Parser.getRawKeycode() );
        event = false;
        msLatestEventCame = millis();

        KBD_Hijacker.TRANSMIT_AFTER_HIJACK();
        
        isExistWaitingEvent_Release = false;
    }

    numDN>0 ? digitalWrite(PIN_LED_BUILTIN, HIGH) : digitalWrite(PIN_LED_BUILTIN, LOW);
}
