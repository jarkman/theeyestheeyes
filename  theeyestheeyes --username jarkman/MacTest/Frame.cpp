/*
 *  Frame.cpp
 *  MacTest
 *
 *  Created by Astroboy on 23/11/2009.
 *  Copyright 2009 uwe. All rights reserved.
 *
 */

#include "Frame.h"

Frame::Frame(int aWidth, int aHeight)
{
	mWidth = aWidth;
	mHeight = aHeight;
}

Frame::~Frame()
{

}

int Frame::readFromFile(std::string aFileName)
{
	int row,col;
	
	FILE* pFile;
	long lSize;
	unsigned char* buffer;
	size_t result;
	int threshold = 120;
	
	pFile = fopen (aFileName , "rb" );
	if (pFile==NULL) {
		fputs ("File error",stderr);
		exit (1);
	}
	
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
	
	// allocate memory to contain the whole file:
	buffer = (unsigned char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	
	for(row = 0; row < mHeight; row++)
	{
		for(col = 0; col < mWidth; col++)
		{
			if(int(*buffer) > 120)
			{
				std::cout << "x ";
			}
			else if(int(*buffer) > 100)
			{
				std::cout << "H ";
			}
			else if(int(*buffer) > 80)
			{
				std::cout << "B ";
			}			
			else if(int(*buffer) > 60)
			{
				std::cout << "M ";
			}
			else if(int(*buffer) > 40)
			{
				std::cout << "Z ";
			}			
			else
			{
				std::cout << "0 ";
			}
			buffer++;
		}
		std::cout << std::endl;
	}
	return 1;
}