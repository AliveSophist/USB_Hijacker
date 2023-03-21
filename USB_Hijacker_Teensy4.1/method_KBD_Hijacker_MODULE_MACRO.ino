
bool isMacroRecording   = false;
bool isMacroPlaying     = false;

bool isMacroOnStarting = false;

uint32_t numRecorded;
uint32_t numPlayed;

bool byMacro;

uint32_t msLeftUntilNextMacro;

//static std::map<String, uint8_t> MAP_CUSTOM_DELAY;





void MODULE_MACRO_PLAYER_OR_RECORDER_START(const char* fname)
{
    if(!isExistSD)
        return;
    if(isMacroRecording)
        return;
    if(isMacroPlaying)
        return;
    
    
    char filename[strlen(fname)+6] = ""; strcpy(filename,fname);
    bool isREADONLY=true;

    //"TXT" file is WRITABLE.
    //"_R.TXT" file is READONLY, it do not use recording function.
    if(String(filename).indexOf("_R") < 0)
        strcat(filename,"_R");
        
    strcat(filename,".TXT");

    // CHECK READONLY
    // "_R.TXT" file has priority over ".TXT" file. Therefore ".TXT" file is ignored if "_R.TXT" file exist.
    if(!SD.exists(filename))
    { strcpy(filename,fname); strcat(filename,".TXT"); isREADONLY=false; }
    
    // CHECK "PLAY" or "RECORD" by pressed ms, when NOT READONLY
    uint32_t mspressed = 0;
    if(!isREADONLY)
        while(numDN){ delay(1); mspressed++; }
    else
        isMacroOnStarting = true;
    

    // START PLAYER
    if(isREADONLY||mspressed<1600)
    {
        if(!SD.exists(filename))
        {
            //shutdown playing
            if(isSerial) Serial.println("\n!!! MODULE_MACRO_PLAYER_START MISFIRE !!! FILE MISSING !!!");
            return;
        }
        
        if(isSerial) MODULE_MACRO_PRINT(filename); //For, Debugging
        
        textfile = SD.open(filename);
        
        isMacroPlaying=true; numPlayed=0; msLeftUntilNextMacro=0;
        if(isSerial) Serial.println("MODULE_MACRO_PLAYER_START");
    }
    
    // START RECORDER
    else
    {
        //reset file
        if(SD.exists(filename)) SD.remove(filename);
        textfile = SD.open(filename,FILE_WRITE);
        
        Buzzzzer.reserveBuzz(   new uint16_t[11] {  NOTE_E5,0,  NOTE_G5,0,  NOTE_E6,0,  NOTE_C6,0,  NOTE_D6,0,  NOTE_G6 },
                                new uint16_t[11] {  170,20,     170,20,     170,20,     170,20,     170,20,     200     },
                                11  );
        
        isMacroRecording=true; numRecorded=0;
        if(isSerial) Serial.println("MODULE_MACRO_RECORDER_START");
    }
}





