
#define KEYPAD_KOREAN_LAYOUT 1      // 0:None   1:CheonJiIn   2:NaRatGeul

#if (KEYPAD_KOREAN_LAYOUT > 0)

#define KORPAD_updateConfirmedInputs()      updateConfirmedInputs(KorSlot0,KorSlot1,KorSlot2,KorSlot3,KorSlot4);
#define KORPAD_resetConfirmedInputs()       resetConfirmedInputs(KorSlot0,KorSlot1,KorSlot2,KorSlot3,KorSlot4);
#define KORPAD_undoConfirmedInputs()        undoConfirmedInputs(KorSlot0,KorSlot1,KorSlot2,KorSlot3,KorSlot4);
#define KORPAD_fixCharacter(CH)             fixCharacter(KorSlot0,CH)
#define KORPAD_correctBottomDualConsonantError()    correctBottomDualConsonantError(KorSlot1,KorSlot2,KorSlot3);
#define KORPAD_correctBottomSingleConsonantError()  correctBottomSingleConsonantError(KorSlot1,KorSlot2);
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
void fixCharacter(char* KorSlot0,char* ch)
{
    KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
    Keyboard.write(ch);
    KorSlot0[0]=ch;
}
bool isExistVowel(char* KorSlotX)
{
    return strcmp(KorSlotX,"k")==0||strcmp(KorSlotX,"o")==0||strcmp(KorSlotX,"i")==0||strcmp(KorSlotX,"O")==0||strcmp(KorSlotX,"j")==0||strcmp(KorSlotX,"p")==0||strcmp(KorSlotX,"u")==0||strcmp(KorSlotX,"P")==0||strcmp(KorSlotX,"h")==0||strcmp(KorSlotX,"y")==0||strcmp(KorSlotX,"n")==0||strcmp(KorSlotX,"b")==0||strcmp(KorSlotX,"m")==0||strcmp(KorSlotX,"l")==0||strcmp(KorSlotX,"hk")==0||strcmp(KorSlotX,"ho")==0||strcmp(KorSlotX,"hl")==0||strcmp(KorSlotX,"nj")==0||strcmp(KorSlotX,"np")==0||strcmp(KorSlotX,"nl")==0||strcmp(KorSlotX,"ml")==0;
}
bool isExistConsonant(char* KorSlotX)
{
    return strcmp(KorSlotX,"r")==0||strcmp(KorSlotX,"z")==0||strcmp(KorSlotX,"R")==0||strcmp(KorSlotX,"s")==0||strcmp(KorSlotX,"f")==0||strcmp(KorSlotX,"e")==0||strcmp(KorSlotX,"x")==0||strcmp(KorSlotX,"E")==0||strcmp(KorSlotX,"q")==0||strcmp(KorSlotX,"b")==0||strcmp(KorSlotX,"Q")==0||strcmp(KorSlotX,"t")==0||strcmp(KorSlotX,"g")==0||strcmp(KorSlotX,"T")==0||strcmp(KorSlotX,"w" )==0||strcmp(KorSlotX,"c")==0||strcmp(KorSlotX,"W")==0||strcmp(KorSlotX,"d")==0||strcmp(KorSlotX,"a")==0;
}
void correctBottomDualConsonantError(char* KorSlot1,char* KorSlot2,char* KorSlot3)
{
    if(isExistVowel(KorSlot2))
    {
        if(isExistConsonant(KorSlot3))
        {
            delay(15); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
            delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot3);
            delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
            delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
            delay(15);
            return;
        }
    }
}
void correctBottomSingleConsonantError(char* KorSlot1,char* KorSlot2)
{
    if(isExistVowel(KorSlot1))
    {
        if(isExistConsonant(KorSlot2))
        {
            delay(15); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
            delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
            delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
            delay(15); 
            return;
        }
    }
}

#endif

#if (KEYPAD_KOREAN_LAYOUT == 1)     // LAYOUT1:CheonJiIn Only

