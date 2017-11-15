#ifndef _MAIN_H
#define _MAIN_H

#include <c8051f120.h>
#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <absacc.h>

//-------------------------------------------------------------------------------------------------------
// System clock information
//-------------------------------------------------------------------------------------------------------
#define 	SYSTEM_CLOCK				98000000
//-------------------------------------------------------------------------------------------------------
// Global declarations for 16-bit registers
//-------------------------------------------------------------------------------------------------------
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
sfr16 		PCA0CP0 					= 0xFB;                  			// PCA0 compare register (Low byte address)
sfr16 		PCA0CP1 					= 0xFD;                  			// PCA1 compare register (Low byte address)
sfr16		PCA0CP2      				= 0xE9;								// PCA2 compare register (Low byte address)
sfr16		PCA0CP3      				= 0xEB;								// PCA3 compare register (Low byte address)
sfr16		PCA0CP4      				= 0xED;								// PCA4 compare register (Low byte address)
sfr16		PCA0CP5      				= 0xE1;								// PCA5 compare register (Low byte address)

sbit 		SDA 						= P0^6;								// SMB data line
sbit 		SCL 						= P0^7;								// SMB clock line

sbit 		GND_INLCK 					= P2^0;
sbit 		INHIBIT 					= P2^1;
sbit 		OVER_VOLTAGE_MONITOR 		= P2^2;
sbit 		CHARGE_END_MONITOR 			= P2^3;
sbit 		OVER_TEMP_MONITOR 			= P2^4;
sbit 		SIMMER_ENABLE				= P2^5;
sbit 		IGBT_FIRE 					= P2^6;
sbit 		SHUTTER_ENABLE 				= P2^7;

sbit		INLCK_0						= P3^0;								// Interlock pin 1
sbit		INLCK_1						= P3^1;								// Interlock pin 2
sbit		OPTICAL_INLCK				= P3^2;
sbit		MAIN_PS_ON 					= P3^5;								// Main power on/off
sbit		BLAST_SHIELD_LED			= P3^6;								// Turn ON/OFF phototransistor LED for home position sensor
sbit		FLASHING_LED				= P3^7;

sbit 		FT_SW_LOW 					= P5^0;								// Normally open High
sbit 		FT_SW_HIGH 					= P5^1;								// Normally closed Low

sbit 		RHW 						= P7^7;								// SMB error line

#define		SWITCH_SAMPLES				3									// Number of samples to detect footswitch pressed

// DAC0 Constants
#define		DAC0_HIGH					0x0FFF
#define		DAC0_LOW					0x0000
#define		DAC1_HIGH					0x0FFF
#define		DAC1_LOW					0x0000

#define		VP_0_0_KV					0x0000
#define		VP_0_5_KV					0x02B0
#define		VP_1_0_KV					0x0502
#define		VP_1_5_KV					0x07AE
#define		VP_2_0_KV					0x0A40
#define		VP_2_5_KV					0x0CCA
#define		VP_3_0_KV					0x0FFF

#define		DAC_0_0V					0x0000
#define		DAC_0_8V					0x0502
#define		DAC_1_1V					0x0780
#define		DAC_1_5V					0x0A40
#define		DAC_2_0V					0x0DEA
#define		DAC_2_3V					0x0FFF

// Number of nanoseconds per tick (period from one tick to another) = 1,000,000,000 ns / SYSTEM_CLOCK
// 1 milisecond = 1,000,000 ns -> The number of ticks per milisecond = 1,000,000 / 1,000,000,000 / SYSTEM_CLOCK = SYSTEM_CLOCK / 1,000
// If the time base is SYSTEM_CLOCK / 4, the number of ticks per milisecond = SYSTEM_CLOCK / 4 / 1,000
// Similarly, the number of ticks per microsecond = SYSTEM_CLOCK / 1,000,000. The number of ticks per nanosecond = SYSTEM_CLOCK / 1,000,000,000
#define		NANOSEC_PER_TICK			(1000000000 / SYSTEM_CLOCK)			// Number of nanoseconds per tick if SYSTEM_CLOCK is time base
#define		MICROSEC_PER_TICK			(1000000 / SYSTEM_CLOCK)			// Number of microseconds per tick if SYSTEM_CLOCK is time base
#define		TICKS_PER_MICROSEC			(SYSTEM_CLOCK / 1000000)			// Number of ticks per microsecond if SYSTEM_CLOCK is time base
#define		TICKS_PER_MICROSEC_4		(SYSTEM_CLOCK / 4 / 1000000)		// Number of ticks per microsecond if SYSTEM_CLOCK / 4 is time base
#define		TICKS_PER_MILSEC_4			(SYSTEM_CLOCK / 4 / 1000)			// Number of ticks per miliseconnd if SYSTEM_CLOCK / 4 is time base
#define		MAX_CONFIRMED_WAIT			2
#define		MAX_RESULT_WAIT				2
#define		PULL_LOW_TRIGGER			500

#define		TICKS_MIC_OFFSET			500
#define		TICKS_MIL_OFFSET			0
#define		ELECTRICAL_OPTICAL_OFFSET	100
#define		PCA0_TIMER_TICKS_MIC		(SYSTEM_CLOCK / 4 / 1000000)
#define		PCA0_TIMER_TICKS_MIL		(SYSTEM_CLOCK / 4 / 1000)
									   	
//-------------------------------------------------------------------------------------------------------
// Definitions of the pages on the touch screen. They are the indexes.
//-------------------------------------------------------------------------------------------------------
#define 	SPLASH_PAGE					1									// Splash screen which is defined in touch screen macro
#define 	MAIN_PAGE					2
#define 	SETTINGS_PAGE				3
#define 	SERVICE_PAGE				4

#define		EQUATION_SLOP				0.001
#define		EQUATION_CONSTANT			0.135

#define		EQUATION_SLOP_350_5			0.001
#define		EQUATION_SLOP_350_7			0.001
#define		EQUATION_SLOP_350_10		0.001
#define		EQUATION_SLOP_350_12		0.001
#define		EQUATION_SLOP_350_15		0.001
#define		EQUATION_SLOP_350_20		0.002

#define		EQUATION_SLOP_700_5			0.001
#define		EQUATION_SLOP_700_7			0.001
#define		EQUATION_SLOP_700_10		0.001
#define		EQUATION_SLOP_700_12		0.001
#define		EQUATION_SLOP_700_15		0.001

#define		EQUATION_CONSTANT_350_5		-0.018
#define		EQUATION_CONSTANT_350_7		0.218
#define		EQUATION_CONSTANT_350_10	0.132
#define		EQUATION_CONSTANT_350_12	0.151
#define		EQUATION_CONSTANT_350_15	0.088
#define		EQUATION_CONSTANT_350_20	-0.141

#define		EQUATION_CONSTANT_700_5		-0.010
#define		EQUATION_CONSTANT_700_7		0.090
#define		EQUATION_CONSTANT_700_10	0.085
#define		EQUATION_CONSTANT_700_12	0.085
#define		EQUATION_CONSTANT_700_15	0.032

#define		START_INDEX					0
#define		TX_START_INDEX				0
#define		RX_START_INDEX				0

//-------------------------------------------------------------------------------------------------------
// Constants which define the buffers
//-------------------------------------------------------------------------------------------------------
#define		SCREEN_TRANSITION			250									// Set a delay when changing from one page to another in m1
																			// to avoid incorrect touch on just selected page
#define 	RX_BUFFER_SIZE 				512									// Receive buffer size

#ifdef		VERIFY_POWER_METER_CAL											// Power meter uses 200 int to store data		
#define 	TX_BUFFER_SIZE 				512								// Transmit buffer size (Short ring buffer)
#else
#define 	TX_BUFFER_SIZE 				1024								// Transmit buffer size (Long ring buffer)
#endif
// EEPROM Parameters
#define		EEPROM_ADDR_SIZE			2									// 2 byte address of EEPROM
#define		EEPROM_TX_BUFFER			512
#define		EEPROM_RX_BUFFER			512
#define		EEPROM_PAGE_SIZE			32

#define		PARAMETERS_SIZE				64									// Array size for parameters stored in flash	 
#define		SERIAL_NUMBER_SIZE			9									// Number of digits for serial number
#define 	BAUD_RATE     				115200 								// Baud rate which is set for communicating with touch screen
#define		ONE_CLOCK_TICK				1000000000 / SYSTEM_CLOCK			// Number of nanoseconds per clock tick: T sec = 1 sec / f Hz
#define		TICKS_PER_MILISEC			(SYSTEM_CLOCK / 1000)				// Number of ticks in 1 milisec = SYSTEM_CLOCK / 1,000 		

#define		BG							"0033CC"	                  		// Default text background
#define		FG							"FFFFFF"	                  	 	// Default text forground

#define		SPRINTF_SIZE				64									// Array size for sprintf()

#define 	SAMPLE_RATE  				50000             					// Sample frequency in Hz
#define 	INT_DEC      				256               					// Integrate and decimate ratio
#define 	SAR_CLK      				2500000           					// Desired SAR clock speed

#define 	BUFFER_SIZE					128									// General-purpose buffer size
#define		TS_BUFFER_SIZE				128									// Buffer size for touch screen commands

//-------------------------------------------------------------------------------------------------------
// General-purpose constants for logic processing
//-------------------------------------------------------------------------------------------------------
#define		NORMAL_MODE					0
#define		LASING_MODE					1
#define 	OTHER_MODE					2

#define		ERROR						0
#define		NORMAL						1
#define		WARNING						2

