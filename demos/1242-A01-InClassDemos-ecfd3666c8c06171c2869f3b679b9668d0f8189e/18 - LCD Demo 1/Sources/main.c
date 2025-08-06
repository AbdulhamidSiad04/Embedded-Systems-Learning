/********************************************************************/
// HC12 Program:  LCD Demo 1
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This program will initialize the LCD and then print 
//                the letter 'T' to it repeatedly               
// Date:          02 DEC 24
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
#include "rti.h"
#include "clock.h"

/********************************************************************/
//Defines
/********************************************************************/
#define LCD_DELAY {char __x = 10; while (--__x);}; //E must be held high for 230ns (Datasheet Pg 52 & 58)

/*** Notice the use of bitfields instead of bit masks when changing the Port K values ***/
#define LCD_READ PORTK_PK1 = 1;DDRH = 0x00; //To read data R/W must be high, port H hust be input
#define LCD_WRITE PORTK_PK1 = 0;DDRH = 0xFF; //To write data R/W must be low, port H must be output
#define LCD_INST PORTK_PK2 = 0; //RS low signals an instruction byte is being sent
#define LCD_DATA PORTK_PK2 = 1; //RS high means data byte is being sent

#define LCD_E_UP PORTK_PK0 = 1; //Set the LCD enable pin high
#define LCD_E_DOWN PORTK_PK0 = 0; //Set the LCD enable pin low


#define LCD_LATCH {LCD_E_UP LCD_DELAY LCD_E_DOWN}; //To latch data into the LCD, set the enable pin high
                                                   //wait a few hundred nanoseconds, then set enable pin low

/********************************************************************/
// Local Prototypes
/********************************************************************/
char lcd_busy(void); // Function to check the busy flag on the LCD controller

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
  EnableInterrupts;
  
/********************************************************************/
  // one-time initializations
/********************************************************************/
Clock_Set20MHZ(); //Set the clock to 20 MHz
RTI_Init();       //Set up the rti for 1 ms interrupts

/***** Initialize the LCD *****/
DDRK |= 0x07; //Bits 0 - 2 of port K must be outputs; they are the LCD control lines
RTI_Delay_ms(50); //Wait at least 45ms after power up (Datasheet pg. 45)
LCD_INST          //Set RS for instructions
LCD_WRITE         //Set R/W and Port H for writing to LCD controller
PTH = 0b00111000; //Put data on Port H (Datasheet pg. 45, 24, 27)
LCD_LATCH         //Latch data into LCD controller
RTI_Delay_ms(5);  //Wait at least 4.1 ms (Datasheet pg. 45)
LCD_LATCH         //Latch in the same data as before
RTI_Delay_ms(1);  //Wait at least 100us (Datasheet pg. 45)
LCD_LATCH         //Lath in the same data as before
/// The LCD busy flag can now be read rather than using delays (Datasheet pg. 45) ///
while(lcd_busy()); //Wait until LCD is not busy
LCD_WRITE         //Set up port H and R/W for writing
PTH = 0b00001111; // Turn on Display, Show Cursor, Blink Cursor (Datasheet pg. 24, 26)
LCD_LATCH         //Latch Data
while(lcd_busy()); //Wait until LCD is not busy
LCD_WRITE         //Set up pins for writing to LCD
PTH = 0b00000001; //Clear the LCD (datasheet pg. 24)
LCD_LATCH         
while(lcd_busy());
LCD_WRITE
PTH = 0b00000110; //Cursor increments after writes, don't shift display (Datasheet pg. 24, 26)
LCD_LATCH
while(lcd_busy()); //Wait until LCD is not busy




/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    RTI_Delay_ms(500);  //500ms blocking delay
    PTH = 'T';          //Put ASCII value for 'T' on port H
    LCD_WRITE           //Set up pins for writing to LCD
    LCD_DATA            //Set RS for data byte
    LCD_LATCH           //Latch the data into the LCD controller

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
char lcd_busy(void){
  char busy;  //Character to store the busy flag
  LCD_INST    //Set PS for instruction (Datasheet pg. 24)
  LCD_READ    //Set up micro to read from LCD controller
  LCD_LATCH   //Latch the read command
  busy = !!(PTH & PTH_PTH7_MASK); //Busy flag is in data bit 7 (Datasheet pg. 24, 29)
  return busy;  //Return 1 if LCD is busy, 0 if it is ready for more instructions

}
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