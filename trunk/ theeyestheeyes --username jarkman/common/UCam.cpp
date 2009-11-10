#include "stdafx.h"

#include "mbed.h"
#include "ucam.h"
#include "Frame.h"
#include "MotionFinder.h"

// ucam protocol implementation for mbed


Logger pcSerial(USBTX, USBRX); // tx, rx



DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);




//LocalFileSystem local("local");
UCam ucam(p13, p14);

MotionFinder *motionFinder = NULL;



void UCamInit() {
   
    ucam.doStartup();
	Frame::initFrames();
	motionFinder = new MotionFinder();
    
}

void UCamGetJpeg()
{
	ucam.doConfig( UCAM_COLOUR_JPEG, UCAM_RAW_SIZE_160x120, UCAM_JPEG_SIZE_640x480 ); 


    uint8_t picType = UCAM_PIC_TYPE_JPEG_PREVIEW;
    
    if( picType == UCAM_PIC_TYPE_SNAPSHOT )
        ucam.doSnapshot( UCAM_SNAPSHOT_JPEG );
        
	ucam.doGetJpegPictureToFile( picType, "C:/mbed/out.jpg" );
    
}

Frame* UCamGetRaw( )
{
    ucam.doConfig( UCAM_COLOUR_8_BIT_GREY, UCAM_RAW_SIZE_80x60, UCAM_JPEG_SIZE_640x480 ); 
	// also see doGetRawPictureToBuffer for related values that have to stay in sync 

    uint8_t picType = UCAM_PIC_TYPE_RAW_PREVIEW;
    
    if( picType == UCAM_PIC_TYPE_SNAPSHOT )
        ucam.doSnapshot( UCAM_SNAPSHOT_RAW );
        
	return ucam.doGetRawPictureToBuffer( picType );
    
}

Frame* UCamGetDiff( )
{
    ucam.doConfig( UCAM_COLOUR_8_BIT_GREY, UCAM_RAW_SIZE_80x60, UCAM_JPEG_SIZE_640x480 ); 
	// also see doGetRawPictureToBuffer for related values that have to stay in sync 



    uint8_t picType = UCAM_PIC_TYPE_RAW_PREVIEW;
    
    if( picType == UCAM_PIC_TYPE_SNAPSHOT )
        ucam.doSnapshot( UCAM_SNAPSHOT_RAW );
        
	Frame *frame = ucam.doGetRawPictureToBuffer( picType );

	motionFinder->processFrame( frame );

	return motionFinder->m_resultFrame;
    
}

Frame* UCamResetDiff( )
{
    ucam.doConfig( UCAM_COLOUR_8_BIT_GREY, UCAM_RAW_SIZE_80x60, UCAM_JPEG_SIZE_640x480 ); 

    uint8_t picType = UCAM_PIC_TYPE_RAW_PREVIEW;
    
    if( picType == UCAM_PIC_TYPE_SNAPSHOT )
        ucam.doSnapshot( UCAM_SNAPSHOT_RAW );
        
	Frame *frame = ucam.doGetRawPictureToBuffer( picType );

	motionFinder->newBackground( frame );

	return motionFinder->m_resultFrame;
    
}

UCam::UCam( PinName tx, PinName	rx ) : camSerial(p13, p14) // tx, rx

{
 lastCommand = 0;

}

void UCam::doStartup()
{
    pcSerial.printf("\r\n\n\nucam waiting\r\n");
    
    wait(5);
          
    pcSerial.printf("\r\n\n\nucam running!\r\n");
    
	// When running on desktop over USB serial cable, this baud rate seems to need to match the rate set in the USB device configuration (via Control Panel/System/Device Manager/Properties/Advanced)
    camSerial.baud(14400);  // lowest supported rate
    
    //camSerial.baud(115200);    // highest supported rate

     myled1 = 1;
     
    doConnect();

    
     
    //wait 2 secs for camera to warm up
    wait(2);

     myled1 = 0;

	 pcSerial.printf("doStartup finished\r\n");
}

void UCam::doConfig( uint8_t colourType, uint8_t rawSize, uint8_t jpegSize )
{   
     myled2 = 1;

    
    // pcSerial.printf("sending INITIAL\r\n");

    doCommand( UCAM_INITIAL, 0x00, 
                                colourType,     // colour type - 07 for jpg
                                rawSize,     // raw resolution
                                jpegSize  );  // jpeg resolution - 05 for 320x240
                                
    
    // pcSerial.printf("sending package size\r\n");


	// package size is only relevant for jpeg transfers
    doCommand( UCAM_SET_PACKAGE_SIZE, 0x08, 
                                        0x00,     // low byte of size
                                        0x02,     // high byte of size
                                        0x00 ); 
                                        // 512 bytes
  

  myled2 = 0;
}


