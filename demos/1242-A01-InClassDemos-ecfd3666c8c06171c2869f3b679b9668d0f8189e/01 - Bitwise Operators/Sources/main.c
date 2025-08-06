/********************************************************************/
// HC12 Program:  Bitwise Operators
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        James Chaulk
// Details:       This program provides several examples of logical and bitwise operators
// Date:          21JAN25
// Revision History :
//  each revision will have a date + desc. of changes
//  21JAN25 - Initial commit


/********************************************************************/
// Library includes
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

//Other system includes or your includes go here
//#include <stdlib.h>
//#include <stdio.h>


/********************************************************************/
//Defines
/********************************************************************/
//These defines are used to make code more readable.
// They also make it easier to maintain and modify code. 
// Anywhere that the compiler sees the text that comes after #define, 
// it will replace it with the value to the right of the text. 
// For example, anywhere that RED_LED appears in my code, it will be replaced 
// with 0b10000000. 
//This also means that if I want to change the value of RED_LED, I only need to
// update the define rather than updating every spot that I would have used 0b10000000
//These defines are all 'bitmasks' for the LEDs and one button
#define RED_LED 0b10000000
#define YELLOW_LED 0b01000000
#define GREEN_LED 0b00100000
#define UP_BUTTON 0b00010000

//These #defines don't have any values associated with them.
// Instead, they are used to comment out blocks of code with 
// #ifdef and #endif statements. There are several examples in the code below. 
// #define LOGICAL
// #define BITWISE
// #define BITSHIFT
// #define BITMASKING
#define OVERFLOW
/********************************************************************/
// Local Prototypes
/********************************************************************/
int add_numbers(unsigned int, unsigned int); //Functions need to be prototyped in C

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
  PT1AD1 &= ~(RED_LED|YELLOW_LED|GREEN_LED); //This statement will turn off all of the LEDs.
                                            // The value inside the brackets will resolve to 0b11100000
                                            // The ~ will invert that to 0b00011111. The bitwise & will then
                                            // clear the first 3 bits of PT1AD1 while leaving the rest of the register alone.

  DDR01AD1 = (RED_LED|YELLOW_LED|GREEN_LED); //This will set all of the LED pins to be outputs
  ATD1DIEN = ~(RED_LED|YELLOW_LED|GREEN_LED); //This will enable digital inputs for the pushbuttons 

/********************************************************************/
  // main program loop
