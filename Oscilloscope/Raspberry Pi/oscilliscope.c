#include <shapes.h>
#include <fontinfo.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <libusb.h>

int mode = 0;
int triggerLevel = 2500;
int triggerChannel = 1;
int slope = 0;
int xScale = 1000;
int yScale = 1000;
int width = 0;
int height = 0; 
int xDivisions = 10;
int yDivisions = 10;
char run[1] = {1};
int chan1FirstX;
int chan1FirstY;
int chan1SecondX;
int chan1SecondY;
int chan2FirstX;
int chan2FirstY;
int chan2SecondX;
int chan2SecondY;
int oldSlope = slope;
int chan1CheckSlope;
int chan2CheckSlope;
int triggerValue = (430.5f * ((float)(triggerLevel)/1000.0f) * 1000.0f)/(4.0f * (float)(yScale));
int startSecond;


void stopWave2()			// this function draws the zeros that occur when the Pi tells the PSOC to stop running
{
	Stroke(0,255,255,1);			// change color to teal
	for (i = 64; i < sizeof(chan2Buffer) + 64; i++)			// iterate through chan2 data
	{
		chan2FirstX = xTransform(i - 64, samplesSec, xScale, xDivisions);			// first x coordinate of chan 2 data
		chan2FirstY = yTransform(chan2Buffer[j], yScale, yDivisions);			// first y coordinate of chan 2 data
		chan2SecondX = xTransform(i + 1 - 64, samplesSec, xScale, xDivisions);			// second x coordinate of chan 2 data
		chan2SecondY = yTransform(chan2Buffer[++j], yScale, yDivisions);			// second y coordinate of chan 2 data
		chan2CheckSlope = (chan2SecondY - chan2FirstY);			// slope of data at this point
		if (			// checks the trigger and slope conditions
			slope == 0 &&
			chan2SecondY >= triggerValue - 1 && 
			chan2FirstY <= triggerValue + 1 && 
			chan2CheckSlope > 0
		   )
		{
			slope = 2;			// trigger condition was found so to start print set slope flag to 2
			startSecond = i + 1;			// save index at which to print first wave at
		}
		if (			// checks trigger and slope conditions
			slope == 1 &&
			chan2SecondY <= triggerValue - 1 &&
			chan2FirstY >= triggerValue + 1 &&
			chan2CheckSlope < 0
		   )
		{
			slope = 2;			// negative trigger condition was found so start to print and set slope flag to 2
			startSecond = i + 1;			// sive index in which to print first wave at
		}
		else if (slope != 2)
		{
			i--;			// didnt find trigger condition so decrement where i start to print at to begin at left side of screen
		}
		if (i >= stopDraw)			// drawn enough samples on the screen so i stop and draw other wave
		{
			
			Stroke(255,0,0,1);			// change line color to red
			for (i = 0; i < sizeof(chan1Buffer); i++)			// iterate through first waves data and draw it
			{
				chan1FirstX = xTransform(i, samplesSec, xScale, xDivisions);
				chan1FirstY = yTransform(chan1Buffer[startSecond], yScale, yDivisions);
				chan1SecondX = xTransform(i + 1, samplesSec, xScale, xDivisions);
				chan1SecondY = yTransform(chan1Buffer[++startSecond], yScale, yDivisions);
				Line(chan1FirstX, chan1FirstY + (pot1Buffer[0] * height/255),
					 chan1SecondX, chan1SecondY + (pot1Buffer[0] * height/255));
				Stroke(0,255,255,1);	
				Line(chan1FirstX, chan1FirstY + (pot2Buffer[0] * height/255),
					 chan1SecondX, chan1SecondY + (pot2Buffer[0] * height/255));
				Stroke(255,0,0,1);
				if ( i >= stopDraw)			// drawn enough data to fill screen so stop.
				{
					break;
				}
			}
			slope = oldSlope;			// reset slope flag so that i contiuously print data
			break;
		}
	}
	End();			// display data on the screen
}

