/* 
 * File:   part6.c
 * Author: Jose
 *
 * Created on April 4, 2021, 5:36 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "AD.h"
#include "Ascii.h"
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"
#include "ToneGeneration.h"

#define TONE1 1
#define TONE2 98
#define TONE3 245
#define TONE4 464

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init(); //init BOARD
    OledInit(); //init Oled
    AD_Init(); //init AD
    ToneGeneration_Init(); //init ToneGeneration
    
    //set up potentiometer
    AD_AddPins(AD_A0);
    
    //variable declarations
    char oled_str[100]; //oled_str
    int btn_press = 0; //boolean to check if button is pressed
    int p; //variable to store potentiometer readings
    int f_offset; //variable to store offset based on pot reading
    int f_buff[100]; //f_buff to store frequency offset values
    int i_f = 0; //f_buff index
    int avg = 0; //avg variable to store avg of f_offset values
    
    while(1){ //infinite loop
        OledClear(OLED_COLOR_BLACK); //clear OLED
        
        //read potentiometer values
        if(AD_IsNewDataReady() == TRUE){ //if new data is ready
            p = AD_ReadADPin(AD_A0); //read new value from pot
            f_offset = ((p*536)/1023); //convert pot read to offset
            //ToneGeneration_SetFrequency(avg); //set ToneGeneration frequency
        }
        
        //software filtering 
        f_buff[i_f%100] = f_offset; //store current frequency into buffer
        int i; //declare index for iterating through f_buff
        for(i = 0; i < 100; i++){ //iterate through f_buff
            avg = avg + f_buff[i]; //add all values in f_buff
        }
        avg = avg/100; //average of f_buff samples
        if(avg > 536){ //if avg is greater than 536
            avg = 536; //set avg to 536
        }
        i_f++; //increment f_buff index
        
        //Button pressing logic
        if(BTN1 == 1){ //if BTN1 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE1 + avg); //set frequency to 196
                sprintf(oled_str,"BTN1 pressed\nTone: %d", TONE1 + avg);
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else if(BTN2 == 1){ //if BTN2 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE2 + avg); //set frequency to 293
                sprintf(oled_str,"BTN2 pressed\nTone: %d", TONE2 + avg);
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else if(BTN3 == 1){ //if BTN3 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE3 + avg); //set frequency to 440
                sprintf(oled_str,"BTN3 pressed\nTone: %d", TONE3 + avg);
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else if(BTN4 == 1){ //if BTN4 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE4 + avg); //set frequency to 659
                sprintf(oled_str,"BTN4 pressed\nTone: %d", TONE4 + avg);
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else{ //if no buttons are pressed
            ToneGeneration_ToneOff(); //turn tone off if no button pressed
            sprintf(oled_str,"No Button pressed");
            btn_press = 0;
        }
        
        OledDrawString(oled_str); //draw OLED string
        OledUpdate(); //update OLED
    }
    
    while(1); //if embedded, never exit main
}