#define		YES							1
#define		NO							0
#define		HI							1
#define		LO							0
#define		ENABLE						1
#define		DISABLE						0
#define		SET							1
#define		CLEAR						0
#define		OK							1
#define		CANCEL						0
#define		RUN							1
#define		STOP						0
#define		PASSED						1
#define		FAILED						0
#define		PRESSED						1
#define		RELEASED					0
#define		UP							1
#define		DOWN						0
#define		ON							1
#define		OFF							0
#define     LOCKED                      1
#define     UNLOCKED                    0
#define		DONE						1
#define		NOT_YET						0
#define		VALID						1
#define		INVALID						0
#define		ALREADY_VERIFIED			1
#define		NOT_YET_VERIFIED			0
#define		ALREADY_PROVIDED			1
#define		NOT_YET_PROVIDED			0
#define		RIGHT_SIDE					1
#define		LEFT_SIDE					0
#define		TRUE						1
#define		FALSE						0
#define		EXIST						1
#define		NONE						0
#define		MANUAL						0
#define		AUTOMATIC					1
#define		LOW							0
#define		HIGH						1
#define		CCW							0
#define		CW							1
#define		STOP						0
#define		START						1
#define		DEFAULT						0
#define		CURRENT						1
#define		UNSPECIFIED					255
#define		SPECIFIED					1

#define		NEITHER						2

#define		FLASH_MEM					0
#define		INTERNAL_MEM				1
#define		EEPROM_0					2
#define		EEPROM_1					3
#define		EEPROM_2					4

//-------------------------------------------------------------------------------------------------------
// Aiming beam intensity and other constants
//-------------------------------------------------------------------------------------------------------
#define		MIN_END_POINT				0
#define		MAX_END_POINT				1

#define		SCALED_PROGRAM_VOLTAGE		0.90								// Start at 90%
#define		FULL_PROGRAM_VOLTAGE		1.00								// Stay with 100%
#define		PROGRAM_VOLTAGE_STEP		0.02								// Increment 2% per step
	
//-------------------------------------------------------------------------------------------------------
// SMB constants
//-------------------------------------------------------------------------------------------------------
#define  	MY_ADDR            			0x02                      	     	// Master address
#define  	SMB_FREQUENCY      			100000L                   	     	// Keep this frequency = 100 Khz to get fastest communication                                  
#define  	WRITE              			0x00                       	   		// Write operation
#define  	READ               			0x01                        	    // Read operation

#define  	MCU_SLAVE_ADDR     			0x20                          	 	// SMB address of slave board
#define		ONE_WIRE_BRIDGE_ADDR		0x30								// SMB address of one wire chip bridge DS2482
#define		REAL_TIME_CLOCK_ADDR		0xD0								// SMB address of real time clock DS3232
#define		SRAM_ADDR					0xD0								// SMB address of real time clock DS2323 SRAM
#define		EEPROM_ADDR					0xA4								// SMB address of EEPROM
#define		WAVEFORM_GEN_ADDR			0xB0								// SMB address of waveform generator
#define		DIGITAL_POT_ADDR			0x5E								// SMB address of volume generator
#define		DEVICE_DUMP_ADDR			0xAB								// SMB dump address only
                                                            		
#define  	SMB_BUS_ERROR      			0                          		  	// Bus error for all modes
#define  	SMB_START          			1                          		  	// Master initiates a transter, STA signal transmitted
#define  	SMB_REPEAT_START       		2                          		  	// Master repeats sending STA signal to slave
#define  	SMB_ADDR_W_TX_ACK_RX       	3                           	 	// Master transmitted Slave address + W; and ACK received
#define  	SMB_ADDR_W_TX_NACK_RX      	4                            		// Master transmitted Slave address + W; and NACK received
#define  	SMB_DATA_TX_ACK_RX        	5                            		// Master transmitted data byte; and ACK received
#define  	SMB_DATA_TX_NACK_RX       	6                         		   	// Master transmitted data byte; and NACK received
#define  	SMB_ARBITRATION_LOST      	7                          		  	// Master transmitter; arbitration lost
#define  	SMB_ADDR_R_TX_ACK_RX       	8                          	  		// Master transmitted Slave address + R; and ACK received
#define  	SMB_ADDR_R_TX_NACK_RX      	9                    				// Master transmitted Slave address + R; and NACK received
#define  	SMB_DATA_RX_ACK_TX        	10                        			// Master received data byte; ACK transmitted
#define  	SMB_DATA_RX_NACK_TX       	11                          	  	// Master received data byte; NACK transmitted

#define  	S_SMB_ADDR_W_RX_ACK_TX   	12                      			// Slave received its own address for write; ACK transmitted
#define	 	S_SMB_ADDR_W_TX_LOST		13									// Slave arbitration lost; slave address + W received and ACK sent
#define  	S_SMB_ADDR_WG_RX_ACK_TX  	14                          		// General call address received: ACK transmitted
#define	 	S_SMB_GENERAL_CALL_LOST		15									// Slave arbitration loast; general call received, ACK sent
#define  	S_SMB_DATA_RX_ACK_TX		16                          		// Slave received data byte; ACK transmitted
#define  	S_SMB_DATA_RX_NACK_TX    	17                          		// Slave received data byte; NACK transmitted
#define  	S_SMB_DATAG_RX_ACK_TX		18                          		// Slave received data byte under general call; ACK transmitted
#define  	S_SMB_DATAG_RX_NACK_TX		19                         			// Slave received data byte under general call; NACK transmitted
#define  	S_SMB_STOP_REPEAT_START  	20                     				// Slave received STOP or REPEAT START

#define  	S_SMB_ADDR_R_RX_ACK_TX   	21                      			// Slave received its own address for read; ACK transmitted
#define  	S_SMB_ADDR_R_RX_LOST		22									// Slave received its own address for read; ACK transmitted
#define  	S_SMB_DATA_TX_ACK_RX    	23                          		// Slave transmitted data byte; ACK received
#define  	S_SMB_DATA_TX_NACK_RX    	24                          		// Slave transmitted data byte; NACK received
#define  	S_SMB_LAST_TX_ACK_RX   		25                          		// Slave transmitted last data byte; ACK received
#define  	S_SMB_CLOCK_HI_TIMEOUT		26                          		// SCL clock high timeout
																			
																			// 1 clock tick takes (1000,000,000 / SYSTEM_CLOCK) ns
																			// No. of ticks in 1 sec = 1000,000,000 ns / (1 clock tick in ns)
																			// --> No. of ticks in 1 sec = 1000,000,000 / 1000,000,000 / SYSTEM_CLOCK
																			// --> No. of ticks in 1 sec = SYSTEM_CLOCK
																			// --> No. of ticks in 20 sec = 20 * SYSTEM_CLOCK
#define	 	TOUCH_SCREEN_POLLING_LIMIT	30000000							// 30 seconds
#define		ADC_DELAY_LIMIT				(SYSTEM_CLOCK / 1000)			
#define		MAX_SMB_RETRY_LIMIT			5 									// Maximum number of times to check SMB communication
#define		MAX_DISPLAY_COUNTER			5									// Maximum time to display a message and then move to another one
#define	 	SMB_POLLING_LIMIT			100000								// Polling any response from slave board 1 second
#define		DELAY_LIMIT					(TICKS_PER_MILISEC * 2)				// Delay 2 miliseconds max
#define		TX_WAIT_LIMIT				100 * (SYSTEM_CLOCK / 1000000)		// Waiting limit for TI0 and TI1 flags to be set (10 microseconds max)
#define		RX_WAIT_LIMIT				100 * (SYSTEM_CLOCK / 1000000)		// Waiting limit for TI0 and TI1 flags to be set (10 microseconds max)
#define	 	USB_POLLING_LIMIT			1000000								// Polling limit for USB communication
#define  	UART0_POLLING_LIMIT			1000000								// Polling limit for UART0 communication
#define	 	DEFAULT_SETTINGS_LIMIT		1000000								// Default polling limit = 30 seconds
#define	 	MAX_NACK_RETRY				10									// Number of times to retry slave board response if NACK is received								
#define  	BYTES_OUT_MAX      			128                         	   	// Number of bytes to write
                                                            				// Master -> Slave
#define  	BYTES_IN_MAX       			128                        	    	// Number of bytes to read
                                                            				// Master <- Slave
#define		MAX_SMB_TIMEOUT				3									// Reset SMB if data line has been pulled low for a period of time in seconds                 	

//------------------------------------------------------------------------------------------------------
// Addresses are shared between the master board and the slave board
//------------------------------------------------------------------------------------------------------
#define		ADDRESS_OFFSET				0x2C								// Substract this offset to get an array index (sharedData[])
																			// Because sharedData[] starts at 0x2C
#define		MASTER_FLAGS_1				0x2C								// Flags bits on master board (High byte)
#define		MASTER_FLAGS_0				0x2D								// Flag bits on master board (Low byte)
#define		SLAVE_FLAGS_1				0x2E								// Flag bit on slave board (High byte)
#define		SLAVE_FLAGS_0				0x2F								// Flag bit on slave board (Low byte)

#define		SLAVE_SW_VERSION			0x30								// Slave software version (2 bytes)
#define     AIR_FLOW_VOLTAGE			0x32								// Air flow voltage (2 bytes)
#define		PRESSURE_VOLTAGE			0x34								// Pressure in mV (2 bytes)
#define     ROOM_TEMP					0x36								// Room temp (1 byte)
#define		WATER_IN_TEMP				0x37								// Water in temp (1 byte)
#define		WATER_OUT_TEMP				0x38								// Water out temp (1 byte)
#define		INTERNAL_ENERGY				0x39								// Internal energy reading (2 byte)
#define		FAN_DUTY_CYCLE				0x3B								// Fan speed (1 byte)
#define		PULSEWIDTH					0x3C								// Pulsewidth (2 bytes)
#define		REP_RATE					0x3E								// Rep rate (1 byte)
#define		PULSE_ENERGY				0x3F								// Energy per pulse (1 byte)
#define		TOTAL_ENERGY				0x40								// Total energy (4 bytes)

