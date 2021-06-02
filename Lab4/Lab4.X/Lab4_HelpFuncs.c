/*
 * File:   Lab4_HelpFuncs.c
 * Author: Jose Luis Santiago jlsantia@ucsc.edu
 *
 * Created on June 1, 2021, 12:11 PM
 */

#include "Lab4_HelpFuncs.h"
#include "MatrixMath.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

/*
 * DCM2Euler extracts Euler angles from DCM.  
 * @param: M, pointer to a 3x3 float array. y, p, and r pointers to float variables.
 * @return: none
 */
void DCM2Euler(float M[3][3], float* y, float* p, float* r){
    *y = atan(M[0][1]/M[0][0]) * 57.2958; //calculate yaw in degrees
    *p = asin((-1)*(M[0][2])) * 57.2958;  //calculate pitch in degrees
    *r = atan(M[1][2]/M[2][2]) * 57.2958; //calculate roll in degrees
}

/*
 * rcross forms the skew symmetric x-product matrix of a 3x1 vector.  
 * @param: R, pointer to a 3x1 float array. RC, pointer to a 3x3  float array.
 * @return: none
 */
void rcross(float R[3][1], float RC[3][3]){
    float r1 = R[0][0];
    float r2 = R[1][0];
    float r3 = R[2][0];
    
    RC[0][0] = 0;
    RC[0][1] = (-1)*r3;
    RC[0][2] = r2;
    
    RC[1][0] = r3;
    RC[1][1] = 0;
    RC[1][2] = (-1)*r1;
    
    RC[2][0] = (-1)*r2;
    RC[2][1] = r1;
    RC[2][2] = 0;
}

/*
 * Rexp returns the exponential Rodrigues parameter form of
 * the integration that keeps R on SO(3). Requires a time step and 
 * rotation rate   
 * @param: Rotation rate, w, pointer to a 3x1 float array. 
 * Time step, deltaT, a float variable. R_exp, exponential Rodrigues parameter 
 * form, pointer to 3x3 float array.
 * @return: none
 */
void Rexp(float w[3][1], float deltaT, float R_exp[3][3]){
    //calculate the norm of omega(w)
    float wnorm = sqrt(pow(w[0][0],2) + pow(w[1][0],2) + pow(w[2][0], 2));
    float wx[3][3] = {{},{},{}}; //wx = omega cross matrix
    float wx2[3][3] = {{},{},{}}; //wx matrix multiplied by itself twice
    float wx_sincw[3][3] = {{},{},{}}; //wx * sincw
    float wx2_oMCW[3][3] = {{},{},{}}; //wx^2 * oneMinusCosW
    float I[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    }; //3x3 identity matrix
    rcross(w, wx);
    float sincw; //variable to store the sinc of omega (w)
    float oneMinusCosW; //variable to store oneMinusCos of omega (w)
    
    //calculate sincw and oneMinusCosW
    if(wnorm < 0.2){ //wnorm is less than 0.2
        sincw = deltaT 
                - (pow(deltaT,3) * pow(wnorm,2))/6 
                + (pow(deltaT,5) * pow(wnorm,4))/120;
        oneMinusCosW = (pow(deltaT,2))/2 
                - (pow(deltaT,4) * pow(wnorm,2))/24 
                + (pow(deltaT,6) * pow(wnorm,4))/720;
    }else{ //wnorm is greater than 0.2
        sincw = (sin(wnorm * deltaT))/wnorm;
        oneMinusCosW = (1 - cos(wnorm*deltaT))/(pow(wnorm,2));
        //printf("sincw: %f\n",sincw);
        //printf("oneMinusCosW: %f\n",oneMinusCosW);
    }
    
    //(-sincw) * wx
    MatrixScalarMultiply(((-1)*sincw), wx, wx_sincw);
    //MatrixPrint(wx_sincw);
    
    //wx * wx
    MatrixMultiply(wx, wx, wx2);
    //MatrixPrint(wx2);
    
    //wx * wx * oneMinusCosW
    MatrixScalarMultiply(oneMinusCosW, wx2, wx2_oMCW);
    //MatrixPrint(wx2_oMCW);
    
    //calculate R_exp
    MatrixAdd(I, wx_sincw, R_exp); //R_exp = I - (sincW * wx)
    //MatrixPrint(R_exp);
    MatrixAdd(R_exp, wx2_oMCW, R_exp); //R_exp = R_exp + ((wx*2)*oneMinusCosW)
    //MatrixPrint(R_exp);
}

/*
 * IntegrateOpenLoop integrates the gyros to attitude DCM.  
 * @param: Rminus, pointer to a 3x3 float array. G, pointer to a 3x1  float array.
 * deltaT, float variable. Rplus, pointer to a 3x3 float array.
 * @return: none
 */
void IntegrateOpenLoop(float Rminus[3][3], float G[3][1], float deltaT, float Rplus[3][3]){
    int UseMatrixExponential = TRUE;
    float R_exp[3][3];
    if(UseMatrixExponential == TRUE){
        Rexp(G, deltaT, R_exp);
        //MatrixPrint(R_exp);
        MatrixMultiply(R_exp, Rminus, Rplus);
        //MatrixPrint(Rplus);
    }else{
        float Gx[3][3];
        float Gx_Rminus[3][3];
        rcross(G, Gx); //convert G into cross product matrix
        MatrixMultiply(Gx, Rminus, Gx_Rminus); //multiply Gx and Rminus
        MatrixScalarMultiply((-1)*deltaT, Gx_Rminus, Gx_Rminus); //multiply by (-)deltaT
        MatrixAdd(Rminus, Gx_Rminus, Rplus); //Rplus = Rminus - Gx*Rminus*deltaT
    }
}