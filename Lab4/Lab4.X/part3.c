/* 
 * File:   part3.c
 * Author: Jose
 *
 * Created on May 27, 2021, 3:45 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BOARD.h"
#include "MatrixMath.h"
#include "Lab4_HelpFuncs.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    
    //variable declarations
    float yaw = 0;
    float pitch = 0;
    float roll = 0;
    
    float R[3][3] = {
        {1, 1, 1},
        {2, 2, 2},
        {3, 3, 3}
    };
    
    //call DCM2Euler function
    DCM2Euler(R, &yaw, &pitch, &roll);
    
    //Print yaw, pitch and roll
    printf("Yaw: %7.3f, Pitch: %7.3f, Roll: %7.3f\n", yaw, pitch, roll);

    while(1);
}

