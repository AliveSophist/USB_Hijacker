
void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{
//    MODIFIERKEYS beforeMod; *((uint8_t*)&beforeMod) = before;
//    MODIFIERKEYS afterMod; *((uint8_t*)&afterMod) = after;
//
//    if(beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl)
//        if (beforeMod.bmLeftCtrl < afterMod.bmLeftCtrl)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_CTRL,true);      }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_CTRL,false);     }
//
//    if (beforeMod.bmLeftShift != afterMod.bmLeftShift)
//        if (beforeMod.bmLeftShift < afterMod.bmLeftShift)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_SHIFT,true);     }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_SHIFT,false);    }
//
//    if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt)
//        if (beforeMod.bmLeftAlt < afterMod.bmLeftAlt)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_ALT,true);       }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_ALT,false);      }
//
//    if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI)
//        if (beforeMod.bmLeftGUI < afterMod.bmLeftGUI)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_GUI,true);       }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_LEFT_GUI,false);      }
//
//    if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl)
//        if (beforeMod.bmRightCtrl < afterMod.bmRightCtrl)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_CTRL,true);     }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_CTRL,false);    }
//
//    if (beforeMod.bmRightShift != afterMod.bmRightShift)
//        if (beforeMod.bmRightShift < afterMod.bmRightShift)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_SHIFT,true);    }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_SHIFT,false);   }
//
//    if (beforeMod.bmRightAlt != afterMod.bmRightAlt)
//        if (beforeMod.bmRightAlt < afterMod.bmRightAlt)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_ALT,true);      }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_ALT,false);     }
//
//    if (beforeMod.bmRightGUI != afterMod.bmRightGUI)
//        if (beforeMod.bmRightGUI < afterMod.bmRightGUI)
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_GUI,true);      }
//        else
//        {   KBD_Hijacker.txHijackedKeyEvent(KEY_RIGHT_GUI,false);     }
//
//    return;
}

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t keycode)
{
    key   = keycode;
    event = true;

    isExistWaitingEvent_Press = true;
    numDN+=1;

    if(isSerial){ if(mod!=255){ Serial.print("(Before Hijack) DN "); KBD_Parser.PrintKey(mod, keycode); Serial.println(); } }

    return;
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t keycode)
{
    key   = keycode;
    event = false;
    
    isExistWaitingEvent_Release = true;
    if(numDN>0) numDN-=1;
    
    if(isSerial){ if(mod!=255){ Serial.print("(Before Hijack) UP "); KBD_Parser.PrintKey(mod, keycode); } }

    return;
}

void KbdRptParser::PrintKey(uint8_t mod, uint8_t keycode) //For, Debugging
{
    MODIFIERKEYS m; *((uint8_t*)&m) = mod;
    Serial.print((m.bmLeftCtrl     == 1) ? "C" : " ");
    Serial.print((m.bmLeftShift    == 1) ? "S" : " ");
    Serial.print((m.bmLeftAlt      == 1) ? "A" : " ");
    Serial.print((m.bmLeftGUI      == 1) ? "G" : " ");
    Serial.print(" >0x"); if(keycode<16)Serial.print('0'); Serial.print(keycode, HEX); Serial.print("< ");
    Serial.print((m.bmRightCtrl    == 1) ? "C" : " ");
    Serial.print((m.bmRightShift   == 1) ? "S" : " ");
    Serial.print((m.bmRightAlt     == 1) ? "A" : " ");
    Serial.print((m.bmRightGUI     == 1) ? "G" : " ");

    return;
}

void KbdRptParser::switchCapsLockLED()
{
    HandleLockingKeys(&kbd, KEY_CAPS_LOCK);

    return;
}
void KbdRptParser::switchScrollLockLED()
{
    HandleLockingKeys(&kbd, KEY_SCROLL_LOCK);

    return;
} 
void KbdRptParser::switchNumLockLED()
{
    HandleLockingKeys(&kbd, KEY_NUM_LOCK);

    return;
}
