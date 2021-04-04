/* 
 * File:   part3.c
 * Author: Jose
 *
 * Created on April 3, 2021, 12:13 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "ToneGeneration.h"

/*
 * 
 */
int main(void)
{
    BOARD_Init(); //init BOARD
    ToneGeneration_Init(); //init ToneGeneration
    
    ToneGeneration_SetFrequency(TONE_293); //set ToneGeneration frequency
    ToneGeneration_ToneOn(); //turn ToneGeneration on
    
    while(1){ //infinite loop
        //keep tone on
    }
    
    while(1); //if embedded we never exit
}

