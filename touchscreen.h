#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_

#include "config.h"


void sendCommand(const char * s);

/**
 * Handles setting the text on the touch screen
 * @param fg        Foreground color
 * @param bg        Background color
 * @param size      The size of the text
 * @param message   Pointer to the message to be displayed
 * @param x         The x-coordinate
 * @param y         The y-coordinate
 */
void display_text(const char * fg, const char * bg, const unsigned char size, const char * message, const char * x, const char * y);

#endif /* TOUCHSCREEN_H_ */