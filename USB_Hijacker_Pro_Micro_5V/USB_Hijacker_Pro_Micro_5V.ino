
/*  This Project using the Arduino Pro Micro & UHS_mini_v2.0
 *  
 *  in case YOU MUST UNCONDITIONALLY OPEN 'Serial Monitor' BEFORE UPLOAD PLEASE!!
 *  
 *  Let's H.I.J.A.C.K.
 */





#include <HID-Project.h>

#include <SPI.h>
#include <usbhub.h>
#include <hidboot.h>

#include "res_layouts.h"
#include "res_pitches.h"

//#include <MemoryFree.h>
//#define MEASURE_FREE_MEMORY if(isSerial){ Serial.print("\nNow Measured FreeMemory : "); Serial.print(freeMemory()); Serial.print("\n\n"); }





#define PIN_BUZZER           2
//      PIN_                 3
//      PIN_                 4
//      PIN_                 5
//      PIN_                 6
//      PIN_                 7
//      PIN_                 8
#define PIN_UHSmini_INT      9
#define PIN_UHSmini_SS      10
#define PIN_UHSmini_MOSI    16
#define PIN_UHSmini_MISO    14
#define PIN_UHSmini_CLK     15
//      PIN_                A0
//      PIN_                A1
//      PIN_                A2
#define PIN_RANDOMSEED      A3





//// For, USER
bool isSerial = true;   //false : if you DO NOT WANT TO USE Serial any circumstances

//uint32_t msMeasuredForDebugging=0;
//#define MEASURE_START_POINT   msMeasuredForDebugging=millis();
//#define MEASURE_END_POINT     if(isSerial){ Serial.print("\nMeasured Time : "); Serial.println(millis()-msMeasuredForDebugging); Serial.println(); }





//// For, HOST(OS)
class KeyboardHijacker
{
 private:
    bool stateCapsLockToggle                    = false;
    bool stateScrollLockToggle                  = false;
    bool stateNumLockToggle                     = false;
    volatile bool stateLogical[255]             = {false};
    uint8_t numBeingHoldDownKey                 = 0;

 public:
    void switchStateCapsLockToggle()            { stateCapsLockToggle=!stateCapsLockToggle; }
    void switchStateScrollLockToggle()          { stateScrollLockToggle=!stateScrollLockToggle; }
    void switchStateNumLockToggle()             { stateNumLockToggle=!stateNumLockToggle; }
    bool getStateCapsLockToggle()               { return stateCapsLockToggle; }
    bool getStateScrollLockToggle()             { return stateScrollLockToggle; }
    bool getStateNumLockToggle()                { return stateNumLockToggle; }
    void setLogicalState                        (uint8_t keycode, bool state)   { stateLogical[keycode] = state; }
    bool getLogicalState                        (uint8_t keycode)               { return stateLogical[keycode]; }
    
    void syncToggleKeyStates                    (void);
    
    bool isExistHoldingDownKey                  (void) { return (numBeingHoldDownKey>0); }
    void releaseAllBeingHoldDownKey             (void);
    
    void pressandreleaseKey                     (KeyboardKeycode key);
    void pressandreleaseKeys                    (uint8_t len, KeyboardKeycode* keys);
    void pressandreleaseKeys                    (String str);
    void pressandreleaseShortcutKey             (uint8_t len, KeyboardKeycode* keys); // ex) ctrl+c, gui+r, ctrl+alt+esc
    
//    void pressandreleaseKey_LikeHuman           (KeyboardKeycode key);
//    void pressandreleaseKey_LikeHuman           (char ch);
//    void pressandreleaseKeys_LikeHuman          (KeyboardKeycode* keys, uint8_t len);
//    void pressandreleaseKeys_LikeHuman          (String str);
//    void pressandreleaseShortcutKey_LikeHuman   (KeyboardKeycode* keys, uint8_t len); // ex) ctrl+c, gui+r, ctrl+alt+esc
//
//    uint16_t msBasedDelay                       = 30;
//    uint16_t msExtraDelayMax                    = +50;
//    void randomDelayChanger                     (int32_t based, int32_t extraMax)   { msBasedDelay = based; msExtraDelayMax = extraMax; }
//    void randomDelayGenerator                   (void)                              { delay(msBasedDelay); if(msExtraDelayMax > 0) delay(random(msExtraDelayMax)); }
//    void randomDelayGenerator_Manually          (int32_t based, int32_t extraMax)   { delay(based); if(extraMax > 0) delay(random(extraMax)); }

