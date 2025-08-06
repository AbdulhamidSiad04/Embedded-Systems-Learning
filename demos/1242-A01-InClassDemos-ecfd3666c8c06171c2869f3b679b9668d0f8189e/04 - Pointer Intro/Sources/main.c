/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     MHz
// Author:        This B. You
// Details:       A more detailed explanation of the program is entered here               
// Date:          Date Created
// Revision History :
//  each revision will have a date + desc. of changes



/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>
//#include <stdio.h>
 #include "sw_led.h"


/********************************************************************/
//Defines
/********************************************************************/
  
/********************************************************************/
// Global Variables
/********************************************************************/
 typedef struct box_struct
  {
    unsigned long length;
    unsigned long width;
    unsigned long height;
    unsigned long volume;
    unsigned long colour;
    char name[50];
  }Box;
 Box box1;
/********************************************************************/
// Local Prototypes
/********************************************************************/
void update_volume(Box*);
void initialize_box(Box*);
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
initialize_box(&box1);
/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    
    static SwState up, down, left, right, center;

    Sw_Process(&up, SWL_UP);
    Sw_Process(&down, SWL_DOWN);
    Sw_Process(&left, SWL_LEFT);
    Sw_Process(&right, SWL_RIGHT);
    Sw_Process(&center, SWL_CTR);

    if ( Sw_Process(&up, SWL_UP) == Pressed){
      box1.height += 10;
    }

    if (down == Pressed && box1.height > 10){
      box1.height -= 10;
    }

    if (left == Pressed && box1.width > 10){
      box1.width -= 10;
    }

    if (right == Pressed){
      box1.width += 10;
    }

    if (center == Pressed){
      update_volume(&box1);
    }

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void update_volume(Box* box){
  box->volume = box->length * box->width * box->height;
  
}

void initialize_box(Box* box){
  box->height = 10;
  box->width = 10;
  box->length = 10;
  box->colour = 0x8B14CA;
  
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
