
void KeyboardHijacker::syncToggleKeyStates() //Synchronize "SLAVE" kbd LEDs to "HOST(OS)" kbd LEDs
{
    //receive HOST states
    uint8_t stateLEDs = BootKeyboard.getLeds();

    if (stateLEDs & LED_CAPS_LOCK)                  //if HOST 's CapsLock state is ON
        if(!KBD_Hijacker.stateCapsLockToggle){      //if SLAVE's CapsLock state is OFF, Synchronize both
            KBD_Hijacker.stateCapsLockToggle = true;
            KBD_Parser.switchCapsLockLED();
        } else{}
    else                                            //if HOST 's CapsLock state is OFF
        if( KBD_Hijacker.stateCapsLockToggle){      //if SLAVE's CapsLock state is ON , Synchronize both
            KBD_Hijacker.stateCapsLockToggle = false;
            KBD_Parser.switchCapsLockLED();
        } else{}
    if (stateLEDs & LED_SCROLL_LOCK)                //if HOST 's ScrollLock state is ON
        if(!KBD_Hijacker.stateScrollLockToggle){    //if SLAVE's ScrollLock state is OFF, Synchronize both
            KBD_Hijacker.stateScrollLockToggle = true;
            KBD_Parser.switchScrollLockLED();
        } else{}
    else                                            //if HOST 's ScrollLock state is OFF
        if( KBD_Hijacker.stateScrollLockToggle){    //if SLAVE's ScrollLock state is ON , Synchronize both
            KBD_Hijacker.stateScrollLockToggle = false;
            KBD_Parser.switchScrollLockLED();
        } else{}
    if (stateLEDs & LED_NUM_LOCK)                   //if HOST 's NumLock state is ON
        if(!KBD_Hijacker.stateNumLockToggle){       //if SLAVE's NumLock state is OFF, Synchronize both
            KBD_Hijacker.stateNumLockToggle = true;
            KBD_Parser.switchNumLockLED();
        } else{}
    else                                            //if HOST 's NumLock state is OFF
        if( KBD_Hijacker.stateNumLockToggle){       //if SLAVE's NumLock state is ON , Synchronize both
            KBD_Hijacker.stateNumLockToggle = false;
            KBD_Parser.switchNumLockLED();
        } else{}

    return;
}

void KeyboardHijacker::releaseAllBeingHoldDownKey()
{
    KBD_Hijacker.numBeingHoldDownKey = 0;
    isActivateKeyEvent=false;
    
    for(uint8_t i=0; i<255; i++)
    {
        stateLogical[i]=false;
        Keyboard.release((KeyboardKeycode)i);
    }
        
    if(isSerial){ Serial.println(">>>>>>>> ALL KEYs ARE RELEASED !!! <<<<<<<<"); }

    return;
}

void KeyboardHijacker::pressandreleaseKey(KeyboardKeycode key)
{
    Keyboard.press(key); delay(3);
    Keyboard.release(key); delay(3);

    return;
}
void KeyboardHijacker::pressandreleaseKeys(uint8_t len, KeyboardKeycode* keys)
{
    for(uint8_t i=0; i<len; i++)
    {
        Keyboard.press(keys[i]); delay(3);
        Keyboard.release(keys[i]); delay(3);
    }

    return;
}
void KeyboardHijacker::pressandreleaseKeys(String str)
{
    for(uint8_t i=0; i<str.length(); i++)
    {
        Keyboard.press(str.charAt(i)); delay(3);
        Keyboard.release(str.charAt(i)); delay(3);
    }

    return;
}
void KeyboardHijacker::pressandreleaseShortcutKey(uint8_t len, KeyboardKeycode* keys)
{
    for(uint8_t i=0; i<len; i++)
    {
        Keyboard.press(keys[i]); delay(3);
    }
    for(uint8_t i=0; i<len; i++)
    {
        Keyboard.release(keys[i]); delay(3);
    }

    return;
}

//void KeyboardHijacker::pressandreleaseKey_LikeHuman(KeyboardKeycode key)
//{
//    Keyboard.press(key); randomDelayGenerator();
//    Keyboard.release(key); randomDelayGenerator();
//
//    return;
//}
//void KeyboardHijacker::pressandreleaseKeys_LikeHuman(uint8_t len, KeyboardKeycode* keys)
//{
//    for(uint8_t i=0; i<len; i++)
//    {
//        Keyboard.press(keys[i]); randomDelayGenerator();
//        Keyboard.release(keys[i]); randomDelayGenerator();
//    }
//
//    return;
//}
//void KeyboardHijacker::pressandreleaseKeys_LikeHuman(String str)
//{
//    for(uint8_t i=0; i<str.length(); i++)
//    {
//        Keyboard.press(str.charAt(i)); randomDelayGenerator();
//        Keyboard.release(str.charAt(i)); randomDelayGenerator();
//    }
//
//    return;
//}
//void KeyboardHijacker::pressandreleaseShortcutKey_LikeHuman(uint8_t len, KeyboardKeycode* keys)
//{
//    for(uint8_t i=0; i<len; i++)
//    {
//        Keyboard.press(keys[i]); randomDelayGenerator();
//    }
//    for(uint8_t i=0; i<len; i++)
//    {
//        Keyboard.release(keys[i]); randomDelayGenerator();
//    }
//
//    return;
//}