#define		SLAVE_ALIVE					0x50								// Check to see if slave is still alive
#define     AIMING           			0x51								// Aiming intensity (1 byte)
#define     SIMMER_V	           		0x52								// Simmer voltage (2 byte)
#define		PROGRAM_V					0x54								// Program voltage(2 byte)
#define     USER_SET_POWER         		0x56								// Set power (1 byte)
#define		SCREEN_NUMBER				0x57								// Touch screen page location (ex. Splash = 1, Main = 2 ...)
#define		FIBER_TYPE_SIZE				0x58							   	// Fiber type and size (1 byte)

#define		SOUND_COMMAND				0x60								// Command
#define		SOUND_WAVEFORM				0x61								// Waveform
#define		SOUND_FREQUENCY				0x62								// Frequency of speaker
#define		SOUND_DURATION				0x63								// Duration of speaker (2 bytes)
#define		SOUND_VOLUME				0x65								// Volume of speaker (1 byte)

#define		SHARED_DATA_MAX				0x7F								// Max number of devices connected to slave (127)


#define		LASING_BEEP_FREQ			2590								// Beep frequency on touch screen when lasing
#define		NORMAL_BEEP_FREQ			2700								// Beep frequency on touch screen for normal operations

#define		MAX_ERROR_RESET_COUNT		10									// Delay in seconds to clear non-critical errors
#define		MAX_FOOTSWITCH_COUNT		1									// Skip first reading from slave. Start displaying bars in second reading 
#define		POWER_PRECISION				0.0001

//-------------------------------------------------------------------------------------------------------
// MainSupports.c constants: These constants are used in MainSupport.c file which supports the activities of the main page
//-------------------------------------------------------------------------------------------------------
#define		UNUSED_FG					"FF00FF"							// This color helps reset forground color in touchScreen.c
#define		UNUSED_BG					"FF00FF"							// This color helps reset background color in touchScreen.c
#define		UNUSED_SIZE					-1									// This color helps reset font size in touchScreen.c
																			// Do not use this color (0xFF00FF) in any context
#define		MAIN_MSG_FG					"FFFFFF"							// Status message foreground on cw mode page
#define		MAIN_MSG_BG					"000099"					   		// Status message background on cw mode page
#define		MAIN_MSG_FONT				1									// Font size for status message in status bar
#define		MAIN_MSG_X					22									// Status message on main page x
#define		MAIN_MSG_Y					425									// Status message on main page y (small display)

#define		READY_MSG_FG				"FFFFFF"							// Status message foreground on cw mode page
#define		READY_MSG_BG				"0033CC"					   		// Status message background on cw mode page
#define		READY_MSG_FONT				1									// Font size for status message in status bar
#define		READY_MSG_X					35									// Status message on main page x
#define		READY_MSG_Y					420									// Status message on main page y

#define		LASING_MSG_FG				"FF0000"							// Status message foreground on cw mode page
#define		LASING_MSG_BG				"0033CC"					   		// Status message background on cw mode page
#define		LASING_MSG_FONT				2									// Font size for status message in status bar
#define		LASING_MSG_X				30									// Status message on main page x
#define		LASING_MSG_Y				429									// Status message on main page y

#define		CHARGING_MESSAGE_BITMAP		134
#define		CHARGING_MESSAGE_X			200									// Status message on main page x
#define		CHARGING_MESSAGE_Y			150									// Status message on main page y

#define		LASING_TEXT_BITMAP			35									// Status message foreground on cw mode page
#define		LASING_TEXT_CLEAR			36
#define		LASING_TEXT_X				170									// Status message on main page x
#define		LASING_TEXT_Y				292									// Status message on main page y

#define		TASK_NUMBER_MSG_FG			"FFFFFF"							// Status message foreground on cw mode page
#define		TASK_NUMBER_MSG_BG			"0033CC"					   		// Status message background on cw mode page
#define		TASK_NUMBER_MSG_FONT		1									// Font size for status message in status bar
#define		TASK_NUMBER_MSG_X			350									// Status message on main page x
#define		TASK_NUMBER_MSG_Y			437									// Status message on main page y (small display)

#define		RED_LED						205
#define		BLANK_RED_LED				206
#define		RED_LED_X					188
#define		RED_LED_Y					300
#define		RED_LED_OFFSET				94
#define		MAX_RED_LED_NUM				3
#define		MAX_RED_LED_DELAY			1

#define		LASING_2_MSG_FG				"FF3300"							// Status message foreground on cw mode page
#define		LASING_2_MSG_BG				"000000"					   		// Status message background on cw mode page
#define		LASING_2_MSG_FONT			7									// Font size for status message in status bar
#define		LASING_2_MSG_X				235									// Status message on main page x
#define		LASING_2_MSG_Y				300									// Status message on main page y

#define		READY_2_MSG_FG				"00FF00"							// Status message foreground on cw mode page
#define		READY_2_MSG_BG				"000000"					   		// Status message background on cw mode page
#define		READY_2_MSG_FONT			7									// Font size for status message in status bar
#define		READY_2_MSG_X				214									// Status message on main page x
#define		READY_2_MSG_Y				210									// Status message on main page y

#define		MAIN_TEMP_FG				"FFFFFF"							// Status message foreground on cw mode page
#define		MAIN_TEMP_BG				"000099"					   		// Status message background on cw mode page
#define		MAIN_TEMP_FONT				1									// Font size for status message in status bar
#define		MAIN_TEMP_X					22									// Status message on main page x
#define		MAIN_TEMP_Y					401									// Status message on main page y

#define		READY_TEMP_FG				"FFFFFF"							// Status message foreground on cw mode page
#define		READY_TEMP_BG				"000000"					   		// Status message background on cw mode page
#define		READY_TEMP_FONT				1									// Font size for status message in status bar
#define		READY_TEMP_X				22									// Status message on main page x
#define		READY_TEMP_Y				380									// Status message on main page y

#define		READY_POWER_FG				"FFFFFF"							// Status message foreground on cw mode page
#define		READY_POWER_BG				"006699"					   		// Status message background on cw mode page
#define		READY_POWER_FONT			1									// Font size for status message in status bar
#define		READY_POWER_X				22									// Status message on main page x
#define		READY_POWER_Y				358									// Status message on main page y

#define		READY						1									// Ready mode
#define		STANDBY						0									// Standby mode

#define		MAIN_BUTTON_OFF				5									// Main button OFF (unpressed/inactive) bitmap index
#define		MAIN_BUTTON_ON				6									// Main button ON (pressed/active) bitmap index
#define		MAIN_BUTTON_X				271									// Main button coordinate x
#define		MAIN_BUTTON_Y				26									// Main button coordinate y

#define		MAIN_R_BIG_MSG_FG			"FFFF00"							// Status message foreground on cw mode ready
#define		MAIN_L_BIG_MSG_FG			"FF0000"							// Status message foreground on cw mode lasing
#define		MAIN_BIG_MSG_BG				"000000"					   		// Status message background on cw mode page
#define		MAIN_BIG_MSG_FONT			5									// Font size for status message in status bar
#define		MAIN_BIG_MSG_X				208									// Status message on main page x
#define		MAIN_BIG_MSG_Y				335									// Status message on main page y

#define		MAIN_TMC_MSG_FG				"FF3300"							// Text message foreground for TMC
#define		MAIN_TMC_MSG_BG				"000000"					   		// Text message background for TMC
#define		MAIN_TMC_MSG_FONT			5									// Font size for TMC
#define		MAIN_TMC_MSG_X				122									// Text message on main page x
#define		MAIN_TMC_MSG_Y				335									// Text message on main page y

#define		READY						1									// Ready mode
#define		STANDBY						0									// Standby mode

#define		MAIN_BUTTON_OFF				5									// Main button OFF (unpressed/inactive) bitmap index
#define		MAIN_BUTTON_ON				6									// Main button ON (pressed/active) bitmap index
#define		MAIN_BUTTON_X				271									// Main button coordinate x
#define		MAIN_BUTTON_Y				26									// Main button coordinate y

#define		SETTINGS_BUTTON_OFF			7									// Settings button OFF (unpressed/inactive) bitmap index
#define		SETTINGS_BUTTON_ON			8									// Settings button ON (pressed/active) bitmap index
#define		SETTINGS_BUTTON_X			399									// Settings button coordinate x
#define		SETTINGS_BUTTON_Y			26									// Settings button coordinate y

#define		SERVICE_BUTTON_OFF			9									// Service button OFF (unpressed/inactive) bitmap index
#define		SERVICE_BUTTON_ON			10 									// Service button ON (pressed/active) bitmap index
#define		SERVICE_BUTTON_X			525									// Service button coordinate x
#define		SERVICE_BUTTON_Y			26									// Service button coordinate y

#define		READY_BUTTON_OFF			15									// Ready button OFF (unpressed/inactive) bitmap index
#define		READY_BUTTON_ON				16									// Ready button ON (pressed/active) bitmap index
#define		READY_BUTTON_X				387									// Ready button coordinate x
#define		READY_BUTTON_Y				400									// Ready button coordinate y
																   	
#define		STANDBY_BUTTON_OFF			17									// Standby button OFF (unpressed/inactive) bitmap index
#define		STANDBY_BUTTON_ON			18									// Standby button ON (pressed/active) bitmap index
#define		STANDBY_BUTTON_X			514									// Standby button coordinate x
#define		STANDBY_BUTTON_Y			400									// Standby button coordinate y

#define		AIMING_BEAM_FG				"00FFFF"							
#define		AIMING_BEAM_BG				"000000"							
#define		AIMING_BEAM_FONT			4									
#define		AIMING_BEAM_X				497									
#define		AIMING_BEAM_Y				342
								
