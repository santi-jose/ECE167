/* 
 * File:   part4.c
 * Author: Jose
 *
 * Created on May 29, 2021, 2:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "BOARD.h"
#include "Lab4_HelpFuncs.h"
#include "MatrixMath.h"
#include "timers.h"

/*
 * 
 */
int main(void)
{
    //initializations
    BOARD_Init();
    TIMERS_Init();
    
    //time variable
    unsigned int t;
    unsigned int t0;
    
    //variable declarations
    float p = 0;
    float q = 0;
    float r = 0;
    float G[3][1] = {
        {p},
        {q},
        {r}
    };
    
    float I[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    
    float RM[3][3] = {{},{},{}};
    
    /*
    //test matrix
    float TM1[3][1] = {
        {1},
        {2},
        {3}
    };
    
    float TM2[3][3] = {{},{},{}};
    float TM3[3][3] = {{},{},{}};
    float TM4[3][3] = {{},{},{}};
    float TM5[3][3] = {{},{},{}};
    
    //test rcross
    rcross(TM1, TM2);
    printf("TM1x: \n");
    MatrixPrint(TM2);
    */
    
    /*
    float R[3][3] = {
        {-0.6949, -0.1920, 0.6930},
        { 0.7135, -0.3038, 0.6313},
        { 0.0893,  0.9332, 0.3481}
    };
    */
    
    //printf("R:\n");
    //MatrixPrint(R);
    
    //float O[3][3] = {{},{},{}};
    
    //MatrixMultiply(I, R, O);
    //printf("IxR: \n");
    //MatrixPrint(O);
    
    //printf("R:\n");
    //MatrixPrint(R);
    
    //test rexp
    /*
    Rexp(TM1, 1, TM3);
    printf("Rexp: \n");
    MatrixPrint(TM3);
    
    //test matrixmultiply
    MatrixMultiply(TM3, I, TM4);
    printf("IntegrateOpenLoop1: \n");
    MatrixPrint(TM4);    
    
    //test IntegrateOpenLoop
    IntegrateOpenLoop(I, TM1, 1, TM5);
    printf("IntegrateOpenLoop2: \n");
    MatrixPrint(TM5);
    */
    
    int i = 0;
    t = TIMERS_GetMilliSeconds();
    t0 = TIMERS_GetMilliSeconds();
    while(1){
        if((t - t0) > 1000){
            //add bias to gyro matrix G
            G[0][0] = G[0][0] + 0.05;
            G[1][0] = G[1][0] + 0.04;
            G[2][0] = G[2][0] + 0.03;
        
            //test IntegrateOpenLoop Function
            IntegrateOpenLoop(I, G, 1, RM);
    
            //print this thing
            printf("R: \n");
            MatrixPrint(RM);  
        
            t0 = TIMERS_GetMilliSeconds();
            i = i + 1;
        }
        t = TIMERS_GetMilliSeconds();
        if(i >= 20){
            break;
        }
    }
        
    while(1);
}
