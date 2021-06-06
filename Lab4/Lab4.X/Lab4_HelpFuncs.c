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
void DCM2Euler(float M[3][3], float Eul[3][1]){ //float* y, float* p, float* r){
    Eul[0][0] = atan2(M[0][1],M[0][0]) * 57.2958; //calculate yaw in degrees
    Eul[1][0] = asin((-1)*(M[0][2])) * 57.2958;  //calculate pitch in degrees
    Eul[2][0] = atan2(M[1][2],M[2][2]) * 57.2958; //calculate roll in degrees
    /*
    *y = atan2(M[0][1],M[0][0]) * 57.2958; //calculate yaw in degrees
    *p = asin((-1)*(M[0][2])) * 57.2958;  //calculate pitch in degrees
    *r = atan2(M[1][2],M[2][2]) * 57.2958; //calculate roll in degrees
    */
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
    }
    
    //(-sincw) * wx
    MatrixScalarMultiply(((-1)*sincw), wx, wx_sincw);
    
    //wx * wx
    MatrixMultiply(wx, wx, wx2);
    
    //wx * wx * oneMinusCosW
    MatrixScalarMultiply(oneMinusCosW, wx2, wx2_oMCW);
    
    //calculate R_exp
    MatrixAdd(I, wx_sincw, R_exp); //R_exp = I - (sincW * wx)
    MatrixAdd(R_exp, wx2_oMCW, R_exp); //R_exp = R_exp + ((wx*2)*oneMinusCosW)
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
        MatrixMultiply(R_exp, Rminus, Rplus);
    }else{
        float Gx[3][3];
        float Gx_Rminus[3][3];
        rcross(G, Gx); //convert G into cross product matrix
        MatrixMultiply(Gx, Rminus, Gx_Rminus); //multiply Gx and Rminus
        MatrixScalarMultiply((-1)*deltaT, Gx_Rminus, Gx_Rminus); //multiply by (-)deltaT
        MatrixAdd(Rminus, Gx_Rminus, Rplus); //Rplus = Rminus - Gx*Rminus*deltaT
    }
}

void IntegrateClosedLoop(float Rm[3][3], float Bm[3][1], float g[3][1], float m[3][1], 
                         float a[3][1], float mI[3][1], float aI[3][1], float dT,
                         float Rp[3][3],float Bp[3][1]){
    float Kp_a = 10;
    float Ki_a = Kp_a/10;
    
    float Kp_m = 10;
    float Ki_m = Kp_m/10;
    
    //set mags and accels to unit vectors
    //accels = accels/norm(accels)
    //mags = mags/norm(mags)
    VectorScalarMultiply(1.0/(sqrt(pow(a[0][0],2)+pow(a[1][0],2)+pow(a[2][0],2))),a,a);
    VectorScalarMultiply(1.0/(sqrt(pow(m[0][0],2)+pow(m[1][0],2)+pow(m[2][0],2))),m,m);
    
    //set inertial reference vectors
    //magInertial = magInertial/norm(magInertial)
    //accelInertial = accelInertial/norm(accelInertial)
    VectorScalarMultiply(1.0/(sqrt(pow(mI[0][0],2)+pow(mI[1][0],2)+pow(mI[2][0],2))),mI,mI);
    VectorScalarMultiply(1.0/(sqrt(pow(aI[0][0],2)+pow(aI[1][0],2)+pow(aI[2][0],2))),aI,aI);
    
    //gyroInputWithBias = gyros - Bminus
    float gIWB[3][1] = {};
    float _Bm[3][1] = {}; 
    VectorScalarMultiply(-1.0,Bm,_Bm); //-Bm
    VectorAdd(g, _Bm, gIWB);
    
    //ax = rcross(accels)
    float ax[3][3] = {};
    rcross(a, ax);
    
    //Rm_aI = Rminus * accelInertial
    float Rm_aI[3][1] = {};
    VectorMatrixMultiply(aI, Rm, Rm_aI);
    
    //accelerometer correction in the body frame
    //wmeas_a = (rcross(accels)) * (Rminus * accelInertial)
    float wmeas_a[3][1] = {};
    VectorMatrixMultiply(Rm_aI, ax, wmeas_a);
    
    //mx = rcross(mags)
    float mx[3][3] = {};
    rcross(m, mx);
    
    //Rm_mI = Rminus * magInertial
    float Rm_mI[3][1] = {};
    VectorMatrixMultiply(mI, Rm, Rm_mI);
    
    //magnetometer correction in the body frame
    //wmeas_m = (rcross(mags)) * (Rminus * magInertial)
    float wmeas_m[3][1] = {};
    VectorMatrixMultiply(Rm_mI, mx, wmeas_m);
    
    //Kp_a_wmeas_a = Kp_a * wmeas_a
    float Kp_a_wmeas_a[3][1] = {};
    VectorScalarMultiply(Kp_a, wmeas_a, Kp_a_wmeas_a);
    
    //Kp_m_wmeas_w = Kp_m * wmeas_m
    float Kp_m_wmeas_m[3][1] = {};
    VectorScalarMultiply(Kp_m, wmeas_m, Kp_m_wmeas_m);    
    
    //Kp_a_wm_a_Kp_m_wm_m = Kp_a_wm_a + Kp_m_wm_m
    float Kp_a_wm_a_Kp_m_wm_m[3][1] = {};
    VectorAdd(Kp_a_wmeas_a, Kp_m_wmeas_m, Kp_a_wm_a_Kp_m_wm_m);
    
    //gyroInputWithFeedback = gyroInputWithBias + Kp_a*wmeas_a + Kp_m*wmeas_m
    float gIWF[3][1] = {};
    VectorAdd(gIWB, Kp_a_wm_a_Kp_m_wm_m, gIWF);
    
    //_Ki_a_wm_a = -(Ki_a * wmeas_a)
    float _Ki_a_wm_a[3][1] = {};
    VectorScalarMultiply(((-1.0)*Ki_a), wmeas_a, _Ki_a_wm_a);
    
    //_Ki_m_wm_m = -(Ki_m * wmeas_m)
    float _Ki_m_wm_m[3][1] = {};
    VectorScalarMultiply(((-1.0)*Ki_m), wmeas_m, _Ki_m_wm_m);
    
    //bdot = -(Ki_a * wmeas_a) - Ki_m * wmeas_m
    float bdot[3][1];
    VectorAdd(_Ki_a_wm_a, _Ki_m_wm_m, bdot);
    
    //R_exp = Rexp(gyroInputWithFeedback, deltaT)
    float R_exp[3][3] = {};
    Rexp(gIWF, dT, R_exp);
    
    //Rplus = (Rexp(gyroInputWithFeedback, deltaT)) * Rminus;
    MatrixMultiply(R_exp, Rm, Rp);
    
    //bd_dT = bdot*deltaT
    float bd_dT[3][1] = {};
    VectorScalarMultiply(dT, bdot, bd_dT);
    
    //Bplus = Bminus + (bdot*deltaT)
    VectorAdd(Bm, bd_dT, Bp);
}