/* 
 * File:   part1_2.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on April 12, 2021, 3:06 PM
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
    BOARD_Init(); //initialize BOARD
    AD_Init(); //initialize AD
    TIMERS_Init(); //initialize  timer
    
    AD_AddPins(AD_A1); //initialize UNO32 pin A1
    
    unsigned int t; //time variable
    int f; //flex sensor reading variable
    int d; //degree conversion variable
    
    while(1){
        t = TIMERS_GetMilliSeconds(); //store current time
        
        //check flex sensor reading
        if(AD_IsNewDataReady() == TRUE){
            f = AD_ReadADPin(AD_A1); //store flex sensor reading
            d = (-0.434*f) + 437; //store degree conversion
        }
        
        //print notifications
        printf("flex sensor value:\n%d\n",f);
        printf("degrees:\n%d\n",d);
        printf("1 second has passed\n");
        
        //wait a second
        while((t%1000)!=0){
            t = TIMERS_GetMilliSeconds();
        }
    }

    while (1);
}

