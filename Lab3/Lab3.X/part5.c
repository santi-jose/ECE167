/* 
 * File:   part5.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on May 15, 2021, 5:29 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    ICM20948_Init();
    TIMERS_Init();
    
    int g_Z; //variable to store Z magnetometer reading
    int g_X; //variable to store X magnetometer reading
    int g_Y; //variable to store Y magnetometer reading
    int t; //time variable
    
    while(1){ //infinite loop
        t = TIMERS_GetMilliSeconds();
        g_Z = ICM20948_ReadGyroZ();
        g_X = ICM20948_ReadGyroX();
        g_Y = ICM20948_ReadGyroY();
        
        //print data to terminal
        if((t%20) == 0){ //every 20ms, 50 times a second(50Hz)
            printf("%d, %d, %d\r\n", g_X, g_Y, g_Z);
        }
        //printf("m_Y: %d\r\n", m_Y);
        //printf("m_Z: %d\r\n", m_Z);
        
        //while((t%3000)!=0){
        //    t = TIMERS_GetMilliSeconds();
        //}
    }

    while(1);
}

