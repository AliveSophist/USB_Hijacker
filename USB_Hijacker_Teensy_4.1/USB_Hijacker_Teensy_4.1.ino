/*  This Project utilizes Teensy® 4.1 Development Board
 *  Teensy® 4.1 Official Site : https://www.pjrc.com/store/teensy41.html
 *  Teensyduino  Installation : https://www.pjrc.com/teensy/td_download.html
 *  
 *  This Project is recommended to
 *  select [USB Type: "Keyboard"] from Tools -> USB Type: menu.
 *  
 *  but if you also want to use Serial communication for debugging,
 *  select [USB Type: "Serial + Keyboard + Mouse + Joystick"]
 *  
 *  in case YOU MUST UNCONDITIONALLY 'OPEN Serial Monitor BEFORE UPLOAD' PLEASE!!
 *  
 *  GOT IT? Let's H.I.J.A.C.K.  (*・_・)ノ*`*`*/





#ifdef __cplusplus
extern "C"
{
#endif
    struct usb_string_descriptor_struct
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t wString[100];
    };
    usb_string_descriptor_struct usb_string_manufacturer_name = {
    /*  
     *  2 + MANUFACTURER_NAME_LEN * 2,
     *  3,
     *  { MANUFACTURER_NAME }
     */
        2 + 7 * 2,
        3,
        {'$','0','?','H','!','$','+'}
    };
    usb_string_descriptor_struct usb_string_product_name = {
    /*
     *  2 + PRODUCT_NAME_LEN * 2,
     *  3,
     *  { PRODUCT_NAME }
     */
        #if defined(USB_KEYBOARDONLY)   // [USB Type: "Keyboard"]
        2 + 19 * 2,
        3,
        {'I','\'','m',' ','n','o','t',' ','H','i','j','a','c','k','e','r',' ','X','D'}
        #elif defined(USB_SERIAL_HID)   // [USB Type: "Serial + Keyboard + Mouse + Joystick"]
        2 + 28 * 2,
        3,
        {'I','\'','m',' ','n','o','t',' ','H','i','j','a','c','k','e','r',' ','S','E','R','I','A','L',' ','M','O','D','E'}
        #endif
    };
    usb_string_descriptor_struct usb_string_serial_number = {
    /*
     *  2 + SERIAL_NUMBER_LEN * 2,
     *  3,
     *  { SERIAL_NUMBER }
     */
        2 + 0 * 2,
        3,
        {}
    };
    /*
     *  CUSTOMIZE YOUR USB DESCRIPTION !!
     *  MANUFACTURER_NAME, PRODUCT_NAME, SERIAL_NUMBER !
     *
     *  And you can also edit VID, PID, etc. from usb_desc.h
     *  Arduino 2.0.x below : YourArduinoFolder\hardware\teensy\avr\cores\teensy4\usb_desc.h
     *  Arduino 2.0.x above : \Appdata\Local\Arduino15\packages\teensy\hardware\avr\1.57.2\cores\teensy4\usb_desc.h
     *
     *  usb_desc.h file's
     *  > #elif defined(USB_KEYBOARDONLY)    ◀ [USB Type: "Keyboard"]
     *  > #elif defined(USB_SERIAL_HID)      ◀ [USB Type: "Serial + Keyboard + Mouse + Joystick"]
     */
#ifdef __cplusplus
}
#endif





//      ___! Teensy® 4.1's PIN MAP !___

//      ____GND____
//      PIN_                            00
//      PIN_                            01
//      PIN_                            02
//      PIN_                            03
//      PIN_                            04
//      PIN_                            05
//      PIN_                            06
//      PIN_                            07
//      PIN_                            08
//      PIN_                            09
//      PIN_                            10
//      PIN_                            11
//      PIN_                            12
//      ____3.3____
//      PIN_                            A10 // 24
//      PIN_                            A11 // 25
//      PIN_                            A12 // 26
//      PIN_                            A13 // 27
//      PIN_                            28
//      PIN_                            29
//      PIN_                            30
//      PIN_                            31
//      PIN_                            32

