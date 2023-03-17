
static int32_t keycode_To_TeensyLayout(uint8_t keycode)
{
    if      (keycode <= 0x65)
        return ( keycode | 0xF000 );

    else if (0x81 <= keycode && keycode <= 0x83)
        return ( keycode | 0xE200 );

    else if (0xB0 <= keycode && keycode <= 0xEA)
        return ( keycode | 0xE400 );

    else if (0x67 <= keycode && keycode <= 0x6E)
        return ( (1 << (keycode-0x67)) | 0xE000 );

//  MODIFIERKEY_LEFT_CTRL   ( 0x01 | 0xE000 )   1
//  MODIFIERKEY_LEFT_SHIFT  ( 0x02 | 0xE000 )   2
//  MODIFIERKEY_LEFT_ALT    ( 0x04 | 0xE000 )   4
//  MODIFIERKEY_LEFT_GUI    ( 0x08 | 0xE000 )   8
//  MODIFIERKEY_RIGHT_CTRL  ( 0x10 | 0xE000 )   16
//  MODIFIERKEY_RIGHT_SHIFT ( 0x20 | 0xE000 )   32
//  MODIFIERKEY_RIGHT_ALT   ( 0x40 | 0xE000 )   64
//  MODIFIERKEY_RIGHT_GUI   ( 0x80 | 0xE000 )   128
//
//    else if (keycode == 0x67)
//        return ( MODIFIERKEY_LEFT_CTRL );
//    else if (keycode == 0x68)
//        return ( MODIFIERKEY_LEFT_SHIFT );
//    else if (keycode == 0x69)
//        return ( MODIFIERKEY_LEFT_ALT );
//    else if (keycode == 0x6A)
//        return ( MODIFIERKEY_LEFT_GUI );
//    else if (keycode == 0x6B)
//        return ( MODIFIERKEY_RIGHT_CTRL );
//    else if (keycode == 0x6C)
//        return ( MODIFIERKEY_RIGHT_SHIFT );
//    else if (keycode == 0x6D)
//        return ( MODIFIERKEY_RIGHT_ALT );
//    else if (keycode == 0x6E)
//        return ( MODIFIERKEY_RIGHT_GUI );
        
    else if (keycode == 0x90) //  KEY_KORENG
        return ( MODIFIERKEY_RIGHT_ALT );

    return 0;
}

static uint8_t TeensyLayout_To_Keycode(int32_t key)
{
    if      (0xF000 <= key && key <= 0xF065)
        return ( key & ~(0xF000) );
        
    else if (0xE281 <= key && key <= 0xE283)
        return ( key & ~(0xE200) );
        
    else if (0xE4B0 <= key && key <= 0xE4EA)
        return ( key & ~(0xE400) );




        
//    else if (0xE001 <= key && key <= 0xE080)
//        return ( key & ~(0xE000) );


//    else if (0x67 <= keycode && keycode <= 0x6E)



        
    else if (key == MODIFIERKEY_LEFT_CTRL)
        return 0x67;
    else if (key == MODIFIERKEY_LEFT_SHIFT)
        return 0x68;
    else if (key == MODIFIERKEY_LEFT_ALT)
        return 0x69;
    else if (key == MODIFIERKEY_LEFT_GUI)
        return 0x6A;
    else if (key == MODIFIERKEY_RIGHT_CTRL)
        return 0x6B;
    else if (key == MODIFIERKEY_RIGHT_SHIFT)
        return 0x6C;
    else if (key == MODIFIERKEY_RIGHT_ALT)
        return 0x6D;
    else if (key == MODIFIERKEY_RIGHT_GUI)
        return 0x6E;

    return 0;
}