void stopWave1()
{
	for (i = 64; i < sizeof(chan1Buffer) + 64; i++)			// iterate through channel 1 data
	{
		chan1FirstX = xTransform(i - 64, samplesSec, xScale, xDivisions);
		chan1FirstY = yTransform(chan2Buffer[j], yScale, yDivisions);
		chan1SecondX = xTransform(i + 1 - 64, samplesSec, xScale, xDivisions);
		chan1SecondY = yTransform(chan2Buffer[++j], yScale, yDivisions);
		chan1CheckSlope = (chan1SecondY - chan1FirstY);
		if (			// check for trigger conditions
			slope == 0 &&
			chan1SecondY >= triggerValue - 1 && 
			chan1FirstY <= triggerValue + 1 && 
			chan1CheckSlope > 0
		   )
		{
			slope = 2;			// set slope flag to 2 to draw data
			startSecond = i + 1;			// save index to begin printing other wave at
		}
		if (			// check for negative trigger conditions
			slope == 1 &&
			chan1SecondY <= triggerValue - 1 &&
			chan1FirstY >= triggerValue + 1 &&
			chan1CheckSlope < 0
		   )
		{
			slope = 2;			// set slope flag to 2 to draw data	
			startSecond = i + 1;			// save index to begin printing other wave at
		}
		else if (slope != 2)			// didnt find trigger condition so decrement where i start to print at to begin at left side of screen
		{
			i--;
		}
		if (i >= stopDraw)			// drew anough samples for screen so stop
		{
			
			Stroke(0,255,255,1);			// change line color to teal
			for (i = 0; i < sizeof(chan2Buffer); i++)			// iterate through channel 2 data
			{
				chan2FirstX = xTransform(i, samplesSec, xScale, xDivisions);
				chan2FirstY = yTransform(chan2Buffer[startSecond], yScale, yDivisions);
				chan2SecondX = xTransform(i + 1, samplesSec, xScale, xDivisions);
				chan2SecondY = yTransform(chan2Buffer[++startSecond], yScale, yDivisions);	
				Line(chan2FirstX, chan2FirstY + (pot2Buffer[0] * height/255),
					 chan2SecondX, chan2SecondY + (pot2Buffer[0] * height/255));
				
				Stroke(255,0,0,1);
				Line(chan2FirstX, chan2FirstY + (pot1Buffer[0] * height/255),
					 chan2SecondX, chan2SecondY + (pot1Buffer[0] * height/255));
				Stroke(0,255,255,1);
				if ( i >= stopDraw)			// drew anough samples for screen so stop
				{
					break;
				}
			}
			slope = oldSlope;			// reset slope flag
			break;
		}
	}
	End();
}

void drawTriggerWave1()			// fucntion to draw when trigger channel is channel 1
{
	for (i = 64; i < sizeof(chan1Buffer) + 64; i++)			// iterate through channel 1 data
	{
		chan1FirstX = xTransform(i - 64, samplesSec, xScale, xDivisions);
		chan1FirstY = yTransform(chan1Buffer[j], yScale, yDivisions);
		chan1SecondX = xTransform(i + 1 - 64, samplesSec, xScale, xDivisions);
		chan1SecondY = yTransform(chan1Buffer[++j], yScale, yDivisions);
		chan1CheckSlope = (chan1SecondY - chan1FirstY);				// slope of data at any given point
		if (			// check for positive trigger/slope condition
			slope == 0 &&
			chan1SecondY >= triggerValue - 1 && 
			chan1FirstY <= triggerValue + 1 && 
			chan1CheckSlope > 0
		   )
		{					slope = 2;			// set slope flag to 2 to begin drawing
			startSecond = i + 1;				// save index to begin drawing other wave at
		}
		if (			// check for negative trigger/slope condition
			slope == 1 &&
			chan1SecondY <= triggerValue - 1 &&
			chan1FirstY >= triggerValue + 1 &&
			chan1CheckSlope < 0
		   )
		{
			slope = 2;
			startSecond = i + 1;
		}
		else if (slope != 2)			// trigger not found so begin printing farther to left of screen to keep at beginning of screen
		{
			i--;
		}
		if (slope == 2)			// trigger flasg was set so draw all data after that point
		{
			Line(chan1FirstX,chan1FirstY + (pot1Buffer[0] * height/255),
				 chan1SecondX,chan1SecondY + (pot1Buffer[0] * height/255));
		}
		if (i >= stopDraw)			// drew enough to fit screen so begin drawing second wave
		{
			
			Stroke(0,255,255,1);			// change to corect wave color
			for (i = 0; i < sizeof(chan2Buffer); i++)			// iterate through channel 2 data and draw it
			{
				chan2FirstX = xTransform(i, samplesSec, xScale, xDivisions);
				chan2FirstY = yTransform(chan2Buffer[startSecond], yScale, yDivisions);
				chan2SecondX = xTransform(i + 1, samplesSec, xScale, xDivisions);
				chan2SecondY = yTransform(chan2Buffer[++startSecond], yScale, yDivisions);
				Line(chan2FirstX, chan2FirstY + (pot2Buffer[0] * height/255),
					 chan2SecondX, chan2SecondY + (pot2Buffer[0] * height/255));
				//End();
				if ( i >= stopDraw)
				{
					break;
				}
			}
			
			slope = oldSlope;			// reset slope flag
			break;
		}
	}
	End();			// output to screen
}

