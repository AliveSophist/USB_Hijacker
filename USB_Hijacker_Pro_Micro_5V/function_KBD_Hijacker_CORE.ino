
#define PRESSED_TIME_UNTIL_RELEASE millis()-msLatestEventPressed





void KeyboardHijacker::txHijackedKeyEvent()
{
    isActivateKeyEvent=true;

    if(event)
    {
        if(key == KEY_NUM_LOCK) switchStateNumLockToggle();
        if(key == KEY_CAPS_LOCK) switchStateCapsLockToggle();
        if(key == KEY_SCROLL_LOCK) switchStateScrollLockToggle();
    }


    //KOREAN_KEYPAD_EVOLUTION_v2.0 Edition by SOPHIST
    MODULE_KOREAN_KEYPAD_EVOLUTION();










    if(!stateNumLockToggle)
    {
        if (key == KEYPAD_DIVIDE)
        {
            key = KEY_LEFT_CTRL;
        }

        if (key == KEYPAD_MULTIPLY)
        {
            key = KEY_ESC;
        }

        if (key == KEYPAD_0)
        {
            key = KEY_LEFT_ALT;
        }
        
        if(KEYPAD_1<=key && key<=KEYPAD_9)
        {
            if(getLogicalState(KEY_LEFT_CTRL) && !(KEYPAD_4<=key && key<=KEYPAD_6))
            { // MACRO Function KEYs (Ctrl+1, Ctrl+2, Ctrl+3, … Ctrl+9)
                if(event)
                {
                    releaseAllBeingHoldDownKey();
                    delay(100);     pressandreleaseKey(KEY_ESC);
                    
                    switch(key)
                    {
                        case KEYPAD_1:
                            delay(100);     pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_GUI,KEY_R} );
                            delay(1000);    pressandreleaseKeys("notepad");
                            delay(100);     pressandreleaseKey(KEY_ENTER);
                            break;
                        case KEYPAD_2:
                            delay(100);     pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_GUI,KEY_R} );
                            delay(1000);    pressandreleaseKeys("calc");
                            delay(100);     pressandreleaseKey(KEY_ENTER);
                            break;
                        case KEYPAD_3:
                            delay(100);     pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_GUI,KEY_R} );
                            delay(1000);    pressandreleaseKeys("mspaint");
                            delay(100);     pressandreleaseKey(KEY_ENTER);
                            break;
//                        case KEYPAD_4:
//                            break;
//                        case KEYPAD_5:
//                            break;
//                        case KEYPAD_6:
//                            break;
                        case KEYPAD_7:
                            delay(100);     pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_GUI,KEY_R} );
                            delay(1000);    pressandreleaseKeys("http://www.google.com");
                            delay(100);     pressandreleaseKey(KEY_ENTER);
                            break;
                        case KEYPAD_8:
                            delay(100);     pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_GUI,KEY_R} );
                            delay(1000);    pressandreleaseKeys("http://www.naver.com");
                            delay(100);     pressandreleaseKey(KEY_ENTER);
                            break;
                        case KEYPAD_9:
                            delay(100);     pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_GUI,KEY_R} );
                            delay(1000);    pressandreleaseKeys("http://www.youtube.com");
                            delay(100);     pressandreleaseKey(KEY_ENTER);
                            break;
                    }
                    KBD_Parser.freezeSystemUntilAllKeyRelease();
                }
                isActivateKeyEvent=false; key=0;
            }
            
            else if(getLogicalState(KEY_LEFT_ALT))
            { // Multipurpose Shortcut KEYs (Alt+1, Alt+2, Alt+3, … Alt+9)
                if(event)
                {
                    releaseAllBeingHoldDownKey();
                    delay(100);     pressandreleaseKey(KEY_ESC);
                    
                    switch(key)
                    {
                        case KEYPAD_1:
                            delay(100);     pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_GUI,KEY_R} );
                            delay(1000);    pressandreleaseKeys("notepad");
                            delay(100);     pressandreleaseKey(KEY_ENTER);
                            delay(2000);    pressandreleaseKeys("I appreciate for your kindness, Si-eun Choi ^^/ 22.05.23");
                            break;
                        case KEYPAD_2:
                            
                            break;
                        case KEYPAD_3:
                            
                            break;
                        case KEYPAD_4:
                            Consumer.write(MEDIA_PREVIOUS);
                            break;
                        case KEYPAD_5:
                            Consumer.write(MEDIA_PLAY_PAUSE);
                            break;
                        case KEYPAD_6:
                            Consumer.write(MEDIA_NEXT);
                            break;
                        case KEYPAD_7:
                            
                            break;
                        case KEYPAD_8:
                            
                            break;
                        case KEYPAD_9:
                            
                            break;
                    }
                    KBD_Parser.freezeSystemUntilAllKeyRelease();
                }
                isActivateKeyEvent=false; key=0;
            }
            
            else if (key == KEYPAD_5)
            {
                key = KEY_SPACE;
            }
            
            else if (key == KEYPAD_9)
            { // Volume++
                if(event)
                {
                    Consumer.write(MEDIA_VOLUME_UP);
                    //Consumer API https://github.com/NicoHood/HID/wiki/Consumer-API
                }
                isActivateKeyEvent=false; key=0;
            }
            
            else if (key == KEYPAD_3)
            { // Volume--, Volume mute
                if(event)
                {
                    Consumer.write(MEDIA_VOL_DOWN);
                }
                else if(PRESSED_TIME_UNTIL_RELEASE > 400)
                {
                    Consumer.write(MEDIA_VOL_MUTE);
                }
                isActivateKeyEvent=false; key=0;
            }
        }
    }













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
            //Skip KEY release event, if key already released by 'releaseAllBeingHoldDownKey()'
            if(!getLogicalState(key))
                return;

            Keyboard.release(key);
            if (numBeingHoldDownKey)numBeingHoldDownKey--;
            setLogicalState(key,false);

            //Release all Pressed signals for contingenciesㅡ
            if (key == KEY_ENTER || key == KEYPAD_ENTER) { releaseAllBeingHoldDownKey(); }
        }
    }
    isActivateKeyEvent=false;
}