    // THE C.O.R.E. of HIJACKER
    void txHijackedKeyEvent();

    void MODULE_KOREAN_KEYPAD_EVOLUTION();
} KBD_Hijacker;





//// For, SLAVE KEYBOARD
USB USBHostOnUHSmini;
USBHub Hub_0(&USBHostOnUHSmini), Hub_1(&USBHostOnUHSmini), Hub_2(&USBHostOnUHSmini), Hub_3(&USBHostOnUHSmini), Hub_4(&USBHostOnUHSmini), Hub_5(&USBHostOnUHSmini), Hub_6(&USBHostOnUHSmini), Hub_7(&USBHostOnUHSmini);
HIDBoot<HID_PROTOCOL_KEYBOARD> kbd(&USBHostOnUHSmini);

volatile KeyboardKeycode key;
volatile bool event;    //true:pressed false:released
volatile bool isActivateKeyEvent;

volatile bool isExistWaitingEvent_Press     = false;
volatile bool isExistWaitingEvent_Release   = false;

volatile uint32_t msLatestEventCame        = millis();
volatile uint32_t msLatestEventPressed     = millis();
volatile uint8_t numDN = 0;

class KbdRptParser : public KeyboardReportParser
{
 protected:
    void OnControlKeysChanged               (uint8_t before, uint8_t after);
    void OnKeyDown                          (uint8_t mod, uint8_t keycode);
    void OnKeyUp                            (uint8_t mod, uint8_t keycode);
 public:
    void PrintKey                           (uint8_t mod, uint8_t keycode);
    void switchCapsLockLED                  (void);
    void switchScrollLockLED                (void);
    void switchNumLockLED                   (void);
    void freezeSystemUntilAllKeyRelease     (void)  { while(numDN)USBHostOnUHSmini.Task(); isExistWaitingEvent_Press=isExistWaitingEvent_Release=false; }
} KBD_Parser;





void setup()
{
    /* ----------------------------- Initializing of Arduino_Pro_Micro ----------------------------- */
    randomSeed(analogRead(PIN_RANDOMSEED));
    
    if(isSerial)
    {
        Serial.begin(115200);
        delay(5252); // NECESSARY FOR "if(Serial)"
        if(!Serial) { isSerial = false; Serial.end(); }
    }
    
    if(isSerial){ Serial.println("SERIAL IS ONLINE\n"); }
    /* ----------------------------- Initializing of Arduino_Pro_Micro ----------------------------- */



    /* ------------------------------- Initializing of UHS_mini_v2.0 ------------------------------- */
    if (USBHostOnUHSmini.Init() == -1)
        if(isSerial) Serial.println("USB Shield Module did not start.");
    kbd.SetReportParser(0, &KBD_Parser);
    delay(100);
    
    KBD_Hijacker.syncToggleKeyStates();
    delay(100);
    
    if(isSerial) Serial.println("SLAVE KEYBOARD IS ATTACHED\n");
    /* ------------------------------- Initializing of UHS_mini_v2.0 ------------------------------- */



    /* --------------------------- Initializing of KEY Event Transmitter --------------------------- */
    BootKeyboard.begin();
    Consumer.begin();
    delay(100);
    /* --------------------------- Initializing of KEY Event Transmitter --------------------------- */



    if(isSerial) Serial.println("OPERATION COMPLETE, SIR! XD");
    TONE(NOTE_C5,200); delay(200);
    TONE(NOTE_D5,200); delay(200);
    TONE(NOTE_E5,200);
}

void loop()
{
    USBHostOnUHSmini.Task();

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

    if(numDN>0)
        PORTD&=0b11011111; // Control TXLED

    static uint32_t msLatestSynced;
    if(!(millis()%100) && msLatestSynced!=millis()){ KBD_Hijacker.syncToggleKeyStates(); msLatestSynced=millis(); }
}