int UCam::doConnect()
{
    while( true )
    {
        pcSerial.printf("sending sync\r\n");

        wait( 0.5 );
        
        sendCommand( UCAM_SYNC, 0x00, 0x00, 0x00, 0x00  ); 
        
       //// pcSerial.printf("sent sync\r\n");

        
        if( camSerial.readable())
        {
            //pcSerial.printf("readable - trying ack\r\n");

            if( readAck(UCAM_SYNC))
                break;
        }
        
    }
    
    readSync();
    
    myled1 = 1;


    sendAck();
    
    return 1;
}

int UCam::sendCommand( int command, int p1, int p2, int p3, int p4 )
{
    camSerial.putc( (command >> 8) & 0xff );     
    camSerial.putc( command & 0xff );    
    camSerial.putc( p1 );
    camSerial.putc( p2 );
    camSerial.putc( p3 );
    camSerial.putc( p4 );
    
    return 1;
}
    
int UCam::doCommand( int command, int p1, int p2, int p3, int p4 )
{
    sendCommand(  command, p1,p2, p3, p4 );
    
    
    return readAck( command );
}

void UCam::sendAck()
{
     sendCommand( UCAM_ACK, 0x0D, 0x00, 0x00, 0x00 );
}
 
void UCam::sendAckForPackage( uint16_t p)  // requests the camera to send the data for the package with that number
{
     sendCommand( UCAM_ACK, 0x00, 0x00, p & 0xff,  (p >> 8) & 0xff);
} 

void UCam::sendAckForRawData( ) 
{
     sendCommand( UCAM_ACK, 0x00, 0x0A, 0x01, 0x00);
} 

int UCam::readAck( uint16_t command )
{
    uint8_t a = 0;

    while( camSerial.readable())
    {
         a = camSerial.getc();
         if( a == 0xAA )
         {
           // pcSerial.printf("ack read AA\r\n");

             break;
         }
         
          pcSerial.printf("ack skipped %x\r\n", (int) a);
    }
    
    uint8_t bytes[5];
    
    readBytes( bytes, 5);
    
    pcSerial.printf("ack read %x  %x %x %x %x %x \r\n", (int) a, (int) bytes[0], (int) bytes[1], (int) bytes[2], (int) bytes[3], (int) bytes[4] );
   
    if( bytes[1] != (command & 0xff))
    {
        pcSerial.printf("ack is for wrong command!\r\n");
        return 0;
    }

    return 1;
}

int UCam::readSync()
{
    uint8_t bytes[6];
    
    readBytes( bytes,6 );
    
     // check content

    //// pcSerial.printf("sync read %x %x %x %x %x %x \r\n", (int) bytes[0], (int) bytes[1], (int) bytes[2], (int) bytes[3], (int) bytes[4], (int) bytes[5] );
    
    return 1;
}



uint16_t UCam::readUInt16()
{
    uint8_t bytes[2];
    
    readBytes( bytes, 2);

    // pcSerial.printf("readUInt16 read %x %x  \r\n", (int) bytes[0], (int) bytes[1] );
    
   
    uint16_t i =  bytes[1]<<8 | bytes[0];
    

    
    return i;
}

int UCam::readBytes(uint8_t *bytes, int size )
{

    int n = 0;
    do
    {
        uint16_t c =  timedGetc();
        if( c == 0x1000 ) // timeout
            break;
            
        bytes[n] = (uint8_t) c;
        n ++;
     }
     while( n < size );
                
    return n;                
}

uint16_t UCam::timedGetc()
{
    //return camSerial.getc();

    Timer t;
    t.start();
    do
    {
        if( camSerial.readable())
            return camSerial.getc();
     }
     while( t.read_ms() < 1000 );
     
     
      pcSerial.printf("timedGetc gave up after %d ms\r\n", (int) t.read_ms());
      
      t.stop();
     return 0x1000;
     
}

int UCam::doSnapshot( uint8_t snapshotType )
{
    return doCommand( UCAM_SNAPSHOT, snapshotType, 0x00, 0x00, 0x00 ); 
}

Frame* UCam::doGetRawPictureToBuffer( uint8_t pictureType )
{

     pcSerial.printf("sending get picture\r\n");
     
     myled3 = 1;

    doCommand( UCAM_GET_PICTURE,   pictureType, 0x00, 0x00, 0x00 );  
    
    pcSerial.printf("sent get_picture\r\n");
        
    uint32_t totalBytes = readData();

	Frame *frame;
	Frame::allocFrame( &frame, UCAM_COLOUR_8_BIT_GREY, 80, 60, totalBytes );

	uint8_t *rawBuffer = frame->m_pixels;

	

    // pcSerial.printf("totalBytes is %d bytes\r\n", (int) totalBytes );

    pcSerial.printf("reading...\r\n");
    uint32_t actuallyRead = readBytes( rawBuffer, totalBytes );

	pcSerial.printf("...read\r\n");

	sendAckForRawData();

	if( actuallyRead < totalBytes )
	{
	    pcSerial.printf("Not enough bytes - %d  < %d \r\n", (int) actuallyRead, (int) totalBytes );
		return NULL;
	}

    
     pcSerial.printf("Done!\r\n");
     
    myled3 = 0;
	
    return frame;
 }

