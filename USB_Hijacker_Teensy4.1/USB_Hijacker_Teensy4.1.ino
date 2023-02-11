
/*  This Project using the Teensy® 4.1 Development Board https://www.pjrc.com/store/teensy41.html
 *  You have to install Teensyduino Installer https://www.pjrc.com/teensy/td_download.html
 *  
 *  This Project is recommended to select USB Type: "Keyboard" from Tools -> USB Type: menu.
 *  
 *  but if you want to use Serial communication for debugging, select USB Type: "Serial + Keyboard + Mouse + Joystick"
 *  in case YOU MUST UNCONDITIONALLY OPEN 'Serial Monitor' BEFORE UPLOAD PLEASE!!
 *  
 *  Let's H.I.J.A.C.K. 
 */





// CUSTOMIZE YOUR USB DESCRIPTION
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
     *  MANUFACTURER_NAME
     */

        2 + 7 * 2,
        3,
        {'$','0','?','H','!','$','+'}
    };
    
    usb_string_descriptor_struct usb_string_product_name = {
    /*
     *  2 + PRODUCT_NAME_LEN * 2,
     *  3,
     *  PRODUCT_NAME
     */

    #if defined(USB_KEYBOARDONLY)
        2 + 19 * 2,
        3,
        {'I','\'','m',' ','n','o','t',' ','H','i','j','a','c','k','e','r',' ','X','D'}
    #elif defined(USB_SERIAL_HID)
        2 + 28 * 2,
        3,
        {'I','\'','m',' ','n','o','t',' ','H','i','j','a','c','k','e','r',' ','S','E','R','I','A','L',' ','M','O','D','E'}
    #endif
    };
    
    usb_string_descriptor_struct usb_string_serial_number = {
    /*
     *  2 + SERIAL_NUMBER_LEN * 2,
     *  3,
     *  SERIAL_NUMBER
     */

        2 + 0 * 2,
        3,
        {}
    };
    
#ifdef __cplusplus
}
#endif
// And you can also edit VID, PID, etc. from YourArduinoFolder\hardware\teensy\avr\cores\teensy4\usb_desc.h





#include <USBHost_t36.h>

//#include <SPI.h>

#include <SD.h>
File textfile;

#include "res_layouts.h"
#include "res_pitches.h"





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

#define PIN_LED             13
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
#define PIN_PROGRAM         A15
#define PIN_RANDOMSEED      A16
#define PIN_BUZZER          A17





//// For, USER
void REBOOT()   { digitalWrite(PIN_PROGRAM,LOW); delay(100); return; }

bool isSerial = true;   //false : if you DO NOT WANT TO USE Serial any circumstances
bool isExistSD = true;  //false : if you DO NOT WANT TO USE SD any circumstances

extern unsigned long _heap_end; //extern char *__brkval;
void MEASURE_FREE_MEMORY() // for Teensy 4.1
{
    if(!isSerial) 
        return;
    
    char* p=(char*)malloc(10000); free(p); // size should be quite big, to avoid allocating fragment!
    uint32_t freeMemory=(char *)&_heap_end - p; //=__brkval;

    Serial.print("\nNow Measured FreeMemory : ");
    Serial.println(freeMemory);
}

//extern uint32_t msMeasuredForDebugging;
//void MEASURE_MILLIS_START_POINT()
//{
//    msMeasuredForDebugging=millis();
//}
//void MEASURE_MILLIS_END_POINT()
//{
//    if(!isSerial) 
//        return;
//    
//    Serial.print(F("\nMeasured Time : "));
//    Serial.println(millis()-msMeasuredForDebugging);
//}

struct Buzzzzer
{
    uint16_t melody[128] = {0};
    uint16_t rhythm[128] = {0};
    uint16_t len = 0;
    uint16_t m_proc,r_proc;

