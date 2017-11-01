#include <main.h>

#define  	MY_ADDR            			0x02                      	     	// Master address
#define  	SMB_FREQUENCY      			100000L                   	     	// Keep this frequency = 100 Khz                                  
#define  	WRITE              			0x00                       	   		// Write operation
#define  	READ               			0x01                        	    // Read operation

#define  	MCU_SLAVE_ADDR     			0x20                          	 	// SMB address of slave board
#define		EEPROM_ADDR					0xA4								// SMB address of EEPROM
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

#define		START_INDEX					0
#define		SHARED_DATA_MAX				0x7F								// Max number of devices connected to slave (127)

// EEPROM Parameters
#define		EEPROM_ADDR_SIZE			2									// 2 byte address of EEPROM
#define		EEPROM_TX_BUFFER			512
#define		EEPROM_RX_BUFFER			512

//-------------------------------------------------------------------------------------------------------
// It is the decription of data string transmitted or received between the master board and slave board
// via the SMB.
// 1. Slave address: 1 byte
// 2. Component address (on slave): 2 bytes. It may be located in the internal RAM or external RAM
// 3. Data

// For writing data to the slave board: [Slave address][Component address][Data]
// For reading data from the slave board: [Slave address][Component address]
// Note that a high byte of data will be stored in lower address and a low byte of data will be stored
// in higher address. For example, data = 0x1234 will be stored as follows:
// 0000 -----> 12
// 0001 -----> 34 
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// Global Declarations
//-------------------------------------------------------------------------------------------------------
bit SMB_BUSY = 0;                                               					// Set to claim the bus, clear to free
bit SMB_RW;                                                 						// Software flag to indicate Read or Write

unsigned char eepromNoResponse;

unsigned char sharedDataRx[SHARED_DATA_MAX];
unsigned char sharedDataTx[SHARED_DATA_MAX];

unsigned char * sharedDataRxPtr;
unsigned char * sharedDataTxPtr;

unsigned int startAddr;
unsigned char slaveAddr;                                       						// Target SMBus slave address

unsigned int numBytesRD;
unsigned int numBytesWR;

unsigned char slaveWriteDone;
unsigned char slaveReadDone;

//-------------------------------------------------------------------------------------------------------
// Function Name: scanPcInput
// Return Value: None 
// Parmeters: None
// Function Description: This function initializes the SMB bus 
//-------------------------------------------------------------------------------------------------------
void smbInit(void)
{
   	int i;
	unsigned long pollingCounter = 0;
	char SFRPAGE_SAVE = SFRPAGE;

   	SFRPAGE = SMB0_PAGE;
	while(SDA == 0 && pollingCounter++ < SMB_POLLING_LIMIT)							
   	{
      	SCL = 0;                                                					// Drive the clock low
      	for(i = 0; i < 255; i++);                               					// Hold the clock low
      	SCL = 1;                                                					// Release the clock
      	while(SCL == 0 && pollingCounter++ < SMB_POLLING_LIMIT);					// Wait for open-drain
      	for(i = 0; i < 10; i++);                                					// Hold the clock high
   	}
			
	SMB0CN = 0x07;                                          						// Assert Acknowledge low (AA bit = 1b);
                                                            						// Enable SMBus Free timeout detect;
	SMB0CR = 267 - (SYSTEM_CLOCK / (8 * SMB_FREQUENCY));							// Derived approximation from the Tlow and Thigh equations
																	
   	SMB0CN |= 0x40;                                         						// Enable SMBus;

   	SFRPAGE = SFRPAGE_SAVE;                                 						// Restore SFR page detector
	
	SMB_BUSY = 0;																	// Release SMB

	sharedDataRxPtr = sharedDataRx;													// Assign a pointer to array for random access
	sharedDataTxPtr = sharedDataTx;													// Assign a pointer to array for random access
	
	slaveWriteDone = FALSE;
	slaveReadDone = FALSE;
}

