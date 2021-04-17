/* 
 * File:   part1_3.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on April 15, 2021, 3:53 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "AD.h"
#include "BOARD.h"
#include "Oled.h"
#include "timers.h"
#include "ToneGeneration.h"

/*
 * 
 */
int main(void)
{
    BOARD_Init(); //initialize BOARD
    AD_Init(); //initialize AD
    TIMERS_Init(); //initialize TIMERS
    ToneGeneration_Init(); //initialize ToneGeneration
    OledInit(); //initialize OLED
    
    AD_AddPins(AD_A1); //initialize UNO32 pin A1
    ToneGeneration_ToneOn(); //turn tone generation on
    
    unsigned int t; //time variable
    int f; //flex sensor reading variable
    int d; //degree conversion variable
    int h; //variable to store Hz to generate tone
    int h_buff[100] = {0}; //hz buffer
    int h_i = 0; //h_buff index
    int avg; //variable to store h_buff average into
    int p_avg; //variable to store previous average
    char oled_str[100]; //oled str to print to oled
    
    while(1){
        OledClear(OLED_COLOR_BLACK); //clear oled
        t = TIMERS_GetMilliSeconds(); //store current time
        
        //check flex sensor reading
        if(AD_IsNewDataReady() == TRUE){
            f = AD_ReadADPin(AD_A1); //store flex sensor reading
        }
        
        //convert flex sensor readings
        d = (-0.434*f) + 437; //store degree conversion
        h = ((d*999)/180) + 1; //convert from degrees to Hz
        
        //software filtering
        h_buff[h_i%100] = h; //store current hz into h_buff
        h_i++; //increment h_buff index
        int i; //define index to loop through h_buff
        for(i=0; i<100; i++){ //calculate average
            avg = avg + h_buff[i];
        }
        avg = avg/100;
        if(avg > 1000){ //hz ceiling
            avg = 1000;
        }
        if(avg < 1){ //hz floor
            avg = 1;
        }
        
        //more software filtering
        if((abs(avg - p_avg)) < 5){
            avg = p_avg;
        }
        p_avg = avg;
        
        ToneGeneration_SetFrequency(avg); //set tone frequency
        
        //Print notification stuff
        sprintf(oled_str,
                "flex sensor: %d\n"
                "degrees: %d\n"
                "Hz: %d\n"
                "%d seconds\n",
                f, d, avg, t/1000);
        OledDrawString(oled_str); //draw oled_str on oled
        OledUpdate(); //update oled
        
        //wait 25 milliseconds
        while(t%25 != 0){
            t = TIMERS_GetMilliSeconds();
        }
    }
    

    while(1); //never leave main in embedded
}

