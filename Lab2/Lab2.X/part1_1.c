/* 
 * File:   part1_1.c
 * Author: Jose
 *
 * Created on April 26, 2021, 2:58 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "BOARD.h"
#include "Oled.h"
#include "QEI.h"
#include "timers.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    QEI_Init();
    TIMERS_Init();
    OledInit();
    
    int q; //QEI count variable
    int t; //time variable
    char oled_str[100]; //oled string
    
    while(1){
        OledClear(OLED_COLOR_BLACK);
        t = TIMERS_GetMilliSeconds(); //read time
        q = QEI_GetPosition(); //read QEI count
        //q++;
        sprintf(oled_str,"QEI_count: %d\n", q);
        
        OledDrawString(oled_str);
        OledUpdate();
        //printf("QEI_count: %d\n", q);
        
        //while((t%1000) != 0){ //wait a second
        //    t = TIMERS_GetMilliSeconds();
        //}
    }
    

    while (1);
}

