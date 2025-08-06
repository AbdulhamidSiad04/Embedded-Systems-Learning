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
#include <stdio.h>
#include "clock.h"
#include "rti.h"
#include "lcd.h"
#include "sw_led.h"
#include "sci.h"
#include "segs.h"

/********************************************************************/
//Defines
/********************************************************************/
//#define READ_ADDRESS
//#define CHANGE_ADDRESS
#define CURSOR_CONTROL
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
char buffer[50];
volatile char rx_data;
volatile char rx_buffer[50];
volatile char string_rxd = 0;
volatile int rx_index = 0;
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
lcd_Init();
sci0_Init(115200, 1);
lcd_DispControl(1,1);
lcd_StringXY(3,1,"Press Buttons");
lcd_StringXY(3,2,"To Move Cursor");
lcd_Home();
Segs_Init();
/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    static SwState up, down, left, right, center;
    unsigned char cursor_position = 0;
    unsigned int address = 0;
    static char ccode = 0b00100001; //Start at the first valid character code

    #ifdef READ_ADDRESS
    RTI_Delay_ms(750);
    lcd_Data(ccode++);
    //This is the same as the function for checking the busy flag
    //but we're interested in different bits now.
    LCD_INST //Set up PS for instruction
    LCD_READ //Set up RW and port H for reading
    LCD_LATCH //Latch command
    cursor_position = PTH & ~0x80;  //Keep all bits except the busy flag
    sprintf(buffer, "%04d\r\n", cursor_position);
    sci0_txStr(buffer);
    #endif

    #ifdef CHANGE_ADDRESS
    if (string_rxd){
      string_rxd = 0;
      sscanf(rx_buffer, "%d", &address);
      lcd_Ins(address | 0b10000000); //Set the DDRAM address
      lcd_Data(ccode++);
    }
    #endif

    #ifdef CURSOR_CONTROL
    //Update the switch states
    Sw_ProcessD(&up, SWL_UP);
    Sw_ProcessD(&down, SWL_DOWN);
    Sw_ProcessD(&left, SWL_LEFT);
    Sw_ProcessD(&right, SWL_RIGHT);
    Sw_ProcessD(&center, SWL_CTR);

    if (up == Pressed){
      cursor_position = lcd_GetAddr();
      lcd_Addr(cursor_position-20);
    }
    if(down == Pressed){
      cursor_position = lcd_GetAddr();
      lcd_Addr(cursor_position+20);
    }
    if(left == Pressed){
      cursor_position = lcd_GetAddr();
      lcd_Addr(cursor_position-1);
    }
    if(right == Pressed){
      cursor_position = lcd_GetAddr();
      lcd_Addr(cursor_position+1);
    }
    if(center == Pressed){
      lcd_Home();
    }
    #endif
    
    Segs_16D(lcd_GetAddr(), Segs_LineTop); //Display the cursor address on the 7 segment displays

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
 rtiDelayCount++;
 rtiMasterCount++;

}

interrupt VectorNumber_Vsci0 void sci0_ISR(void){
  
  if (SCI0SR1_RDRF){  //If a data byte has been received
   rx_data = SCI0DRL;   // put it in the rxData variable
   if (rx_data == '\r'){
    string_rxd = 1;
    rx_buffer[rx_index] = '\0';
    rx_index = 0;
   }
   else if (rx_index < 50){
    rx_buffer[rx_index++] = rx_data;
   }
   else
    string_rxd = 1;
  }
}