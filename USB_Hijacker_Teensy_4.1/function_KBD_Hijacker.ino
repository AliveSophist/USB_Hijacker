
void KeyboardHijacker::TRANSMIT_AFTER_HIJACK()
{
    isActivatedKeyEvent=true;



    if(key == KEY_NUM_LOCK || key == KEY_CAPS_LOCK || key == KEY_SCROLL_LOCK)
    {
        if(event)
        {
            if(key == KEY_NUM_LOCK) switchStateNumLockToggle();
            if(key == KEY_CAPS_LOCK) switchStateCapsLockToggle();
            if(key == KEY_SCROLL_LOCK) switchStateScrollLockToggle();
        }
        else
        {
            // SYNCHRONIZE ToggleKeyStates after a few ms
            reserveSyncToggleKeyStates();
        }
    }



    if(!stateNumLockToggle)
    {
        if  (   numDN == 3
                && (KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_0 || KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_SLASH)
            )
        {
            if  (   (KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_0 && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_SLASH)
                    || (KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_SLASH && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_0)
                )
            {
                if(key == KEYPAD_7)
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);
            
                        pressandreleaseMultiKey( {KEY_GUI,KEY_R} );
                        delay(1000);
                        pressandreleaseKeys("https://drive.google.com/file/d/1jy5C9P_xP0G-GG9I29iKfttaGfzxMcIq/view");
                        delay(1000);
                        pressandreleaseKey(KEY_ENTER);
                    }
                    isActivatedKeyEvent=false; key=0;
                }

                if(key == KEYPAD_8)
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);
                        
                        while(numDN){}
                        REBOOT();
                    }
                    isActivatedKeyEvent=false; key=0;
                }

                if(key == KEYPAD_9)
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);
                        
                        pressandreleaseKeys_LikeHuman(F("Upload completed~ "));
                        pressandreleaseKeys_LikeHuman(__TIMESTAMP__);
                    }
                    isActivatedKeyEvent=false; key=0;
                }
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
            if(event){
                releaseAllBeingHoldDownKey();   //To prevent INTERFERENCE, if press/release needed
                …
                …
            }
            isActivatedKeyEvent=false; key=0;    //To prevent default event
        }


    *  Example4)
    *          Hijack Shortcut TRIGGER EVENT to NONE And excute MACRO

        if(key == KEY_*** && getLogicalState(KEY_$$$))
        {
            if(event){
                releaseAllBeingHoldDownKey();   //To prevent INTERFERENCE, if press/release needed
                …
                …
            }
            isActivatedKeyEvent=false; key=0;    //To prevent default event
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





    // KEY_ESC CAN SHUTDOWN MACRO IMMEDIATELY
    MODULE_MACRO_CHECK_FOR_SHUTDOWN_PLAYER();



    if(isActivatedKeyEvent)
    {
        if(event)
        {
            if(!getLogicalState(key))    //Skip KEY press event, if key ALREADY PRESSED
            {
                Keyboard.press(key);
                numBeingHoldDownKey++;
                setLogicalState(key,true);
            }
        }
        else
        {
            if(getLogicalState(key))   //Skip KEY release event, if key ALREADY RELEASED
            {
                Keyboard.release(key);
                if (numBeingHoldDownKey) numBeingHoldDownKey--;
                setLogicalState(key,false);
            }
        }
    }

    if(isSerial) //For, Debugging
    {
        if(isActivatedKeyEvent)
        {
            uint8_t keycode = TeensyLayout_To_keycode(key);
            if(event)
            {   Serial.print(F("( After Hijack) Executed KEY Press   : ")); print8bitHex(keycode); Serial.println();   }
            else
            {   Serial.print(F("( After Hijack) Executed KEY Release : ")); print8bitHex(keycode); Serial.println();   }
        }
        else
        {
            Serial.println(F("( After Hijack) Executed HIJACKED Actions"));
        }
        
        if(isExistHoldingDownKey())
        {
            Serial.print(F("( After Hijack) Now Holding Down KEY : "));
            
            uint8_t notFirst = 0;
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



    // If CLEANSE reserved?, CLEANSE!
    MODULE_KEYMAPPER_CLEANSE_IF_RESERVED();



    isActivatedKeyEvent=false;
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
        stateNumLockToggle = true;
        KBD_PARSER.numLock(true);
    }
    else                                //if HOST 's NumLock state is OFF
    {
        stateNumLockToggle = false;
        KBD_PARSER.numLock(false);
    }
    
    if (stateLEDs & LED_CAPS_LOCK)      //if HOST 's CapsLock state is ON
    {
        stateCapsLockToggle = true;
        KBD_PARSER.capsLock(true);
    }
    else                                //if HOST 's CapsLock state is OFF
    {
        stateCapsLockToggle = false;
        KBD_PARSER.capsLock(false);
    }
    
    if (stateLEDs & LED_SCROLL_LOCK)    //if HOST 's ScrollLock state is ON
    {
        stateScrollLockToggle = true;
        KBD_PARSER.scrollLock(true);
    }
    else                                //if HOST 's ScrollLock state is OFF
    {
        stateScrollLockToggle = false;
        KBD_PARSER.scrollLock(false);
    }
    
    return;
}

void KeyboardHijacker::releaseAllBeingHoldDownKey()
{
    //Keyboard.releaseAll();
    
    numBeingHoldDownKey = 0;
    
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
void KeyboardHijacker::pressandreleaseKeys(std::initializer_list<int32_t> keys)
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
void KeyboardHijacker::pressandreleaseMultiKey(std::initializer_list<int32_t> keys)
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
void KeyboardHijacker::pressandreleaseMultiKey(int32_t* keys, int32_t len)
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
void KeyboardHijacker::pressandreleaseKeys_LikeHuman(std::initializer_list<int32_t> keys)
{
    for (int32_t key : keys)
    {
        if(key == 0)
            continue;

        Keyboard.press(key); randomDelayGenerator();
        Keyboard.release(key); randomDelayGenerator_Manually(15,+10);
    }

    return;
}
void KeyboardHijacker::pressandreleaseMultiKey_LikeHuman(std::initializer_list<int32_t> keys)
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
