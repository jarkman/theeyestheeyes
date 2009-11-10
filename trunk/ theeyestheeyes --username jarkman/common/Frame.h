
// We're on an LPC2368:
// http://www.standardics.nxp.com/support/documents/microcontrollers/pdf/user.manual.lpc23xx.pdf
// so we have 32k of RAM available
// The faster MBEDs use the 1768, with 64k of RAM

// Assume we will work at 80x60, 1 byte per pixel, hence 5k/frame

// This class keeps an array of reusable frames - 
//   Get one with a call of Frame::allocFrame, 
//   Give it back with Frame::releaseFrame

#define MAX_FRAMES 10

class Frame
{
public:
	Frame();
	~Frame();
	void init( uint8_t pixelFormat, uint16_t width, uint16_t height, uint32_t frameSize );
	uint16_t getPixel( uint32_t p );
	void setPixel( uint32_t p, uint16_t  );


	// Use these methods to manage a pool of frames to avoid fragmentation
	static void initFrames();
	static void allocFrame( Frame **frame, uint8_t pixelFormat, uint16_t width, uint16_t height, uint16_t frameSize );
	static void releaseFrame( Frame **frame );
	static void cloneFrame( Frame **clone, Frame* original );
	
private:
	static Frame* m_frames[MAX_FRAMES];

public:
	uint8_t *m_pixels;
	uint16_t m_width;
	uint16_t m_height;
	uint8_t m_pixelFormat;
	uint32_t m_frameSize;
	uint8_t m_bpp;
	uint32_t m_numPixels;
	boolean m_deleted;

};