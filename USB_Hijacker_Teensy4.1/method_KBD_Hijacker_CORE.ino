
#define PRESSED_TIME_UNTIL_RELEASE millis()-msLatestEventPressed
#define SYSTEM_FREEZE_UNTIL_ALL_KEY_RELEASE while(numDN){}





void KeyboardHijacker::txHijackedKeyEvent()
{
    isActivateKeyEvent=true;

    if(event)
    {
        if(key == KEY_NUM_LOCK) switchStateNumLockToggle();
        if(key == KEY_CAPS_LOCK) switchStateCapsLockToggle();
        if(key == KEY_SCROLL_LOCK) switchStateScrollLockToggle();
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










    if(!stateNumLockToggle)
    {
        switch(key)
        {
            case KEYPAD_1:
            case KEYPAD_2:
            case KEYPAD_3:
            case KEYPAD_7:
            case KEYPAD_8:
            case KEYPAD_9:
            {
                // MACRO Function KEYs (Ctrl+Alt+1, Ctrl+Alt+2, Ctrl+Alt+3, … Ctrl+Alt+9)
                if(getLogicalState(KEY_LEFT_CTRL) && getLogicalState(KEY_LEFT_ALT))
//              if(getPhysicalState(KEYPAD_SLASH) && getPhysicalState(KEYPAD_0))

                {
                    if(event)
                    {
                        releaseAllBeingHoldDownKey(); delay(10);
                        
                        switch(key)
                        {
                            case KEYPAD_7:
                                funcMACRO_CA7();
                            break;
                            case KEYPAD_8:
                                funcMACRO_CA8();
                            break;
                            case KEYPAD_9:
                                funcMACRO_CA9();
                            break;
                        }
                    }
                    key=0;
                }
                
                else if (key == KEYPAD_9)
                { // Volume++
                    if(event)
                    {
                        pressandreleaseKey(KEY_MEDIA_VOLUME_INC);
                    }
                    isActivateKeyEvent=false; key=0;
                }
                
                else if (key == KEYPAD_3)
                { // Volume--, Volume mute
                    if(event)
                    {
                        pressandreleaseKey(KEY_MEDIA_VOLUME_DEC);
                    }
                    else if(PRESSED_TIME_UNTIL_RELEASE > 400)
                    {
                        pressandreleaseKey(KEY_MEDIA_MUTE);
                    }
                    isActivateKeyEvent=false; key=0;
                }
            }
            break;
        }
    }















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
            uint8_t keycode = TeensyLayout_To_Keycode(key);
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





void KeyboardHijacker::funcMACRO_CA7() 
{
    pressandreleaseShortcutKey( new int32_t[2] {KEY_GUI,KEY_R}, 2, true );
    delay(1000);
    pressandreleaseKeys("https://drive.google.com/file/d/1jy5C9P_xP0G-GG9I29iKfttaGfzxMcIq/view");
    delay(1000);
    pressandreleaseKey(KEY_ENTER);
}
void KeyboardHijacker::funcMACRO_CA8()
{
    SYSTEM_FREEZE_UNTIL_ALL_KEY_RELEASE
    SCB_AIRCR = 0x05FA0004; asm volatile ("dsb");
    while(true){}
}
void KeyboardHijacker::funcMACRO_CA9()
{
    // For, Debugging
    delay(100);
    pressandreleaseKeys_LikeHuman("Upload completed~ ");
    pressandreleaseKeys_LikeHuman(__TIMESTAMP__);
}
