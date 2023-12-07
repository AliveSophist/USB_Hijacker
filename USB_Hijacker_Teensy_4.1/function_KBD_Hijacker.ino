
void KeyboardHijacker::TRANSMIT_AFTER_HIJACK()
{
    isActivatedKeyEvent=true;



    // LockState Keys,
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



    // HIJACKER's function Keys,
    if(!stateNumLockToggle && numDN==3)
    {
        if( (KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_SLASH && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_0) ||
            (KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_0 && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_SLASH) )
        {
            #define HIJACKER_OPENS_WEBPAGE__OS_WINDOWS(URL) { pressandreleaseMultiKey({KEY_GUI,KEY_R}); delay(1000); pressandreleaseKeys(URL); delay(1000); pressandreleaseKey(KEY_ENTER); }
            #define HIJACKER_OPENS_NOTEPAD__OS_WINDOWS(STR) { pressandreleaseMultiKey({KEY_GUI,KEY_R}); delay(1000); pressandreleaseKeys("notepad"); delay(1000); pressandreleaseKey(KEY_ENTER); delay(1000); pressandreleaseKeys_LikeHuman(STR); }

            switch(key)
            {
                case KEY_BACKSPACE:
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);

                        // WAKE UP ACCOMPLICE !
                        {
                            digitalWrite(PIN_WAKEUP_ESP,HIGH);  delay(100);
                            digitalWrite(PIN_WAKEUP_ESP,LOW);   delay(100);
                            DarkJunction::PIN_BIDIRECTIONAL_writeHIGHForXXms(1000);
                        }

                        // if ACCOMPLICE all ready, it signals.
                        if(DarkJunction::PIN_BIDIRECTIONAL_readForXXms(10000))
                        {
                            HIJACKER_OPENS_WEBPAGE__OS_WINDOWS("http://accomplice.local");
                        }
                        // if the WIFI is not connected yet, please connect
                        else
                        {
                            HIJACKER_OPENS_NOTEPAD__OS_WINDOWS("PLEASE CONNECT ACCOMPLICE TO NET PLEASE");
                        }
                    }
                    isActivatedKeyEvent=false; key=KEY_NONE;
                }
                break;

                case KEYPAD_MINUS:
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);

                        ;
                    }
                    isActivatedKeyEvent=false; key=KEY_NONE;
                }
                break;

                case KEYPAD_PLUS:
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);

                        ;
                    }
                    isActivatedKeyEvent=false; key=KEY_NONE;
                }
                break;

                case KEYPAD_7:
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);

                        HIJACKER_OPENS_WEBPAGE__OS_WINDOWS("https://drive.google.com/file/d/1jy5C9P_xP0G-GG9I29iKfttaGfzxMcIq/view");
                    }
                    isActivatedKeyEvent=false; key=KEY_NONE;
                }
                break;

                // case KEYPAD_8:
                // {
                //     // REBOOT COMMAND
                // }
                // break;

                case KEYPAD_9:
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);

                        HIJACKER_OPENS_NOTEPAD__OS_WINDOWS("Upload completed~ " + String(__TIMESTAMP__));
                    }
                    isActivatedKeyEvent=false; key=KEY_NONE;
                }
                break;
            }
        }
    }



    // KOREAN_KEYPAD_EVOLUTION_v2.0 Edition by SOPHIST
    MODULE_KOREAN_KEYPAD_EVOLUTION();



    // If mapped?, HIJACK!
    MODULE_KEYMAPPER_HIJACK();



    // KEY_ESC CAN IMMEDIATELY SHUTDOWN THE CURRENTLY PLAYING MACRO
    MODULE_MACRO_CHECK_FOR_SHUTDOWN_PLAYER();



    // TTRANSMIT KEY EVENT!
    if(isActivatedKeyEvent)
    {
        if(event)
        {
            if(!getLogicalState(key))   // Skip KEY press event, if key ALREADY PRESSED
            {
                Keyboard.press(key);
                numBeingHoldDownKey++;
                setLogicalState(key,true);
            }
        }
        else
        {
            if(getLogicalState(key))    // Skip KEY release event, if key ALREADY RELEASED
            {
                Keyboard.release(key);
                if (numBeingHoldDownKey) numBeingHoldDownKey--;
                setLogicalState(key,false);
            }
        }
    }

    if(isReservedReleaseAllBeingHoldDownKey && !event)
    {
        KBD_HIJACKER.releaseAllBeingHoldDownKey();
        isReservedReleaseAllBeingHoldDownKey = false;
    }



    // For, Debugging
    if(isSerial)
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

    MODULE_KEYMAPPER_SHUTDOWN_RAPIDFIRE();
        
    if(isSerial){ Serial.println(F(">>>>>>>> ALL KEYs ARE RELEASED !!! <<<<<<<<")); }

    return;
}





void KeyboardHijacker::pressandreleaseKey(int32_t key)
{
    Keyboard.press(key); delay(5);
    Keyboard.release(key); delay(5);

    return;
}
void KeyboardHijacker::pressandreleaseKeys(String str)
{
    for(uint32_t i=0; i<str.length(); i++)
    {
        Keyboard.press(str.charAt(i)); delay(5);
        Keyboard.release(str.charAt(i)); delay(5);
    }

    return;
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

    return;
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

    return;
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

    return;
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
    for(uint32_t i=0; i<str.length(); i++)
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
