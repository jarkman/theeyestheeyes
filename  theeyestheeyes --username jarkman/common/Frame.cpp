#include "stdafx.h"

#include "mbed.h"
#include "ucam.h"
#include "Frame.h"

extern Logger pcSerial;

Frame* Frame::m_frames[MAX_FRAMES]; // array of reusable frames

Frame::Frame() // don't construct Frame yourself - call allocFrame instead
{
	m_pixels = NULL;
	m_width = 0;
	m_height = 0;
	m_pixelFormat = 0;
	m_deleted = true;
	m_numPixels = 0;
	m_bad = false;
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

	pcSerial.printf("No free frames!\r\n");
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
	m_bad = false;

	switch( m_pixelFormat )
	{
		case UCAM_COLOUR_2_BIT_GREY:
			m_bitsPerPixel = 2;
			break;
		case UCAM_COLOUR_4_BIT_GREY:
			m_bitsPerPixel = 4;
			break;
		case UCAM_COLOUR_8_BIT_GREY:
			m_bitsPerPixel = 8;
			break;
		case UCAM_COLOUR_8_BIT_COLOUR:
			m_bitsPerPixel = 8;
			break;
		case UCAM_COLOUR_12_BIT_COLOR:
			m_bitsPerPixel = 16;
			break;
		case UCAM_COLOUR_16_BIT_COLOR:
			m_bitsPerPixel = 16;
			break;
		case UCAM_COLOUR_JPEG:
		default:
			m_bitsPerPixel = 16; // ?? not at all sure, but we are not using frames to handle jpegs right now
			break;
	}

	m_numPixels = (8 * m_frameSize) / m_bitsPerPixel;
}

uint16_t Frame::getPixel( uint32_t p )
{
	switch( m_bitsPerPixel )
	{
	case 4:
		if( p & 1 )
			return m_pixels[ p >> 1 ] & 0x0f;
		else
			return m_pixels[ p >> 1 ] >> 4;

		break;

	case 8:
		return m_pixels[ p ];

	case 16:
	default:
		return ((m_pixels[p<<1]) << 8) + m_pixels[1 + (p<<1) ]; // todo - check bytes order
	}
}

void Frame::setPixel( uint32_t p, uint16_t val )
{
	switch( m_bitsPerPixel )
	{
	case 4:
		if( p & 1 )
			m_pixels[ p >> 1 ] = ( m_pixels[ p >> 1 ] & 0xf0) | (val & 0x0f);
		else
			m_pixels[ p >> 1 ] = ( m_pixels[ p >> 1 ] & 0x0f) | ((val & 0x0f) << 4 );
		break;

	case 8:
		m_pixels[ p ] = (uint8_t) val;
		break;
	case 16:
	default:
		m_pixels[p<<1] = val>>8;
		m_pixels[1 + (p<<1) ] = val & 0xff;
		break;
	}

	
}

void Frame::writeToFile( char *filename )
{
	pcSerial.printf("writeToFile - %s\r\n", filename);

    FILE *rawFile = fopen(filename, FILE_WRITE_STRING); // "w" or "wb" for Windows

    fwrite( &m_pixelFormat, 1, 1, rawFile );
    fwrite( &m_width, 1, 2, rawFile );
    fwrite( &m_height, 1, 2, rawFile );
    fwrite( &m_frameSize, 1, 4, rawFile );

    fwrite( m_pixels, 1, m_frameSize, rawFile );

    fclose( rawFile );

}

// static
void Frame::readFromFile( char *filename, Frame **frame )
{
	*frame = NULL;

	pcSerial.printf("readFromFile - %s\r\n", filename);

    FILE *rawFile = fopen(filename, FILE_READ_STRING); 
	if( rawFile == NULL )
	{
		pcSerial.printf("readFromFile - failed to open %s\r\n", filename);
		return;
	}

	uint8_t pixelFormat;
	uint16_t width;
	uint16_t height;
	uint32_t frameSize;

    if( 1 == fread( &pixelFormat, 1, 1, rawFile ))
		if( 2 == fread( &width, 1, 2, rawFile ))
			if( 2 == fread( &height, 1, 2, rawFile ))
				if( 4 == fread( &frameSize, 1, 4, rawFile ))
				{

					allocFrame( frame, pixelFormat, width, height, frameSize );

					if( frameSize != fread( (*frame)->m_pixels, 1, frameSize, rawFile ))
					{
						releaseFrame( frame );
						pcSerial.printf("readFromFile - bad size");
					}
				}

    fclose( rawFile );

}