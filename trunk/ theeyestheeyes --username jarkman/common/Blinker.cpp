#include "stdafx.h"

#include "mbed.h"
#include "Blinker.h"

#define EYE_CLOSED 0.25
#define EYE_OPEN 0.85

extern Logger pcSerial;

Blinker::Blinker( ServoMinder *servoMinder )
{
    m_servoMinder = servoMinder;
    m_servoMinder->moveTo( EYE_CLOSED );
    
    m_sleepiness = 0;
    m_boredom = 0;
    
    m_tickTime = 0.1;
    m_blinkTimer = 0;
    
    m_nextMove = -1;


    m_ticker.attach( this, &Blinker::tick, m_tickTime );
}

void Blinker::setBoredom( float boredom )
{
    m_boredom = boredom;
}

void Blinker::setSleepiness( float sleepiness )
{
    float diff = fabs( m_sleepiness - sleepiness );
    m_sleepiness = sleepiness;
    
    if( diff > 0.01 )
        open();
   
}
    
float  Blinker::speedForSleepiness()
{
    // for sleepiness 0->1, return speed 2->0.5
    return 2 - (1.5 * m_sleepiness);
}

float  Blinker::openPosForSleepiness()
{
    // for sleepiness 0->1, return EYE_OPEN ->( )/2
    return  EYE_OPEN +  ( (EYE_CLOSED-EYE_OPEN) * 0.8 * m_sleepiness);
}

void Blinker::close()
{
    m_servoMinder->setSpeed( speedForSleepiness() );
    m_servoMinder->moveTo( EYE_CLOSED );

}
   
 void Blinker::open()
{
    m_servoMinder->setSpeed( speedForSleepiness() );
    m_servoMinder->moveTo(  openPosForSleepiness() );
} 

void Blinker::blink()
{
    m_servoMinder->setSpeed( speedForSleepiness() );
    m_servoMinder->moveTo( EYE_CLOSED );
    m_nextMove = openPosForSleepiness();
}

void Blinker::tick()
{

    if( ! m_servoMinder->isMoving())
        if( m_nextMove >= 0 )
        {
             m_servoMinder->moveTo( m_nextMove );
             m_nextMove = -1;
        }
        
    m_blinkTimer ++;
    if( m_blinkTimer > 100 ) // 10 secs
    {
        m_blinkTimer = 0;
        blink();
    }
    
    if( m_boredom < m_sleepiness )
        setSleepiness(  m_boredom ); // wake up quickly
    else    
        setSleepiness( m_sleepiness + (m_boredom - m_sleepiness) / 200 ); // fall asleep slowly
}

