
bool isMacroRecording   = false;
bool isMacroPlaying     = false;

uint32_t numRecorded, numPlayed;
uint32_t msLeftUntilNextMacro;

bool isReadonlyStarted  = false;
bool isEventbyMacro     = false;

std::map<String, std::list<String>> MAP_MINI_MACRO;
std::list<String> QUEUE_NOW_MINI_MACRO;





void KeyboardHijacker::MODULE_MACRO_START_PLAYER_OR_RECORDER(const char* fname)
{
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


    // there is a difference in START TRIGGER between READONLY and NON-READONLY
    // if READONLY, START TRIGGER is pressed event. (IMMEDIATELY! without release event)
    // or NOT,      START TRIGGER is release event. (to select "PLAY" or "RECORD" based on the msPressed)
    uint32_t msPressed = 0;
    if(isREADONLY)
        isReadonlyStarted = true;
    else
        while(numDN){ delay(1); msPressed++; }


    // START PLAYER
    if(isREADONLY||msPressed<1600)
    {
        if(!SD.exists(filename)) // invalid file
        {   if(isSerial) Serial.println("\n!!! MODULE_MACRO_PLAYER_START MISFIRE !!! FILE MISSING !!!"); return;   }


        if(isSerial) MODULE_MACRO_PRINT(filename); // For, Debugging

        textfile = SD.open(filename);

        isMacroPlaying=true; numPlayed=0; msLeftUntilNextMacro=0;
        MAP_MINI_MACRO.clear();
        if(isSerial) Serial.println("MODULE_MACRO_PLAYER_START");
    }
    
    // START RECORDER
    else
    {
        //reset file
        if(SD.exists(filename)) SD.remove(filename);
        textfile = SD.open(filename,FILE_WRITE);

        Buzzzzer::reserveBuzz   ( { NOTE_E5,0,  NOTE_G5,0,  NOTE_E6,0,  NOTE_C6,0,  NOTE_D6,0,  NOTE_G6 }
                                , { 170,20,     170,20,     170,20,     170,20,     170,20,     200     } );

        isMacroRecording=true; numRecorded=0;
        if(isSerial) Serial.println("MODULE_MACRO_RECORDER_START");
    }


    // PREVENT entanglement of KEY states
    KBD_HIJACKER.releaseAllBeingHoldDownKey();
}

void KeyboardHijacker::MODULE_MACRO_BLOCK_SEVERAL_EVENTS_WHEN_READONLY_STARTED(volatile bool *flag)
{
    if(!isReadonlyStarted)
        return;


    *flag = false;

    if(numDN == 0)
        isReadonlyStarted = false;
}





void KeyboardHijacker::MODULE_MACRO_PROCEED_RECORDER(uint8_t keycode, uint32_t delayed, bool isPressed)
{
    if(!isMacroRecording)
        return;


    //MODULE_RECORDER's shutdown signal (KEY_NUMLOCK x3 press&release)
    if  (   keycode == KEYCODE_KEY_NUM_LOCK &&
            keycode == KBD_PARSER.KeyLogger.peek_keycode(1)  &&
            keycode == KBD_PARSER.KeyLogger.peek_keycode(2)
        )
        MODULE_MACRO_END_RECORDER( String(textfile.name()).c_str() ); //this textfile will remove, therefore textfile.name() have to copy!


    // RECORD DELAY
    if(numRecorded!=0)
    {
        textfile.print(String(delayed, DEC)); textfile.println("ms");
        if(isSerial) Serial.println("MODULE_MACRO_REC_DELAY");
        numRecorded++;
    }


    // RECORD EVENT
    String strDNUP = isPressed ? "DN" : "UP";
    textfile.print(strDNUP+" 0x"); if(keycode<16)textfile.print('0'); textfile.println(String(keycode, HEX));
    if(isSerial) Serial.println("MODULE_MACRO_REC_"+strDNUP+"KEY");
    numRecorded++;
}

