
#define PRESSED_TIME_UNTIL_RELEASE millis()-msLatestEventPressed
#define SYSTEM_FREEZE_UNTIL_ALL_KEY_RELEASE while(numDN){}





void KeyboardHijacker::txHijackedKeyEvent()
{
    isActivateKeyEvent=true;



    if  (key == KEY_NUM_LOCK || key == KEY_CAPS_LOCK || key == KEY_SCROLL_LOCK)
    {
        if(event)
        {
            if(key == KEY_NUM_LOCK) switchStateNumLockToggle();
            if(key == KEY_CAPS_LOCK) switchStateCapsLockToggle();
            if(key == KEY_SCROLL_LOCK) switchStateScrollLockToggle();
        }
        else
        {
            //syncToggleKeyStates() after a few ms
            reserveSyncTKS=true;
        }
    }



    if(!stateNumLockToggle)
    {
        // MACRO Function KEYs (Ctrl+Alt+1, Ctrl+Alt+2, Ctrl+Alt+3, … Ctrl+Alt+9)
        if(getLogicalState(KEY_LEFT_CTRL) && getLogicalState(KEY_LEFT_ALT))
        {
            if(key == KEYPAD_7)
            {
                if(event)
                {
                    releaseAllBeingHoldDownKey(); delay(10);
        
                    pressandreleaseShortcutKey( {KEY_GUI,KEY_R} );
                    delay(1000);
                    pressandreleaseKeys("https://drive.google.com/file/d/1jy5C9P_xP0G-GG9I29iKfttaGfzxMcIq/view");
                    delay(1000);
                    pressandreleaseKey(KEY_ENTER);
                }
                key=0;
            }
            if(key == KEYPAD_8)
            {
                if(event)
                {
                    releaseAllBeingHoldDownKey(); delay(10);
                    
                    SYSTEM_FREEZE_UNTIL_ALL_KEY_RELEASE
                    SCB_AIRCR = 0x05FA0004; asm volatile ("dsb");
                    while(true){}
                }
                key=0;
            }
            if(key == KEYPAD_9)
            {
                if(event)
                {
                    releaseAllBeingHoldDownKey(); delay(10);
                    
                    // For, Debugging
                    delay(100);
                    pressandreleaseKeys_LikeHuman("Upload completed~ ");
                    pressandreleaseKeys_LikeHuman(__TIMESTAMP__);
                }
                key=0;
            }
        }
    }



    // KOREAN_KEYPAD_EVOLUTION_v2.0 Edition by SOPHIST
    MODULE_KOREAN_KEYPAD_EVOLUTION();



    // If mapped?, HIJACK! 
    MODULE_KEYMAPPER_HIJACK();



/*  Example1)
 *          Hijack One KEY to Another KEY

    if(key == KEY_***)
    {
        key = KEY_$$$;
    }


 *  Example2)
 *          Hijack One KEY to ROTATE KEY

    static uint8_t rotateNum_*** = 0;
    if(key == KEY_***)
    {
        switch(rotateNum_***) //rotation
        {
            case 0:
                key = KEY_$$$;
                break;
                
            …
            …
            …
            
            case n:
                key = KEY_%%%;
                break;
        }
        if(!event) rotateNum_***++; //after released, next Event change
        if(rotateNum_***>n) rotateNum_***=0; //reset
    }
    else rotateNum_***=0;


 *  Example3)
 *          Hijack One KEY to NONE And excute MACRO

    if(key == KEY_***)
    {
        releaseAllBeingHoldDownKey();       //To prevent INTERFERENCE, if release needed
        
        if(event){                          //MACROs, Only excute 'release' event
        
            …
            …
            …
        
        }
    }


 *  Example4)
 *          Hijack Shortcut KEY to NONE And excute MACRO

    if(key == KEY_*** && getLogicalState(KEY_$$$))
    {
        releaseAllBeingHoldDownKey();       //To prevent INTERFERENCE, if release needed
        
        if(event){                          //MACROs, Only excute 'release' event
        
            …
            …
            …
        
        }
    }


 *  Example5)
 *          ROTATE KEY Example...
    static uint8_t rotateNum_TILDE = 0;
    if (key == KEY_TILDE)
    {
        switch(rotateNum_TILDE) //rotation
        {
            case 0:
                key = KEY_0;
                break;
            case 1:
                key = KEY_1;
                break;
            case 2:
                key = KEY_2;
                break;
            case 3:
                key = KEY_3;
                break;
            case 4:
                key = KEY_4;
                break;
            case 5:
                key = KEY_5;
                break;
            case 6:
                key = KEY_6;
                break;
            case 7:
                key = KEY_7;
                break;
            case 8:
                key = KEY_8;
                break;
            case 9:
                key = KEY_9;
                break;
        }
        if(!event) rotateNum_TILDE++; //after released, next Event change
        if(rotateNum_TILDE>9) rotateNum_TILDE=0; //reset
    }
    else rotateNum_TILDE=0;

    
 *  
 */















    //MODULE_MACRO
    MODULE_MACRO_PLAYER_SHUTDOWN_ARBITER();



    if(isActivateKeyEvent)
    {
        if(event)
        {
            //Skip KEY release event, if bugged
            if(getLogicalState(key))
                return;

            Keyboard.press(key);
            numBeingHoldDownKey++;
            setLogicalState(key,true);
        }
        else
        {
            //Skip KEY release event, if key ALREADY RELEASED
            if(!getLogicalState(key))
                return;

            Keyboard.release(key);
            if (numBeingHoldDownKey) numBeingHoldDownKey--;
            setLogicalState(key,false);

            //Release all Pressed signals for contingencies, When only ESC event
            if (key == KEY_ESC && numBeingHoldDownKey == 0) releaseAllBeingHoldDownKey();
        }
    }

    if(isSerial) //For, Debugging
    {
        if(isActivateKeyEvent)
        {
            uint8_t keycode = TeensyLayout_To_keycode(key);
            Serial.print(F("( After Hijack) Executed KEY "));
            if(event)
            {   Serial.print(F("Press   : ")); print8bitHex(keycode); Serial.println();   }
            else
            {   Serial.print(F("Release : ")); print8bitHex(keycode); Serial.println();   }
        }
        else
        {
            Serial.println(F("( After Hijack) Executed HIJACKED Actions"));
        }
        
        if(isExistHoldingDownKey())
        {
            Serial.print(F("( After Hijack) Now Holding Down KEY : "));
            
            bool notFirst = false;
            for(uint16_t i=0; i<255; i++)
            {
                if(stateLogical[i])
                {   Serial.print(notFirst++ ? "," : ""); print8bitHex(i);   }
            }
            
            Serial.println();
        }
        else
        {
            Serial.println(F("( After Hijack) Now Holding Down KEY : none"));
        }
    }

    isActivateKeyEvent=false;
}





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