int UCam::doGetJpegPictureToFile( uint8_t pictureType, char*filename )
{
    FILE *jpgFile = fopen(filename, FILE_WRITE_STRING); // "w" or "wb" for Windows
    
    if( jpgFile != NULL )
         pcSerial.printf("opened output file\r\n");
    
     pcSerial.printf("sending get picture\r\n");
     
     myled3 = 1;

    doCommand( UCAM_GET_PICTURE,   pictureType, 0x00, 0x00, 0x00 );  
    
    pcSerial.printf("sent get_picture\r\n");
        
    uint32_t totalBytes = readData();
    
    // pcSerial.printf("totalBytes is %d bytes\r\n", (int) totalBytes );
    
    uint16_t packageN = 0;
    
    
    uint32_t bytesRead = 0;
    
    while( bytesRead < totalBytes )
    {
       sendAckForPackage(packageN);

       int actuallyRead = readPackage( jpgFile, packageN );
       
       pcSerial.printf("read package of %d bytes\r\n", (int) actuallyRead );
       
       if( actuallyRead < 0 )
       {
           pcSerial.printf("didn't read enough bytes of package - giving up\r\n");
           
      
           break;
       }
       
       bytesRead += actuallyRead;
        
       packageN++;
       
        

    }

    sendAckForPackage(0xF0F0);
    
    fclose( jpgFile );
    
     pcSerial.printf("Done!\r\n");
     
    myled3 = 0;
    return 1;
 }
    
    
int UCam::readPackage( FILE *jpgFile, uint16_t targetPackage )
{
    int actuallyRead;
    uint16_t packageId;
    // 2 bytes id
    packageId = readUInt16();


    
   //pcSerial.printf("packageId is %d\r\n", (int)packageId );

    if( packageId != targetPackage )
    {
         pcSerial.printf("bad package id  %d (%x)  in package header for target id %d - giving up\r\n", packageId, packageId, targetPackage);
        
        actuallyRead = readBytes( packageBody, 500 );
         pcSerial.printf("next %d bytes\r\n", actuallyRead);
        for( int i = 0 ; i < actuallyRead; i ++ )
            pcSerial.printf("%x\r\n", packageBody[i]);
            
        return -1;
    }
    
    // 2 bytes data size
    uint16_t packageSize = readUInt16();
    
   //pcSerial.printf("packageSize is %d bytes\r\n", (int)packageSize );


    int dataSize =  packageSize; // - 6;
    
   //pcSerial.printf("dataSize is %d bytes\r\n", (int)dataSize );

    if( dataSize > sizeof( packageBody )) // too big - give up
    {
         pcSerial.printf("bad dataSize  %d in package header for id %d - giving up\r\n", dataSize, packageId);
        return -1;
    }
        
    // image data (package size - 6 bytes)
    actuallyRead = readBytes( packageBody, dataSize);
    

    
   //pcSerial.printf("done readBytes, read %d\r\n", actuallyRead);

    if( actuallyRead < dataSize )
    {
        pcSerial.printf("bad readBytes, read %d\r\n", actuallyRead);
        for( int i = 0 ; i < actuallyRead; i ++ )
            pcSerial.printf("%x\r\n", packageBody[i]);
        return -1;
    }
        
    // 2 bytes verify code
    uint16_t verifyCode = readUInt16();
    
    pcSerial.printf("done readBytes for package, read %d\r\n", actuallyRead);
    pcSerial.printf("verifyCode %d\r\n", verifyCode);
    
    fwrite( packageBody, 1, actuallyRead, jpgFile );
    
    pcSerial.printf("done fwrite\r\n" );

    
    return actuallyRead;

}    

uint32_t UCam::readData()
{
    uint8_t bytes[6];
    
    readBytes( bytes, 6);
    
    uint32_t totalSize = ( bytes[5]<<16 ) | ( bytes[4]<<8 ) | ( bytes[3] );

     // check content - AA 0A tt nn nn nn - tt is the image type, nn tells us the image size
    pcSerial.printf("readData totalSize %d - read %x %x %x %x %x %x \r\n", (int) totalSize, (int) bytes[0], (int) bytes[1], (int) bytes[2], (int) bytes[3], (int) bytes[4], (int) bytes[5] );


    if( bytes[0] != 0xAA )
    {
     pcSerial.printf("readData failed\r\n");
     return 0;
    }
        
    return totalSize;
}

