#include <c8051F120.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>



#define SYSTEM_CLOCK        24500000        ///< System clock frequency
#define BAUD_RATE           115200
#define RX_BUFFER_SIZE      256
#define TX_BUFFER_SIZE      256

#define TX_WAIT_LIMIT       100 * (SYSTEM_CLOCK / 1000000)
#define RX_WAIT_LIMIT       100 * (SYSTEM_CLOCK / 1000000)

sfr16 RCAP2 = 0xCA;
sfr16 TMR2  = 0xCC;
sfr16 RCAP3 = 0xCA;
sfr16 TMR3  = 0xCC;
sfr16 RCAP4 = 0xCA;
sfr16 TMR4  = 0xCC;
sfr16 DAC0  = 0xD2;
sfr16 DAC1  = 0xD2;
sfr16 ADC0  = 0xBE;
sfr16 PCA0  = 0xF9;

#define INTERRUPT_Timer_0           1
#define INTERRUPT_Timer_1           3
#define INTERRUPT_UART_0            4
#define INTERRUPT_Timer_2           5
#define INTERRUPT_SMB               7
#define INTERRUPT_CPA               9
#define INTERRUPT_COMP0_FALLING     10
#define INTERRUPT_COMP0_RISING      11
#define INTERRUPT_COMP1_FALLING     12
#define INTERRUPT_COMP1_RISING      13
#define INTERRUPT_Timer_3           14
#define INTERRUPT_Timer_4           16
#define INTERRUPT_ADC_0             15
#define INTERRUPT_ADC_2             18



void portIOInit(void)
{
    char SFRPAGE_SAVE = SFRPAGE;
    SFRPAGE = CONFIG_PAGE;
    P0MDOUT   = 0x01;
    XBR0      = 0x04;
    XBR1      = 0x01;
    XBR2      = 0xC0;
    
    P0 = 0xFF;
    P1 = 0xFF;
    P2 = 0xFF;
    P3 = 0xFF;
    P4 = 0xFF;
    P5 = 0xFF;
    P6 = 0xFF;
    P7 = 0xFF;
}

void systemClockInit(void)
{
    char SFRPAGE_SAVE = SFRPAGE;
    int i = 0;
    SFRPAGE = CONFIG_PAGE;
    OSCICN = 0x83;
    SFRPAGE = SFRPAGE_SAVE;
}

void TimerInit(void)
{
    SFRPAGE   = TMR2_PAGE;
    TMR2CN    = 0x04;
    TMR2CF    = 0x08;
    RCAP2L    = 0xF3;
    RCAP2H    = 0xFF;
    TMR2      = RCAP2;
    TMR2CN    = 0x04;
}

void UARTInit(void)
{
    SFRPAGE   = UART0_PAGE;
    SCON0     = 0x50;
    SSTA0     = 0x05;
}

void InterruptsInit(void)
{
    IE        = 0x90;
}

void uart0Interrupt(void) interrupt INTERRUPT_UART_0 using 2
{
    // TODO: From Canvas
}

int main(void)
{
    systemClockInit();
    portIOInit();
    TimerInit();
    UARTInit();
    InterruptsInit();
    
    while(1) 
	{
        
	}	
}


    