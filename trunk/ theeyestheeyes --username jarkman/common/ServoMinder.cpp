#include "stdafx.h"

#include "mbed.h"
#include "ServoMinder.h"


extern Logger pcSerial;

ServoMinder::ServoMinder( Servo *servo )
{
    m_servo = servo;
    m_target = 0;
    

    m_tickTime = 0.02;

    setSpeed( 0.25 ); // half-rotations per sec

    m_ticker.attach( this, &ServoMinder::tick, m_tickTime );
}

void ServoMinder::moveTo( float target )
{
    m_target = target;
    
    
}

void ServoMinder::setSpeed( float speed )
{
    m_speed = speed;
    m_delta = m_speed * m_tickTime;
}

void ServoMinder::tick()
{
    float pos = m_servo->read();

      
    if( pos < m_target  )
    {
        pos += m_delta;
        if( pos > m_target )
            pos = m_target;
        
        // can't trace in here - breaks the cam protocol
        //pcSerial.printf("servo to %f\r\n", pos);    
        
        if( pos < 0 )
            pos = 0;
        if( pos > 1.0 )
            pos = 1.0;
                
        m_servo->write( pos );
    }
    else if( pos > m_target )
    {
        pos -= m_delta;
        if( pos < m_target )
            pos = m_target;
            
        if( pos < 0 )
            pos = 0;
        if( pos > 1.0 )
            pos = 1.0;
        //pcSerial.printf("servo to %f\r\n", pos);    
        m_servo->write( pos );
    }

    
}

