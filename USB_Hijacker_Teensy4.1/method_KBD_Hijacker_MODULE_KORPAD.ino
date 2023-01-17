
#define KEYPAD_KOREAN_LAYOUT 2      // 0:None   1:CheonJiIn   2:NaRatGle





#if (KEYPAD_KOREAN_LAYOUT > 0)

#define KORPAD_isExistVowel(KorSlotX)           strcmp(KorSlotX,"k")==0||strcmp(KorSlotX,"o")==0||strcmp(KorSlotX,"i")==0||strcmp(KorSlotX,"O")==0||strcmp(KorSlotX,"j")==0||strcmp(KorSlotX,"p")==0||strcmp(KorSlotX,"u")==0||strcmp(KorSlotX,"P")==0||strcmp(KorSlotX,"h")==0||strcmp(KorSlotX,"y")==0||strcmp(KorSlotX,"n")==0||strcmp(KorSlotX,"b")==0||strcmp(KorSlotX,"m")==0||strcmp(KorSlotX,"l")==0||strcmp(KorSlotX,"hk")==0||strcmp(KorSlotX,"ho")==0||strcmp(KorSlotX,"hl")==0||strcmp(KorSlotX,"nj")==0||strcmp(KorSlotX,"np")==0||strcmp(KorSlotX,"nl")==0||strcmp(KorSlotX,"ml")==0
#define KORPAD_isExistConsonant(KorSlotX)       strcmp(KorSlotX,"r")==0||strcmp(KorSlotX,"z")==0||strcmp(KorSlotX,"R")==0||strcmp(KorSlotX,"s")==0||strcmp(KorSlotX,"f")==0||strcmp(KorSlotX,"e")==0||strcmp(KorSlotX,"x")==0||strcmp(KorSlotX,"E")==0||strcmp(KorSlotX,"q")==0||strcmp(KorSlotX,"b")==0||strcmp(KorSlotX,"Q")==0||strcmp(KorSlotX,"t")==0||strcmp(KorSlotX,"g")==0||strcmp(KorSlotX,"T")==0||strcmp(KorSlotX,"w" )==0||strcmp(KorSlotX,"c")==0||strcmp(KorSlotX,"W")==0||strcmp(KorSlotX,"d")==0||strcmp(KorSlotX,"a")==0
#define KORPAD_updateConfirmedInputs()                  updateConfirmedInputs(KorSlot0,KorSlot1,KorSlot2,KorSlot3,KorSlot4);
#define KORPAD_resetConfirmedInputs()                   resetConfirmedInputs (KorSlot0,KorSlot1,KorSlot2,KorSlot3,KorSlot4);
#define KORPAD_undoConfirmedInputs()                    undoConfirmedInputs  (KorSlot0,KorSlot1,KorSlot2,KorSlot3,KorSlot4);
#define KORPAD_correctBottomDualConsonantError()        correctBottomDualConsonantError  (KorSlot1,KorSlot2,KorSlot3);
#define KORPAD_correctBottomSingleConsonantError()      correctBottomSingleConsonantError(KorSlot1,KorSlot2);
void updateConfirmedInputs(char* KorSlot0,char* KorSlot1,char* KorSlot2,char* KorSlot3,char* KorSlot4)
{
    strcpy(KorSlot4,KorSlot3);
    strcpy(KorSlot3,KorSlot2);
    strcpy(KorSlot2,KorSlot1);
    strcpy(KorSlot1,KorSlot0);
}
void resetConfirmedInputs(char* KorSlot0,char* KorSlot1,char* KorSlot2,char* KorSlot3,char* KorSlot4)
{
    strcpy(KorSlot4,"");
    strcpy(KorSlot3,"");
    strcpy(KorSlot2,"");
    strcpy(KorSlot1,"");
    strcpy(KorSlot0,"");
}
void undoConfirmedInputs(char* KorSlot0,char* KorSlot1,char* KorSlot2,char* KorSlot3,char* KorSlot4)
{
    strcpy(KorSlot0,KorSlot1);
    strcpy(KorSlot1,KorSlot2);
    strcpy(KorSlot2,KorSlot3);
    strcpy(KorSlot3,KorSlot4);
}
void correctBottomDualConsonantError(char* KorSlot1,char* KorSlot2,char* KorSlot3)
{
    if(KORPAD_isExistVowel(KorSlot2))
    {
        if(KORPAD_isExistConsonant(KorSlot3))
        {
            delay(11); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
            delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot3);
            delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
            delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
            delay(11);
            return;
        }
    }
}
void correctBottomSingleConsonantError(char* KorSlot1,char* KorSlot2)
{
    if(KORPAD_isExistVowel(KorSlot1))
    {
        if(KORPAD_isExistConsonant(KorSlot2))
        {
            delay(11); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
            delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
            delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
            delay(11); 
            return;
        }
    }
}
#define KORPAD_writeAlphabet(CH)                delay(11); KBD_Hijacker.pressandreleaseKey(KEY_KORENG); delay(11); Keyboard.write(CH); delay(11); KBD_Hijacker.pressandreleaseKey(KEY_KORENG); delay(11);
#define KORPAD_switchCapsLockToggleIfBattered3()        if(KeyLogger.peek_key(1)==KEYPAD_PERIOD){ if(KeyLogger.peek_key(2)==KEYPAD_PERIOD) KBD_Hijacker.pressandreleaseKey(KEY_CAPS_LOCK); }
#define KORPAD_switchCapsLockToggleIfBattered4()        if(KeyLogger.peek_key(1)==KEYPAD_PERIOD){ if(KeyLogger.peek_key(2)==KEYPAD_PERIOD){ if(KeyLogger.peek_key(3)==KEYPAD_PERIOD) KBD_Hijacker.pressandreleaseKey(KEY_CAPS_LOCK); } }
#define KORPAD_isAlphabetPronounce(STR3,STR2,STR1,STR0)         ((strcmp(KorSlot0,STR0)==0||strcmp("",STR0)==0)&&(strcmp(KorSlot1,STR1)==0||strcmp("",STR1)==0)&&(strcmp(KorSlot2,STR2)==0||strcmp("",STR2)==0)&&(strcmp(KorSlot3,STR3)==0||strcmp("",STR3)==0))

#endif





#if (KEYPAD_KOREAN_LAYOUT == 1)     // LAYOUT1:CheonJiIn Only

#define KORPAD_correctAraeAVowelError()             correctAraeAVowelError(KorSlot1,KorSlot2,KorSlot3,KorSlot4);
void correctAraeAVowelError(char* KorSlot1,char* KorSlot2,char* KorSlot3,char* KorSlot4)
{
    if(KORPAD_isExistConsonant(KorSlot1))
    {
        if(KORPAD_isExistVowel(KorSlot2))
        {
            if(KORPAD_isExistConsonant(KorSlot3))
            {
                delay(11); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot3);
                delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
                delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
                delay(11);
                return;
            }
        }
        else if(KORPAD_isExistConsonant(KorSlot2))
        {
            if((strcmp(KorSlot2,"r")==0&&strcmp(KorSlot1,"t")==0)||(strcmp(KorSlot2,"s")==0&&strcmp(KorSlot1,"w")==0)||(strcmp(KorSlot2,"s")==0&&strcmp(KorSlot1,"g")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"r")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"a")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"q")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"t")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"x")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"v")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"g")==0)||(strcmp(KorSlot2,"q")==0&&strcmp(KorSlot1,"t")==0))
            {
                if(KORPAD_isExistVowel(KorSlot3))
                {
                    if(KORPAD_isExistConsonant(KorSlot4))
                    {
                        delay(11); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot4);
                        delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot3);
                        delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
                        delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
                        delay(11);
                        return;
                    }
                }
            }
        }
        delay(11); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
        delay(11); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
        return;
    }
}

