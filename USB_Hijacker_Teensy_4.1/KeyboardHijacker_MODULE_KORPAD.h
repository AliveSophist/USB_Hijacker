
#include "KeyboardHijacker.h"

#define KEYPAD_KOREAN_LAYOUT    2      // 0:None   1:CheonJiIn   2:NaRatGeul
// CheonJiIn is... https://namu.wiki/w/%EC%B2%9C%EC%A7%80%EC%9D%B8%20%EC%9E%90%ED%8C%90
// NaRatGeul is... https://namu.wiki/w/KT%EB%82%98%EB%9E%8F%EA%B8%80%20%EC%9E%90%ED%8C%90

#define DO_BACKSPACE    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); delay(10);
#define DO_SPACE        KBD_HIJACKER.pressandreleaseKey(KEY_SPACE);     delay(10);
#define COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY        DO_SPACE; DO_BACKSPACE;



void KeyboardHijacker::MODULE_KOREAN_KEYPAD_EVOLUTION()
{
/* ------------------------------------------------------ KEYPAD_KOREAN_LAYOUT COMMON ------------------------------------------------------ */
#if (KEYPAD_KOREAN_LAYOUT > 0)

    static char* KorSlot0 = (char*) malloc( sizeof(char) * 3 );
    static char* KorSlot1 = (char*) malloc( sizeof(char) * 3 );
    static char* KorSlot2 = (char*) malloc( sizeof(char) * 3 );
    static char* KorSlot3 = (char*) malloc( sizeof(char) * 3 );
    static char* KorSlot4 = (char*) malloc( sizeof(char) * 3 );

    static bool isKorSlotUpdated, isKorSlotUnderflowed, isKorSlotReset;
    isKorSlotUpdated = isKorSlotUnderflowed = isKorSlotReset = false; //always reset false

    // Now!! Lambda no longer require double pointers!!  (↓ Old Code, using double pointers. fxxking dirty :p ↓)
    // https://github.com/AliveSophist/USB_Hijacker/blob/e83d5220efb0088f9565fd778ef1e0cd9ec290f2/USB_Hijacker_Teensy_4.1/function_MODULE_KORPAD.ino
    auto KORPAD_updateConfirmedInputs = [&]() -> void
    {
        fillWith(KorSlot4,"");

        char* temp = KorSlot4;
        KorSlot4 = KorSlot3;
        KorSlot3 = KorSlot2;
        KorSlot2 = KorSlot1;
        KorSlot1 = KorSlot0;
        KorSlot0 = temp;
        
        isKorSlotUpdated = true;
    };
    auto KORPAD_undoConfirmedInputs = [&]() -> void
    {
        if(isEquals(KorSlot0,"")){
            isKorSlotUnderflowed = true;
            return;
        }

        fillWith(KorSlot0,"");

        char* temp = KorSlot0;
        KorSlot0 = KorSlot1;
        KorSlot1 = KorSlot2;
        KorSlot2 = KorSlot3;
        KorSlot3 = KorSlot4;
        KorSlot4 = temp;
    };
    auto KORPAD_resetConfirmedInputs = [&]() -> void
    {
        fillWith(KorSlot4,"");
        fillWith(KorSlot3,"");
        fillWith(KorSlot2,"");
        fillWith(KorSlot1,"");
        fillWith(KorSlot0,"");

        isKorSlotReset = true;
    };



    /**⦓   KORPAD_MODE   ⦔**/

    #define KORPAD_MODE_NONE       -1
    #define KORPAD_MODE_HANGEUL    0
    #define KORPAD_MODE_ALPHABET   1
    #define KORPAD_MODE_KANA       2
    static int8_t KORPAD_MODE = KORPAD_MODE_NONE;

    auto KORPAD_switchMODE = [&](uint8_t UPDATE_MODE) -> void
    {
        KORPAD_resetConfirmedInputs();


        // Tune Lock States & KorEng state
        {
            // Tune KorEng state, When entering the KORPAD MODE
            if( KORPAD_MODE == KORPAD_MODE_NONE )
            {
                // While KORPAD_MODE, Numlock is always on
                if(!KBD_HIJACKER.getStateNumLockToggle()) KBD_HIJACKER.pressandreleaseKey(KEY_NUM_LOCK);

                // Reverse KorEng state IMMEDIATELY, if NumLock pressed more than 1000 millis
                if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 1000) KBD_HIJACKER.pressandreleaseKey(KEY_KORENG);
                // Otherwise,
                // If your system supports KorEngStatus.exe (Maker's Blog:https://blog.naver.com/breeze4me/140056743544)
                // Reverse the KorEng state, Dependent on ScrollLock state
                else
                {
                    KBD_HIJACKER.pressandreleaseKey(KEY_SCROLL_LOCK); // For, Update ScrollLock state with THE LATEST
                    delay(69); KBD_HIJACKER.syncToggleKeyStates();

                    if(!KBD_HIJACKER.getStateScrollLockToggle())
                    {
                        bool stateBeforeReverse = KBD_HIJACKER.getStateScrollLockToggle();

                        KBD_HIJACKER.pressandreleaseKey(KEY_SCROLL_LOCK); // For, Monitor the changes in the ScrollLock state
                        delay(69); KBD_HIJACKER.syncToggleKeyStates();

                        bool stateAfterReverse  = KBD_HIJACKER.getStateScrollLockToggle();

                        // Switch KorEng state, If the KorEngStatus.exe's feature that reflects the KORENG state on the ScrollLock state is running
                        if(stateBeforeReverse == stateAfterReverse)
                            KBD_HIJACKER.pressandreleaseKey(KEY_KORENG);
                    }
                    // rollback ScrollLock state
                    else
                        KBD_HIJACKER.pressandreleaseKey(KEY_SCROLL_LOCK); // For, Rollback ScrollLock's state
                }
            }

            //If CAPSLOCK was On, Turn Off BY FORCE
            if(KBD_HIJACKER.getStateCapsLockToggle()) KBD_HIJACKER.pressandreleaseKey(KEY_CAPS_LOCK);

            // Sync Lock states
            KBD_HIJACKER.reserveSyncToggleKeyStates();
        }


        // Enter & Exit Sounds
        {
            if
            ( KORPAD_MODE == KORPAD_MODE_NONE )        // Enter the KORPAD MODE
            {
                Buzzzzer::reserveBuzz   ( { NOTE_G5 }
                                        , { 333     } );
            }
            else if
            ( UPDATE_MODE == KORPAD_MODE_ALPHABET )    // Enter KORPAD_MODE_ALPHABET
            {
                Buzzzzer::reserveBuzz   ( { NOTE_FS5,0, NOTE_GS5,0, NOTE_AS5,0, NOTE_DS6,0, NOTE_CS6    }
                                        , { 200,30,     100,150,    120,200,    150,150,    450         } );
            }
            else if
            ( UPDATE_MODE == KORPAD_MODE_KANA )        // Enter KORPAD_MODE_KANA
            {
                Buzzzzer::reserveBuzz   ( { NOTE_E7,    NOTE_E7,0,  NOTE_E7,0,  NOTE_C7,    NOTE_E7,0,  NOTE_G7,0   }
                                        , { 200,        130,50,     100,50,     100,        130,50,     300,444,    } );
            }

            else if
            ( UPDATE_MODE == KORPAD_MODE_NONE )        // Exit the KORPAD MODE
            {
                Buzzzzer::reserveBuzz   ( { NOTE_D3 }
                                        , { 181     } );
            }
            else if
            ( KORPAD_MODE == KORPAD_MODE_ALPHABET )    // Exit KORPAD_MODE_ALPHABET
            {
                Buzzzzer::reserveBuzz   ( { NOTE_AS5,0, NOTE_GS5,0, NOTE_FS5,0, NOTE_FS5    }
                                        , { 300,50,     230,50,     170,100,    350         } );
            }
            else if
            ( KORPAD_MODE == KORPAD_MODE_KANA )        // Exit KORPAD_MODE_KANA
            {
                Buzzzzer::reserveBuzz   ( { NOTE_G6 }
                                        , { 222     } );
            }
        }


        KORPAD_MODE = UPDATE_MODE;
    };



    /**⦓   About, KORPAD_MODE_HANGEUL   ⦔**/

    auto isExistConsonant = [](char* KorSlotX) -> bool
    {
        return (strcmp(KorSlotX,"r")==0||strcmp(KorSlotX,"z")==0||strcmp(KorSlotX,"R")==0||strcmp(KorSlotX,"s")==0||strcmp(KorSlotX,"f")==0||strcmp(KorSlotX,"e")==0||strcmp(KorSlotX,"x")==0||strcmp(KorSlotX,"E")==0||strcmp(KorSlotX,"q")==0||strcmp(KorSlotX,"v")==0||strcmp(KorSlotX,"Q")==0||strcmp(KorSlotX,"t")==0||strcmp(KorSlotX,"g")==0||strcmp(KorSlotX,"T")==0||strcmp(KorSlotX,"w")==0||strcmp(KorSlotX,"c")==0||strcmp(KorSlotX,"W")==0||strcmp(KorSlotX,"d")==0||strcmp(KorSlotX,"a")==0);
    };
    auto isExistVowel = [](char* KorSlotX) -> bool
    {
        return (strcmp(KorSlotX,"k")==0||strcmp(KorSlotX,"o")==0||strcmp(KorSlotX,"i")==0||strcmp(KorSlotX,"O")==0||strcmp(KorSlotX,"j")==0||strcmp(KorSlotX,"p")==0||strcmp(KorSlotX,"u")==0||strcmp(KorSlotX,"P")==0||strcmp(KorSlotX,"h")==0||strcmp(KorSlotX,"y")==0||strcmp(KorSlotX,"n")==0||strcmp(KorSlotX,"b")==0||strcmp(KorSlotX,"m")==0||strcmp(KorSlotX,"l")==0||strcmp(KorSlotX,"hk")==0||strcmp(KorSlotX,"ho")==0||strcmp(KorSlotX,"hl")==0||strcmp(KorSlotX,"nj")==0||strcmp(KorSlotX,"np")==0||strcmp(KorSlotX,"nl")==0||strcmp(KorSlotX,"ml")==0);
    };
    auto KORPAD_isHeldDualConsonant = [](char* KorSlotX, char* KorSlotY) -> bool
    {
        if(isEquals(KorSlotX,"r") || isEquals(KorSlotX,"q"))
        {
            if(isEquals(KorSlotY,"t"))
                return true;
        }
        else if(isEquals(KorSlotX,"s"))
        {
            if(isEquals(KorSlotY,"w") || isEquals(KorSlotY,"g"))
                return true;
        }
        else if(isEquals(KorSlotX,"f"))
        {
            if(isEquals(KorSlotY,"r") || isEquals(KorSlotY,"a") || isEquals(KorSlotY,"q") || isEquals(KorSlotY,"t") || isEquals(KorSlotY,"x") || isEquals(KorSlotY,"v") || isEquals(KorSlotY,"g"))
                return true;
        }

        return false;
    };

    auto KORPAD_correctBottomDualConsonantError = [&]() -> void
    {
        if(!KORPAD_isHeldDualConsonant(KorSlot1,KorSlot0))
            return;

        if(isExistVowel(KorSlot2))
        {
            if(isExistConsonant(KorSlot3))
            {
                DO_BACKSPACE;
                KBD_HIJACKER.pressandreleaseKeys(KorSlot3); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot2); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot1); delay(5);
            }
        }
    };
    auto KORPAD_correctBottomSingleConsonantError = [&]() -> void
    {
        if(isExistVowel(KorSlot1))
        {
            if(isExistConsonant(KorSlot2))
            {
                DO_BACKSPACE;
                KBD_HIJACKER.pressandreleaseKeys(KorSlot2); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot1); delay(5);
            }
        }
    };

    #if (KEYPAD_KOREAN_LAYOUT == 1) // CheonJiIn Only

    auto KORPAD_correctAraeAVowelError = [&]() -> void
    {
        if(isExistConsonant(KorSlot1))
        {
            if
            (   isExistVowel(KorSlot2) &&
                isExistConsonant(KorSlot3)
            )
            {
                DO_BACKSPACE;
                KBD_HIJACKER.pressandreleaseKeys(KorSlot3); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot2); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot1); delay(5);
            }
            else if
            (   /*isExistConsonant(KorSlot2) &&*/
                KORPAD_isHeldDualConsonant(KorSlot2,KorSlot1) &&
                isExistVowel(KorSlot3) &&
                isExistConsonant(KorSlot4)
            )
            {
                DO_BACKSPACE;
                KBD_HIJACKER.pressandreleaseKeys(KorSlot4); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot3); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot2); delay(5);
                KBD_HIJACKER.pressandreleaseKeys(KorSlot1); delay(5);
            }
            else
            {
                DO_BACKSPACE;
                KBD_HIJACKER.pressandreleaseKeys(KorSlot1); delay(5);
            }
            
            isKorSlotUpdated = true;
        }
    };

    #endif



    /**⦓   About, KORPAD_MODE_ALPHABET   ⦔**/

    #define KORPAD_writeAlphabet(CHSTR) KBD_HIJACKER.pressandreleaseKey(KEY_KORENG); delay(10); Keyboard.write(CHSTR); delay(10); KBD_HIJACKER.pressandreleaseKey(KEY_KORENG);

    auto KORPAD_tryToTypeAlphabetByHangeulPronounce = [&]() -> bool
    {
        if(KORPAD_MODE != KORPAD_MODE_ALPHABET)
            return false;
        if(isEquals(KorSlot0, ""))
            return false;


        COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY;


        #define KORPAD_isAlphabetPronounce(STR3,STR2,STR1,STR0)     ( (strcmp("",STR0)==0||strcmp(KorSlot0,STR0)==0) && (strcmp("",STR1)==0||strcmp(KorSlot1,STR1)==0) && (strcmp(KorSlot2,STR2)==0||strcmp("",STR2)==0) && (strcmp(KorSlot3,STR3)==0||strcmp("",STR3)==0) )

        char alphabet = 0;
        uint8_t numNeededBackspace = 1;

        alphabet =
        KORPAD_isAlphabetPronounce("d","l","c","l") ? 'h' :
        KORPAD_isAlphabetPronounce("m","f","d","b") ? 'w' :
        KORPAD_isAlphabetPronounce("m","f","f","b") ? 'w' : 0;

        if(alphabet!=0) numNeededBackspace++;

        alphabet = alphabet!=0 ? alphabet :
        KORPAD_isAlphabetPronounce("d","o","d","l") ? 'a' :
        KORPAD_isAlphabetPronounce("d","p","d","l") ? 'a' :
        KORPAD_isAlphabetPronounce("d","o","v","m") ? 'f' :
        KORPAD_isAlphabetPronounce("d","p","v","m") ? 'f' :
        KORPAD_isAlphabetPronounce("d","p","c","l") ? 'h' :
        KORPAD_isAlphabetPronounce("d","k","d","l") ? 'i' :
        KORPAD_isAlphabetPronounce("w","p","d","l") ? 'j' :
        KORPAD_isAlphabetPronounce("z","p","d","l") ? 'k' :
        KORPAD_isAlphabetPronounce("d","p","t","m") ? 's' :
        KORPAD_isAlphabetPronounce("q","m","d","l") ? 'v' :
        KORPAD_isAlphabetPronounce("e","j","q","b") ? 'w' :
        KORPAD_isAlphabetPronounce("E","j","q","b") ? 'w' :
        KORPAD_isAlphabetPronounce("o","r","t","m") ? 'x' :
        KORPAD_isAlphabetPronounce("p","r","t","m") ? 'x' :
        KORPAD_isAlphabetPronounce("d","hk","d","l")? 'y' :
        KORPAD_isAlphabetPronounce("w","","x","m")  ? 'z' :
        KORPAD_isAlphabetPronounce("W","","x","m")  ? 'z' : 0;

        if(alphabet!=0) numNeededBackspace++;

        alphabet = alphabet!=0 ? alphabet :
        KORPAD_isAlphabetPronounce("","","d","k")   ? 'a' :
        KORPAD_isAlphabetPronounce("","","d","o")   ? 'a' :
        KORPAD_isAlphabetPronounce("","","q","l")   ? 'b' :
        KORPAD_isAlphabetPronounce("","","Q","l")   ? 'b' :
        KORPAD_isAlphabetPronounce("","","t","l")   ? 'c' :
        KORPAD_isAlphabetPronounce("","","T","l")   ? 'c' :
        KORPAD_isAlphabetPronounce("","","z","m")   ? 'c' :
        KORPAD_isAlphabetPronounce("","","e","l")   ? 'd' :
        KORPAD_isAlphabetPronounce("","","e","m")   ? 'd' :
        KORPAD_isAlphabetPronounce("","","d","l")   ? 'e' :
        KORPAD_isAlphabetPronounce("","","d","j")   ? 'e' :
        KORPAD_isAlphabetPronounce("","","d","p")   ? 'e' :
        KORPAD_isAlphabetPronounce("","","d","m")   ? 'e' :
        KORPAD_isAlphabetPronounce("","d","p","v")  ? 'f' :
        KORPAD_isAlphabetPronounce("","","v","m")   ? 'f' :
        KORPAD_isAlphabetPronounce("","","Q","m")   ? 'f' :
        KORPAD_isAlphabetPronounce("","","w","nl")  ? 'g' :
        KORPAD_isAlphabetPronounce("","","w","l")   ? 'g' :
        KORPAD_isAlphabetPronounce("","","r","m")   ? 'g' :
        KORPAD_isAlphabetPronounce("","","g","l")   ? 'h' :
        KORPAD_isAlphabetPronounce("","","g","m")   ? 'h' :
        KORPAD_isAlphabetPronounce("","","w","p")   ? 'j' :
        KORPAD_isAlphabetPronounce("","","z","p")   ? 'k' :
        KORPAD_isAlphabetPronounce("","","z","l")   ? 'k' :
        KORPAD_isAlphabetPronounce("","","r","l")   ? 'k' :
        KORPAD_isAlphabetPronounce("","d","p","f")  ? 'l' :
        KORPAD_isAlphabetPronounce("","","f","m")   ? 'l' :
        KORPAD_isAlphabetPronounce("","d","o","a")  ? 'm' :
        KORPAD_isAlphabetPronounce("","d","p","a")  ? 'm' :
        KORPAD_isAlphabetPronounce("","","a","m")   ? 'm' :
        KORPAD_isAlphabetPronounce("","","a","l")   ? 'm' :
        KORPAD_isAlphabetPronounce("","d","o","s")  ? 'n' :
        KORPAD_isAlphabetPronounce("","d","p","s")  ? 'n' :
        KORPAD_isAlphabetPronounce("","","s","m")   ? 'n' :
        KORPAD_isAlphabetPronounce("","","s","l")   ? 'n' :
        KORPAD_isAlphabetPronounce("","","d","h")   ? 'o' :
        KORPAD_isAlphabetPronounce("","","v","l")   ? 'p' :
        KORPAD_isAlphabetPronounce("","","z","b")   ? 'q' :
        KORPAD_isAlphabetPronounce("","","z","n")   ? 'q' :
        KORPAD_isAlphabetPronounce("","d","k","f")  ? 'r' :
        KORPAD_isAlphabetPronounce("","d","j","f")  ? 'r' :
        KORPAD_isAlphabetPronounce("","","f","l")   ? 'r' :
        KORPAD_isAlphabetPronounce("","","t","m")   ? 's' :
        KORPAD_isAlphabetPronounce("","","x","l")   ? 't' :
        KORPAD_isAlphabetPronounce("","","x","m")   ? 't' :
        KORPAD_isAlphabetPronounce("","","d","b")   ? 'u' :
        KORPAD_isAlphabetPronounce("","","d","n")   ? 'u' :
        KORPAD_isAlphabetPronounce("","","q","ml")  ? 'v' :
        KORPAD_isAlphabetPronounce("","","q","m")   ? 'v' :
        KORPAD_isAlphabetPronounce("","E","j","q")  ? 'w' :
        KORPAD_isAlphabetPronounce("","d","o","r")  ? 'x' :
        KORPAD_isAlphabetPronounce("","d","p","r")  ? 'x' :
        KORPAD_isAlphabetPronounce("","","T","m")   ? 'x' :
        KORPAD_isAlphabetPronounce("","","d","i")   ? 'y' :
        KORPAD_isAlphabetPronounce("","","d","u")   ? 'y' :
        KORPAD_isAlphabetPronounce("","","d","y")   ? 'y' :
        KORPAD_isAlphabetPronounce("","","d","hk")  ? 'y' :
        KORPAD_isAlphabetPronounce("","w","","t")   ? 'z' :
        KORPAD_isAlphabetPronounce("","","w","ml")  ? 'z' :
        KORPAD_isAlphabetPronounce("","","w","m")   ? 'z' :
        KORPAD_isAlphabetPronounce("","","W","m")   ? 'z' : 0;

        if(alphabet!=0)
        {
            // Tune KorSlots
            KORPAD_resetConfirmedInputs();
            KorSlot0[0]='@'; KorSlot0[1]=alphabet;

            // Type!
            for(uint8_t i=0; i<numNeededBackspace; i++){ DO_BACKSPACE; }
            KORPAD_writeAlphabet( alphabet );

            return true;
        }

        alphabet =
        isEquals(KorSlot0, "r") ? 'g' :
        isEquals(KorSlot0, "R") ? 'q' :
        isEquals(KorSlot0, "s") ? 'n' :
        isEquals(KorSlot0, "e") ? 'd' :
        isEquals(KorSlot0, "E") ? '%' :
        isEquals(KorSlot0, "f") ? 'l' :
        isEquals(KorSlot0, "a") ? 'm' :
        isEquals(KorSlot0, "q") ? 'b' :
        isEquals(KorSlot0, "Q") ? 'f' :
        isEquals(KorSlot0, "t") ? 's' :
        isEquals(KorSlot0, "T") ? 'x' :
        isEquals(KorSlot0, "d") ? '~' :
        isEquals(KorSlot0, "w") ? 'j' :
        isEquals(KorSlot0, "W") ? 'z' :
        isEquals(KorSlot0, "c") ? '*' :
        isEquals(KorSlot0, "z") ? 'c' :
        isEquals(KorSlot0, "x") ? 't' :
        isEquals(KorSlot0, "v") ? 'f' :
        isEquals(KorSlot0, "g") ? 'h' :
        isEquals(KorSlot0, "k") ? 'a' :
        isEquals(KorSlot0, "o") ? 'a' :
        isEquals(KorSlot0, "j") ? 'e' :
        isEquals(KorSlot0, "p") ? 'e' :
        isEquals(KorSlot0, "h") ? 'o' :
        isEquals(KorSlot0, "hk")? 'y' :
        isEquals(KorSlot0, "n") ? 'u' :
        isEquals(KorSlot0, "i") ? 'y' :
        isEquals(KorSlot0, "u") ? 'y' :
        isEquals(KorSlot0, "y") ? 'y' :
        isEquals(KorSlot0, "b") ? 'w' :
        isEquals(KorSlot0, "m") ? 'e' :
        isEquals(KorSlot0, "l") ? 'i' :
        isEquals(KorSlot1, "f")
                                ? ( isEquals(KorSlot0,"@l") ? 'r' :
                                    isEquals(KorSlot0,"@r") ? 'l' : 0 ) :
        isEquals(KorSlot1, "q")
                                ? ( isEquals(KorSlot0,"@b") ? 'v' :
                                    isEquals(KorSlot0,"@v") ? 'b' : 0 ) :
        isEquals(KorSlot1, "t")
                                ? ( isEquals(KorSlot0,"@s") ? 'c' :
                                    isEquals(KorSlot0,"@c") ? 'x' :
                                    isEquals(KorSlot0,"@x") ? 's' : 0 ) :
        isEquals(KorSlot1, "T")
                                ? ( isEquals(KorSlot0,"@x") ? 'c' :
                                    isEquals(KorSlot0,"@c") ? 'x' : 0 ) :
        isEquals(KorSlot1, "w")
                                ? ( isEquals(KorSlot0,"@j") ? 'g' :
                                    isEquals(KorSlot0,"@g") ? 'z' :
                                    isEquals(KorSlot0,"@z") ? 'j' : 0 ) :
        isEquals(KorSlot1, "z")
                                ? ( isEquals(KorSlot0,"@c") ? 'k' :
                                    isEquals(KorSlot0,"@k") ? 'q' :
                                    isEquals(KorSlot0,"@q") ? 'x' :
                                    isEquals(KorSlot0,"@x") ? 'c' : 0 ) :
        isEquals(KorSlot1, "v")
                                ? ( isEquals(KorSlot0,"@f") ? 'p' :
                                    isEquals(KorSlot0,"@p") ? 'f' : 0 ) :
        isEquals(KorSlot1, "l")
                                ? ( isEquals(KorSlot0,"@i") ? 'e' :
                                    isEquals(KorSlot0,"@e") ? 'i' : 0 ) : 0;

        if(alphabet!=0)
        {
            // Tune KorSlots
            if(KorSlot0[0]!='@'){ String temp(KorSlot0); KORPAD_resetConfirmedInputs(); fillWith(KorSlot1,temp.c_str()); }
            KorSlot0[0]='@'; KorSlot0[1]=alphabet;

            // Type!
            DO_BACKSPACE;
            KORPAD_writeAlphabet(   alphabet=='%' ? "th" :
                                    alphabet=='~' ? "ng" :
                                    alphabet=='*' ? "ch" : String(alphabet).c_str()   );
        }

        return true;
    };

    auto KORPAD_tryToDoAlphabetCaseConversion = [&]() -> bool
    {
        if(KORPAD_MODE != KORPAD_MODE_ALPHABET)
            return false;

        if(KorSlot0[0] != '@')
            return false;

        if(!(('A'<=KorSlot0[1] && KorSlot0[1]<='Z') || ('a'<=KorSlot0[1] && KorSlot0[1]<='z')))
            return false;


        if('A'<=KorSlot0[1] && KorSlot0[1]<='Z')
        {
            KorSlot0[1] += 32;

            // switch CapsLockToggle, If Case Conversion Battered 4 times
            if(KBD_PARSER.KeyLogger.peek_keycode(1)==KEYCODE_KEYPAD_PERIOD && KBD_PARSER.KeyLogger.peek_keycode(2)==KEYCODE_KEYPAD_PERIOD && KBD_PARSER.KeyLogger.peek_keycode(3)==KEYCODE_KEYPAD_PERIOD) KBD_HIJACKER.pressandreleaseKey(KEY_CAPS_LOCK);

            DO_BACKSPACE;
            KORPAD_writeAlphabet( KorSlot0[1] );
        }
        else
        {
            KorSlot0[1] -= 32;

            DO_BACKSPACE;
            KORPAD_writeAlphabet( KorSlot0[1] );

            // switch CapsLockToggle, If Case Conversion Battered 3 times
            if(KBD_PARSER.KeyLogger.peek_keycode(1)==KEYCODE_KEYPAD_PERIOD && KBD_PARSER.KeyLogger.peek_keycode(2)==KEYCODE_KEYPAD_PERIOD) KBD_HIJACKER.pressandreleaseKey(KEY_CAPS_LOCK);
        }

        return true;
    };



    /**⦓   About, KORPAD_MODE_KANA   ⦔**/

    #define SET_IME_TO_KOR KBD_HIJACKER.pressandreleaseMultiKey( {KEY_ALT,KEY_SHIFT,KEY_3} );
    #define SET_IME_TO_GJI KBD_HIJACKER.pressandreleaseMultiKey( {KEY_ALT,KEY_SHIFT,KEY_4} );

    static std::list<String> queueKana;
    static std::list<uint8_t> logBeQueued;

    auto KORPAD_enqueueKana = [&](int8_t indexFrom) -> void
    {
        char* KorSlots[] = {KorSlot0, KorSlot1, KorSlot2, KorSlot3, KorSlot4};

        uint8_t lenBeQueued = 0;
        String queueThing;

        for(int8_t i=4; i>=indexFrom; i--)
        {
            queueThing = "";

            if(!isExistConsonant(KorSlots[i]))
            {
                // Hangeul to Kana(Chōonpu)
                if
                (isEquals(KorSlots[i],"m"))
                {
                    queueKana.push_back( "-" );
                    logBeQueued.push_back( 1 );
                }
                // Hangeul to Kana(ExtendedKatakana)
                // ExtendedKatakana is... https://en.wikipedia.org/wiki/Katakana#Extended_katakana
                else if
                (isEquals(KorSlots[i],"k") || isEquals(KorSlots[i],"l") || isEquals(KorSlots[i],"n") || isEquals(KorSlots[i],"p") || isEquals(KorSlots[i],"h"))
                {
                    String queuedLatest = (queueKana.empty() ? "" : queueKana.back());

                    if(queuedLatest[0]=='~' || queuedLatest[0]=='`' || queuedLatest[0]=='?')
                    {
                        queuedLatest =  isContains(queuedLatest,"~~")? (isEquals(KorSlots[i],"l") ? "wi" :
                                                                        isEquals(KorSlots[i],"p") ? "we" :
                                                                        isEquals(KorSlots[i],"h") ? "uxo": "") :
                                        isContains(queuedLatest,"ch")? (isEquals(KorSlots[i],"k") ? "tsa":
                                                                        isEquals(KorSlots[i],"l") ? "tsi":
                                                                        isEquals(KorSlots[i],"p") ? "tse":
                                                                        isEquals(KorSlots[i],"h") ? "tso": "") :
                                        isContains(queuedLatest,'t') ? (isEquals(KorSlots[i],"l") ? "thi":
                                                                        isEquals(KorSlots[i],"n") ? "twu": "") :
                                        isContains(queuedLatest,'d') ? (isEquals(KorSlots[i],"l") ? "dhi":
                                                                        isEquals(KorSlots[i],"n") ? "dwu": "") :
                                        isContains(queuedLatest,'s') ? (isEquals(KorSlots[i],"p") ? "she": "") :
                                        isContains(queuedLatest,'j') ? (isEquals(KorSlots[i],"p") ? "je" : "") :
                                        isContains(queuedLatest,'h') ? (isEquals(KorSlots[i],"k") ? "fa" :
                                                                        isEquals(KorSlots[i],"l") ? "fi" :
                                                                        isEquals(KorSlots[i],"p") ? "fe" :
                                                                        isEquals(KorSlots[i],"h") ? "fo" : "") :
                                        isContains(queuedLatest,'p') ? (isEquals(KorSlots[i],"k") ? "fa" :
                                                                        isEquals(KorSlots[i],"l") ? "fi" :
                                                                        isEquals(KorSlots[i],"p") ? "fe" :
                                                                        isEquals(KorSlots[i],"h") ? "fo" : "") :
                                        isContains(queuedLatest,'b') ? (isEquals(KorSlots[i],"k") ? "va" :
                                                                        isEquals(KorSlots[i],"l") ? "vi" :
                                                                        isEquals(KorSlots[i],"n") ? "vu" :
                                                                        isEquals(KorSlots[i],"p") ? "ve" :
                                                                        isEquals(KorSlots[i],"h") ? "vo" : "") : "";

                        // Dequeue the latest and Enqueue both an empty and the fixed
                        queueKana.pop_back(); queueKana.push_back("");
                        queueKana.push_back(queuedLatest);
                        logBeQueued.push_back( 1 );
                    }
                    else
                    {
                        queuedLatest = (isEquals(KorSlots[i],"k") ? "la" :
                                        isEquals(KorSlots[i],"l") ? "li" :
                                        isEquals(KorSlots[i],"n") ? "lu" :
                                        isEquals(KorSlots[i],"p") ? "le" :
                                        isEquals(KorSlots[i],"h") ? "lo" : "");

                        queueKana.push_back(queuedLatest);
                        logBeQueued.push_back( 1 );
                    }
                }
                else if
                (!isEquals(KorSlots[i],""))
                {
                    logBeQueued.push_back( 0 );
                }

                // Clear KorSlots
                fillWith(KorSlots[i],"");

                continue;
            }
            else if(i-1>=indexFrom && isExistVowel(KorSlots[i-1]))
            {
                // Hangeul to Kana(a, e, i, o, u, ya, yu, yo)
                queueThing += ( isEquals(KorSlots[i],   "d") ? ""   :
                                isEquals(KorSlots[i],   "z") ? "k"  :
                                isEquals(KorSlots[i],   "t") ? "s"  :
                                isEquals(KorSlots[i],   "c") ? "ch" :
                                isEquals(KorSlots[i],   "x") ? "t"  :
                                isEquals(KorSlots[i],   "s") ? "n"  :
                                isEquals(KorSlots[i],   "g") ? "h"  :
                                isEquals(KorSlots[i],   "a") ? "m"  :
                                isEquals(KorSlots[i],   "f") ? "r"  :
                                isEquals(KorSlots[i],   "r") ? "g"  :
                                isEquals(KorSlots[i],   "w") ? "z"  :
                                isEquals(KorSlots[i],   "e") ? "d"  :
                                isEquals(KorSlots[i],   "q") ? "b"  :
                                isEquals(KorSlots[i],   "v") ? "p"  : "_" );
                queueThing += ( isEquals(KorSlots[i-1], "k") ? "a"  :
                                isEquals(KorSlots[i-1], "l") ? "i"  :
                                isEquals(KorSlots[i-1], "n") ? "u"  :
                                isEquals(KorSlots[i-1], "m") ? "u"  :
                                isEquals(KorSlots[i-1], "p") ? "e"  :
                                isEquals(KorSlots[i-1], "o") ? "e"  :
                                isEquals(KorSlots[i-1], "h") ? "o"  :
                                isEquals(KorSlots[i-1], "i") ? "ya" :
                                isEquals(KorSlots[i-1], "u") ? "ya" :
                                isEquals(KorSlots[i-1], "b") ? "yu" :
                                isEquals(KorSlots[i-1], "y") ? "yo" : "_" );

                // Hangeul to Kana(ch fix)
                if(isContains(queueThing,"ch"))
                {
                    queueThing = (  isContains(queueThing,"chu")  ? "tu" :
                                    isContains(queueThing,"chya") ? "tya":
                                    isContains(queueThing,"chyu") ? "tyu":
                                    isContains(queueThing,"chyo") ? "tyo": queueThing  );
                }

                // Hangeul to Kana(wa, wo) and Error Characters to ""
                if(isContains(queueThing,"_"))
                {
                    queueThing = (String("")+KorSlots[i]+KorSlots[i-1]);
                    queueThing = (  isEquals(queueThing.c_str(),"dhk") ? "wa" :
                                    isEquals(queueThing.c_str(),"dj")  ? "wo" :
                                    isEquals(queueThing.c_str(),"dnj") ? "wo" : ""  );
                }

                // Clear KorSlots
                fillWith(KorSlots[i],"");
                fillWith(KorSlots[i-1],"");
            }
            else
            {
                // Hangeul to Kana(kana n, Sokuon)
                queueThing = (  isEquals(KorSlots[i],   "d") ? "~~" : // kana n
                                isEquals(KorSlots[i],   "s") ? "~n" :
                                isEquals(KorSlots[i],   "a") ? "~m" :
                                isEquals(KorSlots[i],   "r") ? "`g" : // Sokuon
                                isEquals(KorSlots[i],   "e") ? "`d" :
                                isEquals(KorSlots[i],   "q") ? "`b" :
                                isEquals(KorSlots[i],   "t") ? "`s" :
                                isEquals(KorSlots[i],   "z") ? "?k" : // unknown
                                isEquals(KorSlots[i],   "c") ? "?ch":
                                isEquals(KorSlots[i],   "x") ? "?t" :
                                isEquals(KorSlots[i],   "g") ? "?h" :
                                isEquals(KorSlots[i],   "f") ? "?r" :
                                isEquals(KorSlots[i],   "w") ? "?z" :
                                isEquals(KorSlots[i],   "v") ? "?p" : ""  );

                // Clear KorSlots
                fillWith(KorSlots[i],"");
            }
            queueKana.push_back( queueThing );
            lenBeQueued++;
        }
        if(lenBeQueued > 0) logBeQueued.push_back( lenBeQueued );
    };
    auto KORPAD_undoEnqueueKana = [&]() -> void
    {
        if(queueKana.empty())
            return;

        for(int i=0; i<logBeQueued.back(); i++)
            queueKana.pop_back();
        
        logBeQueued.pop_back();
    };
    auto KORPAD_clearAllQueuedKana = [&]() -> void
    {
        queueKana.clear();
        logBeQueued.clear();
    };

    auto KORPAD_surveillanceKorSlotsForKana = [&]() -> void
    {
        if(isKorSlotUpdated)
        {
            if      (isEquals(KorSlot0,""))
            {
                KORPAD_enqueueKana(1);
            }
            else if (isExistConsonant(KorSlot0))
            {
                if(!isExistVowel(KorSlot1) && !KORPAD_isHeldDualConsonant(KorSlot1,KorSlot0))
                    KORPAD_enqueueKana(1);
            }
            else if (isExistVowel(KorSlot0))
            {
                // Prevent enqueue DokkaebiBul errored character
                // DokkaebiBul is... https://namu.wiki/w/%EB%8F%84%EA%B9%A8%EB%B9%84%EB%B6%88%20%ED%98%84%EC%83%81
                if(isExistConsonant(KorSlot1) && isExistVowel(KorSlot2))
                    KORPAD_enqueueKana(2);
            }
        }
        if(isKorSlotUnderflowed)
        {
            KORPAD_undoEnqueueKana();
        }
        if(isKorSlotReset)
        {
            KORPAD_clearAllQueuedKana();
        }
    };

    auto KORPAD_tryToTypeKanaByHangeulPronounce = [&]() -> bool
    {
        if(KORPAD_MODE != KORPAD_MODE_KANA)
            return false;


        COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY;


        KORPAD_enqueueKana(0);
        if(logBeQueued.empty())
            return false;


        // Clear Outputs
        for(uint8_t i=0; i<logBeQueued.size(); i++)
        {   DO_BACKSPACE;   }

        // Set to GJI IME
        SET_IME_TO_GJI;

        // ReType Outputs
        bool isTyped = false;
        for(String strKana : queueKana)
        {
            if(strKana.length() > 0)
            {
                delay(11);

                if      (strKana[0]=='~') // kana n
                {   KBD_HIJACKER.pressandreleaseKeys("nn");   }
                else if (strKana[0]=='`') // Sokuon
                {   KBD_HIJACKER.pressandreleaseKeys("cc"); DO_BACKSPACE;   }
                else if (strKana[0]!='?')
                {   KBD_HIJACKER.pressandreleaseKeys(strKana);   }

                isTyped = true;

                delay(11);
            }
        }

        // Select Kana
        if(isTyped)
        {
            // Open GJI IME's Kana Selector
            KBD_HIJACKER.pressandreleaseKey(KEY_TAB); delay(10);

            // Now... Selecting...
            int32_t keyJustLatestPressed;
            while(numDN)
            {
                delay(1);
                if((keyJustLatestPressed=KBD_PARSER.KeyLogger.peek_keycode(0)) == KEYCODE_KEYPAD_ENTER)
                    continue;

                switch(keyJustLatestPressed)
                {
                    case KEYPAD_ASTERIX:
                    {
                        KBD_HIJACKER.pressandreleaseKey(KEY_ESC);
                    }
                    break;

                    case KEYPAD_8:
                    {
                        KBD_HIJACKER.pressandreleaseKey(KEY_UP_ARROW);
                    }
                    break;
                    case KEYPAD_2:
                    {
                        KBD_HIJACKER.pressandreleaseKey(KEY_DOWN_ARROW);
                    }
                    break;

                    case KEYPAD_9:
                    {
                        KBD_HIJACKER.pressandreleaseKey(KEY_PAGE_UP);
                    }
                    break;
                    case KEYPAD_3:
                    {
                        KBD_HIJACKER.pressandreleaseKey(KEY_PAGE_DOWN);
                    }
                    break;

                    case KEY_TAB:
                    {
                        KBD_HIJACKER.pressandreleaseKey(KEY_TAB);
                    }
                    break;
                    case KEYPAD_PERIOD:
                    {
                        KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_DELETE} );
                    }
                    break;
                }
                KBD_PARSER.KeyLogger.pop();
            }

            // Close GJI IME's Kana Selector
            KBD_HIJACKER.pressandreleaseKey(KEY_ENTER); delay(10);
        }

        // Back to Kor IME
        SET_IME_TO_KOR;


        if(isDEBUG)
        {
            Serial.println("##########################");
            Serial.println("##### ALL KANA TYPED #####");
            Serial.println("HANGEUL LEN   : " + String(logBeQueued.size()));
            for(uint8_t num : logBeQueued){
                Serial.print("_");
                Serial.print(num);
            } Serial.println("_");
            Serial.println("NOW QUEUE HAS : " + String(queueKana.size()));
            for(String str : queueKana){
                Serial.print("_");
                Serial.print(str);
            } Serial.println("_");
            Serial.println("##### ALL KANA TYPED #####");
            Serial.println("##########################");
        }


        KORPAD_clearAllQueuedKana();

        return true;
    };










    if (key == KEY_NUM_LOCK)
    {
        if(KORPAD_MODE > KORPAD_MODE_NONE)
        {
            // Exit the KORPAD MODE
            KORPAD_switchMODE( KORPAD_MODE_NONE );
        }
        else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400)
        {
            // KEY_NUM_LOCK release needed
            KBD_HIJACKER.releaseAllBeingHoldDownKey();

            // Enter the KORPAD MODE
            KORPAD_switchMODE( KORPAD_MODE_HANGEUL );
        }

        return;
    }



    if(KORPAD_MODE == KORPAD_MODE_NONE)
        return;



    switch(key)
    {
        case KEYPAD_SLASH:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_HIJACKER.pressandreleaseKey(KEY_F2); delay(50);
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_C} );
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_F} );
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_ASTERIX:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_HIJACKER.pressandreleaseKey(KEY_F2); delay(50);
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_V} );
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_H} );
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEY_BACKSPACE:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_SHIFT,KEY_LEFT_ARROW} );
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 1000){
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_A} );
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_Z} );
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_MINUS:
        {
            if(event){
                KORPAD_undoConfirmedInputs();

                Keyboard.press(KEY_BACKSPACE);
                KBD_HIJACKER.setLogicalState(KEY_BACKSPACE,true);
                KBD_HIJACKER.reserveReleaseAllBeingHoldDownKey();
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_PLUS:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0," ");

                Keyboard.press(KEY_SPACE);
                KBD_HIJACKER.setLogicalState(KEY_SPACE,true);
                KBD_HIJACKER.reserveReleaseAllBeingHoldDownKey();
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;



        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT1 CheonJiIn ------------------------------------------------ */
        #if (KEYPAD_KOREAN_LAYOUT == 1)

        case KEYPAD_7:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"*")   ){
                    fillWith(KorSlot0,"j");

                    DO_BACKSPACE;               KORPAD_correctAraeAVowelError();
                    Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,":")   ){
                    fillWith(KorSlot0,"u");

                    DO_BACKSPACE;               KORPAD_correctAraeAVowelError();
                    Keyboard.write('u');
                }
                else if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"o");

                    DO_BACKSPACE;
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"i")   ){
                    fillWith(KorSlot0,"O");

                    DO_BACKSPACE;
                    Keyboard.write('O');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"p");

                    DO_BACKSPACE;
                    Keyboard.write('p');
                }
                else if
                (   isEquals(KorSlot0,"u")   ){
                    fillWith(KorSlot0,"P");

                    DO_BACKSPACE;
                    Keyboard.write('P');
                }
                else if
                (   isEquals(KorSlot0,"h")   ){
                    fillWith(KorSlot0,"hl");

                    Keyboard.write('l');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"nl");

                    Keyboard.write('l');
                }
                else if
                (   isEquals(KorSlot0,"m")   ){
                    fillWith(KorSlot0,"ml");

                    Keyboard.write('l');
                }
                else if
                (   isEquals(KorSlot0,"hk")   ){
                    fillWith(KorSlot0,"ho");

                    DO_BACKSPACE;
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"b")   ){
                    fillWith(KorSlot0,"nj");

                    DO_BACKSPACE;
                    Keyboard.write('n'); Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,"nj")   ){
                    fillWith(KorSlot0,"np");

                    DO_BACKSPACE;
                    Keyboard.write('p');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"l");

                    Keyboard.write('l');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"7");

                DO_BACKSPACE;
                Keyboard.write('7');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_8:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"l")   ){
                    fillWith(KorSlot0,"k");

                    DO_BACKSPACE;
                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"i");

                    DO_BACKSPACE;
                    Keyboard.write('i');
                }
                else if
                (   isEquals(KorSlot0,"m")   ){
                    fillWith(KorSlot0,"n");

                    DO_BACKSPACE;
                    Keyboard.write('n');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"b");

                    DO_BACKSPACE;
                    Keyboard.write('b');
                }
                else if
                (   isEquals(KorSlot0,"hl")   ){
                    fillWith(KorSlot0,"hk");

                    DO_BACKSPACE;
                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"*")   ){
                    fillWith(KorSlot0,":");

                    DO_BACKSPACE;
                    Keyboard.write(':');
                }
                else if
                (   isEquals(KorSlot0,":")   ){
                    fillWith(KorSlot0,"*");

                    DO_BACKSPACE;
                    Keyboard.write('*');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"*");

                    Keyboard.write('*');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"8");

                DO_BACKSPACE;
                Keyboard.write('8');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_9:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"*")   ){
                    fillWith(KorSlot0,"h");

                    DO_BACKSPACE;               KORPAD_correctAraeAVowelError();
                    Keyboard.write('h');
                }
                else if
                (   isEquals(KorSlot0,":")   ){
                    fillWith(KorSlot0,"y");

                    DO_BACKSPACE;               KORPAD_correctAraeAVowelError();
                    Keyboard.write('y');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"m");

                    Keyboard.write('m');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"9");

                DO_BACKSPACE;
                Keyboard.write('9');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_4:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"r")   ){
                    fillWith(KorSlot0,"z");

                    DO_BACKSPACE;
                    Keyboard.write('z');
                }
                else if
                (   isEquals(KorSlot0,"z")   ){
                    fillWith(KorSlot0,"R");

                    DO_BACKSPACE;
                    Keyboard.write('R');
                }
                else if
                (   isEquals(KorSlot0,"R")   ){
                    fillWith(KorSlot0,"r");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('r');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"r");

                    Keyboard.write('r');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"4");

                DO_BACKSPACE;
                Keyboard.write('4');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_5:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"s")   ){
                    fillWith(KorSlot0,"f");

                    DO_BACKSPACE;
                    Keyboard.write('f');
                }
                else if
                (   isEquals(KorSlot0,"f")   ){
                    fillWith(KorSlot0,"s");

                    DO_BACKSPACE;
                    Keyboard.write('s');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"s");

                    Keyboard.write('s');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"5");

                DO_BACKSPACE;
                Keyboard.write('5');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_6:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"e")   ){
                    fillWith(KorSlot0,"x");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('x');
                }
                else if
                (   isEquals(KorSlot0,"x")   ){
                    fillWith(KorSlot0,"E");

                    DO_BACKSPACE;
                    Keyboard.write('E');
                }
                else if
                (   isEquals(KorSlot0,"E")   ){
                    fillWith(KorSlot0,"e");

                    DO_BACKSPACE;               KORPAD_correctBottomSingleConsonantError();
                    Keyboard.write('e');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"e");

                    Keyboard.write('e');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"6");

                DO_BACKSPACE;
                Keyboard.write('6');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_1:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"q")   ){
                    fillWith(KorSlot0,"v");

                    DO_BACKSPACE;
                    Keyboard.write('v');
                }
                else if
                (   isEquals(KorSlot0,"v")   ){
                    fillWith(KorSlot0,"Q");

                    DO_BACKSPACE;
                    Keyboard.write('Q');
                }
                else if
                (   isEquals(KorSlot0,"Q")   ){
                    fillWith(KorSlot0,"q");

                    DO_BACKSPACE;               KORPAD_correctBottomSingleConsonantError();
                                                KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('q');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"q");

                    Keyboard.write('q');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"1");

                DO_BACKSPACE;
                Keyboard.write('1');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_2:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"t")   ){
                    fillWith(KorSlot0,"g");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('g');
                }
                else if
                (   isEquals(KorSlot0,"g")   ){
                    fillWith(KorSlot0,"T");

                    DO_BACKSPACE;
                    Keyboard.write('T');
                }
                else if
                (   isEquals(KorSlot0,"T")   ){
                    fillWith(KorSlot0,"t");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('t');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"t");

                    Keyboard.write('t');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"2");

                DO_BACKSPACE;
                Keyboard.write('2');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_3:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"w")   ){
                    fillWith(KorSlot0,"c");

                    DO_BACKSPACE;
                    Keyboard.write('c');
                }
                else if
                (   isEquals(KorSlot0,"c")   ){
                    fillWith(KorSlot0,"W");

                    DO_BACKSPACE;
                    Keyboard.write('W');
                }
                else if
                (   isEquals(KorSlot0,"W")   ){
                    fillWith(KorSlot0,"w");

                    DO_BACKSPACE;               KORPAD_correctBottomSingleConsonantError();
                                                KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('w');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"w");

                    Keyboard.write('w');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"3");

                DO_BACKSPACE;
                Keyboard.write('3');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_0:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"d")   ){
                    fillWith(KorSlot0,"a");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('a');
                }
                else if
                (   isEquals(KorSlot0,"a")   ){
                    fillWith(KorSlot0,"d");

                    DO_BACKSPACE;
                    Keyboard.write('d');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"d");

                    Keyboard.write('d');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"0");

                DO_BACKSPACE;
                Keyboard.write('0');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        #endif
        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT1 CheonJiIn ------------------------------------------------ */



        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT2 NaRatGeul ------------------------------------------------ */
        #if (KEYPAD_KOREAN_LAYOUT == 2)

        case KEYPAD_7:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                fillWith(KorSlot0,"r");

                Keyboard.write('r');
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"7");

                DO_BACKSPACE;
                Keyboard.write('7');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_8:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                fillWith(KorSlot0,"s");

                Keyboard.write('s');
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"8");

                DO_BACKSPACE;
                Keyboard.write('8');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_9:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"j");

                    DO_BACKSPACE;
                    Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"k");

                    DO_BACKSPACE;
                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"h")   ){
                    fillWith(KorSlot0,"hk");

                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"nj");

                    Keyboard.write('j');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"k");

                    Keyboard.write('k');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"9");

                DO_BACKSPACE;
                Keyboard.write('9');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_4:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                fillWith(KorSlot0,"f");

                Keyboard.write('f');
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"4");

                DO_BACKSPACE;
                Keyboard.write('4');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_5:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                fillWith(KorSlot0,"a");

                Keyboard.write('a');
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"5");

                DO_BACKSPACE;
                Keyboard.write('5');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_6:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"h")   ){
                    fillWith(KorSlot0,"n");

                    DO_BACKSPACE;
                    Keyboard.write('n');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"h");

                    DO_BACKSPACE;
                    Keyboard.write('h');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"h");

                    Keyboard.write('h');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"6");

                DO_BACKSPACE;
                Keyboard.write('6');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_1:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                fillWith(KorSlot0,"t");

                Keyboard.write('t');
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"1");

                DO_BACKSPACE;
                Keyboard.write('1');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_2:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                fillWith(KorSlot0,"d");

                Keyboard.write('d');
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"2");

                DO_BACKSPACE;
                Keyboard.write('2');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_3:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"o");

                    DO_BACKSPACE;
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"i")   ){
                    fillWith(KorSlot0,"O");

                    DO_BACKSPACE;
                    Keyboard.write('O');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"p");

                    DO_BACKSPACE;
                    Keyboard.write('p');
                }
                else if
                (   isEquals(KorSlot0,"u")   ){
                    fillWith(KorSlot0,"P");

                    DO_BACKSPACE;
                    Keyboard.write('P');
                }
                else if
                (   isEquals(KorSlot0,"h")   ){
                    fillWith(KorSlot0,"hl");

                    Keyboard.write('l');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"nl");

                    Keyboard.write('l');
                }
                else if
                (   isEquals(KorSlot0,"m")   ){
                    fillWith(KorSlot0,"ml");

                    Keyboard.write('l');
                }
                else if
                (   isEquals(KorSlot0,"hk")   ){
                    fillWith(KorSlot0,"ho");

                    DO_BACKSPACE;
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"nj")   ){
                    fillWith(KorSlot0,"np");

                    DO_BACKSPACE;
                    Keyboard.write('p');
                }
                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"l");

                    Keyboard.write('l');
                }
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"3");

                DO_BACKSPACE;
                Keyboard.write('3');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_0:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                fillWith(KorSlot0,"m");

                Keyboard.write('m');
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0,"0");

                DO_BACKSPACE;
                Keyboard.write('0');
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        #endif
        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT2 NaRatGeul ------------------------------------------------ */



        case KEY_TAB:
        {
            if(event){
                #if (KEYPAD_KOREAN_LAYOUT == 2) // NaRatGeul

                if
                (   isEquals(KorSlot0,"r")   ){
                    fillWith(KorSlot0,"z");

                    DO_BACKSPACE;
                    Keyboard.write('z');
                }
                else if
                (   isEquals(KorSlot0,"z")   ){
                    fillWith(KorSlot0,"r");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('r');
                }
                else if
                (   isEquals(KorSlot0,"s")   ){
                    fillWith(KorSlot0,"e");

                    DO_BACKSPACE;
                    Keyboard.write('e');
                }
                else if
                (   isEquals(KorSlot0,"e")   ){
                    fillWith(KorSlot0,"x");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('x');
                }
                else if
                (   isEquals(KorSlot0,"x")   ){
                    fillWith(KorSlot0,"s");

                    DO_BACKSPACE;
                    Keyboard.write('s');
                }
                else if
                (   isEquals(KorSlot0,"a")   ){
                    fillWith(KorSlot0,"q");

                    DO_BACKSPACE;
                    Keyboard.write('q');
                }
                else if
                (   isEquals(KorSlot0,"q")   ){
                    fillWith(KorSlot0,"v");

                    DO_BACKSPACE;
                    Keyboard.write('v');
                }
                else if
                (   isEquals(KorSlot0,"v")   ){
                    fillWith(KorSlot0,"a");

                    DO_BACKSPACE;
                    Keyboard.write('a');
                }
                else if
                (   isEquals(KorSlot0,"t")   ){
                    fillWith(KorSlot0,"w");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('w');
                }
                else if
                (   isEquals(KorSlot0,"w")   ){
                    fillWith(KorSlot0,"c");

                    DO_BACKSPACE;
                    Keyboard.write('c');
                }
                else if
                (   isEquals(KorSlot0,"c")   ){
                    fillWith(KorSlot0,"t");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('t');
                }
                else if
                (   isEquals(KorSlot0,"d")   ){
                    fillWith(KorSlot0,"g");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('g');
                }
                else if
                (   isEquals(KorSlot0,"g")   ){
                    fillWith(KorSlot0,"d");

                    DO_BACKSPACE;
                    Keyboard.write('d');
                }
                else if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"i");

                    DO_BACKSPACE;
                    Keyboard.write('i');
                }
                else if
                (   isEquals(KorSlot0,"i")   ){
                    fillWith(KorSlot0,"k");

                    DO_BACKSPACE;
                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"u");

                    DO_BACKSPACE;
                    Keyboard.write('u');
                }
                else if
                (   isEquals(KorSlot0,"u")   ){
                    fillWith(KorSlot0,"j");

                    DO_BACKSPACE;
                    Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,"h")   ){
                    fillWith(KorSlot0,"y");

                    DO_BACKSPACE;
                    Keyboard.write('y');
                }
                else if
                (   isEquals(KorSlot0,"y")   ){
                    fillWith(KorSlot0,"h");

                    DO_BACKSPACE;
                    Keyboard.write('h');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"b");

                    DO_BACKSPACE;
                    Keyboard.write('b');
                }
                else if
                (   isEquals(KorSlot0,"b")   ){
                    fillWith(KorSlot0,"n");

                    DO_BACKSPACE;
                    Keyboard.write('n');
                }

                #endif                          // NaRatGeul End
            }
            else 
            if(isEquals(KorSlot0,"") || isEquals(KorSlot0," "))
            {
                if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400)
                {
                    if(KORPAD_MODE != KORPAD_MODE_KANA)
                    {
                        // Preclude about GJI malfunction
                        SET_IME_TO_KOR;

                        // Turn On KORPAD_MODE_KANA
                        KORPAD_switchMODE( KORPAD_MODE_KANA );

                        // Clear the KANA Queue
                        KORPAD_clearAllQueuedKana();
                    }
                    else
                    {
                        // Preclude about GJI malfunction
                        SET_IME_TO_KOR;

                        // Back To KORPAD_MODE_HANGEUL
                        KORPAD_switchMODE( KORPAD_MODE_HANGEUL );
                    }
                }
                else
                {
                    KORPAD_resetConfirmedInputs();

                    KBD_HIJACKER.pressandreleaseKey(KEY_TAB);
                }
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_PERIOD:
        {
            if(event){
                // When KORPAD_MODE_KANA, Prevent other Conversions
                if
                (   KORPAD_MODE == KORPAD_MODE_KANA   ){
                    if(KBD_PARSER.KeyLogger.peek_keycode(1)!=KEYCODE_KEYPAD_PERIOD) // Prevent duplicated updates
                    {   
                        KORPAD_updateConfirmedInputs();
                        COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY;
                    }
                }
                else

                #if (KEYPAD_KOREAN_LAYOUT == 1) // CheonJiIn

                if
                (   isEquals(KorSlot0,"") || isEquals(KorSlot0," ")   ){
                    fillWith(KorSlot0,"\\ ");
                }
                else 

                #endif // CheonJiIn End

                #if (KEYPAD_KOREAN_LAYOUT == 2) // NaRatGeul

                // NaRatGeul's Double Consonent Conversions
                if
                (   isEquals(KorSlot0,"r")   ){
                    fillWith(KorSlot0,"R");

                    DO_BACKSPACE;
                    Keyboard.write('R');
                }
                else if
                (   isEquals(KorSlot0,"R")   ){
                    fillWith(KorSlot0,"r");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('r');
                }
                else if
                (   isEquals(KorSlot0,"e")   ){
                    fillWith(KorSlot0,"E");

                    DO_BACKSPACE;
                    Keyboard.write('E');
                }
                else if
                (   isEquals(KorSlot0,"E")   ){
                    fillWith(KorSlot0,"e");

                    DO_BACKSPACE;               KORPAD_correctBottomSingleConsonantError();
                    Keyboard.write('e');
                }
                else if
                (   isEquals(KorSlot0,"q")   ){
                    fillWith(KorSlot0,"Q");

                    DO_BACKSPACE;
                    Keyboard.write('Q');
                }
                else if
                (   isEquals(KorSlot0,"Q")   ){
                    fillWith(KorSlot0,"q");

                    DO_BACKSPACE;               KORPAD_correctBottomSingleConsonantError();
                                                KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('q');
                }
                else if
                (   isEquals(KorSlot0,"t")   ){
                    fillWith(KorSlot0,"T");

                    DO_BACKSPACE;
                    Keyboard.write('T');
                }
                else if
                (   isEquals(KorSlot0,"T")   ){
                    fillWith(KorSlot0,"t");

                    DO_BACKSPACE;               KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('t');
                }
                else if
                (   isEquals(KorSlot0,"w")   ){
                    fillWith(KorSlot0,"W");

                    DO_BACKSPACE;
                    Keyboard.write('W');
                }
                else if
                (   isEquals(KorSlot0,"W")   ){
                    fillWith(KorSlot0,"w");

                    DO_BACKSPACE;               KORPAD_correctBottomSingleConsonantError();
                                                KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('w');

                // NaRatGeul's Special Character Conversions
                }
                else if
                (   isEquals(KorSlot0,"") || isEquals(KorSlot0," ")   ){

                    /* NOTHING */

                }
                else if
                (   isEquals(KorSlot0,";")   ){
                    fillWith(KorSlot0,"()");

                    DO_BACKSPACE;
                    Keyboard.write('('); Keyboard.write(')');
                }
                else if
                (   isEquals(KorSlot0,"()")   ){
                    fillWith(KorSlot0,"{}");

                    DO_BACKSPACE; DO_BACKSPACE;
                    Keyboard.write('{'); Keyboard.write('}');
                }
                else if
                (   isEquals(KorSlot0,"{}")   ){
                    fillWith(KorSlot0,"?");

                    DO_BACKSPACE; DO_BACKSPACE;
                    Keyboard.write('?');
                }
                else if
                (   isEquals(KorSlot0,"?")   ){
                    fillWith(KorSlot0,"!");

                    DO_BACKSPACE;
                    Keyboard.write('!');
                }
                else if
                (   isEquals(KorSlot0,"!")   ){
                    fillWith(KorSlot0,".");

                    DO_BACKSPACE;
                    Keyboard.write('.');
                }
                else if
                (   isEquals(KorSlot0,".")   ){
                    fillWith(KorSlot0,"~");

                    DO_BACKSPACE;
                    Keyboard.write('~');
                }
                else if
                (   isEquals(KorSlot0,"~")   ){
                    fillWith(KorSlot0,"^^");

                    DO_BACKSPACE;
                    Keyboard.write('^'); Keyboard.write('^');
                }
                else if
                (   isEquals(KorSlot0,"^^")   ){
                    fillWith(KorSlot0,"\\ ");

                    DO_BACKSPACE; DO_BACKSPACE;
                    Keyboard.write(' ');
                }
                else if
                (   isEquals(KorSlot0,"\\ ")   ){
                    fillWith(KorSlot0,";");

                    DO_BACKSPACE;
                    Keyboard.write(';');
                }
                else 

                #endif // NaRatGeul End

                // KORPAD_MODE_ALPHABET
                if
                (   KORPAD_tryToDoAlphabetCaseConversion()   )
                {
                    /* Already done Alphabet Case Conversion */
                }

                else
                {
                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"");

                    COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY;
                }
            }
            else 
            if(isEquals(KorSlot0,"") || isEquals(KorSlot0," ") || isEquals(KorSlot0,"\\ "))
            {
                if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400)
                {
                    if(KORPAD_MODE != KORPAD_MODE_ALPHABET)
                    {
                        // Turn On KORPAD_MODE_ALPHABET
                        KORPAD_switchMODE( KORPAD_MODE_ALPHABET );
                    }
                    else
                    {
                        // Back To KORPAD_MODE_HANGEUL
                        KORPAD_switchMODE( KORPAD_MODE_HANGEUL );
                    }
                }
                else
                {
                    #if (KEYPAD_KOREAN_LAYOUT == 1) // CheonJiIn

                    if
                    (   isEquals(KorSlot0,"\\ ")   ){
                        KORPAD_resetConfirmedInputs();

                        DO_SPACE;
                    }

                    #endif // CheonJiIn End

                    #if (KEYPAD_KOREAN_LAYOUT == 2) // NaRatGeul

                    if
                    (   isEquals(KorSlot0," ")   ){
                        fillWith(KorSlot0,";");

                        DO_BACKSPACE;
                        Keyboard.write(';');
                    }

                    #endif // NaRatGeul End
                }
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;

        case KEYPAD_ENTER:
        {
            if(event){
                // both MODEs, [ " " + ENTER ] means [ SHIFT + ENTER ]
                if(isEquals(KorSlot0," "))
                {
                    KORPAD_resetConfirmedInputs();

                    DO_BACKSPACE;
                    KBD_HIJACKER.pressandreleaseMultiKey( {KEY_SHIFT,KEY_ENTER} );
                }

                // KORPAD_MODE_ALPHABET
                else if(KORPAD_tryToTypeAlphabetByHangeulPronounce())
                {
                    /* Already typed Alphabet By Hangeul Pronounce */
                }

                // KORPAD_MODE_KANA
                else if(KORPAD_tryToTypeKanaByHangeulPronounce())
                {
                    /* Already typed Kana By Hangeul Pronounce */
                }

                // KORPAD_MODE_HANGEUL and also, When typing is disallowed on other MODEs
                else
                {
                    KORPAD_resetConfirmedInputs();

                    KBD_HIJACKER.pressandreleaseKey(KEY_ENTER);
                }
            }
            isActivatedKeyEvent=false; key=KEY_NONE;
        }
        break;
    }



    if(KORPAD_MODE == KORPAD_MODE_KANA)
        KORPAD_surveillanceKorSlotsForKana();

#endif
/* ------------------------------------------------------ KEYPAD_KOREAN_LAYOUT COMMON ------------------------------------------------------ */
}