uint32_t StringHex_To_int(String str) 
{
    char buf[str.length()+1];
    str.toCharArray(buf,str.length()+1);
    
    char* s = buf+2; // except '0x'
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

#include <map>

uint8_t keycodeStr_To_uint8_t(String str)
{
    static std::map<String, uint8_t> keycodeStrMap;
    static bool isMapInit = false;
    if(!isMapInit)
    {
        keycodeStrMap["KEY_RESERVED"]           = 0x00;
        keycodeStrMap["KEY_ERROR_ROLLOVER"]     = 0x01;
        keycodeStrMap["KEY_POST_FAIL"]          = 0x02;
        keycodeStrMap["KEY_ERROR_UNDEFINED"]    = 0x03;
        keycodeStrMap["KEY_A"]                  = 0x04;
        keycodeStrMap["KEY_B"]                  = 0x05;
        keycodeStrMap["KEY_C"]                  = 0x06;
        keycodeStrMap["KEY_D"]                  = 0x07;
        keycodeStrMap["KEY_E"]                  = 0x08;
        keycodeStrMap["KEY_F"]                  = 0x09;
        keycodeStrMap["KEY_G"]                  = 0x0A;
        keycodeStrMap["KEY_H"]                  = 0x0B;
        keycodeStrMap["KEY_I"]                  = 0x0C;
        keycodeStrMap["KEY_J"]                  = 0x0D;
        keycodeStrMap["KEY_K"]                  = 0x0E;
        keycodeStrMap["KEY_L"]                  = 0x0F;
        keycodeStrMap["KEY_M"]                  = 0x10;
        keycodeStrMap["KEY_N"]                  = 0x11;
        keycodeStrMap["KEY_O"]                  = 0x12;
        keycodeStrMap["KEY_P"]                  = 0x13;
        keycodeStrMap["KEY_Q"]                  = 0x14;
        keycodeStrMap["KEY_R"]                  = 0x15;
        keycodeStrMap["KEY_S"]                  = 0x16;
        keycodeStrMap["KEY_T"]                  = 0x17;
        keycodeStrMap["KEY_U"]                  = 0x18;
        keycodeStrMap["KEY_V"]                  = 0x19;
        keycodeStrMap["KEY_W"]                  = 0x1A;
        keycodeStrMap["KEY_X"]                  = 0x1B;
        keycodeStrMap["KEY_Y"]                  = 0x1C;
        keycodeStrMap["KEY_Z"]                  = 0x1D;
        keycodeStrMap["KEY_1"]                  = 0x1E;
        keycodeStrMap["KEY_2"]                  = 0x1F;
        keycodeStrMap["KEY_3"]                  = 0x20;
        keycodeStrMap["KEY_4"]                  = 0x21;
        keycodeStrMap["KEY_5"]                  = 0x22;
        keycodeStrMap["KEY_6"]                  = 0x23;
        keycodeStrMap["KEY_7"]                  = 0x24;
        keycodeStrMap["KEY_8"]                  = 0x25;
        keycodeStrMap["KEY_9"]                  = 0x26;
        keycodeStrMap["KEY_0"]                  = 0x27;
        keycodeStrMap["KEY_ENTER"]              = 0x28;
        keycodeStrMap["KEY_ESC"]                = 0x29;
        keycodeStrMap["KEY_BACKSPACE"]          = 0x2A;
        keycodeStrMap["KEY_TAB"]                = 0x2B;
        keycodeStrMap["KEY_SPACE"]              = 0x2C;
        keycodeStrMap["KEY_MINUS"]              = 0x2D;
        keycodeStrMap["KEY_EQUAL"]              = 0x2E;
        keycodeStrMap["KEY_LEFT_BRACE"]         = 0x2F;
        keycodeStrMap["KEY_RIGHT_BRACE"]        = 0x30;
        keycodeStrMap["KEY_BACKSLASH"]          = 0x31;
        keycodeStrMap["KEY_NON_US_NUM"]         = 0x32;
        keycodeStrMap["KEY_SEMICOLON"]          = 0x33;
        keycodeStrMap["KEY_QUOTE"]              = 0x34;
        keycodeStrMap["KEY_TILDE"]              = 0x35;
        keycodeStrMap["KEY_COMMA"]              = 0x36;
        keycodeStrMap["KEY_PERIOD"]             = 0x37;
        keycodeStrMap["KEY_SLASH"]              = 0x38;
        keycodeStrMap["KEY_CAPS_LOCK"]          = 0x39;
        keycodeStrMap["KEY_CAPSLOCK"]           = 0x39;
        keycodeStrMap["KEY_F1"]                 = 0x3A;
        keycodeStrMap["KEY_F2"]                 = 0x3B;
        keycodeStrMap["KEY_F3"]                 = 0x3C;
        keycodeStrMap["KEY_F4"]                 = 0x3D;
        keycodeStrMap["KEY_F5"]                 = 0x3E;
        keycodeStrMap["KEY_F6"]                 = 0x3F;
        keycodeStrMap["KEY_F7"]                 = 0x40;
        keycodeStrMap["KEY_F8"]                 = 0x41;
        keycodeStrMap["KEY_F9"]                 = 0x42;
        keycodeStrMap["KEY_F10"]                = 0x43;
        keycodeStrMap["KEY_F11"]                = 0x44;
        keycodeStrMap["KEY_F12"]                = 0x45;
        keycodeStrMap["KEY_PRINTSCREEN"]        = 0x46;
        keycodeStrMap["KEY_SCROLL_LOCK"]        = 0x47;
        keycodeStrMap["KEY_SCROLLLOCK"]         = 0x47;
        keycodeStrMap["KEY_PAUSE"]              = 0x48;
        keycodeStrMap["KEY_INSERT"]             = 0x49;
        keycodeStrMap["KEY_HOME"]               = 0x4A;
        keycodeStrMap["KEY_PAGE_UP"]            = 0x4B;
        keycodeStrMap["KEY_DELETE"]             = 0x4C;
        keycodeStrMap["KEY_END"]                = 0x4D;
        keycodeStrMap["KEY_PAGE_DOWN"]          = 0x4E;
        keycodeStrMap["KEY_RIGHT_ARROW"]        = 0x4F;
        keycodeStrMap["KEY_LEFT_ARROW"]         = 0x50;
        keycodeStrMap["KEY_DOWN_ARROW"]         = 0x51;
        keycodeStrMap["KEY_UP_ARROW"]           = 0x52;
        keycodeStrMap["KEY_RIGHT"]              = 0x4F;
        keycodeStrMap["KEY_LEFT"]               = 0x50;
        keycodeStrMap["KEY_DOWN"]               = 0x51;
        keycodeStrMap["KEY_UP"]                 = 0x52;
        keycodeStrMap["KEY_NUM_LOCK"]           = 0x53;
        keycodeStrMap["KEY_NUMLOCK"]            = 0x53;
        keycodeStrMap["KEYPAD_DIVIDE"]          = 0x54;
        keycodeStrMap["KEYPAD_SLASH"]           = 0x54;
        keycodeStrMap["KEYPAD_MULTIPLY"]        = 0x55;
        keycodeStrMap["KEYPAD_ASTERIX"]         = 0x55;
        keycodeStrMap["KEYPAD_SUBTRACT"]        = 0x56;
        keycodeStrMap["KEYPAD_MINUS"]           = 0x56;
        keycodeStrMap["KEYPAD_ADD"]             = 0x57;
        keycodeStrMap["KEYPAD_PLUS"]            = 0x57;
        keycodeStrMap["KEYPAD_ENTER"]           = 0x58;
        keycodeStrMap["KEYPAD_1"]               = 0x59;
        keycodeStrMap["KEYPAD_2"]               = 0x5A;
        keycodeStrMap["KEYPAD_3"]               = 0x5B;
        keycodeStrMap["KEYPAD_4"]               = 0x5C;
        keycodeStrMap["KEYPAD_5"]               = 0x5D;
        keycodeStrMap["KEYPAD_6"]               = 0x5E;
        keycodeStrMap["KEYPAD_7"]               = 0x5F;
        keycodeStrMap["KEYPAD_8"]               = 0x60;
        keycodeStrMap["KEYPAD_9"]               = 0x61;
        keycodeStrMap["KEYPAD_0"]               = 0x62;
        keycodeStrMap["KEYPAD_DOT"]             = 0x63;
        keycodeStrMap["KEYPAD_PERIOD"]          = 0x63;
        keycodeStrMap["KEY_NON_US"]             = 0x64;
        keycodeStrMap["KEY_APPLICATION"]        = 0x65;
        keycodeStrMap["KEY_CONTEXT"]            = 0x65;
        keycodeStrMap["KEY_MENU"]               = 0x65;
        
        keycodeStrMap["KEY_KORENG"]             = 0x90;
        
        keycodeStrMap["KEY_CTRL"]               = 0x67;
        keycodeStrMap["KEY_SHIFT"]              = 0x68;
        keycodeStrMap["KEY_ALT"]                = 0x69;
        keycodeStrMap["KEY_GUI"]                = 0x6A;
        keycodeStrMap["KEY_WINDOWS"]            = 0x6A;
        keycodeStrMap["KEY_LEFT_CTRL"]          = 0x67;
        keycodeStrMap["KEY_LEFT_SHIFT"]         = 0x68;
        keycodeStrMap["KEY_LEFT_ALT"]           = 0x69;
        keycodeStrMap["KEY_LEFT_GUI"]           = 0x6A;
        keycodeStrMap["KEY_LEFT_WINDOWS"]       = 0x6A;
        keycodeStrMap["KEY_RIGHT_CTRL"]         = 0x6B;
        keycodeStrMap["KEY_RIGHT_SHIFT"]        = 0x6C;
        keycodeStrMap["KEY_RIGHT_ALT"]          = 0x6D;
        keycodeStrMap["KEY_RIGHT_GUI"]          = 0x6E;
        keycodeStrMap["KEY_RIGHT_WINDOWS"]      = 0x6E;

        isMapInit = true;
    }
    
    char buf[str.length()+1];
    str.toCharArray(buf,str.length()+1);
    
    char* s = buf;
    uint8_t end_num = 0;
    while(true)
    {
        char c = *s;
        
        if (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || c == '_')
            end_num++;
        else
            break;
        
        s++;
    }    

    if(0<end_num) return keycodeStrMap[ str.substring(0,end_num) ];

    return 0; // Invalid str !!
}

/*
// AMAZING DEFINE !! https://oojjrs.tistory.com/25
#define STR_SWITCH_BEGIN(key)                               \
{                                                           \
    static std::map<String, uint32_t> mapper;               \
    static bool s_bInit = false;                            \
    bool bLoop = true;                                      \
    while(bLoop)                                            \
    {                                                       \
        int nKey = -1;                                      \
        if(s_bInit) { nKey=mapper[key]; bLoop = false; }    \
        switch(nKey)                                        \
        {                                                   \
            case -1:                                        \
            {
#define CASE(token)  } case __LINE__: if(!s_bInit) mapper[token] = __LINE__; else {
#define DEFAULT()    } case 0: default: if(s_bInit) {
#define STR_SWITCH_END()            \
            }                       \
        }                           \
        if(!s_bInit) s_bInit=true;  \
    }                               \
}

String split_keycodeStr(String str)
{
    char buf[str.length()+1];
    str.toCharArray(buf,str.length()+1);
    
    char* s = buf;
    uint8_t end_num = 0;
    while(true)
    {
        char c = *s;
        
        if (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || c == '_')
            end_num++;
        else
            break;
        
        s++;
    }

    if(0<end_num) return str.substring(0,end_num);

    return "0"; // Invalid str !!
}

uint8_t keycodeStr_To_keycode(String keycodeStr)
{
    STR_SWITCH_BEGIN(keycodeStr)
    {
        CASE("KEY_RESERVED")        return 0x00;
        CASE("KEY_ERROR_ROLLOVER")  return 0x01;
        CASE("KEY_POST_FAIL")       return 0x02;
        CASE("KEY_ERROR_UNDEFINED") return 0x03;
        CASE("KEY_A")               return 0x04;
        CASE("KEY_B")               return 0x05;
        CASE("KEY_C")               return 0x06;
        ...
    }
    STR_SWITCH_END();

    return 0; // Invalid keycodeStr !!
}
*/

uint8_t String_To_keycode(String str)
{
    if (str.indexOf("0X") >= 0)
    {
        //Serial.println("*************************"); Serial.println(StringHex_To_int(   str.substring(str.indexOf("0X"))   ));

        return StringHex_To_int(   str.substring(str.indexOf("0X"))   );
    }

    if (str.indexOf("KEY") >= 0)
    {
        //Serial.println("*************************"); Serial.println(keycodeStr_To_uint8_t( str.substring(str.indexOf("KEY")) ));

        return keycodeStr_To_uint8_t( str.substring(str.indexOf("KEY")) );
    }

    return 0; // Invalid str !!
}





/************************************************
 **                                            **
 ** OnRawPress & OnRawRelease's keycode layout **
 **                                            **
 ************************************************/

//  KEY_RESERVED        0x00
//  KEY_ERROR_ROLLOVER  0x01
//  KEY_POST_FAIL       0x02
//  KEY_ERROR_UNDEFINED 0x03
//  KEY_A               0x04
//  KEY_B               0x05
//  KEY_C               0x06
//  KEY_D               0x07
//  KEY_E               0x08
//  KEY_F               0x09
//  KEY_G               0x0A
//  KEY_H               0x0B
//  KEY_I               0x0C
//  KEY_J               0x0D
//  KEY_K               0x0E
//  KEY_L               0x0F
//  KEY_M               0x10
//  KEY_N               0x11
//  KEY_O               0x12
//  KEY_P               0x13
//  KEY_Q               0x14
//  KEY_R               0x15
//  KEY_S               0x16
//  KEY_T               0x17
//  KEY_U               0x18
//  KEY_V               0x19
//  KEY_W               0x1A
//  KEY_X               0x1B
//  KEY_Y               0x1C
//  KEY_Z               0x1D
//  KEY_1               0x1E
//  KEY_2               0x1F
//  KEY_3               0x20
//  KEY_4               0x21
//  KEY_5               0x22
//  KEY_6               0x23
//  KEY_7               0x24
//  KEY_8               0x25
//  KEY_9               0x26
//  KEY_0               0x27
//  KEY_ENTER           0x28
//  KEY_ESC             0x29
//  KEY_BACKSPACE       0x2A
//  KEY_TAB             0x2B
//  KEY_SPACE           0x2C
//  KEY_MINUS           0x2D //   '-'
//  KEY_EQUAL           0x2E //   '='
//  KEY_LEFT_BRACE      0x2F //   '['
//  KEY_RIGHT_BRACE     0x30 //   ']'
//  KEY_BACKSLASH       0x31 //   '\'
//  KEY_NON_US_NUM      0x32 //   Don't Know 1
//  KEY_SEMICOLON       0x33 //   ';'
//  KEY_QUOTE           0x34 //   '''
//  KEY_TILDE           0x35 //   '`'
//  KEY_COMMA           0x36 //   ','
//  KEY_PERIOD          0x37 //   '.'
//  KEY_SLASH           0x38 //   '/'
//  KEY_CAPS_LOCK       0x39
//  KEY_CAPSLOCK        0x39 //   Alias
//  KEY_F1              0x3A
//  KEY_F2              0x3B
//  KEY_F3              0x3C
//  KEY_F4              0x3D
//  KEY_F5              0x3E
//  KEY_F6              0x3F
//  KEY_F7              0x40
//  KEY_F8              0x41
//  KEY_F9              0x42
//  KEY_F10             0x43
//  KEY_F11             0x44
//  KEY_F12             0x45
//  KEY_PRINTSCREEN     0x46
//  KEY_SCROLL_LOCK     0x47
//  KEY_SCROLLLOCK      0x47 //   Alias
//  KEY_PAUSE           0x48
//  KEY_INSERT          0x49
//  KEY_HOME            0x4A
//  KEY_PAGE_UP         0x4B
//  KEY_DELETE          0x4C
//  KEY_END             0x4D
//  KEY_PAGE_DOWN       0x4E
//  KEY_RIGHT_ARROW     0x4F
//  KEY_LEFT_ARROW      0x50
//  KEY_DOWN_ARROW      0x51
//  KEY_UP_ARROW        0x52
//  KEY_RIGHT           0x4F //   Alias
//  KEY_LEFT            0x50 //   Alias
//  KEY_DOWN            0x51 //   Alias
//  KEY_UP              0x52 //   Alias
//  KEY_NUM_LOCK        0x53
//  KEY_NUMLOCK         0x53 //   Alias
//  KEYPAD_DIVIDE       0x54 //   '/'
//  KEYPAD_MULTIPLY     0x55 //   '*'
//  KEYPAD_SUBTRACT     0x56 //   '-'
//  KEYPAD_ADD          0x57 //   '+'
//  KEYPAD_ENTER        0x58
//  KEYPAD_1            0x59
//  KEYPAD_2            0x5A
//  KEYPAD_3            0x5B
//  KEYPAD_4            0x5C
//  KEYPAD_5            0x5D
//  KEYPAD_6            0x5E
//  KEYPAD_7            0x5F
//  KEYPAD_8            0x60
//  KEYPAD_9            0x61
//  KEYPAD_0            0x62
//  KEYPAD_DOT          0x63 //   '.'
//  KEY_NON_US          0x64 //   Don't Know 2
//  KEY_APPLICATION     0x65
//  KEY_CONTEXT         0x65 //   Alias
//  KEY_MENU            0x65 //   Alias
//  
//  KEY_CTRL            0x67
//  KEY_SHIFT           0x68
//  KEY_ALT             0x69
//  KEY_GUI             0x6A
//  KEY_WINDOWS         0x6A //   Alias
//  KEY_LEFT_CTRL       0x67 //   Alias
//  KEY_LEFT_SHIFT      0x68 //   Alias
//  KEY_LEFT_ALT        0x69 //   Alias
//  KEY_LEFT_GUI        0x6A //   Alias
//  KEY_LEFT_WINDOWS    0x6A //   Alias
//  KEY_RIGHT_CTRL      0x6B
//  KEY_RIGHT_SHIFT     0x6C
//  KEY_RIGHT_ALT       0x6D
//  KEY_RIGHT_GUI       0x6E
//  KEY_RIGHT_WINDOWS   0x6E //   Alias
//  
//  KEY_KORENG          0x90 //   KOR-ENG Swap





/******************************************************************
 **                                                              **
 **                    Teensy4.1's key layout                    **
 **                                                              **
 ** ArduinoFolder\hardware\teensy\avr\cores\teensy4\keylayouts.h **
 **                                                              **
 ******************************************************************/

//  MODIFIERKEY_CTRL        ( 0x01 | 0xE000 )
//  MODIFIERKEY_CTRL        ( 0x01 | 0xE000 )
//  MODIFIERKEY_SHIFT       ( 0x02 | 0xE000 )
//  MODIFIERKEY_ALT         ( 0x04 | 0xE000 )
//  MODIFIERKEY_GUI         ( 0x08 | 0xE000 )
//  MODIFIERKEY_LEFT_CTRL   ( 0x01 | 0xE000 )
//  MODIFIERKEY_LEFT_SHIFT  ( 0x02 | 0xE000 )
//  MODIFIERKEY_LEFT_ALT    ( 0x04 | 0xE000 )
//  MODIFIERKEY_LEFT_GUI    ( 0x08 | 0xE000 )
//  MODIFIERKEY_RIGHT_CTRL  ( 0x10 | 0xE000 )
//  MODIFIERKEY_RIGHT_SHIFT ( 0x20 | 0xE000 )
//  MODIFIERKEY_RIGHT_ALT   ( 0x40 | 0xE000 )
//  MODIFIERKEY_RIGHT_GUI   ( 0x80 | 0xE000 )
//  
//  KEY_SYSTEM_POWER_DOWN   ( 0x81 | 0xE200 )
//  KEY_SYSTEM_SLEEP        ( 0x82 | 0xE200 )
//  KEY_SYSTEM_WAKE_UP      ( 0x83 | 0xE200 )
//  
//  KEY_MEDIA_PLAY          ( 0xB0 | 0xE400 )
//  KEY_MEDIA_PAUSE         ( 0xB1 | 0xE400 )
//  KEY_MEDIA_RECORD        ( 0xB2 | 0xE400 )
//  KEY_MEDIA_FAST_FORWARD  ( 0xB3 | 0xE400 )
//  KEY_MEDIA_REWIND        ( 0xB4 | 0xE400 )
//  KEY_MEDIA_NEXT_TRACK    ( 0xB5 | 0xE400 )
//  KEY_MEDIA_PREV_TRACK    ( 0xB6 | 0xE400 )
//  KEY_MEDIA_STOP          ( 0xB7 | 0xE400 )
//  KEY_MEDIA_EJECT         ( 0xB8 | 0xE400 )
//  KEY_MEDIA_RANDOM_PLAY   ( 0xB9 | 0xE400 )
//  KEY_MEDIA_PLAY_PAUSE    ( 0xCD | 0xE400 )
//  KEY_MEDIA_PLAY_SKIP     ( 0xCE | 0xE400 )
//  KEY_MEDIA_MUTE          ( 0xE2 | 0xE400 )
//  KEY_MEDIA_VOLUME_INC    ( 0xE9 | 0xE400 )
//  KEY_MEDIA_VOLUME_DEC    ( 0xEA | 0xE400 )
//  
//  KEY_A                   (   4  | 0xF000 )
//  KEY_B                   (   5  | 0xF000 )
//  KEY_C                   (   6  | 0xF000 )
//  KEY_D                   (   7  | 0xF000 )
//  KEY_E                   (   8  | 0xF000 )
//  KEY_F                   (   9  | 0xF000 )
//  KEY_G                   (  10  | 0xF000 )
//  KEY_H                   (  11  | 0xF000 )
//  KEY_I                   (  12  | 0xF000 )
//  KEY_J                   (  13  | 0xF000 )
//  KEY_K                   (  14  | 0xF000 )
//  KEY_L                   (  15  | 0xF000 )
//  KEY_M                   (  16  | 0xF000 )
//  KEY_N                   (  17  | 0xF000 )
//  KEY_O                   (  18  | 0xF000 )
//  KEY_P                   (  19  | 0xF000 )
//  KEY_Q                   (  20  | 0xF000 )
//  KEY_R                   (  21  | 0xF000 )
//  KEY_S                   (  22  | 0xF000 )
//  KEY_T                   (  23  | 0xF000 )
//  KEY_U                   (  24  | 0xF000 )
//  KEY_V                   (  25  | 0xF000 )
//  KEY_W                   (  26  | 0xF000 )
//  KEY_X                   (  27  | 0xF000 )
//  KEY_Y                   (  28  | 0xF000 )
//  KEY_Z                   (  29  | 0xF000 )
//  KEY_1                   (  30  | 0xF000 )
//  KEY_2                   (  31  | 0xF000 )
//  KEY_3                   (  32  | 0xF000 )
//  KEY_4                   (  33  | 0xF000 )
//  KEY_5                   (  34  | 0xF000 )
//  KEY_6                   (  35  | 0xF000 )
//  KEY_7                   (  36  | 0xF000 )
//  KEY_8                   (  37  | 0xF000 )
//  KEY_9                   (  38  | 0xF000 )
//  KEY_0                   (  39  | 0xF000 )
//  KEY_ENTER               (  40  | 0xF000 )
//  KEY_ESC                 (  41  | 0xF000 )
//  KEY_BACKSPACE           (  42  | 0xF000 )
//  KEY_TAB                 (  43  | 0xF000 )
//  KEY_SPACE               (  44  | 0xF000 )
//  KEY_MINUS               (  45  | 0xF000 )
//  KEY_EQUAL               (  46  | 0xF000 )
//  KEY_LEFT_BRACE          (  47  | 0xF000 )
//  KEY_RIGHT_BRACE         (  48  | 0xF000 )
//  KEY_BACKSLASH           (  49  | 0xF000 )
//  KEY_NON_US_NUM          (  50  | 0xF000 )
//  KEY_SEMICOLON           (  51  | 0xF000 )
//  KEY_QUOTE               (  52  | 0xF000 )
//  KEY_TILDE               (  53  | 0xF000 )
//  KEY_COMMA               (  54  | 0xF000 )
//  KEY_PERIOD              (  55  | 0xF000 )
//  KEY_SLASH               (  56  | 0xF000 )
//  KEY_CAPS_LOCK           (  57  | 0xF000 )
//  KEY_F1                  (  58  | 0xF000 )
//  KEY_F2                  (  59  | 0xF000 )
//  KEY_F3                  (  60  | 0xF000 )
//  KEY_F4                  (  61  | 0xF000 )
//  KEY_F5                  (  62  | 0xF000 )
//  KEY_F6                  (  63  | 0xF000 )
//  KEY_F7                  (  64  | 0xF000 )
//  KEY_F8                  (  65  | 0xF000 )
//  KEY_F9                  (  66  | 0xF000 )
//  KEY_F10                 (  67  | 0xF000 )
//  KEY_F11                 (  68  | 0xF000 )
//  KEY_F12                 (  69  | 0xF000 )
//  KEY_PRINTSCREEN         (  70  | 0xF000 )
//  KEY_SCROLL_LOCK         (  71  | 0xF000 )
//  KEY_PAUSE               (  72  | 0xF000 )
//  KEY_INSERT              (  73  | 0xF000 )
//  KEY_HOME                (  74  | 0xF000 )
//  KEY_PAGE_UP             (  75  | 0xF000 )
//  KEY_DELETE              (  76  | 0xF000 )
//  KEY_END                 (  77  | 0xF000 )
//  KEY_PAGE_DOWN           (  78  | 0xF000 )
//  KEY_RIGHT               (  79  | 0xF000 )
//  KEY_LEFT                (  80  | 0xF000 )
//  KEY_DOWN                (  81  | 0xF000 )
//  KEY_UP                  (  82  | 0xF000 )
//  KEY_NUM_LOCK            (  83  | 0xF000 )
//  KEYPAD_SLASH            (  84  | 0xF000 )
//  KEYPAD_ASTERIX          (  85  | 0xF000 )
//  KEYPAD_MINUS            (  86  | 0xF000 )
//  KEYPAD_PLUS             (  87  | 0xF000 )
//  KEYPAD_ENTER            (  88  | 0xF000 )
//  KEYPAD_1                (  89  | 0xF000 )
//  KEYPAD_2                (  90  | 0xF000 )
//  KEYPAD_3                (  91  | 0xF000 )
//  KEYPAD_4                (  92  | 0xF000 )
//  KEYPAD_5                (  93  | 0xF000 )
//  KEYPAD_6                (  94  | 0xF000 )
//  KEYPAD_7                (  95  | 0xF000 )
//  KEYPAD_8                (  96  | 0xF000 )
//  KEYPAD_9                (  97  | 0xF000 )
//  KEYPAD_0                (  98  | 0xF000 )
//  KEYPAD_PERIOD           (  99  | 0xF000 )
//  KEY_NON_US_BS           ( 100  | 0xF000 )
//  KEY_MENU                ( 101  | 0xF000 )
//  KEY_F13                 ( 104  | 0xF000 )
//  KEY_F14                 ( 105  | 0xF000 )
//  KEY_F15                 ( 106  | 0xF000 )
//  KEY_F16                 ( 107  | 0xF000 )
//  KEY_F17                 ( 108  | 0xF000 )
//  KEY_F18                 ( 109  | 0xF000 )
//  KEY_F19                 ( 110  | 0xF000 )
//  KEY_F20                 ( 111  | 0xF000 )
//  KEY_F21                 ( 112  | 0xF000 )
//  KEY_F22                 ( 113  | 0xF000 )
//  KEY_F23                 ( 114  | 0xF000 )
//  KEY_F24                 ( 115  | 0xF000 )
//  
//  for compatibility with Leonardo's slightly different names
//  KEY_UP_ARROW    KEY_UP
//  KEY_DOWN_ARROW  KEY_DOWN
//  KEY_LEFT_ARROW  KEY_LEFT
//  KEY_RIGHT_ARROW KEY_RIGHT
//  KEY_RETURN      KEY_ENTER
//  KEY_LEFT_CTRL   MODIFIERKEY_LEFT_CTRL
//  KEY_LEFT_SHIFT  MODIFIERKEY_LEFT_SHIFT
//  KEY_LEFT_ALT    MODIFIERKEY_LEFT_ALT
//  KEY_LEFT_GUI    MODIFIERKEY_LEFT_GUI
//  KEY_RIGHT_CTRL  MODIFIERKEY_RIGHT_CTRL
//  KEY_RIGHT_SHIFT MODIFIERKEY_RIGHT_SHIFT
//  KEY_RIGHT_ALT   MODIFIERKEY_RIGHT_ALT
//  KEY_RIGHT_GUI   MODIFIERKEY_RIGHT_GUI

// Alias
#define KEY_CAPSLOCK         KEY_CAPS_LOCK
#define KEY_SCROLLLOCK       KEY_SCROLL_LOCK
#define KEY_NUMLOCK          KEY_NUM_LOCK
#define KEYPAD_DIVIDE        KEYPAD_SLASH
#define KEYPAD_MULTIPLY      KEYPAD_ASTERIX
#define KEYPAD_SUBTRACT      KEYPAD_MINUS
#define KEYPAD_ADD           KEYPAD_PLUS
#define KEYPAD_DOT           KEYPAD_PERIOD
#define KEY_APPLICATION      KEY_MENU
#define KEY_CONTEXT          KEY_MENU
#define KEY_KORENG           KEY_RIGHT_ALT
#define KEY_CTRL             MODIFIERKEY_LEFT_CTRL
#define KEY_SHIFT            MODIFIERKEY_LEFT_SHIFT
#define KEY_ALT              MODIFIERKEY_LEFT_ALT
#define KEY_GUI              MODIFIERKEY_LEFT_GUI
#define KEY_WINDOWS          MODIFIERKEY_LEFT_GUI
#define KEY_LEFT_WINDOWS     MODIFIERKEY_LEFT_GUI
#define KEY_RIGHT_WINDOWS    MODIFIERKEY_RIGHT_GUI