void KeyboardHijacker::MODULE_MACRO_END_RECORDER(const char* filename)
{
    textfile.close();

    File bakfile;
    uint32_t numValidLine=0;


    //Copy textfile to bakfile
    {
        //reset file
        if(SD.exists("temp.bak")) SD.remove("temp.bak");

        bakfile = SD.open("temp.bak",FILE_WRITE);
        textfile = SD.open(filename);
    
        uint32_t i=0;
        while(textfile.available())
        {
            String line = textfile.readStringUntil('\n');
            bakfile.println(line.indexOf('\r') > -1 ? line.substring(0,line.lastIndexOf('\r')) : line);
    
    
            uint8_t lineKeycode = String_To_keycode(line.toUpperCase());
            if(lineKeycode)
            {
                if(lineKeycode == KEYCODE_KEY_NUM_LOCK)
                {
                    if(numValidLine == 0)
                        numValidLine = i;
                }
                else
                    numValidLine = 0;
            }
            
            i++;
        }
        textfile.close();
        bakfile.close();
        SD.remove(filename);
    }


    //Copy bakfile to textfile, except for the MODULE_RECORDER's shutdown signal (KEY_NUMLOCK x3 press&release)
    {
        bakfile = SD.open("temp.bak");
        textfile = SD.open(filename,FILE_WRITE);
    
        for(uint32_t i=0; i<numValidLine-1; i++) // numValidLine-1 meaning except last DELAY
        {
            String line = bakfile.readStringUntil('\n');
            textfile.println(line.indexOf('\r') > -1 ? line.substring(0,line.lastIndexOf('\r')) : line);
        }
        bakfile.close();
        textfile.close();
        SD.remove("temp.bak");
    }

    
    // PREVENT entanglement of KEY states
    KBD_HIJACKER.releaseAllBeingHoldDownKey(); delay(10);
                                        
    Buzzzzer::reserveBuzz   ( { NOTE_A4,    NOTE_E4,    NOTE_A3,0,  NOTE_A4,    NOTE_E4,    NOTE_A3,0,  NOTE_A4,    NOTE_E4,    NOTE_A3 }
                            , { 80,         80,         80,150,     80,         80,         80,150,     80,         80,         80      } );
    
    if(isSerial){ MODULE_MACRO_PRINT(filename); Serial.print("MODULE_MACRO_END_RECORDER    RECORDED LINES : "); Serial.println(numRecorded); }
    isMacroRecording=false;
}





