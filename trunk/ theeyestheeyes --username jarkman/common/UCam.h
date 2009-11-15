
// This is a class for talking to the 4DSystems uCam JPEG camera module
// from an mbed (mbed.org)

// Datasheet:
// http://www.4dsystems.com.au/downloads/micro-CAM/Docs/uCAM-DS-rev2.pdf


// Commands:
#define UCAM_INITIAL             0xAA01
#define UCAM_GET_PICTURE         0xAA04
#define UCAM_SNAPSHOT             0xAA05
#define UCAM_SET_PACKAGE_SIZE     0xAA06
#define UCAM_RESET                0xAA08
#define UCAM_DATA                 0xAA0A
#define UCAM_SYNC                0xAA0D
#define UCAM_ACK                 0xAA0E
#define UCAM_ACK                0xAA0E
#define UCAM_NAK                 0xAA0F
#define UCAM_LIGHT                0xAA13


// Picture types;
#define UCAM_PIC_TYPE_SNAPSHOT  0x01        // gets the pic taken with the SNAPSHOT command
                                            // in which case, the actualy type of the picture is the one specified by the SNAPSHOT
                                            
#define UCAM_PIC_TYPE_RAW_PREVIEW 0x02      // gets the current pic, doesn't require the SNAPSHOT command
#define UCAM_PIC_TYPE_JPEG_PREVIEW 0x05    // gets the current pic, doesn't require the SNAPSHOT command



#define UCAM_COLOUR_2_BIT_GREY 0x01 //2-bit Gray Scale (RAW, 2-bit for Y only) 01h
#define UCAM_COLOUR_4_BIT_GREY 0x02 //4-bit Gray Scale (RAW, 4-bit for Y only) 02h
#define UCAM_COLOUR_8_BIT_GREY 0x03 // (RAW, 8-bit for Y only) 
#define UCAM_COLOUR_8_BIT_COLOUR 0x04 // (RAW, 332(RGB))

#define UCAM_COLOUR_12_BIT_COLOR 0x05 // (RAW, 444(RGB)) 05h
#define UCAM_COLOUR_16_BIT_COLOR 0x06 //(RAW, 565(RGB)) 06h
#define UCAM_COLOUR_JPEG 0x07

#define UCAM_COLOUR_NOT_SET 0xFF


// Sizes for raw images
#define UCAM_RAW_SIZE_80x60 0x01
#define UCAM_RAW_SIZE_160x120 0x03
#define UCAM_RAW_SIZE_320x240 0x05
#define UCAM_RAW_SIZE_640x480 0x07
#define UCAM_RAW_SIZE_128x128 0x09
#define UCAM_RAW_SIZE_128x96 0x0B

// Sizes for jpeg images

#define UCAM_JPEG_SIZE_80x64     0x01
#define UCAM_JPEG_SIZE_160x128   0x03
#define UCAM_JPEG_SIZE_320x240    0x05
#define UCAM_JPEG_SIZE_640x480    0x07

// Snapshot types
#define UCAM_SNAPSHOT_JPEG 0x00
#define UCAM_SNAPSHOT_RAW 0x01

#ifndef FILE_WRITE_STRING
#define FILE_WRITE_STRING "w" // value for mbed - will be defined earlier on Win
#define FILE_READ_STRING "r"
#endif

class Frame;

class UCam
{
public:

    UCam(PinName tx, PinName rx);

    void doStartup();
    int doConfig( bool raw, uint8_t colourType, uint8_t imageSize );

    Frame *doGetRawPictureToBuffer( uint8_t picType );
    int doGetJpegPictureToFile( uint8_t picType, char* filename );

private:

    int doConnect();
    int fixConfusion();
    void sendCommand( int command, int p1, int p2, int p3, int p4 );
    int doCommand( int command, int p1, int p2, int p3, int p4 );
    int doReset();
    int doSyncs();
    int doSnapshot( uint8_t snapshotType );

    void sendAck();
    void sendAckForPackage( uint16_t p);
    void sendAckForRawData( ) ;
    int readAckPatiently( uint16_t command );
    int readAck( uint16_t command );
    int readSync();
    uint32_t readData();
    int readBytes(uint8_t *bytes, int size );

    uint16_t timedGetc();
    uint16_t readUInt16();
    int readPackage( FILE *jpgFile, uint16_t targetPackage );



private:
    Serial camSerial;
    uint8_t lastCommand;

    int m_confused;

    uint8_t packageBody[512];

    bool m_raw;
    uint8_t m_colourType;
    uint8_t m_imageSize;

    uint8_t m_bitsPerPixel;
    uint16_t m_width;
    uint16_t m_height;



};