#pragma once



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



void KeyboardHijacker::printKeyInfo(uint8_t keycode)
{
    Serial.print(getLogicalState(KEY_LEFT_CTRL)  ? "C" : " ");
    Serial.print(getLogicalState(KEY_LEFT_SHIFT) ? "S" : " ");
    Serial.print(getLogicalState(KEY_LEFT_ALT)   ? "A" : " ");
    Serial.print(getLogicalState(KEY_LEFT_GUI)   ? "G" : " ");
    Serial.print(" >"); print8bitHex(keycode); Serial.print("< ");
    Serial.print(getLogicalState(KEY_RIGHT_CTRL)  ? "C" : " ");
    Serial.print(getLogicalState(KEY_RIGHT_SHIFT) ? "S" : " ");
    Serial.print(getLogicalState(KEY_RIGHT_ALT)   ? "A" : " ");
    Serial.print(getLogicalState(KEY_RIGHT_GUI)   ? "G" : " ");
}

void KeyboardHijacker::syncToggleKeyStates() //Synchronize "SLAVE" kbd LEDs to "HOST(OS)" kbd LEDs
{
    //receive HOST states
    uint8_t stateLEDs = keyboard_leds;
    
    const uint8_t LED_NUM_LOCK    = 0b00000001;
    const uint8_t LED_CAPS_LOCK   = 0b00000010;
    const uint8_t LED_SCROLL_LOCK = 0b00000100;

    KBD_PARSER.numLock( stateNumLockToggle = (stateLEDs & LED_NUM_LOCK) );

    KBD_PARSER.capsLock( stateCapsLockToggle = (stateLEDs & LED_CAPS_LOCK) );

    KBD_PARSER.scrollLock( stateScrollLockToggle = (stateLEDs & LED_SCROLL_LOCK) );
}

void KeyboardHijacker::releaseAllBeingHoldDownKey()
{
    if(isDEBUG){ Serial.println(F(">>>>>>>> RELEASE ALL KEYS !!! <<<<<<<<")); }

    numBeingHoldDownKey = 0;

    for(uint8_t i=0; i<255; i++)
    {
        if(stateLogical[i])
        {
            stateLogical[i]=false;
            Keyboard.release(keycode_To_TeensyLayout(i));
        }
    }

    isExistWaitingEvent_Press=false;
    isExistWaitingEvent_Release=false;
}



void KeyboardHijacker::pressandreleaseKey(int32_t key)
{
    Keyboard.press(key); delay(5);
    Keyboard.release(key); delay(5);
}
void KeyboardHijacker::pressandreleaseKeys(String str)
{
    for(uint32_t i=0; i<str.length(); i++)
    {
        Keyboard.press(str.charAt(i)); delay(5);
        Keyboard.release(str.charAt(i)); delay(5);
    }
}
void KeyboardHijacker::pressandreleaseKeys(std::initializer_list<int32_t> keys)
{
    for(int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key); delay(5);
        Keyboard.release(key); delay(5);
    }
    
    delay(10);
}
void KeyboardHijacker::pressandreleaseKeys(int32_t* keys, int32_t len)
{
    for(int32_t i=0; i<len; i++)
    {
        if(keys[i] == 0)
            continue;

        Keyboard.press(keys[i]); delay(5);
        Keyboard.release(keys[i]); delay(5);
    }
    
    delay(10);
}
void KeyboardHijacker::pressandreleaseMultiKey(std::initializer_list<int32_t> keys)
{
    for(int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key);
        setLogicalState(key,true);
        delay(5);
    }
    
    releaseAllBeingHoldDownKey(); delay(10);
}
void KeyboardHijacker::pressandreleaseMultiKey(int32_t* keys, int32_t len)
{
    for(int32_t i=0; i<len; i++)
    {
        if(keys[i] == 0)
            continue;

        Keyboard.press(keys[i]);
        setLogicalState(keys[i],true);
        delay(5);
    }
    
    releaseAllBeingHoldDownKey(); delay(10);
}

void KeyboardHijacker::pressandreleaseKey_LikeHuman(int32_t key)
{
    Keyboard.press(key); randomDelayGenerator();
    Keyboard.release(key); randomDelayGenerator_Manually(15,+10);
}
void KeyboardHijacker::pressandreleaseKeys_LikeHuman(String str)
{
    for(uint32_t i=0; i<str.length(); i++)
    {
        Keyboard.press(str.charAt(i)); randomDelayGenerator();
        Keyboard.release(str.charAt(i)); randomDelayGenerator_Manually(15,+10);
    }
}
void KeyboardHijacker::pressandreleaseKeys_LikeHuman(std::initializer_list<int32_t> keys)
{
    for (int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key); randomDelayGenerator();
        Keyboard.release(key); randomDelayGenerator_Manually(15,+10);
    }
}



#include "KeyboardHijacker_TRANSMIT_AFTER_HIJACK.h"
#include "KeyboardHijacker_MODULE_KORPAD.h"
#include "KeyboardHijacker_MODULE_KEYMAPPER.h"
#include "KeyboardHijacker_MODULE_MACRO.h"
