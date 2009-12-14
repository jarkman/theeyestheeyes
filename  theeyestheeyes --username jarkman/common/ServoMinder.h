#pragma once
#include "Servo.h"

class ServoMinder
{
public:
    ServoMinder( Servo *servo );

    void moveTo( float target );
    void moveToAndWait( float target );
     void setSpeed( float speed );
     bool isMoving();
     
private:
   

    Servo *m_servo;
    float m_target;
    float m_speed;

    float m_delta;
    float m_tickTime;
    Ticker m_ticker;

    void tick();

};