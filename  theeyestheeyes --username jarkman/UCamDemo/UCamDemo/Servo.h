#pragma once

class Servo
{
public :
		Servo( int pin ){pos = 0;};
		float read() { return pos;};
		void write( float p ) { pos = p;};
private:
	float pos;
};