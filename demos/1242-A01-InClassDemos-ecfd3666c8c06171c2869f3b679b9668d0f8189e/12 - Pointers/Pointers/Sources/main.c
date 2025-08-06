/********************************************************************/
// HC12 Program:  Pointer Demo - Simple Demonstration of Pointers in C
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This program will demonstrate how pointers work through
//                several function calls               
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
void increment_int(int);
void increment_int_ptr(int *);
void increment_ptr(int *);
/********************************************************************/
// Global Variables
/********************************************************************/
char string_buffer[100];
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
SWL_Init();
RTI_Init();
sci0_Init(115200, 0);

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    SwState any_button;
    static int i = 1; // A normal integer for examining
    int * ptr_i;  // An integer pointer; a pointer that looks at an integer

    ptr_i = &i; // Set the pointer to the address of i

    // Display the starting values of the variables
    sprintf(string_buffer,"The value of i is: %d\n\r", i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The value of ptr_i is: 0x%X\n\r",ptr_i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The dereferenced value of ptr_i is: %d\n\n\r", *ptr_i);
    sci0_txStr(string_buffer);

    while(Sw_ProcessD(&any_button, SWL_ANY) != Pressed); //Hold until button is pressed to proceed
    // Run the increment int function
    sprintf(string_buffer,"Running increment_int()\n\r");
    sci0_txStr(string_buffer);
    increment_int(i);
    
    //Show the variable values again
    sprintf(string_buffer,"The value of i is: %d\n\r", i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The value of ptr_i is: 0x%X\n\r",ptr_i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The dereferenced value of ptr_i is: %d\n\n\r", *ptr_i);
    sci0_txStr(string_buffer);

    while(Sw_ProcessD(&any_button, SWL_ANY) != Pressed); //Hold until button is pressed to proceed
    // Run the increment int pointer function
    sprintf(string_buffer,"Running increment_int_ptr()\n\r");
    sci0_txStr(string_buffer);
    increment_int_ptr(&i);

    //Show the variable values again
    sprintf(string_buffer,"The value of i is: %d\n\r", i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The value of ptr_i is: 0x%X\n\r",ptr_i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The dereferenced value of ptr_i is: %d\n\n\r", *ptr_i);
    sci0_txStr(string_buffer);

    while(Sw_ProcessD(&any_button, SWL_ANY) != Pressed); //Hold until button is pressed to proceed
    // Run the increment ptr function
    sprintf(string_buffer,"Running increment_ptr()\n\r");
    sci0_txStr(string_buffer);
    increment_ptr(&i);
    //Show the variable values again
    sprintf(string_buffer,"The value of i is: %d\n\r", i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The value of ptr_i is: 0x%X\n\r",ptr_i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"The dereferenced value of ptr_i is: %d\n\n\r", *ptr_i);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"*** Restarting Main() Loop ***\n\n\r");
    sci0_txStr(string_buffer);
    
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

// This function will increment an int, but not return it
void increment_int(int x){
    sprintf(string_buffer,"\tInside increment_int:\n\r");
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tThe value of x is: %d\n\r", x);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\t***Increment int value***\n\r");
    sci0_txStr(string_buffer);
    x++; //Increment the integer
    sprintf(string_buffer,"\tThe value of x is: %d\n\r", x);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tReturn to main\n\r");
    sci0_txStr(string_buffer);
}

// This function will change the value at the address in the pointer
void increment_int_ptr(int * ptr){
    sprintf(string_buffer,"\tInside increment_int_ptr:\n\r");
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tThe value of ptr is: 0x%X\n\r", ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tThe dereferenced value of ptr is: %d\n\r", *ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\t***Increment dereferenced pointer value***\n\r");
    sci0_txStr(string_buffer);
    *ptr += 1; //Increment the dereferenced pointer value
    sprintf(string_buffer,"\tThe value of ptr is: 0x%X\n\r",ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tThe dereferenced value of ptr is: %d\n\r", *ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tReturn to main\n\r");
    sci0_txStr(string_buffer);
}

// This function will change the address in the pointer
void increment_ptr(int * ptr){
    sprintf(string_buffer,"\tInside increment_ptr:\n\r");
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tThe value of ptr is: 0x%X\n\r",ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tThe dereferenced value of ptr is: %d\n\r", *ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\t***Increment pointer***\n\r");
    sci0_txStr(string_buffer);
    ptr++; //Increment the pointer value
    sprintf(string_buffer,"\tThe value of ptr is: 0x%X\n\r",ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tThe dereferenced value of ptr is: %d\n\r", *ptr);
    sci0_txStr(string_buffer);
    sprintf(string_buffer,"\tReturn to main\n\r");
    sci0_txStr(string_buffer);
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