void KeyboardHijacker::MODULE_MACRO_PROCEED_PLAYER()
{
    if(!isMacroPlaying)
        return;

    if(msLeftUntilNextMacro)
    {   msLeftUntilNextMacro--; return;   }


    String readline;

    while(true)
    {
        if(textfile.available() || QUEUE_NOW_MINI_MACRO.size()>0)
        {
            if(QUEUE_NOW_MINI_MACRO.size()>0)
            {
                readline = QUEUE_NOW_MINI_MACRO.front();
                QUEUE_NOW_MINI_MACRO.pop_front();
            }
            else
            {
                readline = textfile.readStringUntil('\n');

                // To UPPERCASE without contents after double quotation mark
                uint16_t index_DQM = readline.indexOf("\"");
                if(!index_DQM)
                    readline = readline.toUpperCase();
                else
                    readline = String( readline.substring(0,index_DQM).toUpperCase() + readline.substring(index_DQM) );
            }
        }
        else
        {   MODULE_MACRO_END_PLAYER(); return;   }


        // Exclude COMMENT
        if(0==readline.indexOf('/'))
            return;


        // DN and UP function by Lambda
        auto func_DN =  [](uint8_t keycode)
                        {
                            if(isSerial){ Serial.print(F("(*MACRO EVENT*) DN ")); KBD_HIJACKER.printKeyInfo(keycode); Serial.println(); }
                            KBD_PARSER.KeyLogger.push(keycode);

                            // TRANSMIT_AFTER_HIJACK By Macro
                            key   = keycode_To_TeensyLayout(keycode);
                            event = true;
                            msLatestEventCame = msLatestEventPressed = millis();

                            KBD_HIJACKER.TRANSMIT_AFTER_HIJACK();
                        };
        auto func_UP =  [](uint8_t keycode)
                        {
                            if(isSerial){ Serial.print(F("(*MACRO EVENT*) UP ")); KBD_HIJACKER.printKeyInfo(keycode); Serial.print(F("         Pressed Time : ")); Serial.println(MILLIS_FROM_PRESSED_UNTIL_RELEASE); }

                            // TRANSMIT_AFTER_HIJACK By Macro
                            key   = keycode_To_TeensyLayout(keycode);
                            event = false;
                            msLatestEventCame = millis();

                            KBD_HIJACKER.TRANSMIT_AFTER_HIJACK();
                        };


        // Analyze MacroEvents
        if(-1 < readline.indexOf("DNUP"))
        {
            uint8_t keycode = String_To_keycode(readline);
            
            if(keycode!=0)
            {
                Serial.println();
                func_DN(keycode); delay(11);
                func_UP(keycode);
            }

            numPlayed++;

            continue;
        }
        else if(-1 < readline.indexOf("DN"))
        {
            uint8_t keycode = String_To_keycode(readline);
            
            if(keycode!=0)
            {
                Serial.println();
                func_DN(keycode);
            }
            
            numPlayed++;

            continue;
        }
        else if(-1 < readline.indexOf("UP"))
        {
            uint8_t keycode = String_To_keycode(readline);
            Serial.println();
            
            if(keycode!=0)
            {
                Serial.println();
                func_UP(keycode);
            }

            numPlayed++;

            continue;
        }
        else if (-1 < readline.indexOf("TY"))
        {
            int32_t index_Start = readline.indexOf("\"");
            int32_t index_End = readline.lastIndexOf("\"");
            if( index_Start<0 || index_Start>=index_End ) return;

            String strTyping = readline.substring(index_Start+1,index_End);

            KBD_HIJACKER.pressandreleaseKeys(strTyping);

            if(isSerial){ Serial.print(F("\n(*MACRO EVENT*) TYPE : ")); Serial.println(strTyping); }
            
            numPlayed++;

            continue;
        }
        else if(-1 < readline.indexOf('@'))
        {
            String mapKey = trimming_str( readline.substring(readline.indexOf('@')+1) );

            // SKIP until '{'
            textfile.readStringUntil('{');

            // MINI_MACRO's contents RECORD
            while(true)
            {
                readline = textfile.readStringUntil('\n').toUpperCase();;

                if(readline.indexOf('}') > -1)
                    break;

                MAP_MINI_MACRO[mapKey].push_back(readline);
            }   MAP_MINI_MACRO[mapKey].push_back(readline.replace('}',' '));

            continue;
        }
        else if(-1 < readline.indexOf('$'))
        {
            // MINI_MACRO SEARCH
            for (auto iteratorM = MAP_MINI_MACRO.begin(); iteratorM != MAP_MINI_MACRO.end(); iteratorM++)
            {
                // MINI_MACRO START
                if(readline.indexOf(iteratorM->first) > -1)
                {
                    for(auto iteratorL = (iteratorM->second).begin(); iteratorL != (iteratorM->second).end(); iteratorL++)
                    {
                        QUEUE_NOW_MINI_MACRO.push_back( *iteratorL );
                    }
                }
            }
            
            continue;
        }
        else
        {
            // Based Delay
            msLeftUntilNextMacro = StringDec_To_uint32_t(trimming_num(readline));

            // Extra Delay
            if(readline.indexOf('~') > -1)
                msLeftUntilNextMacro += random( StringDec_To_uint32_t(trimming_num(readline.substring(readline.indexOf('~')))) +1 );

            // Invalid Delay !!
            if(msLeftUntilNextMacro == 0)
                continue;

            if(isSerial){ Serial.print(F("                                            Next MacroEvent lefts : ")); Serial.println(msLeftUntilNextMacro); }

            isEventbyMacro=true;
            numPlayed++;

            return;
        }
    }
}

void KeyboardHijacker::MODULE_MACRO_END_PLAYER()
{
    if(!isMacroPlaying)
        return;


    textfile.close();


    // PREVENT entanglement of KEY states
    KBD_HIJACKER.releaseAllBeingHoldDownKey(); delay(10);
    
    isMacroPlaying=false; isEventbyMacro=false;
    if(isSerial){ Serial.print("\n\nMODULE_MACRO_END_PLAYER    PLAYED LINES : "); Serial.println(numPlayed); Serial.println(); }
}

void KeyboardHijacker::MODULE_MACRO_CHECK_FOR_SHUTDOWN_PLAYER()
{
    if(!isMacroPlaying)
        return;


    // BY FORCE, NOT BY MACRO!
    if(isEventbyMacro)
        return;

    // WHEN KEY_ESC PUSHED, SHUTDOWN PLAYER!!
    if (key == KEY_ESC)
    {
        if(event)
        {
            MODULE_MACRO_END_PLAYER();
        }
        isActivatedKeyEvent=false; key=KEY_NONE;
    }
}

void KeyboardHijacker::MODULE_MACRO_MANUAL_EVENT_DETECTED()
{
    if(!isMacroPlaying)
        return;


    isEventbyMacro=false;
}





void MODULE_MACRO_PRINT(const char* filename) // For, Debugging
{
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