//-------------------------------------------------------------------------------------------------------
// Function Name: timer3Init
// Return Value: None 
// Parmeters: None
// Function Description: This function nitializes timer 3 which is used to time out the SMB if errors occur
//-------------------------------------------------------------------------------------------------------
void timer3Init (void)
{
   	char SFRPAGE_SAVE = SFRPAGE;        

   	SFRPAGE = TMR3_PAGE;

   	TMR3CN = 0x00;                                          						// Timer 3 in timer mode
																					// Timer 3 auto reload
   	TMR3CF = 0x00;                                          						// Timer 3 prescaler = 12

   	RCAP3 = -(SYSTEM_CLOCK / 12 / 40);                           					// Timer 3 overflows after 25 ms
   	TMR3 = RCAP3;                                           					

   	TR3 = 1;                                                						// Start Timer 3

   	SFRPAGE = SFRPAGE_SAVE;                                 						// Restore SFR page
}

//-------------------------------------------------------------------------------------------------------
// Function Name: timer3ISR
// Return Value: None 
// Parmeters: None
// Function Description: This function is timer 3 ISR which is used to reset the SMB bus if the clock line is held for too long
//-------------------------------------------------------------------------------------------------------
void timer3ISR(void) interrupt INTERRUPT_Timer_3
{
   	char SFRPAGE_SAVE = SFRPAGE;                             						// Save Current SFR page

   	SFRPAGE = SMB0_PAGE;
   	SMB0CN &= ~0x40;                                         						// Disable SMBus
   	SMB0CN |= 0x40;                                          						// Re-enable SMBus

   	SFRPAGE = SFRPAGE_SAVE;                                  						// Switch back to the Timer3 SFRPAGE
   	TF3 = 0;                                                 						// Clear Timer3 interrupt-pending flag
   	SMB_BUSY = 0;                                            						// Free bus
   
   	SFRPAGE = SFRPAGE_SAVE;                                 						// Restore SFR page detector
}

//-------------------------------------------------------------------------------------------------------
// Function Name: checkSlavePresence
// Return Value: None 
// Parmeters: None
// Function Description: This function checks the slave board's presence on SMB
//-------------------------------------------------------------------------------------------------------
void checkSlavePresence(void)
{   
	sharedDataTx[TX_START_INDEX] = 0;
	smbWrite(MCU_SLAVE_ADDR, DEVICE_DUMP_ADDR, 0); 
}

//-------------------------------------------------------------------------------------------------------
// Function Name: writeBytesToSlave
// Return Value: None 
// Parmeters: target, startAddr, content, bytes
// Function Description: This function writes one to four bytes to the slave board randomly
//-------------------------------------------------------------------------------------------------------
void writeOneByteToSlave(unsigned char startAddr, unsigned char content)
{   		
	sharedDataTx[startAddr] = content;
	smbWrite(MCU_SLAVE_ADDR, startAddr, 1);
}

//-------------------------------------------------------------------------------------------------------
// Function Name: writeBytesToSlave
// Return Value: None 
// Parmeters: target, startAddr, content, bytes
// Function Description: This function writes one to four bytes to the slave board randomly
//-------------------------------------------------------------------------------------------------------
void writeTwoBytesToSlave(unsigned char startAddr, unsigned int content)
{   		
	sharedDataTx[startAddr] = content / 256;
	sharedDataTx[startAddr + 1] = content % 256;	
	smbWrite(MCU_SLAVE_ADDR, startAddr, 2);
}

//-------------------------------------------------------------------------------------------------------
// Function Name: writeBytesToSlave
// Return Value: None 
// Parmeters: startAddr, content, bytes
// Function Description: This function writes one to four bytes to the slave board randomly
//-------------------------------------------------------------------------------------------------------
void writeBytesToSlave(unsigned char startAddr, unsigned char numBytes)
{   		
	if(numBytes == 1)
	{
		writeOneByteToSlave(startAddr, sharedDataTx[startAddr]);
	}
	else if(numBytes == 2)
	{
		writeTwoBytesToSlave(startAddr, (sharedDataTx[startAddr] * 256) + sharedDataTx[startAddr]);
	}
	else
	{
		smbWrite(MCU_SLAVE_ADDR, startAddr, numBytes);
	}
}

