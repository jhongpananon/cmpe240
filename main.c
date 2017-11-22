#include <main.h>
#include "macros.h"

//-------------------------------------------------------------------------------------------------------
// Global Declarations
//-------------------------------------------------------------------------------------------------------
unsigned char tsByte;
unsigned char passcode[4][3] = {"140", "141", "142","143"}; 

int k = 0;

bit splashEnd = 0;
bit screenReset = 0;
bit ackFromScreen = 0;
bit tsCommandReceived = 0;
bit tsCommandTransmitted = 0;
bit SMB_RW;                                                                     // Software flag to indicate Read or Write

unsigned char sharedDataRx[SHARED_DATA_MAX];
unsigned char sharedDataTx[SHARED_DATA_MAX];
unsigned char eepromTx[EEPROM_TX_BUFFER];
unsigned char eepromRx[EEPROM_RX_BUFFER];
unsigned char eepromPageTx[EEPROM_PAGE_SIZE];

unsigned int pageCount;
unsigned int bytesExtra;
unsigned int eepromPageCounter;
unsigned int startAddr;
unsigned char slaveAddr;                                                        // Target SMBus slave address
unsigned char eepromDataByte;

unsigned int numBytesRD;
unsigned int numBytesWR;
unsigned char slaveWriteDone;
unsigned char slaveReadDone;
unsigned char eepromWriteDone;
unsigned char eepromReaddone;

unsigned char tsRxBuffer[RX_BUFFER_SIZE];
unsigned char tsTxBuffer[TX_BUFFER_SIZE];
unsigned char userCommand[RX_BUFFER_SIZE];

unsigned int tsRxIn;
unsigned int tsRxOut; 
unsigned int tsTxIn;
unsigned int tsTxOut;

bit tsRxEmpty;
bit tsTxEmpty;
bit tsLastCharGone; 

bit screenChanged;
unsigned char screen;
unsigned char lastScreen;

const char code * Font[] = {/*0*/   "m10B", 
                            /*1*/   "m12B",
                            /*2*/   "m14B",
                            /*3*/   "m16B",
                            /*4*/   "m20B",
                            /*5*/   "m24B",
                            /*6*/   "m32B",
                            /*7*/   "m48",
                            /*8*/   "m64"};

bit SMB_BUSY = 0;                                                               // Set to claim the bus, clear to free
bit SMB_RW;                                                                     // Software flag to indicate Read or Write

unsigned int startAddr;
unsigned char slaveAddr;                                                        // Target SMBus slave address

unsigned int numBytesRD;
unsigned int numBytesWR;

unsigned char slaveWriteDone;
unsigned char slaveReadDone;
                            
unsigned char roomTemp;

//-------------------------------------------------------------------------------------------------------
// System Configurations
//-------------------------------------------------------------------------------------------------------
void systemClockInit(void)
{
    char SFRPAGE_SAVE = SFRPAGE;                                                // Save Current SFR page
    int i = 0;
    
    SFRPAGE  = CONFIG_PAGE;
    
    OSCICN    = 0x83;

    SFRPAGE = SFRPAGE_SAVE;                                                     // Restore SFRPAGE
}

void portInit(void)
{
    char SFRPAGE_SAVE = SFRPAGE;                                                // Save Current SFR page

    SFRPAGE = CONFIG_PAGE;                                                      // Set SFR page

    XBR0 = 0x2F;                                                                // Enable UART0, UART1, SPI0, SMB, CEX0 - CEX4                                                                          
    XBR1 = 0x01;                                        
    XBR2 = 0xC4;                                                                // Enable crossbar and disable weak pull-up                                             
    
    P0MDOUT = 0x01;                                                             // Set TX0 pin to push-pull
                                                                                // TX0 = P0.0; RX0 = P0.1
    P1MDOUT = 0x01;                                                             // Set TX1 pin to push-pull, P1.0
                                                                                // TX1 = P1.0; RX1 = P1.1
    P3MDOUT = 0xCA;                                                             // P3.0, P3.2, P3.4, P3.5: open drain; P3.1, P3.3, P3.6, P3.7: push pull
    
    P4MDOUT = 0x0F;                                                             // P4.0, P4.1, P4.2, P4.3: push pull
                                                                                
    P5MDOUT = 0x04;                                                             // P5.0 open drain; P5.1 Open drain; P5.2 Push pull
    
    P6MDOUT = 0x00;
                                                                                                                                                            
    P7MDOUT = 0x80;                                                             // Set P7.7 push-pull (smb error line)
    
    P0 = 0xFF;                                                                  // Initialize port P0 latch
    P1 = 0xFF;                                                                  // Initialize port P1 latch
    P2 = 0xFF;                                                                  // Initialize port P2 latch
    P3 = 0xFF;                                                                  // Initialize port P3 latch
    P4 = 0xFF;                                                                  // Initialize port P4 latch
    P5 = 0xFF;                                                                  // Initialize port P5 latch
    P6 = 0xFF;                                                                  // Initialize port P6 latch
    P7 = 0xFF;                                                                  // Initialize port P7 latch

    RHW = 0;                                                                    // Pull low SMB error line

    SFRPAGE = SFRPAGE_SAVE;                                                     // Restore SFR page
}

