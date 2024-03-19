#define KEYCODE_NONE    0
#define KEY_NONE        0

constexpr int32_t keycode_To_TeensyLayout(uint8_t keycode)
{
    return  (keycode <= 0x65)                       ?   ( keycode | 0xF000 ) :
            (0x67 <= keycode && keycode <= 0x6E)    ?   ( (1 << (keycode-0x67)) | 0xE000 ) :    // MODIFIERKEY_
            (0x81 <= keycode && keycode <= 0x83)    ?   ( keycode | 0xE200 ) :                  // KEY_SYSTEM_
            (0xB0 <= keycode && keycode <= 0xEA)    ?   ( keycode | 0xE400 ) :                  // KEY_MEDIA_
            (keycode == 0x90)                       ?   ( MODIFIERKEY_RIGHT_ALT ) :             // KEY_KORENG
                                                        ( KEY_NONE );
}
constexpr uint8_t TeensyLayout_To_keycode(int32_t key)
{
    return  (0xF000 <= key && key <= 0xF065)        ?   ( key & ~(0xF000) ) :
            (0xE001 <= key && key <= 0xE080)        ?   ( __builtin_ctz(key) + 0x67 ) :         // MODIFIERKEY_
            (0xE281 <= key && key <= 0xE283)        ?   ( key & ~(0xE200) ) :                   // KEY_SYSTEM_
            (0xE4B0 <= key && key <= 0xE4EA)        ?   ( key & ~(0xE400) ) :                   // KEY_MEDIA_
                                                        ( KEY_NONE );
}



/************************************************
 **                                            **
 ** OnRawPress & OnRawRelease's keycode layout **
 **                                            **
 ************************************************/

