/* 
 * File:   part10.c
 * Author: Jose
 *
 * Created on June 6, 2021, 9:45 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BOARD.h"
#include "Lab4_HelpFuncs.h"
#include "MatrixMath.h"
#include "timers.h"
#include "ICM20948.h"
#include "Oled.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init(); //initialize BOARD
    TIMERS_Init(); //initialize TIMERS
    ICM20948_Init(); //initialize ICM
    OledInit(); //initialize OLED
    
    //OLED printing stuff
    char oled_str[100];
    
    //tkme variables
    unsigned int t = TIMERS_GetMilliSeconds(); //set current time variable t
    unsigned int t0 = TIMERS_GetMilliSeconds(); //set start time variable t0
    
    //R output for DCMfromTriad() function
    float R[3][3] = {};
    
    float aI[3][1] = { //accel inertial
        { 0}, //x
        { 0}, //y
        { 1}  //z
    };
    
    float mI[3][1] = { //mag inertial (microTesla)
        { 23.5595}, //N 
        {  4.9062}, //E 
        { 39.7532}  //D
    };
    
    while(1){
        //every 20ms (50Hz)
        if((t - t0) > 20){ //if ((current time - start time) > 20ms) -> if 20 ms have passed
            //Clear oled
            OledClear(OLED_COLOR_BLACK);
            
            //collect data from sensors
            float a[3][1] = { //accelerometer with bias subtracted
                {ICM20948_ReadAccelX() - 158.0},
                {ICM20948_ReadAccelY() + 48.0},
                {ICM20948_ReadAccelZ() + 21.0}
            };
            
            float m[3][1] = { //magnetometer with bias subtracted
                {ICM20948_ReadMagX() - 14231},
                {ICM20948_ReadMagY() + 9444},
                {ICM20948_ReadMagZ() + 32568}
            };
            
            /*
            float a[3][1] = { //accelerometer with bias subtracted
                {1},
                {2},
                {3}
            };
            
            float m[3][1] = { //magnetometer with bias subtracted
                {4},
                {5},
                {6}
            };
            */
            
            //call DCM fromTriad Function
            DCMfromTriad(m, a, mI, aI, R);
            
            //print output
            //printf("R:\n");
            //MatrixPrint(R);
            float Eul[3][1] = {};
            DCM2Euler(R, Eul);
            sprintf(oled_str,"Euler Angles:\nYaw: %8.4f\nPitch: %8.4f\nRoll: %8.4f\n",
                             (double)Eul[0][0],(double)Eul[1][0],(double)Eul[2][0]);
            OledDrawString(oled_str);
            OledUpdate();
        
            t0 = TIMERS_GetMilliSeconds(); //update initial time t0
        }
        t = TIMERS_GetMilliSeconds(); //update time variable t
    }

    while(1);
}

