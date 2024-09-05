#pragma once
#include "KeyboardHijacker.h"



struct MappedData
{
    uint8_t stateNCS_CSAG;
    // stateNCS_CSAG's each bits represents...
    #define MASK_NL     0b10000000
    #define MASK_CL     0b01000000
    #define MASK_SL     0b00100000
    #define MASK_IsLock 0b00010000
    #define MASK_CTRL   0b00001000
    #define MASK_SHIFT  0b00000100
    #define MASK_ALT    0b00000010
    #define MASK_GUI    0b00000001

    uint8_t mappedLen;
    void* mappedThings;

    uint32_t* millisPressedTime;

    bool isRequiredCleanse;
    bool isRequiredRapidfire;
};
std::map<uint8_t, std::list<MappedData>> MAP_KEYMAPPER;

struct RapidfireEvent
{
    uint8_t keycode;
    int32_t key;
};
std::vector<RapidfireEvent> queueActivatedRapidfire;
bool isUpdatedActivatedRapidfire = false;
bool isRapidfiring = false;

const char MAPPER[] PROGMEM = "MAPPER.TXT";



void KeyboardHijacker::MODULE_KEYMAPPER_INITIALIZE()
{
    if(!SD.exists(MAPPER)) // MAPPER.TXT not exists, Create blank textfile, then skip mapping
    {   textfile = SD.open(MAPPER, FILE_WRITE); textfile.close(); return;   }
    else
        textfile = SD.open(MAPPER);


    if(isDEBUG){ Serial.println("MODULE_KEYMAPPER_INITIALIZE START\n"); }


    while(textfile.available())
    {
        String readline = textfile.readStringUntil('\n').toUpperCase();


        // Exclude COMMENT
        if(readline.indexOf('/') > -1)
            readline = readline.substring(0, readline.indexOf('/'));


        // Get mapKey that is both 'a key of MAP_KEYMAPPER' and 'the keycode'
        uint8_t mapKey
         = (readline.indexOf('[') > -1 && (readline.indexOf('[') < readline.indexOf(']')))
            ? String_To_keycode(readline.substring(0, readline.indexOf('[')))
            : 0;

        // mapKey NOT FOUNDED, invalid line
        if(mapKey == 0)
            continue;


        // Get CLEANSE event status
        bool isRequiredCleanse
         = (readline.indexOf("CLEANSE") > -1) ? true : false;


        // Get RAPIDFIRE event status
        bool isRequiredRapidfire
         = (readline.indexOf("RAPIDFIRE") > -1) ? true : false;


        // Get PTIME event info
        bool isExistPTIME = (readline.indexOf("PTIME") > -1);
        uint32_t millisPressedTime = isExistPTIME ? StringDec_To_uint32_t( trimming_num( readline.substring(readline.indexOf("PTIME")) ) ) : 0;


        // Get NLCLSL & CSAG states
        uint8_t stateNCS_CSAG = 0;
        if(readline.indexOf('<') > -1 && readline.indexOf('<') < readline.indexOf('>'))
        {
            String strNCS_CSAG = readline.substring(readline.indexOf('<')+1, readline.indexOf('>'));
            int8_t index_CSAG = (strNCS_CSAG.lastIndexOf('T')>strNCS_CSAG.lastIndexOf('F') ? strNCS_CSAG.lastIndexOf('T') : strNCS_CSAG.lastIndexOf('F')) + 1;

            // Get NumCapsScroll states
            if(strNCS_CSAG.lastIndexOf('L') > -1)
            {
                String strNLCLSL = strNCS_CSAG.substring(0, index_CSAG);

                stateNCS_CSAG |= (strNLCLSL.indexOf("NL") > -1 || strNLCLSL.indexOf("NUM") > -1) ? MASK_NL : 0;
                stateNCS_CSAG |= (strNLCLSL.indexOf("CL") > -1 || strNLCLSL.indexOf("CAPS") > -1) ? MASK_CL : 0;
                stateNCS_CSAG |= (strNLCLSL.indexOf("SL") > -1 || strNLCLSL.indexOf("SCR") > -1) ? MASK_SL : 0;

                stateNCS_CSAG |= (strNLCLSL.lastIndexOf('T') > -1) ? MASK_IsLock : 0;
            }

            // Get CtrlShiftAltGui states
            // Shortcut(CtrlShiftAltGui) TRIGGER EVENT and PTIME CANNOT COEXIST
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
        {
            MappedData data;

            String strMapThings = readline.substring(readline.indexOf('[')+1,readline.indexOf(']'));

            // Mapping MACRO event
            if(strMapThings.indexOf("\"") > -1)
            {
                int8_t index_Start = readline.indexOf("\"")+1;
                int8_t index_End;
                if(readline.lastIndexOf(".TXT") > -1)
                    index_End = (readline.lastIndexOf("_R") > -1) ? readline.lastIndexOf("_R") : readline.lastIndexOf(".TXT");
                else
                    index_End = (readline.lastIndexOf("_R") > -1) ? readline.lastIndexOf("_R") : readline.lastIndexOf("\"");

                // When SYNTAX ERROR occured, Go back to 'while'
                if(index_Start < 0 || index_Start >= index_End)
                { 
                    if(isDEBUG) Serial.println("\n!!! SYNTAX ERROR !!!");

                    continue;
                }

                String strFilename = readline.substring(index_Start,index_End);
                uint8_t strLength = strFilename.length();

                char* mappedFilename = (char*)malloc((strLength+1) * sizeof(char));
                strFilename.toCharArray(mappedFilename,strLength+1);

                // Create a MappedData object and assign values
                data.stateNCS_CSAG = stateNCS_CSAG;
                data.mappedLen = 0;
                data.mappedThings = mappedFilename;

                if(isDEBUG) 
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
            // Mapping One KEY / Multi KEY event
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
                    strMapThings = String( trimming_keycodeStr(strMapThings) + "+KEY_NONE" );
                    mappedLen+=1;
                }

                uint8_t* mappedKeycodes = (uint8_t*)malloc(mappedLen * sizeof(uint8_t));
                for(uint8_t i=0; i<mappedLen; i++)
                {
                    mappedKeycodes[i] = String_To_keycode(strMapThings);

                    // When invalid keycode detected, SYNTAX ERROR occurs
                    if(mappedKeycodes[i] == 0 && !(trimming_keycodeStr(strMapThings).equals("KEY_NONE")))
                    {
                        free(mappedKeycodes);
                        mappedKeycodes = NULL;
                        break;
                    }

                    if(strMapThings.indexOf('+') > -1)
                        strMapThings = strMapThings.substring(strMapThings.indexOf('+')+1);
                }

                // When map MultiKey(like 'Ctrl+Shift+ESC') event that triggered by ShortcutEvent,
                // In the case of the ShortcutEvent that already includes Ctrl, does not require 'cleanse'. (only write 'Shift+ESC')
                // but already includes Alt or a key other than Ctrl, requires 'cleanse'.
                if(mappedLen > 1)
                {
                    if((stateNCS_CSAG & 0b00001111) == 0)
                        isRequiredCleanse = true;
                    else
                    {
                        auto isAlreadyExist = [&mappedLen, &mappedKeycodes](uint8_t keycode){ return std::find(mappedKeycodes, mappedKeycodes+mappedLen, keycode) != mappedKeycodes+mappedLen; };
                        isRequiredCleanse =     !(
                                                    (stateNCS_CSAG & MASK_CTRL ? isAlreadyExist(KEYCODE_KEY_LEFT_CTRL) : true) &&
                                                    (stateNCS_CSAG & MASK_SHIFT? isAlreadyExist(KEYCODE_KEY_LEFT_SHIFT): true) &&
                                                    (stateNCS_CSAG & MASK_ALT  ? isAlreadyExist(KEYCODE_KEY_LEFT_ALT)  : true) &&
                                                    (stateNCS_CSAG & MASK_GUI  ? isAlreadyExist(KEYCODE_KEY_LEFT_GUI)  : true)
                                                );

                        if(!isRequiredCleanse)
                        {
                            for(uint8_t i=0; i<mappedLen; i++)
                            {
                                if(stateNCS_CSAG & MASK_CTRL && mappedKeycodes[i]==KEYCODE_KEY_LEFT_CTRL ) mappedKeycodes[i]=KEYCODE_NONE;
                                if(stateNCS_CSAG & MASK_SHIFT&& mappedKeycodes[i]==KEYCODE_KEY_LEFT_SHIFT) mappedKeycodes[i]=KEYCODE_NONE;
                                if(stateNCS_CSAG & MASK_ALT  && mappedKeycodes[i]==KEYCODE_KEY_LEFT_ALT  ) mappedKeycodes[i]=KEYCODE_NONE;
                                if(stateNCS_CSAG & MASK_GUI  && mappedKeycodes[i]==KEYCODE_KEY_LEFT_GUI  ) mappedKeycodes[i]=KEYCODE_NONE;
                            }
                        }
                    }
                }

                // When SYNTAX ERROR occured, Go next line
                if(mappedKeycodes==NULL)
                {
                    if(isDEBUG) Serial.println("\n!!! SYNTAX ERROR !!!"); 

                    continue;
                }

                // Create a MappedData object and assign values
                data.stateNCS_CSAG = stateNCS_CSAG;
                data.mappedLen = mappedLen;
                data.mappedThings = mappedKeycodes;
                data.millisPressedTime = new uint32_t(millisPressedTime);
                data.isRequiredCleanse = isRequiredCleanse;
                if((stateNCS_CSAG & 0b00001111) == 0) // only One key to One key EVENT can activate rapidfire
                    data.isRequiredRapidfire = isRequiredRapidfire;

                if(isDEBUG) 
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
                        print8bitHex( ((uint8_t*)data.mappedThings)[i] );
                        Serial.println();
                    }

                    Serial.println();
                }
            }

            // Shortcut(CtrlShiftAltGui) TRIGGER EVENT's priority is higher than One key to One key EVENT's
            if((data.stateNCS_CSAG & 0b00001111) != 0)
                MAP_KEYMAPPER[mapKey].push_front(data);
            else
                MAP_KEYMAPPER[mapKey].push_back(data);
        }
    }
    
    textfile.close();
    
    if(isDEBUG){ Serial.println("MODULE_KEYMAPPER_INITIALIZE END\n"); }
}