void enableInterrupts(void)
{
    IE = 0x92;                                                                  // Enable all interrupts + UART0 + Timer 0
    EIE2 |= 0x01;                                                               // Enable Timer 3 interrupt
    EIE2 |= 0x40;                                                               // Enable UART1 interrupt
    EIE1 |= 0x0A;                                                               // Enable SMBus interrupt
}

void uart0Init(void)
{
   char SFRPAGE_SAVE;

   SFRPAGE_SAVE = SFRPAGE;                                                      // Preserve SFRPAGE

   SFRPAGE = TMR2_PAGE;
   TMR2CN = 0x00;                                                               // Stop timer. Timer 2 in 16-bit auto-reload up timer mode
   TMR2CF = 0x08;                                                               // SYSCLK is time base; no output; up count only
   RCAP2L = 0xF3;                                                               // Low byte
   RCAP2H = 0xFF;                                                               // High byte
   TMR2 = RCAP2;                                                                // Load 16 bit reload value into timer 2
   TMR2CN = 0x04;                                                               // Enable timer 2 (Start timer 2)

   SFRPAGE = UART0_PAGE;
   SCON0 = 0x50;                                                                // 8-bit variable baud rate; 9th bit ignored; RX enabled
   SSTA0 = 0x05;                                                                // Enable baud rate                                                                         
                                                                                // Use timer 2 as RX and TX baud rate source
   IE = 0x90;                                                                   // Enable all interrupts and UART0 Interrupt

   SFRPAGE = SFRPAGE_SAVE;                                                      // Restore SFRPAGE
}

void disableWatchdog(void)
{
    WDTCN = 0xDE;                                                               // Disable watchdog timer
    WDTCN = 0xAD;
}

void uart0Interrupt(void) interrupt INTERRUPT_UART_0 using 2
{
    char SFRPAGE_SAVE = SFRPAGE;
    unsigned int i = 0;
    unsigned long txWaitCounter = 0;

    SFRPAGE = UART0_PAGE;

    if(RI0 == 1)                                                                // There is a char in SBUF
    {
        RI0 = 0;                                                                // Clear interrupt flag

        tsByte = SBUF0;                                                         // Read a character from UART

        if(tsRxIn < RX_BUFFER_SIZE)                                             // If buffer size is within limit
        {
            if(tsByte != '\r')                                                  // Check end of a command from touch screen
            {
                tsRxBuffer[tsRxIn] = tsByte;                                    // Store a character in software buffer
                tsRxIn++;                                                       // Increment index
            }
            else                                                                // If it is CR character, it marks end of command
            {                                                               
                if(tsRxBuffer[0] == '{')                                        // Splash screen indicator
                {
                    if(tsRxBuffer[1] == 'c' && tsRxBuffer[2] == 'm' && tsRxBuffer[3] == 'p' && tsRxBuffer[4] == 'e' && tsRxBuffer[5] == '}')
                    {
                        splashEnd = 1;                                          // Detect end of splash screen
                        screenReset = 1;                                        // Screen was reset, so touch screen sends {babe\r}
                    }
                    else
                    {
                        splashEnd = 0;                                          // End of splash screen NOT detected
                        screenReset = 0;                                    
                    }
                }
                else if(tsRxBuffer[0] == 'x')                                   // It is a command from touch screen controller
                {                                                               // A command starts with '('
                    for(i = 0; i < tsRxIn; i++)
                    {
                        userCommand[i] = tsRxBuffer[i];                         // Copy to command array for later evaluation
                    }

                    ackFromScreen = 0;                                          // This is a command, NOT an ACK
                    tsCommandReceived = 1;                                      // Set flag when a complete command is received
                }
                else if(tsRxBuffer[0] == '(')                                   // It is a command from touch screen controller
                {                                                               // A command starts with '('
                    for(i = 0; i < tsRxIn; i++)
                    {
                        userCommand[i] = tsRxBuffer[i];                         // Copy to command array for later evaluation
                    }

                    ackFromScreen = 0;                                          // This is a command, NOT an ACK
                    tsCommandReceived = 1;                                      // Set flag when a complete command is received
                }
                else                                                            // Not a command from touch screen controller
                {
                    ackFromScreen = 1;                                          // Set a flag to indicate it is an ACK from screen
                    tsCommandReceived = 0;                                      // No need to set flag because it is not a command
                }

                for(i = 0; i < tsRxIn; i++)
                {
                    tsRxBuffer[i] = '\0';                                       // Delete all contents
                }
                
                tsRxOut = 0;                                                    // Reset index Out
                tsRxIn = 0;                                                     // Reset index In
            }   
        }
        else                                                                    // Reset all indexes
        {   
            while(tsRxOut < tsRxIn)
            {
                tsRxBuffer[tsRxOut] = '\0';
                tsRxOut++;
            }
            tsRxOut = 0;
            tsRxIn = 0;
            tsCommandReceived = 0;
        }
   }

   if(TI0 == 1)                                                                 // Check if one character is successfully sent out
   {
        TI0 = 0;                                                                // Clear interrupt flag

        if(tsTxEmpty == 0)                                                      // TX buffer has something to send
        {
            SBUF0 = tsTxBuffer[tsTxOut];                                        // Send a character in TX buffer
            tsTxOut++;                                                          // Move to next character

            while(TI0 == 0 && txWaitCounter++ < TX_WAIT_LIMIT);                 // Wait until completion of transmission TI0 = 1
            if(txWaitCounter >= TX_WAIT_LIMIT)
            {
                TI0 = 1;                                                        // TI0 is not set by hardware, set it by software
            }                                                                   // When TI0 is set to 1, this ISR is executed again
            
            txWaitCounter = 0;                                                  // Reset counter for next execution 

            if(tsTxOut >= TX_BUFFER_SIZE)
            {
                tsTxOut = 0;                                                    // Reset index to 0
            }                               

            if(tsTxOut == tsTxIn)                                               // If two indexes are equal
            {
                tsTxEmpty = 1;                                                  // No more character in buffer. Empty
            }
        }
        else
        {
            tsLastCharGone = 1;                                                 // Last character has gone. Buffer is empty
        }
    }
    
    SFRPAGE = SFRPAGE_SAVE;                                                     // Restore SFR page detector
}

