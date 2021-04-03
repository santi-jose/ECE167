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
    int p; //declare potentiometer value variable
    char oled_str[100]; //declare oled_str char array
    
    while(1){ //infinite loop
        OledClear(OLED_COLOR_BLACK); //clear oled
        if(AD_IsNewDataReady() == TRUE){ //if new data is ready
            p = AD_ReadADPin(AD_A0); //read new value from pot
        }
        sprintf(oled_str, "Potentiometer value:\n%d",p); //define oled_str
        OledDrawString(oled_str); //draw oled_str on oled
        OledUpdate(); //update oled
    }
    
    
    while(1); //never exit in embedded
}