#define KORPAD_correctAraeAVowelError()             correctAraeAVowelError(KorSlot1,KorSlot2,KorSlot3,KorSlot4);
void correctAraeAVowelError(char* KorSlot1,char* KorSlot2,char* KorSlot3,char* KorSlot4)
{
    if(isExistConsonant(KorSlot1))
    {
        if(isExistVowel(KorSlot2))
        {
            if(isExistConsonant(KorSlot3))
            {
                delay(15); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot3);
                delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
                delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
                delay(15);
                return;
            }
        }
        else if(isExistConsonant(KorSlot2))
        {
            if((strcmp(KorSlot2,"r")==0&&strcmp(KorSlot1,"t")==0)||(strcmp(KorSlot2,"s")==0&&strcmp(KorSlot1,"w")==0)||(strcmp(KorSlot2,"s")==0&&strcmp(KorSlot1,"g")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"r")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"a")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"q")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"t")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"x")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"v")==0)||(strcmp(KorSlot2,"f")==0&&strcmp(KorSlot1,"g")==0)||(strcmp(KorSlot2,"q")==0&&strcmp(KorSlot1,"t")==0))
            {
                if(isExistVowel(KorSlot3))
                {
                    if(isExistConsonant(KorSlot4))
                    {
                        delay(15); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                        delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot4);
                        delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot3);
                        delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot2);
                        delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
                        delay(15);
                        return;
                    }
                }
            }
        }
        delay(15); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
        delay(15); KBD_Hijacker.pressandreleaseKeys(KorSlot1);
        return;
    }
}

#endif