//-------------------------------------------------------------------------------------------------------
// Function Name: sendCommand
// Return Value: None 
// Parmeters: s (a string to send)
// Function Description: This function sends a command from the touch screen
//-------------------------------------------------------------------------------------------------------
void sendCommand(const char * s)
{   
    char SFRPAGE_SAVE = SFRPAGE;
    
    while(*s != '\0')                                                           // Search for end of touch screen command in buffer
    {
        if(tsTxEmpty == 1 || (tsTxOut != tsTxIn))                               // Tx is empty or two indexes are not equal
        {
            tsTxBuffer[tsTxIn++] = *s;
            if(tsTxIn >= TX_BUFFER_SIZE)                                        // Check for limit
            {
                tsTxIn = 0;                                                     // Reset if limit reached
            }

            if(tsTxEmpty == 1)                                                  // If buffer is empty
            {
                tsTxEmpty = 0;                                                  // Now buffer has at leat 1 character, set flag
            }
        }
        
        s++;                                                                    // Point to next char to send out
    }
                                                                                
    if(tsLastCharGone == 1)                                                     // All characters in buffer has sent out
    {
        tsLastCharGone = 0;                                                     // Reset flag to indicate no char left in buffer
        SFRPAGE = UART0_PAGE;                                                                           
        TI0 = 1;                                                                // Set this flage to call ISR to send out one character
    }                                                                           
                                                                                
    SFRPAGE = SFRPAGE_SAVE;                                                     // Restore SFRPAGE
}

//-------------------------------------------------------------------------------------------------------
// Function Name: displayText
// Return Value: None 
// Parmeters: fg, bg, size, message, x, y
// Function Description: This function displays a text on the touch screen
//-------------------------------------------------------------------------------------------------------
void displayText(const char* fg, const char* bg, const unsigned char size, const char* message, const unsigned int x, const unsigned int y)
{
    char str[TS_BUFFER_SIZE];                                                   // String
        
    sprintf(str, "S %s %s\r", fg, bg);                                          // Set forground and background color
    sendCommand(str);                                                       
    sprintf(str, "f %s\r", Font[size]);                                         // Set text font
    sendCommand(str);                                                       
    sprintf(str, "t \"%s\" %u %u\r", message, x, y);                            // Display text
    sendCommand(str);                                                       
}


//-------------------------------------------------------------------------------------------------------
// Function Name: showBitmap
// Return Value: None 
// Parmeters: index, x, y (bitmap index and coordinates)
// Function Description: This function displays a bitmap image
//-------------------------------------------------------------------------------------------------------
void showBitmap(const unsigned int index, const unsigned int x, const unsigned int y)
{
    char str[TS_BUFFER_SIZE];

    sprintf(str, "xi %u %u %u\r", index, x, y);                                 // Bitmap index
    sendCommand(str);
}

//-------------------------------------------------------------------------------------------------------
// Function Name: changeScreen
// Return Value: None 
// Parmeters: screenIndex (macro number)
// Function Description: This function switches to the new screen
//-------------------------------------------------------------------------------------------------------
void changeScreen(const unsigned char screenIndex)
{
    callMacro(screenIndex);                                                     // Change screen        
}

