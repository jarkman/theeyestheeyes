#include "stdafx.h"

#include "mbed.h"
#include "ServoMinder.h"

ServoMinder::ServoMinder( Servo *servo )
{
	m_servo = servo;
	m_target = 0;
	

	m_tickTime = 0.1;

	setSpeed( 2 ); // time in seconds for full travel

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
		if( pos > m_delta )
			pos = m_delta;
	}
	else if( pos > m_target )
	{
		pos -= m_delta;
		if( pos < m_target )
			pos = m_target;
	}

	m_servo->write( pos );
}

