/**
 * @file    CAPTOUCH.h
 * @brief   Capacitive touch sensing module
 * @author  CMPE167 Staff
 * @date    1/15/2019
 * @detail  This module uses the input capture peripheral to detect touch inputs.
 *          If you are unfamiliar with input capture please read the lab appendix before beginning.
 *          The peripheral is configured to generate an interrupt every rising edge which means
 *          that the difference of two interrupts gives you the period of the signal.
 *          NOTE: Given the limited number of timers on board this module is incompatible with PWM
 */
  
#include <stdio.h>

#include "CAPTOUCH.h"

#define FALSE ((int8_t) 0)
#define TRUE ((int8_t) 1)

unsigned int t; 
unsigned int c;
unsigned int c1;
unsigned int c2;
unsigned int f;
char touch;

char CAPTOUCH_Init(void)
{
    // following block inits the timer
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;
    PR2 = 0xFFFF;
    T2CONbits.ON = 1;
    
    //this block inits input capture
    IC4CON = 0;
    IC4CONbits.ICTMR = 1;
    IC4CONbits.ICM = 0b010;
    
    IFS0bits.IC4IF = 0;
    IPC4bits.IC4IP = 7;
    IEC0bits.IC4IE = 1;
    IC4CONbits.ON = 1;
    // whatever else you need to do to initialize your module
    t = 0; //set the trigger to zero
    return SUCCESS;
}
           
           /**
 * @function    CAPTOUCH_IsTouched(void)
 * @brief       Returns TRUE if finger is detected. Averaging of previous measurements 
 *              may occur within this function, however you are NOT allowed to do any I/O
 *              inside this function.
 * @return      TRUE or FALSE
 */
char CAPTOUCH_IsTouched(void){
    return touch;
}
  
void __ISR(_INPUT_CAPTURE_4_VECTOR) InputCapture_Handler(void)
{
    IFS0bits.IC4IF = 0;
    // IC4BUF contains the timer value when the rising edge occurred.
    c = IC4BUF; //current IC4BUF count
    
    if(t == 1){
      c1 = c; //count 1, first trigger
      t = 0; //set trigger flag to 0
    }else{
      c2 = c; //count 2, second trigger
      f = c2 - c1; //calculate frequency
      if(f > 100000){ //if f is greater than 100,000
      }else if(f > 1000){
          touch = TRUE;
      }else if(f < 1000){
          touch = FALSE;
      }
      t = 1; //set trigger flag to 1
    }
}

