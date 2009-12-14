#pragma once

#include "ServoMinder.h"
class Blinker
{
public:
    Blinker( ServoMinder *servoMinder );

    void blink();
    void close();
    void open();
    void setSleepiness( float sleepiness );
    void setBoredom( float boredom );
     
private:
   

    ServoMinder *m_servoMinder;
    
    float m_boredom;
    float m_sleepiness;
    float m_tickTime;
    Ticker m_ticker;
    float m_nextMove;
    
    float m_blinkTimer;

    float speedForSleepiness();
    float openPosForSleepiness();
    
    void tick();

};