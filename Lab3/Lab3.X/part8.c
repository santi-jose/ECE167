/* 
 * File:   part8.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on May 17, 2021, 1:02 PM
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
    
    int a_X; //variable to store X accelerometer reading
    int a_Y; //variable to store Y accelerometer reading
    int a_Z; //variable to store Z accelerometer reading
    
    int m_X; //variable to store X magnetometer reading
    int m_Y; //variable to store Y magnetometer reading
    int m_Z; //variable to store Z magnetometer reading
    
    while(1){ //infinite loop
        //Store acceleration readings in all axes
        a_X = ICM20948_ReadAccelX();
        a_Y = ICM20948_ReadAccelY();
        a_Z = ICM20948_ReadAccelZ();
        
        //store magnetometer readings in all axes
        m_X = ICM20948_ReadMagX();
        m_Y = ICM20948_ReadMagY();
        m_Z = ICM20948_ReadMagZ();
 
        printf("%d, %d, %d, , %d, %d, %d\r\n",
                a_X, a_Y, a_Z,
                m_X, m_Y, m_Z);
        
    }

    while(1);
}

