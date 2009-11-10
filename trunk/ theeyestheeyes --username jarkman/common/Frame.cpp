#include "stdafx.h"

#include "mbed.h"
#include "Frame.h"

Frame* Frame::m_frames[MAX_FRAMES]; // array of reusable frames

Frame::Frame() // don't construct Frame yourself - call allocFrame instead
{
	m_pixels = NULL;
	m_width = 0;
	m_height = 0;
	m_pixelFormat = 0;
	m_deleted = true;
	m_numPixels = 0;
}


Frame::~Frame()
{
	if( m_pixels != NULL )
		free( m_pixels );

}

// static - initialise the array of frames - call once at app start time
void Frame::initFrames()
{
	for( int f = 0; f < MAX_FRAMES; f ++ )
		m_frames[f] = NULL;
}

//static - reuse an existing frame, or make a new one
void Frame::allocFrame( Frame **frame, uint8_t pixelFormat, uint16_t width, uint16_t height, uint16_t frameSize )
{
	for( int f = 0; f < MAX_FRAMES; f ++ )
		if( m_frames[f] == NULL )
		{
			m_frames[f] = new Frame();
			m_frames[f]->init( pixelFormat, width, height, frameSize );
			*frame = m_frames[f];

			return;
		}
		else
		{
			if( m_frames[f]->m_deleted )
			{
				m_frames[f]->init( pixelFormat, width, height, frameSize );
				*frame = m_frames[f];
				return;
			}

		}


		*frame = NULL;

}

//static - make an existing frame available for reuse
void Frame::releaseFrame( Frame **frame )
{
	for( int f = 0; f < MAX_FRAMES; f ++ )
		if( m_frames[f] == *frame )
		{
			m_frames[f]->m_deleted = true;
			*frame = NULL;
			return;
		}
}

//static - make a new frame with the same parameters as an existing one, alloc a pixel buffer, but do not initialise the pixels
void Frame::cloneFrame( Frame **clone, Frame* original )
{
	allocFrame( clone, original->m_pixelFormat, original->m_width, original->m_height, original->m_frameSize );

}


void Frame::init( uint8_t pixelFormat, uint16_t width, uint16_t height, uint32_t frameSize )
{
	
	if( m_frameSize != frameSize )  // keep the same pixel buffer if we can, to avoid fragmentation
	{
		free( m_pixels );
		m_pixels = (uint8_t *) malloc( frameSize );
	}

	m_pixelFormat = pixelFormat;
	m_width = width;
	m_height = height;
	m_frameSize = frameSize;
	m_deleted = false;

	m_bpp = 1; // for now

	m_numPixels = m_frameSize / m_bpp;
}

uint16_t Frame::getPixel( uint32_t p )
{
	if( m_bpp == 1 )
		return m_pixels[ p ];
	else
		return ((m_pixels[p<<1]) << 8) + m_pixels[1 + (p<<1) ]; // todo - check bytes order
}

void Frame::setPixel( uint32_t p, uint16_t val )
{
	if( m_bpp == 1 )
		m_pixels[ p ] = (uint8_t) val;
	else
	{
		m_pixels[p<<1] = val>>8;
		m_pixels[1 + (p<<1) ] = val & 0xff;
	}
}
