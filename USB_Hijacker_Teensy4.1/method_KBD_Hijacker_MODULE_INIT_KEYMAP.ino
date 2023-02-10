
static std::map<uint8_t, void*> keymap;

struct MappedData {
    uint8_t* mappedKeys;
    uint8_t mappedNum;
};

void MODULE_INITIALIZE_KEYMAP()
{
    if(!isExistSD)
        return;

    if(!SD.exists("MAPPER.TXT")){
        textfile = SD.open("MAPPER.TXT",FILE_WRITE);
        textfile.close();
        return;
    }



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

            //ignore 'TY ("")' inner strings
            if(readline[i] == '\"')
                break;
        }

        //uint8_t keycode = String_To_keycode( readline.substring(0, readline.lastIndexOf('[')) );
        
        String mapThings = readline.substring(readline.lastIndexOf('['),readline.lastIndexOf(']'));

        //uint8_t mappedKeycodes[]
        uint8_t mappedNum = 0;
        {
            char buf[mapThings.length()+1];
            mapThings.toCharArray(buf,mapThings.length()+1);

            char* s = buf;
            while(*s!='\0'){ if(*s=='+')mappedNum++; s++; }
        }




        
        //malloc 어쩌구 저쩌구
        
        //std::getline( iss, mapThings, '+' )
        
        Serial.print("mapThings : "); Serial.println(mapThings);
        Serial.print("mappedNum : "); Serial.println(mappedNum);
        Serial.println(""); 
    }

    textfile.close();

    
    Serial.println(""); 
    Serial.println(""); 
    Serial.println("INIT END"); 
}
