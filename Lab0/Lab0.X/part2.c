/* 
 * File:   part2.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on April 1, 2021, 4:37 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "AD.h"
#include "Ascii.h"
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"

/*
 * 
 */
int main(void)
{
    BOARD_Init(); //init BOARD
    OledInit(); //init Oled
    AD_Init(); //init AD
    
    AD_AddPins(AD_A0); //add pin A0, potentiometer
    int p = 0; //initialize potentiometer value
    char print_statement[100];
    
    while(1){ //infinite loop
        OledClear(OLED_COLOR_BLACK);
        if(AD_IsNewDataReady() == TRUE){ //if new data is ready
            p = AD_ReadADPin(AD_A0);
        }
        sprintf(print_statement, "Potentiometer value: %d",p);
        OledDrawString(print_statement);
        OledUpdate();
    }
    
    
    while(1); //never exit in embedded
}
