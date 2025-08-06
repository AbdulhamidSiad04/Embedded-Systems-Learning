/********************************************************************/
// HC12 Program:  SCI Demos
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James C.
// Details:       This program provides several SCI demos
//                Defines can be used to select the demo being used               
// Date:          November 3, 2024
// Revision History :
//  each revision will have a date + desc. of changes
//14 March 25 - Modified RTI ISR to decrement rtiDelayCount. Added tx_flag 
//                to prevent multiple transmissions in one millisecond



/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>

#include <stdio.h>
#include "clock.h"
#include "rti.h"
#include "sw_led.h"

/********************************************************************/
//Defines
/********************************************************************/

#define SIMPLE_TX
//#define SIMPLE_TX_RX
//#define STRING_POINTER
//#define SPRINTF //Must incude stdio.h

/********************************************************************/
// Local Prototypes
/********************************************************************/
void tx_string(char*);  //This function accepts a character pointer
                        // and transmits a null-terminated string.

/********************************************************************/
// Global Variables
/********************************************************************/
volatile int tx_flag = 0; // A flag to indicate that it is time to tx data

/********************************************************************/
// Constants
/********************************************************************/

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
Clock_Set20MHZ();
RTI_Init();
SWL_Init();

/********** Setup the SCI port *********/
SCI0CR1 = 0;

// Enable transmit and receive
SCI0CR2 = SCI0CR2_TE_MASK | SCI0CR2_RE_MASK;

// Set the BAUD rate to 9600
// 20MHz / 16 / 9600 = 130 
SCI0BD = 130;

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    //This code block will transmit the character 'A' every 500ms 
    #ifdef SIMPLE_TX
    RTI_Delay_ms(500);  //500ms blocking delay

    // Check if the transmit data register is empty
    if (SCI0SR1 & SCI0SR1_TDRE_MASK){
      SCI0DRL = 'A'; //Put an A in the SCI0 data register
    }
    #endif

    //This code block will transmit the last button that was pressed every 500ms
    //It does not use a blocking delay. A blocking delay would prevent it from receiving new characters. 
    #ifdef SIMPLE_TX_RX
    static char data = 'A'; //Start with the letter 'A'
    
    //if the Tx register is empty and the tx_flag is true
    if ((SCI0SR1 & SCI0SR1_TDRE_MASK) && tx_flag){
      SCI0DRL = data;   // Send the data
      tx_flag = 0;    //Set the Tx flag to false. This will prevent multiple transmissions in 1 ms.
    }

    // If the Rx register is full and complete
    if (SCI0SR1 & SCI0SR1_RDRF_MASK){
      data = SCI0DRL; //Update the data variable
    }
    #endif

    //This uses pointers to transmit a null-terminated string
    #ifdef STRING_POINTER
    char test_message[] = "This is a test!\n\r"; // Define a string
                                                // The compiler will null-terminate it for us.
    static char data = ' '; //Set a variable to the 'space' character

    //if the Tx register is empty and the tx_flag is true
    if ((SCI0SR1 & SCI0SR1_TDRE_MASK) && tx_flag){
      
      if (data == 'T') //Check to see if the received data was "T"
        tx_string(test_message); //Send the test_message if the received data was 'T'
                                  // tx_string(&message) would also work, but arrays are always passed by reference in C
                                  // That means that arrays are always passed as pointers.

      else //Otherwise, send the data character
        SCI0DRL = data;   // Send the data
      
      tx_flag = 0;    //Set the Tx flag to false. This will prevent multiple transmissions in 1 ms. 
    }

    // If the Rx register is full and complete
    if (SCI0SR1 & SCI0SR1_RDRF_MASK){
      data = SCI0DRL; //Update the data variable
    }
    #endif

    //This code block uses the sprintf() function to send a string that includes a variable
    #ifdef SPRINTF
    char message[50]; //Make an empty character array. 

    // If a new byte of data has arrived
    if (SCI0SR1 & SCI0SR1_RDRF_MASK){
      //Use sprintf to generate a string.
      // The string will be "The The received byte was: %c\r\n"
      // %c will be replaced with whatever data is in SCI0DRL, interpreted as an ascii character
      // the generated string will be stored in the 'message' character array
      sprintf(message, "The received byte was: %c\r\n", SCI0DRL);
      tx_string(message); //Transmit the string that was just created.
                          // tx_string(&message) would also work, but arrays are always passed by reference in C
                          // That means that arrays are always passed as pointers. 
    }
    #endif
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void tx_string(char* message){
  while (*message != 0x00){   //Send characters until the pointer reaches the string null termination
    if (SCI0SR1 & SCI0SR1_TDRE_MASK){ //If the SCI is ready for more data
      SCI0DRL = *message;   //Dereference the pointer and transmit the character it points to
      message++;  //Increment the pointer to the next character in the array
    }
  }
}

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

interrupt VectorNumber_Vrti void Vrti_ISR(void)
{
 CRGFLG = CRGFLG_RTIF_MASK; //clear flag;

 //Adjust counters 
 rtiDelayCount--;   //rtiDelayCount may increment or decrement depending on how you've implemented RTI_Delay_ms()
 rtiMasterCount++;  //rtiMasterCount always increments

 if(!(rtiMasterCount % 500))
  tx_flag = 1;
}