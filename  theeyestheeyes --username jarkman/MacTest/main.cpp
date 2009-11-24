#include <iostream>
#include <stdio.h>
#include <stdlib.h>


unsigned char* readFromFile(std::string aFileName);
int thresholdImage(unsigned char* aBuffer, int aThreshold);
int downscaleImage(unsigned char* aBuffer);
int averageLight(unsigned char* aBuffer);
int edgeDetection(unsigned char* aBuffer);

// globals for eventual class
int width = 80;
int height = 60;


int main (int argc, char * const argv[]) {

	unsigned char* buffer;
	int ave = 0;
	
	std::cout << "Frame 0 " << std::endl;
	buffer = readFromFile("/Users/AstroBoy/Documents/Tech_Projects/Eyes/testframes/frame0");
	ave = averageLight(buffer);
	std::cout << "Ave Light = "<< ave << std::endl;
	thresholdImage(buffer, ave);	
	edgeDetection(buffer);
	
    return 0;
}

unsigned char* readFromFile(std::string aFileName)
{
	// insert code here...

	
	FILE* pFile;
	long lSize;
	unsigned char* buffer;
	size_t result;
	int threshold = 120;
	
	pFile = fopen (aFileName.c_str() , "rb" );
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
	
	return buffer;
}

// currently just prints
int thresholdImage(unsigned char* aBuffer,int aThreshold)
{
	int row,col;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col++)
		{
			if(int(*aBuffer) > aThreshold){
				std::cout << "1 ";
			}
			else{
				std::cout << "0 ";
			}

			aBuffer++;
		}
		std::cout << std::endl;
	}
	return 1;
}

// currently just prints
int downscaleImage(unsigned char* aBuffer)
{
	int row,col;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col++)
		{
			if(int(*aBuffer) > 120){
				std::cout << "x ";
			}
			else if(int(*aBuffer) > 100){
				std::cout << "H ";
			}
			else if(int(*aBuffer) > 80){
				std::cout << "B ";
			}			
			else if(int(*aBuffer) > 60){
				std::cout << "M ";
			}
			else if(int(*aBuffer) > 40){
				std::cout << "Z ";
			}			
			else{
				std::cout << "0 ";
			}
			aBuffer++;
		}
		std::cout << std::endl;
	}
	return 1;
}

int averageLight(unsigned char* aBuffer)
{
	int row,col,sum;
	sum = 0;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col++)
		{
			sum += int(*aBuffer);
		}
		aBuffer++;
	}
	return sum/(width*height);
}

int edgeDetection(unsigned char* aBuffer)
{
	int row,col;
	unsigned char* fbuffer;
	unsigned char* bbuffer;
	aBuffer++;
	for(row = 1; row < (height-1); row++)
	{
		for(col = 1; col < (width-1); col++)
		{
			fbuffer = aBuffer;
			aBuffer++;
			bbuffer = aBuffer;
			bbuffer++;
			if((int(*aBuffer) != int(*fbuffer)) || (int(*aBuffer) != int(*bbuffer))){
				std::cout << "x ";
			}
			else{
				std::cout << "0 ";
			}
		}
		std::cout << std::endl;
	}
	return 1;
}
