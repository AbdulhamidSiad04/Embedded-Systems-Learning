/********************************************************************/
// HC12 Program:  7 Seg Demo 1 - Demonstration of basic 7-segment display operations
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz
// Author:        James Chaulk
// Details:       This program provides several examples of basic operations with
//                the 7-segment displays. The running example can be selected by commenting
//                and uncommenting the relevent define statements               
// Date:          17 Nov 2024
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



/********************************************************************/
//Defines
/********************************************************************/

/*****   #ifdef defines   *****/
#define HEX_COUNT
//#define LONG_COUNT
//#define CUSTOM_SEGMENTS
//#define CODE_B_DECODE

/*****    Macros for 7 segment displays    *****/
//For example, when the compiler runs it will replace every instance of SEGS_LATCH
//with PORTA &= ~0x01; PORTA |= 0x01;
#define SEGS_LATCH PORTA &= ~0x01; PORTA |= 0x01;
#define SEGS_ML PORTA &=~0x02;
#define SEGS_MH PORTA |= 0x02;
/********************************************************************/
// Local Prototypes
/********************************************************************/

void seg_send(int address, int value); //This helper function will put 'value' on the 7-seg display at 'address'

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
Clock_Set20MHZ();
SWL_Init();
RTI_Init();

///////Set up the 7 Segment Display

PORTA |= 0x03; //The two LSB on port A are set high to avoid sending values when changing the pins to outputs
DDRA |= 0x03;  //Set PORTA pin 0 and pin 1 as outputs
PORTB = 0;     //Preset port b to low before making an output
DDRB = 0xFF;  //Make all of port B outputs




/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)
  {
    // This code will create a single digit counter
    #ifdef HEX_COUNT
    static unsigned char value = 0;
    unsigned char address = 0;

    PORTB = 0b01011000; //Set up the device 
  /*          ||||||||_____Address
              |||||________Bank A
              ||||_________No Shutdown
              |||__________Decode  
              ||___________Decode as HEX
              |____________Single Data Byte
    */

    //Set the address in the controller
    PORTB |= (address & 0b111); //Mask to the three lsb of address and set those bit in port b
    SEGS_MH //Set control mode high for control byte
    SEGS_LATCH //Latch the settings in the ICM7218

    //Send the value to display to the controller
    PORTB = value++; //Put value on port b and increment the variable afterwards
    PORTB |= 0x80; //Turn off the decimal point (ICM7218 Datasheet pg 8)
    SEGS_ML //Set Data Mode
    SEGS_LATCH //Latch the data in the ICM7218

    RTI_Delay_ms(500); //Delay 500ms before updating the display.

    #endif
    
    //This code will create a multi-digit counter that spans across all of the 7-segment displays
    #ifdef LONG_COUNT
    int i = 0; //Counter used in for loop
    static unsigned long value = 0; //Long counter to display on all 8 7-seg displays
    unsigned long value_conv = value; //Variable used to convert the value counter to individual digits

    for (i=0; i <= 8; i++){ //For each of the 8 display digits
      unsigned char digit; //Variable to hold the least significant digit of value_conv
      digit = value_conv %16; //This will result in the least significant hex digit of value_conv
      seg_send(7-i, digit);   //Display 'digit' on 7-i position of the 7-segment displays
      value_conv = value_conv >> 4; //Shift value_conv so the second least significant digit is now the least significant
    }
    value++;  //Increment value after it has been displayed
   // RTI_Delay_ms(1);  //The value counter is large enough that we don't need a delay to see it running. 
    #endif

    //This code will light up each of the individual segments in sequence
    #ifdef CUSTOM_SEGMENTS
    static unsigned char address = 0; //Start with the top left digit
    static char segment = 1;          //Start with data byte 0b00000001. See ICM7218 Datasheet pg 5.

    PORTB = 0b01111000; //Set up the device
   /*         ||||||||_____Address
              |||||________Bank A
              ||||_________No Shutdown
              |||__________No Decode  
              ||___________Don't Care (Decode is disabled)
              |____________Single data byte
    */ 
    PORTB |= (address & 0b111); // Set the address 
    SEGS_MH //Set control mode
    SEGS_LATCH  //Toggle write pin to latch the settings
  
    PORTB = segment; //Put the data byte on port B
    PORTB ^= 0x80;  //Decimal point is active low. XOR port B with 0b1000000 to set it on only when data = 0b10000000
    SEGS_ML //Set data mode
    SEGS_LATCH //Toggle write pin to latch the data
    
    if(!segment){ //If segment data == 0
      segment = 1;  //Reset it to 1
      address++;    //Move to the next digit. Address will roll over to 0 when it goes past 7.
     }
     else
      segment <<= 1;  //Otherwise, shift the segment bit by 1
    RTI_Delay_ms(300); //Wait a few milliseconds. 
    #endif

    //This code will use Code B decoding to dispaly "HELP" and "----" on the 7 segment displays
    #ifdef CODE_B_DECODE
    static int i = 0; //integer for counting in for loop
    unsigned char help[] = {0xC, 0xB, 0xD, 0xE}; //array of hex values to display "HELP" (ICM7218 Datasheet pg 8)
    
    
    for(i=0; i<= 3; i++){ //There are 4 values to display
    //Display "HELP" on the upper display with Code B decoding
      PORTB = 0b00011000; //Set up the device
    /*         ||||||||_____Address
                |||||________Bank A
                ||||_________No Shutdown
                |||__________Decode  
                ||___________Code B Decode
                |____________No Data coming
      */ 
      PORTB |= (i&0b111); //Set the address
      SEGS_MH //Set port A bit 1 high for control mode
      SEGS_LATCH  //Toggle port A bit 0 to latch the data in the display driver chip
    
      PORTB = help[i]; //Port B is one of the HELP values
      PORTB |= 0x80;  //Turn off the decimal point
      SEGS_ML           //Set data mode
      SEGS_LATCH        //Latch the data in the driver IC

      //Display "----" On the lower display
      PORTB = 0b00011000; //Set up the device
      PORTB |= ((7-i)&0b111); //Set the address (Starting at the lower right and moving to the lower left)
      SEGS_MH
      SEGS_LATCH  //Latch the settings in the driver IC
    
      PORTB = 10; //In Code B, 0xA = '-'
      PORTB |= 0x80;  //Turn off the decimal point
      SEGS_ML         //Set the mode pin low
      SEGS_LATCH      //Latch the data in the ICM7218 IC. 
    }
    
    
    #endif
 }
}

/********************************************************************/
// Functions
/********************************************************************/
void seg_send(int address, int value){

  PORTB = 0b01011000; //Set up the device
/*          ||||||||_____Address
            |||||________Bank A
            ||||_________No Shutdown
            |||__________Decode  
            ||___________Decode as HEX
            |____________No Data coming
  */ 
  //Set the address in the controller
  PORTB |= (address & 0b111); //Mask to the three lsb
  SEGS_MH //Set control mode high to set address and options
  SEGS_LATCH //Latch the address

  //Send the value to display to the controller
  
  PORTB = value;  
  PORTB |= 0x80; //No Decimal Point
  SEGS_ML //Set Data Mode
  SEGS_LATCH //Latch the data
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