void drawTriggerWave2)()			//function to draw when trigger channel is 2
{
	Stroke(0,255,255,1);				// change to correct wave color
	for (i = 64; i < sizeof(chan2Buffer) + 64; i++)			// iterate through channel 2 data
	{
		chan2FirstX = xTransform(i - 64, samplesSec, xScale, xDivisions);
		chan2FirstY = yTransform(chan2Buffer[j], yScale, yDivisions);
		chan2SecondX = xTransform(i + 1 - 64, samplesSec, xScale, xDivisions);
		chan2SecondY = yTransform(chan2Buffer[++j], yScale, yDivisions);
		chan2CheckSlope = (chan2SecondY - chan2FirstY);			// slope of channel 2 at any point
		if (			// check for positive slope/trigger condition
			slope == 0 &&
			chan2SecondY >= triggerValue - 1 && 
			chan2FirstY <= triggerValue + 1 && 
			chan2CheckSlope > 0
		   )
		{
			slope = 2;			// set slop flag to 2 to print data
			startSecond = i + 1;			// save index to begin other wave at
		}
		if (			// check for negative slope/trigger condition
			slope == 1 &&
			chan2SecondY <= triggerValue - 1 &&
			chan2FirstY >= triggerValue + 1 &&
			chan2CheckSlope < 0
		   )
		{
			slope = 2;
			startSecond = i + 1;
		}
		else if (slope != 2)			// trigger not found so decrement where to print from
		{
			i--;
		}
		if (slope == 2)			// trigger flag set so draw all values
		{
			Line(chan2FirstX,chan2FirstY + (pot2Buffer[0] * height/255),
				 chan2SecondX,chan2SecondY + (pot2Buffer[0] * height/255));
		}
		if (i >= stopDraw)			// drew enough samples so draw other wave
		{
			
			Stroke(255,0,0,1);
			for (i = 0; i < sizeof(chan1Buffer); i++)			// iterate through channle 1 data
			{
				chan1FirstX = xTransform(i, samplesSec, xScale, xDivisions);
				chan1FirstY = yTransform(chan1Buffer[startSecond], yScale, yDivisions);
				chan1SecondX = xTransform(i + 1, samplesSec, xScale, xDivisions);
				chan1SecondY = yTransform(chan1Buffer[++startSecond], yScale, yDivisions);
				Line(chan1FirstX, chan1FirstY + (pot1Buffer[0] * height/255),
					 chan1SecondX, chan1SecondY + (pot1Buffer[0] * height/255));
				if ( i >= stopDraw)
				{
					break;
				}
			}
			
			slope = oldSlope;			// reset slope flag
			break;
		}
	}
	End();			// display onto screen
}

void drawFreeWave()			// functionn to draw when mode is set to free
{
	for (i = 64; i < sizeof(chan1Buffer) + 64; i++)			// iterate through and draw data from channel 1
	{
		chan1FirstX = xTransform(i - 64, samplesSec, xScale, xDivisions);
		chan1FirstY = yTransform(chan1Buffer[j], yScale, yDivisions);
		chan1SecondX = xTransform(i + 1 - 64, samplesSec, xScale, xDivisions);
		chan1SecondY = yTransform(chan1Buffer[++j], yScale, yDivisions);
		Line(chan1FirstX, chan1FirstY + (pot1Buffer[0] * height/255),
			 chan1SecondX, chan1SecondY + (pot1Buffer[0] * height/255));
		if (i >= stopDraw)
		{
			break;
		}
	}
	j = 64;
	Stroke(0,255,255,1);
	for (i = 64; i < sizeof(chan2Buffer) + 64; i++)			// iterate through and draw data from channel 2
	{
		chan2FirstX = xTransform(i - 64, samplesSec, xScale, xDivisions);
		chan2FirstY = yTransform(chan2Buffer[j], yScale, yDivisions);
		chan2SecondX = xTransform(i + 1 - 64, samplesSec, xScale, xDivisions);
		chan2SecondY = yTransform(chan2Buffer[++j], yScale, yDivisions);
		Line(chan2FirstX, chan2FirstY + (pot2Buffer[0] * height/255),
			 chan2SecondX, chan2SecondY + (pot2Buffer[0] * height/255));
		if (i >= stopDraw)
		{
			break;
		}
		
	}
	End();			// display to screen
}