#endif





void MODULE_KOREAN_KEYPAD_EVOLUTION()
{
    /* ------------------------------------------------------ KEYPAD_KOREAN_LAYOUT COMMON ------------------------------------------------------ */
    #if (KEYPAD_KOREAN_LAYOUT > 0)

    static bool KoreanKeypadToggle = false;
    static char KorSlot0[3], KorSlot1[3], KorSlot2[3], KorSlot3[3], KorSlot4[3];
    
    static bool ConvertMode_Alphabet = false;

    if (key == KEY_NUM_LOCK)
    {
        if(KoreanKeypadToggle){
            //Turn Off KoreanKeypadToggle
            KoreanKeypadToggle = false;
            ConvertMode_Alphabet = false;
            
            Buzzzzer.reserveBuzz(   new uint16_t[1] {NOTE_D3},
                                    new uint16_t[1] {181},
                                    1   );
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KBD_Hijacker.releaseAllBeingHoldDownKey();   // KEY_NUM_LOCK release needed

            //Turn On  KoreanKeypadToggle
            KoreanKeypadToggle = true;
            
            Buzzzzer.reserveBuzz(   new uint16_t[1] {NOTE_G5},
                                    new uint16_t[1] {333},
                                    1   );
            
            //While KoreanKeypadToggle is on, Numlock is always on
            if(!KBD_Hijacker.getStateNumLockToggle()){
                KBD_Hijacker.pressandreleaseKey(KEY_NUM_LOCK);
            }

            //Reverse KorEng status, when ScrollLock off
            //Support by KorEngStatus.exe (Maker's Blog:https://blog.naver.com/breeze4me/140056743544)
            if(!KBD_Hijacker.getStateScrollLockToggle()){
                bool stateBeforeReverse = KBD_Hijacker.getStateScrollLockToggle();
                
                KBD_Hijacker.pressandreleaseKey(KEY_KORENG);
                KBD_Hijacker.reserveSyncTKS=true; delay(80);

                //Rollback KorEng status, if system is not using KorEngStatus.exe
                if(stateBeforeReverse == KBD_Hijacker.getStateScrollLockToggle()){
                    KBD_Hijacker.pressandreleaseKey(KEY_KORENG);
                }
            }
            
            //Reverse KorEng status, NumLock pressed more than 1200 millis, no matter the ScrollLock on/off
            if(PRESSED_TIME_UNTIL_RELEASE > 1200){
                KBD_Hijacker.pressandreleaseKey(KEY_KORENG);
            }
            //syncToggleKeyStates() in a few ms
            KBD_Hijacker.reserveSyncTKS=true; delay(80);
            KORPAD_resetConfirmedInputs();

            isActivateKeyEvent=false; key=0;
        }
    }





    if(!KoreanKeypadToggle)
        return;


    switch(key)
    {
        case KEYPAD_SLASH:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_Hijacker.pressandreleaseKey(KEY_F2);
                delay(10);
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_CTRL,KEY_C} );
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_CTRL,KEY_F} );
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_ASTERIX:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_Hijacker.pressandreleaseKey(KEY_F2);
                delay(10);
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_CTRL,KEY_V} );
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_CTRL,KEY_H} );
            }
            isActivateKeyEvent=false; key=0;
        }

        case KEY_BACKSPACE:
        {
            if(event){
                KORPAD_resetConfirmedInputs();
    
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_SHIFT,KEY_LEFT_ARROW} );
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 1600){
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_CTRL,KEY_A} );
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_CTRL,KEY_Z} );
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_SUBTRACT:
        {
            if(event){
                KORPAD_undoConfirmedInputs();
            }
            key = KEY_BACKSPACE;
        }
        break;

        case KEYPAD_ADD:
        {
            if(event){
                KORPAD_resetConfirmedInputs(); strcpy(KorSlot0," ");
            }
            key = KEY_SPACE;
        }
        break;
        
        case KEYPAD_ENTER:
        {
            if(event){
                if(!ConvertMode_Alphabet){  //ConvertMode_Alphabet is OFF
                    if(strcmp(KorSlot0," ")==0){
                        KBD_Hijacker.pressandreleaseShortcutKey( 2 , new int32_t[2] {KEY_SHIFT,KEY_ENTER} );
                    }
                    else{
                        KBD_Hijacker.pressandreleaseKey(KEY_ENTER);
                    }
                    KORPAD_resetConfirmedInputs();
                }
                else{                       //ConvertMode_Alphabet is ON
                    if       (strcmp(KorSlot0," ")==0){
                        //Turn Off ConvertMode_Alphabet
                        ConvertMode_Alphabet = false;
                        
                        Buzzzzer.reserveBuzz(   new uint16_t[7] {   NOTE_AS5,0, NOTE_GS5,0, NOTE_FS5,0, NOTE_FS5    },
                                                new uint16_t[7] {   300,50,     230,50,     170,100,    350         },
                                                7   );
                        
                        KORPAD_resetConfirmedInputs();
                        
                    } else if(KORPAD_isAlphabetPronounce("d","o","d","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('a');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*a");
                    } else if(KORPAD_isAlphabetPronounce("d","p","d","l")){                 //convert Hangle's alphabet pronounce to Alphabet
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('a');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*a");
                    } else if(KORPAD_isAlphabetPronounce("d","p","v","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('f');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*f");
                    } else if(KORPAD_isAlphabetPronounce("d","l","c","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('h');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*h");
                    } else if(KORPAD_isAlphabetPronounce("d","p","c","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('h');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*h");
                    } else if(KORPAD_isAlphabetPronounce("d","k","d","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('i');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*i");
                    } else if(KORPAD_isAlphabetPronounce("w","p","d","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('j');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*j");
                    } else if(KORPAD_isAlphabetPronounce("z","p","d","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('k');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*k");
                    } else if(KORPAD_isAlphabetPronounce("d","h","d","n")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('o');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*o");
                    } else if(KORPAD_isAlphabetPronounce("d","p","t","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('s');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*s");
                    } else if(KORPAD_isAlphabetPronounce("q","m","d","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('v');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*v");
                    } else if(KORPAD_isAlphabetPronounce("m","f","d","b")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('w');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*w");
                    } else if(KORPAD_isAlphabetPronounce("m","f","f","b")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('w');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*w");
                    } else if(KORPAD_isAlphabetPronounce("e","j","q","b")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('w');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*w");
                    } else if(KORPAD_isAlphabetPronounce("E","j","q","b")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('w');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*w");
                    } else if(KORPAD_isAlphabetPronounce("p","r","t","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*x");
                    } else if(KORPAD_isAlphabetPronounce("z","m","t","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*x");
                    } else if(KORPAD_isAlphabetPronounce("d","hk","d","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('y');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*y");
                    } else if(KORPAD_isAlphabetPronounce("w","p","x","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('z');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*z");
                    } else if(KORPAD_isAlphabetPronounce("W","p","x","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('z');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*z");
                        
                    } else if(KORPAD_isAlphabetPronounce("","","d","k")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('a');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*a");
                    } else if(KORPAD_isAlphabetPronounce("","","d","o")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('a');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*a");
                    } else if(KORPAD_isAlphabetPronounce("","","q","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('b');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*b");
                    } else if(KORPAD_isAlphabetPronounce("","","Q","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('b');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*b");
                    } else if(KORPAD_isAlphabetPronounce("","","t","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('c');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*c");
                    } else if(KORPAD_isAlphabetPronounce("","","T","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('c');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*c");
                    } else if(KORPAD_isAlphabetPronounce("","","z","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('c');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*c");
                    } else if(KORPAD_isAlphabetPronounce("","","e","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('d');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*d");
                    } else if(KORPAD_isAlphabetPronounce("","","e","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('d');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*d");
                    } else if(KORPAD_isAlphabetPronounce("","","d","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('e');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*e");
                    } else if(KORPAD_isAlphabetPronounce("","","d","p")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('e');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*e");
                    } else if(KORPAD_isAlphabetPronounce("","","d","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('e');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*e");
                    } else if(KORPAD_isAlphabetPronounce("","d","p","v")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('f');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*f");
                    } else if(KORPAD_isAlphabetPronounce("","","v","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('f');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*f");
                    } else if(KORPAD_isAlphabetPronounce("","","Q","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('f');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*f");
                    } else if(KORPAD_isAlphabetPronounce("","","w","nl")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('g');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*g");
                    } else if(KORPAD_isAlphabetPronounce("","","w","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('g');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*g");
                    } else if(KORPAD_isAlphabetPronounce("","","g","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('h');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*h");
                    } else if(KORPAD_isAlphabetPronounce("","","g","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('h');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*h");
                    } else if(KORPAD_isAlphabetPronounce("","","w","p")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('j');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*j");
                    } else if(KORPAD_isAlphabetPronounce("","","z","p")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('k');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*k");
                    } else if(KORPAD_isAlphabetPronounce("","","z","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('k');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*k");
                    } else if(KORPAD_isAlphabetPronounce("","d","p","f")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('l');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*l");
                    } else if(KORPAD_isAlphabetPronounce("","","f","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('l');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*l");
                    } else if(KORPAD_isAlphabetPronounce("","d","o","a")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('m');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*m");
                    } else if(KORPAD_isAlphabetPronounce("","d","p","a")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('m');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*m");
                    } else if(KORPAD_isAlphabetPronounce("","","a","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('m');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*m");
                    } else if(KORPAD_isAlphabetPronounce("","d","o","s")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('n');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*n");
                    } else if(KORPAD_isAlphabetPronounce("","d","p","s")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('n');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*n");
                    } else if(KORPAD_isAlphabetPronounce("","","s","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('n');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*n");
                    } else if(KORPAD_isAlphabetPronounce("","","d","h")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('o');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*o");
                    } else if(KORPAD_isAlphabetPronounce("","","v","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('p');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*p");
                    } else if(KORPAD_isAlphabetPronounce("","","z","b")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('q');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*q");
                    } else if(KORPAD_isAlphabetPronounce("","","z","n")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('q');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*q");
                    } else if(KORPAD_isAlphabetPronounce("","d","k","f")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('r');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*r");
                    } else if(KORPAD_isAlphabetPronounce("","d","j","f")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('r');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*r");
                    } else if(KORPAD_isAlphabetPronounce("","","f","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('r');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*r");
                    } else if(KORPAD_isAlphabetPronounce("","","t","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('s');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*s");
                    } else if(KORPAD_isAlphabetPronounce("","","x","l")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('t');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*t");
                    } else if(KORPAD_isAlphabetPronounce("","","x","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('t');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*t");
                    } else if(KORPAD_isAlphabetPronounce("","","d","b")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('u');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*u");
                    } else if(KORPAD_isAlphabetPronounce("","","d","n")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('u');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*u");
                    } else if(KORPAD_isAlphabetPronounce("","","d","j")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('u');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*u");
                    } else if(KORPAD_isAlphabetPronounce("","","q","ml")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('v');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*v");
                    } else if(KORPAD_isAlphabetPronounce("","","q","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('v');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*v");
                    } else if(KORPAD_isAlphabetPronounce("","E","j","q")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('w');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*w");
                    } else if(KORPAD_isAlphabetPronounce("","d","o","r")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*x");
                    } else if(KORPAD_isAlphabetPronounce("","d","p","r")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*x");
                    } else if(KORPAD_isAlphabetPronounce("","","T","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*x");
                    } else if(KORPAD_isAlphabetPronounce("","","d","i")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('y');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*y");
                    } else if(KORPAD_isAlphabetPronounce("","","d","u")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('y');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*y");
                    } else if(KORPAD_isAlphabetPronounce("","","d","y")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('y');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*y");
                    } else if(KORPAD_isAlphabetPronounce("","","d","hk")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('y');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*y");
                    } else if(KORPAD_isAlphabetPronounce("","w","p","t")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('z');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*z");
                    } else if(KORPAD_isAlphabetPronounce("","","w","ml")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('z');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*z");
                    } else if(KORPAD_isAlphabetPronounce("","","w","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('z');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*z");
                    } else if(KORPAD_isAlphabetPronounce("","","W","m")){
                        KBD_Hijacker.pressandreleaseKey(KEY_SPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('z');
                        KORPAD_resetConfirmedInputs();
                        strcpy(KorSlot0,"*z");
                        
                    } else if(strcmp(KorSlot0,"r")==0){                                     //convert Hangle Consonant to Alphabet
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('g');
                        strcpy(KorSlot0,"*g");
                    } else if(strcmp(KorSlot1,"r")==0&&strcmp(KorSlot0,"*g")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('r');
                        strcpy(KorSlot0,"r");
    
                    } else if(strcmp(KorSlot0,"s")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('n');
                        strcpy(KorSlot0,"*n");
                    } else if(strcmp(KorSlot1,"s")==0&&strcmp(KorSlot0,"*n")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('s');
                        strcpy(KorSlot0,"s");
    
                    } else if(strcmp(KorSlot0,"e")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('d');
                        strcpy(KorSlot0,"*d");
                    } else if(strcmp(KorSlot1,"e")==0&&strcmp(KorSlot0,"*d")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('e');
                        strcpy(KorSlot0,"e");
    
                    } else if(strcmp(KorSlot0,"f")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('l');
                        strcpy(KorSlot0,"*l");
                    } else if(strcmp(KorSlot1,"f")==0&&strcmp(KorSlot0,"*l")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('r');
                        strcpy(KorSlot0,"*r");
                    } else if(strcmp(KorSlot1,"f")==0&&strcmp(KorSlot0,"*r")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('f');
                        strcpy(KorSlot0,"f");
    
                    } else if(strcmp(KorSlot0,"a")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('m');
                        strcpy(KorSlot0,"*m");
                    } else if(strcmp(KorSlot1,"a")==0&&strcmp(KorSlot0,"*m")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('a');
                        strcpy(KorSlot0,"a");
    
                    } else if(strcmp(KorSlot0,"q")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('b');
                        strcpy(KorSlot0,"*b");
                    } else if(strcmp(KorSlot1,"q")==0&&strcmp(KorSlot0,"*b")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('v');
                        strcpy(KorSlot0,"*v");
                    } else if(strcmp(KorSlot1,"q")==0&&strcmp(KorSlot0,"*v")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('q');
                        strcpy(KorSlot0,"q");
    
                    } else if(strcmp(KorSlot0,"t")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('s');
                        strcpy(KorSlot0,"*s");
                    } else if(strcmp(KorSlot1,"t")==0&&strcmp(KorSlot0,"*s")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('c');
                        strcpy(KorSlot0,"*c");
                    } else if(strcmp(KorSlot1,"t")==0&&strcmp(KorSlot0,"*c")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        strcpy(KorSlot0,"*x");
                    } else if(strcmp(KorSlot1,"t")==0&&strcmp(KorSlot0,"*x")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('t');
                        strcpy(KorSlot0,"t");
    
                    } else if(strcmp(KorSlot0,"T")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('c');
                        strcpy(KorSlot0,"*c");
                    } else if(strcmp(KorSlot1,"T")==0&&strcmp(KorSlot0,"*c")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        strcpy(KorSlot0,"*x");
                    } else if(strcmp(KorSlot1,"T")==0&&strcmp(KorSlot0,"*x")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('T');
                        strcpy(KorSlot0,"T");
    
                    } else if(strcmp(KorSlot0,"d")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('n'); KORPAD_writeAlphabet('g');
                        strcpy(KorSlot0,"*~");
                    } else if(strcmp(KorSlot1,"d")==0&&strcmp(KorSlot0,"*~")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('d');
                        strcpy(KorSlot0,"d");
    
                    } else if(strcmp(KorSlot0,"w")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('j');
                        strcpy(KorSlot0,"*j");
                    } else if(strcmp(KorSlot1,"w")==0&&strcmp(KorSlot0,"*j")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('g');
                        strcpy(KorSlot0,"*g");
                    } else if(strcmp(KorSlot1,"w")==0&&strcmp(KorSlot0,"*g")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('z');
                        strcpy(KorSlot0,"*z");
                    } else if(strcmp(KorSlot1,"w")==0&&strcmp(KorSlot0,"*z")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('w');
                        strcpy(KorSlot0,"w");
    
                    } else if(strcmp(KorSlot0,"c")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('c'); KORPAD_writeAlphabet('h');
                        strcpy(KorSlot0,"*!");
                    } else if(strcmp(KorSlot1,"c")==0&&strcmp(KorSlot0,"*!")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('c');
                        strcpy(KorSlot0,"c");
    
                    } else if(strcmp(KorSlot0,"z")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('c');
                        strcpy(KorSlot0,"*c");
                    } else if(strcmp(KorSlot1,"z")==0&&strcmp(KorSlot0,"*c")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('k');
                        strcpy(KorSlot0,"*k");
                    } else if(strcmp(KorSlot1,"z")==0&&strcmp(KorSlot0,"*k")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('q');
                        strcpy(KorSlot0,"*q");
                    } else if(strcmp(KorSlot1,"z")==0&&strcmp(KorSlot0,"*q")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('x');
                        strcpy(KorSlot0,"*x");
                    } else if(strcmp(KorSlot1,"z")==0&&strcmp(KorSlot0,"*x")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('z');
                        strcpy(KorSlot0,"z");
    
                    } else if(strcmp(KorSlot0,"x")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('t');
                        strcpy(KorSlot0,"*t");
                    } else if(strcmp(KorSlot1,"x")==0&&strcmp(KorSlot0,"*t")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('x');
                        strcpy(KorSlot0,"x");
    
                    } else if(strcmp(KorSlot0,"v")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('f');
                        strcpy(KorSlot0,"*f");
                    } else if(strcmp(KorSlot1,"v")==0&&strcmp(KorSlot0,"*f")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('p');
                        strcpy(KorSlot0,"*p");
                    } else if(strcmp(KorSlot1,"v")==0&&strcmp(KorSlot0,"*p")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('v');
                        strcpy(KorSlot0,"v");
    
                    } else if(strcmp(KorSlot0,"g")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('h');
                        strcpy(KorSlot0,"*h");
                    } else if(strcmp(KorSlot1,"g")==0&&strcmp(KorSlot0,"*h")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('g');
                        strcpy(KorSlot0,"g");
    
                    } else if(strcmp(KorSlot0,"k")==0||strcmp(KorSlot0,"o")==0){            //convert Hangle Vowel to Alphabet
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('a');
                        strcpy(KorSlot0,"*a");
                    } else if(strcmp(KorSlot1,"k")==0&&strcmp(KorSlot0,"*a")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('k');
                        strcpy(KorSlot0,"k");
                    } else if(strcmp(KorSlot1,"o")==0&&strcmp(KorSlot0,"*a")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('o');
                        strcpy(KorSlot0,"o");
    
                    } else if(strcmp(KorSlot0,"j")==0||strcmp(KorSlot0,"p")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('e');
                        strcpy(KorSlot0,"*e");
                    } else if(strcmp(KorSlot1,"j")==0&&strcmp(KorSlot0,"*e")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('j');
                        strcpy(KorSlot0,"j");
                    } else if(strcmp(KorSlot1,"p")==0&&strcmp(KorSlot0,"*e")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('p');
                        strcpy(KorSlot0,"p");
    
                    } else if(strcmp(KorSlot0,"h")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('o');
                        strcpy(KorSlot0,"*o");
                    } else if(strcmp(KorSlot1,"h")==0&&strcmp(KorSlot0,"*o")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('h');
                        strcpy(KorSlot0,"h");
    
                    } else if(strcmp(KorSlot0,"n")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('u');
                        strcpy(KorSlot0,"*u");
                    } else if(strcmp(KorSlot1,"n")==0&&strcmp(KorSlot0,"*u")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('o'); KORPAD_writeAlphabet('o');
                        strcpy(KorSlot0,"*@");
                    } else if(strcmp(KorSlot1,"n")==0&&strcmp(KorSlot0,"*@")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('n');
                        strcpy(KorSlot0,"n");
    
                    } else if(strcmp(KorSlot0,"u")==0||strcmp(KorSlot0,"i")==0||strcmp(KorSlot0,"y")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('y');
                        strcpy(KorSlot0,"*y");
                    } else if(strcmp(KorSlot1,"u")==0&&strcmp(KorSlot0,"*y")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('u');
                        strcpy(KorSlot0,"u");
                    } else if(strcmp(KorSlot1,"i")==0&&strcmp(KorSlot0,"*y")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('i');
                        strcpy(KorSlot0,"i");
                    } else if(strcmp(KorSlot1,"y")==0&&strcmp(KorSlot0,"*y")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('y');
                        strcpy(KorSlot0,"y");
    
                    } else if(strcmp(KorSlot0,"b")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('w');
                        strcpy(KorSlot0,"*w");
                    } else if(strcmp(KorSlot1,"b")==0&&strcmp(KorSlot0,"*w")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('b');
                        strcpy(KorSlot0,"b");
    
                    } else if(strcmp(KorSlot0,"l")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('i');
                        strcpy(KorSlot0,"*i");
                    } else if(strcmp(KorSlot1,"l")==0&&strcmp(KorSlot0,"*i")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('e');
                        strcpy(KorSlot0,"*e");
                    } else if(strcmp(KorSlot1,"l")==0&&strcmp(KorSlot0,"*e")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('l');
                        strcpy(KorSlot0,"l");
    
                    } else if(strcmp(KorSlot0,"hk")==0){
                        KORPAD_updateConfirmedInputs(); strcpy(KorSlot2,"");
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        KORPAD_writeAlphabet('w');
                        KORPAD_writeAlphabet('a');
                        strcpy(KorSlot0,"*#");
                    } else if(strcmp(KorSlot1,"hk")==0&&strcmp(KorSlot0,"*#")==0){
                        KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        Keyboard.write ('h'); Keyboard.write ('k');
                        strcpy(KorSlot0,"hk");
                    }
                }
            }
            isActivateKeyEvent=false; key=0;
        }
        break;



        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT1 CheonJiIn ------------------------------------------------ */
        #if (KEYPAD_KOREAN_LAYOUT == 1)

        case KEYPAD_4:
        {
            if(event){
                if       (strcmp(KorSlot0,"r")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('z');
                    strcpy(KorSlot0,"z");
                } else if(strcmp(KorSlot0,"z")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('R');
                    strcpy(KorSlot0,"R");
                } else if(strcmp(KorSlot0,"R")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('r');
                    strcpy(KorSlot0,"r");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('r');
                    strcpy(KorSlot0,"r");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('4');
                strcpy(KorSlot0,"4");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_5:
        {
            if(event){
                if       (strcmp(KorSlot0,"s")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('f');
                    strcpy(KorSlot0,"f");
                } else if(strcmp(KorSlot0,"f")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('s');
                    strcpy(KorSlot0,"s");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('s');
                    strcpy(KorSlot0,"s");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('5');
                strcpy(KorSlot0,"5");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_6:
        {
            if(event){
                if       (strcmp(KorSlot0,"e")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('x');
                    strcpy(KorSlot0,"x");
                } else if(strcmp(KorSlot0,"x")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('E');
                    strcpy(KorSlot0,"E");
                } else if(strcmp(KorSlot0,"E")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctBottomSingleConsonantError();
                    Keyboard.write ('e');
                    strcpy(KorSlot0,"e");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('e');
                    strcpy(KorSlot0,"e");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('6');
                strcpy(KorSlot0,"6");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_1:
        {
            if(event){
                if       (strcmp(KorSlot0,"q")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('v');
                    strcpy(KorSlot0,"v");
                } else if(strcmp(KorSlot0,"v")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('Q');
                    strcpy(KorSlot0,"Q");
                } else if(strcmp(KorSlot0,"Q")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctBottomSingleConsonantError();
                                                        if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('q');
                    strcpy(KorSlot0,"q");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('q');
                    strcpy(KorSlot0,"q");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('1');
                strcpy(KorSlot0,"1");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_2:
        {
            if(event){
                if       (strcmp(KorSlot0,"t")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('g');
                    strcpy(KorSlot0,"g");
                } else if(strcmp(KorSlot0,"g")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('T');
                    strcpy(KorSlot0,"T");
                } else if(strcmp(KorSlot0,"T")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"r")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('t');
                    strcpy(KorSlot0,"t");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('t');
                    strcpy(KorSlot0,"t");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('2');
                strcpy(KorSlot0,"2");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_3:
        {
            if(event){
                if       (strcmp(KorSlot0,"w")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('c');
                    strcpy(KorSlot0,"c");
                } else if(strcmp(KorSlot0,"c")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('W');
                    strcpy(KorSlot0,"W");
                } else if(strcmp(KorSlot0,"W")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctBottomSingleConsonantError();
                                                        if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('w');
                    strcpy(KorSlot0,"w");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('w');
                    strcpy(KorSlot0,"w");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('3');
                strcpy(KorSlot0,"3");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_0:
        {
            if(event){
                if       (strcmp(KorSlot0,"d")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('a');
                    strcpy(KorSlot0,"a");
                } else if(strcmp(KorSlot0,"a")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('d');
                    strcpy(KorSlot0,"d");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('d');
                    strcpy(KorSlot0,"d");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('0');
                strcpy(KorSlot0,"0");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_7:
        {
            if(event){
                if       (strcmp(KorSlot0,"*")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctAraeAVowelError();
                    Keyboard.write ('j');
                    strcpy(KorSlot0,"j");
                } else if(strcmp(KorSlot0,":")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctAraeAVowelError();
                    Keyboard.write ('u');
                    strcpy(KorSlot0,"u");
                } else if(strcmp(KorSlot0,"k")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('o');
                    strcpy(KorSlot0,"o");
                } else if(strcmp(KorSlot0,"i")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('O');
                    strcpy(KorSlot0,"O");
                } else if(strcmp(KorSlot0,"j")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('p');
                    strcpy(KorSlot0,"p");
                } else if(strcmp(KorSlot0,"u")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('P');
                    strcpy(KorSlot0,"P");
                } else if(strcmp(KorSlot0,"h")==0){
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"hl");
                } else if(strcmp(KorSlot0,"n")==0){
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"nl");
                } else if(strcmp(KorSlot0,"m")==0){
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"ml");
                } else if(strcmp(KorSlot0,"hk")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('o');
                    strcpy(KorSlot0,"ho");
                } else if(strcmp(KorSlot0,"b")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('n');
                    Keyboard.write ('j');
                    strcpy(KorSlot0,"nj");
                } else if(strcmp(KorSlot0,"nj")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('p');
                    strcpy(KorSlot0,"np");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"l");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('7');
                strcpy(KorSlot0,"7");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_8:
        {
            if(event){
                if       (strcmp(KorSlot0,"l")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('k');
                    strcpy(KorSlot0,"k");
                } else if(strcmp(KorSlot0,"k")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('i');
                    strcpy(KorSlot0,"i");
                } else if(strcmp(KorSlot0,"m")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('n');
                    strcpy(KorSlot0,"n");
                } else if(strcmp(KorSlot0,"n")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('b');
                    strcpy(KorSlot0,"b");
                } else if(strcmp(KorSlot0,"hl")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('k');
                    strcpy(KorSlot0,"hk");
                } else if(strcmp(KorSlot0,"*")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write (':');
                    strcpy(KorSlot0,":");
                } else if(strcmp(KorSlot0,":")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('*');
                    strcpy(KorSlot0,"*");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('*');
                    strcpy(KorSlot0,"*");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('8');
                strcpy(KorSlot0,"8");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_9:
        {
            if(event){
                if       (strcmp(KorSlot0,"*")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctAraeAVowelError();
                    Keyboard.write ('h');
                    strcpy(KorSlot0,"h");
                } else if(strcmp(KorSlot0,":")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctAraeAVowelError();
                    Keyboard.write ('y');
                    strcpy(KorSlot0,"y");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('m');
                    strcpy(KorSlot0,"m");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('9');
                strcpy(KorSlot0,"9");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

        case KEYPAD_PERIOD:
        {
            if(event){
                
            } else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                if(!ConvertMode_Alphabet)
                {
                    KORPAD_resetConfirmedInputs();
                    
                    //Turn On  ConvertMode_Alphabet
                    ConvertMode_Alphabet = true;
                        
                    Buzzzzer.reserveBuzz(   new uint16_t[9] {   NOTE_FS5,0, NOTE_GS5,0, NOTE_AS5,0, NOTE_DS6,0, NOTE_CS6    },
                                            new uint16_t[9] {   200,30,     100,150,    120,200,    150,150,    450         },
                                            9   );
                    
                    //If CAPSLOCK was On, Turn Off BY FORCE
                    if(KBD_Hijacker.getStateCapsLockToggle()){
                        KBD_Hijacker.pressandreleaseKey(KEY_CAPS_LOCK);
                    } KBD_Hijacker.reserveSyncTKS=true;
                }
                else
                {
                    KORPAD_resetConfirmedInputs();
                    
                    //Turn Off ConvertMode_Alphabet
                    ConvertMode_Alphabet = false;
                        
                    Buzzzzer.reserveBuzz(   new uint16_t[7] {   NOTE_AS5,0, NOTE_GS5,0, NOTE_FS5,0, NOTE_FS5    },
                                            new uint16_t[7] {   300,50,     230,50,     170,100,    350         },
                                            7   );
                }
            } else{
                if       (strcmp(KorSlot0,"")==0){
                    Keyboard.write (' ');
                    strcpy(KorSlot0," ");
                } else if(strcmp(KorSlot0," ")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('?');
                    strcpy(KorSlot0,"?");
                } else if(strcmp(KorSlot0,"?")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('!');
                    strcpy(KorSlot0,"!");
                } else if(strcmp(KorSlot0,"!")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('.');
                    strcpy(KorSlot0,".");
                } else if(strcmp(KorSlot0,".")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write (' ');
                    strcpy(KorSlot0," ");
                    
                } else if(strcmp(KorSlot0,"*a")==0){        //a→A
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('A');
                    strcpy(KorSlot0,"*A");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*A")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('a');
                    strcpy(KorSlot0,"*a");
                } else if(strcmp(KorSlot0,"*b")==0){        //b→B
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('B');
                    strcpy(KorSlot0,"*B");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*B")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('b');
                    strcpy(KorSlot0,"*b");
                } else if(strcmp(KorSlot0,"*c")==0){        //c→C
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('C');
                    strcpy(KorSlot0,"*C");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*C")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('c');
                    strcpy(KorSlot0,"*c");
                } else if(strcmp(KorSlot0,"*d")==0){        //d→D
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('D');
                    strcpy(KorSlot0,"*D");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*D")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('d');
                    strcpy(KorSlot0,"*d");
                } else if(strcmp(KorSlot0,"*e")==0){        //e→E
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('E');
                    strcpy(KorSlot0,"*E");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*E")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('e');
                    strcpy(KorSlot0,"*e");
                } else if(strcmp(KorSlot0,"*f")==0){        //f→F
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('F');
                    strcpy(KorSlot0,"*F");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*F")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('f');
                    strcpy(KorSlot0,"*f");
                } else if(strcmp(KorSlot0,"*g")==0){        //g→G
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('G');
                    strcpy(KorSlot0,"*G");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*G")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('g');
                    strcpy(KorSlot0,"*g");
                } else if(strcmp(KorSlot0,"*h")==0){        //h→H
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('H');
                    strcpy(KorSlot0,"*H");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*H")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('h');
                    strcpy(KorSlot0,"*h");
                } else if(strcmp(KorSlot0,"*i")==0){        //i→I
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('I');
                    strcpy(KorSlot0,"*I");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*I")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('i');
                    strcpy(KorSlot0,"*i");
                } else if(strcmp(KorSlot0,"*j")==0){        //j→J
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('J');
                    strcpy(KorSlot0,"*J");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*J")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('j');
                    strcpy(KorSlot0,"*j");
                } else if(strcmp(KorSlot0,"*k")==0){        //k→K
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('K');
                    strcpy(KorSlot0,"*K");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*K")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('k');
                    strcpy(KorSlot0,"*k");
                } else if(strcmp(KorSlot0,"*l")==0){        //l→L
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('L');
                    strcpy(KorSlot0,"*L");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*L")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('l');
                    strcpy(KorSlot0,"*l");
                } else if(strcmp(KorSlot0,"*m")==0){        //m→M
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('M');
                    strcpy(KorSlot0,"*M");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*M")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('m');
                    strcpy(KorSlot0,"*m");
                } else if(strcmp(KorSlot0,"*n")==0){        //n→N
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('N');
                    strcpy(KorSlot0,"*N");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*N")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('n');
                    strcpy(KorSlot0,"*n");
                } else if(strcmp(KorSlot0,"*o")==0){        //o→O
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('O');
                    strcpy(KorSlot0,"*O");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*O")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('o');
                    strcpy(KorSlot0,"*o");
                } else if(strcmp(KorSlot0,"*p")==0){        //p→P
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('P');
                    strcpy(KorSlot0,"*P");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*P")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('p');
                    strcpy(KorSlot0,"*p");
                } else if(strcmp(KorSlot0,"*q")==0){        //q→Q
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('Q');
                    strcpy(KorSlot0,"*Q");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*Q")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('q');
                    strcpy(KorSlot0,"*q");
                } else if(strcmp(KorSlot0,"*r")==0){        //r→R
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('R');
                    strcpy(KorSlot0,"*R");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*R")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('r');
                    strcpy(KorSlot0,"*r");
                } else if(strcmp(KorSlot0,"*s")==0){        //s→S
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('S');
                    strcpy(KorSlot0,"*S");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*S")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('s');
                    strcpy(KorSlot0,"*s");
                } else if(strcmp(KorSlot0,"*t")==0){        //t→T
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('T');
                    strcpy(KorSlot0,"*T");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*T")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('t');
                    strcpy(KorSlot0,"*t");
                } else if(strcmp(KorSlot0,"*u")==0){        //u→U
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('U');
                    strcpy(KorSlot0,"*U");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*U")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('u');
                    strcpy(KorSlot0,"*u");
                } else if(strcmp(KorSlot0,"*v")==0){        //v→V
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('V');
                    strcpy(KorSlot0,"*V");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*V")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('v');
                    strcpy(KorSlot0,"*v");
                } else if(strcmp(KorSlot0,"*w")==0){        //w→W
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('W');
                    strcpy(KorSlot0,"*W");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*W")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('w');
                    strcpy(KorSlot0,"*w");
                } else if(strcmp(KorSlot0,"*x")==0){        //x→X
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('X');
                    strcpy(KorSlot0,"*X");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*X")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('x');
                    strcpy(KorSlot0,"*x");
                } else if(strcmp(KorSlot0,"*y")==0){        //y→Y
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('Y');
                    strcpy(KorSlot0,"*Y");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*Y")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('y');
                    strcpy(KorSlot0,"*y");
                } else if(strcmp(KorSlot0,"*z")==0){        //z→Z
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('Z');
                    strcpy(KorSlot0,"*Z");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*Z")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('z');
                    strcpy(KorSlot0,"*z");
                } else{
                    KORPAD_resetConfirmedInputs();
                    Keyboard.write (' '); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    strcpy(KorSlot0,"");
                }
            }
            isActivateKeyEvent=false; key=0;
        }
        break;

//        case KEY_TAB:
//        {
//            if(event){
//                
//            }
//            isActivateKeyEvent=false; key=0;
//        }
//        break;

        #endif
        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT1 CheonJiIn ------------------------------------------------ */



        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT2 NaRatGle ------------------------------------------------ */
        #if (KEYPAD_KOREAN_LAYOUT == 2)
        
        case KEYPAD_7:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('r');
                strcpy(KorSlot0,"r");
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('7');
                strcpy(KorSlot0,"7");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_8:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('s');
                strcpy(KorSlot0,"s");
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('8');
                strcpy(KorSlot0,"8");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_4:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('f');
                strcpy(KorSlot0,"f");
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('4');
                strcpy(KorSlot0,"4");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_5:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('a');
                strcpy(KorSlot0,"a");
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('5');
                strcpy(KorSlot0,"5");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_1:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('t');
                strcpy(KorSlot0,"t");
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('1');
                strcpy(KorSlot0,"1");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_2:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('d');
                strcpy(KorSlot0,"d");
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('2');
                strcpy(KorSlot0,"2");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_9:
        {
            if(event){
                if       (strcmp(KorSlot0,"k")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('j');
                    strcpy(KorSlot0,"j");
                } else if(strcmp(KorSlot0,"j")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('k');
                    strcpy(KorSlot0,"k");
                } else if(strcmp(KorSlot0,"h")==0){
                    Keyboard.write ('k');
                    strcpy(KorSlot0,"hk");
                } else if(strcmp(KorSlot0,"n")==0){
                    Keyboard.write ('j');
                    strcpy(KorSlot0,"nj");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('k');
                    strcpy(KorSlot0,"k");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('9');
                strcpy(KorSlot0,"9");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_6:
        {
            if(event){
                if       (strcmp(KorSlot0,"h")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('n');
                    strcpy(KorSlot0,"n");
                } else if(strcmp(KorSlot0,"n")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('h');
                    strcpy(KorSlot0,"h");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('h');
                    strcpy(KorSlot0,"h");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('6');
                strcpy(KorSlot0,"6");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_3:
        {
            if(event){
                if       (strcmp(KorSlot0,"k")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('o');
                    strcpy(KorSlot0,"o");
                } else if(strcmp(KorSlot0,"i")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('O');
                    strcpy(KorSlot0,"O");
                } else if(strcmp(KorSlot0,"j")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('p');
                    strcpy(KorSlot0,"p");
                } else if(strcmp(KorSlot0,"u")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('P');
                    strcpy(KorSlot0,"P");
                } else if(strcmp(KorSlot0,"h")==0){
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"hl");
                } else if(strcmp(KorSlot0,"n")==0){
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"nl");
                } else if(strcmp(KorSlot0,"m")==0){
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"ml");
                } else if(strcmp(KorSlot0,"hk")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('o');
                    strcpy(KorSlot0,"ho");
                } else if(strcmp(KorSlot0,"nj")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('p');
                    strcpy(KorSlot0,"np");
                } else{
                    KORPAD_updateConfirmedInputs();
                    Keyboard.write ('l');
                    strcpy(KorSlot0,"l");
                }
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('3');
                strcpy(KorSlot0,"3");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_0:
        {
            if(event){
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('m');
                strcpy(KorSlot0,"m");
            }
            else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                KORPAD_updateConfirmedInputs();
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('0');
                strcpy(KorSlot0,"0");
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEY_TAB:
        {
            if(event){
                if       (strcmp(KorSlot0,"r")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('z');
                    strcpy(KorSlot0,"z");
                } else if(strcmp(KorSlot0,"z")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('r');
                    strcpy(KorSlot0,"r");
                } else if(strcmp(KorSlot0,"s")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('e');
                    strcpy(KorSlot0,"e");
                } else if(strcmp(KorSlot0,"e")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('x');
                    strcpy(KorSlot0,"x");
                } else if(strcmp(KorSlot0,"x")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('s');
                    strcpy(KorSlot0,"s");
                } else if(strcmp(KorSlot0,"a")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('q');
                    strcpy(KorSlot0,"q");
                } else if(strcmp(KorSlot0,"q")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('v');
                    strcpy(KorSlot0,"v");
                } else if(strcmp(KorSlot0,"v")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('a');
                    strcpy(KorSlot0,"a");
                } else if(strcmp(KorSlot0,"t")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('w');
                    strcpy(KorSlot0,"w");
                } else if(strcmp(KorSlot0,"w")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('c');
                    strcpy(KorSlot0,"c");
                } else if(strcmp(KorSlot0,"c")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"r")==0){ KORPAD_correctBottomDualConsonantError(); }
                                                                    if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                                                                    if(strcmp(KorSlot1,"q")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('t');
                    strcpy(KorSlot0,"t");
                } else if(strcmp(KorSlot0,"d")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                                                                    if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('g');
                    strcpy(KorSlot0,"g");
                } else if(strcmp(KorSlot0,"g")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('d');
                    strcpy(KorSlot0,"d");
                } else if(strcmp(KorSlot0,"k")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('i');
                    strcpy(KorSlot0,"i");
                } else if(strcmp(KorSlot0,"i")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('k');
                    strcpy(KorSlot0,"k");
                } else if(strcmp(KorSlot0,"j")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('u');
                    strcpy(KorSlot0,"u");
                } else if(strcmp(KorSlot0,"u")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('j');
                    strcpy(KorSlot0,"j");
                } else if(strcmp(KorSlot0,"h")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('y');
                    strcpy(KorSlot0,"y");
                } else if(strcmp(KorSlot0,"y")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('h');
                    strcpy(KorSlot0,"h");
                } else if(strcmp(KorSlot0,"n")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('b');
                    strcpy(KorSlot0,"b");
                } else if(strcmp(KorSlot0,"b")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('n');
                    strcpy(KorSlot0,"n");
                } else if(strcmp(KorSlot0,"")==0||KeyLogger.peek_key(1)==KEY_TAB){
                        KORPAD_resetConfirmedInputs();
                        KBD_Hijacker.pressandreleaseKey(KEY_TAB);
                }
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        case KEYPAD_PERIOD:
        {
            if(event){
                
            } else if(PRESSED_TIME_UNTIL_RELEASE > 400){
                if(!ConvertMode_Alphabet)
                {
                    KORPAD_resetConfirmedInputs();
                    
                    //Turn On  ConvertMode_Alphabet
                    ConvertMode_Alphabet = true;
    
                    Buzzzzer.reserveBuzz(   new uint16_t[9] {   NOTE_FS5,0, NOTE_GS5,0, NOTE_AS5,0, NOTE_DS6,0, NOTE_CS6    },
                                            new uint16_t[9] {   200,30,     100,150,    120,200,    150,150,    450         },
                                            9   );
                    
                    //If CAPSLOCK was On, Turn Off BY FORCE
                    if(KBD_Hijacker.getStateCapsLockToggle()){
                        KBD_Hijacker.pressandreleaseKey(KEY_CAPS_LOCK);
                    } KBD_Hijacker.reserveSyncTKS=true;
                }
                else
                {
                    KORPAD_resetConfirmedInputs();
                    
                    //Turn Off ConvertMode_Alphabet
                    ConvertMode_Alphabet = false;
                                            
                    Buzzzzer.reserveBuzz(   new uint16_t[7] {   NOTE_AS5,0, NOTE_GS5,0, NOTE_FS5,0, NOTE_FS5    },
                                            new uint16_t[7] {   300,50,     230,50,     170,100,    350         },
                                            7   );
                }
            } else{
                if       (strcmp(KorSlot0,"r")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('R');
                    strcpy(KorSlot0,"R");
                } else if(strcmp(KorSlot0,"R")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('r');
                    strcpy(KorSlot0,"r");
                } else if(strcmp(KorSlot0,"e")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('E');
                    strcpy(KorSlot0,"E");
                } else if(strcmp(KorSlot0,"E")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
                    Keyboard.write ('e');
                    strcpy(KorSlot0,"e");
                } else if(strcmp(KorSlot0,"q")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('Q');
                    strcpy(KorSlot0,"Q");
                } else if(strcmp(KorSlot0,"Q")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
                                                                    if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('q');
                    strcpy(KorSlot0,"q");
                } else if(strcmp(KorSlot0,"t")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('T');
                    strcpy(KorSlot0,"T");
                } else if(strcmp(KorSlot0,"T")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"r")==0){ KORPAD_correctBottomDualConsonantError(); }
                                                                    if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                                                                    if(strcmp(KorSlot1,"q")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('t');
                    strcpy(KorSlot0,"t");
                } else if(strcmp(KorSlot0,"w")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('W');
                    strcpy(KorSlot0,"W");
                } else if(strcmp(KorSlot0,"W")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
                                                                    if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                    Keyboard.write ('w');
                    strcpy(KorSlot0,"w");
                } else if(strcmp(KorSlot0," ")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write (';');
                    strcpy(KorSlot0,";");
                } else if(strcmp(KorSlot0,";")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('?');
                    strcpy(KorSlot0,"?");
                } else if(strcmp(KorSlot0,"?")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('!');
                    strcpy(KorSlot0,"!");
                } else if(strcmp(KorSlot0,"!")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('.');
                    strcpy(KorSlot0,".");
                } else if(strcmp(KorSlot0,".")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('~');
                    strcpy(KorSlot0,"~");
                } else if(strcmp(KorSlot0,"~")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write ('^'); Keyboard.write ('^');
                    strcpy(KorSlot0,"^^");
                } else if(strcmp(KorSlot0,"^^")==0){
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    Keyboard.write (' ');
                    strcpy(KorSlot0," ");
    
                } else if(strcmp(KorSlot0,"*a")==0){        //a→A
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('A');
                    strcpy(KorSlot0,"*A");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*A")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('a');
                    strcpy(KorSlot0,"*a");
                } else if(strcmp(KorSlot0,"*b")==0){        //b→B
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('B');
                    strcpy(KorSlot0,"*B");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*B")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('b');
                    strcpy(KorSlot0,"*b");
                } else if(strcmp(KorSlot0,"*c")==0){        //c→C
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('C');
                    strcpy(KorSlot0,"*C");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*C")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('c');
                    strcpy(KorSlot0,"*c");
                } else if(strcmp(KorSlot0,"*d")==0){        //d→D
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('D');
                    strcpy(KorSlot0,"*D");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*D")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('d');
                    strcpy(KorSlot0,"*d");
                } else if(strcmp(KorSlot0,"*e")==0){        //e→E
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('E');
                    strcpy(KorSlot0,"*E");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*E")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('e');
                    strcpy(KorSlot0,"*e");
                } else if(strcmp(KorSlot0,"*f")==0){        //f→F
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('F');
                    strcpy(KorSlot0,"*F");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*F")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('f');
                    strcpy(KorSlot0,"*f");
                } else if(strcmp(KorSlot0,"*g")==0){        //g→G
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('G');
                    strcpy(KorSlot0,"*G");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*G")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('g');
                    strcpy(KorSlot0,"*g");
                } else if(strcmp(KorSlot0,"*h")==0){        //h→H
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('H');
                    strcpy(KorSlot0,"*H");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*H")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('h');
                    strcpy(KorSlot0,"*h");
                } else if(strcmp(KorSlot0,"*i")==0){        //i→I
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('I');
                    strcpy(KorSlot0,"*I");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*I")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('i');
                    strcpy(KorSlot0,"*i");
                } else if(strcmp(KorSlot0,"*j")==0){        //j→J
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('J');
                    strcpy(KorSlot0,"*J");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*J")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('j');
                    strcpy(KorSlot0,"*j");
                } else if(strcmp(KorSlot0,"*k")==0){        //k→K
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('K');
                    strcpy(KorSlot0,"*K");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*K")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('k');
                    strcpy(KorSlot0,"*k");
                } else if(strcmp(KorSlot0,"*l")==0){        //l→L
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('L');
                    strcpy(KorSlot0,"*L");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*L")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('l');
                    strcpy(KorSlot0,"*l");
                } else if(strcmp(KorSlot0,"*m")==0){        //m→M
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('M');
                    strcpy(KorSlot0,"*M");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*M")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('m');
                    strcpy(KorSlot0,"*m");
                } else if(strcmp(KorSlot0,"*n")==0){        //n→N
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('N');
                    strcpy(KorSlot0,"*N");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*N")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('n');
                    strcpy(KorSlot0,"*n");
                } else if(strcmp(KorSlot0,"*o")==0){        //o→O
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('O');
                    strcpy(KorSlot0,"*O");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*O")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('o');
                    strcpy(KorSlot0,"*o");
                } else if(strcmp(KorSlot0,"*p")==0){        //p→P
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('P');
                    strcpy(KorSlot0,"*P");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*P")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('p');
                    strcpy(KorSlot0,"*p");
                } else if(strcmp(KorSlot0,"*q")==0){        //q→Q
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('Q');
                    strcpy(KorSlot0,"*Q");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*Q")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('q');
                    strcpy(KorSlot0,"*q");
                } else if(strcmp(KorSlot0,"*r")==0){        //r→R
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('R');
                    strcpy(KorSlot0,"*R");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*R")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('r');
                    strcpy(KorSlot0,"*r");
                } else if(strcmp(KorSlot0,"*s")==0){        //s→S
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('S');
                    strcpy(KorSlot0,"*S");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*S")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('s');
                    strcpy(KorSlot0,"*s");
                } else if(strcmp(KorSlot0,"*t")==0){        //t→T
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('T');
                    strcpy(KorSlot0,"*T");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*T")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('t');
                    strcpy(KorSlot0,"*t");
                } else if(strcmp(KorSlot0,"*u")==0){        //u→U
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('U');
                    strcpy(KorSlot0,"*U");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*U")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('u');
                    strcpy(KorSlot0,"*u");
                } else if(strcmp(KorSlot0,"*v")==0){        //v→V
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('V');
                    strcpy(KorSlot0,"*V");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*V")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('v');
                    strcpy(KorSlot0,"*v");
                } else if(strcmp(KorSlot0,"*w")==0){        //w→W
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('W');
                    strcpy(KorSlot0,"*W");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*W")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('w');
                    strcpy(KorSlot0,"*w");
                } else if(strcmp(KorSlot0,"*x")==0){        //x→X
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('X');
                    strcpy(KorSlot0,"*X");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*X")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('x');
                    strcpy(KorSlot0,"*x");
                } else if(strcmp(KorSlot0,"*y")==0){        //y→Y
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('Y');
                    strcpy(KorSlot0,"*Y");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*Y")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('y');
                    strcpy(KorSlot0,"*y");
                } else if(strcmp(KorSlot0,"*z")==0){        //z→Z
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('Z');
                    strcpy(KorSlot0,"*Z");
                    KORPAD_switchCapsLockToggleIfBattered3();
                } else if(strcmp(KorSlot0,"*Z")==0){
                    KORPAD_switchCapsLockToggleIfBattered4();
                    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    KORPAD_writeAlphabet('z');
                    strcpy(KorSlot0,"*z");
                } else{
                    KORPAD_resetConfirmedInputs();
                    Keyboard.write (' '); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                    strcpy(KorSlot0,"");
                }
            }
            isActivateKeyEvent=false; key=0;
        }
        break;
        
        #endif
        /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT2 NaRatGle ------------------------------------------------ */



    }
    
    #endif
    /* ------------------------------------------------------ KEYPAD_KOREAN_LAYOUT COMMON ------------------------------------------------------ */
}
