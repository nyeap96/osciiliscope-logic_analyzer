#include <shapes.h>
#include <fontinfo.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <libusb.h>
#include <time.h>
#include <math.h>
#include "logicExpression.h"

int ch;
int width;
int height;
int xDivisions = 10;
int nChannel = 8;
int tExpr;
uint8_t direction = 0;
int memDepth = 5000;
int freq = 10;
int xScale = 100;
int samplesSec;
char freqSelect;
FILE *inFile;

void drawCursor()
{
	returnValue = libusb_bulk_transfer(dev, (0x83), potValues, 2, &receiveByte,  0);			// receive pot values from PSOC
	pot1Scaled = potValues[0] * width/255;		// scale pot values as needed
	pot2Scaled = (potValues[1] * memDepth/255) - memDepth/2;
	Stroke(0,255,0,1);	
	Line(pot1Scaled, 0 , pot1Scaled, height);			// draw cursor
}

void drawWaves()
{
	for (i = 0; i < memDepth; i++)			// iterate through data
	{
		for (j = 0; j < nChannel; j++)			// iterate through each bit of data
		{
			Line(xTransform(i,samplesSec, xDivisions, xScale),			// draw data
				 bitData[i + triggerIndex - xScale/4 + pot2Scaled][j] * 50 + (j*150),
				 xTransform(i + 1, samplesSec, xDivisions, xScale),
				 bitData[i + 1 + triggerIndex - xScale/4 + pot2Scaled][j] * 50 + (j*150));
			Stroke(0,0,255,1);
			
			Line(xTransform(triggerIndex, samplesSec, xDivisions, xScale),			// draw blue line on trigger event
				 0,
				 xTransform(triggerIndex, samplesSec, xDivisions, xScale),
				 height);
			Stroke(255,0,0,1);
		}
		if (xTransform(i + 1, samplesSec, xDivisions, xScale) > width)
		{
			break;	
		}
	}	
}

int xTransform(float sample, float samplesSec, float xDivisions, float xScale)			// function to do xScale
{
	float xResult = sample;
	xResult = xResult * (1.0f/(samplesSec));	
	xResult = xResult * (1.0f/(xScale/1000000.0f));
	xResult = xResult * (width/xDivisions);
	return xResult;
}

int main (int argc, char* argv[]){

	int receiveByte;
	char data[10000];
	char bitData[10000][8];
	libusb_device_handle* dev;		// USB device initialization
	libusb_init(NULL);
	dev = libusb_open_device_with_vid_pid(NULL,0x04B4,0x8051);
	
	if (dev == NULL)
	{
		printf("device not found\n");
		return -1;
	}
	// Set configuration of USB device
	if (libusb_set_configuration(dev, 1) != 0)
	{
		perror("devise config failed");
		return -1;
	}

	if (libusb_reset_device(dev) != 0)
	{
		printf("Device reset failed\n");
		return -1;
 	}
	
	// Claim the interface.  This step is needed before any I/Os can be
	// issued to the USB device.
	if (libusb_claim_interface(dev, 0) !=0)
	{
		printf("Cannot claim interface");
		return -1;
	}

	while((ch = getopt(argc,argv, "n:t:d:m:f:x:")) != -1)			// while loop that checks for user arguments and sets them accordingly
	{
		switch(ch)
		{
			case 'n':
				nChannel = atoi(optarg);
				if (!(nChannel == 2 || nChannel == 4 || nChannel == 8))
				{
					perror("-n must be 2, 4, or 8\n");
					return -1;
				}
				break;
			
			case 't':
				inFile = fopen(optarg, "r");
				if (inFile == NULL)
				{
					printf("invalid file name\n");
				}
				break;

			case 'd':
				if (strcmp(optarg,"pos") == 0)
				{
					direction = 0;
				}
				else if(strcmp(optarg, "neg") == 0)
				{
					direction = 1;
				}
				else
				{
					perror("-d must be eithre pos or neg\n");
					return -1;
				}
				break;

			case 'm':
				memDepth = atoi(optarg);
				if (memDepth % 100 != 0 || memDepth > 10000)
				{
					perror("-m must be between 0 and 10000 and divisible by 100\n");
					return -1;
				}
				break;

			case 'f':
				freq = atoi(optarg);
				if (!(freq == 1 || freq == 10 ||freq == 50 || freq == 100)) 
				{
					perror("-f must be 1, 10, 50, or 100\n");
					return -1;
				}
				if (freq == 1)
				{
					freqSelect = 0;
					samplesSec = 1000;
				}
				else if(freq == 10)
				{
					freqSelect = 1;
					samplesSec = 10000;
				}
				else if(freq == 50)
				{
					freqSelect = 2;
					samplesSec = 50000;
				}
				else if(freq == 100)
				{
					freqSelect = 3;
					samplesSec = 100000;
				}
				break;

			case 'x':
				xScale = atoi(optarg);
				if (!(xScale == 1 || xScale == 5 || xScale == 10 || xScale == 100 ||
					  xScale == 1000 || xScale == 2000 || xScale == 5000 || xScale == 10000))
				{
					perror("-x must be 1, 5, 10, 100, 1000, 2000, 5000, or 10000\n");
					return -1;
				}
				break;
			default:
				break;
		}
	}
	uint8_t table[256];
	if (inFile != NULL)			// read logic expression from file and then put it inot postfix and get the truth table frmo it
	{
		char expression[100];
		struct Stack exprStack;
		fgets(expression, 100, inFile);
		exprStack = inToPostfix(expression);
		truthTable(exprStack, table);
	}
	
		
	int returnValue;
	int sentByte;	

	returnValue = libusb_bulk_transfer(dev, 0x02, &freqSelect, 1,&sentByte,0);			// send frequency to PSOC
	returnValue = libusb_bulk_transfer(dev, 0x02, (unsigned char*)&memDepth, 4, &sentByte, 0);			// send memoryDepth to PSOC
	returnValue = libusb_bulk_transfer(dev, 0x02, table, 256, &sentByte, 0);			// send truth table to PSOC
	returnValue = libusb_bulk_transfer(dev, 0x02, (unsigned char*)&direction, 1, &sentByte, 0);				// send trigger direction to PSOc
	returnValue = libusb_bulk_transfer(dev, 0x81, data, memDepth, &receiveByte, 0);			// receive data to output FROM PSOC

	init(&width, &height);			// get widht/height
	int i = 0;
	int j = 0;
	int dataIndex = 64;
	int mask;	
	int display = 0;
	int pot1Scaled = 0;
	int pot2Scaled = 0;
	int returnValueData;	
	int triggerFound = 0;	
	int checkTrig1;
	int checkTrig2;
	
	for (i = 0; i <  sizeof(data); i++)		// get data into bit format and store in memory
	{
		mask = 128;
		for(j = 7; j >= 0; j--)
		{
			bitData[i][j] = (data[i] & mask)/mask;
			mask >>= 1;
		}
	}
	
	uint8_t potValues[2];
	int supIndex = memDepth/2;
	int triggerIndex = memDepth/2;
	while(1)
	{
		Start(width, height);			// begin drawing
		Background(0,0,0);
		Stroke(255,0,0,1);
		StrokeWidth(2);
		
		drawWaves();
		
		drawCursor();
		End(); 			// output to screen
	//END OF MAIN WHILE LOOP BELOW
	}	
// END OF MAIN BELOW
	return 0;
}

