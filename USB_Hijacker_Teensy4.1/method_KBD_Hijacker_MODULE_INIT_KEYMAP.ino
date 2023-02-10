
struct MappedData {
    uint8_t mappedLen;
    uint8_t* mappedKeycodes;
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
                Serial.println();
                Serial.print("MAPPED keycode : "); Serial.println(keycode);
                Serial.print("mappedTXT : "); Serial.println(mapThings);
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
                for(uint8_t i=0; i<mappedLen; i++){
                    mappedKeycodes[i] = String_To_keycode( mapThings );
    
                    if(mappedKeycodes[i] == 0){
                        free(mappedKeycodes);
                        textfile.close();
                        
                        //shutdown initializing
                        if(isSerial) Serial.println("\n!!! MODULE_INITIALIZE_KEYMAP BE STOPPED !!! SYNTAX ERROR !!!");
                        return;
                    }
                    
                    if(mapThings.indexOf('+')>0)
                        mapThings = mapThings.substring(mapThings.indexOf('+'));
                }
        
                // Create a MappedData object and assign values
                MappedData data;
                data.mappedLen = mappedLen;
                data.mappedKeycodes = mappedKeycodes;
                keymap[keycode] = data;
    
                if(isSerial) 
                {
                    Serial.println();
                    Serial.print("MAPPED keycode : "); Serial.println(keycode);
                    Serial.print("mappedLen : "); Serial.println(data.mappedLen);
                    for(uint8_t i=0; i<mappedLen; i++){ Serial.print("mappedKey[");Serial.print(i);Serial.print("] : "); Serial.println(data.mappedKeycodes[i]); }
                }
            }
        }
    }
    
    textfile.close();
    
    Serial.println("\nMODULE_INITIALIZE_KEYMAP END\n"); 
}