void KeyboardHijacker::MODULE_KOREAN_KEYPAD_EVOLUTION()
{
    /* -------------------------------------------------- KEYPAD_KOREAN_LAYOUT COMMON -------------------------------------------------- */
    #if (KEYPAD_KOREAN_LAYOUT > 0)

    static bool KoreanKeypadToggle = false;
    static char KorSlot0[3], KorSlot1[3], KorSlot2[3], KorSlot3[3], KorSlot4[3];

    if (key == KEY_NUM_LOCK)
    {
        if(KoreanKeypadToggle){
            //Turn Off KoreanKeypadToggle
            KoreanKeypadToggle = false;
            TONE(NOTE_D3,222);
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KBD_Hijacker.releaseAllBeingHoldDownKey();   // KEY_NUM_LOCK release needed

            //Turn On  KoreanKeypadToggle
            KoreanKeypadToggle = true;
            TONE(NOTE_G5,333);
            
            //While KoreanKeypadToggle is on, Numlock is always on
            if(!KBD_Hijacker.getStateNumLockToggle()){
                KBD_Hijacker.pressandreleaseKey(KEY_NUM_LOCK);
            }

            //Reverse KorEng status, when ScrollLock off
            //Support by KorEngStatus.exe (Maker's Blog:https://blog.naver.com/breeze4me/140056743544)
            if(!KBD_Hijacker.getStateScrollLockToggle()){
                bool stateBeforeReverse = KBD_Hijacker.getStateScrollLockToggle();
                
                KBD_Hijacker.pressandreleaseKey(KEY_KORENG);
                delay(50); KBD_Hijacker.syncToggleKeyStates();

                //Rollback KorEng status, if system is not using KorEngStatus.exe
                if(stateBeforeReverse == KBD_Hijacker.getStateScrollLockToggle()){
                    KBD_Hijacker.pressandreleaseKey(KEY_KORENG);
                }
            }
            
            //Reverse KorEng status, NumLock pressed more than 1600 millis
            if(PRESSED_TIME_UNTIL_RELEASE > 1200){
                KBD_Hijacker.pressandreleaseKey(KEY_KORENG);
            }
            delay(50); KBD_Hijacker.syncToggleKeyStates();
            KORPAD_resetConfirmedInputs();

            isActivateKeyEvent=false; key=0;
        }
    }





    if(!KoreanKeypadToggle)
        return;


    if (key == KEYPAD_DIVIDE)
    {
        if(event){
            KORPAD_resetConfirmedInputs();

            KBD_Hijacker.pressandreleaseKey(KEY_F2);
            delay(10);
            KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_CTRL,KEY_C} );
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_CTRL,KEY_F} );
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_MULTIPLY)
    {
        if(event){
            KORPAD_resetConfirmedInputs();

            KBD_Hijacker.pressandreleaseKey(KEY_F2);
            delay(10);
            KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_CTRL,KEY_V} );
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_CTRL,KEY_H} );
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEY_BACKSPACE)
    {
        if(event){
            KORPAD_resetConfirmedInputs();

            KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_SHIFT,KEY_LEFT_ARROW} );
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 1600){
            KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_CTRL,KEY_A} );
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_CTRL,KEY_Z} );
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_SUBTRACT)
    {
        if(event){
            KORPAD_undoConfirmedInputs();
        }
        key = KEY_BACKSPACE;
    }
    else if (key == KEYPAD_ADD)
    {
        if(event){
            KORPAD_resetConfirmedInputs(); strcpy(KorSlot0," ");
        }
        key = KEY_SPACE;
    }
    else if (key == KEYPAD_ENTER)
    {
        if(event){
            if(strcmp(KorSlot0," ")==0){
                KBD_Hijacker.pressandreleaseShortcutKey( 2 , new KeyboardKeycode[2] {KEY_SHIFT,KEY_ENTER} );
            }
            else{
                KBD_Hijacker.pressandreleaseKey(KEY_ENTER);
            }
            KORPAD_resetConfirmedInputs();
        }
        isActivateKeyEvent=false; key=0;
    }



    /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT1 CheonJiIn ------------------------------------------------ */
    #if (KEYPAD_KOREAN_LAYOUT == 1)

    else if (key == KEYPAD_4){
        if(event){
            if       (strcmp(KorSlot0,"r")==0){
                KORPAD_fixCharacter('z');
            } else if(strcmp(KorSlot0,"z")==0){
                KORPAD_fixCharacter('R');
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
            KORPAD_fixCharacter('4');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_5){
        if(event){
            if       (strcmp(KorSlot0,"s")==0){
                KORPAD_fixCharacter('f');
            } else if(strcmp(KorSlot0,"f")==0){
                KORPAD_fixCharacter('s');
            } else{
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('s');
                strcpy(KorSlot0,"s");
            }
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('5');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_6){
        if(event){
            if       (strcmp(KorSlot0,"e")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('x');
                strcpy(KorSlot0,"x");
            } else if(strcmp(KorSlot0,"x")==0){
                KORPAD_fixCharacter('E');
            } else if(strcmp(KorSlot0,"E")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
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
            KORPAD_fixCharacter('6');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_1){
        if(event){
            if       (strcmp(KorSlot0,"q")==0){
                KORPAD_fixCharacter('v');
            } else if(strcmp(KorSlot0,"v")==0){
                KORPAD_fixCharacter('Q');
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
            KORPAD_fixCharacter('1');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_2){
        if(event){
            if       (strcmp(KorSlot0,"t")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('g');
                strcpy(KorSlot0,"g");
            } else if(strcmp(KorSlot0,"g")==0){
                KORPAD_fixCharacter('T');
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
            KORPAD_fixCharacter('2');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_3){
        if(event){
            if       (strcmp(KorSlot0,"w")==0){
                KORPAD_fixCharacter('c');
            } else if(strcmp(KorSlot0,"c")==0){
                KORPAD_fixCharacter('W');
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
            KORPAD_fixCharacter('3');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_0){
        if(event){
            if       (strcmp(KorSlot0,"d")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('a');
                strcpy(KorSlot0,"a");
            } else if(strcmp(KorSlot0,"a")==0){
                KORPAD_fixCharacter('d');
            } else{
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('d');
                strcpy(KorSlot0,"d");
            }
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('0');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_7){
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
                KORPAD_fixCharacter('o');
            } else if(strcmp(KorSlot0,"i")==0){
                KORPAD_fixCharacter('O');
            } else if(strcmp(KorSlot0,"j")==0){
                KORPAD_fixCharacter('p');
            } else if(strcmp(KorSlot0,"u")==0){
                KORPAD_fixCharacter('P');
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
                Keyboard.write ('n'); Keyboard.write ('j');
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
            KORPAD_fixCharacter('7');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_8){
        if(event){
            if       (strcmp(KorSlot0,"l")==0){
                KORPAD_fixCharacter('k');
            } else if(strcmp(KorSlot0,"k")==0){
                KORPAD_fixCharacter('i');
            } else if(strcmp(KorSlot0,"m")==0){
                KORPAD_fixCharacter('n');
            } else if(strcmp(KorSlot0,"n")==0){
                KORPAD_fixCharacter('b');
            } else if(strcmp(KorSlot0,"hl")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('k');
                strcpy(KorSlot0,"hk");
            } else if(strcmp(KorSlot0,"*")==0){
                KORPAD_fixCharacter(':');
            } else if(strcmp(KorSlot0,":")==0){
                KORPAD_fixCharacter('*');
            } else{
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('*');
                strcpy(KorSlot0,"*");
            }
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('8');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_9){
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
            KORPAD_fixCharacter('9');
        }
        isActivateKeyEvent=false; key=0;
    }
//    else if (key == KEY_TAB){
//        if(event){
//            
//        }
//        isActivateKeyEvent=false; key=0;
//    }
    else if (key == KEYPAD_DOT){
        if(event){
            
        } else{
            if       (strcmp(KorSlot0,"")==0){
                Keyboard.write (' ');
                strcpy(KorSlot0," ");
            } else if(strcmp(KorSlot0," ")==0){
                KORPAD_fixCharacter('?');
            } else if(strcmp(KorSlot0,"?")==0){
                KORPAD_fixCharacter('!');
            } else if(strcmp(KorSlot0,"!")==0){
                KORPAD_fixCharacter('.');
            } else if(strcmp(KorSlot0,".")==0){
                KORPAD_fixCharacter(' ');
            } else{
                KORPAD_resetConfirmedInputs();
                Keyboard.write (' '); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                strcpy(KorSlot0,"");
            }
        }
        isActivateKeyEvent=false; key=0;
    }

    #endif
    /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT1 CheonJiIn ------------------------------------------------ */



    /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT2 NaRatGeul ------------------------------------------------ */
    #if (KEYPAD_KOREAN_LAYOUT == 2)
    
    if (key == KEYPAD_7){
        if(event){
            KORPAD_updateConfirmedInputs();
            Keyboard.write ('r');
            strcpy(KorSlot0,"r");
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('7');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_8){
        if(event){
            KORPAD_updateConfirmedInputs();
            Keyboard.write ('s');
            strcpy(KorSlot0,"s");
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('8');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_4){
        if(event){
            KORPAD_updateConfirmedInputs();
            Keyboard.write ('f');
            strcpy(KorSlot0,"f");
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('4');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_5){
        if(event){
            KORPAD_updateConfirmedInputs();
            Keyboard.write ('a');
            strcpy(KorSlot0,"a");
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('5');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_1){
        if(event){
            KORPAD_updateConfirmedInputs();
            Keyboard.write ('t');
            strcpy(KorSlot0,"t");
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('1');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_2){
        if(event){
            KORPAD_updateConfirmedInputs();
            Keyboard.write ('d');
            strcpy(KorSlot0,"d");
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('2');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_9){
        if(event){
            if       (strcmp(KorSlot0,"k")==0){
                KORPAD_fixCharacter('j');
            } else if(strcmp(KorSlot0,"j")==0){
                KORPAD_fixCharacter('k');
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
            KORPAD_fixCharacter('9');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_6){
        if(event){
            if       (strcmp(KorSlot0,"h")==0){
                KORPAD_fixCharacter('n');
            } else if(strcmp(KorSlot0,"n")==0){
                KORPAD_fixCharacter('h');
            } else{
                KORPAD_updateConfirmedInputs();
                Keyboard.write ('h');
                strcpy(KorSlot0,"h");
            }
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('6');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_3){
        if(event){
            if       (strcmp(KorSlot0,"k")==0){
                KORPAD_fixCharacter('o');
            } else if(strcmp(KorSlot0,"i")==0){
                KORPAD_fixCharacter('O');
            } else if(strcmp(KorSlot0,"j")==0){
                KORPAD_fixCharacter('p');
            } else if(strcmp(KorSlot0,"u")==0){
                KORPAD_fixCharacter('P');
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
            KORPAD_fixCharacter('3');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_0){
        if(event){
            KORPAD_updateConfirmedInputs();
            Keyboard.write ('m');
            strcpy(KorSlot0,"m");
        }
        else if(PRESSED_TIME_UNTIL_RELEASE > 400){
            KORPAD_updateConfirmedInputs();
            KORPAD_fixCharacter('0');
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEY_TAB){
        if(event){
            if       (strcmp(KorSlot0,"r")==0){
                KORPAD_fixCharacter('z');
            } else if(strcmp(KorSlot0,"z")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('r');
                strcpy(KorSlot0,"r");
            } else if(strcmp(KorSlot0,"s")==0){
                KORPAD_fixCharacter('e');
            } else if(strcmp(KorSlot0,"e")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('x');
                strcpy(KorSlot0,"x");
            } else if(strcmp(KorSlot0,"x")==0){
                KORPAD_fixCharacter('s');
            } else if(strcmp(KorSlot0,"a")==0){
                KORPAD_fixCharacter('q');
            } else if(strcmp(KorSlot0,"q")==0){
                KORPAD_fixCharacter('v');
            } else if(strcmp(KorSlot0,"v")==0){
                KORPAD_fixCharacter('a');
            } else if(strcmp(KorSlot0,"t")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('w');
                strcpy(KorSlot0,"w");
            } else if(strcmp(KorSlot0,"w")==0){
                KORPAD_fixCharacter('c');
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
                KORPAD_fixCharacter('d');
            } else if(strcmp(KorSlot0,"k")==0){
                KORPAD_fixCharacter('i');
            } else if(strcmp(KorSlot0,"i")==0){
                KORPAD_fixCharacter('k');
            } else if(strcmp(KorSlot0,"j")==0){
                KORPAD_fixCharacter('u');
            } else if(strcmp(KorSlot0,"u")==0){
                KORPAD_fixCharacter('j');
            } else if(strcmp(KorSlot0,"h")==0){
                KORPAD_fixCharacter('y');
            } else if(strcmp(KorSlot0,"y")==0){
                KORPAD_fixCharacter('h');
            } else if(strcmp(KorSlot0,"n")==0){
                KORPAD_fixCharacter('b');
            } else if(strcmp(KorSlot0,"b")==0){
                KORPAD_fixCharacter('n');
            }
        }
        isActivateKeyEvent=false; key=0;
    }
    else if (key == KEYPAD_DOT){
        if(event){
        } else{
            if       (strcmp(KorSlot0,"r")==0){
                KORPAD_fixCharacter('R');
            } else if(strcmp(KorSlot0,"R")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('r');
                strcpy(KorSlot0,"r");
            } else if(strcmp(KorSlot0,"e")==0){
                KORPAD_fixCharacter('E');
            } else if(strcmp(KorSlot0,"E")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KORPAD_correctBottomSingleConsonantError();
                Keyboard.write ('e');
                strcpy(KorSlot0,"e");
            } else if(strcmp(KorSlot0,"q")==0){
                KORPAD_fixCharacter('Q');
            } else if(strcmp(KorSlot0,"Q")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctBottomSingleConsonantError();
                                                    if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('q');
                strcpy(KorSlot0,"q");
            } else if(strcmp(KorSlot0,"t")==0){
                KORPAD_fixCharacter('T');
            } else if(strcmp(KorSlot0,"T")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  if(strcmp(KorSlot1,"r")==0){ KORPAD_correctBottomDualConsonantError(); }
                                                    if(strcmp(KorSlot1,"f")==0){ KORPAD_correctBottomDualConsonantError(); }
                                                    if(strcmp(KorSlot1,"q")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('t');
                strcpy(KorSlot0,"t");
            } else if(strcmp(KorSlot0,"w")==0){
                KORPAD_fixCharacter('W');
            } else if(strcmp(KorSlot0,"W")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);  KORPAD_correctBottomSingleConsonantError();
                                                    if(strcmp(KorSlot1,"s")==0){ KORPAD_correctBottomDualConsonantError(); }
                Keyboard.write ('w');
                strcpy(KorSlot0,"w");
            } else if(strcmp(KorSlot0," ")==0){
                KORPAD_fixCharacter(';');
            } else if(strcmp(KorSlot0,";")==0){
                KORPAD_fixCharacter('?');
            } else if(strcmp(KorSlot0,"?")==0){
                KORPAD_fixCharacter('!');
            } else if(strcmp(KorSlot0,"!")==0){
                KORPAD_fixCharacter('.');
            } else if(strcmp(KorSlot0,".")==0){
                KORPAD_fixCharacter('~');
            } else if(strcmp(KorSlot0,"~")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write ('^'); Keyboard.write ('^');
                strcpy(KorSlot0,"^^");
            } else if(strcmp(KorSlot0,"^^")==0){
                KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                Keyboard.write (' ');
                strcpy(KorSlot0," ");
            } else{
                KORPAD_resetConfirmedInputs();
                Keyboard.write (' '); KBD_Hijacker.pressandreleaseKey(KEY_BACKSPACE);
                strcpy(KorSlot0,"");
            }
        }
        isActivateKeyEvent=false; key=0;
    }

    #endif
    /* ------------------------------------------------ KEYPAD_KOREAN_LAYOUT2 NaRatGeul ------------------------------------------------ */

    #endif
    /* -------------------------------------------------- KEYPAD_KOREAN_LAYOUT COMMON -------------------------------------------------- */
}