    void reserveBuzz(uint16_t* m, uint16_t* r, uint16_t l)
    {
        for(uint16_t i=0; i<l; i++){
            melody[i]=m[i];
            rhythm[i]=r[i];
        }
        delete[] m;
        delete[] r;
        
        len=l;
        m_proc=r_proc=0;
    }
/*
 *  reserveBuzz Example)
    
    Buzzzzer.reserveBuzz(   new uint16_t[13] {  NOTE_DS7,0, NOTE_DS6,0, NOTE_B6,0,  NOTE_A6,0,  NOTE_DS6,0, NOTE_DS7,0, NOTE_B6 },
                            new uint16_t[13] {  220,20,     130,20,     270,30,     230,20,     170,20,     270,20,     400     },
                            13  );
 * 
 */

    void playBuzz()
    {
        if(len==0)
            return;
        
        if(m_proc<len)
            if(r_proc<rhythm[m_proc]){
                if(r_proc==0){
                    if(melody[m_proc]!=0)
                    { tone(PIN_BUZZER,melody[m_proc],rhythm[m_proc]); }
                    else
                    { noTone(PIN_BUZZER); }
                }
                r_proc++;
            }
            else{
                m_proc++;
                r_proc=0;
            }
        else
            len=m_proc=r_proc=0;
    }
} Buzzzzer;





//// For, HOST(OS)
class KeyboardHijacker
{
 private:
    bool stateCapsLockToggle    = false;
    bool stateScrollLockToggle  = false;
    bool stateNumLockToggle     = false;
    bool stateLogical[255]      = {false};
    uint8_t numBeingHoldDownKey = 0;

 public:
    void switchStateCapsLockToggle()    { stateCapsLockToggle=!stateCapsLockToggle; }
    void switchStateScrollLockToggle()  { stateScrollLockToggle=!stateScrollLockToggle; }
    void switchStateNumLockToggle()     { stateNumLockToggle=!stateNumLockToggle; }
    bool getStateCapsLockToggle()       { return stateCapsLockToggle; }
    bool getStateScrollLockToggle()     { return stateScrollLockToggle; }
    bool getStateNumLockToggle()        { return stateNumLockToggle; }
    void setLogicalState                (int32_t key, bool state)   { stateLogical[TeensyLayout_To_Keycode(key)] = state; }
    bool getLogicalState                (int32_t key)               { return stateLogical[TeensyLayout_To_Keycode(key)]; }

    void syncToggleKeyStates            (void);
    bool reserveSyncTKS = false;

    bool isExistHoldingDownKey          (void) { return (numBeingHoldDownKey>0); }
    void releaseAllBeingHoldDownKey     (void);
    void syncBeingHoldDownKey           (void);

    void pressandreleaseKey             (int32_t key);
    void pressandreleaseKeys            (int32_t len, int32_t* keys);
    void pressandreleaseKeys            (String str);
    void pressandreleaseShortcutKey     (int32_t len, int32_t* keys); // ex) ctrl+c, gui+r, ctrl+alt+esc

    void pressandreleaseKey_LikeHuman           (int32_t key);
    void pressandreleaseKeys_LikeHuman          (int32_t len, int32_t* keys);
    void pressandreleaseKeys_LikeHuman          (String str);
    void pressandreleaseShortcutKey_LikeHuman   (int32_t len, int32_t* keys); // ex) ctrl+c, gui+r, ctrl+alt+esc

    uint32_t msBasedDelay                       = 30;
    uint32_t msExtraDelayMax                    = +50;
    void randomDelayChanger                     (int32_t based, int32_t extraMax)   { msBasedDelay = based; msExtraDelayMax = extraMax; }
    void randomDelayGenerator                   (void)                              { delay(msBasedDelay); if(msExtraDelayMax > 0) delay(random(msExtraDelayMax)); }
    void randomDelayGenerator_Manually          (int32_t based, int32_t extraMax)   { delay(based); if(extraMax > 0) delay(random(extraMax)); }

    // THE C.O.R.E. of HIJACKER
    void txHijackedKeyEvent();
    void funcMACRO_CA7(), funcMACRO_CA8(), funcMACRO_CA9();
} KBD_Hijacker;





//// For, SLAVE KEYBOARD
USBHost USBHostOnTeensy;
USBHub Hub_0(USBHostOnTeensy), Hub_1(USBHostOnTeensy), Hub_2(USBHostOnTeensy), Hub_3(USBHostOnTeensy), Hub_4(USBHostOnTeensy), Hub_5(USBHostOnTeensy), Hub_6(USBHostOnTeensy), Hub_7(USBHostOnTeensy);
KeyboardController KBD_Parser(USBHostOnTeensy);

