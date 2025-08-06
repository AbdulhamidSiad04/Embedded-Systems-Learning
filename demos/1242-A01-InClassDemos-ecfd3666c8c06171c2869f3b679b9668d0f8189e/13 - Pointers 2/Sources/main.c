/********************************************************************/
// HC12 Program:  Pointer Demo - Demonstration of Pointer Overflows
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This program will use a pointer overflow to overwrite
//                one string from the address of another. Eventually it will
//                start overwriting unknown memory addresses               
// Date:          November 12, 2024
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

#include "clock.h"
#include "rti.h"
#include "sw_led.h"
#include "sci.h"

/********************************************************************/
//Defines
/********************************************************************/

/********************************************************************/
// Local Prototypes
/********************************************************************/
void ptr_overflow(char *); //A bad function that will cause a pointer overflow

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
 // EnableInterrupts;
  
/********************************************************************/
  // one-time initializations
/********************************************************************/
Clock_Set20MHZ();
SWL_Init();
sci0_Init(115200, 0);

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    static SwState any_button;
    static char string1[] = "This is string 1\n\r"; //Create two distrinct strings
    static char string2[] = "This is string 2\n\r";
    
    Sw_Process(&any_button, SWL_ANY);
    
    if(any_button == Pressed){
      sci0_txStr(string1); //Print both of them on the terminal when any putton is pressed
      sci0_txStr(string2);
    }
    
    if(any_button == Released)
      ptr_overflow(string1); //Demonstrate a pointer overflow when button is released
    
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

// This function will overwrite the wrong string via pointer overflows
//The address of string1 was passed in, but the pointer is immediately incremented
//into the address space of string 2. Eventually it will overwrite the integer i.
//At that point, we will start overwriting memory that is very far removed
//from the strings. This is a dangerous state which can cause unpredictable
//faults in your programs. 
void ptr_overflow(char * string_ptr){
  static unsigned int i = 19; //This will push the pointer past the end of the string
                              //Be careful not to do this in your code!
  string_ptr += i++; //increment i for the next time this function runs
  *string_ptr = '~';//Overwrite some memory address with the '~' character
}
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