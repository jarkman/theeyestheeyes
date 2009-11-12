#include "stdafx.h"

#include "mbed.h"
#include "Frame.h"
#include "ServoMinder.h"
#include "MotionFinder.h"


#include "ServoMinder.h"



extern Logger pcSerial;

// Motion detection for the mbed


MotionFinder::MotionFinder( ServoMinder *xServoMinder, ServoMinder *yServoMinder )
{
	m_backgroundFrame = NULL;
	m_resultFrame = NULL;

	m_xServoMinder = xServoMinder;
	m_yServoMinder = yServoMinder;
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
	if( frame == NULL )
    {
		m_resultFrame->m_bad = false;
        return;
    }


    if( m_backgroundFrame == NULL )
    {
        m_backgroundFrame = frame;

		m_delta = 1 << (m_backgroundFrame->m_bitsPerPixel - 4); // smallest interesting change - make sure this is nonzero for 4-bit iamges!

		if( m_delta < 2 )
			m_delta = 2;

        Frame::cloneFrame( &m_resultFrame, m_backgroundFrame );
		m_resultFrame->m_bad = false;
        return;
    }

	if( frame->m_numPixels != m_backgroundFrame->m_numPixels )
    {       
		m_resultFrame->m_bad = false;
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

        if( ( pf > pb && pf - pb > m_delta ) || ( pf < pb && pb - pf > m_delta ))
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
    
    pcSerial.printf("\r\n%d percent changed pixels\r\n", (int) percentage);
    
    if( percentage < 3 ) // no real target, no COG
    {
        pcSerial.printf("No COG\r\n");
    }
    else if( sumN > 0 )
    {
        cogX = sumX / sumN;
        cogY = sumY / sumN;

		m_xServoMinder->moveTo( (float)cogX  / frame->m_width);
		m_yServoMinder->moveTo( (float)cogY  / frame->m_width); // use the larger dimension so x & y get the same scaling


        pcSerial.printf("COG is %d, %d\r\n", (int) cogX, (int) cogY);

    }


    Frame::releaseFrame( &frame );

	m_resultFrame->m_bad = false;
    return;
}



