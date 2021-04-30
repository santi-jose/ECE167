/* 
 * File:   part2_1.c
 * Author: Jose
 *
 * Created on April 28, 2021, 7:26 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "Oled.h"
#include "PING.h"
#include "timers.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    //OledInit();
    PING_Init();
    TIMERS_Init();
    
    unsigned int tf; //time of flight variable
    unsigned int d; //distance variable
    //char oled_str[100]; //oled_str
    unsigned int t; //time variable
    
    while(1){
        //OledClear(OLED_COLOR_BLACK);
        t = TIMERS_GetMilliSeconds();
        
        //get time of flight and distance according to PING sensor
        tf = PING_GetTimeofFlight();
        d = PING_GetDistance();
        
        printf("ToF: %d us\nD: %d mm\n", tf, d);
        
        /*
        sprintf(oled_str, 
                "ToF: %d us\n"
                "D: %d mm\n"
                , tf, d);
        OledDrawString(oled_str);
        OledUpdate();
        */
        
        //wait a microsecond
        while((t%250) != 0){
            t = TIMERS_GetMilliSeconds();
        }
    }
    
    while(1);
}

