#include <c8051F120.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>



#define SYSTEM_CLOCK        24500000        ///< System clock frequency

/// Initialize the IO ports
static void portIOInit(void);

// Initialize the system clock
static void systemClockInit(void);



int main(void)
{
    portIOInit();
    systemClockInit();
    
    while(1) 
	{
        
	}	
}

static void portIOInit(void)
{
    XBR0 = 0x04;
    XBR2 = 0x40;
}

static void systemClockInit(void)
{
    OSCICN = 0x83;
}