//-------------------------------------------------------------------------------------------------------
// Function Name: callMacro
// Return Value: None 
// Parmeters: macroNumber (macro number in the macro file)
// Function Description: This function calls a macro
//-------------------------------------------------------------------------------------------------------
void callMacro(const unsigned int macroNumber)
{
    char str[TS_BUFFER_SIZE];

    sprintf(str, "m %u\r", macroNumber);                                        // Execute macro number
    sendCommand(str);
}

//-------------------------------------------------------------------------------------------------------
// Function Name: scanUserInput
// Return Value: None 
// Parmeters: None
// Function Description: This function processes commands from the touch screen
//-------------------------------------------------------------------------------------------------------
void scanUserInput(void)
{   
    int i = 0;

    if(screen == MAIN_PAGE)                                                     // Main screen
    {        
        if(userCommand[0] == '(')                                               // Check for an actual command followed by this '(' character
        {
            switch (userCommand[1])                                             // Scan a command type
            {
                case '1':                                                       // Main page
                    changeScreen(MAIN_PAGE);                                    // Stay in main page if main button is pressed again
                    break;
                case '2':                                                       // Settings page
                    changeScreen(SETTINGS_PAGE);
                    break;
                case '3':                                                       // Service page
                    changeScreen(SERVICE_PAGE);
                    break;
                case 'A':
                    // Call a function here or do something here            
                    break;
                case 'B':
                    // Call a function here or do something here
                    break;
                case 'C':
                    // Call a function here or do something here
                    break;  
                default:                                                        // Other options
                    break;
                }
        }
        else                                                                    // Not a command, empty buffer with null char
        {
            i = 0;
            while(userCommand[i] != '\0')
            {
                userCommand[i] = '\0';
                i++;
            }
        }   
    }
    else if(screen == SETTINGS_PAGE)                                            // Settings page
    {
        if(userCommand[0] == '(')
        {
            switch (userCommand[1])
            {
                case '1':
                    changeScreen(MAIN_PAGE);
                    break;
                case '2':
                    changeScreen(SETTINGS_PAGE);
                    break;
                case '3':
                    changeScreen(SERVICE_PAGE);
                    break;
                case 'A':
                    // Call a function here or do something here
                    break;
                case 'B':
                    // Call a function here or do something here
                    break;
                case 'C':
                    // Call a function here or do something here
                    break;
                default:
                    break;
            }
        }
        else                                                                    // Not a command, empty buffer with null
        {
            i = 0;
            while(userCommand[i] != '\0')
            {
                userCommand[i] = '\0';
                i++;
            }
        }               
    }
    else if(screen == SERVICE_PAGE)
    {
        if(userCommand[0] == '(')
        {
            switch (userCommand[1])
            {
                case '1':
                    changeScreen(MAIN_PAGE);
                    break;
                case '2':
                    changeScreen(SETTINGS_PAGE);
                    break;
                case '3':
                    changeScreen(SERVICE_PAGE);
                    break;
                case 'A':
                    // Call a function here or do something here
                case 'B':
                    // Call a function here or do something here
                case 'C':
                    // Call a function here or do something here
                default:
                    break;
            }
        }
        else                                                                    // Not a command, empty buffer with null
        {                                                           
            i = 0;
            while(userCommand[i] != '\0')
            {
                userCommand[i] = '\0';
                i++;
            }
        }               
    }   
    else
    {

    }

    i = 0;
    while(userCommand[i] != '\0')
    {
        userCommand[i] = '\0';                                                  // Delete all contents in array
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------
// Function Name: smbInit
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
    while(SDA == 0 && pollingCounter++ < SMB_POLLING_LIMIT)                         // If slave is holding SDA low because of error or reset
    {
        SCL = 0;                                                                    // Drive the clock low
        for(i = 0; i < 255; i++);                                                   // Hold the clock low
        SCL = 1;                                                                    // Release the clock
        while(SCL == 0 && pollingCounter++ < SMB_POLLING_LIMIT);                    // Wait for open-drain
        for(i = 0; i < 10; i++);                                                    // Hold the clock high
    }
            
    SMB0CN = 0x07;                                                                  // Assert Acknowledge low (AA bit = 1b);
                                                                                    // Enable SMBus Free timeout detect;
    SMB0CR = 267 - (SYSTEM_CLOCK / (8 * SMB_FREQUENCY));                            // Derived approximation from the Tlow and Thigh equations
                                                                    
    SMB0CN |= 0x40;                                                                 // Enable SMBus;

    SFRPAGE = SFRPAGE_SAVE;                                                         // Restore SFR page detector
    
    SMB_BUSY = 0;                                                                   // Release SMB
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

    TMR3CN = 0x00;                                                                  // Timer 3 in timer mode
                                                                                    // Timer 3 auto reload
    TMR3CF = 0x00;                                                                  // Timer 3 prescaler = 12

    RCAP3 = -(SYSTEM_CLOCK / 12 / 40);                                              // Timer 3 overflows after 25 ms
    TMR3 = RCAP3;                                                               

    TR3 = 1;                                                                        // Start Timer 3

    SFRPAGE = SFRPAGE_SAVE;                                                         // Restore SFR page
}

//-------------------------------------------------------------------------------------------------------
// Function Name: timer3ISR
// Return Value: None 
// Parmeters: None
// Function Description: This function is timer 3 ISR which is used to reset the SMB bus if the clock line is held for too long
//-------------------------------------------------------------------------------------------------------
void timer3ISR(void) interrupt INTERRUPT_Timer_3
{
    char SFRPAGE_SAVE = SFRPAGE;                                                    // Save Current SFR page

    SFRPAGE = SMB0_PAGE;
    SMB0CN &= ~0x40;                                                                // Disable SMBus
    SMB0CN |= 0x40;                                                                 // Re-enable SMBus

    SFRPAGE = SFRPAGE_SAVE;                                                         // Switch back to the Timer3 SFRPAGE
    TF3 = 0;                                                                        // Clear Timer3 interrupt-pending flag
    SMB_BUSY = 0;                                                                   // Free bus
   
    SFRPAGE = SFRPAGE_SAVE;                                                         // Restore SFR page detector
}

//-------------------------------------------------------------------------------------------------------
// Function Name: writeOneByteToSlave
// Return Value: None 
// Parmeters: target, startAddr, content
// Function Description: This function writes one to the slave microprocessor
//-------------------------------------------------------------------------------------------------------
void writeOneByteToSlave(unsigned char startAddr, unsigned char content)
{           
    sharedDataTx[startAddr] = content;
    smbWrite(MCU_SLAVE_ADDR, startAddr, 1);
}

//-------------------------------------------------------------------------------------------------------
// Function Name: readOneByteFromSlave
// Return Value: long 
// Parmeters: startAddr, bytes
// Function Description: This function reads one from the slave microprocessor
//-------------------------------------------------------------------------------------------------------
unsigned char readOneByteFromSlave(unsigned char startAddr)
{
    smbRead(MCU_SLAVE_ADDR, startAddr, 1);
    return sharedDataRx[startAddr];                                                                                             
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

    SFRPAGE = SMB0_PAGE;
    
    while(BUSY || SMB_BUSY);                                                        // Wait for free SMB

    SFRPAGE = SFRPAGE_SAVE;
        
    switch(deviceId)
    {
        case MCU_SLAVE_ADDR:
        case EEPROM_ADDR:
            smbWrite(deviceId, location, 0);                                        // Write address before reading
            break;
        default:
            break;  
    }

    SFRPAGE = SMB0_PAGE;
        
    while(BUSY || SMB_BUSY);
    slaveAddr = deviceId;                                                           // Address of MCU slave
    startAddr = location;                                                           // Starting address to read from slave
    numBytesRD = bytes;                                                             // Number of bytes to read
    
    SMB_BUSY = 1;                                                                   // Claim SMBus (set to busy)
    SMB_RW = 1;                                                                     // Mark this transfer as a READ
    STA = 1;

    while(BUSY || SMB_BUSY);                                                        // Wait for SMB
        
    SFRPAGE = SFRPAGE_SAVE;
            
    switch(deviceId)
    {
        case MCU_SLAVE_ADDR:
            while(slaveReadDone == 0);                                              // Wait until slave write completed
            break;
        case EEPROM_ADDR:
            while(eepromReadDone == 0);                                             // Wait until EEPROM write completed
            break;
        default:
            break;  
    }    
}

//-------------------------------------------------------------------------------------------------------
// Function Name: smbWrite
// Return Value: unsigned char * 
// Parmeters: target, startAddr, bytes
// Function Description: This function reads to SM bus
//-------------------------------------------------------------------------------------------------------
void smbWrite(unsigned char deviceId, unsigned int location, unsigned int bytes)
{
    unsigned char i = 0;
    unsigned int pageWrittenDelay = 0;
    char SFRPAGE_SAVE = SFRPAGE;
        
    SFRPAGE = SMB0_PAGE;
    
    while(BUSY || SMB_BUSY);                                                        // Wait for SMB to be free
    slaveAddr = deviceId;                                                           // Address of MCU slave board
    startAddr = location;                                                           // Starting address to write to slave

    switch(deviceId)
    {
        case MCU_SLAVE_ADDR:                                                        // Pass through
        case DEVICE_DUMP_ADDR:                                                      // Pass through
            numBytesWR = bytes;                                                     // Number of bytes to read
            SMB_BUSY = 1;                                                           // Claim SMBus (set to busy)
            SMB_RW = 0;                                                             // Mark this transfer as a WRITE
            STA = 1;                                                                // Start transfer
            while(slaveWriteDone == 0);                                             // Wait until SRAM write completed or timeout occurs
            break;
        default:
            break;      
    }

    SFRPAGE = SFRPAGE_SAVE;                                                         // Restore SFR page
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
    bit FAIL = 0;                                                                   // Used by the ISR to flag failed transfers
    static unsigned int TxCounter;                                                  // Initialize counter
    static unsigned int RxCounter;                                                  // Initialize counter
    static unsigned int slaveCount = 0;
    static unsigned int eepromCount = 0;
    static unsigned char eepromAddrDone;
    
    switch (SMB0STA >> 3)                                                           // Check SMB bus status
    {
//-------------------------------------------------------------------------------------------------------
// Master Transmitter/Receiver: START condition transmitted. Load SMB0DAT with slave device address
//-------------------------------------------------------------------------------------------------------
        case SMB_START:                                                             // Master initiates a transfer

//-------------------------------------------------------------------------------------------------------
// Master Transmitter/Receiver: repeated START condition transmitted. Load SMB0DAT with slave device address
//-------------------------------------------------------------------------------------------------------
        case SMB_REPEAT_START:
            SMB0DAT = slaveAddr;                                                    // Load address of the slave.
            SMB0DAT &= 0xFE;                                                        // Clear the LSB of the address for the R/W bit
            SMB0DAT |= SMB_RW;                                                      // Load R/W bit (Read = 1; Write = 0)
            STA = 0;                                                                // Manually clear STA bit

            RxCounter = 0;                                                          // Reset the counter
            TxCounter = 0;                                                          // Reset the counter
            eepromAddrDone = CLEAR;                                                 // For 2 byte EEPROM address    
            
            break;

//-------------------------------------------------------------------------------------------------------
// Master Transmitter: Slave address + WRITE transmitted.  ACK received. For a READ: N/A
// For a WRITE: Send the first data byte to the slave
//-------------------------------------------------------------------------------------------------------
        case SMB_ADDR_W_TX_ACK_RX:
            if(slaveAddr == MCU_SLAVE_ADDR)
            {
                if(startAddr == DEVICE_DUMP_ADDR)                                   // Dump device address to check slave presence only
                {
                    STO = 1;                                                        // Stop this transfer
                    SMB_BUSY = 0;                                                   // Releas SMB
                }
                else
                {
                    SMB0DAT = startAddr;                                            // Send 1 byte address to slave
                    slaveWriteDone = 0;                                             // Mark start of slave write
                }
            }
            break;

//-------------------------------------------------------------------------------------------------------
// Master Transmitter: Slave address + WRITE transmitted.  NACK received. Restart the transfer
//-------------------------------------------------------------------------------------------------------
        case SMB_ADDR_W_TX_NACK_RX:
            if(slaveAddr == MCU_SLAVE_ADDR || slaveAddr == WAVEFORM_GEN_ADDR)
            {
                if(slaveCount < MAX_NACK_RETRY)
                {
                    slaveCount++;                                                   // Increment number of attempts when NACK is received
                    STA = 1;                                                        // Restart a new transfer
                }
                else
                {
                    slaveCount = 0;                                                 // Reset this counter to keep retry seeking slave response
                    slaveWriteDone = 1;
                    STO = 1;
                    SMB_BUSY = 0;
                    FAIL = 1;
                }   
            }
            else if(slaveAddr == EEPROM_ADDR)
            {
                if(eepromCount < MAX_NACK_RETRY)
                {
                    eepromCount++;                                                  // Increment number of attempts when NACK is received
                    STA = 1;                                                        // Restart a new transfer
                }
                else
                {
                    eepromCount = 0;
                    eepromWriteDone = 1;
                    STO = 1;
                    SMB_BUSY = 0;
                    FAIL = 1;
                }   
            }
            else
            {}

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
                    SMB0DAT = sharedDataTx[startAddr + TxCounter];                  // Send data byte
                    TxCounter++;
                }
                else
                {
                    STO = 1;                                                        // Set STO to terminate transfer                                                
                    SMB_BUSY = 0;                                                   // And free SMBus interface
                    slaveWriteDone = 1;                                             // Mark end of slave write
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
                    slaveCount++;                                                   // Increment number of attempts when NACK is received
                    STA = 1;                                                        // Restart a new transfer
                }
                else
                {
                    slaveCount = 0;
                    STO = 1;
                    SMB_BUSY = 0;
                    FAIL = 1;
                }   
            }
            else if(slaveAddr == EEPROM_ADDR)
            {
                if(eepromCount < MAX_NACK_RETRY)
                {
                    eepromCount++;                                                  // Increment number of attempts when NACK is received
                    STA = 1;                                                        // Restart a new transfer
                }
                else
                {
                    eepromCount = 0;
                    STO = 1;
                    SMB_BUSY = 0;
                    FAIL = 1;
                }   
            }
            else
            {}
            break;

//-------------------------------------------------------------------------------------------------------
// Master Receiver: Slave address + READ transmitted.  ACK received. 
// For a READ: check if this is a one-byte transfer. if so, set the NACK after the data byte
// is received to end the transfer. if not, set the ACK and receive the other data bytes
//-------------------------------------------------------------------------------------------------------
        case SMB_ADDR_R_TX_ACK_RX:
            if(numBytesRD == 1)                                                     // If there is one byte to transfer, send a NACK and go to
            {                                                                       // SMB_DATA_RX_NACK_TX case to accept data from slave
                AA = 0;                                                             // Clear AA flag before data byte is received
                                                                                    // send NACK signal to slave after byte is received
            }
            else
            {
                AA = 1;                                                             // More than one byte in this transfer,
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
                    slaveCount++;                                                   // Increment number of attempts when NACK is received
                    STA = 1;                                                        // Restart transfer after receiving a NACK
                }
                else
                {
                    slaveCount = 0;                                                 // Reset counter
                    STO = 1;
                    SMB_BUSY = 0;
                    FAIL = 1;
                }   
            }
            else if(slaveAddr == EEPROM_ADDR)
            {
                if(eepromCount < MAX_NACK_RETRY)
                {
                    eepromCount++;                                                  // Increment number of attempts when NACK is received
                    STA = 1;                                                        // Restart a new transfer
                }
                else
                {
                    eepromCount = 0;
                    STO = 1;
                    SMB_BUSY = 0;
                    FAIL = 1;
                }   
            }
            else
            {}
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
                    AA = 1;                                                         // Send ACK to indicate byte received
                    RxCounter++;                                                    // Increment the byte counter
                    slaveReadDone = 0;                                              // Mark start of slave read
                }
                else
                {
                    AA = 0;                                                         // Send NACK to indicate last byte is received
                    slaveReadDone = 1;                                              // Mark end of slave read
                }
            }
            else if(slaveAddr == EEPROM_ADDR)
            {
                if(RxCounter < numBytesRD)
                {
                    eepromDataByte = eepromRx[RxCounter] = SMB0DAT;
                    AA = 1;                                                         // Send ACK to indicate byte received
                    RxCounter++;                                                    // Increment the byte counter
                    eepromReadDone = 0;                                             // Mark start of fram read
                }
                else
                {
                    AA = 0;                                                         // Send NACK to indicate last byte is received
                    eepromReadDone = 1;                                             // Mark end of fram read
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
                STO = 1;                                                            // Stop transfer
                SMB_BUSY = 0;                                                       // Release SMB
                AA = 1;
                slaveReadDone = 1;                                                  // Mark end of slave read                                                               // Set AA for next transfer                                                         
            }
            else if(slaveAddr == EEPROM_ADDR)
            {
                eepromRx[RxCounter] = SMB0DAT;
                STO = 1;                                                            // Stop transfer
                SMB_BUSY = 0;                                                       // Release SMB
                AA = 1;
                eepromReadDone = 1;                                                 // Mark end of eeprom read      
            }
            break;

//-------------------------------------------------------------------------------------------------------
// Master Transmitter: Arbitration lost
//-------------------------------------------------------------------------------------------------------
        case SMB_ARBITRATION_LOST:
            FAIL = 1;                                                               // Indicate failed transfer
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
    if(FAIL)                                                                        // If the transfer failed,
    {
        SMB0CN &= ~0x40;                                                            // Reset communication
        SMB0CN |= 0x40;
        STA = 0;
        STO = 0;
        AA = 0;

        SMB_BUSY = 0;                                                               // Free SMBus

        FAIL = 0;
                                                                                    // Set to finish all pending processes
        slaveWriteDone = 1;                                                         // Mark end of slave write
        slaveReadDone = 1;                                                          // Mark end of slave read
    }

    SI = 0;                                                                         // Clear interrupt flag
}

