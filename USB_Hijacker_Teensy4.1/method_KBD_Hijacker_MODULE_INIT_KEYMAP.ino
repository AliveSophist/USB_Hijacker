
struct MappedData {
    uint8_t mappedLen;
    void* mappedThings;
};

static std::map<uint8_t, MappedData> keymap;

void MODULE_INITIALIZE_KEYMAP()
{
    if(!isExistSD)
        return;

    if(!SD.exists("MAPPER.TXT")){
        textfile = SD.open("MAPPER.TXT",FILE_WRITE);
        textfile.close();
        return;
    }


    Serial.println("\n\nMODULE_INITIALIZE_KEYMAP START"); 
    
    textfile = SD.open("MAPPER.TXT");
    String readline;

    while(textfile.available())
    {
        readline = textfile.readStringUntil('\n');
        
        //To uppercase
        for(uint32_t i=0; i<readline.length(); i++)
        {
            if('a' <= readline[i] && readline[i] <= 'z')
                readline[i] -= 32;
        }

        uint8_t keycode = String_To_keycode( readline.substring(0, readline.lastIndexOf('[')) );
        if(keycode != 0)
        {
            String mapThings = readline.substring(readline.indexOf('[')+1,readline.indexOf(']'));

            if(mapThings.indexOf(".TXT")>0)
            {
                //int8_t index_Start = readline.indexOf("\"");
                //int8_t index_End = readline.lastIndexOf(".TXT\"");
                int8_t index_Start = readline.indexOf(" ");
                int8_t index_End = readline.lastIndexOf(".TXT ");

                // When SYNTAX ERROR occured, Go to 'while'
                if(index_Start<0||index_Start>=index_End)
                { if(isSerial) Serial.println("\n!!! SYNTAX ERROR !!!"); continue; }

                String strFilename = readline.substring(index_Start+1,index_End);
                uint8_t strLength = strFilename.length();

                char* mappedFilename = (char*)malloc((strLength+1) * sizeof(char));
                strFilename.toCharArray(mappedFilename,strLength+1);
                mappedFilename[strLength] = '\0';



                
                Serial.println();
                Serial.print("MAPPED keycode : "); Serial.println(keycode);
                Serial.print("mappedTXT : "); Serial.println(mappedFilename);
                Serial.print("strLength : "); Serial.println(strLength);
            }
            else
            {
                uint8_t mappedLen = 1;
                {
                    char buf[mapThings.length()+1];
                    mapThings.toCharArray(buf,mapThings.length()+1);
        
                    char* s = buf;
                    while(*s!='\0'){ if(*s=='+')mappedLen++; s++; }
                }
                
                uint8_t* mappedKeycodes = (uint8_t*)malloc(mappedLen * sizeof(uint8_t));
                for(uint8_t i=0; i<mappedLen; i++)
                {
                    mappedKeycodes[i] = String_To_keycode( mapThings );
    
                    if(mappedKeycodes[i] == 0)
                    {
                        free(mappedKeycodes);
                        mappedKeycodes = NULL;
                        break;
                    }
                    
                    if(mapThings.indexOf('+')>0)
                        mapThings = mapThings.substring(mapThings.indexOf('+'));
                }
                
                // When SYNTAX ERROR occured, Go to 'while'
                if(mappedKeycodes==NULL)
                { if(isSerial) Serial.println("\n!!! SYNTAX ERROR !!!"); continue; }
        
                // Create a MappedData object and assign values
                MappedData data;
                data.mappedLen = mappedLen;
                data.mappedThings = mappedKeycodes;
                keymap[keycode] = data;

                if(isSerial) 
                {
                    Serial.println();
                    Serial.print("MAPPED keycode : "); Serial.println(keycode);
                    Serial.print("mappedLen : "); Serial.println(data.mappedLen);
                    for(uint8_t i=0; i<mappedLen; i++)
                    { 
                        Serial.print("mappedT[");Serial.print(i);Serial.print("] : "); Serial.print(  ((uint8_t*)data.mappedThings)[i]  ); // it's same '(reinterpret_cast<uint8_t*>(data.mappedThings))[i]'
                        Serial.print("  /  mappedK[");Serial.print(i);Serial.print("] : "); Serial.println(  mappedKeycodes[i]  ); 
                    } //Serial.println(  *((uint8_t*)data.mappedThings+(i * sizeof(uint8_t)))  );
                }
            }
        }
    }
    
    textfile.close();
    
    Serial.println("\nMODULE_INITIALIZE_KEYMAP END\n"); 
}
