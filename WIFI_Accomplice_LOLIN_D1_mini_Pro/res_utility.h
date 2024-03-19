void MEASURE_FREE_MEMORY() // for Teensy® 4.1 =32bit!!
{   if(!isDEBUG)
        return;


    Serial.print("\nNow Measured FreeMemory : "); Serial.println(ESP.getFreeHeap()); //=__brkval;
}



volatile uint32_t msMeasuredForDebugging;
#define MEASURE_MILLIS_START_POINT()    { msMeasuredForDebugging=millis(); }
#define MEASURE_MILLIS_END_POINT()      { if(isDEBUG){ Serial.println("\n#\nMeasured Time : " + String(millis()-msMeasuredForDebugging)); Serial.println("#\n"); } }



#define isEquals(STRX,STRY)     (strcmp(STRX,STRY)==0)
#define fillWith(STRX,STRY)     strcpy(STRX,STRY)
#define isContains(STR,CHSTR)   ((STR).indexOf(CHSTR)>-1)