#define		MAX_AIMING_INTENSITY		250
#define		MIN_AIMING_INTENSITY		0									// Aiming is barely invisible
#define		AMING_INTENSITY_OFFSET		0									// Offset from where it actually produces aiming beam
#define		AIMING_INTENSITY_STEP		10									// 1 step = 18, Start = 70; End = 250. Total = 10 steps
#define		MAX_AIMING_ITEMS			11
#define		MAX_AIMING_INDEX			10
#define		MIN_AIMING_INDEX			0

#define		MAX_POSSIBLE_TEMP			100
#define		MIN_POSSIBLE_TEMP			10

#define		MIN_ROOM_TEMP				15									// In C
#define		MAX_ROOM_TEMP				40

#define		MIN_WATER_TEMP				15
#define		MAX_WATER_TEMP				35

#define		MIN_POWER_METER				0
#define		MAX_POWER_METER				500

#define		POWER_METER_DELAY_LIMIT		(SYSTEM_CLOCK / 1000000)

#define		POWER_METER_READING			0

#define		POWER_METER_SCALER			0.010000000

//------------------------------------------------------------------------------------------------------
// Scheduler constants and structures
//------------------------------------------------------------------------------------------------------
typedef xdata struct 
{
   void (code * taskPtr)(void);  
   unsigned int delay;       
   unsigned int period;       
   unsigned char ready;       
} taskStructure;

//------------------------------------------------------------------------------------------------------
// Sound structure
//------------------------------------------------------------------------------------------------------ 
typedef xdata struct 
{ 
   unsigned char waveform;       
   unsigned char frequency;       
   unsigned int duration;       
}sound;

#define     MAX_TASKS         			24                            	 	// Maximum number of tasks in scheduling is not greater than 24
																			// Do not change this number or scheduler is not functioning properly
//------------------------------------------------------------------------------------------------------
// Interrupt service rountine vectors
//------------------------------------------------------------------------------------------------------ 
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

//------------------------------------------------------------------------------------------------------
// Descriptions of clock ticks and timer ticks
//------------------------------------------------------------------------------------------------------ 
// SYSTEM_CLOCK = 98 Mhz = 98,000,000 Hz => 1 clock tick without prescaler = 1 / 98,000,000 = 0.0102 us (micro second)
// PRESCALER = 48 => Clock source for timer = 98 Mhz / 48 = 2.04 Mhz
// 1 clock tick with prescaler = 1 / 2.04 Mhz = 0.490 us => 1 timer tick = 0.490 us
// Number of timer ticks = 1000 us / 0.490 us = 2000 ticks. Note that 1 ms = 1000 us
// Timer starts at 63535 because 65535 - 2000 = 63535. After the timer has counted 2000 ticks (1 ms) we get 1 ms per overflow
//-------------------------------------------------------------------------------------------------------
// Use timer 0 to generate 1 ms tick used for the scheduler
//-------------------------------------------------------------------------------------------------------
#define 	TIMER0_PRESCALER           	 4  								// Set CKCON for this prescaler
#define 	T0_OVERFLOW_RATE           	 1  								// Overflow rate is 1 in this case
#define 	TIMER0_TICKS_PER_MS  		SYSTEM_CLOCK/TIMER0_PRESCALER/1020	// Number of timer ticks per 1 milisecond. Use 1020
#define 	T0_TEMP1     				TIMER0_TICKS_PER_MS * T0_OVERFLOW_RATE// instead of 1000 because of some offset
#define 	T0_TEMP2     				-T0_TEMP1
#define 	T0_TEMP3     				(T0_TEMP2 % 256)					// Get low byte
#define 	T0_TEMP4     				(T0_TEMP2 / 256)					// Get high byte

#define 	TIMER0_RELOAD_HIGH      	T0_TEMP4  							// Reload value for Timer1 high byte
#define 	TIMER0_RELOAD_LOW        	T0_TEMP3  							// Reload value for Timer1 low byte

//-------------------------------------------------------------------------------------------------------
// Use timer 1 to generate 1 ms tick used for the UART1
//-------------------------------------------------------------------------------------------------------
#define TIMER1_PRESCALER           	 	48  								// Set CKCON for this prescaler
#define T1_OVERFLOW_RATE           	 	1  									// Overflow rate is 1 in this case
#define TIMER1_TICKS_PER_MS  			SYSTEM_CLOCK/TIMER1_PRESCALER/1020	// Number of timer ticks per 1 milisecond. Use 1020
#define T1_TEMP1     					TIMER1_TICKS_PER_MS * T1_OVERFLOW_RATE// instead of 1000 because of some offset
#define T1_TEMP2     					-T1_TEMP1
#define T1_TEMP3     					(T1_TEMP2 % 256)					// Get low byte
#define T1_TEMP4     					(T1_TEMP2 / 256)					// Get high byte

#define TIMER1_RELOAD_HIGH      		T1_TEMP4  							// Reload value for Timer1 high byte
#define TIMER1_RELOAD_LOW        		T1_TEMP3  							// Reload value for Timer1 low byte

// -----------------------------------------------------------------
// One wire chip bridge DS2482 commands
// -----------------------------------------------------------------
#define		ONE_WIRE_COMMAND			0x00								// Array index
#define		ONE_WIRE_PARAMETER			0x01								// Array index

#define 	DS2482_CMD_DRST				0xF0								// DS2482 Device Reset
#define 	DS2482_CMD_WCFG				0xD2								// DS2482 Write Configuration
#define 	DS2482_CMD_CHSL				0xC3								// DS2482 Channel Select
#define 	DS2482_CMD_SRP				0xE1								// DS2482 Set Read Pointer
#define 	DS2482_CMD_1WRS				0xB4								// DS2482 1-Wire Reset
#define 	DS2482_CMD_1WWB				0xA5								// DS2482 1-Wire Write Byte
#define 	DS2482_CMD_1WRB				0x96								// DS2482 1-Wire Read Byte
#define 	DS2482_CMD_1WSB				0x87								// DS2482 1-Wire Single Bit
#define 	DS2482_CMD_1WT				0x78								// DS2482 1-Wire Triplet

// -----------------------------------------------------------------
// One wire chip bridge DS2482 status register bits
// -----------------------------------------------------------------
#define 	DS2482_STATUS_1WB			0x01								// DS2482 Status 1-Wire Busy
#define 	DS2482_STATUS_PPD			0x02								// DS2482 Status Presence Pulse Detect
#define 	DS2482_STATUS_SD			0x04								// DS2482 Status Short Detected
#define 	DS2482_STATUS_LL			0x08								// DS2482 Status 1-Wire Logic Level
#define 	DS2482_STATUS_RST			0x10								// DS2482 Status Device Reset
#define 	DS2482_STATUS_SBR			0x20								// DS2482 Status Single Bit Result
#define 	DS2482_STATUS_TSB			0x40								// DS2482 Status Triplet Second Bit
#define 	DS2482_STATUS_DIR			0x80								// DS2482 Status Branch Direction Taken

// -----------------------------------------------------------------
// One wire chip bridge DS2482 configuration register bits
// -----------------------------------------------------------------
#define DS2482_CFG_APU					0x01								// DS2482 Config Active Pull-Up
#define DS2482_CFG_PPM					0x02								// DS2482 Config Presence Pulse Masking
#define DS2482_CFG_SPU					0x04								// DS2482 Config Strong Pull-Up
#define DS2482_CFG_1WS					0x08								// DS2482 Config 1-Wire Speed

// -----------------------------------------------------------------
// One wire chip bridge DS2482 channel selection codes
// -----------------------------------------------------------------
#define DS2482_CH_IO0					0xF0								// DS2482 Select Channel IO0
#define DS2482_CH_IO1					0xE1								// DS2482 Select Channel IO1
#define DS2482_CH_IO2					0xD2								// DS2482 Select Channel IO2
#define DS2482_CH_IO3					0xC3								// DS2482 Select Channel IO3
#define DS2482_CH_IO4					0xB4								// DS2482 Select Channel IO4
#define DS2482_CH_IO5					0xA5								// DS2482 Select Channel IO5
#define DS2482_CH_IO6					0x96								// DS2482 Select Channel IO6
#define DS2482_CH_IO7					0x87								// DS2482 Select Channel IO7

// -----------------------------------------------------------------
// One wire chip bridge DS2482 read pointer codes
// -----------------------------------------------------------------
#define DS2482_READPTR_SR				0xF0								// DS2482 Status Register
#define DS2482_READPTR_RDR				0xE1								// DS2482 Read Data Register
#define DS2482_READPTR_CSR				0xD2								// DS2482 Channel Selection Register
#define DS2482_READPTR_CR				0xC3								// DS2482 Configuration Register
#define DS2431_MAX_POLLING				1000								// Max number of polling to check 1-wire line 

// -----------------------------------------------------------------
// One wire chip DS2431 commands
// -----------------------------------------------------------------
#define	DS2431_CMD_WSP					0x0F								// DS2431 Write scratchpad
#define	DS2431_CMD_RSP					0xAA								// DS2431 Read scratchpad
#define	DS2431_CMD_CSP					0x55								// DS2431 Copy scratchpad
#define	DS2431_CMD_RMEM					0xF0								// DS2431 Read memory
#define	DS2431_CMD_RROM					0x33								// DS2431 Read ROM
#define	DS2431_CMD_MROM					0x55								// DS2431 Match ROM
#define	DS2431_CMD_SROM					0xCC								// DS2431 Skip ROM
#define	DS2431_CMD_RESUME				0xA5								// DS2431 Resume
#define	DS2431_CMD_ODSROM				0x3C								// DS2431 Overdrive Skip ROM
#define	DS2431_CMD_ODMROM				0x69								// DS2431 Overdrive Match ROM
	
//-------------------------------------------------------------------------------------------------------
// Configurations.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void configureSystem(void);
void systemClockInit (void);
void voltageRefInit(void);
void enableInterrupts(void);
void enableSMBInterruptOnly(void);
void portInit (void);
void setLasingInterrupts(unsigned char);

