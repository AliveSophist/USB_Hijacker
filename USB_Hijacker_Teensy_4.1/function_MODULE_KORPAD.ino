
#define KEYPAD_KOREAN_LAYOUT 2      // 0:None   1:CheonJiIn   2:NaRatGeul

void KeyboardHijacker::MODULE_KOREAN_KEYPAD_EVOLUTION()
{
/* ------------------------------------------------------ KEYPAD_KOREAN_LAYOUT COMMON ------------------------------------------------------ */
#if (KEYPAD_KOREAN_LAYOUT > 0)

    static bool KoreanKeypadToggle = false;

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
                                            strcpy(KorSlot4,"");

                                            char* temp = KorSlot4;
                                            KorSlot4 = KorSlot3;
                                            KorSlot3 = KorSlot2;
                                            KorSlot2 = KorSlot1;
                                            KorSlot1 = KorSlot0;
                                            KorSlot0 = temp;
                                            
                                            isKorSlotUpdated = true;
                                        };
    auto KORPAD_undoConfirmedInputs =   [&]() -> void
                                        {
                                            if(strcmp(KorSlot0,"")==0){
                                                isKorSlotUnderflowed = true;
                                                return;
                                            }

                                            strcpy(KorSlot0,"");

                                            char* temp = KorSlot0;
                                            KorSlot0 = KorSlot1;
                                            KorSlot1 = KorSlot2;
                                            KorSlot2 = KorSlot3;
                                            KorSlot3 = KorSlot4;
                                            KorSlot4 = temp;
                                        };
    auto KORPAD_resetConfirmedInputs =  [&]() -> void
                                        {
                                            strcpy(KorSlot4,"");
                                            strcpy(KorSlot3,"");
                                            strcpy(KorSlot2,"");
                                            strcpy(KorSlot1,"");
                                            strcpy(KorSlot0,"");

                                            isKorSlotReset = true;
                                        };



    /**⦓   KORPAD_MODE   ⦔**/

    #define MODE_HANGEUL    0
    #define MODE_ALPHABET   1
    #define MODE_KANA       2
    static uint8_t KORPAD_MODE = MODE_HANGEUL;



    /**⦓   About, MODE_HANGEUL   ⦔**/

    #define isEquals(KorSlotX,STR)      (strcmp(KorSlotX,STR)==0)
    #define fillWith(KorSlotX,STR)      strcpy(KorSlotX,STR)

    auto isExistConsonant =             [](char* KorSlotX) -> bool
                                        {
                                            return (strcmp(KorSlotX,"r")==0||strcmp(KorSlotX,"z")==0||strcmp(KorSlotX,"R")==0||strcmp(KorSlotX,"s")==0||strcmp(KorSlotX,"f")==0||strcmp(KorSlotX,"e")==0||strcmp(KorSlotX,"x")==0||strcmp(KorSlotX,"E")==0||strcmp(KorSlotX,"q")==0||strcmp(KorSlotX,"v")==0||strcmp(KorSlotX,"Q")==0||strcmp(KorSlotX,"t")==0||strcmp(KorSlotX,"g")==0||strcmp(KorSlotX,"T")==0||strcmp(KorSlotX,"w")==0||strcmp(KorSlotX,"c")==0||strcmp(KorSlotX,"W")==0||strcmp(KorSlotX,"d")==0||strcmp(KorSlotX,"a")==0);
                                        };
    auto isExistVowel =                 [](char* KorSlotX) -> bool
                                        {
                                            return (strcmp(KorSlotX,"k")==0||strcmp(KorSlotX,"o")==0||strcmp(KorSlotX,"i")==0||strcmp(KorSlotX,"O")==0||strcmp(KorSlotX,"j")==0||strcmp(KorSlotX,"p")==0||strcmp(KorSlotX,"u")==0||strcmp(KorSlotX,"P")==0||strcmp(KorSlotX,"h")==0||strcmp(KorSlotX,"y")==0||strcmp(KorSlotX,"n")==0||strcmp(KorSlotX,"b")==0||strcmp(KorSlotX,"m")==0||strcmp(KorSlotX,"l")==0||strcmp(KorSlotX,"hk")==0||strcmp(KorSlotX,"ho")==0||strcmp(KorSlotX,"hl")==0||strcmp(KorSlotX,"nj")==0||strcmp(KorSlotX,"np")==0||strcmp(KorSlotX,"nl")==0||strcmp(KorSlotX,"ml")==0);
                                        };
    auto KORPAD_isHeldDualConsonant =   [](char* KorSlotX, char* KorSlotY) -> bool
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

    auto KORPAD_correctBottomDualConsonantError =   [&]() -> void
                                                    {
                                                        if(!KORPAD_isHeldDualConsonant(KorSlot1,KorSlot0))
                                                            return;

                                                        if(isExistVowel(KorSlot2))
                                                        {
                                                            if(isExistConsonant(KorSlot3))
                                                            {
                                                                delay(11); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                                                                delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot3);
                                                                delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot2);
                                                                delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot1);
                                                                delay(11);
                                                            }
                                                        }
                                                    };
    auto KORPAD_correctBottomSingleConsonantError = [&]() -> void
                                                    {
                                                        if(isExistVowel(KorSlot1))
                                                        {
                                                            if(isExistConsonant(KorSlot2))
                                                            {
                                                                delay(11); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                                                                delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot2);
                                                                delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot1);
                                                                delay(11);
                                                            }
                                                        }
                                                    };

    #if (KEYPAD_KOREAN_LAYOUT == 1) // CheonJiIn Only

    auto KORPAD_correctAraeAVowelError =    [&]() -> void
                                            {
                                                if(isExistConsonant(KorSlot1))
                                                {
                                                    if
                                                    (   isExistVowel(KorSlot2) &&
                                                        isExistConsonant(KorSlot3)
                                                    )
                                                    {
                                                        delay(11); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot3);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot2);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot1);
                                                        delay(11);
                                                    }
                                                    else if
                                                    (   /*isExistConsonant(KorSlot2) &&*/
                                                        KORPAD_isHeldDualConsonant(KorSlot2,KorSlot1) &&
                                                        isExistVowel(KorSlot3) &&
                                                        isExistConsonant(KorSlot4)
                                                    )
                                                    {
                                                        delay(11); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot4);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot3);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot2);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot1);
                                                        delay(11);
                                                    }
                                                    else
                                                    {
                                                        delay(11); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                                                        delay(11); KBD_HIJACKER.pressandreleaseKeys(KorSlot1);
                                                        delay(11);
                                                    }
                                                    
                                                    isKorSlotUpdated = true;
                                                }
                                            };

    #endif

    #define COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY    KBD_HIJACKER.pressandreleaseKey(KEY_SPACE); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);



    /**⦓   About, MODE_ALPHABET   ⦔**/

    #define KORPAD_writeAlphabet(CHSTR)     KBD_HIJACKER.pressandreleaseKey(KEY_KORENG); delay(11); Keyboard.write(CHSTR); delay(11); KBD_HIJACKER.pressandreleaseKey(KEY_KORENG); delay(11);

    auto KORPAD_tryToTypeAlphabetByHangeulPronounce =   [&]() -> bool
                                                        {
                                                            if(KORPAD_MODE != MODE_ALPHABET)
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

                                                            alphabet = (alphabet!=0) ? alphabet :
                                                            KORPAD_isAlphabetPronounce("d","o","d","l") ? 'a' :
                                                            KORPAD_isAlphabetPronounce("d","p","d","l") ? 'a' :
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

                                                            char isExCase_dl_ = 0;
                                                            alphabet = (alphabet!=0) ? alphabet :
                                                            KORPAD_isAlphabetPronounce("","","d","k")   ? 'a' :
                                                            KORPAD_isAlphabetPronounce("","","d","o")   ? 'a' :
                                                            KORPAD_isAlphabetPronounce("","","q","l")   ? 'b' :
                                                            KORPAD_isAlphabetPronounce("","","Q","l")   ? 'b' :
                                                            KORPAD_isAlphabetPronounce("","","t","l")   ? 'c' :
                                                            KORPAD_isAlphabetPronounce("","","T","l")   ? 'c' :
                                                            KORPAD_isAlphabetPronounce("","","z","m")   ? 'c' :
                                                            KORPAD_isAlphabetPronounce("","","e","l")   ? 'd' :
                                                            KORPAD_isAlphabetPronounce("","","e","m")   ? 'd' :
                                                            KORPAD_isAlphabetPronounce("","","d","l")   ? (isExCase_dl_='e') :
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
                                                            KORPAD_isAlphabetPronounce("","","d","j")   ? 'u' :
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
                                                                KORPAD_resetConfirmedInputs(); if(isExCase_dl_!=0)fillWith(KorSlot1,"l");
                                                                KorSlot0[0]='@'; KorSlot0[1]=alphabet;

                                                                // Type!
                                                                for(uint8_t i=0; i<numNeededBackspace; i++){ KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); delay(11); }
                                                                KORPAD_writeAlphabet( alphabet );

                                                                return true;
                                                            }

                                                            alphabet =
                                                            //Hangeul Consonent to Alphabet
                                                            isEquals(KorSlot0, "r") ? 'g' :
                                                            isEquals(KorSlot1, "r") 
                                                                                    ? ( isEquals(KorSlot0,"@g") ? 'g' : 0 ) :
                                                            isEquals(KorSlot0, "s") ? 'n' :
                                                            isEquals(KorSlot1, "s") 
                                                                                    ? ( isEquals(KorSlot0,"@n") ? 'n' : 0 ) :
                                                            isEquals(KorSlot0, "e") ? 'd' :
                                                            isEquals(KorSlot1, "e")
                                                                                    ? ( isEquals(KorSlot0,"@d") ? 'd' : 0 ) :
                                                            isEquals(KorSlot0, "f") ? 'l' :
                                                            isEquals(KorSlot1, "f")
                                                                                    ? ( isEquals(KorSlot0,"@l") ? 'r' :
                                                                                        isEquals(KorSlot0,"@r") ? 'l' : 0 ) :
                                                            isEquals(KorSlot0, "a") ? 'm' :
                                                            isEquals(KorSlot1, "a")
                                                                                    ? ( isEquals(KorSlot0,"@m") ? 'm' : 0 ) :
                                                            isEquals(KorSlot0, "q") ? 'b' :
                                                            isEquals(KorSlot1, "q")
                                                                                    ? ( isEquals(KorSlot0,"@b") ? 'v' :
                                                                                        isEquals(KorSlot0,"@v") ? 'b' : 0 ) :
                                                            isEquals(KorSlot0, "t") ? 's' :
                                                            isEquals(KorSlot1, "t")
                                                                                    ? ( isEquals(KorSlot0,"@s") ? 'c' :
                                                                                        isEquals(KorSlot0,"@c") ? 'x' :
                                                                                        isEquals(KorSlot0,"@x") ? 's' : 0 ) :
                                                            isEquals(KorSlot0, "T") ? 'c' :
                                                            isEquals(KorSlot1, "T")
                                                                                    ? ( isEquals(KorSlot0,"@c") ? 'x' :
                                                                                        isEquals(KorSlot0,"@x") ? 'c' : 0 ) :
                                                            isEquals(KorSlot0, "d") ? '~' :
                                                            isEquals(KorSlot1, "d")
                                                                                    ? ( isEquals(KorSlot0,"@~") ? '~' : 0 ) : // '~' is ng
                                                            isEquals(KorSlot0, "w") ? 'j' :
                                                            isEquals(KorSlot1, "w")
                                                                                    ? ( isEquals(KorSlot0,"@j") ? 'g' :
                                                                                        isEquals(KorSlot0,"@g") ? 'z' :
                                                                                        isEquals(KorSlot0,"@z") ? 'j' : 0 ) :
                                                            isEquals(KorSlot0, "c") ? '*' :
                                                            isEquals(KorSlot1, "c")
                                                                                    ? ( isEquals(KorSlot0,"@*") ? '*' : 0 ) : // '*' is ch
                                                            isEquals(KorSlot0, "z") ? 'c' :
                                                            isEquals(KorSlot1, "z")
                                                                                    ? ( isEquals(KorSlot0,"@c") ? 'k' :
                                                                                        isEquals(KorSlot0,"@k") ? 'q' :
                                                                                        isEquals(KorSlot0,"@q") ? 'x' :
                                                                                        isEquals(KorSlot0,"@x") ? 'c' : 0 ) :
                                                            isEquals(KorSlot0, "x") ? 't' :
                                                            isEquals(KorSlot1, "x")
                                                                                    ? ( isEquals(KorSlot0,"@t") ? 't' : 0 ) :
                                                            isEquals(KorSlot0, "v") ? 'f' :
                                                            isEquals(KorSlot1, "v")
                                                                                    ? ( isEquals(KorSlot0,"@f") ? 'p' :
                                                                                        isEquals(KorSlot0,"@p") ? 'f' : 0 ) :
                                                            isEquals(KorSlot0, "g") ? 'h' :
                                                            isEquals(KorSlot1, "g")
                                                                                    ? ( isEquals(KorSlot0,"@h") ? 'h' : 0 ) :
                                                            //Hangeul Vowel to Alphabet
                                                            isEquals(KorSlot0, "k") ? 'a' :
                                                            isEquals(KorSlot1, "k")
                                                                                    ? ( isEquals(KorSlot0,"@a") ? 'a' : 0 ) :
                                                            isEquals(KorSlot0, "o") ? 'a' :
                                                            isEquals(KorSlot1, "o")
                                                                                    ? ( isEquals(KorSlot0,"@a") ? 'a' : 0 ) :
                                                            isEquals(KorSlot0, "j") ? 'e' :
                                                            isEquals(KorSlot1, "j")
                                                                                    ? ( isEquals(KorSlot0,"@e") ? 'e' : 0 ) :
                                                            isEquals(KorSlot0, "p") ? 'e' :
                                                            isEquals(KorSlot1, "p")
                                                                                    ? ( isEquals(KorSlot0,"@e") ? 'e' : 0 ) :
                                                            isEquals(KorSlot0, "h") ? 'o' :
                                                            isEquals(KorSlot1, "h")
                                                                                    ? ( isEquals(KorSlot0,"@o") ? 'o' : 0 ) :
                                                            isEquals(KorSlot0, "n") ? 'u' :
                                                            isEquals(KorSlot1, "n")
                                                                                    ? ( isEquals(KorSlot0,"@u") ? 'u' : 0 ) :
                                                            isEquals(KorSlot0, "i") ? 'y' :
                                                            isEquals(KorSlot1, "i")
                                                                                    ? ( isEquals(KorSlot0,"@y") ? 'y' : 0 ) :
                                                            isEquals(KorSlot0, "u") ? 'y' :
                                                            isEquals(KorSlot1, "u")
                                                                                    ? ( isEquals(KorSlot0,"@y") ? 'y' : 0 ) :
                                                            isEquals(KorSlot0, "y") ? 'y' :
                                                            isEquals(KorSlot1, "y")
                                                                                    ? ( isEquals(KorSlot0,"@y") ? 'y' : 0 ) :
                                                            isEquals(KorSlot0, "b") ? 'w' :
                                                            isEquals(KorSlot1, "b")
                                                                                    ? ( isEquals(KorSlot0,"@w") ? 'w' : 0 ) :
                                                            isEquals(KorSlot0, "m") ? 'e' :
                                                            isEquals(KorSlot1, "m")
                                                                                    ? ( isEquals(KorSlot0,"@e") ? 'e' : 0 ) :
                                                            isEquals(KorSlot0, "l") ? 'i' :
                                                            isEquals(KorSlot1, "l")
                                                                                    ? ( isEquals(KorSlot0,"@i") ? 'e' :
                                                                                        isEquals(KorSlot0,"@e") ? 'i' : 0 ) : 0;

                                                            if(alphabet==KorSlot0[1]) // DO NOT NEED TO RETYPE
                                                                return true;

                                                            if(alphabet!=0)
                                                            {
                                                                // Tune KorSlots
                                                                if(KorSlot0[0]!='@'){ String temp(KorSlot0); KORPAD_resetConfirmedInputs(); fillWith(KorSlot1,temp.c_str()); }
                                                                KorSlot0[0]='@'; KorSlot0[1]=alphabet;

                                                                // Type!
                                                                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); delay(11);
                                                                KORPAD_writeAlphabet( alphabet=='~' ? "ng" : alphabet=='*' ? "ch" : String(alphabet).c_str() );

                                                                return true;
                                                            }

                                                            return false;
                                                        };

    auto KORPAD_tryToDoAlphabetCaseConversion =         [&]() -> bool
                                                        {
                                                            if(KORPAD_MODE != MODE_ALPHABET)
                                                                return false;

                                                            if(KorSlot0[0] != '@')
                                                                return false;

                                                            if(!(('A'<=KorSlot0[1] && KorSlot0[1]<='Z') || ('a'<=KorSlot0[1] && KorSlot0[1]<='z')))
                                                                return false;


                                                            if('A'<=KorSlot0[1] && KorSlot0[1]<='Z')
                                                            {
                                                                KorSlot0[1] += 32;

                                                                // switch CapsLockToggle, If Case Conversion Battered 4 times
                                                                if(KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_PERIOD && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_PERIOD && KBD_PARSER.KeyLogger.peek_key(3)==KEYPAD_PERIOD) KBD_HIJACKER.pressandreleaseKey(KEY_CAPS_LOCK);

                                                                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); delay(11);
                                                                KORPAD_writeAlphabet( KorSlot0[1] );
                                                            }
                                                            else
                                                            {
                                                                KorSlot0[1] -= 32;

                                                                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); delay(11);
                                                                KORPAD_writeAlphabet( KorSlot0[1] );

                                                                // switch CapsLockToggle, If Case Conversion Battered 3 times
                                                                if(KBD_PARSER.KeyLogger.peek_key(1)==KEYPAD_PERIOD && KBD_PARSER.KeyLogger.peek_key(2)==KEYPAD_PERIOD) KBD_HIJACKER.pressandreleaseKey(KEY_CAPS_LOCK);
                                                            }

                                                            return true;
                                                        };



    /**⦓   About, MODE_KANA   ⦔**/

    static std::list<String> queueKana;
    static std::list<uint8_t> logBeQueued;

    auto KORPAD_doQueueingKana =                [&](int8_t indexExcludeSlot) -> void
                                                {
                                                    char* KorSlots[] = {KorSlot0, KorSlot1, KorSlot2, KorSlot3, KorSlot4};
                                                    uint8_t lenBeQueued = 0;
                                                    String queueThing;

                                                    for(int8_t i=4; i>indexExcludeSlot; i--)
                                                    {
                                                        if(!isExistConsonant(KorSlots[i]))
                                                            continue;

                                                        if(i-1>indexExcludeSlot && isExistVowel(KorSlots[i-1]))
                                                        {
                                                            // Hangeul to Kana (exclude nn, cc Kana)
                                                            fillWith(KorSlots[i],   isEquals(KorSlots[i],"z") ? "k" :
                                                                                    isEquals(KorSlots[i],"t") ? "s" :
                                                                                    isEquals(KorSlots[i],"c") ? "t" :
                                                                                    isEquals(KorSlots[i],"x") ? "t" :
                                                                                    isEquals(KorSlots[i],"s") ? "n" :
                                                                                    isEquals(KorSlots[i],"g") ? "h" :
                                                                                    isEquals(KorSlots[i],"a") ? "m" :
                                                                                    isEquals(KorSlots[i],"f") ? "r" :
                                                                                    isEquals(KorSlots[i],"r") ? "g" :
                                                                                    isEquals(KorSlots[i],"w") ? "z" :
                                                                                    isEquals(KorSlots[i],"e") ? "d" :
                                                                                    isEquals(KorSlots[i],"q") ? "b" :
                                                                                    isEquals(KorSlots[i],"v") ? "p" : "" );
                                                            fillWith(KorSlots[i-1], isEquals(KorSlots[i-1],"k") ? "a" :
                                                                                    isEquals(KorSlots[i-1],"o") ? "a" :
                                                                                    isEquals(KorSlots[i-1],"l") ? "i" :
                                                                                    isEquals(KorSlots[i-1],"n") ? "u" :
                                                                                    isEquals(KorSlots[i-1],"m") ? "u" :
                                                                                    isEquals(KorSlots[i-1],"p") ? "e" :
                                                                                    isEquals(KorSlots[i-1],"h") ? "o" :
                                                                                    isEquals(KorSlots[i-1],"i") ? "ya" :
                                                                                    isEquals(KorSlots[i-1],"u") ? "ya" :
                                                                                    isEquals(KorSlots[i-1],"b") ? "yu" :
                                                                                    isEquals(KorSlots[i-1],"y") ? "yo" :
                                                                                    isEquals(KorSlots[i-1],"hk")? "wa" :
                                                                                    isEquals(KorSlots[i-1],"j") ? "wo" : "wo");
                                                            queueThing = String("") + KorSlots[i] + KorSlots[i-1];

                                                            // Clear KorSlots
                                                            fillWith(KorSlots[i],"");
                                                            fillWith(KorSlots[i-1],"");
                                                        }
                                                        else
                                                        {
                                                            // Hangeul to Kana (nn, cc Kana)
                                                            fillWith(KorSlots[i],   isEquals(KorSlots[i],"s") ? "nn" :
                                                                                    isEquals(KorSlots[i],"d") ? "nn" :
                                                                                    isEquals(KorSlots[i],"a") ? "nn" :
                                                                                    isEquals(KorSlots[i],"t") ? "cc" : "cc");
                                                            queueThing = String("") + KorSlots[i];

                                                            // Clear KorSlots
                                                            fillWith(KorSlots[i],"");
                                                        }
                                                        queueKana.push_back( queueThing );
                                                        lenBeQueued++;
                                                    }

                                                    if(lenBeQueued > 0)
                                                        logBeQueued.push_back( lenBeQueued );
                                                };
    auto KORPAD_undoQueueingKana =              [&]() -> void
                                                {
                                                    if(queueKana.empty())
                                                        return;

                                                    for(int i=0; i<logBeQueued.back(); i++)
                                                        queueKana.pop_back();
                                                    
                                                    logBeQueued.pop_back();
                                                };
    auto KORPAD_clearQueuedKana =               [&]() -> void
                                                {
                                                    queueKana.clear();
                                                    logBeQueued.clear();
                                                };

    auto KORPAD_surveillanceKorSlotsForKana =   [&]() -> void
                                                {
                                                    if(KORPAD_MODE != MODE_KANA)
                                                        return;


                                                    if(isKorSlotUpdated)
                                                    {
                                                        if(isExistConsonant(KorSlot0))
                                                        {
                                                            if(!isExistVowel(KorSlot1) && !KORPAD_isHeldDualConsonant(KorSlot1,KorSlot0))
                                                            {
                                                                KORPAD_doQueueingKana(0);
                                                            }
                                                        }
                                                        else if(isExistVowel(KorSlot0))
                                                        {
                                                            if(isExistConsonant(KorSlot1) && isExistVowel(KorSlot2))
                                                            {
                                                                KORPAD_doQueueingKana(1);
                                                            }
                                                        }
                                                    }
                                                    if(isKorSlotUnderflowed)
                                                    {
                                                        KORPAD_undoQueueingKana();
                                                    }
                                                    if(isKorSlotReset)
                                                    {
                                                        KORPAD_clearQueuedKana();
                                                    }
                                                };

    auto KORPAD_tryToTypeKanaByHangeulPronounce =   [&]() -> bool
                                                    {
                                                        if(KORPAD_MODE != MODE_KANA)
                                                            return false;


                                                        COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY;


                                                        KORPAD_doQueueingKana(-1);
                                                        if(logBeQueued.size()==0)
                                                            return false;


                                                        // Clear Outputs
                                                        for(uint8_t i=0; i<logBeQueued.size(); i++){
                                                            KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); delay(11);
                                                        }

                                                        // Change to GJI IME
                                                        KBD_HIJACKER.pressandreleaseMultiKey( {KEY_ALT,KEY_SHIFT,KEY_2} ); delay(11);

                                                        // ReType Outputs
                                                        for(String strKana : queueKana){
                                                            KBD_HIJACKER.pressandreleaseKeys(strKana);
                                                            if( strcmp(strKana.c_str(),"cc")==0 ) KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                                                        }

                                                        // Open GJI IME's Kana Selector
                                                        KBD_HIJACKER.pressandreleaseKey(KEY_TAB); delay(11);

                                                        // Now... Select Kana
                                                        int32_t keyJustLatestPressed;
                                                        while(numDN)
                                                        {
                                                            delay(1);
                                                            if((keyJustLatestPressed=KBD_PARSER.KeyLogger.peek_key(0)) == KEYPAD_ENTER)
                                                                continue;

                                                            switch(keyJustLatestPressed)
                                                            {
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
                                                            }
                                                            KBD_PARSER.KeyLogger.pop();
                                                        }

                                                        // Close GJI IME's Kana Selector
                                                        KBD_HIJACKER.pressandreleaseKey(KEY_ENTER); delay(11);

                                                        // Change Back Kor IME
                                                        KBD_HIJACKER.pressandreleaseMultiKey( {KEY_ALT,KEY_SHIFT,KEY_1} );

                                                        // Serial.println("###################");
                                                        // Serial.println("###################");
                                                        // Serial.println("HANGEUL LEN   : " + String(logBeQueued.size()));
                                                        // for(uint8_t num : logBeQueued){
                                                        //     Serial.print("_");
                                                        //     Serial.print(num);
                                                        // } Serial.println("");
                                                        // Serial.println("NOW QUEUE HAS : " + String(queueKana.size()));
                                                        // for(String str : queueKana){
                                                        //     Serial.print("_");
                                                        //     Serial.print(str);
                                                        // } Serial.println("");

                                                        KORPAD_clearQueuedKana();

                                                        return true;
                                                    };










    if (key == KEY_NUM_LOCK)
    {
        if(KoreanKeypadToggle)
        {
            //Turn Off KoreanKeypadToggle
            KoreanKeypadToggle = false;
            
            Buzzzzer::reserveBuzz   ( { NOTE_D3 }
                                    , { 181     } );
        }
        else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400)
        {
            KBD_HIJACKER.releaseAllBeingHoldDownKey(); // KEY_NUM_LOCK release needed


            // Turn On  KoreanKeypadToggle
            KoreanKeypadToggle = true;

            // Reset ConvertMode
            KORPAD_MODE = MODE_HANGEUL;

            KORPAD_resetConfirmedInputs();

            if(!KBD_HIJACKER.getStateNumLockToggle()) // While KoreanKeypadToggle is on, Numlock is always on
                KBD_HIJACKER.pressandreleaseKey(KEY_NUM_LOCK);

            Buzzzzer::reserveBuzz   ( { NOTE_G5 }
                                    , { 333     } );


            // Reverse KorEng state IMMEDIATELY, if NumLock pressed more than 1200 millis
            if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 1000)
                KBD_HIJACKER.pressandreleaseKey(KEY_KORENG);
            // Otherwise,
            // In order to reverse KorEng state, Dependent on KorEngStatus.exe...
            // If your system supports KorEngStatus.exe (Maker's Blog:https://blog.naver.com/breeze4me/140056743544)
            else
            {
                KBD_HIJACKER.pressandreleaseKey(KEY_SCROLL_LOCK); // For, Update ScrollLock state with THE LATEST
                delay(77); KBD_HIJACKER.syncToggleKeyStates();

                if(!KBD_HIJACKER.getStateScrollLockToggle())
                {
                    bool stateBeforeReverse = KBD_HIJACKER.getStateScrollLockToggle();

                    KBD_HIJACKER.pressandreleaseKey(KEY_SCROLL_LOCK); // For, Monitor the changes in the ScrollLock state
                    delay(77); KBD_HIJACKER.syncToggleKeyStates();

                    bool stateAfterReverse  = KBD_HIJACKER.getStateScrollLockToggle();

                    // Switch KorEng state, If the KorEngStatus.exe's feature that reflects the KORENG state on the ScrollLock state is running
                    if(stateBeforeReverse == stateAfterReverse)
                        KBD_HIJACKER.pressandreleaseKey(KEY_KORENG);
                }
                // rollback ScrollLock state
                else
                    KBD_HIJACKER.pressandreleaseKey(KEY_SCROLL_LOCK); // For, Rollback ScrollLock's state
            }


            KBD_HIJACKER.reserveSyncToggleKeyStates();
            isActivatedKeyEvent=false; key=0;
        }

        return;
    }



    if(!KoreanKeypadToggle)
        return;



    switch(key)
    {
        case KEYPAD_SLASH:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_HIJACKER.pressandreleaseKey(KEY_F2); delay(11);
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_C} );
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_F} );
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_ASTERIX:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_HIJACKER.pressandreleaseKey(KEY_F2); delay(11);
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_V} );
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KBD_HIJACKER.pressandreleaseMultiKey( {KEY_CTRL,KEY_H} );
            }
            isActivatedKeyEvent=false; key=0;
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
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_MINUS:
        {
            if(event){
                KORPAD_undoConfirmedInputs();
            }
            else if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400){
                KORPAD_resetConfirmedInputs();
            }
            key = KEY_BACKSPACE;
        }
        break;

        case KEYPAD_PLUS:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
                fillWith(KorSlot0," ");
            }
            key = KEY_SPACE;
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

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctAraeAVowelError();
                    Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,":")   ){
                    fillWith(KorSlot0,"u");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctAraeAVowelError();
                    Keyboard.write('u');
                }
                else if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"o");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"i")   ){
                    fillWith(KorSlot0,"O");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('O');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"p");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('p');
                }
                else if
                (   isEquals(KorSlot0,"u")   ){
                    fillWith(KorSlot0,"P");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"b")   ){
                    fillWith(KorSlot0,"nj");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('n'); Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,"nj")   ){
                    fillWith(KorSlot0,"np");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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
                fillWith(KorSlot0,"7");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('7');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_8:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"l")   ){
                    fillWith(KorSlot0,"k");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"i");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('i');
                }
                else if
                (   isEquals(KorSlot0,"m")   ){
                    fillWith(KorSlot0,"n");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('n');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"b");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('b');
                }
                else if
                (   isEquals(KorSlot0,"hl")   ){
                    fillWith(KorSlot0,"hk");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"*")   ){
                    fillWith(KorSlot0,":");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write(':');
                }
                else if
                (   isEquals(KorSlot0,":")   ){
                    fillWith(KorSlot0,"*");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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
                fillWith(KorSlot0,"8");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('8');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_9:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"*")   ){
                    fillWith(KorSlot0,"h");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctAraeAVowelError();
                    Keyboard.write('h');
                }
                else if
                (   isEquals(KorSlot0,":")   ){
                    fillWith(KorSlot0,"y");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctAraeAVowelError();
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
                fillWith(KorSlot0,"9");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('9');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_4:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"r")   ){
                    fillWith(KorSlot0,"z");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('z');
                }
                else if
                (   isEquals(KorSlot0,"z")   ){
                    fillWith(KorSlot0,"R");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('R');
                }
                else if
                (   isEquals(KorSlot0,"R")   ){
                    fillWith(KorSlot0,"r");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
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
                fillWith(KorSlot0,"4");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('4');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_5:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"s")   ){
                    fillWith(KorSlot0,"f");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('f');
                }
                else if
                (   isEquals(KorSlot0,"f")   ){
                    fillWith(KorSlot0,"s");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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
                fillWith(KorSlot0,"5");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('5');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_6:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"e")   ){
                    fillWith(KorSlot0,"x");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('x');
                }
                else if
                (   isEquals(KorSlot0,"x")   ){
                    fillWith(KorSlot0,"E");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('E');
                }
                else if
                (   isEquals(KorSlot0,"E")   ){
                    fillWith(KorSlot0,"e");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
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
                fillWith(KorSlot0,"6");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('6');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_1:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"q")   ){
                    fillWith(KorSlot0,"v");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('v');
                }
                else if
                (   isEquals(KorSlot0,"v")   ){
                    fillWith(KorSlot0,"Q");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('Q');
                }
                else if
                (   isEquals(KorSlot0,"Q")   ){
                    fillWith(KorSlot0,"q");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
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
                fillWith(KorSlot0,"1");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('1');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_2:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"t")   ){
                    fillWith(KorSlot0,"g");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('g');
                }
                else if
                (   isEquals(KorSlot0,"g")   ){
                    fillWith(KorSlot0,"T");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('T');
                }
                else if
                (   isEquals(KorSlot0,"T")   ){
                    fillWith(KorSlot0,"t");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
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
                fillWith(KorSlot0,"2");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('2');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_3:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"w")   ){
                    fillWith(KorSlot0,"c");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('c');
                }
                else if
                (   isEquals(KorSlot0,"c")   ){
                    fillWith(KorSlot0,"W");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('W');
                }
                else if
                (   isEquals(KorSlot0,"W")   ){
                    fillWith(KorSlot0,"w");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
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
                fillWith(KorSlot0,"3");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('3');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_0:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"d")   ){
                    fillWith(KorSlot0,"a");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('a');
                }
                else if
                (   isEquals(KorSlot0,"a")   ){
                    fillWith(KorSlot0,"d");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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
                fillWith(KorSlot0,"0");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('0');
            }
            isActivatedKeyEvent=false; key=0;
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
                fillWith(KorSlot0,"7");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('7');
            }
            isActivatedKeyEvent=false; key=0;
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
                fillWith(KorSlot0,"8");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('8');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_9:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"j");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"k");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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
                fillWith(KorSlot0,"9");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('9');
            }
            isActivatedKeyEvent=false; key=0;
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
                fillWith(KorSlot0,"4");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('4');
            }
            isActivatedKeyEvent=false; key=0;
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
                fillWith(KorSlot0,"5");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('5');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_6:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"h")   ){
                    fillWith(KorSlot0,"n");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('n');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"h");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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
                fillWith(KorSlot0,"6");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('6');
            }
            isActivatedKeyEvent=false; key=0;
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
                fillWith(KorSlot0,"1");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('1');
            }
            isActivatedKeyEvent=false; key=0;
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
                fillWith(KorSlot0,"2");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('2');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_3:
        {
            if(event){
                if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"o");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"i")   ){
                    fillWith(KorSlot0,"O");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('O');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"p");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('p');
                }
                else if
                (   isEquals(KorSlot0,"u")   ){
                    fillWith(KorSlot0,"P");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('o');
                }
                else if
                (   isEquals(KorSlot0,"nj")   ){
                    fillWith(KorSlot0,"np");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
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
                fillWith(KorSlot0,"3");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('3');
            }
            isActivatedKeyEvent=false; key=0;
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
                fillWith(KorSlot0,"0");

                KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write('0');
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        #endif
        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT2 NaRatGeul ------------------------------------------------ */





        case KEY_TAB:
        {
            if(event){
                #if (KEYPAD_KOREAN_LAYOUT == 2) //NaRatGeul

                if
                (   isEquals(KorSlot0,"r")   ){
                    fillWith(KorSlot0,"z");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('z');
                }
                else if
                (   isEquals(KorSlot0,"z")   ){
                    fillWith(KorSlot0,"r");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('r');
                }
                else if
                (   isEquals(KorSlot0,"s")   ){
                    fillWith(KorSlot0,"e");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('e');
                }
                else if
                (   isEquals(KorSlot0,"e")   ){
                    fillWith(KorSlot0,"x");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('x');
                }
                else if
                (   isEquals(KorSlot0,"x")   ){
                    fillWith(KorSlot0,"s");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('s');
                }
                else if
                (   isEquals(KorSlot0,"a")   ){
                    fillWith(KorSlot0,"q");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('q');
                }
                else if
                (   isEquals(KorSlot0,"q")   ){
                    fillWith(KorSlot0,"v");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('v');
                }
                else if
                (   isEquals(KorSlot0,"v")   ){
                    fillWith(KorSlot0,"a");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('a');
                }
                else if
                (   isEquals(KorSlot0,"t")   ){
                    fillWith(KorSlot0,"w");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('w');
                }
                else if
                (   isEquals(KorSlot0,"w")   ){
                    fillWith(KorSlot0,"c");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('c');
                }
                else if
                (   isEquals(KorSlot0,"c")   ){
                    fillWith(KorSlot0,"t");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('t');
                }
                else if
                (   isEquals(KorSlot0,"d")   ){
                    fillWith(KorSlot0,"g");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('g');
                }
                else if
                (   isEquals(KorSlot0,"g")   ){
                    fillWith(KorSlot0,"d");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('d');
                }
                else if
                (   isEquals(KorSlot0,"k")   ){
                    fillWith(KorSlot0,"i");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('i');
                }
                else if
                (   isEquals(KorSlot0,"i")   ){
                    fillWith(KorSlot0,"k");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('k');
                }
                else if
                (   isEquals(KorSlot0,"j")   ){
                    fillWith(KorSlot0,"u");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('u');
                }
                else if
                (   isEquals(KorSlot0,"u")   ){
                    fillWith(KorSlot0,"j");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('j');
                }
                else if
                (   isEquals(KorSlot0,"h")   ){
                    fillWith(KorSlot0,"y");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('y');
                }
                else if
                (   isEquals(KorSlot0,"y")   ){
                    fillWith(KorSlot0,"h");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('h');
                }
                else if
                (   isEquals(KorSlot0,"n")   ){
                    fillWith(KorSlot0,"b");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('b');
                }
                else if
                (   isEquals(KorSlot0,"b")   ){
                    fillWith(KorSlot0,"n");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('n');
                }

                #endif                          //NaRatGeul End
            }
            else 
            if(isEquals(KorSlot0,"") || isEquals(KorSlot0," "))
            {
                if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400)
                {
                    if(KORPAD_MODE != MODE_KANA)
                    {
                        KORPAD_resetConfirmedInputs();
                        
                        //Turn On MODE_KANA
                        KORPAD_MODE = MODE_KANA;
        
                        Buzzzzer::reserveBuzz   ( { NOTE_E7,    NOTE_E7,0,  NOTE_E7,0,  NOTE_C7,    NOTE_E7,0,  NOTE_G7,0   }
                                                , { 200,        130,50,     100,50,     100,        130,50,     300,444,    } );
                    }
                    else
                    {
                        KORPAD_resetConfirmedInputs();
                        
                        //Back To MODE_HANGEUL
                        KORPAD_MODE = MODE_HANGEUL;
                                                
                        Buzzzzer::reserveBuzz   ( { NOTE_G6 }
                                                , { 222     } );
                    }

                    //If CAPSLOCK was On, Turn Off BY FORCE
                    if(KBD_HIJACKER.getStateCapsLockToggle())
                        KBD_HIJACKER.pressandreleaseKey(KEY_CAPS_LOCK);
                    KBD_HIJACKER.reserveSyncToggleKeyStates();
                }
                else
                {
                    KBD_HIJACKER.pressandreleaseKey(KEY_TAB);

                    KORPAD_resetConfirmedInputs();
                }
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_PERIOD:
        {
            if(event){
                #if (KEYPAD_KOREAN_LAYOUT == 1) //CheonJiIn

                if
                (   isEquals(KorSlot0,"") || isEquals(KorSlot0," ")   ){
                    fillWith(KorSlot0,"\\ ");
                }
                else 

                #endif                          //CheonJiIn End

                #if (KEYPAD_KOREAN_LAYOUT == 2) //NaRatGeul

                if
                (   isEquals(KorSlot0,"r")   ){
                    fillWith(KorSlot0,"R");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('R');
                }
                else if
                (   isEquals(KorSlot0,"R")   ){
                    fillWith(KorSlot0,"r");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('r');
                }
                else if
                (   isEquals(KorSlot0,"e")   ){
                    fillWith(KorSlot0,"E");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('E');
                }
                else if
                (   isEquals(KorSlot0,"E")   ){
                    fillWith(KorSlot0,"e");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
                    Keyboard.write('e');
                }
                else if
                (   isEquals(KorSlot0,"q")   ){
                    fillWith(KorSlot0,"Q");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('Q');
                }
                else if
                (   isEquals(KorSlot0,"Q")   ){
                    fillWith(KorSlot0,"q");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
                                                                    KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('q');
                }
                else if
                (   isEquals(KorSlot0,"t")   ){
                    fillWith(KorSlot0,"T");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('T');
                }
                else if
                (   isEquals(KorSlot0,"T")   ){
                    fillWith(KorSlot0,"t");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('t');
                }
                else if
                (   isEquals(KorSlot0,"w")   ){
                    fillWith(KorSlot0,"W");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('W');
                }
                else if
                (   isEquals(KorSlot0,"W")   ){
                    fillWith(KorSlot0,"w");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
                                                                    KORPAD_correctBottomDualConsonantError();
                    Keyboard.write('w');

                // Special Characters
                }
                else if
                (   isEquals(KorSlot0,"") || isEquals(KorSlot0," ")   ){

                    /* NOTHING */

                }
                else if
                (   isEquals(KorSlot0,";")   ){
                    fillWith(KorSlot0,"()");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('('); Keyboard.write(')');
                }
                else if
                (   isEquals(KorSlot0,"()")   ){
                    fillWith(KorSlot0,"{}");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('{'); Keyboard.write('}');
                }
                else if
                (   isEquals(KorSlot0,"{}")   ){
                    fillWith(KorSlot0,"?");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('?');
                }
                else if
                (   isEquals(KorSlot0,"?")   ){
                    fillWith(KorSlot0,"!");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('!');
                }
                else if
                (   isEquals(KorSlot0,"!")   ){
                    fillWith(KorSlot0,".");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('.');
                }
                else if
                (   isEquals(KorSlot0,".")   ){
                    fillWith(KorSlot0,"~");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('~');
                }
                else if
                (   isEquals(KorSlot0,"~")   ){
                    fillWith(KorSlot0,"^^");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write('^'); Keyboard.write('^');
                }
                else if
                (   isEquals(KorSlot0,"^^")   ){
                    fillWith(KorSlot0,"\\ ");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE); KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write(' ');
                }
                else if
                (   isEquals(KorSlot0,"\\ ")   ){
                    fillWith(KorSlot0,";");

                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write(';');
                }
                else 

                #endif                          //NaRatGeul End

                // MODE_ALPHABET
                if
                (   KORPAD_tryToDoAlphabetCaseConversion()   )
                {
                    /* Already done Alphabet Case Conversion */
                }

                else
                {
                    COMPLETE_THE_IN_PROGRESS_HANGEUL_IMMEDIATELY;

                    KORPAD_updateConfirmedInputs();
                    fillWith(KorSlot0,"");
                }
            }
            else 
            if(isEquals(KorSlot0,"") || isEquals(KorSlot0," ") || isEquals(KorSlot0,"\\ "))
            {
                if(MILLIS_FROM_PRESSED_UNTIL_RELEASE > 400)
                {
                    if(KORPAD_MODE != MODE_ALPHABET)
                    {
                        KORPAD_resetConfirmedInputs();
                        
                        //Turn On MODE_ALPHABET
                        KORPAD_MODE = MODE_ALPHABET;
        
                        Buzzzzer::reserveBuzz   ( { NOTE_FS5,0, NOTE_GS5,0, NOTE_AS5,0, NOTE_DS6,0, NOTE_CS6    }
                                                , { 200,30,     100,150,    120,200,    150,150,    450         } );
                    }
                    else
                    {
                        KORPAD_resetConfirmedInputs();
                        
                        //Back To MODE_HANGEUL
                        KORPAD_MODE = MODE_HANGEUL;
                                                
                        Buzzzzer::reserveBuzz   ( { NOTE_AS5,0, NOTE_GS5,0, NOTE_FS5,0, NOTE_FS5    }
                                                , { 300,50,     230,50,     170,100,    350         } );
                    }

                    //If CAPSLOCK was On, Turn Off BY FORCE
                    if(KBD_HIJACKER.getStateCapsLockToggle())
                        KBD_HIJACKER.pressandreleaseKey(KEY_CAPS_LOCK);
                    KBD_HIJACKER.reserveSyncToggleKeyStates();
                }
                else
                {
                    #if (KEYPAD_KOREAN_LAYOUT == 1) //CheonJiIn

                    if(isEquals(KorSlot0,"\\ ")){
                        KORPAD_resetConfirmedInputs();
                        KBD_HIJACKER.pressandreleaseKey(KEY_SPACE);
                    }

                    #endif                          //CheonJiIn End

                    #if (KEYPAD_KOREAN_LAYOUT == 2) //NaRatGeul

                    if(isEquals(KorSlot0," ")){
                        KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write(';');
                        fillWith(KorSlot0,";");
                    }

                    #endif                          //NaRatGeul End
                }
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_ENTER:
        {
            if(event){
                // both MODES, [ " " + ENTER ] means [ SHIFT + ENTER ]
                if(isEquals(KorSlot0," "))
                {
                    KBD_HIJACKER.pressandreleaseKey(KEY_BACKSPACE);
                    KBD_HIJACKER.pressandreleaseMultiKey( {KEY_SHIFT,KEY_ENTER} );
                    KORPAD_resetConfirmedInputs();
                }

                // MODE_ALPHABET
                else if(KORPAD_tryToTypeAlphabetByHangeulPronounce())
                {
                    /* Already typed Alphabet By Hangeul Pronounce */
                }

                // MODE_KANA
                else if(KORPAD_tryToTypeKanaByHangeulPronounce())
                {
                    /* Already typed Kana By Hangeul Pronounce */
                }

                // MODE_HANGEUL
                else
                {
                    KBD_HIJACKER.pressandreleaseKey(KEY_ENTER);
                    KORPAD_resetConfirmedInputs();
                }
            }
            isActivatedKeyEvent=false; key=0;
        }
        break;
    }



    KORPAD_surveillanceKorSlotsForKana();

#endif
/* ------------------------------------------------------ KEYPAD_KOREAN_LAYOUT COMMON ------------------------------------------------------ */
}