void printText()			// function to write text for xScale and yScale
{
	setfill(color);	
	if (xScale >= 1000)
	{
		sprintf(str, "X scale =  %0d ms/div", xScale/1000);
	}
	else 
	{
		sprintf(str, "X scale =  %0d us/div", xScale);
	}
	Text(width - 300, height - 50, str, SansTypeface, 18);
	sprintf(str, "Y scale = %3.2f V/div", yScale * 0.25);
	Text(width - 300, height - 100, str, SansTypeface, 18);
}

void grid(VGfloat x, VGfloat y, // Coordinates of lower left corner
	  int nx, int ny, // Number of x and y divisions
	  int w, int h) // screen width and height
{
	VGfloat ix, iy;
	Stroke(128, 128, 128, 0.5); // Set color
	StrokeWidth(2); // Set width of lines
	for (ix = x; ix <= x + w; ix += w/nx) 
	{
		Line(ix, y, ix, y + h);
	}

	for (iy = y; iy <= y + h; iy += h/ny) 
	{
    	Line(x, iy, x + w, iy);
	}
}


int xTransform(float sample,float samplesSec, float xScale, float xDivisions)			// function to do xScale, its just dimensional analysis sample -> seconds -> division -> pixels
{
	float xResult = sample;
	xResult = xResult * (1.0f/samplesSec);
	xResult = xResult * (1.0f/(xScale/1000000.0f));
	xResult = xResult * ((float)width/(float)xDivisions);
	return xResult;
} 

int yTransform (float value, int yScale, float yDivisions)				// function to do yScale, its just dimensional analysis dValue -> division -> pixels
{
		float yResult = value;
		yResult = yResult * 1.0f/(yScale/18.5f);
		yResult = yResult * (float)height/(float)yDivisions;
		return yResult;
}