//-------------------------------------------------------------------------------------------------------
// Function Name: readBytesFromSlave
// Return Value: long 
// Parmeters: startAddr, bytes
// Function Description: This function reads one or two bytes from the slave board randomly
//-------------------------------------------------------------------------------------------------------
unsigned char readOneByteFromSlave(unsigned char startAddr)
{
	smbRead(MCU_SLAVE_ADDR, startAddr, 1);
	return sharedDataRx[startAddr];																								
}

//-------------------------------------------------------------------------------------------------------
// Function Name: readBytesFromSlave
// Return Value: long 
// Parmeters: startAddr, bytes
// Function Description: This function reads one or two bytes from the slave board randomly
//-------------------------------------------------------------------------------------------------------
unsigned int readTwoBytesFromSlave(unsigned char startAddr)
{
	smbRead(MCU_SLAVE_ADDR, startAddr, 2);
	return (sharedDataRx[startAddr] * 256) + sharedDataRx[startAddr + 1];																								
}

//-------------------------------------------------------------------------------------------------------
// Function Name: readBytesFromSlave
// Return Value: long 
// Parmeters: startAddr, bytes
// Function Description: This function reads one or two bytes from the slave board randomly
//-------------------------------------------------------------------------------------------------------
void readBytesFromSlave(unsigned char startAddr, unsigned char numBytes)
{
	if(numBytes == 1)
	{
		readOneByteFromSlave(startAddr);
	}
	else if(numBytes == 2)
	{
		readTwoBytesFromSlave(startAddr);
	}
	else
	{
		smbRead(MCU_SLAVE_ADDR, startAddr, numBytes);
	}																									
}


//-------------------------------------------------------------------------------------------------------
// Function Name: smbRead
// Return Value: unsigned char * 
// Parmeters: target, startAddr, bytes
// Function Description: This function reads from SM bus
//-------------------------------------------------------------------------------------------------------
void smbRead(unsigned char deviceId, unsigned int location, unsigned int bytes)
{
	char SFRPAGE_SAVE = SFRPAGE;
	
	slaveAddr = deviceId;                                        			// Address of MCU slave
	startAddr = location;													// Starting address to read from slave
	numBytesRD = bytes;														// Number of bytes to read
	
	SFRPAGE = SMB0_PAGE;
	SMB_BUSY = 1;                                           				// Claim SMBus (set to busy)
	SMB_RW = 1;                                             				// Mark this transfer as a READ
	STA = 1;
	SFRPAGE = SFRPAGE_SAVE;
											
}

//-------------------------------------------------------------------------------------------------------
// Function Name: smbWrite
// Return Value: unsigned char * 
// Parmeters: target, startAddr, bytes
// Function Description: This function reads to SM bus
//-------------------------------------------------------------------------------------------------------
void smbWrite(unsigned char deviceId, unsigned int location, unsigned int bytes)
{
	char SFRPAGE_SAVE = SFRPAGE;
		
	SFRPAGE = SMB0_PAGE;
	slaveAddr = deviceId;                                        				// Address of MCU slave board
	startAddr = location;														// Starting address to write to slave
	SFRPAGE = SFRPAGE_SAVE;															// Restore SFR page
}

