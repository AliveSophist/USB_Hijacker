
#define MASK_NL     0b10000000
#define MASK_CL     0b01000000
#define MASK_SL     0b00100000

#define MASK_IsLock 0b00010000

#define MASK_CTRL   0b00001000
#define MASK_SHIFT  0b00000100
#define MASK_ALT    0b00000010
#define MASK_GUI    0b00000001

struct MappedData {
    uint32_t millisThreshold;
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

        //exclude COMMENT
        if(0==readline.indexOf('/'))
            continue;
        
        // To uppercase
        for(uint32_t i=0; i<readline.length(); i++)
        {
            if('a' <= readline[i] && readline[i] <= 'z')
                readline[i] -= 32;
        }

        uint8_t keymapKey = String_To_keycode( readline );
        
        uint32_t millisThreshold = 0;
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
                
                stateNCS_CSAG |= (strNLCLSL.indexOf("NL") > -1 || strNLCLSL.indexOf("NUMLOCK") > -1) ? MASK_NL : 0;
                stateNCS_CSAG |= (strNLCLSL.indexOf("CL") > -1 || strNLCLSL.indexOf("CAPSLOCK") > -1) ? MASK_CL : 0;
                stateNCS_CSAG |= (strNLCLSL.indexOf("SL") > -1 || strNLCLSL.indexOf("SCROLLLOCK") > -1) ? MASK_SL : 0;
                
                stateNCS_CSAG |= (strNLCLSL.lastIndexOf('T') > -1) ? MASK_IsLock : 0;
            }

            // Threshold event and Shortcut(CtrlShiftAltGui) event CANNOT COEXIST
            // Reflect Threshold millis
            if(readline.indexOf("THRESHOLD") > -1)
            {
                millisThreshold = StringDec_To_int( split_findNum(readline.substring(readline.indexOf("THRESHOLD"))) );                
            }
            // Reflect CtrlShiftAltGui states
            else
            {
                String strCSAG = strNCS_CSAG.substring(index_CSAG);
                
                stateNCS_CSAG |= (strCSAG.indexOf('C') > -1) ? MASK_CTRL : 0;
                stateNCS_CSAG |= (strCSAG.indexOf('S') > -1) ? MASK_SHIFT : 0;
                stateNCS_CSAG |= (strCSAG.indexOf('A') > -1) ? MASK_ALT : 0;
                stateNCS_CSAG |= (strCSAG.indexOf('G') > -1) ? MASK_GUI : 0;
            }
        }

        // Map!
        if(readline.indexOf('[') < readline.indexOf(']') && keymapKey != 0)
        {
            String strMapThings = readline.substring(readline.indexOf('[')+1,readline.indexOf(']'));

            if(strMapThings.indexOf("\"") > -1)
            {
                int8_t index_Start = readline.indexOf("\"")+1;
                int8_t index_End;
                if(readline.lastIndexOf(".TXT") > -1)
                    index_End = (readline.lastIndexOf("_R") > -1) ? readline.lastIndexOf("_R") : readline.lastIndexOf(".TXT");
                else
                    index_End = (readline.lastIndexOf("_R") > -1) ? readline.lastIndexOf("_R") : readline.lastIndexOf("\"");

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
                data.millisThreshold = millisThreshold;
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
                data.millisThreshold = millisThreshold;
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
    // Hmmm.. ALREADY DEAD KeyEvent
    if(key == 0 || !isActivateKeyEvent)
        return;

    // THIS KEY IS NOT MAPPED
    if(keymap.find( TeensyLayout_To_Keycode(key) ) == keymap.end())
        return;


    uint8_t keycode = TeensyLayout_To_Keycode(key);

    uint8_t nowStateCSAG = 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_CTRL) || KBD_Hijacker.getLogicalState(KEY_RIGHT_CTRL)) ? MASK_CTRL : 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_SHIFT)|| KBD_Hijacker.getLogicalState(KEY_RIGHT_SHIFT))? MASK_SHIFT: 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_ALT)  || KBD_Hijacker.getLogicalState(KEY_RIGHT_ALT))  ? MASK_ALT  : 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_GUI)  || KBD_Hijacker.getLogicalState(KEY_RIGHT_GUI))  ? MASK_GUI  : 0;

    bool nowStateNumLock    = KBD_Hijacker.getStateNumLockToggle();
    bool nowStateCapsLock   = KBD_Hijacker.getStateCapsLockToggle();
    bool nowStateScrollLock = KBD_Hijacker.getStateScrollLockToggle();


    for (auto& data : keymap[keycode])
    {
        // CSAGState IS NOT MATCHED !!
        if((data.stateNCS_CSAG & 0b00001111) != 0 && (data.stateNCS_CSAG & 0b00001111) != nowStateCSAG)
            continue;

        bool isMatchLockState = false;

        if      ((data.stateNCS_CSAG & MASK_NL) && (data.stateNCS_CSAG & MASK_IsLock)==nowStateNumLock)
        {
            isMatchLockState = true;
        }
        else if ((data.stateNCS_CSAG & MASK_CL) && (data.stateNCS_CSAG & MASK_IsLock)==nowStateCapsLock)
        {
            isMatchLockState = true;
        }
        else if ((data.stateNCS_CSAG & MASK_SL) && (data.stateNCS_CSAG & MASK_IsLock)==nowStateScrollLock)
        {
            isMatchLockState = true;
        }
        
        // LockState IS NOT MATCHED !!
        if(!isMatchLockState)
            continue;


        // This key Mapped One key to One key
        if      (data.mappedLen == 1)
        {
            key = keycode_To_TeensyLayout( ((uint8_t*)data.mappedThings)[0] );
        }
        
        // This key Mapped One KEY to NONE And press Shortcut KEY
        else if (data.mappedLen > 1)
        {
            isActivateKeyEvent=false; key=0;
            
            if(event)
            {
                if(data.millisThreshold != 0)
                    continue;
                
                KBD_Hijacker.releaseAllBeingHoldDownKey(); delay(10);
                
                int32_t keys[data.mappedLen];
                for(uint8_t i=0; i<data.mappedLen; i++)
                    keys[i] = keycode_To_TeensyLayout( ((uint8_t*)data.mappedThings)[i] );
                
                KBD_Hijacker.pressandreleaseShortcutKey(keys, data.mappedLen, false);
            }
            else
            {
                if(data.millisThreshold >= PRESSED_TIME_UNTIL_RELEASE)
                    continue;
                
                KBD_Hijacker.releaseAllBeingHoldDownKey(); delay(10);
                
                int32_t keys[data.mappedLen];
                for(uint8_t i=0; i<data.mappedLen; i++)
                    keys[i] = keycode_To_TeensyLayout( ((uint8_t*)data.mappedThings)[i] );
                
                KBD_Hijacker.pressandreleaseShortcutKey(keys, data.mappedLen, false);
            }
        }
        
        // This key Mapped One KEY to NONE And excute MACRO
        else
        {
            isActivateKeyEvent=false; key=0;
            
            if(event)
            {
                KBD_Hijacker.releaseAllBeingHoldDownKey(); delay(10);
                
                MODULE_MACRO_PLAYER_OR_RECORDER_START( ((char*)data.mappedThings) );
            }
        }


        if(isSerial)
        {
            Serial.println();
            Serial.println("******************************************");

            Serial.println("MAPPED HIJACK EVENT ACTIVATED !"); 
            Serial.print("Now Hijacking KEY : "); print8bitHex(keycode); Serial.println(); Serial.println();

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

            Serial.println("******************************************");
            Serial.println();
        }
        
        break;
    }
}
