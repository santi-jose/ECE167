/* 
 * File:   part7.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on June 3, 2021, 5:30 PM
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
    
    //input
    float Rm[3][3] = { //Rminus
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    float Bm[3][1] = { //Bminus
        {1},
        {1},
        {1}
    };
    //VectorPrint(Bm);
    
    float g[3][1] = { //gyros
        {2},
        {2},
        {2}
    };
    //VectorPrint(g);
    
    float m[3][1] = { //mags
        {3},
        {3},
        {3}
    };
    //VectorPrint(m);
    
    float a[3][1] = { //accels
        {4},
        {4},
        {4}
    };
    //VectorPrint(a);
    
    float mI[3][1] = { //magInertial
        {5},
        {5},
        {5}
    };
    //VectorPrint(mI);
    
    float aI[3][1] = { //accelInertial
        {6},
        {6},
        {6}
    };
    //VectorPrint(aI);
    
    float dT = 1.0/50.0; //deltaT
    //printf("dT: %f\n",dT);
    
    //output
    float Rp[3][3] = {}; //Rplus
    float Bp[3][1] = {}; //Bplus
      
    //MatrixPrint(Rp);
    //VectorPrint(Bp);
    
    IntegrateClosedLoop(Rm,Bm,g,m,a,mI,aI,dT,Rp,Bp);

        
    while(1);
}

