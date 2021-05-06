/* 
 * File:   part3_4.c
 * Author: Jose
 *
 * Created on May 5, 2021, 11:47 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "CAPTOUCH.h"
#include "timers.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    TIMERS_Init();
    CAPTOUCH_Init();
    
    unsigned int t;
    
    while(1){
        t = TIMERS_GetMilliSeconds();
        
        if(CAPTOUCH_IsTouched() == TRUE){
            printf("Sensor is touched\n");
        }else{
            printf("Sensor is not touched\n");
        }
        
        while((t%1000) != 0){
            t = TIMERS_GetMilliSeconds();
        }
    }
    
    while (1);
}