int main (int argc, char* argv[]){
	
	
	libusb_device_handle* dev; // Pointer to data structure representing USB device
	char rxBuffer[64]; // Receive data block
	int receivedByte; // Bytes received
	int i = 0;	
	int ch = 0;
	
	
	libusb_init(NULL); // Initialize the LIBUSB library

	// Open the USB device (the Cypress device has
	// Vendor ID = 0x04B4 and Product ID = 0x8051)
	dev = libusb_open_device_with_vid_pid(NULL, 0x04B4, 0x8051);
	
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
  
	while ((ch = getopt(argc, argv, "m:t:s:c:x:y:r:")) != -1)			// while loop that iterates through user arguments and sets them accordingly
	{
		switch(ch)
		{
			case 'm':
	  			if (strcmp(optarg, "free") != 0 && strcmp(optarg, "trigger") != 0)
	    			{
					printf("-m must be \"free\" or \"trigger\"\n");
					return -1;
				}
				else if (strcmp(optarg, "trigger") == 0)
				{
					mode = 0;
				}	
				else if(strcmp(optarg, "free") == 0)
				{
					mode = 1;
				}
				break;

			case 't':
	  			triggerLevel = atoi(optarg);
	  			if (triggerLevel % 100 != 0 || triggerLevel > 5000 || triggerLevel < 0)
	    		{
	      			printf("triggerLevel must be 0-5000 and divisible by 100\n");
					return -1;
				}
				break;	
			
			case 's':
				if (strcmp(optarg, "pos") == 0)
		    	{
	      			//slope = pos;
					slope = 0;
	    		}
	  			else if (strcmp(optarg, "neg") == 0)
	    		{
	      			//slope = neg;
					slope = 1;
	    		}
				else
				{
					printf("slope must be \"pos\" or \"neg\"\n");
					return -1;
				}
				break;

			case 'c':
	  			triggerChannel = atoi(optarg);
	  			if (!(triggerChannel == 1 || triggerChannel == 2))
	    			{
	      			printf("channel must be 1 or 2\n");
	    			return -1;
				}
				break;

			case 'x':
				xScale = atoi(optarg);
				if (!(xScale == 1 || xScale == 10 || xScale == 100 || xScale == 500 || xScale == 1000 || 
					 xScale == 2000 || xScale == 5000 || xScale == 10000 || xScale == 50000 ||
					  xScale == 100000))
				{
	      			printf("-x must be 1,10,500,1000,2000,5000,10000,50000,100000\n");
	    			return -1;
				}
				break;

	  		case 'y':
				//char yTextString[sizeof(optarg)] = optarg;
				yScale = atoi(optarg);
	      		if (!(yScale == 100 || yScale == 500 || yScale == 1000 ||
			      	  yScale == 2000 || yScale == 2500))
				{
					printf("-y must be 100,500,1000,2000,25000");
					return -1;	
				}
				break;
			case 'r':
				run[0] = atoi(optarg);
				if (!(run[0] == 0 || run[0] == 1))
				{
					fprintf(stderr,"-r must be either 0 or 1");
					return -1;
				}
				break;
			default:
				break;
  		}
	}
		 	
	// Clear receive buffer
	for (i=0; i<64; i++)
	{
		rxBuffer[i] = 0;
	}

	int returnValue;	
	returnValue = libusb_bulk_transfer(dev, 0x05, run, 1, &sentByte, 0);  			// send run argument back to PSOC to either start or stop PSOC from gathering data

	init(&width, &height);			// initialize openVG function that get screens widht/height


	//int returnValue = 0;
	int samplesSec = 50000;			// should mathc DMA frequency
	char chan1Buffer[((samplesSec/10) * xDivisions) + 64];		// takes in maximum amount of samples eveytime in case of large xScale
	char chan2Buffer[((samplesSec/10) * xDivisions) + 64];
	int j = 0;
	char pot1Buffer[1];
	char pot2Buffer[1];
	int potReceived = 0;
	int sentByte = 0;

	char str[100];
	VGfloat color[4] = {255,255,0,1};
	char xTextString[10];
	char yTextString[10];

	// series of if statements that determine when to stop drawing as the screen will be filled
	if (slope == 1 && triggerLevel == 0)
	{
		slope = 0;
		oldSlope = slope;
	}
		
	if (xScale == 1 || xScale == 10 ||xScale == 100)
	{
		stopDraw = width/10;
	}
	else if (xScale == 500 || xScale == 1000)
	{
		stopDraw = width;
	}
	else if (xScale == 2000)
	{
		stopDraw = width*2;
	}
	else if (xScale == 5000)
	{
		stopDraw = width*3;

	}
	else if (xScale == 10000)
	{
		stopDraw = width*3;
	}
	else if (xScale == 50000)
	{
		stopDraw = width*13;
	}
	else if (xScale == 100000)
	{
		stopDraw = width * 26;
	}
	
	while(1)
	{
		// take in max amount of samples everytime
		returnValue = libusb_bulk_transfer(dev, (0x01|0x80), chan1Buffer, sizeof(chan1Buffer), &receivedByte, 0);			// receive channel 1 data
		returnValue = libusb_bulk_transfer(dev, (0x02|0x80), chan2Buffer, sizeof(chan2Buffer), &receivedByte, 0);			// receive channel 2 data
		returnValue = libusb_bulk_transfer(dev, (0x03|0x80), pot1Buffer, 1, &potReceived, 0);								// receive channel 1 pot data
		returnValue = libusb_bulk_transfer(dev, (0x04|0x80), pot2Buffer, 1, &potReceived, 0);								// receive channel 2 pot data		
			
		Start(width, height);			// begin drawing
 		Background(0,0,0); 				// draw black background
		grid(0,0,xDivisions,yDivisions,width, height); 			// draws grid
		printText();			// print text

		Stroke(255,0,0,0.5);			// set line color to red
		StrokeWidth(4);
		j = 64;
		// depending on use input draw either triggered wave on set channel or draw free waves
		if (mode == 0 && triggerChannel == 1 && run[0] == 1)
		{
			drawTriggerWave1();
		}
				
		else if (mode == 0 && triggerChannel == 2 && run[0] == 1)
		{
			drawTriggerWave2();
		}
		
		else if (mode == 1)
		{
			drawFreeWave();
		}

		else if (run[0] == 0 && triggerChannel == 1)
		{
			stopWave1();
		}
		
		else if (run[0] == 0 && triggerChannel == 2)
		{
			stopWave2();
		}	
		End();			// output to screen
	// END OF WHILE BELOW
	}	
	return 1;
}
