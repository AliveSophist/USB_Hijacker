
void KeyboardParser::OnRawPress(uint8_t keycode)
{
    KBD_Parser.rawKeycode = keycode;
    
    KBD_Parser.KeyLogger.push(keycode);

    isExistWaitingEvent_Press = true;
    numDN++;

    //MODULE_MACRO
    {
        //if Recording
        MODULE_MACRO_RECORDER_REC_PRESSED(keycode, MILLIS_SINCE_LATEST_EVENT);

        //if Playing
        MODULE_MACRO_PLAYER_SETSTATE_BY_FORCE();
        
        //PREVENT several events when MACRO JUST STARTED
        if(MODULE_MACRO_PREVENT_SEVERAL_EVENTS_JUST_STARTED())
            isExistWaitingEvent_Press = false;
    }

    if(isSerial)
    {
        MEASURE_FREE_MEMORY();
        
        Serial.print(F("\n(Before Hijack) DN ")); KBD_Hijacker.printKeyInfo(keycode);
        Serial.println();
    }
}

void KeyboardParser::OnRawRelease(uint8_t keycode)
{
    KBD_Parser.rawKeycode = keycode;

    isExistWaitingEvent_Release = true;
    numDN--; //if(numDN) numDN-=1;

    //MODULE_MACRO
    {
        //if MACRO is RECORDING
        MODULE_MACRO_RECORDER_REC_RELEASED(keycode, MILLIS_SINCE_LATEST_EVENT);

        //if MACRO is PLAYING
        MODULE_MACRO_PLAYER_SETSTATE_BY_FORCE();

        //PREVENT several events when MACRO JUST STARTED
        if(MODULE_MACRO_PREVENT_SEVERAL_EVENTS_JUST_STARTED())
            isExistWaitingEvent_Release = false;
    }

    if(isSerial)
    {
        MEASURE_FREE_MEMORY();
        
        Serial.print(F("\n(Before Hijack) UP ")); KBD_Hijacker.printKeyInfo(keycode);
        Serial.print(F("         Pressed Time : ")); Serial.println(MILLIS_FROM_PRESSED_UNTIL_RELEASE);
    }
}