//-------------------------------------------------------------------------------------------------------
// Initializations.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void setDefaultValues(void);
void initializeParameters(void);
void startTouchScreen(void);
void checkSplashScreenEnd(void);
void setSlaveData(void);
void getSlaveInfo(void);
void getSlaveData(void);
void resetAllDisplayCounters(void);
void resetAllErrorCounters(void);
void clearAllErrorCodes(void);
void resetAllPreviousValues(void);
bit activateOneWireBridge(unsigned char);
void setDefaultSettings(void);
void setDefaultSystemInfo(void);
void setDefaultSecurity(void);
void setDefaultLastData(void);
void setDefaultLaserData(void);
void setDefaultOtherData(void);
void setDefaultTmc(void);
void setTouchScreenParameters(void);
void configureHardware(void);
void resetSystem(void);
void rebootSystem(void);

//-------------------------------------------------------------------------------------------------------
// Task.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void updateFlags(void);
void protectSavingData(void);
void generateLasingBeep(void);
void detectFiber(void);
void verifyFiberCode(void);
void checkMasterErrors(void);
void checkSlaveErrors(void);
void showErrorMessages(void);
void getUserInput(void);
void saveSystemInfoInFiber(void);
void timeOut(void);
void showEnergy(void);
void refreshScreen(void);
void displaySystemInfo(void);
void sendUpdatesToPC(void);
void displayRealTimeClock(void);
void turnOffScreen(void);
void checkFootswitch(void);
void getPeripheralData(void);
void checkSlaveStatus(void);
void flashLed(void);
void showLasingLed(void);
void verifyPeripherals(void);
void reset(void);
void controlBeeping(void);
void displayWarning(void);
void adjustPowerOutput(void);
void getPcInput(void);

//-------------------------------------------------------------------------------------------------------
// ScanInput.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void scanUserInput(void);

//-------------------------------------------------------------------------------------------------------
// TouchScreen.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void displayText(const unsigned char*, const unsigned char*, const unsigned char, const char*, const unsigned int, const unsigned int);
void showBitmap(const unsigned int, const unsigned int, const unsigned int);
void changeScreen(const unsigned char);
void callMacro(const unsigned int);
void sendCommand(const char *);

//-------------------------------------------------------------------------------------------------------
// MainSupports.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void mainInit(void);
void displayParameters(void);
void adjustPulsewidth(const unsigned char);
void adjustRepRate(const unsigned char);
void adjustEnergy(const unsigned char);
void showMaxEnergyReached(void);
void showMaxFiberRepRateReached(void);
void showMaxFiberEnergyReached(void);
void showMaxPowerReached(void);
void showMaxRepRateReached(void);
void showMinRepRateReached(void);
void showMinEnergyReached(void);
void showRepRateSettingAdjusted(void);
void showEnergySettingAdjusted(void);
void adjustAimingBeam(const unsigned char);
unsigned int getPowerIndex(void);
void displayPower(void);
void setMode(unsigned int);
void setMethod(void);
void setStandby(void);
void setReady(void);
void chargeCapBank(void);
void resetReadyData(unsigned char);
void getMaxV(unsigned int);
void displayMainMessage(unsigned int);

//-------------------------------------------------------------------------------------------------------
// SettingsSupports.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void adjustBrightness(unsigned char);
void adjustBeeperVolume(unsigned char);
void adjustScreenSaver(unsigned char);
void seekBlastShieldHome(void);
void moveBlastShield(unsigned char, unsigned int);
void restoreBlastShield(void);
void restoreSettings(void);
void setConfirm(void);
void saveSettings(unsigned char);
void retrieveSettings(void);
void saveSystemInfo(unsigned char);
void retrieveSystemInfo(void);
void saveSecurity(unsigned char);
void saveOpenCoverSecurity(void);
void retrieveSecurity(void);
void retrieveLastData(void);
void saveLastData(unsigned char);
void retrieveLaserData(void);
void saveLaserData(unsigned char);
void retrieveOtherData(void);
void saveOtherData(unsigned char);
void displaySettingsMessage(unsigned char);
void increaseProgramVoltage(void);
void decreaseProgramVoltage(void);

//-------------------------------------------------------------------------------------------------------
// ServiceSupport.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void showPasscode(void);
void verifyPasscode(void);
void clearPasscode(void);
void resetPasscodeEntry(void);
unsigned int generateRandom(unsigned int, unsigned int);

//-------------------------------------------------------------------------------------------------------
// ServiceMenuSupport.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void displayServiceMenuMessage(unsigned char);

//-------------------------------------------------------------------------------------------------------
// Scheduler.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void runTask(void);
void addTask(void (code*) (void), const unsigned int, const unsigned int);

//-------------------------------------------------------------------------------------------------------
// TickGenerator.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void tickGeneratorInit(void);
void startScheduler(void);
void timer1Reload(void);

//-------------------------------------------------------------------------------------------------------
// Serial.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void serialBuffersInit(void);
void uart0Init(void);
void uart1Init(void);

//-------------------------------------------------------------------------------------------------------
// SMB.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void smbInit(void);
void timer3Init(void);
void smbISR(void);
void timer3ISR(void);
void checkSlavePresence(void);

void writeBytesToRealTimeClock(unsigned char, unsigned char);
void readBytesFromRealTimeClock(unsigned char, unsigned char);
void writeOneByteToSram(unsigned char, unsigned char);
void writeTwoBytesToSram(unsigned char, unsigned int);
void writeBytesToSram(unsigned char, unsigned char);
void readBytesFromSram(unsigned char, unsigned char);
unsigned char readOneByteFromSram(unsigned char);
unsigned int readTwoBytesFromSram(unsigned char);

unsigned char readOneByteFromSlave(unsigned char);
unsigned int readTwoBytesFromSlave(unsigned char);
void readBytesFromSlave(unsigned char, unsigned char);

void writeBytesToSlave(unsigned char, unsigned char);
void writeOneByteToSlave(unsigned char, unsigned char);
void writeTwoBytesToSlave(unsigned char, unsigned int);

void writeBytesToWaveformGen(unsigned char, unsigned char);
void readBytesFromWaveformGen(unsigned char, unsigned char);

void writeToDS2482(unsigned char, unsigned char);
unsigned char readFromDS2482(void);

void writeOneByteToEeprom(unsigned int, unsigned char);
void writeTwoBytesToEeprom(unsigned int, unsigned int);
void writeBytesToEeprom(unsigned int, unsigned int);

void readBytesFromEeprom(unsigned int, unsigned int);
unsigned char readOneByteFromEeprom(unsigned int);
unsigned int readTwoBytesFromEeprom(unsigned int);

void writeOneByteToDigitalPot(unsigned char);

void smbRead(char, unsigned int, unsigned int);
void smbWrite(char, unsigned int, unsigned int);

//-------------------------------------------------------------------------------------------------------
// OneWire.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
bit reset1WireBridge(void);
bit writeConfig1WireBridge(unsigned char);
void write1WireBridge(unsigned char, unsigned char);
unsigned char read1WireBridge(void);
bit reset1WireChip(void);
bit check1WireLineStatus(void);
void writeFiberScratchpad(unsigned int, unsigned char*);
void readFiberScratchpad(void);
bit copyFiberScratchpadToROM(void);
void readFiberROM(unsigned int, unsigned char);
void readFiberSN(void);

//-------------------------------------------------------------------------------------------------------
// Other.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void delayGeneratorInit(void);
void getFiberData(void);
char * convertByteToBits(unsigned char num);
void delayMicroSec(unsigned int);
void delay(unsigned int); 
unsigned long getData(unsigned char);
void putData(unsigned char, unsigned long);
void resetValuesOnChange(void);
void resetWatchdog(void);
void disableWatchdog(void);
void enableWatchdog(void);
void showTimer(void);
void setStandbyByErrors(void);
void activateRollingPasscode(void);
void resetInLasingErrors(void);
void startIGBTPulse(void);
void stopIGBTPulse(void);
void generateBeeps(unsigned char, unsigned char, unsigned char);
void sendBeeps(void);
void readInternalEnergy(void);
void controlLasingBeep(unsigned char);
void resetSMB(void);

//-------------------------------------------------------------------------------------------------------
// USB.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void sendStringToPc(const char *);
void sendStringToPc2(unsigned char, unsigned char, unsigned int, int, char[]);
void sendDataToPc(const char *, int);
void scanPcInput(void);
void tokenizeData(char *);
void tokenizeCalData(char *);

//-------------------------------------------------------------------------------------------------------
// Crc.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
unsigned int getCRC16(unsigned char [], unsigned int);

//-------------------------------------------------------------------------------------------------------
// RealTimeClock.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void setRealTimeClockControl(void);
void setRealTimeClock(void);
void getRealTimeClock(void);
void adjustDateTime(const unsigned char);
void moveDateTimeUp(void);
void moveDateTimeDown(void);
void enterDateTime(void);
void setRealTimeClockOnScreen(void);
void resetRealTimeClock(void);
unsigned char convertDecimalToBCD(unsigned char);
unsigned char convertBCDToDecimal(unsigned char);

//-------------------------------------------------------------------------------------------------------
// DACs.c
//-------------------------------------------------------------------------------------------------------
void dac0Init(void);
void dac1Init(void);
void updateDac0(unsigned int);
void updateDac1(unsigned int);

//-------------------------------------------------------------------------------------------------------
// ADCs.c
////-------------------------------------------------------------------------------------------------------
void adc0Init(void);
void adc0ISR(void);
void startADC0(void);

void adc2Init(void);
void adc2ISR(void);
void startADC2(void);

void readSensor(unsigned char, unsigned char);

//-------------------------------------------------------------------------------------------------------
// PCAs.c
////-------------------------------------------------------------------------------------------------------
void PCA0Init(void);

//-------------------------------------------------------------------------------------------------------
// Comparators.c
////-------------------------------------------------------------------------------------------------------
void comparator0Init(void);
void comparator1Init(void);
void comparator0ISR(void);
void comparator1ISR(void);

