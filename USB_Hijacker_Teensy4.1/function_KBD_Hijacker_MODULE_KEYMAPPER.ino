
#define MASK_NL     0b10000000
#define MASK_CL     0b01000000
#define MASK_SL     0b00100000

#define MASK_IsLock 0b00010000

#define MASK_CTRL   0b00001000
#define MASK_SHIFT  0b00000100
#define MASK_ALT    0b00000010
#define MASK_GUI    0b00000001

struct MappedData
{
    uint32_t* millisPressedTime;
    uint8_t stateNCS_CSAG;
    
    uint8_t mappedLen;
    void* mappedThings;
};

static std::map<uint8_t, list<MappedData>> MAP_KEYMAPPER;

void MODULE_KEYMAPPER_INITIALIZE()
{
    if(!isExistSD)
        return;

    if(!SD.exists("MAPPER.TXT"))
    {
        //MAPPER.TXT not exists, Create blank textfile
        textfile = SD.open("MAPPER.TXT",FILE_WRITE); textfile.close();
        return;
    }


    if(isSerial){ Serial.println("\n\nMODULE_KEYMAPPER_INITIALIZE START\n"); }
    
    textfile = SD.open("MAPPER.TXT");
    String readline;

    while(textfile.available())
    {
        readline = textfile.readStringUntil('\n').toUpperCase();

        // Exclude COMMENT
        if(readline.indexOf('/') > -1)
            readline = readline.substring(0, readline.indexOf('/'));

        uint8_t mapKey = String_To_keycode(readline);

        bool isExistPTIME = false;
        uint32_t millisPressedTime = 0;
        
        // Reflect PTIME event
        if(readline.indexOf("PTIME") > -1)
        {
            isExistPTIME = true;
            millisPressedTime = StringDec_To_uint32_t( trimming_num(readline.substring(readline.indexOf("PTIME"))) );                
        }
        
        uint8_t stateNCS_CSAG = 0;
        
        // Reflect NLCLSL states & CSAG states
        if(readline.indexOf('<') < readline.indexOf('>'))
        {
            String strNCS_CSAG = readline.substring(readline.indexOf('<')+1, readline.indexOf('>'));
            // 0b 0000 0000
            //  = NCS_ CSAG
            
            int8_t index_CSAG = (strNCS_CSAG.lastIndexOf('T')>strNCS_CSAG.lastIndexOf('F') ? strNCS_CSAG.lastIndexOf('T') : strNCS_CSAG.lastIndexOf('F')) + 1;

            
            // Reflect NumCapsScroll states
            if(strNCS_CSAG.lastIndexOf('L') > -1)
            {
                String strNLCLSL = strNCS_CSAG.substring(0, index_CSAG);
                
                stateNCS_CSAG |= (strNLCLSL.indexOf("NL") > -1 || strNLCLSL.indexOf("NUM") > -1) ? MASK_NL : 0;
                stateNCS_CSAG |= (strNLCLSL.indexOf("CL") > -1 || strNLCLSL.indexOf("CAPS") > -1) ? MASK_CL : 0;
                stateNCS_CSAG |= (strNLCLSL.indexOf("SL") > -1 || strNLCLSL.indexOf("SCR") > -1) ? MASK_SL : 0;
                
                stateNCS_CSAG |= (strNLCLSL.lastIndexOf('T') > -1) ? MASK_IsLock : 0;
            }


            // Reflect CtrlShiftAltGui states
            // Shortcut(CtrlShiftAltGui) event and PTIME event CANNOT COEXIST
            if(!isExistPTIME)
            {
                String strCSAG = strNCS_CSAG.substring(index_CSAG);
                
                stateNCS_CSAG |= (strCSAG.indexOf('C') > -1) ? MASK_CTRL : 0;
                stateNCS_CSAG |= (strCSAG.indexOf('S') > -1) ? MASK_SHIFT : 0;
                stateNCS_CSAG |= (strCSAG.indexOf('A') > -1) ? MASK_ALT : 0;
                stateNCS_CSAG |= (strCSAG.indexOf('G') > -1) ? MASK_GUI : 0;
            }
        }

        // Map!
        if(readline.indexOf('[') < readline.indexOf(']') && mapKey != 0)
        {
            MappedData data;
            
            String strMapThings = readline.substring(readline.indexOf('[')+1,readline.indexOf(']'));

            // Mapping MACRO
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
                data.millisPressedTime = NULL;
                data.stateNCS_CSAG = stateNCS_CSAG;
                data.mappedLen = 0;
                data.mappedThings = mappedFilename;

                if(isSerial) 
                {
                    Serial.print("MAPPED keycode : ");
                    print8bitHex(mapKey); Serial.println();
                    
                    Serial.print("MAPPED mask    : ");
                    print8bitBin(data.stateNCS_CSAG); Serial.println();
                    
                    Serial.print("    mappedLength : ");
                    Serial.print(data.mappedLen); Serial.println(" (0 means MACRO)");
                    
                    Serial.print("    mappedTXT : ");
                    Serial.println(((char*)data.mappedThings));
                    
                    Serial.println();
                }
            }
            // Mapping keycodes
            else
            {
                uint8_t mappedLen = 1;
                {
                    char buf[strMapThings.length()+1];
                    strMapThings.toCharArray(buf,strMapThings.length()+1);
        
                    char* s = buf;
                    while(*s!='\0'){ if(*s=='+')mappedLen++; s++; }
                }

                if(mappedLen == 1 && isExistPTIME)
                {
                    mappedLen = 2;
                    strMapThings = String( trimming_keycodeStr(strMapThings) + " + KEY_NONE" );
                }
                
                uint8_t* mappedKeycodes = (uint8_t*)malloc(mappedLen * sizeof(uint8_t));
                for(uint8_t i=0; i<mappedLen; i++)
                {
                    mappedKeycodes[i] = String_To_keycode(strMapThings);
    
                    if(mappedKeycodes[i] == 0 && !(trimming_keycodeStr(strMapThings).equals("KEY_NONE")))
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
                data.millisPressedTime = (uint32_t*)malloc(sizeof(uint32_t));
                data.millisPressedTime[0] = millisPressedTime;
                data.stateNCS_CSAG = stateNCS_CSAG;
                data.mappedLen = mappedLen;
                data.mappedThings = mappedKeycodes;

                if(isSerial) 
                {
                    Serial.print("MAPPED keycode : ");
                    print8bitHex(mapKey); Serial.println();
                    
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

            if((data.stateNCS_CSAG & 0b00001111) == 0)
                MAP_KEYMAPPER[mapKey].push_back(data);
            else
                MAP_KEYMAPPER[mapKey].push_front(data);
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
    if(MAP_KEYMAPPER.find( TeensyLayout_To_keycode(key) ) == MAP_KEYMAPPER.end())
        return;


    uint8_t keycode = TeensyLayout_To_keycode(key);

    bool nowStateNumLock    = KBD_Hijacker.getStateNumLockToggle();
    bool nowStateCapsLock   = KBD_Hijacker.getStateCapsLockToggle();
    bool nowStateScrollLock = KBD_Hijacker.getStateScrollLockToggle();
    
    uint8_t nowStateCSAG = 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_CTRL) || KBD_Hijacker.getLogicalState(KEY_RIGHT_CTRL)) ? MASK_CTRL : 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_SHIFT)|| KBD_Hijacker.getLogicalState(KEY_RIGHT_SHIFT))? MASK_SHIFT: 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_ALT)  || KBD_Hijacker.getLogicalState(KEY_RIGHT_ALT))  ? MASK_ALT  : 0;
    nowStateCSAG |= (KBD_Hijacker.getLogicalState(KEY_LEFT_GUI)  || KBD_Hijacker.getLogicalState(KEY_RIGHT_GUI))  ? MASK_GUI  : 0;


    for (auto& data : MAP_KEYMAPPER[keycode])
    {
        // CSAGState IS NOT MATCHED !!
        if  (   (data.stateNCS_CSAG & 0b00001111) != 0 &&
                !( (data.stateNCS_CSAG & 0b00001111) == nowStateCSAG )
            )
            continue;

        // LockState IS NOT MATCHED !!
        if  (   (data.stateNCS_CSAG & 0b11110000) != 0 && 
                !(  ((data.stateNCS_CSAG & MASK_NL) && (data.stateNCS_CSAG & MASK_IsLock) == nowStateNumLock)   ||
                    ((data.stateNCS_CSAG & MASK_CL) && (data.stateNCS_CSAG & MASK_IsLock) == nowStateCapsLock)  ||
                    ((data.stateNCS_CSAG & MASK_SL) && (data.stateNCS_CSAG & MASK_IsLock) == nowStateScrollLock)
                )
            )
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
            
            if  (   event &&
                    data.millisPressedTime[0]==0
                )
            {
                KBD_Hijacker.releaseAllBeingHoldDownKey(); delay(10);
                
                int32_t keys[data.mappedLen];
                for(uint8_t i=0; i<data.mappedLen; i++)
                    keys[i] = keycode_To_TeensyLayout( ((uint8_t*)data.mappedThings)[i] );
                
                KBD_Hijacker.pressandreleaseShortcutKey(keys, data.mappedLen);
            }
            else if (   !event &&
                        data.millisPressedTime[0]!=0 &&
                        data.millisPressedTime[0]<PRESSED_TIME_UNTIL_RELEASE
                    )
            {
                KBD_Hijacker.releaseAllBeingHoldDownKey(); delay(10);
                
                int32_t keys[data.mappedLen];
                for(uint8_t i=0; i<data.mappedLen; i++)
                    keys[i] = keycode_To_TeensyLayout( ((uint8_t*)data.mappedThings)[i] );
                
                KBD_Hijacker.pressandreleaseShortcutKey(keys, data.mappedLen);
            }
            else
                continue;
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
