/* 
 * File:   part3_2.c
 * Author: Jose (jlantia@ucsc.edu)
 *
 * Created on May 4, 2021, 9:59 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "AD.h"
#include "BOARD.h"
#include "timers.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    AD_Init();
    TIMERS_Init();
    
    //add AD pin to read from touch sensor
    AD_AddPins(AD_A1);
    
    //variable to store touch sensor input into
    int s;
    unsigned int t;
    
    while(1){
        t = TIMERS_GetMilliSeconds();
        
        if(AD_IsNewDataReady() == TRUE){
            s = AD_ReadADPin(AD_A1);
        }
        printf("Touch sensor value: %d\n", s);
        
        if(s > 10){
            printf("Touch sensor has been touched!\n");
        }
        
        while((t%1000) != 0){ //wait a second
            t = TIMERS_GetMilliSeconds();
        }
    }

}