//-------------------------------------------------------------------------------------------------------
// CalSupports.c Function Prototypes
//-------------------------------------------------------------------------------------------------------
void powerCalInit(void);
void displayPowerCalParameters(void);
void adjustInternalPowerMeterOffset(const unsigned char);
void adjustProgramVoltage(const unsigned char);
void setCalPower(void);
void setAllCalPowers(void);
void getAllCalPowers(void);
void getPowerMeterTable(void);
void saveCalPower(void);
void saveAllCalPowers(void);
void setInternalPowerMeterOffset(void);
void saveAllInternalPowerMeterOffsets(void);
void getAllInternalPowerMeterOffsets(void);
void showPowerCalText(void);
void lockUnlockInternalMeterOffset(void); 
void saveMaxMinAiming(unsigned char);
int getCheckSum(unsigned char [], unsigned int);

//-------------------------------------------------------------------------------------------------------
// External Declarations
//-------------------------------------------------------------------------------------------------------
extern unsigned char screen;
extern unsigned char lastScreen;

extern bit screenChanged;
extern bit screenReset;
extern bit splashEnd;
extern bit ackFromScreen;
extern bit startOfRunningTasks;
extern bit watchdogDisabled;
extern bit eepromChanged;

extern bit lasingBeep;
extern bit beeperActivated;

extern unsigned char sharedData[]; 

extern unsigned char power;
extern float maxPower;
extern unsigned int pulsewidth;
extern unsigned int repRate;

extern unsigned char TA1;
extern unsigned char TA2;
extern unsigned char ES;
extern unsigned char CRC16WL;
extern unsigned char CRC16WH;
extern unsigned char CRC16RL;
extern unsigned char CRC16RH;
extern unsigned int CRC16R;
extern unsigned int CRC16W;

extern unsigned char scratchpadDataIn[];
extern unsigned char scratchpadDataOut[];
extern unsigned char fiberDataIn[];
extern unsigned char fiberDataOut[];
extern unsigned char serialNumber[];
extern unsigned char lastSerialNumber[];
extern unsigned char errorCodes[];

extern bit scratchpadReadError;
extern bit scratchpadWriteError;
extern bit oneWireMemoryReadError;
extern bit oneWireMemoryWriteError;

extern bit tsCommandReceived;
extern bit tsCommandTransmitted;
extern unsigned char tsRxBuffer[];
extern unsigned char tsTxBuffer[];
extern unsigned char userCommand[];
extern unsigned int tsRxIn;
extern unsigned int tsRxOut;
extern unsigned int tsTxIn;
extern unsigned int tsTxOut; 
extern bit tsRxEmpty;
extern bit tsTxEmpty;
extern bit tsLastCharGone;

extern bit pcDataReceived;
extern bit pcDataTransmitted;
extern unsigned char pcRxBuffer[];
extern unsigned char pcTxBuffer[];
extern unsigned char pcDataIn[];
extern unsigned char pcDataOut[];
extern unsigned int pcRxIn;
extern unsigned int pcRxOut;
extern unsigned int pcTxIn;
extern unsigned int pcTxOut;
extern bit pcRxEmpty;
extern bit pcTxEmpty;
extern bit pcLastCharGone;

extern bit flagUpdatesForPC;
extern bit parameterUpdatesForPC;

extern bit fiberInfoUpdate;
extern bit fiberDataSaved;

extern unsigned char fiberSystem;
extern unsigned char fiberRegion;
extern unsigned char fiberOhType;
extern unsigned char fiberTypeSize;
extern unsigned char fiberType;
extern unsigned int fiberSize;
extern unsigned int fiberSizeAux;
extern unsigned long fiberLotNumber;
extern unsigned long lotNumberAux;
extern unsigned char fiberMaxUses;
extern unsigned char fiberPreviousUses;
extern unsigned char fiberCurrentUses;
extern unsigned int fiberSecretCode;
extern unsigned int newFiberSecretCode;
extern unsigned int decryptedCode;
extern unsigned int newDecryptedCode;
extern unsigned long laserTotalEnergy;
extern unsigned long previousLasingEnergy;
extern unsigned long totalEnergy;
extern unsigned long savedTotalEnergy;
extern unsigned long maxEnergyPerUse;
extern unsigned long maxEnergyPerUseAux;
extern unsigned long last1Energy;
extern unsigned long last2Energy;
extern unsigned char last1Power;
extern unsigned char last2Power;

extern unsigned int productSerialNumber;
extern unsigned int modelName;
extern unsigned int laserDiodeVersion;
extern unsigned int hardwareVersion;
extern unsigned int passcodeSeed;
extern unsigned int mfMonth;
extern unsigned int mfDay;
extern unsigned int mfYear;
extern unsigned int storedYear;
extern unsigned int serviceAccessLocked;
extern unsigned int incorrectPasscodeEntered;

extern unsigned int tmcOffCounter;
extern unsigned char fiberSecretRequest;
extern unsigned char passcodeEnteredNum;

extern float masterSoftwareVer;
extern float slaveSoftwareVer;

extern unsigned char flashStatus;
extern unsigned char flashContent;

extern unsigned int elapsedHours;
extern unsigned int elapsedMinutes;
extern unsigned int elapsedSeconds;

extern unsigned int usableFiberRecord[];											// Keep usable fibers in record

extern bit screenCurrentlySleeps;
extern unsigned int screenSaverCounter;
extern unsigned char screenBeforeSleep;

extern unsigned char brightness;
extern unsigned char aimingIntensity;
extern unsigned char aimingIntensityPumpRate;
extern unsigned char sleep;
extern unsigned char beeperVolume;
extern unsigned char fanDutyCycle;
extern unsigned char lastBlastShieldPosition;
extern unsigned char sramUsedIndicator;
extern unsigned char eepromUsedIndicator;
extern unsigned char initializationIndicator;
extern unsigned char currentBlastShieldPosition;
extern unsigned char blastShieldHomeRef;
extern unsigned char newBlastShieldPosition;
extern unsigned char blastShieldHomeDetected;
extern unsigned char stepDone;

extern unsigned char seconds;
extern unsigned char minutes;
extern unsigned char hours;
extern unsigned char hours24;
extern unsigned char amPm;
extern unsigned char day;
extern unsigned char date;
extern unsigned char month;
extern unsigned char year;
extern unsigned char century;
extern unsigned char timeMode;

extern unsigned char currentDate;
extern unsigned char currentMonth;
extern unsigned char currentYear;

extern unsigned char realTimeClockItems;

extern const char code * dayOfWeek[];
extern const char code * monthOfYear[];
extern unsigned char sramTx[];
extern unsigned char sramRx[];
extern unsigned char eepromTx[];
extern unsigned char eepromRx[];

extern unsigned char fullSerialNumber[];

extern unsigned char adjustedSeconds;
extern unsigned char adjustedMinutes;
extern unsigned char adjustedHours;
extern unsigned char adjustedAmPm;
extern unsigned char adjustedDay;
extern unsigned char adjustedDate;
extern unsigned char adjustedMonth;
extern unsigned char adjustedYear;
extern unsigned char adjustedCentury;
extern unsigned char adjustedTimeMode;

extern bit monthUpdated;
extern bit dateUpdated;
extern bit yearUpdated;
extern bit hoursUpdated;
extern bit minutesUpdated;
extern bit secondsUpdated;
extern bit amPmUpdated;

extern unsigned char previousMonth;
extern unsigned char previousDate;
extern unsigned char previousYear;
extern unsigned char previousHours;
extern unsigned char previousMinutes;
extern unsigned char previousSeconds;

extern bit monthDateYearUpdated;
extern bit timeUpdated;

extern unsigned int repRateIndex;
extern float energy;
extern unsigned int energyIndex;
extern unsigned char programVoltage;
extern unsigned int currentPowerIndex;
extern unsigned char programVoltageReset;
extern unsigned char eepromProgramVoltage;
extern signed char internalPowerMeterOffset;
extern float floatInternalPowerMeterOffset;
extern float eepromFloatInternalPowerMeterOffset;
extern signed char eepromInternalPowerMeterOffset;
extern unsigned char timeForPulse;
extern unsigned int pcaCounter;
extern unsigned char pcaValueLoaded;
extern bit IGBTFiring;
extern unsigned int calSetPower;
extern unsigned char screenUpdate;
extern unsigned int intensity;
extern unsigned char numPress;
extern unsigned char passcodeStatus;
extern unsigned char tpcPasscodeStatus;

extern const char code * mainMsg[];
extern const char code * settingsMsg[];
extern const char code * serviceMsg[];
extern const char code * superpulseMsg[];
extern const char code * modelId[];

extern bit masterWatchdogReset;
extern bit startOfRunningTasks;
extern bit topCoverOpened;
extern bit SMB_BUSY;

extern bit systemErrorDetectionOption;
extern bit fiberDetectionOption;
extern bit powerSupplyDetectionOption;
extern bit footswitchDetectionOption;
extern bit blastShieldDetectionOption;
extern bit oneWireBridgeDetectionOption;
extern bit smbDetectionOption;
extern bit ambientDetectionOption;
extern bit presetEnergyZeroDetectionOption;
extern bit remoteInterlockDetectionOption;
extern bit coverInterlockDetectionOption;
extern bit powerErrorDetectionOption;
extern bit totalEnergyLimitDetectionOption;
extern bit overPowerDetectionOption;
extern bit underPowerDetectionOption;
extern bit slaveErrorDetectionOption;

extern bit useCountWriteOnce;
extern bit useCountUpdated;
extern bit footswitchInStandbyErr;
extern bit DS2482Activated;
extern bit alreadyCheckedUseCount;
extern bit serialNumberChanged;
extern bit systemStatus;