void KeyboardHijacker::pressandreleaseKey(int32_t key)
{
    Keyboard.press(key); delay(1);
    Keyboard.release(key); delay(1);

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
void KeyboardHijacker::pressandreleaseKeys(initializer_list<int32_t> keys)
{
    for(int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key); delay(1);
        Keyboard.release(key); delay(1);
    }
    
    delay(11);

    return;
}
void KeyboardHijacker::pressandreleaseShortcutKey(initializer_list<int32_t> keys)
{
    for(int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key); delay(1);
    }
    
    for(int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.release(key); delay(1);
    }
    
    delay(11);

    return;
}
void KeyboardHijacker::pressandreleaseKeys(int32_t* keys, int32_t len)
{
    for(int32_t i=0; i<len; i++)
    {
        if(keys[i] == 0)
            continue;

        Keyboard.press(keys[i]); delay(1);
        Keyboard.release(keys[i]); delay(1);
    }
    
    delay(11);

    return;
}
void KeyboardHijacker::pressandreleaseShortcutKey(int32_t* keys, int32_t len)
{
    for(int32_t i=0; i<len; i++)
    {
        if(keys[i] == 0)
            continue;

        Keyboard.press(keys[i]); delay(1);
    }
    
    for(int32_t i=0; i<len; i++)
    {
        if(keys[i] == 0)
            continue;

        Keyboard.release(keys[i]); delay(1);
    }
    
    delay(11);

    return;
}





void KeyboardHijacker::pressandreleaseKey_LikeHuman(int32_t key)
{
    Keyboard.press(key); randomDelayGenerator();
    Keyboard.release(key); randomDelayGenerator_Manually(15,+10);

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
void KeyboardHijacker::pressandreleaseKeys_LikeHuman(initializer_list<int32_t> keys)
{
    for (int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key); randomDelayGenerator();
        Keyboard.release(key); randomDelayGenerator_Manually(15,+10);
    }

//    for(int32_t i=0; i<len; i++)
//    {
//        if(keys[i] == 0)
//            continue;
//
//        Keyboard.press(keys[i]); randomDelayGenerator();
//        Keyboard.release(keys[i]); randomDelayGenerator_Manually(15,+10);
//    }
//    
//    delay(11);
//
//    if(isKeysDynamic)
//        delete[] keys;

    return;
}
void KeyboardHijacker::pressandreleaseShortcutKey_LikeHuman(initializer_list<int32_t> keys)
{
    for(int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key); randomDelayGenerator();
    }

    for(int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.release(key); randomDelayGenerator_Manually(15,+10);
    }

    return;
}
