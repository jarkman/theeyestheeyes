
// Definitions to emulate mbed libraries in an MFC environment


typedef unsigned char uint8_t;


typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef int PinName;

class Serial
{
public:
	Serial( PinName rx, PinName tx );
	void baud( int rate );
	uint8_t getc();
	void putc( uint8_t c);
	int readable();
	void printf(char *msg, ...);

private:
	// variables used with the com port
BOOL     m_bPortReady;
HANDLE   m_hCom;
CString  m_sComPort;
DCB      m_dcb;
COMMTIMEOUTS m_CommTimeouts;
BOOL     bWriteRC;
BOOL     bReadRC;
DWORD iBytesWritten;
DWORD iBytesRead;
char       sBuffer[128];


};

class Logger
{
public:
	Logger( PinName rx, PinName tx );
	
	//void putc( uint8_t c);
	
	void printf(char *msg, ...);
};

class DigitalOut
{
public:
	DigitalOut( PinName pin ) {};
};

class Timer
{
public:
	void start();
	int read_ms();
	void stop();

	time_t startTime;
};



#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3


#define p13 13
#define p14 14

#define USBTX 0
#define USBRX 1

#define FILE_WRITE_STRING "wb"
#define FILE_READ_STRING "rb"

void wait( float secs );
void wait_ms( int ms );

#define ON_DESKTOP
