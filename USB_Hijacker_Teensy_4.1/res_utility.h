#define DISABLE_INTERRUPTS cli()
#define ENABLE_INTERRUPTS sei()



bool isReservedReboot = false;
void REBOOT() { SCB_AIRCR = 0x05FA0004; asm volatile ("dsb"); while(true){} }



extern unsigned long _heap_end; //extern char *__brkval;
void MEASURE_FREE_MEMORY() // for Teensy® 4.1 =32bit!!
{
    char* p=(char*)malloc(10000); free(p); // size should be quite big, to avoid allocating fragment!
    uint32_t freeMemory = (char *)&_heap_end - p;

    Serial.print("\nNow Measured FreeMemory : "); Serial.println(freeMemory); //=__brkval;
}



volatile uint32_t msMeasuredForDebugging;
#define MEASURE_MILLIS_START_POINT()    { msMeasuredForDebugging=millis(); }
#define MEASURE_MILLIS_END_POINT()      { Serial.println("\n#\nMeasured Time : " + String(millis()-msMeasuredForDebugging)); Serial.println("#\n"); }



#define isEquals(STRX,STRY)     (strcmp(STRX,STRY)==0)
#define fillWith(STRX,STRY)     strcpy(STRX,STRY)
#define isContains(STR,CHSTR)   ((STR).indexOf(CHSTR)>-1)



void print8bitHex(uint8_t hexcode)      {   Serial.print("0x"); Serial.print((hexcode<16) ? "0" : ""); Serial.print(hexcode, HEX);   }
void print8bitBin(uint8_t bincode)      {   Serial.print("0b"); for(int i=7; i>=0;i--) Serial.print( (bincode & (1<<i)) ? '1' : '0' );   }



String trimming_str(String str)
{
    int8_t index_Num = -1;
    uint32_t lastIndex_Num = 0;
    
    for(uint32_t i=0; i<str.length(); i++)
    {
        if(index_Num == -1){
            if(!(str[i]==' ' || str[i]=='\'' || str[i]=='\"' || str[i]=='\t' || str[i]=='\r' || str[i]=='\n')){
                index_Num = i;
            }
        }
        else{
            if( (str[i]==' ' || str[i]=='\'' || str[i]=='\"' || str[i]=='\t' || str[i]=='\r' || str[i]=='\n') ){
                lastIndex_Num = i;
                break;
            }
        }
    }

    if(-1 < index_Num) return str.substring(index_Num, lastIndex_Num);
    
    return "0"; // Invalid str !!
}
String trimming_num(String str)
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
String trimming_keycodeStr(String str)
{
    str = str.substring(str.indexOf("KEY"));
    
    uint8_t end_num = 0;
    
    for(uint32_t i=0; i<str.length(); i++)
    {
        if (('0' <= str[i] && str[i] <= '9') || ('A' <= str[i] && str[i] <= 'Z') || str[i] == '_')
            end_num++;
        else
            break;
    }
    
    return str.substring(0,end_num);
}



uint32_t StringDec_To_uint32_t(String str)
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
uint8_t StringHex_To_uint8_t(String str)
{
    str = str.substring(str.indexOf("0X"));
    
    char buf[str.length()+1];
    str.toCharArray(buf,str.length()+1);
    
    char* s = buf+2; // except '0x'
    uint8_t x = 0;
    while(true)
    {
        char c = *s;
        
        if  ('0' <= c && c <= '9'){
            x *= 16;
            x += c - '0'; 
        }
        else
        if  ('A' <= c && c <= 'F'){
            x *= 16;
            x += (c - 'A')+10;
        }
        else
        if  ('a' <= c && c <= 'f'){
            x *= 16;
            x += (c - 'a')+10;
        }
        else break;
        
        s++;
    }
    
    return x;
}



