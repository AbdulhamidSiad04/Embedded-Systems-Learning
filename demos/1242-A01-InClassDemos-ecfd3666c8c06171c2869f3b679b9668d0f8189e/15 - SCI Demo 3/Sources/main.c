/********************************************************************/
// HC12 Program:  SCI Demo 3 - Demonstrating memset
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This program demonstartes how to use memset to clear
//                a buffer. SCI data reception in handled by an ISR            
// Date:          November 13, 2024
// Revision History :
//  each revision will have a date + desc. of changes



/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>
#include <stdio.h>

#include "sw_led.h"
#include "clock.h"
#include "rti.h"
#include "sci.h"


/********************************************************************/
//Defines
/********************************************************************/
#define BUFFER_SIZE 50

//#define NO_MEMSET
//#define MEMSET
#define CONST_STRING
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
char start_message[] = "\n\rEnter a string: ";
char tx_buffer[BUFFER_SIZE*2]; // Buffer to store strings for sci transmission

// Volitile variables for sci0_ISR
volatile char rx_data; 
volatile char rx_buffer[BUFFER_SIZE];
volatile int rx_index = 0;
volatile unsigned char string_rxd = 0; // Flag to check if a complete string has been received

/********************************************************************/
// Constants
/********************************************************************/
const char const_msg[] = "CONSTANT MESSAGE\n\r";
/********************************************************************/
// Main Entry
/********************************************************************/
void main(void)
{
  //Any main local variables must be declared here

  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  
/********************************************************************/
  // one-time initializations
/********************************************************************/
SWL_Init();
RTI_Init();
Clock_Set20MHZ();
sci0_Init(115200, 1); //115200 baud, enable RDRF interrupt

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    // In this section, the receive buffer is not cleared between strings
    // This will cause elements of the previous strings to be present when 
    // printing to the terminal
    #ifdef NO_MEMSET
    sci0_txStr(start_message);
    while(!string_rxd); // Blocking delay until the enter button is pressed
    sprintf(tx_buffer, "\n\rYour string was: %s\n\r", rx_buffer); //Create formatted output string
    sci0_txStr(tx_buffer); //Send the output string to the terminal
    rx_index = 0; //Reset the reception buffer index
    string_rxd=0; //Reset the string received flag
    #endif

    //In this section, the receive buffer is cleared with memset() after 
    //sending a string to the terminal. Previous strings cannot exist between
    //loops as the buffer is cleared to 0s after each transmission
    #ifdef MEMSET
    sci0_txStr(start_message);
    while(!string_rxd); //blocking delay until 'enter' is pressed
    sprintf(tx_buffer, "\n\rYour string was: %s\n\r", rx_buffer);
    sci0_txStr(tx_buffer);
    memset(rx_buffer, 0, rx_index); //Set 'rx_index' bytes to 0x00 in 'rx_buffer'
    rx_index = 0;
    string_rxd = 0;               
    #endif

    //This section will attempt to use memset() to modify a const variable.
    //You will not be able to modify the const. Look at the memory address of the
    //const and check to see where it is in the memory map within the microcontroller
    //datasheet. You should find that the constant variable is stored in flash memory, which 
    //can't normally be written to.
    #ifdef CONST_STRING
    SwState any_button; 
    static int mem_index = 0; //static variable so mem_index isn't reset every loop
    Sw_ProcessD(&any_button, SWL_ANY); //Update the button state
    if (any_button == Pressed){  //if a button has been pressed
      sci0_txStr(const_msg);  //send constant string to the terminal
      memset(const_msg, '*', mem_index); //try to overwrite the const string
      mem_index++; //increment the index counter
    }
    #endif

 }
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vrti void Vrti_ISR(void)
{
 CRGFLG = CRGFLG_RTIF_MASK; //clear flag;

 // Increment counters 
 rtiDelayCount--;
 rtiMasterCount++;

}

interrupt VectorNumber_Vsci0 void sci0_ISR(void){
  
  if (SCI0SR1_RDRF){  //Interrupt flag is reset by reading SCI0SR1 (11.5.3.1.3)
   rx_data = SCI0DRL;   //followed by reading the SCI data register (11.5.3.1.3)
   if (rx_data == '\r'){
    string_rxd = 1; //set the srting received high if a carraige return is received
   }
   else if (rx_index < BUFFER_SIZE){ 
    rx_buffer[rx_index++] = rx_data; //Otherwise, add the character to rx_buffer
   }
   else
    string_rxd = 1; //set the string received flag high if buffer is full
  }
}