volatile int32_t key;   //keycode on the Teensy layout
volatile bool event;    //true:pressed false:released
volatile bool isActivateKeyEvent;

bool statePhysical[255] = {false};
void setPhysicalState(int32_t key, bool state)  { statePhysical[TeensyLayout_To_Keycode(key)] = state; }
bool getPhysicalState(int32_t key)              { return statePhysical[TeensyLayout_To_Keycode(key)]; }

volatile bool isExistWaitingEvent_Press     = false;
volatile bool isExistWaitingEvent_Release   = false;

volatile uint32_t msLatestEventCame        = millis();
volatile uint32_t msLatestEventPressed     = millis();
volatile uint8_t numDN = 0;

#define KEYLOGGER_LEN_MAX 10
#define null NULL
struct nodeKeycode
{
    uint8_t keycode;
    uint32_t millis;
    
    nodeKeycode* next;
    nodeKeycode* prev;
};
struct linkedlistKeyLogger
{
    nodeKeycode* top;
    nodeKeycode* bottom;
    uint32_t len;

    linkedlistKeyLogger() //Initialize
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

        return;
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
    void pop()
    {
        nodeKeycode* delNode = top;
        top = delNode->next;
        top->prev = NULL;
        free(delNode);
        len--;

        return;
    }

    void PrintAll() //For, Debugging
    {
        if(!isSerial)
            return;
        
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
        
        return;
    }
} KeyLogger;

void PrintKey(uint8_t keycode)
{
    Serial.print((KBD_Hijacker.getLogicalState(KEY_LEFT_CTRL)   == true) ? "C" : " ");
    Serial.print((KBD_Hijacker.getLogicalState(KEY_LEFT_SHIFT)  == true) ? "S" : " ");
    Serial.print((KBD_Hijacker.getLogicalState(KEY_LEFT_ALT)    == true) ? "A" : " ");
    Serial.print((KBD_Hijacker.getLogicalState(KEY_LEFT_GUI)    == true) ? "G" : " ");
    Serial.print(" >0x"); if(keycode<16)Serial.print('0'); Serial.print(keycode, HEX); Serial.print("< ");
    Serial.print((KBD_Hijacker.getLogicalState(KEY_RIGHT_CTRL)  == true) ? "C" : " ");
    Serial.print((KBD_Hijacker.getLogicalState(KEY_RIGHT_SHIFT) == true) ? "S" : " ");
    Serial.print((KBD_Hijacker.getLogicalState(KEY_RIGHT_ALT)   == true) ? "A" : " ");
    Serial.print((KBD_Hijacker.getLogicalState(KEY_RIGHT_GUI)   == true) ? "G" : " ");
}
void OnRawPress(uint8_t keycode)
{
    KeyLogger.push(keycode);
    
    key   = keycode_To_TeensyLayout(keycode);
    event = true;
    setPhysicalState(key,true);
    
    //MODULE_MACRO
    {
        //if Recording
        MODULE_MACRO_RECORDER_REC_PRESSED(keycode, millis()-msLatestEventCame);

        //if Playing
        MODULE_MACRO_PLAYER_SETSTATE_BY_FORCE();
    }
    
    isExistWaitingEvent_Press = true;
    numDN+=1;



    if(isSerial)
    {
        MEASURE_FREE_MEMORY();
        
        Serial.print(F("\n(Before Hijack) DN ")); PrintKey(keycode);
        Serial.println();
    }
}
void OnRawRelease(uint8_t keycode)
{
    key   = keycode_To_TeensyLayout(keycode);
    event = false;
    setPhysicalState(key,false);

    //MODULE_MACRO
    {
        //if Recording
        MODULE_MACRO_RECORDER_REC_RELEASED(keycode, millis()-msLatestEventCame);

        //if Playing
        MODULE_MACRO_PLAYER_SETSTATE_BY_FORCE();
    }
    
    isExistWaitingEvent_Release = true;
    if(numDN>0) numDN-=1;

    //syncToggleKeyStates() in a few ms
    if(key == KEY_NUM_LOCK || key == KEY_CAPS_LOCK || key == KEY_SCROLL_LOCK) KBD_Hijacker.reserveSyncTKS=true;


    
    if(isSerial)
    {
        MEASURE_FREE_MEMORY();
        
        Serial.print(F("\n(Before Hijack) UP ")); PrintKey(keycode);
        Serial.print(F("         Pressed Time : ")); Serial.println(millis()-msLatestEventPressed);
    }
}





