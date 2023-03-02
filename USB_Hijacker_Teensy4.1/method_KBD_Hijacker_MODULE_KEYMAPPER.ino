
struct MappedData {
    uint8_t stateNCS_CSAG;
    uint8_t mappedLen;
    void* mappedThings;
};

static std::map<uint8_t, std::list<MappedData>> keymap;

void MODULE_KEYMAPPER_INITIALIZE()
{
    if(!isExistSD)
        return;

    if(!SD.exists("MAPPER.TXT")){
        // create blank textfile
        textfile = SD.open("MAPPER.TXT",FILE_WRITE); textfile.close();
        return;
    }

    if(isSerial){ Serial.println("\n\nMODULE_KEYMAPPER_INITIALIZE START\n"); }
    
    textfile = SD.open("MAPPER.TXT");
    String readline;

    while(textfile.available())
    {
        readline = textfile.readStringUntil('\n');
        
        // To uppercase
        for(uint32_t i=0; i<readline.length(); i++)
        {
            if('a' <= readline[i] && readline[i] <= 'z')
                readline[i] -= 32;
        }

        uint8_t keymapKey = String_To_keycode( readline.substring(0, readline.lastIndexOf('[')) );

        uint8_t stateNCS_CSAG = 0;
        // Reflect NLCLSL states & CSAG states
        if(readline.indexOf('<') < readline.indexOf('>'))
        {
            String strNCS_CSAG = readline.substring(readline.indexOf('<')+1,readline.indexOf('>'));
            // 0b 0000 0000
            //  = NCS_ CSAG
            
            int8_t index_CSAG = (strNCS_CSAG.lastIndexOf('T')>strNCS_CSAG.lastIndexOf('F') ? strNCS_CSAG.lastIndexOf('T') : strNCS_CSAG.lastIndexOf('F')) + 1;
            
            // Reflect NumCapsScroll states
            if (strNCS_CSAG.lastIndexOf('L') > -1)
            {
                String strNLCLSL = strNCS_CSAG.substring(0, index_CSAG);
                
                if      (strNLCLSL.indexOf("NL") > -1 || strNLCLSL.indexOf("NUMLOCK") > -1)
                    stateNCS_CSAG |= 0b10000000;
                else if (strNLCLSL.indexOf("CL") > -1 || strNLCLSL.indexOf("CAPSLOCK") > -1)
                    stateNCS_CSAG |= 0b01000000;
                else if (strNLCLSL.indexOf("SL") > -1 || strNLCLSL.indexOf("SCROLLLOCK") > -1)
                    stateNCS_CSAG |= 0b00100000;
                
                if (strNLCLSL.lastIndexOf('T') > -1)
                    stateNCS_CSAG |= 0b00010000;
            }

            // Reflect CtrlShiftAltGui states
            {
                String strCSAG = strNCS_CSAG.substring(index_CSAG);
                
                if (strCSAG.indexOf('C') > -1)
                    stateNCS_CSAG |= 0b00001000;
                if (strCSAG.indexOf('S') > -1)
                    stateNCS_CSAG |= 0b00000100;
                if (strCSAG.indexOf('A') > -1)
                    stateNCS_CSAG |= 0b00000010;
                if (strCSAG.indexOf('G') > -1)
                    stateNCS_CSAG |= 0b00000001;
            }
        }

        // Map!
        if(readline.indexOf('[') < readline.indexOf(']') && (keymapKey & 0b11111111) != 0)
        {
            String strMapThings = readline.substring(readline.indexOf('[')+1,readline.indexOf(']'));

            if(strMapThings.indexOf(".TXT") > -1)
            {
                int8_t index_Start = readline.indexOf("\"")+1;
                int8_t index_End = (readline.lastIndexOf("_R.TXT") > -1 ? readline.lastIndexOf("_R") : readline.lastIndexOf(".TXT"));

                // When SYNTAX ERROR occured, Go to 'while'
                if(index_Start<0||index_Start>=index_End)
                { 
                    if(isSerial) Serial.println("\n!!! SYNTAX ERROR !!!");
                    continue;
                }

                String strFilename = readline.substring(index_Start,index_End);
                uint8_t strLength = strFilename.length();

                char* mappedFilename = (char*)malloc((strLength+1) * sizeof(char));
                strFilename.toCharArray(mappedFilename,strLength+1);
        
                // Create a MappedData object and assign values
                MappedData data;
                data.stateNCS_CSAG = stateNCS_CSAG;
                data.mappedLen = 0;
                data.mappedThings = mappedFilename;
                keymap[keymapKey].push_back(data);

                if(isSerial) 
                {
                    Serial.print("MAPPED keycode : ");
                    print8bitHex(keymapKey); Serial.println();
                    
                    Serial.print("MAPPED mask    : ");
                    print8bitBin(data.stateNCS_CSAG); Serial.println();
                    
                    Serial.print("    mappedLength : ");
                    Serial.print(data.mappedLen); Serial.println(" (0 means MACRO)");
                    
                    Serial.print("    mappedTXT : ");
                    Serial.println(((char*)data.mappedThings));
                    
                    Serial.println();
                }
            }
            else
            {
                uint8_t mappedLen = 1;
                {
                    char buf[strMapThings.length()+1];
                    strMapThings.toCharArray(buf,strMapThings.length()+1);
        
                    char* s = buf;
                    while(*s!='\0'){ if(*s=='+')mappedLen++; s++; }
                }
                
                uint8_t* mappedKeycodes = (uint8_t*)malloc(mappedLen * sizeof(uint8_t));
                for(uint8_t i=0; i<mappedLen; i++)
                {
                    mappedKeycodes[i] = String_To_keycode( strMapThings );
    
                    if(mappedKeycodes[i] == 0)
                    {
                        free(mappedKeycodes);
                        mappedKeycodes = NULL;
                        break;
                    }
                    
                    if(strMapThings.indexOf('+') > -1)
                        strMapThings = strMapThings.substring(strMapThings.indexOf('+')+1);
                }
                
                // When SYNTAX ERROR occured, Go next line
                if(mappedKeycodes==NULL)
                {
                    if(isSerial) Serial.println("\n!!! SYNTAX ERROR !!!"); 
                    continue;
                }
        
                // Create a MappedData object and assign values
                MappedData data;
                data.stateNCS_CSAG = stateNCS_CSAG;
                data.mappedLen = mappedLen;
                data.mappedThings = mappedKeycodes;
                keymap[keymapKey].push_back(data);

                if(isSerial) 
                {
                    Serial.print("MAPPED keycode : ");
                    print8bitHex(keymapKey); Serial.println();
                    
                    Serial.print("MAPPED mask    : ");
                    print8bitBin(data.stateNCS_CSAG); Serial.println();
                    
                    Serial.print("    mappedLength : ");
                    Serial.println(data.mappedLen);
                    
                    for(uint8_t i=0; i<mappedLen; i++)
                    { 
                        Serial.print("    mappedThings[");Serial.print(i);Serial.print("] : ");
                        print8bitHex( ((uint8_t*)data.mappedThings)[i] ); Serial.println();
                        // ((uint8_t*)data.mappedThings)[i] is same '(reinterpret_cast<uint8_t*>(data.mappedThings))[i]'
                    }
                    
                    Serial.println();
                }
            }
        }
    }
    
    textfile.close();
    
    if(isSerial){ Serial.println("MODULE_KEYMAPPER_INITIALIZE END\n"); }
}

