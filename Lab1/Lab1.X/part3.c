/* 
 * File:   part3.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on April 17, 2021, 3:32 PM
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
    
    //Add pins
    AD_AddPins(AD_A1); //add UNO32 pin A1(flex input)
    AD_AddPins(AD_A2); //add UNO32 pin A2(piezo input)
    
    //declare variables
    unsigned int t; //time variable
    unsigned int td = 1000; //variable to store tone duration (default 1000ms)
    unsigned int f; //flex sensor reading variable
    unsigned int p; //piezo sensor reading variable
    int d; //degree variable
    int h; //Hz variable for tone generation
    int h_buff[100] = {0}; //hz buffer
    int h_i = 0; //h_buff index
    int avg; //variable to store h_buff average into
    int p_avg; //variable to store previous average
    char oled_str[100]; //oled_str to print to oled
    
    while(1){
        OledClear(OLED_COLOR_BLACK); //clear oled
        ToneGeneration_ToneOff(); //turn tone off
        t = TIMERS_GetMilliSeconds(); //store current time
        
        //check flex sensor & piezo reading
        if(AD_IsNewDataReady() == TRUE){
            f = AD_ReadADPin(AD_A1); //store flex sensor reading
            p = AD_ReadADPin(AD_A2); //store piezo sensor reading
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
        if((abs(avg - p_avg)) < 5){ //if diff b/w avg and p_avg is less than 5
            avg = p_avg; //keep avg as p_avg
        }
        p_avg = avg; //store current average in p_avg
        
        //check for piezo sensor trigger
        if(p > 100){ //if the piezo sensor analog input is over 100
            ToneGeneration_SetFrequency(avg); //set tone frequency
            ToneGeneration_ToneOn(); //turn tone on
            while(t%td != 0){ //keep tone on for tone duration(td)
                if(AD_IsNewDataReady() == TRUE){ //if we have new AD data
                    p = AD_ReadADPin(AD_A2); //store piezo sensor reading
                    if(p > 100){ //if piezo sensor has been triggered
                        td = TIMERS_GetMilliSeconds() + 1000; //increase td (1s)
                    }
                }
                t = TIMERS_GetMilliSeconds(); //set current time in milliseconds
            }
            td = 1000; //reset td to 1 second (1000ms)
        }
        
        //Print notification stuff
        sprintf(oled_str,"Frequency: %dHz\n", avg);
        OledDrawString(oled_str); //draw oled_str on oled
        OledUpdate(); //update oled
    }
    

    while(1); //never leave main in embedded
}