void MODULE_MACRO_RECORDER_REC_PRESSED(uint8_t keycode, uint32_t delayed)
{
    if(!isExistSD)
        return;
    if(!isMacroRecording)
        return;


    if(keycode == TeensyLayout_To_Keycode(KEY_NUM_LOCK))
        if(keycode == KeyLogger.peek_keycode(1) && keycode == KeyLogger.peek_keycode(2))
        {
            textfile.println("1ms");
            MODULE_MACRO_RECORDER_END(textfile.name());
        }

    if(numRecorded) //if numRecorded==0 DO NOT RECORD DELAY, only record "DN 0x##"
    {
        textfile.print(String(delayed, DEC)); textfile.println("ms");
        if(isSerial) Serial.println("MODULE_MACRO_RECORDER_REC_DELAY");
        numRecorded++;
    }
    
    textfile.print("DN 0x"); if(keycode<16)textfile.print('0'); textfile.println(String(keycode, HEX));
    if(isSerial) Serial.println("MODULE_MACRO_RECORDER_REC_DNKEY");
    numRecorded++;
}
void MODULE_MACRO_RECORDER_REC_RELEASED(uint8_t keycode, uint32_t delayed)
{
    if(!isExistSD)
        return;
    if(!isMacroRecording)
        return;


    textfile.print(String(delayed, DEC)); textfile.println("ms");
    if(isSerial) Serial.println("MODULE_MACRO_RECORDER_REC_DELAY");
    numRecorded++;
    
    textfile.print("UP 0x"); if(keycode<16)textfile.print('0'); textfile.println(String(keycode, HEX));
    if(isSerial) Serial.println("MODULE_MACRO_RECORDER_REC_UPKEY");
    numRecorded++;
}
void MODULE_MACRO_RECORDER_END(const char* filename)
{
    if(!isExistSD)
        return;
    if(!isMacroRecording)
        return;


    textfile.close();


    //Copy textfile to bakfile
    if(SD.exists("temp.bak")) SD.remove("temp.bak");
    File bakfile = SD.open("temp.bak",FILE_WRITE);
    textfile = SD.open(filename);
    
    uint32_t numValidLine=0;
    while(textfile.available())
    {
        String str = textfile.readStringUntil('\n');
        bakfile.println(str.substring(0,str.lastIndexOf('\r')));
        
        numValidLine++;
    }
    textfile.close();
    bakfile.close();
    SD.remove(filename);


    //Copy bakfile to textfile, except for the MODULE_RECORDER's shutdown signal
    bakfile = SD.open("temp.bak");
    textfile = SD.open(filename,FILE_WRITE);

    if(numValidLine>9)
    {
        numValidLine -= 9;
        for(uint32_t i=0; i<numValidLine; i++)
        {
            String str = bakfile.readStringUntil('\n');
            textfile.println(str.substring(0,str.lastIndexOf('\r')));
        }
    }
    bakfile.close();
    textfile.close();
    SD.remove("temp.bak");
    
    if(isSerial) MODULE_MACRO_PRINT(filename); //For, Debugging


    KBD_Hijacker.releaseAllBeingHoldDownKey();
                                        
    Buzzzzer.reserveBuzz(   new uint16_t[11] {  NOTE_A4,    NOTE_E4,    NOTE_A3,0,  NOTE_A4,    NOTE_E4,    NOTE_A3,0,  NOTE_A4,    NOTE_E4,    NOTE_A3 },
                            new uint16_t[11] {  80,         80,         80,150,     80,         80,         80,150,     80,         80,         80      },
                            11  );
    
    isMacroRecording=false;
    if(isSerial){ Serial.print("MODULE_MACRO_RECORDER_END    RECORDED LINES : "); Serial.println(numRecorded); }
}





