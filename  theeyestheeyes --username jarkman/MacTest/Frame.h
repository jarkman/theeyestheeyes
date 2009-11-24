/*
 *  Frame.h
 *  MacTest
 *
 *  Created by Astroboy on 23/11/2009.
 *  Copyright 2009 uwe. All rights reserved.
 *
 */


class Frame{

	Frame(int aWidth, int aHeight);
	~Frame();
	int readFromFile(std::string aFileName);
	
	int mWidth, mHeight;

};