uint8_t keycodeStr_To_uint8_t(String str)
{
    str = trimming_keycodeStr(str);

    static std::map<String, uint8_t> MAP_keycodeStr;
    static bool isMapInit = false;
    if(!isMapInit)
    {
        MAP_keycodeStr["KEY_RESERVED"]           = 0x00;
        MAP_keycodeStr["KEY_ERROR_ROLLOVER"]     = 0x01;
        MAP_keycodeStr["KEY_POST_FAIL"]          = 0x02;
        MAP_keycodeStr["KEY_ERROR_UNDEFINED"]    = 0x03;
        MAP_keycodeStr["KEY_A"]                  = 0x04;
        MAP_keycodeStr["KEY_B"]                  = 0x05;
        MAP_keycodeStr["KEY_C"]                  = 0x06;
        MAP_keycodeStr["KEY_D"]                  = 0x07;
        MAP_keycodeStr["KEY_E"]                  = 0x08;
        MAP_keycodeStr["KEY_F"]                  = 0x09;
        MAP_keycodeStr["KEY_G"]                  = 0x0A;
        MAP_keycodeStr["KEY_H"]                  = 0x0B;
        MAP_keycodeStr["KEY_I"]                  = 0x0C;
        MAP_keycodeStr["KEY_J"]                  = 0x0D;
        MAP_keycodeStr["KEY_K"]                  = 0x0E;
        MAP_keycodeStr["KEY_L"]                  = 0x0F;
        MAP_keycodeStr["KEY_M"]                  = 0x10;
        MAP_keycodeStr["KEY_N"]                  = 0x11;
        MAP_keycodeStr["KEY_O"]                  = 0x12;
        MAP_keycodeStr["KEY_P"]                  = 0x13;
        MAP_keycodeStr["KEY_Q"]                  = 0x14;
        MAP_keycodeStr["KEY_R"]                  = 0x15;
        MAP_keycodeStr["KEY_S"]                  = 0x16;
        MAP_keycodeStr["KEY_T"]                  = 0x17;
        MAP_keycodeStr["KEY_U"]                  = 0x18;
        MAP_keycodeStr["KEY_V"]                  = 0x19;
        MAP_keycodeStr["KEY_W"]                  = 0x1A;
        MAP_keycodeStr["KEY_X"]                  = 0x1B;
        MAP_keycodeStr["KEY_Y"]                  = 0x1C;
        MAP_keycodeStr["KEY_Z"]                  = 0x1D;
        MAP_keycodeStr["KEY_1"]                  = 0x1E;
        MAP_keycodeStr["KEY_2"]                  = 0x1F;
        MAP_keycodeStr["KEY_3"]                  = 0x20;
        MAP_keycodeStr["KEY_4"]                  = 0x21;
        MAP_keycodeStr["KEY_5"]                  = 0x22;
        MAP_keycodeStr["KEY_6"]                  = 0x23;
        MAP_keycodeStr["KEY_7"]                  = 0x24;
        MAP_keycodeStr["KEY_8"]                  = 0x25;
        MAP_keycodeStr["KEY_9"]                  = 0x26;
        MAP_keycodeStr["KEY_0"]                  = 0x27;
        MAP_keycodeStr["KEY_ENTER"]              = 0x28;
        MAP_keycodeStr["KEY_ESC"]                = 0x29;
        MAP_keycodeStr["KEY_BACKSPACE"]          = 0x2A;
        MAP_keycodeStr["KEY_TAB"]                = 0x2B;
        MAP_keycodeStr["KEY_SPACE"]              = 0x2C;
        MAP_keycodeStr["KEY_MINUS"]              = 0x2D;
        MAP_keycodeStr["KEY_EQUAL"]              = 0x2E;
        MAP_keycodeStr["KEY_LEFT_BRACE"]         = 0x2F;
        MAP_keycodeStr["KEY_RIGHT_BRACE"]        = 0x30;
        MAP_keycodeStr["KEY_BACKSLASH"]          = 0x31;
        MAP_keycodeStr["KEY_NON_US_NUM"]         = 0x32;
        MAP_keycodeStr["KEY_SEMICOLON"]          = 0x33;
        MAP_keycodeStr["KEY_QUOTE"]              = 0x34;
        MAP_keycodeStr["KEY_TILDE"]              = 0x35;
        MAP_keycodeStr["KEY_COMMA"]              = 0x36;
        MAP_keycodeStr["KEY_PERIOD"]             = 0x37;
        MAP_keycodeStr["KEY_SLASH"]              = 0x38;
        MAP_keycodeStr["KEY_CAPS_LOCK"]          = 0x39;
        MAP_keycodeStr["KEY_CAPSLOCK"]           = 0x39;
        MAP_keycodeStr["KEY_F1"]                 = 0x3A;
        MAP_keycodeStr["KEY_F2"]                 = 0x3B;
        MAP_keycodeStr["KEY_F3"]                 = 0x3C;
        MAP_keycodeStr["KEY_F4"]                 = 0x3D;
        MAP_keycodeStr["KEY_F5"]                 = 0x3E;
        MAP_keycodeStr["KEY_F6"]                 = 0x3F;
        MAP_keycodeStr["KEY_F7"]                 = 0x40;
        MAP_keycodeStr["KEY_F8"]                 = 0x41;
        MAP_keycodeStr["KEY_F9"]                 = 0x42;
        MAP_keycodeStr["KEY_F10"]                = 0x43;
        MAP_keycodeStr["KEY_F11"]                = 0x44;
        MAP_keycodeStr["KEY_F12"]                = 0x45;
        MAP_keycodeStr["KEY_PRINTSCREEN"]        = 0x46;
        MAP_keycodeStr["KEY_SCROLL_LOCK"]        = 0x47;
        MAP_keycodeStr["KEY_SCROLLLOCK"]         = 0x47;
        MAP_keycodeStr["KEY_PAUSE"]              = 0x48;
        MAP_keycodeStr["KEY_INSERT"]             = 0x49;
        MAP_keycodeStr["KEY_HOME"]               = 0x4A;
        MAP_keycodeStr["KEY_PAGE_UP"]            = 0x4B;
        MAP_keycodeStr["KEY_DELETE"]             = 0x4C;
        MAP_keycodeStr["KEY_END"]                = 0x4D;
        MAP_keycodeStr["KEY_PAGE_DOWN"]          = 0x4E;
        MAP_keycodeStr["KEY_RIGHT_ARROW"]        = 0x4F;
        MAP_keycodeStr["KEY_LEFT_ARROW"]         = 0x50;
        MAP_keycodeStr["KEY_DOWN_ARROW"]         = 0x51;
        MAP_keycodeStr["KEY_UP_ARROW"]           = 0x52;
        MAP_keycodeStr["KEY_RIGHT"]              = 0x4F;
        MAP_keycodeStr["KEY_LEFT"]               = 0x50;
        MAP_keycodeStr["KEY_DOWN"]               = 0x51;
        MAP_keycodeStr["KEY_UP"]                 = 0x52;
        MAP_keycodeStr["KEY_NUM_LOCK"]           = 0x53;
        MAP_keycodeStr["KEY_NUMLOCK"]            = 0x53;
        MAP_keycodeStr["KEYPAD_DIVIDE"]          = 0x54;
        MAP_keycodeStr["KEYPAD_SLASH"]           = 0x54;
        MAP_keycodeStr["KEYPAD_MULTIPLY"]        = 0x55;
        MAP_keycodeStr["KEYPAD_ASTERIX"]         = 0x55;
        MAP_keycodeStr["KEYPAD_SUBTRACT"]        = 0x56;
        MAP_keycodeStr["KEYPAD_MINUS"]           = 0x56;
        MAP_keycodeStr["KEYPAD_ADD"]             = 0x57;
        MAP_keycodeStr["KEYPAD_PLUS"]            = 0x57;
        MAP_keycodeStr["KEYPAD_ENTER"]           = 0x58;
        MAP_keycodeStr["KEYPAD_1"]               = 0x59;
        MAP_keycodeStr["KEYPAD_2"]               = 0x5A;
        MAP_keycodeStr["KEYPAD_3"]               = 0x5B;
        MAP_keycodeStr["KEYPAD_4"]               = 0x5C;
        MAP_keycodeStr["KEYPAD_5"]               = 0x5D;
        MAP_keycodeStr["KEYPAD_6"]               = 0x5E;
        MAP_keycodeStr["KEYPAD_7"]               = 0x5F;
        MAP_keycodeStr["KEYPAD_8"]               = 0x60;
        MAP_keycodeStr["KEYPAD_9"]               = 0x61;
        MAP_keycodeStr["KEYPAD_0"]               = 0x62;
        MAP_keycodeStr["KEYPAD_DOT"]             = 0x63;
        MAP_keycodeStr["KEYPAD_PERIOD"]          = 0x63;
        MAP_keycodeStr["KEY_NON_US"]             = 0x64;
        MAP_keycodeStr["KEY_APPLICATION"]        = 0x65;
        MAP_keycodeStr["KEY_CONTEXT"]            = 0x65;
        MAP_keycodeStr["KEY_MENU"]               = 0x65;
        
        MAP_keycodeStr["KEY_KORENG"]             = 0x90;
        
        MAP_keycodeStr["KEY_CTRL"]               = 0x67;
        MAP_keycodeStr["KEY_SHIFT"]              = 0x68;
        MAP_keycodeStr["KEY_ALT"]                = 0x69;
        MAP_keycodeStr["KEY_GUI"]                = 0x6A;
        MAP_keycodeStr["KEY_WINDOWS"]            = 0x6A;
        MAP_keycodeStr["KEY_LEFT_CTRL"]          = 0x67;
        MAP_keycodeStr["KEY_LEFT_SHIFT"]         = 0x68;
        MAP_keycodeStr["KEY_LEFT_ALT"]           = 0x69;
        MAP_keycodeStr["KEY_LEFT_GUI"]           = 0x6A;
        MAP_keycodeStr["KEY_LEFT_WINDOWS"]       = 0x6A;
        MAP_keycodeStr["KEY_RIGHT_CTRL"]         = 0x6B;
        MAP_keycodeStr["KEY_RIGHT_SHIFT"]        = 0x6C;
        MAP_keycodeStr["KEY_RIGHT_ALT"]          = 0x6D;
        MAP_keycodeStr["KEY_RIGHT_GUI"]          = 0x6E;
        MAP_keycodeStr["KEY_RIGHT_WINDOWS"]      = 0x6E;

        MAP_keycodeStr["KEY_SYSTEM_POWER_DOWN"]  = 0x81;
        MAP_keycodeStr["KEY_SYSTEM_SLEEP"]       = 0x82;
        MAP_keycodeStr["KEY_SYSTEM_WAKE_UP"]     = 0x83;
        
        MAP_keycodeStr["KEY_MEDIA_PLAY"]         = 0xB0;
        MAP_keycodeStr["KEY_MEDIA_PAUSE"]        = 0xB1;
        MAP_keycodeStr["KEY_MEDIA_RECORD"]       = 0xB2;
        MAP_keycodeStr["KEY_MEDIA_FAST_FORWARD"] = 0xB3;
        MAP_keycodeStr["KEY_MEDIA_REWIND"]       = 0xB4;
        MAP_keycodeStr["KEY_MEDIA_NEXT_TRACK"]   = 0xB5;
        MAP_keycodeStr["KEY_MEDIA_PREV_TRACK"]   = 0xB6;
        MAP_keycodeStr["KEY_MEDIA_STOP"]         = 0xB7;
        MAP_keycodeStr["KEY_MEDIA_EJECT"]        = 0xB8;
        MAP_keycodeStr["KEY_MEDIA_RANDOM_PLAY"]  = 0xB9;
        MAP_keycodeStr["KEY_MEDIA_PLAY_PAUSE"]   = 0xCD;
        MAP_keycodeStr["KEY_MEDIA_PLAY_SKIP"]    = 0xCE;
        MAP_keycodeStr["KEY_MEDIA_MUTE"]         = 0xE2;
        MAP_keycodeStr["KEY_MEDIA_VOLUME_INC"]   = 0xE9;
        MAP_keycodeStr["KEY_MEDIA_VOLUME_DEC"]   = 0xEA;

        isMapInit = true;
    }
    
    if(MAP_keycodeStr.find( str ) == MAP_keycodeStr.end())
        return 0; // Invalid str !!

    return MAP_keycodeStr[ trimming_keycodeStr(str) ];
}
uint8_t String_To_keycode(String str)
{
    if (str.indexOf("0X") > -1)
        return StringHex_To_uint8_t(str);

    if (str.indexOf("KEY") > -1)
        return keycodeStr_To_uint8_t(str);

    return 0; // Invalid str !!
}