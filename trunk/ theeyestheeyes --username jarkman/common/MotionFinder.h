#pragma once
// Class to detect motion in a series of frames

class Frame;


class MotionFinder
{
public:
    MotionFinder( ServoMinder *xServoMinder, ServoMinder *yServoMinder  );

    void processFrame( Frame *frame );
    void newBackground( Frame *frame );

    ~MotionFinder();


    Frame *m_backgroundFrame;
    Frame *m_resultFrame;
    uint16_t m_delta;

    float m_attentionX;
    float m_attentionY;

    
    ServoMinder *m_xServoMinder;
    ServoMinder *m_yServoMinder;

};
