/* 
 * File:   part4_m.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on May 15, 2021, 2:00 AM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "ICM20948.h"
#include "timers.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    ICM20948_Init();
    //TIMERS_Init();
    
    int m_X; //variable to store X magnetometer reading
    int m_Y; //variable to store Y magnetometer reading
    int m_Z; //variable to store Z magnetometer reading
    //int t; //time variable
    
    while(1){ //infinite loop
        //t = TIMERS_GetMilliSeconds();
        m_X = ICM20948_ReadMagX();
        m_Y = ICM20948_ReadMagY();
        m_Z = ICM20948_ReadMagZ();
        
        //print data to terminal
        printf("%d, %d, %d\r\n", m_X, m_Y, m_Z);
        //printf("m_Y: %d\r\n", m_Y);
        //printf("m_Z: %d\r\n", m_Z);
        
        //while((t%3000)!=0){
        //    t = TIMERS_GetMilliSeconds();
        //}
    }

    while(1);
}