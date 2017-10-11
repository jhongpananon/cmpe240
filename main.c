#include <c8051F120.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>



#define SYSTEM_CLOCK        24500000

// Initialize the system clock
static void systemClockInit(void);



int main(void)
{
    systemClockInit();
    
    while(1) 
	{
        
	}	
}

static void systemClockInit(void)
{
    OSCICN = 0x83;
}