//      KEYCODE_KEY_RESERVED        0x00
#define KEYCODE_KEY_ERROR_ROLLOVER  0x01
#define KEYCODE_KEY_POST_FAIL       0x02
#define KEYCODE_KEY_ERROR_UNDEFINED 0x03
#define KEYCODE_KEY_A               0x04
#define KEYCODE_KEY_B               0x05
#define KEYCODE_KEY_C               0x06
#define KEYCODE_KEY_D               0x07
#define KEYCODE_KEY_E               0x08
#define KEYCODE_KEY_F               0x09
#define KEYCODE_KEY_G               0x0A
#define KEYCODE_KEY_H               0x0B
#define KEYCODE_KEY_I               0x0C
#define KEYCODE_KEY_J               0x0D
#define KEYCODE_KEY_K               0x0E
#define KEYCODE_KEY_L               0x0F
#define KEYCODE_KEY_M               0x10
#define KEYCODE_KEY_N               0x11
#define KEYCODE_KEY_O               0x12
#define KEYCODE_KEY_P               0x13
#define KEYCODE_KEY_Q               0x14
#define KEYCODE_KEY_R               0x15
#define KEYCODE_KEY_S               0x16
#define KEYCODE_KEY_T               0x17
#define KEYCODE_KEY_U               0x18
#define KEYCODE_KEY_V               0x19
#define KEYCODE_KEY_W               0x1A
#define KEYCODE_KEY_X               0x1B
#define KEYCODE_KEY_Y               0x1C
#define KEYCODE_KEY_Z               0x1D
#define KEYCODE_KEY_1               0x1E
#define KEYCODE_KEY_2               0x1F
#define KEYCODE_KEY_3               0x20
#define KEYCODE_KEY_4               0x21
#define KEYCODE_KEY_5               0x22
#define KEYCODE_KEY_6               0x23
#define KEYCODE_KEY_7               0x24
#define KEYCODE_KEY_8               0x25
#define KEYCODE_KEY_9               0x26
#define KEYCODE_KEY_0               0x27
#define KEYCODE_KEY_ENTER           0x28
#define KEYCODE_KEY_ESC             0x29
#define KEYCODE_KEY_BACKSPACE       0x2A
#define KEYCODE_KEY_TAB             0x2B
#define KEYCODE_KEY_SPACE           0x2C
#define KEYCODE_KEY_MINUS           0x2D // '-'
#define KEYCODE_KEY_EQUAL           0x2E // '='
#define KEYCODE_KEY_LEFT_BRACE      0x2F // '['
#define KEYCODE_KEY_RIGHT_BRACE     0x30 // ']'
#define KEYCODE_KEY_BACKSLASH       0x31 // '\'
#define KEYCODE_KEY_NON_US_NUM      0x32 // Don't Know 1
#define KEYCODE_KEY_SEMICOLON       0x33 // ';'
#define KEYCODE_KEY_QUOTE           0x34 // '''
#define KEYCODE_KEY_TILDE           0x35 // '`'
#define KEYCODE_KEY_COMMA           0x36 // ','
#define KEYCODE_KEY_PERIOD          0x37 // '.'
#define KEYCODE_KEY_SLASH           0x38 // '/'
#define KEYCODE_KEY_CAPS_LOCK       0x39
#define KEYCODE_KEY_CAPSLOCK        0x39 // Alias
#define KEYCODE_KEY_F1              0x3A
#define KEYCODE_KEY_F2              0x3B
#define KEYCODE_KEY_F3              0x3C
#define KEYCODE_KEY_F4              0x3D
#define KEYCODE_KEY_F5              0x3E
#define KEYCODE_KEY_F6              0x3F
#define KEYCODE_KEY_F7              0x40
#define KEYCODE_KEY_F8              0x41
#define KEYCODE_KEY_F9              0x42
#define KEYCODE_KEY_F10             0x43
#define KEYCODE_KEY_F11             0x44
#define KEYCODE_KEY_F12             0x45
#define KEYCODE_KEY_PRINTSCREEN     0x46
#define KEYCODE_KEY_SCROLL_LOCK     0x47
#define KEYCODE_KEY_SCROLLLOCK      0x47 // Alias
#define KEYCODE_KEY_PAUSE           0x48
#define KEYCODE_KEY_INSERT          0x49
#define KEYCODE_KEY_HOME            0x4A
#define KEYCODE_KEY_PAGE_UP         0x4B
#define KEYCODE_KEY_DELETE          0x4C
#define KEYCODE_KEY_END             0x4D
#define KEYCODE_KEY_PAGE_DOWN       0x4E
#define KEYCODE_KEY_RIGHT_ARROW     0x4F
#define KEYCODE_KEY_RIGHT           0x4F // Alias
#define KEYCODE_KEY_LEFT_ARROW      0x50
#define KEYCODE_KEY_LEFT            0x50 // Alias
#define KEYCODE_KEY_DOWN_ARROW      0x51
#define KEYCODE_KEY_DOWN            0x51 // Alias
#define KEYCODE_KEY_UP_ARROW        0x52
#define KEYCODE_KEY_UP              0x52 // Alias
#define KEYCODE_KEY_NUM_LOCK        0x53
#define KEYCODE_KEY_NUMLOCK         0x53 // Alias
#define KEYCODE_KEYPAD_DIVIDE       0x54 // '/'
#define KEYCODE_KEYPAD_SLASH        0x54 // Alias
#define KEYCODE_KEYPAD_MULTIPLY     0x55 // '*'
#define KEYCODE_KEYPAD_ASTERIX      0x55 // Alias
#define KEYCODE_KEYPAD_SUBTRACT     0x56 // '-'
#define KEYCODE_KEYPAD_MINUS        0x56 // Alias
#define KEYCODE_KEYPAD_ADD          0x57 // '+'
#define KEYCODE_KEYPAD_PLUS         0x57 // Alias
#define KEYCODE_KEYPAD_ENTER        0x58
#define KEYCODE_KEYPAD_1            0x59
#define KEYCODE_KEYPAD_2            0x5A
#define KEYCODE_KEYPAD_3            0x5B
#define KEYCODE_KEYPAD_4            0x5C
#define KEYCODE_KEYPAD_5            0x5D
#define KEYCODE_KEYPAD_6            0x5E
#define KEYCODE_KEYPAD_7            0x5F
#define KEYCODE_KEYPAD_8            0x60
#define KEYCODE_KEYPAD_9            0x61
#define KEYCODE_KEYPAD_0            0x62
#define KEYCODE_KEYPAD_DOT          0x63 // '.'
#define KEYCODE_KEYPAD_PERIOD       0x63 // Alias
#define KEYCODE_KEY_NON_US          0x64 // Don't Know 2
#define KEYCODE_KEY_APPLICATION     0x65
#define KEYCODE_KEY_CONTEXT         0x65 // Alias
#define KEYCODE_KEY_MENU            0x65 // Alias

#define KEYCODE_KEY_CTRL            0x67
#define KEYCODE_KEY_SHIFT           0x68
#define KEYCODE_KEY_ALT             0x69
#define KEYCODE_KEY_GUI             0x6A
#define KEYCODE_KEY_WINDOWS         0x6A // Alias
#define KEYCODE_KEY_LEFT_CTRL       0x67 // Alias
#define KEYCODE_KEY_LEFT_SHIFT      0x68 // Alias
#define KEYCODE_KEY_LEFT_ALT        0x69 // Alias
#define KEYCODE_KEY_LEFT_GUI        0x6A // Alias
#define KEYCODE_KEY_LEFT_WINDOWS    0x6A // Alias
#define KEYCODE_KEY_RIGHT_CTRL      0x6B
#define KEYCODE_KEY_RIGHT_SHIFT     0x6C
#define KEYCODE_KEY_RIGHT_ALT       0x6D
#define KEYCODE_KEY_RIGHT_GUI       0x6E
#define KEYCODE_KEY_RIGHT_WINDOWS   0x6E // Alias

#define KEYCODE_KEY_KORENG          0x90 // KOR-ENG Swap



/******************************************************************
 **                                                              **
 **                    Teensy® 4.1's key layout                    **
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