extern unsigned int fiberOutBlocks;															// Number of blocks (8 bytes each) to write to memory
extern float fiberOutBytes;																	// Number of bytes to write to scratchpad

extern bit footswitchPressedOnAmbient;
extern bit lasingBeep;
extern bit ambientDetectedBeep;
extern bit flashChanged;

extern unsigned char buttonDisplayRepeat;
extern unsigned char mainPageDisplayRepeat;
extern unsigned char readyDataDisplayRepeat;
extern unsigned char mainMessageDisplayRepeat;
extern unsigned char fiberIconsDisplayRepeat;
extern unsigned char settingsMessageDisplayRepeat;
extern unsigned char settingsDataDisplayRepeat;
extern unsigned char serviceMessageDisplayRepeat;
extern unsigned char serviceMenuMessageDisplayRepeat;
extern unsigned char powerDisplayRepeat;
extern unsigned char clockReadRepeat;
extern unsigned char clockSetupDisplayRepeat;
extern unsigned char energyRepeat;
extern unsigned char allBarsRepeat;
extern unsigned char blankBarRepeat;
extern unsigned char blankBarInReadyRepeat;
extern unsigned char randomBarRepeat;
extern unsigned char serviceMessageRepeat;
extern unsigned char choppedpulseMessageRepeat;
extern unsigned char choppedpulsewidthRepeat;
extern unsigned char superpulseMessageRepeat;
extern unsigned char superpulseRepRateRepeat;
extern unsigned char powerCalMessageRepeat;
extern unsigned char systemInfoDisplayRepeat;
extern unsigned char helpMessageDisplayRepeat;
extern unsigned char presetEnergyRepeat;
extern unsigned char totalEnergyRepeat;
extern unsigned char energiesReadRepeat;
extern unsigned char monthDateYearDisplayRepeat;
extern unsigned char calMessageDisplayRepeat;
extern unsigned char calPowerDisplayRepeat;
extern unsigned char calSuperPowerDisplayRepeat;
extern unsigned char calSuperPowerRepRateRepeat;
extern unsigned char mainParametersDisplayRepeat;
extern unsigned char powerCalParametersDisplayRepeat;
extern unsigned char powerCalParametersDisplayRepeatUpdate;
extern unsigned char powerCalTableDisplayRepeat;
extern unsigned char powerMeterTableDisplayRepeat;
extern unsigned char powerMeterOffsetTableDisplayRepeat;
extern unsigned char fiberDetectedRepeat;
extern unsigned char fiberNotDetectedRepeat;

extern unsigned char bdata masterFlags1;													// Declare a set of 16 flags on master
extern bit standbyMode;																		// Starting byte address = 0x2C
extern bit readyMode;																		// bit address = 0x60
extern bit inLasing;
extern bit footswitchState;
extern bit systemStatus;
extern bit IGBTStarted;
extern bit reserved0;
extern bit reserved4;

extern unsigned char bdata masterFlags0;
extern bit fiberErr;
extern bit footswitchErr;
extern bit overPowerErr;
extern bit underPowerErr;
extern bit reserved8;
extern bit reserved9;
extern bit slaveRestartedAck;
extern bit reserved11;

extern unsigned char bdata slaveFlags1;														
extern bit waterPumpErr;										
extern bit airFlowSensorErr;										
extern bit pressureSensorErr;
extern bit roomTempSensorErr;
extern bit waterInTempSensorErr;
extern bit waterOutTempSensorErr;
extern bit slaveRestarted;
extern bit slaveWatchdogReset;

extern unsigned char bdata slaveFlags0;	
extern bit lowWaterPressure;
extern bit hiRoomTemp;									
extern bit hiWaterInTemp;										
extern bit hiWaterOutTemp;
extern bit hiLaserTemp;
extern bit lowAirFlow;
extern bit reserved6;
extern bit reserved7;

extern bit noBlastShieldErr;
extern bit remoteInterlockErr;
extern bit totalEnergyLimitErr;
extern bit oneWireBridgeErr;
extern bit fiberMatchedErr;
extern bit fiberUsableErr;
extern bit fiberSystemErr;

extern unsigned char eepromNoResponse;
extern unsigned char digitalPotNoResponse;
extern unsigned char realTimeClockNoResponse;
extern unsigned char oneWireBridgeNoResponse;
extern unsigned char slaveNoResponse;
extern unsigned char smbBusyTooLong;		

extern unsigned char slaveWriteDone;
extern unsigned char slaveReadDone;
extern unsigned char sramWriteDone;
extern unsigned char sramReadDone;
extern unsigned char eempromWriteDone;
extern unsigned char eepromReadDone;
extern unsigned char digitalPotWriteDone;
extern unsigned char digitalPotReadDone;

extern unsigned long oneWireWriteCount;
extern unsigned char fiberCurrentUses;
extern unsigned int fiberInsertionCount;
extern unsigned char currentSavedPower;
extern unsigned long currentEnergy;
extern unsigned char lastPower;

extern unsigned char realTimeClockNoResponse;
extern unsigned char digitalPotNoResponse;
extern unsigned char eepromNoResponse;

extern unsigned char maxLaserTemp;
extern unsigned char voltageSetpoint;
extern unsigned char currentSetpoint;
extern unsigned char roomTemp;
extern unsigned char waterTemp;
extern unsigned char exchangerTemp;

extern unsigned char rollingPasscodeUsed;
extern unsigned char passcodeValidated;
extern unsigned char tableCrcStatus;
extern unsigned char fiberErrorOff;
extern unsigned char allErrorsOff;

extern unsigned char testModeActivationCounter;
extern unsigned char testReady;
extern unsigned char footwitchStillPressed;

extern unsigned char powerCalTable[];
extern unsigned int adc0Output;
extern unsigned int adc2Output;

extern const char code * mainMsg[];
extern const char code * settingsMsg[];
extern const char code * serviceMsg[];
extern const char code * calMsg[];
extern const char code * clockSetupMsg[];
extern const char code * methodMsg[];
extern const char code * fiberRegionList[];
extern const char code * fiberTypeList[];
extern const char code * fiberSizeList[];
extern const char code * modelId[];

extern bit initializationErr;
extern bit footswitchState;

extern bit yesButtonPressed;
extern bit splashEnd;
extern bit ackFromScreen;
extern bit readDone;
extern bit writeDone;

extern unsigned char realTimeClockNoResponse;
extern unsigned char rtcWriteDone;
extern unsigned char rtcReadDone;
extern unsigned char sramWriteDone;
extern unsigned char sramReadDone;
extern unsigned char eepromWriteDone;
extern unsigned char eepromReadDone;

extern bit scratchpadReadError;
extern bit scratchpadWriteError;
extern bit oneWireMemoryReadError;
extern bit oneWireMemoryWriteError;
extern bit tsCommandReceived;
extern bit tsCommandTransmitted;
extern bit adjustmentWindowClosed;

extern unsigned char power;
extern unsigned char currentSavedPower;
extern unsigned char lastPower;
extern float floatPower;
extern float vPower;
extern float vEnergy;
extern unsigned int vRepRate;

extern unsigned long servicePasscode;
extern unsigned int calPowerAdjustment;											// Transmitted to slave board
extern unsigned char calRepRate;
extern unsigned char calPowerFineAdjustment;
extern unsigned char calPowerCourseAdjustment;									
extern unsigned int calPowerSetPoint;											// Get it from flash of slave board																									
extern unsigned int calPowerMeter;												// Get it from power meter of slave board
extern unsigned int calPowerDiff;
extern unsigned char passcodeStatus;
extern unsigned char tpcPasscodeStatus;
extern unsigned char lastStoredPower;

extern unsigned char mainMessageDisplayRepeat;
extern unsigned char fiberIconsDisplayRepeat;
extern unsigned char settingsMessageDisplayRepeat;
extern unsigned char serviceMessageDisplayRepeat;
extern unsigned char serviceMenuMessageDisplayRepeat;
extern unsigned char powerDisplayRepeat;
extern unsigned int lastSuperpulsePower;
extern unsigned char powerCalButtonsDisplayRepeat;

extern bit powerOffsetPressed;
extern unsigned int pressUpCounter;
extern unsigned int pressDownCounter;

extern unsigned int numSavedPower;
extern unsigned int presetEnergy;
extern unsigned char TA1;
extern unsigned char TA2;
extern unsigned char ES;
extern unsigned char CRC16WL;
extern unsigned char CRC16WH;
extern unsigned char CRC16RL;
extern unsigned char CRC16RH;
extern unsigned int CRC16R;
extern unsigned int CRC16W;

extern unsigned char sharedDataRx[];
extern unsigned char sharedDataTx[];
extern unsigned char eepromRx[];
extern unsigned char eepromTx[];
extern unsigned char scratchpadDataIn[];
extern unsigned char scratchpadDataOut[];
extern unsigned char fiberDataIn[];
extern unsigned char fiberDataOut[];
extern unsigned char serialNumber[];
extern unsigned char lastSerialNumber[];

extern unsigned char tsRxBuffer[];
extern unsigned char tsTxBuffer[];
extern unsigned char userCommand[];

extern float pulsewidthTable[];

extern bit pcDataReceived;
extern bit pcDataTransmitted;
//extern unsigned char pcRxBuffer[];
//extern unsigned char pcTxBuffer[];
extern unsigned char pcDataIn[];
extern unsigned char pcDataOut[];
extern unsigned int pcRxIn;
extern unsigned int pcRxOut;
extern unsigned int pcTxIn;
extern unsigned int pcTxOut;

extern int pcDataInStart;
extern int pcDataInBytes;
extern unsigned char pcDataInRW;
extern unsigned char pcDataInMemType;
extern int pcDataInAddress;

extern bit pcRxEmpty;
extern bit pcTxEmpty;
extern bit pcLastCharGone;
extern bit pcDataAlreadySentOut;
extern bit flagUpdatesForPC;
extern bit parameterUpdatesForPC;

