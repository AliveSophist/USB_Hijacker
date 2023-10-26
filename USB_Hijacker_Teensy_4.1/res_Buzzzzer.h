/*  
    First,
    Please insert this function into your '1ms interval Timer Interrupt'

        Buzzzzer::playBuzz();


    Then,
    Enjoy Buzzzzer! \>_</

    example)
        Buzzzzer::reserveBuzz   ( { NOTE_DS7,0, NOTE_DS6,0, NOTE_B6,0,  NOTE_A6,0,  NOTE_DS6,0, NOTE_DS7,0, NOTE_B6 }
                                , { 220,20,     130,20,     270,30,     230,20,     170,20,     270,20,     400     } );
 */

#define BUZZZZER_SHEET_MAX 128

namespace Buzzzzer
{
    uint16_t melody[BUZZZZER_SHEET_MAX] = {0};
    uint16_t rhythm[BUZZZZER_SHEET_MAX] = {0};
    uint16_t len = 0;
    uint16_t m_proc,r_proc;

    void reserveBuzz(std::initializer_list<uint16_t> m, std::initializer_list<uint16_t> r)
    {
        len = m.size();

        auto iteratorM = m.begin();
        auto iteratorR = r.begin();

        for (uint16_t i=0; i<len; i++) {
            melody[i] = *iteratorM++;
            rhythm[i] = *iteratorR++;
        }

        m_proc=r_proc=0;
    }
    void playBuzz()
    {
        if(len==0)
            return;
        
        if(m_proc<len)
            if(r_proc<rhythm[m_proc]){
                if(r_proc==0){
                    if(melody[m_proc]!=0)
                        tone(PIN_BUZZER,melody[m_proc],rhythm[m_proc]);
                    else
                        noTone(PIN_BUZZER);
                }
                r_proc++;
            }
            else{
                m_proc++;
                r_proc=0;
            }
        else
            len = 0;
    }
    void replayBuzz()
    {
        len = m_proc+1;

        m_proc=r_proc=0;
    }
}
