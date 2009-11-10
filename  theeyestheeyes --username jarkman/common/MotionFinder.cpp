#include "stdafx.h"

#include "mbed.h"
#include "Frame.h"
#include "MotionFinder.h"

#include "Servo.h"

Servo myServo (p21);
extern Logger pcSerial;

// Motion detection for the mbed


MotionFinder::MotionFinder()
{
	m_backgroundFrame = NULL;
	m_resultFrame = NULL;
}

	

MotionFinder::~MotionFinder()
{
	if( m_backgroundFrame != NULL )
		Frame::releaseFrame( & m_backgroundFrame );

	if( m_resultFrame != NULL )
		Frame::releaseFrame( & m_resultFrame );

}

void MotionFinder::newBackground( Frame *frame )
{
	Frame::releaseFrame( & m_backgroundFrame );
	return processFrame( frame );
}

void MotionFinder::processFrame( Frame *frame )
{
    if( m_backgroundFrame == NULL )
    {
        m_backgroundFrame = frame;
        Frame::cloneFrame( &m_resultFrame, m_backgroundFrame );
        return;
    }

    uint32_t sumX = 0;
    uint32_t sumY = 0;
    uint32_t sumN = 0;
    uint16_t x = 0, y = 0;

    for( uint32_t i = 0; i < frame->m_numPixels; i += 1 )
    {
        x ++;
        if( x >= frame->m_width )
        {
            y++;
            x = 0;
        }

        uint16_t pb = m_backgroundFrame->getPixel( i );
        uint16_t pf = frame->getPixel( i );

        if( ( pf > pb && pf - pb > 10 ) || ( pf < pb && pb - pf > 10 ))
        {
            // different from background
            m_resultFrame->setPixel( i, pf );
            sumX += x;
            sumY += y;
            sumN ++;

        }
        else
        {
            // same-ish as background
            m_resultFrame->setPixel( i, 0 );


        }
        
        //and make the background a little bit more like this pixel, to adjust to slow changes in lighting
        if( pf > pb )
            m_backgroundFrame->setPixel( i, pb +1 );
                
        if( pf < pb )
            m_backgroundFrame->setPixel( i, pb - 1 );

    }

    uint32_t cogX = 0;
    uint32_t cogY = 0;

	if( frame->m_numPixels < 1 )
		frame->m_numPixels = 1;

    uint32_t percentage = (sumN * 100) / frame->m_numPixels;
    
    pcSerial.printf("\r\n%d %% changed pixels\r\n", (int) percentage);
    
    if( percentage < 5 ) // no real target, no COG
    {
        pcSerial.printf("No COG\r\n");
    }
    else if( sumN > 0 )
    {
        cogX = sumX / sumN;
        cogY = sumY / sumN;

        myServo = (cogX  / 80.0);
        
        pcSerial.printf("COG is %d, %d\r\n", (int) cogX, (int) cogY);

    }


    Frame::releaseFrame( &frame );

    return;
}