//      ____Vin____ (3.6 to 5.5)
//      ____GND____
//      ____3.3____ (250mA max)
//      PIN_DARKJUNCTION_DIGITAL_RX     A9 // 23
//      PIN_DARKJUNCTION_DIGITAL_TX     A8 // 22
//      PIN_DARKJUNCTION_SERIAL_RX      A7 // 21
//      PIN_DARKJUNCTION_SERIAL_TX      A6 // 20
//      PIN_TO_ACCOMPLICE_WAKEUP        A5 // 19
//      PIN_                            A4 // 18
//      PIN_                            A3 // 17
//      PIN_                            A2 // 16
//      PIN_                            A1 // 15
//      PIN_                            A0 // 14
#define PIN_LED_BUILTIN                 13
//      ____GND____
//      PIN_BUZZER                      A17 // 41
#define PIN_RANDOMSEED                  A16 // 40
//      PIN_                            A15 // 39
//      PIN_                            A14 // 38
//      PIN_                            37
//      PIN_                            36
//      PIN_                            35
//      PIN_                            34
//      PIN_                            33

// determine whether to use Serial&SD or not.
// false : if you DO NOT WANT TO USE IT any circumstances
bool isDEBUG = true, isExistSD = true;

// Teensy 4.1's Timer lib
IntervalTimer IntervalTimer_per1ms;




#ifndef KEYBOARDHIJACKER_INCLUDE_THINGS
#define KEYBOARDHIJACKER_INCLUDE_THINGS

#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include <SD.h>
File textfile;

/**⦓ FOR, CONTROL TEENSY ⦔**/
#include "res_utility.h"
#include "res_layouts.h"

/**⦓ For, USER EXPERIENCE ⦔**/
#include "Buzzzzer.h"

/**⦓ For, EDIT FILES using WIFI ⦔**/
#include "DarkJunction.h"

/**⦓ For, SNATCH THE KEY EVENT FROM SLAVE KEYBOARD ⦔**/
#include "KeyboardParser.h"

#endif // KEYBOARDHIJACKER_INCLUDE_THINGS

/**⦓ For, TRANSMIT THE HIJACKED KEY EVENT TO HOST(OS) ⦔**/
#include "KeyboardHijacker.h"