//-------------------------------------------------------------------------------------------------------
// Function Name: smbISR
// Return Value: None 
// Parmeters: None
// Function Description: 
// SMBus Interrupt Service Routine (ISR)
// Anytime the SDA is pulled low by the master, this ISR will be called. For example, if STA = 1,
// this ISR is called and SMB0STA = SMB_START = SMB_REPEAT_START. These cases are executed within the switch statement.
//-------------------------------------------------------------------------------------------------------
void smbISR (void) interrupt INTERRUPT_SMB using 2
{
   	bit FAIL = 0;                                            						// Used by the ISR to flag failed transfers
   	static unsigned int TxCounter;													// Initialize counter
   	static unsigned int RxCounter;													// Initialize counter
	static unsigned int slaveCount = 0;
	
	switch (SMB0STA >> 3)															// Check SMB bus status
   	{
//-------------------------------------------------------------------------------------------------------
// Master Transmitter/Receiver: START condition transmitted. Load SMB0DAT with slave device address
//-------------------------------------------------------------------------------------------------------
      	case SMB_START:																// Master initiates a transfer

//-------------------------------------------------------------------------------------------------------
// Master Transmitter/Receiver: repeated START condition transmitted. Load SMB0DAT with slave device address
//-------------------------------------------------------------------------------------------------------
      	case SMB_REPEAT_START:
		 	SMB0DAT = slaveAddr;                                  					// Load address of the slave.
         	SMB0DAT &= 0xFE;                                   						// Clear the LSB of the address for the R/W bit
         	SMB0DAT |= SMB_RW;                                 						// Load R/W bit (Read = 1; Write = 0)
			STA = 0;                                           						// Manually clear STA bit

         	RxCounter = 0;                                     						// Reset the counter
         	TxCounter = 0;                                     						// Reset the counter		
			break;

//-------------------------------------------------------------------------------------------------------
// Master Transmitter: Slave address + WRITE transmitted.  ACK received. For a READ: N/A
// For a WRITE: Send the first data byte to the slave
//-------------------------------------------------------------------------------------------------------
      	case SMB_ADDR_W_TX_ACK_RX:
			if(slaveAddr == MCU_SLAVE_ADDR)
			{
				if(startAddr == DEVICE_DUMP_ADDR)									// Dump device address to check slave presence only
				{
					STO = 1;														// Stop this transfer
					SMB_BUSY = 0;													// Releas SMB
				}
				else
				{
					SMB0DAT = startAddr;											// Send 1 byte address to slave
					slaveWriteDone = 0;												// Mark start of slave write
				}

				slaveNoResponse = FALSE;											// Slave is alive
			}
         	break;

//-------------------------------------------------------------------------------------------------------
// Master Transmitter: Slave address + WRITE transmitted.  NACK received. Restart the transfer
//-------------------------------------------------------------------------------------------------------
      	case SMB_ADDR_W_TX_NACK_RX:
			if(slaveAddr == MCU_SLAVE_ADDR)
			{
				if(slaveCount < MAX_NACK_RETRY)
				{
				 	slaveCount++;													// Increment number of attempts when NACK is received
					STA = 1;														// Restart a new transfer
				}
				else
				{
				 	slaveNoResponse = TRUE;											// Report no response from slave
					slaveCount = 0;													// Reset this counter to keep retry seeking slave response
					slaveWriteDone = 1;
					STO = 1;
					SMB_BUSY = 0;
					FAIL = 1;
				}	
			}
         	break;

//-------------------------------------------------------------------------------------------------------
//Master Transmitter: Data byte transmitted.  ACK received. For a READ: N/A
//For a WRITE: Send all data.  After the last data byte, send the stop bit
//-------------------------------------------------------------------------------------------------------
      	case SMB_DATA_TX_ACK_RX:
         	if(slaveAddr == MCU_SLAVE_ADDR)
			{
				if(TxCounter < numBytesWR)
         		{
					SMB0DAT = sharedDataTx[startAddr + TxCounter];					// Send data byte
            		TxCounter++;
         		}
         		else
         		{
            		STO = 1;                                        				// Set STO to terminate transfer												
            		SMB_BUSY = 0;                                   				// And free SMBus interface
					slaveWriteDone = 1;												// Mark end of slave write
         		}
			}
         	break;

//-------------------------------------------------------------------------------------------------------
// Master Transmitter: Data byte transmitted.  NACK received. Restart the transfer
//-------------------------------------------------------------------------------------------------------
      	case SMB_DATA_TX_NACK_RX:
			if(slaveAddr == MCU_SLAVE_ADDR)
			{
				if(slaveCount < MAX_NACK_RETRY)
				{
				 	slaveCount++;													// Increment number of attempts when NACK is received
					STA = 1;														// Restart a new transfer
				}
				else
				{
				 	slaveNoResponse = TRUE;											// Report no response from slave
					slaveCount = 0;
					STO = 1;
					SMB_BUSY = 0;
					FAIL = 1;
				}	
			}
         	break;

//-------------------------------------------------------------------------------------------------------
// Master Receiver: Slave address + READ transmitted.  ACK received. 
// For a READ: check if this is a one-byte transfer. if so, set the NACK after the data byte
// is received to end the transfer. if not, set the ACK and receive the other data bytes
//-------------------------------------------------------------------------------------------------------
      	case SMB_ADDR_R_TX_ACK_RX:
         	if(numBytesRD == 1)														// If there is one byte to transfer, send a NACK and go to
         	{																		// SMB_DATA_RX_NACK_TX case to accept data from slave
            	AA = 0;                                         					// Clear AA flag before data byte is received
                                                            						// send NACK signal to slave after byte is received
         	}
         	else
         	{
            	AA = 1;                                         					// More than one byte in this transfer,
                                                            						// send ACK after byte is received
         	}
         	break;

//-------------------------------------------------------------------------------------------------------
// Master Receiver: Slave address + READ transmitted.  NACK received. Restart the transfer
//-------------------------------------------------------------------------------------------------------
      	case SMB_ADDR_R_TX_NACK_RX:
			if(slaveAddr == MCU_SLAVE_ADDR)
			{
				if(slaveCount < MAX_NACK_RETRY)
				{
				 	slaveCount++;													// Increment number of attempts when NACK is received
					STA = 1;														// Restart transfer after receiving a NACK
				}
				else
				{
				 	slaveNoResponse = TRUE;											// Report no response from slave
					slaveCount = 0;													// Reset counter
					STO = 1;
					SMB_BUSY = 0;
					FAIL = 1;
				}	
			}
         	break;

//-------------------------------------------------------------------------------------------------------
// Master Receiver: Data byte received. If AA flag was set beforehand, then ACK transmitted.
// For a READ: receive each byte from the slave.  if this is the last byte, send a NACK and set the STOP bit
//-------------------------------------------------------------------------------------------------------
      	case SMB_DATA_RX_ACK_TX:
			
			if(slaveAddr == MCU_SLAVE_ADDR)
			{
				if (RxCounter < numBytesRD)
         		{
					sharedDataRx[startAddr + RxCounter] = SMB0DAT;
            		AA = 1;                                         				// Send ACK to indicate byte received
            		RxCounter++;                                    				// Increment the byte counter
					slaveReadDone = 0;												// Mark start of slave read
         		}
         		else
         		{
            		AA = 0;                                         				// Send NACK to indicate last byte is received
					slaveReadDone = 1;												// Mark end of slave read
         		}
			}
         	break;

//-------------------------------------------------------------------------------------------------------
// Master Receiver: Data byte received. If AA flag was cleared, then NACK transmitted.
// For a READ: Read operation has completed.  Read data register and send STOP
//-------------------------------------------------------------------------------------------------------
      	case SMB_DATA_RX_NACK_TX:
		 	if(slaveAddr == MCU_SLAVE_ADDR)
			{
				sharedDataRx[startAddr + RxCounter] = SMB0DAT;
         		STO = 1;															// Stop transfer
         		SMB_BUSY = 0;														// Release SMB
         		AA = 1;
				slaveReadDone = 1;													// Mark end of slave read																// Set AA for next transfer                                          				
			}
         	break;

//-------------------------------------------------------------------------------------------------------
// Master Transmitter: Arbitration lost
//-------------------------------------------------------------------------------------------------------
      	case SMB_ARBITRATION_LOST:
         	FAIL = 1;                                          						// Indicate failed transfer
                                                            						// and handle at end of ISR
         	break;

//-------------------------------------------------------------------------------------------------------
// All other status codes invalid.  Reset communication
//-------------------------------------------------------------------------------------------------------
      	default:
         	FAIL = 1;
         	break;
   }
//-------------------------------------------------------------------------------------------------------
// If all failed, reset everything
//-------------------------------------------------------------------------------------------------------
   	if(FAIL)                                                						// If the transfer failed,
   	{
      	SMB0CN &= ~0x40;                                      						// Reset communication
      	SMB0CN |= 0x40;
      	STA = 0;
      	STO = 0;
      	AA = 0;

      	SMB_BUSY = 0;                                         						// Free SMBus

      	FAIL = 0;
																					// Set to finish all pending processes
		slaveWriteDone = 1;															// Mark end of slave write
		slaveReadDone = 1;															// Mark end of slave read
	}

	SI = 0;                                                  						// Clear interrupt flag
}

//-------------------------------------------------------------------------------------------------------
// End Of File
//-------------------------------------------------------------------------------------------------------