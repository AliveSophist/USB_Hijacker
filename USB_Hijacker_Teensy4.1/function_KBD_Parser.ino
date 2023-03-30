
void KeyboardParser::OnRawPress(uint8_t keycode)
{
    numDN++;

    key   = keycode_To_TeensyLayout(keycode);
    event = true;

    KBD_Parser.isExistWaitingEvent_Press = true;
    KBD_Parser.KeyLogger.push(keycode);

    //MODULE_MACRO
    {
        //if Recording
        MODULE_MACRO_RECORDER_REC_PRESSED(keycode, millis()-msLatestEventCame);

        //if Playing
        MODULE_MACRO_PLAYER_SETSTATE_BY_FORCE();
        
        //PREVENT several events when MACRO is ON STARTING
        if(MODULE_MACRO_PREVENT_SEVERAL_EVENTS_ON_STARTING())
            KBD_Parser.isExistWaitingEvent_Press = false;
    }

    if(isSerial)
    {
        MEASURE_FREE_MEMORY();
        
        Serial.print(F("\n(Before Hijack) DN ")); KBD_Hijacker.printKeyInfo(keycode);
        Serial.println();
    }
    
    msLatestEventCame = msLatestEventPressed = millis();
}

void KeyboardParser::OnRawRelease(uint8_t keycode)
{
    numDN--; //if(numDN) numDN-=1;
    
    key   = keycode_To_TeensyLayout(keycode);
    event = false;
    
    KBD_Parser.isExistWaitingEvent_Release = true;

    //MODULE_MACRO
    {
        //if MACRO is RECORDING
        MODULE_MACRO_RECORDER_REC_RELEASED(keycode, millis()-msLatestEventCame);

        //if MACRO is PLAYING
        MODULE_MACRO_PLAYER_SETSTATE_BY_FORCE();

        //PREVENT several events when MACRO is ON STARTING
        if(MODULE_MACRO_PREVENT_SEVERAL_EVENTS_ON_STARTING())
            KBD_Parser.isExistWaitingEvent_Release = false;
    }

    if(isSerial)
    {
        MEASURE_FREE_MEMORY();
        
        Serial.print(F("\n(Before Hijack) UP ")); KBD_Hijacker.printKeyInfo(keycode);
        Serial.print(F("         Pressed Time : ")); Serial.println(millis()-msLatestEventPressed);
    }
    
    msLatestEventCame = millis();
}
