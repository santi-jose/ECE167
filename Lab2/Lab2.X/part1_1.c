/* 
 * File:   part1_1.c
 * Author: Jose
 *
 * Created on April 26, 2021, 2:58 AM
 */

#include <proc/p32mx340f512h.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include <xc.h>

#include "BOARD.h"
#include "Oled.h"
#include "QEI.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    QEI_Init();
    OledInit();
    
    int q; //QEI count variable
    int d; //QEI degrees variable
    char oled_str[100]; //oled string
    
    while(1){
        OledClear(OLED_COLOR_BLACK);
        q = QEI_GetPosition(); //read QEI count
        
        //convert QEI position to degrees
        d = abs((q*15)%360);
        sprintf(oled_str,
                "QEI count: %d\n"
                "QEI degrees: %d\n"
                , q, d);
        
        OledDrawString(oled_str);
        OledUpdate();
    }
    

    while (1);
}