/********************************************************************/

  for (;;)  //This is the main "super-loop" that prevents your code from reaching the end of main()
  {
    unsigned char button = 0;
    static unsigned int counter = 0; //Static means this variable will retain it's value between loop iterations
    volatile unsigned int counter2 = 0;  //This variable is initalized to 0 at the begining of each loop iteration
    volatile int counter3 = 0; //The volatile keyword tells the compiler not to optimize this variable away. It will come up later when
                                // we discuss interrupts. 

  //This code block will demonstrate using logical statements in C. It will only run if the define for LOGICAL exists. 
  // It will only run if "#define LOGICAL" has been added to the Defines section of the code.
  #ifdef LOGICAL
    counter++; //Increment counter. Because it is no longer 0, C will evaluate it as TRUE in logical statements
    //This statement is false because to and both have to be true 
    //for it to be true, so ++counter is true but counter2 is = 0  so false
    if (++counter && counter2){ //Counter is not 0, but counter2 is. This will evaluate to if(TRUE && FALSE).
      PT1AD1 |= RED_LED;  //If this statement is true, the red LED bit will be set. 
    }

    //This statement is true beause only one has to be true to or it and ++counter is true.
    if (++counter || counter2){ //This statement will evaluate to if(TRUE || FALSE).
      PT1AD1 |= YELLOW_LED; //If the statement was true, the yellow LED will be set
    }

    if (++counter && !counter2){ //Counter is not 0, but counter2 is. This will evaluate to if(TRUE && !FALSE).
                                // That means that the LED should now turn on. 
      PT1AD1 |= RED_LED;  //If this statement is true, the red LED bit will be set. 
    }

    if (!(++counter || counter2)){ //This statement will evaluate to if !(TRUE || FALSE). It should be false.
      PT1AD1 &= ~YELLOW_LED; //If the statement was true, the yellow LED bit will be cleared
    }
  #endif

  //This code block shows several examples using bitwise operators to SET, CLEAR, TOGGLE, and INVERT bits. 
  #ifdef BITWISE

    //Bits can be set with the | (bitwise or) operator
    PT1AD1 |= RED_LED|GREEN_LED|YELLOW_LED; //This will set all three of the LED bits.
    
    //Bits can be cleared with the & (bitwise &) operator
    PT1AD1 &= 0b10111111; //This will clear the yellow LED bit while leaving all of the others set
    //These two are same way to turn yellow off.
    PT1AD1 &= ~YELLOW_LED;

    //Bits can be toggled with the ^ (bitwise xor) operator
    PT1AD1 ^= GREEN_LED; //This will toggle the green LED on and off
    PT1AD1 ^= GREEN_LED;
    PT1AD1 ^= GREEN_LED;

    //Bits can be inverted with the ~ (bitwise not) operator
    PT1AD1 &= ~RED_LED; //To clear the red LED, we must & PT1AD1 with 0b01111111
                        // RED_LED will be replaced with 0b10000000
                        // So, ~RED_LED is 0b01111111

    PT1AD1 = 0x00;  //0x can be used to enter hexadecimal values. This will set PT1AD1 to 0b00000000
    PT1AD1 |= RED_LED||GREEN_LED||YELLOW_LED; //This will not do what we want. The right hand of the equals sign is using 
                                              // logical operators. So this will evaluate to PT1AD1 |= 0b00000001;
  #endif

  //This code block provides several examples of using bitshift operators. It also gives an example of the C operator precedence.
  #ifdef BITSHIFT
  //Shifting unsigned values.
  //When shifting unsigned values, 0s are shifted in for both left and right shifts
  counter = counter << 5;
  counter >>= 3;
  
  //When working with signed values, 0s will be shifted in on a left shift.
  // but right shifts will preserve the sign be shifting in whatever the MSB is. 
  counter3 = -500;
  counter3 <<= 5;
  counter3 >>= 3;
  counter3 = 500;
  counter3 <<= 2;
  counter3 >>= 3;

    //You should be aware of the C Operator Precedence (order of operations). Try to predict what the value of PT1AD1 will be
    // after the next four lines have executed. Then consider how much easier it would have been if brackets had been used to 
    // control the order of operator execution. 
    counter = 5000;
    counter2 = 100;
    counter3 = -1000;
    counter = counter*counter2<<3+counter2|counter3>>5&counter*counter2; //You should be aware of the C Operator Precedence table. 
                                                                          // But use () to make complex statements more clear. 
  #endif

  //This code block provides an example of how to mask a single bit when reading a variable or register.
  // We will be using this technique to read button states.
  #ifdef BITMASKING
    //The five least significant bits of PT1AD1 show the current button states. 
    // Set = Pressed. Cleared = Not Pressed.
    // A bitmask for the Up Button has been provided for you.
    
    button = PT1AD1; //This will store the entire value of PT1AD1 in the button variable. 
                    // Because C treats any value other than 0 as true, we can't check if a button has been pressed.
    
    PT1AD1 |= RED_LED; //Turn on the red LED
    button = PT1AD1; //Try to read a button state without masking
    if(button){
      PT1AD1 |= YELLOW_LED; //The yellow LED will turn on because a bit of PT1AD1 was set to turn on the red LED
    }

    button = PT1AD1 & UP_BUTTON; //This will do a bitwise AND with PT1AD1 and 0b00010000;
                                  // If the up button isn't pressed, the value will be 0
    if(button){
      PT1AD1 |= GREEN_LED;  //Now the Green LED will only turn on if the Up Button is pressed
    }
    //Without the else if up is pressed it will just stay on.
    else                    //What would happen if this else statement were removed?
      PT1AD1 &= ~GREEN_LED;

  #endif

  //This code block provides several examples of overflowing variables. You will need to be more mindful of the width of your variables than you were
  // in C#
  #ifdef OVERFLOW
    counter = add_numbers(35000, 35000);  //Why isn't this 70000?
    counter3 = add_numbers(20000, 20000); //Why isn't this 40000?
    _asm("nop");    //This is a no-operation command. It is just here to make it easier to see the value of counter3 at the end of the previous line. 
  #endif
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
int add_numbers(unsigned int a, unsigned int b){
  return a+b;
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
