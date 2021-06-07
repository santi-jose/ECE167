/* 
 * File:   part9.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on June 6, 2021, 1:21 AM
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
    
    //Required Matrices and arrays from Part 8
    /*float Atilde_A[3][3] = { //Acceleration Atilde
        { 137.1583, 1.6615, -1.8007},
        {  1.8111, 133.1654, 2.8591},
        {-2.2770, -2.4555, 141.7359}
    };
    */
    
    float Atilde_A[3][3] = { //Acceleration Atilde
        {0.00005747852254497799, 0.0000006962760133109267, -0.0000007546228815041981},
        {0.0000007589899614982062, 0.00005580524191675066, -0.000001198147055853526},
        {-0.0000009542186182729570, -0.000001029007307272294, 0.00005939687630099455}
    }; 
    
    /*
    float Atilde_H[3][3] = { //Magnetometer Atilde
        { 28620.4965, -22.5672, 310.3165},
        {-131.8772, 28284.2558, 341.8528},
        { -40.8672, 100.1499, 28048.0695}
    }; 
    */
    
    float Atilde_H[3][3] = { //Magnetometer Atilde
        { 0.003437506848173, -0.000002710464790030440, 0.00003727102558938816},
        {-0.00001583930943518170, 0.003397122170054, 0.00004105873401311830},
        {-0.000004908413532090992, 0.00001202865575987469, 0.003368754667169}
    }; 
    
    
    float Btilde_A[3][1] = { //Acceleration Btilde
        {-0.0071},
        {-0.0023},
        { 0.0257}
    }; 
    /*float Btilde_A[3][1] = { //Acceleration Btilde
        {-0.0071},
        {-0.0023},
        { 0.0257}
    };*/
    
    float Btilde_H[3][1] = { //Magnetometer Btilde
        {  47.4216},
        { -33.4039},
        {-109.1870}
    };
    /*float Btilde_H[3][1] = { //Magnetometer Btilde
        {  47.4216},
        { -33.4039},
        {-109.1870}
    }; */
    
    float Rmis[3][3] = { //Misalignment Matrix
        {  -0.6133, 0.6482, 0.4514},
        {-0.6756, -0.1343, -0.7250},
        { -0.4093, -0.7495, 0.5203}
    };
    /*float Rmis[3][3] = { //Misalignment Matrix
        {  -0.6133, 0.6482, 0.4514},
        {-0.6756, -0.1343, -0.7250},
        { -0.4093, -0.7495, 0.5203}
    };*/
    
    float R[3][3] = { //initial DCM
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    
    float bhat[3][1] = { //initial gyro bias
        {0},
        {0},
        {0}
    };
      
    float aI[3][1] = { //accel inertial
        { 0}, //x
        { 0}, //y
        { 1}  //z
    };
    
    float mI[3][1] = { //mag inertial (nanoTesla)
        { 23.5595}, //N 
        {  4.9062}, //E 
        { 39.7532}  //D
    };
    
    //Feedback values
    //float Kp_a = 1; //these work for Yaw and Roll
    //float Kp_m = 1;
    float Kp_a = 1;
    float Kp_m = 0.3;
    float Ki_a = Kp_a/10.0;
    float Ki_m = Kp_m/10.0;
    
    //time step
    float dT = 1.0/50.0;
    
    //sensor data variables
    float a[3][1] = {}; //raw accelerometer sensor data
    float m[3][1] = {}; //raw magnetometer sensor data
    float g[3][1] = {}; //raw gyro sensor data
    
    //tkme variables
    unsigned int t = TIMERS_GetMilliSeconds(); //set current time variable t
    unsigned int t0 = TIMERS_GetMilliSeconds(); //set start time variable t0
    
    while(1){
        //every 20ms (50Hz)
        if((t - t0) > 20){ //if ((current time - start time) > 20ms) -> if 20 ms have passed
            //Clear Oled
            OledClear(OLED_COLOR_BLACK);
            
            //Algorithm 1: Data Processing for Attitude Estimation
            
            //Read ICM Data 
            //remove accelerometer bias
            //a[0][0] = ICM20948_ReadAccelX();
            //a[1][0] = ICM20948_ReadAccelY();
            //a[2][0] = ICM20948_ReadAccelZ();            
            a[0][0] = (ICM20948_ReadAccelX() - 158.0)*-1;
            a[1][0] = (ICM20948_ReadAccelY() + 48.0);
            a[2][0] = (ICM20948_ReadAccelZ() + 21.0);
            
            //normalize accelerometer data
            //VectorScalarMultiply(1.0/sqrt(pow(a[0][0],2)+pow(a[1][0],2)+pow(a[2][0],2)), a, a);
            //printf("a:\n");
            //VectorPrint(a);
            
            //remove magnetometer bias
            //m[0][0] = ICM20948_ReadMagX();
            //m[1][0] = ICM20948_ReadMagY();
            //m[2][0] = ICM20948_ReadMagZ();
            m[0][0] = (ICM20948_ReadMagX() - 14231);
            m[1][0] = (ICM20948_ReadMagY() + 9444);
            m[2][0] = (ICM20948_ReadMagZ() + 32568);
            
            //normalize magnetometer data
            //VectorScalarMultiply(1.0/sqrt(pow(m[0][0],2)+pow(m[1][0],2)+pow(m[2][0],2)), m, m);
            //printf("m:\n");
            //VectorPrint(m);
            
            g[0][0] = ICM20948_ReadGyroX();
            g[1][0] = ICM20948_ReadGyroY();
            g[2][0] = ICM20948_ReadGyroZ();
            
            //printf("g:\n");
            //VectorPrint(g);
            
            //convert accels to unit norm
            //ahat = (Atilde_A*a) + Btilde_A
            float ahat[3][1] = {};
            VectorMatrixMultiply(a, Atilde_A, ahat); //ahat = Atilde_A*a
            VectorAdd(ahat, Btilde_A, ahat); //ahat = ahat + Btilde_A = Atilde_A*a + Btilde_A
            
            //convert mags to unit norm
            //mhat = (Atilde_H*m) + Btilde_H
            float mhat[3][1] = {};
            VectorMatrixMultiply(m, Atilde_H, mhat); //mhat = Atilde_H*m
            VectorAdd(mhat, Btilde_H, mhat); //mhat = mhat + Btilde_H = Atilde_H*m + Btilde_H
            
            //align magnetometer to accels
            //mhat_aligned = Rmis*mhat
            VectorMatrixMultiply(mhat, Rmis, mhat);
            
            //convert gyro data to rad/s
            float w[3][1] = {};
            VectorScalarMultiply(1.0/131.0, g, w); //convert to dps
            //printf("w dps: \n");
            //VectorPrint(w);
            VectorScalarMultiply((M_PI/180), w, w); //convert to rad/s
            //printf("w rps: \n");
            //VectorPrint(w);
            
            //remove gyro bias
            //w = gyro(rad/s) - bhat;
            float _bhat[3][1] = {}; // -bhat
            VectorScalarMultiply(-1.0, bhat, _bhat);
            VectorAdd(w, _bhat, w); //w = gyro(rad/s) - bhat
            //printf("w - bhat:\n");
            //VectorPrint(bhat);
            //VectorPrint(w);
            
            //Feedback
            // wmeas_a = [ab_x]*(R*aI)
            float wmeas_a[3][1] = {};
            float ab_x[3][3] = {};
            float R_aI[3][1] = {};
            //rcross(a, ab_x); //not sure if this is right: ab_x = rcross(a)
            rcross(ahat, ab_x); //ab_x = rcross(ahat)
            //printf("ahat: \n");
            //VectorPrint(ahat);
            //printf("ab_x:\n");
            //MatrixPrint(ab_x);
            VectorMatrixMultiply(aI, R, R_aI); //R_aI = R * aI
            //printf("R:\n");
            //MatrixPrint(R);
            //printf("aI:\n");
            //VectorPrint(aI);
            VectorMatrixMultiply(R_aI, ab_x, wmeas_a); //wmeas_a = [ab_x](R*aI)
            //printf("wmeas_a:\n");
            //VectorPrint(wmeas_a);
            
            //wmeas_m = [mb_x](RT*mI)
            float wmeas_m[3][1] = {};
            float mb_x[3][3] = {};
            float R_mI[3][1] = {};
            //rcross(m, mb_x); //mb_x = rcross(m)
            rcross(mhat, mb_x); //mb_x = rcross(mhat)
            VectorMatrixMultiply(mI, R, R_mI); //R_mI = R * mI
            VectorMatrixMultiply(R_mI, mb_x, wmeas_m); //wmeas_m = [mb_x](R*mI)
            
            //wtotal = w + (Kp_a * wmeas_a) + (Kp_m * wmeas_m)
            float wtotal[3][1] = {};
            float Kp_a_wmeas_a[3][1] = {};
            float Kp_m_wmeas_m[3][1] = {};
            VectorScalarMultiply(Kp_a, wmeas_a, Kp_a_wmeas_a); //Kp_a_wmeas_a = Kp_a*wmeas_a
            VectorScalarMultiply(Kp_m, wmeas_m, Kp_m_wmeas_m); //Kp_m_wmeas_m = Kp_m*wmeas_m
            VectorAdd(Kp_a_wmeas_a, Kp_m_wmeas_m, wtotal); //wtotal = Kp_a_wmeas_a + Kp_m_wmeas_m
            VectorAdd(w, wtotal, wtotal); //wtotal = w + wtotal = w + (Kp_a_wmeas_a + Kp_m_wmeas_m)
            
            //Integrate: ClosedLoopIntegrate()
            //R = R * Rexp(wtotal, dT)
            float R_exp[3][3] = {};
            Rexp(wtotal, dT, R_exp); //R_exp = Rexp(wtotal, dT)
            MatrixMultiply(R_exp, R, R); //R = R_exp*R = (Rexp(wtotal,dT))*R
            //MatrixMultiply(R, R_exp, R);
            
            //bdot = (-Ki_a * wmeas_a) + (-Ki_m * wmeas_m)
            float bdot[3][1] = {};
            float _Ki_a_wmeas_a[3][1] = {};
            float _Ki_m_wmeas_m[3][1] = {};
            VectorScalarMultiply((-1.0)*Ki_a, wmeas_a, _Ki_a_wmeas_a); //_Ki_a_wmeas_a = -(Ki_a*wmeas_a)
            VectorScalarMultiply((-1.0)*Ki_m, wmeas_m, _Ki_m_wmeas_m); //_Ki_m_wmeas_m = -(Ki_m*wmeas_m)
            VectorAdd(_Ki_a_wmeas_a, _Ki_m_wmeas_m, bdot); //bdot = (_Ki_a_wmeas_a) + (Ki_m_wmeas_m) = -(Ki_a*wmeas_a) - (Kp_m*wmeas_m)
            
            //bhat = bhat + (bdot * dT)
            float bdot_dT[3][1] = {};
            VectorScalarMultiply(dT, bdot, bdot_dT); //bdot_dT = bdot * dT
            VectorAdd(bhat, bdot_dT, bhat); //bhat = bhat + (bdot_dT) = (bhat + bdot*dT)
            
            //Calculate & print Euler Angles
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

