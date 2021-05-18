/* 
 * File:   part6.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on May 16, 2021, 5:53 PM
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
    
    int g_Z; //variable to store Z magnetometer reading
    int g_X; //variable to store X magnetometer reading
    int g_Y; //variable to store Y magnetometer reading
    
    while(1){ //infinite loop
        g_X = ICM20948_ReadGyroX();
        g_Y = ICM20948_ReadGyroY();
        g_Z = ICM20948_ReadGyroZ();
 
        printf("%d, %d, %d\r\n", g_X, g_Y, g_Z);
        
    }

    while(1);
}