void display_text(const char * fg, const char * bg, const unsigned char size, const char * message, const int x, const int y)
{
    char str[128] = { 0 };
    
    int i = 0;
    while(i < 10000) i++;
        
    sprintf(str, "S %s %s\r", fg, bg);
    sendCommand(str);
    sprintf(str, "f %s\r", Font[size]);
    sendCommand(str);
    sprintf(str, "t \"%s\" %u %u\r", message, x, y);
    sendCommand(str);
}


static void send_macro(const unsigned int macro_index)
{
    char str[8] = { 0 };
    
    int i = 0;
    while(i < 10000) i++;
        
    sprintf(str, "m %u\r", macro_index);
    sendCommand(str);
}

//-------------------------------------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------------------------------------

#define PAGE_MAIN            1
#define PAGE_SETTINGS        2
#define PAGE_SERVICE         3
#define PAGE_CONFIG          4
int current_page;

void main()
{
    int i = 0;
    int count = 0;
    int prev_temp = 0;
    int display_celsius = 0;
    int state_changed = 1;
    char str[SPRINTF_SIZE];
    
    disableWatchdog();
    systemClockInit();
    portInit();
    enableInterrupts();
    uart0Init();
    smbInit();
    timer3Init();
    
    tsLastCharGone = 1;
    tsTxOut = tsTxIn = 0;
    tsTxEmpty = 1;
    
    sprintf(str, "z\r");
    sendCommand(str);
    
    send_macro(display_temperature);
    
    while(1)
    {
        //scanUserInput();                                                        // Detect a string input from the touch screen
        
        #define change_state(state) \
            current_page = state;   \
            state_changed = 1


        switch(current_page) 
        {
            case (PAGE_SETTINGS):
            {
                if (state_changed) {
                    state_changed = 0;
                    display_text("000000", "FFFFFF", 8, "set!", 240, 110);
                }

                if ('1' == userCommand[1] && '3' == userCommand[2] && '1' == userCommand[3]) {
                    change_state(PAGE_SETTINGS);
                }
                else if ('1' == userCommand[1] && '2' == userCommand[2] && '8' == userCommand[3]) {
                    change_state(PAGE_MAIN);
                }
                else if ('1' == userCommand[1] && '3' == userCommand[2] && '2' == userCommand[3]) {
                    change_state(PAGE_SERVICE);
                }
                else {
                    // NOOP
                }
                break;
            }
            case (PAGE_CONFIG):
            {
                if (state_changed) {
                    state_changed = 0;
                    display_text("000000", "FFFFFF", 8, "cfg!", 240, 110);
                }

                if ('1' == userCommand[1] && '3' == userCommand[2] && '1' == userCommand[3]) {
                    change_state(PAGE_SETTINGS);
                }
                else if ('1' == userCommand[1] && '2' == userCommand[2] && '8' == userCommand[3]) {
                    change_state(PAGE_MAIN);
                }
                else if ('1' == userCommand[1] && '3' == userCommand[2] && '2' == userCommand[3]) {
                    change_state(PAGE_SERVICE);
                }
                else {
                    // NOOP
                }
                break;
            }
            case (PAGE_SERVICE):
            {
                if (state_changed) {
                    state_changed = 0;
                    display_text("000000", "FFFFFF", 8, "svc!", 240, 110);
                }
								
								
							  for (k = 0; k < 4 ; k++){
									if (passcode[k][0] == userCommand[1] && passcode[k][1] == userCommand[2] && passcode[k][2] == userCommand[3]) {
									tsCommandReceived  = 0;
										if (k == 3)
											display_text("000000","FFFFFF",8,"OK!", 240,200);
									while(tsCommandReceived == 0);
								 }
									else{
										continue;
									}
							 }								
                if ('1' == userCommand[1] && '3' == userCommand[2] && '1' == userCommand[3]) {
                    change_state(PAGE_SETTINGS);
                }
                else if ('1' == userCommand[1] && '2' == userCommand[2] && '8' == userCommand[3]) {
                    change_state(PAGE_MAIN);
                }
                else if ('1' == userCommand[1] && '3' == userCommand[2] && '2' == userCommand[3]) {
                    change_state(PAGE_SERVICE);
                }
                else {
                    // NOOP
                }
                break;
            }
            default:            // no break
            case (PAGE_MAIN) :
            {
                roomTemp = readOneByteFromSlave(ROOM_TEMP);

                if (state_changed) {
                    state_changed = 0;
                    sprintf(str, "%-3buC", roomTemp);
                    display_text("000000", "FFFFFF", 8, str, 240, 110);
                }
                
                if (tsCommandReceived || roomTemp != prev_temp) 
                {
                    prev_temp = roomTemp;
                    
                    if ('1' == userCommand[1] && '2' == userCommand[2] && '9' == userCommand[3]) {
                        display_celsius = 1;
                        sprintf(str, "%-3buC", roomTemp);
                        display_text("000000", "FFFFFF", 8, str, 240, 110);
                    }
                    else if ('1' == userCommand[1] && '3' == userCommand[2] && '0' == userCommand[3]) {
                        display_celsius = 0;
                        roomTemp = (roomTemp * 9) / 5 + 32;
                        sprintf(str, "%-3buF", roomTemp);
                        display_text("000000", "FFFFFF", 8, str, 240, 110);
                    }
                    else if ('1' == userCommand[1] && '3' == userCommand[2] && '1' == userCommand[3]) {
                        change_state(PAGE_SETTINGS);
                    }
                    else if ('1' == userCommand[1] && '2' == userCommand[2] && '8' == userCommand[3]) {
                        change_state(PAGE_MAIN);
                    }
                    else if ('1' == userCommand[1] && '3' == userCommand[2] && '2' == userCommand[3]) {
                        change_state(PAGE_SERVICE);
                    }
                    else {
                        // Noop
                    }
                }
                break;
            }
        }
    }
}