void KeyboardHijacker::MODULE_KEYMAPPER_HIJACK()
{
    uint8_t keycode = TeensyLayout_To_keycode(key);


    // Hmmm.. ALREADY DEAD KeyEvent
    if(key == 0 || !isActivatedKeyEvent)
        return;

    // THIS KEY IS NOT MAPPED
    if(MAP_KEYMAPPER.find( keycode ) == MAP_KEYMAPPER.end())
        return;


    bool nowStateNumLock    = KBD_HIJACKER.getStateNumLockToggle();
    bool nowStateCapsLock   = KBD_HIJACKER.getStateCapsLockToggle();
    bool nowStateScrollLock = KBD_HIJACKER.getStateScrollLockToggle();

    uint8_t nowStateCSAG = 0;
    nowStateCSAG |= ( (KBD_HIJACKER.getLogicalState(KEY_LEFT_CTRL) || KBD_HIJACKER.getLogicalState(KEY_RIGHT_CTRL)) ? MASK_CTRL : 0 );
    nowStateCSAG |= ( (KBD_HIJACKER.getLogicalState(KEY_LEFT_SHIFT)|| KBD_HIJACKER.getLogicalState(KEY_RIGHT_SHIFT))? MASK_SHIFT: 0 );
    nowStateCSAG |= ( (KBD_HIJACKER.getLogicalState(KEY_LEFT_ALT)  || KBD_HIJACKER.getLogicalState(KEY_RIGHT_ALT))  ? MASK_ALT  : 0 );
    nowStateCSAG |= ( (KBD_HIJACKER.getLogicalState(KEY_LEFT_GUI)  || KBD_HIJACKER.getLogicalState(KEY_RIGHT_GUI))  ? MASK_GUI  : 0 );


    for (auto& data : MAP_KEYMAPPER[keycode])
    {
        // CSAGState IS NOT MATCHED !!
        bool isShortcutEvent = (data.stateNCS_CSAG & 0b00001111) != 0;
        if(
            isShortcutEvent
            && (data.stateNCS_CSAG & 0b00001111) != nowStateCSAG
        )
            continue;

        // LockState IS NOT MATCHED !!
        bool isShouldMatchLockState = (data.stateNCS_CSAG & 0b11110000) != 0;
        if(
            isShouldMatchLockState
            && !(
                    ((data.stateNCS_CSAG & MASK_NL)!=0 && ((data.stateNCS_CSAG & MASK_IsLock)!=0) == nowStateNumLock)   ||
                    ((data.stateNCS_CSAG & MASK_CL)!=0 && ((data.stateNCS_CSAG & MASK_IsLock)!=0) == nowStateCapsLock)  ||
                    ((data.stateNCS_CSAG & MASK_SL)!=0 && ((data.stateNCS_CSAG & MASK_IsLock)!=0) == nowStateScrollLock)
                )
        )
            continue;


        /*** This key Mapped One KEY event ***/
        if(data.mappedLen == 1)
        {
            key = keycode_To_TeensyLayout( ((uint8_t*)data.mappedThings)[0] );


            if(isShortcutEvent) // if Shortcut event, cleanse before and after event
            {
                KBD_HIJACKER.releaseAllBeingHoldDownKey();
                KBD_HIJACKER.reserveReleaseAllBeingHoldDownKey();
            }

            if(data.isRequiredCleanse)
            {
                if(event) KBD_HIJACKER.reserveReleaseAllBeingHoldDownKey();
            }

            if(data.isRequiredRapidfire)
            {
                if(event)
                {
                    // push RapidfireEvent with this keycode
                    if(queueActivatedRapidfire.size()<=6)
                    {   isRapidfiring=true; queueActivatedRapidfire.push_back( {keycode, key} );   }
                }
                else
                {
                    // pop only this keycode's RapidfireEvent
                    if(!queueActivatedRapidfire.empty())
                    {   queueActivatedRapidfire.erase( std::remove_if(queueActivatedRapidfire.begin(), queueActivatedRapidfire.end(), [keycode](const RapidfireEvent& item){ return item.keycode == keycode; }), queueActivatedRapidfire.end() );   }
                }
                isUpdatedActivatedRapidfire=true;

                isActivatedKeyEvent=false; key=KEY_NONE;
            }
        }

        /*** This key Mapped the MultiKey event ***/
        else if(data.mappedLen > 1)
        {
            if(
                (   *(data.millisPressedTime)==0 &&
                    event
                )
                ||
                (   *(data.millisPressedTime)!=0 &&
                    *(data.millisPressedTime)<MILLIS_FROM_PRESSED_UNTIL_RELEASE &&
                    !event
                )
            )
            {
                if(data.isRequiredCleanse)
                {   KBD_HIJACKER.releaseAllBeingHoldDownKey(); delay(10);   }

                int32_t keys[data.mappedLen];
                for(uint8_t i=0; i<data.mappedLen; i++)
                    keys[i] = keycode_To_TeensyLayout( ((uint8_t*)data.mappedThings)[i] );

                KBD_HIJACKER.pressandreleaseMultiKey(keys, data.mappedLen);
            }
            else
                continue;

            isActivatedKeyEvent=false; key=KEY_NONE;
        }

        /*** KEY to NONE And excute MACRO event ***/
        else
        {
            if(event)
                MODULE_MACRO_START_PLAYER_OR_RECORDER( ((char*)data.mappedThings) );

            isActivatedKeyEvent=false; key=KEY_NONE;
        }


        if(isDEBUG)
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



void KeyboardHijacker::MODULE_KEYMAPPER_RAPIDFIRE()
{
    if(!isRapidfiring)
        return;

    if(numDN==0) // Rapidfire NOT EXCUTES, when no key is pressed or during MACRO events
        queueActivatedRapidfire.clear();


    static bool isTurnPress = true;
    static uint8_t indexRapidfire = 0;
    static int32_t keyRapidfire;

    if(isTurnPress)
    {
        if(queueActivatedRapidfire.empty())
        {   isRapidfiring = false; return;   }
        else
            Keyboard.press( keyRapidfire = queueActivatedRapidfire[ indexRapidfire = isUpdatedActivatedRapidfire?(isUpdatedActivatedRapidfire=false,0):indexRapidfire ].key );
    }
    else
    {
        Keyboard.release( keyRapidfire );

        if(queueActivatedRapidfire.empty())
            isRapidfiring = false;
        else
            indexRapidfire = (indexRapidfire + 1) % queueActivatedRapidfire.size();
    }

    isTurnPress = !isTurnPress;
}