void setup()
{
    /* -------------------------------------- Initializing of Teensy® 4.1's Physical Modules -------------------------------------- */
    {
        pinMode(PIN_LED_BUILTIN, OUTPUT);
        randomSeed(analogRead(PIN_RANDOMSEED));


        // SERIAL CHECK
        if(isDEBUG)
        {
            Serial.begin(115200);
            delay(5252);
            
            if(!Serial) { isDEBUG = false; Serial.end(); }
        }
        if(isDEBUG) Serial.println(F("SERIAL IS ONLINE\n"));


        // USB HOST on Teensy® 4.1
        USBHostOnTeensy.begin();
        if(isDEBUG) Serial.println(F("USB HOST IS ONLINE\n"));


        // SD CARD
        if(isExistSD)
        {
            if (SD.begin(BUILTIN_SDCARD))
            { isExistSD = true; if(isDEBUG) Serial.println(F("SD CARD IS AVAILABLE :)\n")); }
            else
            { isExistSD = false; if(isDEBUG) Serial.println(F("SD CARD IS NOT AVAILABLE :(\n")); }
        }


        // ESP8266 a.k.a. ACCOMPLICE
        DECLARE_DARKJUNCTION_PINS();
        MODULE_ACCOMPLICE_PUTTOSLEEP();

        DarkJunction::CONFIGURE_REQUEST_HANDLER (
                                                    /*  function PROCESS_REQUEST  */[](String strRequest) -> int8_t
                                                    {
                                                        if(!isExistSD)
                                                            return DarkJunction_REQUEST_ERRORED_NotExistSD;


                                                        // if activated MACRO_EVENT exists, SHUTDOWN!
                                                        KBD_HIJACKER.MODULE_MACRO_END_RECORDER( String(textfile.name()).c_str() );
                                                        KBD_HIJACKER.MODULE_MACRO_END_PLAYER();


                                                        static File dir, fileRequested;
                                                        if(dir)
                                                            dir.close();
                                                        if(fileRequested)
                                                            fileRequested.close();


                                                        if(isContains(strRequest,"PULL_ROOT_DATA"))
                                                        {
                                                            dir = SD.open("/");
                                                            File entry;

                                                            String strSend = "";
                                                            String strRemainder = "";

                                                            while(entry = dir.openNextFile())
                                                            {
                                                                if(entry.isDirectory())
                                                                    continue;

                                                                // LOAD entry's info
                                                                {
                                                                    strSend += String(entry.name());
                                                                    strSend += "\t";

                                                                    char buf[20];
                                                                    sprintf(buf, "%llu", entry.size());
                                                                    strSend += String(buf);
                                                                    strSend += "\n";

                                                                    entry.close();
                                                                }

                                                                // PREVENT Buffer Overflow
                                                                while(strSend.length() > UPDOWNLOAD_MAX_LENGTH)
                                                                {
                                                                    strRemainder = strSend.substring(UPDOWNLOAD_MAX_LENGTH);
                                                                    strSend      = strSend.substring(0, UPDOWNLOAD_MAX_LENGTH);

                                                                    // SEND MESSAGE
                                                                    bool isUploadSuccessful = DarkJunction::S3R14L_upload(&strSend);
                                                                    if(!isUploadSuccessful)
                                                                        return DarkJunction_REQUEST_ERRORED_FailedUpload;

                                                                    // WAIT FOR NEXT REQUEST
                                                                    bool isNextRequested = DarkJunction::detectHIGHForXXms(500, true);
                                                                    if(!isNextRequested)
                                                                        return DarkJunction_REQUEST_ERRORED_FailedPolling;

                                                                    strSend      = strRemainder;
                                                                    strRemainder = "";
                                                                }
                                                            }

                                                            if(strSend.length() > 0)
                                                            {
                                                                // SEND MESSAGE
                                                                bool isUploadSuccessful = DarkJunction::S3R14L_upload(&strSend);
                                                                if(!isUploadSuccessful)
                                                                    return DarkJunction_REQUEST_ERRORED_FailedUpload;

                                                                // WAIT FOR NEXT REQUEST
                                                                bool isNextRequested = DarkJunction::detectHIGHForXXms(500, true);
                                                                if(!isNextRequested)
                                                                    return DarkJunction_REQUEST_ERRORED_FailedPolling;
                                                            }

                                                            dir.close();

                                                            // SEND 'TRANSMISSION_COMPLETED' MESSAGE
                                                            bool isUploadSuccessful = DarkJunction::S3R14L_upload(&TRANSMISSION_COMPLETED);
                                                            if(!isUploadSuccessful)
                                                                return DarkJunction_REQUEST_ERRORED_FailedUpload;

                                                            return DarkJunction_REQUEST_SUCCESS;
                                                        }
                                                        else
                                                        if(isContains(strRequest,"PULL_FILE")) 
                                                        {
                                                            String strFilenameRequested = "";
                                                            {
                                                                int32_t index_Start = strRequest.indexOf("\"");
                                                                int32_t index_End = strRequest.lastIndexOf("\"");
                                                                strFilenameRequested = ( index_Start<0 || index_Start>=index_End ) ? "" : strRequest.substring(index_Start+1,index_End);
                                                            }
                                                            fileRequested = SD.open( strFilenameRequested.c_str() );

                                                            String strSend = "";
                                                            String strRemainder = "";


                                                            if (fileRequested)
                                                            {
                                                                while (fileRequested.available())
                                                                {
                                                                    strSend += fileRequested.readStringUntil('\n');

                                                                    // PREVENT Buffer Overflow
                                                                    while(strSend.length() > UPDOWNLOAD_MAX_LENGTH)
                                                                    {
                                                                        strRemainder = strSend.substring(UPDOWNLOAD_MAX_LENGTH);
                                                                        strSend      = strSend.substring(0, UPDOWNLOAD_MAX_LENGTH);

                                                                        // SEND MESSAGE
                                                                        bool isUploadSuccessful = DarkJunction::S3R14L_upload(&strSend);
                                                                        if(!isUploadSuccessful)
                                                                            return DarkJunction_REQUEST_ERRORED_FailedUpload;

                                                                        // WAIT FOR NEXT REQUEST
                                                                        bool isNextRequested = DarkJunction::detectHIGHForXXms(500, true);
                                                                        if(!isNextRequested)
                                                                            return DarkJunction_REQUEST_ERRORED_FailedPolling;

                                                                        strSend      = strRemainder;
                                                                        strRemainder = "";
                                                                    }
                                                                }

                                                                if(strSend.length() > 0)
                                                                {
                                                                    // SEND MESSAGE
                                                                    bool isUploadSuccessful = DarkJunction::S3R14L_upload(&strSend);
                                                                    if(!isUploadSuccessful)
                                                                        return DarkJunction_REQUEST_ERRORED_FailedUpload;

                                                                    // WAIT FOR NEXT REQUEST
                                                                    bool isNextRequested = DarkJunction::detectHIGHForXXms(500, true);
                                                                    if(!isNextRequested)
                                                                        return DarkJunction_REQUEST_ERRORED_FailedPolling;
                                                                }

                                                                fileRequested.close();
                                                            }
                                                            else
                                                                return DarkJunction_REQUEST_ERRORED_NotExistFile;


                                                            // SEND 'TRANSMISSION_COMPLETED' MESSAGE
                                                            bool isUploadSuccessful = DarkJunction::S3R14L_upload(&TRANSMISSION_COMPLETED);
                                                            if(!isUploadSuccessful)
                                                                return DarkJunction_REQUEST_ERRORED_FailedUpload;


                                                            return DarkJunction_REQUEST_SUCCESS;
                                                        }
                                                        else
                                                        if(isContains(strRequest,"PUSH_FILE"))
                                                        {
                                                            String strFilenameRequested = "";
                                                            {
                                                                int32_t index_Start = strRequest.indexOf("\"");
                                                                int32_t index_End = strRequest.lastIndexOf("\"");
                                                                strFilenameRequested = ( index_Start<0 || index_Start>=index_End ) ? "" : strRequest.substring(index_Start+1,index_End);
                                                            }
                                                            String strFilenameTemporary = "PUSHED.TEMP";


                                                            if( SD.exists( strFilenameTemporary.c_str() ) )
                                                                SD.remove( strFilenameTemporary.c_str() );
                                                            fileRequested = SD.open( strFilenameTemporary.c_str(), FILE_WRITE );

                                                            String strSend = "";
                                                            String strRemainder = "";


                                                            if (fileRequested)
                                                            {
                                                                while(true)
                                                                {
                                                                    DarkJunction::writeHIGHForXXms(50);

                                                                    bool isDownloadSuccessful = DarkJunction::S3R14L_download();
                                                                    if(!isDownloadSuccessful)
                                                                        return DarkJunction_REQUEST_ERRORED_FailedDownload;


                                                                    if(!isContains(DarkJunction::getMessage(),"7R4N5M15510N_C0MPL373D"))
                                                                    {
                                                                        uint16_t start = 0;
                                                                        while (start < DarkJunction::getMessage().length())
                                                                        {
                                                                            int16_t index_LF = DarkJunction::getMessage().indexOf('\n', start);

                                                                            if (index_LF > -1) {
                                                                                fileRequested.println   ( DarkJunction::getMessage().substring(start, index_LF) );
                                                                                start = index_LF + 1;
                                                                            } else {
                                                                                fileRequested.print     ( DarkJunction::getMessage().substring(start) );
                                                                                break;
                                                                            }
                                                                        }
                                                                    }
                                                                    else
                                                                        break;
                                                                }
                                                                fileRequested.close();


                                                                if( SD.exists( strFilenameRequested.c_str() ) )
                                                                    SD.remove( strFilenameRequested.c_str() );
                                                                SD.rename( strFilenameTemporary.c_str(), strFilenameRequested.c_str() );
                                                            }
                                                            else
                                                                return DarkJunction_REQUEST_ERRORED_NotExistFile;


                                                            return DarkJunction_REQUEST_SUCCESS;
                                                        }
                                                        else
                                                        if(isContains(strRequest,"CREATE_FILE"))
                                                        {
                                                            String strFilenameRequested = "";
                                                            {
                                                                int32_t index_Start = strRequest.indexOf("\"");
                                                                int32_t index_End = strRequest.lastIndexOf("\"");
                                                                strFilenameRequested = ( index_Start<0 || index_Start>=index_End ) ? "" : strRequest.substring(index_Start+1,index_End);
                                                            }

                                                            if( !SD.exists( strFilenameRequested.c_str() ) ){
                                                                fileRequested = SD.open( strFilenameRequested.c_str(), FILE_WRITE );
                                                                fileRequested.close();
                                                            }

                                                            return DarkJunction_REQUEST_SUCCESS;
                                                        }
                                                        else
                                                        if(isContains(strRequest,"DELETE_FILE"))
                                                        {
                                                            String strFilenameRequested = "";
                                                            {
                                                                int32_t index_Start = strRequest.indexOf("\"");
                                                                int32_t index_End = strRequest.lastIndexOf("\"");
                                                                strFilenameRequested = ( index_Start<0 || index_Start>=index_End ) ? "" : strRequest.substring(index_Start+1,index_End);
                                                            }

                                                            if( SD.exists( strFilenameRequested.c_str() ) )
                                                                SD.remove( strFilenameRequested.c_str() );
                                                            else
                                                                return DarkJunction_REQUEST_ERRORED_NotExistFile;

                                                            return DarkJunction_REQUEST_SUCCESS;
                                                        }
                                                        else
                                                        if(isContains(strRequest,"RENAME_FILE"))
                                                        {
                                                            String strFilenameRequested1 = strRequest.substring(0, strRequest.indexOf("|"));
                                                            {
                                                                int32_t index_Start = strFilenameRequested1.indexOf("\"");
                                                                int32_t index_End = strFilenameRequested1.lastIndexOf("\"");
                                                                strFilenameRequested1 = ( index_Start<0 || index_Start>=index_End ) ? "" : strFilenameRequested1.substring(index_Start+1,index_End);
                                                            }
                                                            String strFilenameRequested2 = strRequest.substring(strRequest.indexOf("|"));
                                                            {
                                                                int32_t index_Start = strFilenameRequested2.indexOf("\"");
                                                                int32_t index_End = strFilenameRequested2.lastIndexOf("\"");
                                                                strFilenameRequested2 = ( index_Start<0 || index_Start>=index_End ) ? "" : strFilenameRequested2.substring(index_Start+1,index_End);
                                                            }

                                                            if( SD.exists( strFilenameRequested1.c_str() ) && !SD.exists( strFilenameRequested2.c_str() ) )
                                                                SD.rename( strFilenameRequested1.c_str(), strFilenameRequested2.c_str() );
                                                            else
                                                                return DarkJunction_REQUEST_ERRORED_NotExistFile;

                                                            return DarkJunction_REQUEST_SUCCESS;
                                                        }
                                                        else // Unknown REQUEST
                                                        {
                                                            DarkJunction::S3R14L_upload(&ERRORED);
                                                            return DarkJunction_REQUEST_ERRORED_UnknownRequest;
                                                        }
                                                    }
                                                );


        // PIEZO BUZZER
        Buzzzzer::reserveBuzz   ( { NOTE_B6,0,  NOTE_E7,0   }
                                , { 111,11,     444,111     } );
        if(isDEBUG) Serial.println(F("BUZZER PLAYS STARTUP SOUND ♬\n"));

        if(isDEBUG) Serial.println(F("ALL PHYSICAL MODULES ARE INITIALIZED !!\n\n\n"));
    }
    /* -------------------------------------- Initializing of Teensy® 4.1's Physical Modules -------------------------------------- */



    /* -------------------------------------- Initializing of Objects for Keyboard Hijacking -------------------------------------- */
    {
        // ATTACH Lamda Functions that calls when PRESS / RELEASE interrupt occurs
        KBD_PARSER.begin(
                            /*  function OnRawPress  */ [](uint8_t keycode)
                            {
                                isExistWaitingEvent_Press = true;

                                rawKeycode = keycode;
                                numDN++;


                                // LOG physical keycode
                                KBD_PARSER.KeyLogger.push(keycode);


                                // REBOOT COMMAND [ KEYPAD_SLASH + KEYPAD_0 + KEYPAD_8 ] or [ KEYPAD_0 + KEYPAD_SLASH + KEYPAD_8 ]
                                if(keycode==KEYCODE_KEYPAD_8 && numDN==3)
                                {
                                    if( (KBD_PARSER.KeyLogger.peek_keycode(1)==KEYCODE_KEYPAD_SLASH && KBD_PARSER.KeyLogger.peek_keycode(2)==KEYCODE_KEYPAD_0) ||
                                        (KBD_PARSER.KeyLogger.peek_keycode(1)==KEYCODE_KEYPAD_0     && KBD_PARSER.KeyLogger.peek_keycode(2)==KEYCODE_KEYPAD_SLASH) )
                                    {   isReservedReboot=true; isExistWaitingEvent_Press=false; return;   }
                                }


                                // MODULE_MACRO's
                                if(isExistSD)
                                {
                                    // if MACRO is PLAYING
                                    KBD_HIJACKER.MODULE_MACRO_MANUAL_EVENT_DETECTED();

                                    // if MACRO is RECORDING
                                    KBD_HIJACKER.MODULE_MACRO_PROCEED_RECORDER(keycode, MILLIS_SINCE_LATEST_EVENT, true);

                                    // block several events when READONLY MACRO just started
                                    KBD_HIJACKER.MODULE_MACRO_BLOCK_SEVERAL_EVENTS_WHEN_READONLY_STARTED(&isExistWaitingEvent_Press);
                                }


                                // For, Debugging
                                if(isDEBUG)
                                {
                                    MEASURE_FREE_MEMORY();
                                    
                                    Serial.print(F("\n(Before Hijack) DN ")); KBD_HIJACKER.printKeyInfo(keycode);
                                    Serial.println();
                                }
                            },
                            /* function OnRawRelease */ [](uint8_t keycode)
                            {
                                if(numDN==0) // invalid Event
                                    return;


                                isExistWaitingEvent_Release = true;

                                rawKeycode = keycode;
                                numDN--;


                                // MODULE_MACRO's
                                if(isExistSD)
                                {
                                    // if MACRO is PLAYING
                                    KBD_HIJACKER.MODULE_MACRO_MANUAL_EVENT_DETECTED();

                                    // if MACRO is RECORDING
                                    KBD_HIJACKER.MODULE_MACRO_PROCEED_RECORDER(keycode, MILLIS_SINCE_LATEST_EVENT, false);

                                    // block several events when READONLY MACRO just started
                                    KBD_HIJACKER.MODULE_MACRO_BLOCK_SEVERAL_EVENTS_WHEN_READONLY_STARTED(&isExistWaitingEvent_Release);
                                }


                                // For, Debugging
                                if(isDEBUG)
                                {
                                    MEASURE_FREE_MEMORY();
                                    
                                    Serial.print(F("\n(Before Hijack) UP ")); KBD_HIJACKER.printKeyInfo(keycode);
                                    Serial.print(F("         Pressed Time : ")); Serial.println(MILLIS_FROM_PRESSED_UNTIL_RELEASE);
                                }
                            }
                        );
        if(isDEBUG) Serial.println(F("KEYBOARD PARSER's INTERRUPTS HAVE BEEN CONFIGURED\n"));


        // GET READY FOR HIJACKING
        KBD_HIJACKER.begin();
        if(isDEBUG) Serial.println(F("KEYBOARD HIJACKER IS ON STANDBY\n"));


        // CONFIGURE 1ms INTERVAL TIMER INTERRUPT
        IntervalTimer_per1ms.begin  (
                                        []
                                        {
                                            static bool isDormancy = false;


                                            // per1ms!
                                            {
                                                Buzzzzer::playBuzz();


                                                if(isExistSD)
                                                    KBD_HIJACKER.MODULE_MACRO_PROCEED_PLAYER();


                                                static int8_t countdownSyncTKS = -1;
                                                if(countdownSyncTKS>-1)
                                                {
                                                    countdownSyncTKS--;
                                                    if(countdownSyncTKS==0) KBD_HIJACKER.syncToggleKeyStates();
                                                }
                                                if(KBD_HIJACKER.isReservedSyncToggleKeyStates)
                                                {
                                                    countdownSyncTKS=77;
                                                    KBD_HIJACKER.isReservedSyncToggleKeyStates = false;
                                                }
                                            }


                                            // per10ms!
                                            if( !(millis()%10) )
                                            {
                                                KBD_HIJACKER.MODULE_KEYMAPPER_RAPIDFIRE();
                                            }


                                            // per10000ms!
                                            if( !(millis()%10000) )
                                            {
                                                isDormancy = (MILLIS_SINCE_LATEST_EVENT)>33333 ? true : false;

                                                if(isDormancy) KBD_HIJACKER.syncToggleKeyStates();
                                            }
                                        },
                                        1000
                                    );
        if(isDEBUG) Serial.println(F("1ms INTERVAL TIMER INTERRUPT HAS BEEN CONFIGURED\n"));


        if(isDEBUG) Serial.println(F("ALL OBJECTS ARE INITIALIZED !!\n\n\n"));
    }
    /* -------------------------------------- Initializing of Objects for Keyboard Hijacking -------------------------------------- */


    if(isDEBUG) Serial.println(F("OPERATION COMPLETE, SIR ! XD\n\n\n\n"));
}



