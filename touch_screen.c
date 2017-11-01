#include "touch_screen.h"



void displayText(const char* fg, const char* bg, const unsigned char size, const char* message, const unsigned int x, const unsigned int y)
{
	char str[TS_BUFFER_SIZE];													// String

	delay(5);																	// Do not remove this delay
		
	sprintf(str, "S %s %s\r", fg, bg);											// Set forground and background color
	sendCommand(str);														
	sprintf(str, "f %s\r", Font[size]);											// Set text font
	sendCommand(str);														
	sprintf(str, "t \"%s\" %u %u\r", message, x, y); 							// Display text
	sendCommand(str);														
}

void showBitmap(const unsigned int index, const unsigned int x, const unsigned int y)
{
	char str[TS_BUFFER_SIZE];

	sprintf(str, "xi %u %u %u\r", index, x, y);									// Bitmap index
	sendCommand(str);
}

void changeScreen(const unsigned char screenIndex)
{
	callMacro(screenIndex);														// Change screen		
}

void callMacro(const unsigned int macroNumber)
{
	char str[TS_BUFFER_SIZE];

	sprintf(str, "m %u\r", macroNumber);										// Execute macro number
	sendCommand(str);
}