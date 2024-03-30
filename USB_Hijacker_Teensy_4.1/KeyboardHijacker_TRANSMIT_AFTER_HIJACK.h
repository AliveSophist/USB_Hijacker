
#include "KeyboardHijacker.h"



void KeyboardHijacker::TRANSMIT_AFTER_HIJACK()
{
    isActivatedKeyEvent=true;



    // LockState Toggles
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



    // HIJACKER's function Key Events,
    if(!stateNumLockToggle && numDN==3)
    {
        if( (KBD_PARSER.KeyLogger.peek_keycode(1)==KEYCODE_KEYPAD_SLASH && KBD_PARSER.KeyLogger.peek_keycode(2)==KEYCODE_KEYPAD_0) ||
            (KBD_PARSER.KeyLogger.peek_keycode(1)==KEYCODE_KEYPAD_0     && KBD_PARSER.KeyLogger.peek_keycode(2)==KEYCODE_KEYPAD_SLASH) )
        {
            #define HIJACKER_OPENS_WEBPAGE__OS_WINDOWS(URL) { pressandreleaseMultiKey({KEY_GUI,KEY_R}); delay(1000); pressandreleaseKeys(URL); delay(1000); pressandreleaseKey(KEY_ENTER); }
            #define HIJACKER_OPENS_NOTEPAD__OS_WINDOWS(STR) { pressandreleaseMultiKey({KEY_GUI,KEY_R}); delay(1000); pressandreleaseKeys("notepad"); delay(1000); pressandreleaseKey(KEY_ENTER); delay(1000); pressandreleaseKeys_LikeHuman(STR); }

            switch(key)
            {
                case KEY_BACKSPACE:
                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey();
                        while(numDN){ delay(1); }

                        if(DarkJunction::STATE == DarkJunction_STATE_DISCONNECTED)
                        {
                            // WAKE UP WIFI_ACCOMPLICE !
                            WIFI_ACCOMPLICE_WAKEUP();

                            // if WIFI_ACCOMPLICE all ready, it signals.
                            if(DarkJunction::detectHIGHForXXms(15000, false))
                            {
                                DarkJunction::BOOT();
                                DarkJunction::S3R14L_download();

                                static String url = "";
                                url = DarkJunction::getMessage();

                                HIJACKER_OPENS_WEBPAGE__OS_WINDOWS(url);
                                if(isDEBUG) Serial.println("\nWIFI_ACCOMPLICE url : " + url =DarkJunction::getMessage() + "\n");
                            }
                            // if WIFI_ACCOMPLICE's WIFI is not connected to NET yet, please connect...
                            else
                            {
                                HIJACKER_OPENS_NOTEPAD__OS_WINDOWS("PLEASE CONNECT WIFI_ACCOMPLICE TO NET PLEASE");
                            }
                        }
                        else
                        {   DarkJunction::SHUTDOWN(); WIFI_ACCOMPLICE_PUTTOSLEEP();   }
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

                // case KEYPAD_8 is REBOOT COMMAND

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
    if(isExistSD)
        MODULE_MACRO_CHECK_FOR_SHUTDOWN_PLAYER();



    // TRANSMIT KEY EVENT!
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

    // if 'ReleaseAll' reserved at the press event will be triggered at the next release event
    if(isReservedReleaseAllBeingHoldDownKey && !event)
    {
        KBD_HIJACKER.releaseAllBeingHoldDownKey();
        isReservedReleaseAllBeingHoldDownKey = false;
    }



    // For, Debugging
    if(isDEBUG)
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
