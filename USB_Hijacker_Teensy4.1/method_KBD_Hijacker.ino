
void KeyboardHijacker::syncToggleKeyStates() //Synchronize "SLAVE" kbd LEDs to "HOST(OS)" kbd LEDs
{
    //receive HOST states
    uint8_t stateLEDs = keyboard_leds;
    
    const uint8_t LED_NUM_LOCK    = 0b00000001;
    const uint8_t LED_CAPS_LOCK   = 0b00000010;
    const uint8_t LED_SCROLL_LOCK = 0b00000100;

    if (stateLEDs & LED_NUM_LOCK)       //if HOST 's NumLock state is ON
    {
        KBD_Hijacker.stateNumLockToggle = true;
        KBD_Parser.numLock(true);
    }
    else                                //if HOST 's NumLock state is OFF
    {
        KBD_Hijacker.stateNumLockToggle = false;
        KBD_Parser.numLock(false);
    }
    
    if (stateLEDs & LED_CAPS_LOCK)      //if HOST 's CapsLock state is ON
    {
        KBD_Hijacker.stateCapsLockToggle = true;
        KBD_Parser.capsLock(true);
    }
    else                                //if HOST 's CapsLock state is OFF
    {
        KBD_Hijacker.stateCapsLockToggle = false;
        KBD_Parser.capsLock(false);
    }
    
    if (stateLEDs & LED_SCROLL_LOCK)    //if HOST 's ScrollLock state is ON
    {
        KBD_Hijacker.stateScrollLockToggle = true;
        KBD_Parser.scrollLock(true);
    }
    else                                //if HOST 's ScrollLock state is OFF
    {
        KBD_Hijacker.stateScrollLockToggle = false;
        KBD_Parser.scrollLock(false);
    }
    
    return;
}

void KeyboardHijacker::releaseAllBeingHoldDownKey()
{
    //Keyboard.releaseAll();
    
    KBD_Hijacker.numBeingHoldDownKey = 0;
    isActivateKeyEvent=false;
    
    for(uint8_t i=0; i<255; i++)
    {
        if(stateLogical[i])
        {
            stateLogical[i]=false;
            Keyboard.release(keycode_To_TeensyLayout(i));
        }
    }
        
    if(isSerial){ Serial.println(F(">>>>>>>> ALL KEYs ARE RELEASED !!! <<<<<<<<")); }

    return;
}

void KeyboardHijacker::syncBeingHoldDownKey()
{
    //Keyboard.releaseAll();
    
    uint8_t numSynced = 0;
    
    for(uint8_t i=0; i<255; i++)
    {
        if(statePhysical[i]>stateLogical[i])
        {
            Keyboard.press(keycode_To_TeensyLayout(i));
            
            stateLogical[i]=true;
            numSynced++;
        }
    }

    KBD_Hijacker.numBeingHoldDownKey += numSynced;

    return;
}

void KeyboardHijacker::pressandreleaseKey(int32_t key)
{
    Keyboard.press(key); delay(1);
    Keyboard.release(key); delay(1);

    return;
}
void KeyboardHijacker::pressandreleaseKeys(int32_t len, int32_t* keys)
{
    for(int32_t i=0; i<len; i++)
    {
        Keyboard.press(keys[i]); delay(1);
        Keyboard.release(keys[i]); delay(1);
    }

    return;
}
void KeyboardHijacker::pressandreleaseKeys(String str)
{
    if(str.length()==1)
    {
        Keyboard.press(str.charAt(0)); delay(1);
        Keyboard.release(str.charAt(0)); delay(1);

        return;
    }
    else
    {
        int32_t len = str.length();
        
        for(int32_t i=0; i<len; i++)
        {
            Keyboard.press(str.charAt(i)); delay(1);
            Keyboard.release(str.charAt(i)); delay(1);
        }
    }
    return;
}
void KeyboardHijacker::pressandreleaseShortcutKey(int32_t len, int32_t* keys)
{
    for(int32_t i=0; i<len; i++)
    {
        Keyboard.press(keys[i]); delay(1);
    }
    for(int32_t i=0; i<len; i++)
    {
        Keyboard.release(keys[i]); delay(1);
    }

    return;
}

void KeyboardHijacker::pressandreleaseKey_LikeHuman(int32_t key)
{
    Keyboard.press(key); randomDelayGenerator();
    Keyboard.release(key); randomDelayGenerator_Manually(15,+10);

    return;
}
void KeyboardHijacker::pressandreleaseKeys_LikeHuman(int32_t len, int32_t* keys)
{
    for(int32_t i=0; i<len; i++)
    {
        Keyboard.press(keys[i]); randomDelayGenerator();
        Keyboard.release(keys[i]); randomDelayGenerator_Manually(15,+10);
    }

    return;
}
void KeyboardHijacker::pressandreleaseKeys_LikeHuman(String str)
{
    int32_t len = str.length();
    
    for(int32_t i=0; i<len; i++)
    {
        Keyboard.press(str.charAt(i)); randomDelayGenerator();
        Keyboard.release(str.charAt(i)); randomDelayGenerator_Manually(15,+10);
    }

    return;
}
void KeyboardHijacker::pressandreleaseShortcutKey_LikeHuman(int32_t len, int32_t* keys)
{
    for(int32_t i=0; i<len; i++)
    {
        Keyboard.press(keys[i]); randomDelayGenerator();
    }

    for(int32_t i=0; i<len; i++)
    {
        Keyboard.release(keys[i]); randomDelayGenerator_Manually(15,+10);
    }

    return;
}
