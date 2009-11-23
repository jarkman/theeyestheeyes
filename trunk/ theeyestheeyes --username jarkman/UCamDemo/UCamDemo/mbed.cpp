#include "stdafx.h"
#include "mbed.h"

// Emulation for the mbed environment in a VC++ world

// NB - initialise m_sComPort to refer to the serial port that connects to your camera!

CString ErrorString(DWORD err);

void wait( float secs )
{
	Sleep( (int) (secs * 1000.0));
}

void wait_ms(int ms)
{
	Sleep( ms );
}


uint32_t timeInMillis()
{
	CTime tim = CTime::GetCurrentTime();
	SYSTEMTIME sysTime;

	tim.GetAsSystemTime(sysTime);

	uint32_t now = (((((sysTime.wHour * 60) + sysTime.wMinute) * 60) + sysTime.wSecond) * 1000) + sysTime.wMilliseconds;

	return now;
}


void Timer::reset()
{
	startTime = timeInMillis();
}

void Timer::start() // not a very good emulation, but close enough for our purposes
{
	startTime = timeInMillis();
}

int Timer::read_ms()
{
	return (int) (timeInMillis() - startTime);

}


	



float Timer::read()
{
	return ((float)read_ms()) / 1000.0;
}

void Timer::stop()
{

	
}

Serial::Serial( PinName rx, PinName tx )
{
	iBytesRead = 0;

	m_sComPort = L"\\\\.\\COM19"; // from http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&p=295206

	m_hCom = CreateFile( m_sComPort, 
		GENERIC_READ | GENERIC_WRITE,
		0, //FILE_SHARE_READ | FILE_SHARE_WRITE, // exclusive access
		NULL, // no security
		OPEN_EXISTING,
		0, // no overlapped I/O
		NULL); // null template 

	if (m_hCom == INVALID_HANDLE_VALUE) 
	{
		DWORD err = ::GetLastError();
		CString errmsg = ErrorString(err);

		AfxMessageBox(errmsg);

	}

	m_bPortReady = SetupComm(m_hCom, 128, 128); // set buffer sizes

	
}

void Serial::baud( int rate )
{
	//Port settings are specified in a Data Communication Block (DCB). The easiest way to initialize a DCB is to call GetCommState to fill in its default values, override the values that you want to change and then call SetCommState to set the values.

	m_bPortReady = GetCommState(m_hCom, &m_dcb);
	m_dcb.BaudRate = rate;
	m_dcb.ByteSize = 8;
	m_dcb.Parity = NOPARITY;
	m_dcb.StopBits = ONESTOPBIT;
	m_dcb.fAbortOnError = TRUE;

	m_bPortReady = SetCommState(m_hCom, &m_dcb);

	
	m_bPortReady = GetCommTimeouts (m_hCom, &m_CommTimeouts);

	m_CommTimeouts.ReadIntervalTimeout = 1;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1;
	m_CommTimeouts.WriteTotalTimeoutConstant = 50;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 10;

	m_bPortReady = SetCommTimeouts (m_hCom, &m_CommTimeouts);
	

}

uint8_t Serial::getc()
{
	while( ! readable())
		Sleep( 1 );

	if( iBytesRead == 0 )
	{
		do
		{
			bReadRC = ReadFile(m_hCom, &sBuffer, 1, &iBytesRead, NULL);
		}
		while( iBytesRead == 0 );
	}
	iBytesRead = 0; // consume it
	return sBuffer[0];
}

void Serial::putc( uint8_t c)
{
	bWriteRC = WriteFile(m_hCom, &c,1,&iBytesWritten,NULL);
}

int Serial::readable()
{
	if( iBytesRead == 0 )
	{
		bReadRC = ReadFile(m_hCom, &sBuffer, 1, &iBytesRead, NULL);
	}

	return iBytesRead > 0 ;
}

void Serial::printf(char *fmt, ...) {
	//  http://channel9.msdn.com/forums/TechOff/254707-Wrapping-printf-in-C/

    va_list arg_ptr;
    va_start(arg_ptr, fmt);

	char str[1000];
	int i;
    int c = vsprintf (str, fmt, arg_ptr);

	for( i = 0; str[i] != '\0'; i ++ );
		putc( str[i] );


	
}


Logger::Logger( PinName rx, PinName tx )
{
}

void Logger::printf(char *fmt, ...) {
	//  http://channel9.msdn.com/forums/TechOff/254707-Wrapping-printf-in-C/

    va_list arg_ptr;
    va_start(arg_ptr, fmt);

	char str[1000];

    int c = vsprintf (str, fmt, arg_ptr);

	TRACE( str );


    

}

CString ErrorString(DWORD err)
    {
     CString Error;
     LPTSTR s;
     if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            err,
            0,
            (LPTSTR)&s,
            0,
            NULL) == 0)
    { /* failed */
     // Unknown error code %08x (%d)

     /*CString fmt = "Unknown error 0x%08x (%d)";
     CString t;
     
     t.Format(fmt, err, LOWORD(err));
     Error = t;
	 */
    } /* failed */
     else
    { /* success */
     LPTSTR p = _tcschr(s, _T('\r'));
     if(p != NULL)
        { /* lose CRLF */
         *p = _T('\0');
        } /* lose CRLF */
     Error = s;
     ::LocalFree(s);
    } /* success */
     return Error;
    } // ErrorString