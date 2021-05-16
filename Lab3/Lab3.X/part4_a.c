/* 
 * File:   part4_a.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on May 13, 2021, 4:10 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "ICM20948.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    ICM20948_Init();
    
    int a_Z; //variable to store Z acceleration
    int a_X; //variable to store X acceleration
    int a_Y; //variable to store Y acceleration
    
    while(1){ //infinite loop
        //t = TIMERS_GetMilliSeconds();
        a_Z = ICM20948_ReadAccelZ();
        a_X = ICM20948_ReadAccelX();
        a_Y = ICM20948_ReadAccelY();
        
        //print data to terminal
        printf("a_X: %d\r\n", a_X);
        printf("a_Y: %d\r\n", a_Y);
        printf("a_Z: %d\r\n", a_Z);
    }

    while(1);
}