void MODULE_MACRO_PLAYER_ONGOING()
{
    if(!isExistSD)
        return;
    if(!isMacroPlaying)
        return;

    if(msLeftUntilNextMacro)
    {
        msLeftUntilNextMacro--;
        return;
    }


    String readline;

    while(true)
    {
        if(textfile.available())
        {
            readline = textfile.readStringUntil('\n');
        }
        else
        {
            if(isSerial) Serial.println();
            
            MODULE_MACRO_PLAYER_END();
            return;
        }
    
        // Exclude COMMENT
        if(0==readline.indexOf('/'))
            return;

        // To uppercase
        for(uint32_t i=0; i<readline.length(); i++)
        {
            if('a' <= readline[i] && readline[i] <= 'z')
                readline[i] -= 32;

            //ignore 'TY ("")' inner strings
            if(readline[i] == '\"')
                break;
        }

        // Check readline
        //if(isSerial){ Serial.print("\nREADLINE:"); Serial.println(readline); }

        // Analyze MacroEvent
        if(-1 < readline.indexOf("DNUP"))
        {
            uint8_t keycode = String_To_keycode(readline);
            
            if(keycode!=0)
            {
                // OnRawPress By Macro
                key   = keycode_To_TeensyLayout(keycode);
                event = true;
                
                KeyLogger.push(keycode);
                
                if(isSerial){ Serial.print(F("\n(*MACRO EVENT*) DN ")); PrintKey(keycode); Serial.println(); }


                // txHijackedKeyEvent By Macro
                msLatestEventCame = msLatestEventPressed = millis();
                KBD_Hijacker.txHijackedKeyEvent();

                delay(11);
                
                // OnRawRelease By Macro
                key   = keycode_To_TeensyLayout(keycode);
                event = false;

                if(isSerial){ Serial.print(F("(*MACRO EVENT*) UP ")); PrintKey(keycode); Serial.print(F("         Pressed Time : ")); Serial.println(millis()-msLatestEventPressed); }


                // txHijackedKeyEvent By Macro
                msLatestEventCame = millis();
                KBD_Hijacker.txHijackedKeyEvent();
            }

            byMacro=true;
            numPlayed++;
            continue;
        }
        else if(-1 < readline.indexOf("DN"))
        {
            uint8_t keycode = String_To_keycode(readline);
            
            if(keycode!=0)
            {
                // OnRawPress By Macro
                key   = keycode_To_TeensyLayout(keycode);
                event = true;
                
                KeyLogger.push(keycode);
                
                if(isSerial){ Serial.print(F("\n(*MACRO EVENT*) DN ")); PrintKey(keycode); Serial.println(); }


                // txHijackedKeyEvent By Macro
                msLatestEventCame = msLatestEventPressed = millis();
                KBD_Hijacker.txHijackedKeyEvent();
            }
            
            byMacro=true;
            numPlayed++;
            continue;
        }
        else if(-1 < readline.indexOf("UP"))
        {
            uint8_t keycode = String_To_keycode(readline);
            
            if(keycode!=0)
            {
                // OnRawRelease By Macro
                key   = keycode_To_TeensyLayout(keycode);
                event = false;
                
                if(isSerial){ Serial.print(F("\n(*MACRO EVENT*) UP ")); PrintKey(keycode); Serial.print(F("         Pressed Time : ")); Serial.println(millis()-msLatestEventPressed); }


                // txHijackedKeyEvent By Macro
                msLatestEventCame = millis();
                KBD_Hijacker.txHijackedKeyEvent();
            }

            byMacro=true;
            numPlayed++;
            continue;
        }
        else if (-1 < readline.indexOf("TY"))
        {
            int32_t index_Start = readline.indexOf("\"");
            int32_t index_End = readline.lastIndexOf("\"");
            if(index_Start<0||index_Start>=index_End) return;

            String strTyping = readline.substring(index_Start+1,index_End);

            KBD_Hijacker.pressandreleaseKeys(strTyping);

            if(isSerial){ Serial.print(F("\n(*MACRO EVENT*) TYPE : ")); Serial.println(strTyping); }
            
            byMacro=true;
            numPlayed++;
            continue;
        }
        else
        {
            msLeftUntilNextMacro = StringDec_To_int(split_findNum(readline));
            
            if(msLeftUntilNextMacro == 0)
                continue;

            if(isSerial){ Serial.print(F("                                            Next MacroEvent lefts : ")); Serial.println(msLeftUntilNextMacro); }

            byMacro=true;
            numPlayed++;
            return;
        }
    }
}
void MODULE_MACRO_PLAYER_END()
{
    if(!isExistSD)
        return;
    if(!isMacroPlaying)
        return;


    textfile.close();

    KBD_Hijacker.releaseAllBeingHoldDownKey();
    
    isMacroPlaying=false; byMacro=false;
    if(isSerial){ Serial.print("\nMODULE_MACRO_PLAYER_END    PLAYED LINES : "); Serial.println(numPlayed); Serial.println(); }
}
bool MODULE_MACRO_PREVENT_SEVERAL_EVENTS_ON_STARTING()
{
    if(isMacroOnStarting)
    {
        if(numDN == 0)
            isMacroOnStarting = false;
       
        return true;
    }

    return false;
}
void MODULE_MACRO_PLAYER_SHUTDOWN_ARBITER()
{
    if(!isExistSD)
        return;
    if(!isMacroPlaying)
        return;


    if(byMacro)
        return;


    // KEY_ESC PUSHED BY FORCE, SHUTDOWN MACRO!!
    if (key == KEY_ESC)
    {
        if(event)
        {
            MODULE_MACRO_PLAYER_END();
        }
        isActivateKeyEvent=false; key=0;
    }
}
void MODULE_MACRO_PLAYER_SETSTATE_BY_FORCE()
{
    if(!isExistSD)
        return;
    if(!isMacroPlaying)
        return;


    byMacro=false;
}





String split_findNum(String str)
{
    int8_t index_Num = -1;
    uint32_t lastIndex_Num = 0;
    
    for(uint32_t i=0; i<str.length(); i++)
    {
        if(index_Num == -1){
            if ('0' <= str[i] && str[i] <= '9'){
                index_Num = i;
            }
        }
        else{
            if (!('0' <= str[i] && str[i] <= '9')){
                lastIndex_Num = i;
                break;
            }
        }
    }

    if(-1 < index_Num) return str.substring(index_Num, lastIndex_Num);
    
    return "0"; // Invalid str !!
}

uint32_t StringDec_To_int(String str) 
{
    char buf[str.length()+1];
    str.toCharArray(buf,str.length()+1);
    
    char* s = buf;
    uint32_t x = 0;
    while(true)
    {
        char c = *s;
        
        if ('0' <= c && c <= '9'){
            x *= 10;
            x += c - '0'; 
        }
        else break;
        
        s++;
    }
    return x;
}





void MODULE_MACRO_PRINT(const char* filename) //For, Debugging
{
    if(!isSerial) return;
    
    textfile = SD.open(filename);
    
    Serial.println();
    Serial.println("\n++++++++++++++++++++++[ Now Printing ]++++++++++++++++++++++");
    while(textfile.available())
    {
        String line = textfile.readStringUntil('\n');
        Serial.println(line);
    }
    Serial.println("++++++++++++++++++++++++[ Complete ]++++++++++++++++++++++++\n");

    textfile.close();
}
