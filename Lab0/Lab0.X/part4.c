/* 
 * File:   part4.c
 * Author: Jose
 *
 * Created on April 3, 2021, 7:04 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "AD.h"
#include "Ascii.h"
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"
#include "ToneGeneration.h"

/*
 * 
 */
int main(void)
{
    BOARD_Init(); //init BOARD
    OledInit(); //init Oled
    AD_Init(); //init AD
    ToneGeneration_Init(); //init ToneGeneration
    
    AD_AddPins(AD_A0); //add pin A0, potentiometer
    ToneGeneration_ToneOn(); //turn ToneGeneration on
    int p; //declare potentiometer value variable
    int f; //initialize frequency variable
    int f_buff[100] = {0}; //initialize frequency buffer
    int i_f = 0; //initialize frequency buffer index
    int avg = 0; //initialize avg variable
    char oled_str[100]; //declare oled_str char array
    
    while(1){ //infinite loop
        OledClear(OLED_COLOR_BLACK); //clear oled
        
        //check POT value
        if(AD_IsNewDataReady() == TRUE){ //if new data is ready
            p = AD_ReadADPin(AD_A0); //read new value from pot
            f = ((p*999)/1023) + 1; //convert potentiometer reading to frequency
            ToneGeneration_SetFrequency(avg); //set ToneGeneration frequency
        }
        
        //software filtering 
        f_buff[i_f%100] = f; //store current frequency into buffer
        int i; //declare index for iterating through f_buff
        for(i = 0; i < 100; i++){ //sum of all frequency samples
            avg = avg + f_buff[i];
        }
        avg = avg/100; //average of frequency samples
        if(avg > 1000){ //if avg is greater than 1000
            avg = 1000; //set to avg 1000
        }
        i_f++; //increment f_buff index
        //ToneGeneration_SetFrequency(avg); //set ToneGeneration frequency
        
        //Print frequency to OLED
        sprintf(oled_str, "Frequency: %d",avg); //define oled_str to print f avg
        OledDrawString(oled_str); //draw oled_str on oled
        OledUpdate(); //update oled
    }
    
    while(1); //if embedded we never exit
}

