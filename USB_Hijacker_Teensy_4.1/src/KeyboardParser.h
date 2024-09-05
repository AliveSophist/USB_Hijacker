#pragma once



#include <USBHost_t36.h>
USBHost USBHostOnTeensy;
USBHub Hub_0(USBHostOnTeensy), Hub_1(USBHostOnTeensy), Hub_2(USBHostOnTeensy), Hub_3(USBHostOnTeensy), Hub_4(USBHostOnTeensy), Hub_5(USBHostOnTeensy), Hub_6(USBHostOnTeensy), Hub_7(USBHostOnTeensy);



volatile uint8_t rawKeycode;

volatile uint8_t numDN = 0;

volatile bool isExistWaitingEvent_Press   = false;
volatile bool isExistWaitingEvent_Release = false;

class KeyboardParser : public KeyboardController
{
/*********************************** SINGLETON ***********************************/
private:
    // SET private Constructor & Destructor
    KeyboardParser(USBHost usbhost) : KeyboardController(usbhost) {}
    ~KeyboardParser() {}
    // Delete copy constructor to prevent copying
    KeyboardParser(const KeyboardParser& ref) = delete;
    // Delete copy assignment operator to prevent copying
    KeyboardParser& operator=(const KeyboardParser& ref) = delete;

public:
    // STATIC SINGLETON INSTANCE !!
    static KeyboardParser& getInstance()
    {
        static KeyboardParser instance(USBHostOnTeensy);
        return instance;
    }
/*********************************** SINGLETON ***********************************/

public:
    #define KEYLOGGER_LEN_MAX 10
    struct DoublyLinkedStackKeyLogger
    {
        struct nodeKeycode
        {
            uint8_t keycode;
            uint32_t millis;

            nodeKeycode* next;
            nodeKeycode* prev;
        } *top, *bottom;
        uint32_t len;

        DoublyLinkedStackKeyLogger() // Initialize
        {
            top = bottom = NULL;
            len = 0;
        }

        bool size()         { return len; }
        bool isEmpty()      { return (len==0); }
        bool isOverflow()   { return (len > KEYLOGGER_LEN_MAX); }

        void push(uint8_t keycode)
        {
            nodeKeycode* newNode = (nodeKeycode*)malloc(sizeof(nodeKeycode));
            newNode->keycode = keycode;
            newNode->millis = millis();
            newNode->prev = NULL;

            if(isEmpty()){
                newNode->next = NULL;
                top = bottom = newNode;
            }
            else{
                newNode->next = top;
                top->prev = newNode;
                top = newNode;
            }
            len++;

            // if overflowed, delete oldest node
            if(isOverflow()){
                nodeKeycode* delNode = bottom;
                bottom = bottom->prev;
                bottom->next = NULL;
                free(delNode);
                len--;
            }
        }
        void pop()
        {
            nodeKeycode* delNode = top;
            top = top->next;
            top->prev = NULL;
            free(delNode);
            len--;
        }
        uint8_t peek_keycode(uint32_t num)
        {
            nodeKeycode* readNode = top;
            for(uint32_t i=0; i<num; i++)
                readNode = readNode->next;

            return (readNode->keycode);
        }
        uint32_t peek_millis(uint32_t num)
        {
            nodeKeycode* readNode = top;
            for(uint32_t i=0; i<num; i++)
                readNode = readNode->next;

            return (readNode->millis);
        }

        void PrintAll() // For, Debugging
        {
            uint32_t num=0;
            Serial.println();
            while(num<len)
            {
                nodeKeycode* readNode = top;
                
                for(uint32_t i=0; i<num; i++)
                    readNode = readNode->next;
                    
                Serial.print("node "); Serial.println(num);
                Serial.print("keycode : "); Serial.println((int)readNode->keycode);
                Serial.print("millis  : "); Serial.println((int)readNode->millis);
                Serial.print("*prev : ");
                (readNode->prev==NULL) ? Serial.println("NULL") : Serial.println((int)readNode->prev);
                Serial.print(" this : "); Serial.println((int)readNode);
                Serial.print("*next : ");
                (readNode->next==NULL) ? Serial.println("NULL") : Serial.println((int)readNode->next);
                Serial.println();
                
                num++;
            }
        }
    } KeyLogger;

    void begin(auto OnRawPress, auto OnRawRelease) // Initialize
    {
        this->attachRawPress(OnRawPress);
        this->attachRawRelease(OnRawRelease);
    }
}
&KBD_PARSER = KeyboardParser::getInstance(); // refers to the SINGLETON
