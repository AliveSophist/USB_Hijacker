
File textfile;

bool isMacroRecording   = false;
bool isMacroPlaying     = false;

uint32_t numRecorded;
uint32_t numPlayed;

bool byMacro;

uint32_t msLeftUntilNextMacro;





void MODULE_MACRO_PLAYER_OR_RECORDER_START(const char* fname)
{
    if(!isExistSD)
        return;
    if(isMacroRecording)
        return;
    if(isMacroPlaying)
        return;
    
    
    char filename[strlen(fname)+6] = "";
    bool isREADONLY;

    //"TXT" file is WRITABLE.
    //"_R.TXT" file is READONLY, it do not use recording function.
    //"_R.TXT" file has priority over ".TXT" file. Therefore, ".TXT" file is ignored if "_R.TXT" file exist.
    strcpy(filename,fname); strcat(filename,"_R.TXT"); isREADONLY=true;
    if(!SD.exists(filename))
    { strcpy(filename,fname); strcat(filename,".TXT"); isREADONLY=false; }
    
    
    // CHECK "PLAY" or "RECORD" by pressed ms
    uint32_t mspressed = 0;
    while(numDN){ delay(1); mspressed++; }

    // START PLAYER
    if(isREADONLY||mspressed<1600)
    {
        if(!SD.exists(filename))
        {
            //call off playing
            if(isSerial) Serial.println("MODULE_MACRO_PLAYER_START MISFIRE !!! FILE MISSING !!!");
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
        //reeset file
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
            MODULE_MACRO_PLAYER_END();
            return;
        }
    
        //exclude COMMENT
        if(0==readline.indexOf('/'))
            return;

        //To uppercase
        for(uint32_t i=0; i<readline.length(); i++)
        {
            if('a' <= readline[i] && readline[i] <= 'z')
                readline[i] -= 32;
        }
    
        //analyze MacroEvent
        if      (-1 < readline.indexOf("DNUP"))
        {
            int8_t index_0x = readline.indexOf("0X");
            if(index_0x < 4) return;
            
            uint8_t keycodeLatest = x2i(readline.substring(index_0x+2));

            KBD_Hijacker.pressandreleaseKey(keycode_To_TeensyLayout(keycodeLatest));
            
            byMacro=true;
            numPlayed++;
            return;
        }
        else if (-1 < readline.indexOf("DN"))
        {
            int8_t index_0x = readline.indexOf("0X");
            if(index_0x < 2) return;
            
            uint8_t keycodeLatest = x2i(readline.substring(index_0x+2));
    
            //simmilar to OnRawPress(uint8_t keycode)
            {
                KeyLogger.push(keycodeLatest);
                
                key   = keycode_To_TeensyLayout(keycodeLatest);
                event = true;
                
                isExistWaitingEvent_Press = true;
                
                if(isSerial){ Serial.print(F("\n(*MACRO EVENT*) DN ")); PrintKey(keycodeLatest); Serial.println(); }
            }
            
            byMacro=true;
            numPlayed++;
            return;
        }
        else if (-1 < readline.indexOf("UP"))
        {
            int8_t index_0x = readline.indexOf("0X");
            if(index_0x < 2) return;
            
            uint8_t keycodeLatest = x2i(readline.substring(index_0x+2));
    
            //simmilar to OnRawRelease(uint8_t keycode)
            {
                key   = keycode_To_TeensyLayout(keycodeLatest);
                event = false;
                
                isExistWaitingEvent_Release = true;
                
                if(isSerial){ Serial.print(F("\n(*MACRO EVENT*) UP ")); PrintKey(keycodeLatest); Serial.print(F("         Pressed Time : ")); Serial.println(millis()-msLatestEventPressed); }
            }
            
            byMacro=true;
            numPlayed++;
            return;
        }
        else if (-1 < readline.indexOf("TY"))
        {
            int32_t index_Start = readline.indexOf("(\"");
            int32_t index_End = readline.lastIndexOf("\")");
            if(index_Start >= index_End) return;

            String strTyping = readline.substring(index_Start+2,index_End);

            KBD_Hijacker.pressandreleaseKeys(strTyping);
            
            byMacro=true;
            numPlayed++;
            return;
        }
        else
        {
            msLeftUntilNextMacro = d2i(split_findNum(readline));
    
            if(isSerial){ Serial.print(F("                                            Next MacroEvent : ")); Serial.println(msLeftUntilNextMacro); }
            
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

    char buf[str.length()+1]; str.toCharArray(buf,str.length()+1);
    char* s = buf;

    uint32_t x = 0;
    while(*s)
    {
        if ('0' <= *s && *s <= '9'){
            index_Num = x;
            break;
        }
        
        s++; x++;
    }

    if(-1 < index_Num)
    {
        return str.substring(index_Num);
    }
    else
    {
        return "0";
    }
}

uint32_t x2i(String str) 
{
    char buf[str.length()+1]; str.toCharArray(buf,str.length()+1);
    char* s = buf;

    uint32_t x = 0;
    while(true)
    {
        char c = *s;
        
        if ('0' <= c && c <= '9'){
            x *= 16;
            x += c - '0'; 
        } else if ('A' <= c && c <= 'F'){
            x *= 16;
            x += (c - 'A')+10;
        } else if ('a' <= c && c <= 'f'){
            x *= 16;
            x += (c - 'a')+10;
        }
        else break;
        
        s++;
    }
    return x;
}
uint32_t d2i(String str) 
{
    char buf[str.length()+1]; str.toCharArray(buf,str.length()+1);
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