void MODULE_KEYMAPPER_HIJACK()
{
    if(key == 0 || !isActivateKeyEvent)
        return;

    if(keymap.find( TeensyLayout_To_Keycode(key) ) == keymap.end())
        return;
    
    uint8_t keycode = TeensyLayout_To_Keycode(key);
    
    uint8_t nowStateCSAG = 0;
    if(KBD_Hijacker.getLogicalState(KEY_LEFT_CTRL) || KBD_Hijacker.getLogicalState(KEY_RIGHT_CTRL))
        nowStateCSAG |= 0b00001000;
    if(KBD_Hijacker.getLogicalState(KEY_LEFT_SHIFT)|| KBD_Hijacker.getLogicalState(KEY_RIGHT_SHIFT))
        nowStateCSAG |= 0b00000100;
    if(KBD_Hijacker.getLogicalState(KEY_LEFT_ALT)  || KBD_Hijacker.getLogicalState(KEY_RIGHT_ALT))
        nowStateCSAG |= 0b00000010;
    if(KBD_Hijacker.getLogicalState(KEY_LEFT_GUI)  || KBD_Hijacker.getLogicalState(KEY_RIGHT_GUI))
        nowStateCSAG |= 0b00000001;

//    bool nowStateNumLock    = KBD_Hijacker.getStateNumLockToggle();
//    bool nowStateCapsLock   = KBD_Hijacker.getStateCapsLockToggle();
//    bool nowStateScrollLock = KBD_Hijacker.getStateScrollLockToggle();
        
    //if(isSerial){ Serial.println(); Serial.println("MODULE_KEYMAPPER_HIJACK ACTIVATED"); Serial.print("Now Hijacking KEY : "); print8bitHex(keycode); Serial.println(); Serial.println(); }


    Serial.println("******************************************");
    for (auto& data : keymap[keycode]) {





        
        Serial.print("MAPPED mask    : ");
        print8bitBin(data.stateNCS_CSAG); Serial.println();
        
        Serial.print("    mappedLength : ");
        Serial.println(data.mappedLen);

        if(data.mappedLen == 0)
        {
            Serial.print("    mappedTXT : ");
            Serial.println(((char*)data.mappedThings));
        }
        else
        {
            for(uint8_t i=0; i<data.mappedLen; i++)
            { 
                Serial.print("    mappedThings[");Serial.print(i);Serial.print("] : ");
                print8bitHex( ((uint8_t*)data.mappedThings)[i] ); Serial.println();
            }
        }
    }
    Serial.println("******************************************");




}