IntervalTimer REPEATER_per1ms;
void per1ms()
{
    static bool isDormancy = false;
    (millis()-msLatestEventCame > 30000) ? isDormancy=true : isDormancy=false;

    //per1ms!
    {
        Buzzzzer.playBuzz();
        
        MODULE_MACRO_PLAYER_ONGOING();

        static int8_t countdownSyncTKS = -1;
        if(-1<countdownSyncTKS)
        {
            countdownSyncTKS--;
            if(countdownSyncTKS==0) KBD_Hijacker.syncToggleKeyStates();
        }
        if(KBD_Hijacker.reserveSyncTKS)
        {
            countdownSyncTKS=70;
            KBD_Hijacker.reserveSyncTKS=false;
        }
    }

    //per100ms!
    if( !(millis()%100) )
    {
        
    }

    //per1000ms!
    if( !(millis()%1000) )
    {
        
    }

    //per10000ms!
    if( !(millis()%10000) )
    {
        if(isDormancy) KBD_Hijacker.syncToggleKeyStates();
    }
}

void setup()
{
    /* ------------------------------------------------ Initializing of Teensy4.1 ------------------------------------------------ */
    {
        pinMode(PIN_LED, OUTPUT);
        pinMode(PIN_PROGRAM, OUTPUT); digitalWrite(PIN_PROGRAM, HIGH);
        randomSeed(analogRead(PIN_RANDOMSEED));
        
        if(isSerial)
        {
            Serial.begin(115200);
            delay(5252);
            
            if(!Serial) { isSerial = false; Serial.end(); }
        }
        if(isSerial) Serial.println(F("SERIAL IS ONLINE\n"));
        
        REPEATER_per1ms.begin(per1ms,1000);
        if(isSerial) Serial.println(F("TIMER INTERRUPT STARTED\n"));
    }
    /* ------------------------------------------------ Initializing of Teensy4.1 ------------------------------------------------ */



    /* ------------------------------------------ Initializing of USB Host on Teensy4.1 ------------------------------------------ */
    {
        USBHostOnTeensy.begin();
        delay(100);
        
        KBD_Hijacker.syncToggleKeyStates();
        delay(100);
        
        KBD_Parser.attachRawPress(OnRawPress);
        KBD_Parser.attachRawRelease(OnRawRelease);
        
        if(isSerial) Serial.println(F("SLAVE KEYBOARD IS ATTACHED\n"));
    }
    /* ------------------------------------------ Initializing of USB Host on Teensy4.1 ------------------------------------------ */



    /* ------------------------------------------ Initializing of miscellaneous modules ------------------------------------------ */
    {
        // PIEZO BUZZER
        Buzzzzer.reserveBuzz(   new uint16_t[4] {   NOTE_B6,0,  NOTE_E7,    0   },
                                new uint16_t[4] {   111,11,     444,        111 },
                                4   );
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

    MODULE_INITIALIZE_KEYMAP();
}

void loop()
{
    USBHostOnTeensy.Task();

    if(isExistWaitingEvent_Press)
    {
        msLatestEventCame = msLatestEventPressed = millis();
        
        KBD_Hijacker.txHijackedKeyEvent();
        
        isExistWaitingEvent_Press   = false;
    }

    if(isExistWaitingEvent_Release)
    {
        msLatestEventCame = millis();
        
        KBD_Hijacker.txHijackedKeyEvent();
        
        isExistWaitingEvent_Release = false;
    }

    numDN>0 ? digitalWrite(PIN_LED, HIGH) : digitalWrite(PIN_LED, LOW);
}
