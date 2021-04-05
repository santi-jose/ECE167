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
    
    //variable declarations
    char oled_str[100]; //oled_str
    int btn_press = 0; //boolean to check if button is pressed
    
    while(1){ //infinite loop
        OledClear(OLED_COLOR_BLACK); //clear OLED
        
        //Button pressing logic
        if(BTN1 == 1){ //if BTN1 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE_196); //set frequency to 196
                sprintf(oled_str,"BTN1 pressed"); //update OLED string
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else if(BTN2 == 1){ //if BTN2 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE_293); //set frequency to 293
                sprintf(oled_str,"BTN2 pressed"); //update OLED str
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else if(BTN3 == 1){ //if BTN3 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE_440); //set frequency to 440
                sprintf(oled_str,"BTN3 pressed"); //update OLED str
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else if(BTN4 == 1){ //if BTN4 is pressed
            if(btn_press == 0){ //if no button is pressed
                ToneGeneration_SetFrequency(TONE_659); //set frequency to 659
                sprintf(oled_str,"BTN4 pressed"); //update OLED str
                btn_press = 1; //set btn_press to 1
                ToneGeneration_ToneOn(); //turn ToneGeneration on
            }
        }else{ //if no buttons are pressed
            ToneGeneration_ToneOff(); //turn tone off if no button pressed
            sprintf(oled_str,"No Button pressed");
            btn_press = 0;
        }
        
        OledDrawString(oled_str);
        OledUpdate(); //update OLED
    }
    
    while(1);
}

