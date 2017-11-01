#include "touch_screen.h"



//-------------------------------------------------------------------------------------------------------
// Function Name: displayText
// Return Value: None 
// Parmeters: fg, bg, size, message, x, y
// Function Description: This function displays a text on the touch screen
//-------------------------------------------------------------------------------------------------------
void displayText(const char* fg, const char* bg, const unsigned char size, const char* message, const unsigned int x, const unsigned int y);

//-------------------------------------------------------------------------------------------------------
// Function Name: showBitmap
// Return Value: None 
// Parmeters: index, x, y (bitmap index and coordinates)
// Function Description: This function displays a bitmap image
//-------------------------------------------------------------------------------------------------------
void showBitmap(const unsigned int index, const unsigned int x, const unsigned int y);

//-------------------------------------------------------------------------------------------------------
// Function Name: changeScreen
// Return Value: None 
// Parmeters: screenIndex (macro number)
// Function Description: This function switches to the new screen
//-------------------------------------------------------------------------------------------------------
void changeScreen(const unsigned char screenIndex);

//-------------------------------------------------------------------------------------------------------
// Function Name: callMacro
// Return Value: None 
// Parmeters: macroNumber (macro number in the macro file)
// Function Description: This function calls a macro
//-------------------------------------------------------------------------------------------------------
void callMacro(const unsigned int macroNumber);