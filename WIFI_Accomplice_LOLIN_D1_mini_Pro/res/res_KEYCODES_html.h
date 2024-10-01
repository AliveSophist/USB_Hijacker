const char raw_html_KEYCODES[] PROGMEM =
/* before head compressed

<head>
<style>
@import url('https://fonts.cdnfonts.com/css/lcd');
@font-face {
    font-family: 'D2Coding';
    src: url('https://cdn.jsdelivr.net/gh/projectnoonnu/noonfonts_three@1.0/D2Coding.woff') format('woff');
    font-weight: normal;
    font-style: normal;
}
* { scrollbar-color: #2f2f2f transparent; }
*::-webkit-scrollbar-thumb { background-color: #2f2f2f; }
*::-webkit-scrollbar { background-color: transparent; }

body, html {
    margin: 0;
    padding: 0;
    height: 100%;
}
</style>
</head>
*/
R"=====(
<head><style>@import url('https://fonts.cdnfonts.com/css/lcd'); @font-face { font-family: 'D2Coding'; src: url('https://cdn.jsdelivr.net/gh/projectnoonnu/noonfonts_three@1.0/D2Coding.woff') format('woff'); font-weight: normal; font-style: normal;}* { scrollbar-color: #2f2f2f transparent; }*::-webkit-scrollbar-thumb { background-color: #2f2f2f; } *::-webkit-scrollbar { background-color: transparent; } body, html { margin: 0; padding: 0; height: 100%; }</style></head>
<body>
<textarea style="width: 100%; height: 100%; padding: 20px; background-color: #212121; color: #ECECEC; font-family: D2Coding; font-size: 16px;" readonly>
KEY_RESERVED		0x00
KEY_ERROR_ROLLOVER0	0x01
KEY_POST_FAIL		0x02
KEY_ERROR_UNDEFINED	0x03
KEY_A			0x04
KEY_B			0x05
KEY_C			0x06
KEY_D			0x07
KEY_E			0x08
KEY_F			0x09
KEY_G			0x0A
KEY_H			0x0B
KEY_I			0x0C
KEY_J			0x0D
KEY_K			0x0E
KEY_L			0x0F
KEY_M			0x10
KEY_N			0x11
KEY_O			0x12
KEY_P			0x13
KEY_Q			0x14
KEY_R			0x15
KEY_S			0x16
KEY_T			0x17
KEY_U			0x18
KEY_V			0x19
KEY_W			0x1A
KEY_X			0x1B
KEY_Y			0x1C
KEY_Z			0x1D
KEY_1			0x1E
KEY_2			0x1F
KEY_3			0x20
KEY_4			0x21
KEY_5			0x22
KEY_6			0x23
KEY_7			0x24
KEY_8			0x25
KEY_9			0x26
KEY_0			0x27
KEY_ENTER		0x28
KEY_ESC			0x29
KEY_BACKSPACE		0x2A
KEY_TAB			0x2B
KEY_SPACE		0x2C
KEY_MINUS		0x2D //   '-'
KEY_EQUAL		0x2E //   '='
KEY_LEFT_BRACE		0x2F //   '['
KEY_RIGHT_BRACE		0x30 //   ']'
KEY_BACKSLASH		0x31 //   '\'
KEY_NON_US_NUM		0x32 //   Don't Know 1
KEY_SEMICOLON		0x33 //   ';'
KEY_QUOTE		0x34 //   '''
KEY_TILDE		0x35 //   '`'
KEY_COMMA		0x36 //   ','
KEY_PERIOD		0x37 //   '.'
KEY_SLASH		0x38 //   '/'
KEY_CAPS_LOCK		0x39
KEY_CAPSLOCK		0x39 //   Alias
KEY_F1			0x3A
KEY_F2			0x3B
KEY_F3			0x3C
KEY_F4			0x3D
KEY_F5			0x3E
KEY_F6			0x3F
KEY_F7			0x40
KEY_F8			0x41
KEY_F9			0x42
KEY_F10			0x43
KEY_F11			0x44
KEY_F12			0x45
KEY_PRINTSCREEN		0x46
KEY_SCROLL_LOCK		0x47
KEY_SCROLLLOCK		0x47 // Alias
KEY_PAUSE		0x48
KEY_INSERT		0x49
KEY_HOME		0x4A
KEY_PAGE_UP		0x4B
KEY_DELETE		0x4C
KEY_END			0x4D
KEY_PAGE_DOWN		0x4E
KEY_RIGHT_ARROW		0x4F
KEY_RIGHT		0x4F // Alias
KEY_LEFT_ARROW		0x50
KEY_LEFT		0x50 // Alias
KEY_DOWN_ARROW		0x51
KEY_DOWN		0x51 // Alias
KEY_UP_ARROW		0x52
KEY_NUM_LOCK		0x53
KEY_NUMLOCK		0x53 //   Alias
KEYPAD_DIVIDE		0x54 //   '/'
KEYPAD_MULTIPLY		0x55 //   '*'
KEYPAD_SUBTRACT		0x56 //   '-'
KEYPAD_ADD		0x57 //   '+'
KEYPAD_ENTER		0x58
KEYPAD_1		0x59
KEYPAD_2		0x5A
KEYPAD_3		0x5B
KEYPAD_4		0x5C
KEYPAD_5		0x5D
KEYPAD_6		0x5E
KEYPAD_7		0x5F
KEYPAD_8		0x60
KEYPAD_9		0x61
KEYPAD_0		0x62
KEYPAD_DOT		0x63 //   '.'
KEY_NON_US		0x64 //   Don't Know 2
KEY_APPLICATION		0x65
KEY_CONTEXT		0x65 //   Alias
KEY_MENU		0x65 //   Alias

KEY_KORENG		0x90 //   KOR-ENG Swap

KEY_CTRL		0x67
KEY_SHIFT		0x68
KEY_ALT			0x69
KEY_GUI			0x6A
KEY_WINDOWS		0x6A //   Alias
KEY_LEFT_CTRL		0x67 //   Alias
KEY_LEFT_SHIFT		0x68 //   Alias
KEY_LEFT_ALT		0x69 //   Alias
KEY_LEFT_GUI		0x6A //   Alias
KEY_LEFT_WINDOWS	0x6A //   Alias
KEY_RIGHT_CTRL		0x6B
KEY_RIGHT_SHIFT		0x6C
KEY_RIGHT_ALT		0x6D
KEY_RIGHT_GUI		0x6E
KEY_RIGHT_WINDOWS	0x6E //   Alias

KEY_SYSTEM_POWER_DOWN	0x81
KEY_SYSTEM_SLEEP	0x82
KEY_SYSTEM_WAKE_UP	0x83
        
KEY_MEDIA_PLAY		0xB0
KEY_MEDIA_PAUSE		0xB1
KEY_MEDIA_RECORD	0xB2
KEY_MEDIA_FAST_FORWARD	0xB3
KEY_MEDIA_REWIND	0xB4
KEY_MEDIA_NEXT_TRACK	0xB5
KEY_MEDIA_PREV_TRACK	0xB6
KEY_MEDIA_STOP		0xB7
KEY_MEDIA_EJECT		0xB8
KEY_MEDIA_RANDOM_PLAY	0xB9
KEY_MEDIA_PLAY_PAUSE	0xCD
KEY_MEDIA_PLAY_SKIP	0xCE
KEY_MEDIA_MUTE		0xE2
KEY_MEDIA_VOLUME_INC	0xE9
KEY_MEDIA_VOLUME_DEC	0xEA</textarea>
</body>
)=====";