extern unsigned char seconds;
extern unsigned char minutes;
extern unsigned char hours;
extern unsigned char amPm;
extern unsigned char day;
extern unsigned char date;
extern unsigned char month;
extern unsigned char year;
extern unsigned char century;
extern unsigned char timeMode;

extern unsigned char currentDate;
extern unsigned char currentMonth;
extern unsigned char currentYear;

extern unsigned char realTimeClockItems;

extern unsigned char tableCrcStatus;

extern unsigned int tmcOffCounter;

extern const char code * dayOfWeek[];
extern const char code * monthOfYear[];

extern unsigned char adjustedSeconds;
extern unsigned char adjustedMinutes;
extern unsigned char adjustedHours;
extern unsigned char adjustedAmPm;
extern unsigned char adjustedDay;
extern unsigned char adjustedDate;
extern unsigned char adjustedMonth;
extern unsigned char adjustedYear;
extern unsigned char adjustedCentury;
extern unsigned char adjustedTimeMode;

extern bit monthUpdated;
extern bit dateUpdated;
extern bit yearUpdated;
extern bit hoursUpdated;
extern bit minutesUpdated;
extern bit secondsUpdated;
extern bit amPmUpdated;

extern bit monthDateYearUpdated;
extern bit timeUpdated;

extern unsigned int elapsedHours;
extern unsigned int elapsedMinutes;
extern unsigned int elapsedSeconds;

extern bit systemStatus;
extern bit systemErrorDetectionOption;
extern bit fiberDetectionOption;
extern bit footswitchDetectionOption;
extern bit smbDetectionOption;
extern bit remoteInterlockDetectionOption;
extern bit coverInterlockDetectionOption;

extern unsigned int numSavedPower;


extern unsigned char continuousPress;
extern unsigned char pressCounter;

extern unsigned char userCommand[];
extern unsigned char tsRxStart;
extern unsigned char tsTxStart;
extern unsigned char tsRxEnd;
extern unsigned char tsTxEnd; 
extern unsigned char tsTxReady;

extern unsigned char usbDataReceived;
extern unsigned char usbDataTransmitted;
extern unsigned char usbRxBuffer[];
extern unsigned char usbTxBuffer[];
extern unsigned char usbRxStart;
extern unsigned char usbTxStart;
extern unsigned char usbRxEnd;
extern unsigned char usbTxEnd; 
extern unsigned char usbTxReady;

extern unsigned char seconds;
extern unsigned char minutes;
extern unsigned char hours;
extern unsigned char amPm;
extern unsigned char day;
extern unsigned char date;
extern unsigned char month;
extern unsigned char year;
extern unsigned char century;
extern unsigned char timeMode;

extern unsigned char testMode;
extern unsigned int waitForTestCounter;
extern unsigned char lasingSetupDone;

extern bit screenCurrentlySleeps;
extern unsigned int screenSaverCounter;
extern unsigned char screenBeforeSleep;
extern unsigned char rollingPasscodeUsed;

extern const char code * mainMsg[];
extern const char code * settingsMsg[];
extern const char code * serviceMsg[];
extern const char code * calMsg[];
										
extern unsigned char interlockFlag;
extern unsigned char totalEnergyReset;

extern unsigned char testModeActivationCounter;

extern bit systemStatus;
extern bit lasingBeep;
extern unsigned long totalEnergy;
extern unsigned char superpulseBeepCount;
extern unsigned char pulseBeepCount;
extern unsigned int elapsedHours;
extern unsigned int elapsedMinutes;
extern unsigned int elapsedSeconds;
extern unsigned char aimingIntensity;

extern bit footswitchInStandbyErr;
extern bit unmatchedPowersErr;
extern bit totalEnergyLimitErr;

extern const char code * settingsMsg[];
extern const char code * serviceMsg[];

extern unsigned char testMode;
extern unsigned int waitForTestCounter;
extern unsigned char lasingSetupDone;
extern unsigned char testReady;
																			
extern unsigned char interlockFlag;

extern unsigned char powerCalTable[];
extern unsigned int powerCalTableIndex;
extern unsigned int globalPowerIndex;

extern unsigned long lastStoredTotalEnergy;

extern unsigned char touchScreenSWVer[];
extern unsigned char errorMessagesQueu[];

extern const unsigned int code pulsewidthArray[];
extern const unsigned int code repRateArray[];
extern const float code energyArray[]; 	

extern unsigned char systemInfoUpdated;

extern unsigned long fiberEnergy;
extern unsigned long fiberSavedEnergy;
extern unsigned long fiberEnergyCount;
extern unsigned long energyCount;

extern bit clockAdjustmentConfirmed;
extern taskStructure task[];
extern unsigned int numTasks;

extern bit startOfRunningTasks;
extern bit watchdogDisabled;
extern bit calAdjustmentRequested;
extern bit powerCalOffsetStatus;

extern unsigned char oneWireDataByte;
extern unsigned char rtcNoResponse;

extern unsigned char slaveWriteDone;
extern unsigned char slaveReadDone;
extern unsigned char oneWireWriteDone;
extern unsigned char oneWireReadDone;
extern unsigned char rtcWriteDone;
extern unsigned char rtcReadDone;
extern unsigned char eepromWriteDone;
extern unsigned char eepromReadDone;
extern unsigned char digitalPotWriteDone;
extern unsigned char digitalPotReadDone;

extern unsigned char * sharedDataRxPtr;
extern unsigned char * sharedDataTxPtr;

extern unsigned char adc0SelectInput;
extern unsigned char adc2SelectInput;
extern float adc0FloatOutput;
extern float adc2FloatOutput;

extern unsigned int simmerVoltageInput;
extern unsigned int programVoltageInput;
extern unsigned char simmerVoltageReadingDone;
extern unsigned char programVoltageReadingDone;

extern unsigned char errorTrackingEnable;
extern unsigned char errorTrackingIndex;

extern unsigned char hiLaserTempCounter;
extern unsigned char lowAirFlowCounter;
extern unsigned char lowWaterPressureCounter;
extern unsigned char hiRoomTempCounter;
extern unsigned char hiWaterInTempCounter;
extern unsigned char hiWaterOutTempCounter;

extern unsigned char airFlowSensorErrCounter;
extern unsigned char pressureSensorErrCounter;
extern unsigned char roomTempSensorErrCounter;
extern unsigned char waterInTempSensorErrCounter;
extern unsigned char waterOutTempSensorErrCounter;
extern unsigned char footswitchErrCounter;
extern unsigned char noBlastShieldErrCounter;
extern unsigned char footswitchInStandbyErrCounter;
extern unsigned char remoteInterlockErrCounter;
extern unsigned char overPowerErrCounter;
extern unsigned char underPowerErrCounter;
extern unsigned char DS2482ActivatedCounter;
extern unsigned char fiberDetectedCounter;
extern unsigned char fiberMatchedCounter;
extern unsigned char fiberUsableCounter;
extern unsigned char fiberSystemCounter;
extern unsigned char oneWireBridgeNoResponseCounter;
extern unsigned char smbBusyTooLongCounter;
extern unsigned char slaveNoResponseCounter;
extern unsigned char ambientDetectedCounter;
extern unsigned char footswitchPressedOnAmbientCounter;
extern unsigned char unmatchedPowersErrCounter;
extern unsigned char totalEnergyLimitDetectedCounter;
extern unsigned char resetCounter;

extern unsigned int readyModeIdleCounter;
extern unsigned int fiberRemovedCounter;
extern unsigned int fiberConnectedCounter;
extern unsigned int buttonPressedHoldCounter;
extern unsigned int overPowerErrHoldCounter;

extern unsigned char rollingPasscodeUsed;
extern unsigned char passcodeValidated;
extern unsigned char tableCrcStatus;

extern unsigned long servicePasscode;
extern unsigned int passcodePosition;
extern unsigned char turnOffTPCRequested;
extern unsigned int userPasscode;
extern unsigned char passcodeBox[];
extern unsigned int passcodeBoxIndex;
extern unsigned char passcodeStatus;

extern unsigned char pca0PulsewidthLo;
extern unsigned char pca0PulsewidthHi;
extern unsigned char pca0RepRateLo;
extern unsigned char pca0RepRateHi;

extern unsigned char errorMessageIndex;
extern unsigned char numErrors;
extern unsigned char previousNumErrors;
extern unsigned int previousMessage;
extern unsigned int previousErrorMessage;
extern unsigned int previousWarningMessage;
extern unsigned char underPowerMessageHold;

extern unsigned int airFlowVoltage;
extern unsigned int pressureVoltage;
extern unsigned char roomTemp;
extern unsigned char waterInTemp;
extern unsigned char waterOutTemp;

extern unsigned int internalEnergy;
extern float actualEnergy;

extern const unsigned int code fiberLibrary[];
extern unsigned int decryptedCode;
extern unsigned char fiberCodeIndex;

extern unsigned char allMasterErrorsDefeatedCounter;
extern unsigned char allSlaveErrorsDefeatedCounter;
extern unsigned char allErrorsDefeatedCounter;
extern unsigned char capBankCharged;

extern unsigned char buttonPressed;
extern unsigned char blastShieldMoving;
extern unsigned char restartErrorMessage;

extern unsigned int powerReadingDelayCounter;
extern unsigned char backStandbyErr;
extern unsigned char settingsRestored;

extern unsigned char blastShieldMovedByUser;

extern unsigned char minEndPoint;
extern unsigned char maxEndPoint;
extern unsigned char aimingStep;
extern unsigned char aimingIndex;

#ifdef	VERIFY_POWER_METER_CAL	
extern unsigned int internalPowerMeterTable[];
#endif

extern signed char internalPowerMeterOffsetTable[];

#endif
//------------------------------------------------------------------------------------------------------
// End of File
//-------------------------------------------------------------------------------------------------------