void loop()
{
    USBHostOnTeensy.Task();


    if(isReservedReboot)
    {
        KBD_HIJACKER.releaseAllBeingHoldDownKey();
        while(numDN){ delay(1); }
        REBOOT();
    }


    if(isExistWaitingEvent_Press)
    {
        key   = keycode_To_TeensyLayout( rawKeycode );
        event = true;
        msLatestEventCame = msLatestEventPressed = millis();

        KBD_HIJACKER.TRANSMIT_AFTER_HIJACK();

        isExistWaitingEvent_Press   = false;
    }


    if(isExistWaitingEvent_Release)
    {
        key   = keycode_To_TeensyLayout( rawKeycode );
        event = false;
        msLatestEventCame = millis();

        KBD_HIJACKER.TRANSMIT_AFTER_HIJACK();

        isExistWaitingEvent_Release = false;
    }


    numDN>0 ? digitalWrite(PIN_LED_BUILTIN, HIGH) : digitalWrite(PIN_LED_BUILTIN, LOW);


    if(DarkJunction::STATE != DarkJunction_STATE_DISCONNECTED)
    {
        DarkJunction::HANDLE_REQUEST();

        if(millis() - DarkJunction::msLatestFunctioned > 600000)
        {   DarkJunction::SHUTDOWN(); MODULE_ACCOMPLICE_PUTTOSLEEP();   }
    }
}
