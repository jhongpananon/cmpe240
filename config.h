#ifndef CONFIG_H_
#define CONFIG_H_

#include <c8051f120.h>
#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <absacc.h>

#define     SYSTEM_CLOCK                24500000
#define 	BAUD_RATE		            115200
#define		RX_BUFFER_SIZE	            256
#define		TX_BUFFER_SIZE	            256

#define		TX_WAIT_LIMIT				100 * (SYSTEM_CLOCK / 1000000)		// Waiting limit for TI0 and TI1 flags to be set (10 microseconds max)
#define		RX_WAIT_LIMIT				100 * (SYSTEM_CLOCK / 1000000)		// Waiting limit for TI0 and TI1 flags to be set (10 microseconds max)

sfr16   	RCAP2   					= 0xCA;              				// Timer 2 capture/reload
sfr16   	TMR2    					= 0xCC;                 	        // Timer 2
sfr16   	RCAP3   					= 0xCA;                             // Timer 3 reload registers
sfr16   	TMR3    					= 0xCC;                             // Timer 3 counter registers
sfr16   	RCAP4   					= 0xCA;                             // Timer 3 reload registers
sfr16		TMR4						= 0xCC;								// Timer 4
sfr16 		DAC0    					= 0xD2;                 			// DAC0 data
sfr16 		DAC1    					= 0xD2;                 			// DAC1 data
sfr16 		ADC0    					= 0xBE;                 			// ADC0 data
sfr16		PCA0						= 0xF9;								// PCA register

#define 	INTERRUPT_Timer_0 			1
#define 	INTERRUPT_Timer_1 			3
#define 	INTERRUPT_UART_0 			4
#define 	INTERRUPT_Timer_2 			5
#define		INTERRUPT_SMB				7
#define		INTERRUPT_CPA				9
#define		INTERRUPT_COMP0_FALLING		10
#define 	INTERRUPT_COMP0_RISING		11
#define		INTERRUPT_COMP1_FALLING		12
#define		INTERRUPT_COMP1_RISING		13
#define 	INTERRUPT_Timer_3 			14
#define 	INTERRUPT_Timer_4 			16
#define 	INTERRUPT_ADC_0 			15
#define 	INTERRUPT_ADC_2 			18
#define		INTERRUPT_UART_1			20

#endif /* CONFIG_H */