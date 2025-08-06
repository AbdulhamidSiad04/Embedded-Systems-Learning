/********************************************************************/
// HC12 Program:  SCI Demo 2 - Demonstrate sscanf and carriage returns
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This program demonstrates how to buffer inputs until 
//                the 'enter' button is pressed. It also introduces using
//                ssprintf() to parse a formatted input.               
// Date:          13 November, 2024
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
#define RX_STRING
//#define SCANF

#define BUFFER_SIZE 100
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/

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
  //EnableInterrupts;
  
/********************************************************************/
  // one-time initializations
/********************************************************************/
SWL_Init();
RTI_Init();
Clock_Set20MHZ();
sci0_Init(115200, 0);

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    int i, x, result;
    unsigned char serial_byte;
    char buffer[BUFFER_SIZE]; //Buffer to store received data
    char output_buffer[BUFFER_SIZE*2]; //Buffer to hold formatted strings before transmission
    static unsigned int buffer_index = 0; //Index to track location in buffers
  
  //This section demonstates how to store data until a user presses enter in the terminal.
  //Then all of the buffered data is acted upon. 
  #ifdef RX_STRING
    if (sci0_rxByte(&serial_byte)){ //If a byte has been received
      if (serial_byte == '\r'){ //If the byte is a carraige return (If someone pressed enter)
        buffer[buffer_index] = 0; //Put a 0 at the end of the string (null terminate it)
        sprintf(output_buffer, "Your string was: %s\n\r", buffer); //Create a formatted string with sprintf()
        sci0_txStr(output_buffer); //Send the output string to the terminal
        buffer_index = 0;  //Reset the buffer index
      }
      else{ //If the byte wasn't a carriage return
        buffer[buffer_index++] = serial_byte; //Append it to the buffer and increment the index
      }
    }
  #endif

  //This section demonstrates how to use ssprintf() to convert a formatted string into 
  //appropriate data types. In this example, ascii numbers are converted into decimal numbers
  #ifdef SCANF
  sci0_txStr("\r\nEnter first number: ");
  i = sci0_bread(); //Blocking read to get the numbers
  sci0_txStr("\r\nEnter second number: ");
  x = sci0_bread();                   
  result = i+x; //Add the values together
  sprintf(output_buffer, "\r\nThe result of %d + %d is %04d.\n\rThe result in hex is %#06X\n\r", i, x, result, result);
  sci0_txStr(output_buffer); // Your results shouldn't make any sense because we are adding ascii characters together
  
  // Now let's try it with scanf
  sci0_txStr("\r\nEnter first number: ");
  buffer[0] = sci0_bread(); //Blocking read into the buffer
  sscanf(buffer, "%d", &i); //run sscanf on the buffer, tell it to read the string as a single decimal value, store the result in i
  sci0_txStr("\r\nEnter second number: ");
  buffer[0] = sci0_bread();
  sscanf(buffer, "%d", &x); //Convert from ascii to decimal again. 
  result = i+x;
  sprintf(output_buffer, "\r\nThe result of %d + %d is %04d.\n\rThe result in hex is %#06X", i, x, result, result);
  sci0_txStr(output_buffer); //We should get the correct output this time
  
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