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
    float Atilde_A[3][3] = { //Acceleration Atilde
        { 137.1583, 1.6615, -1.8007},
        {  1.8111, 133.1654, 2.8591},
        {-2.2770, -2.4555, 141.7359}
    }; 
    /*
    float Atilde_A[3][3] = { //Acceleration Atilde
        { 137.1583, 1.6615, -1.8007},
        {  1.8111, 133.1654, 2.8591},
        {-2.2770, -2.4555, 141.7359}
    }; 
    */
    
    float Atilde_H[3][3] = { //Magnetometer Atilde
        { 28620.4965, -22.5672, 310.3165},
        {-131.8772, 28284.2558, 341.8528},
        { -40.8672, 100.1499, 28048.0695}
    }; 
    /*
    float Atilde_H[3][3] = { //Magnetometer Atilde
        { 28620.4965, -22.5672, 310.3165},
        {-131.8772, 28284.2558, 341.8528},
        { -40.8672, 100.1499, 28048.0695}
    }; 
    */
    
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
    
    float bhat[3][1] = { //gyro bias
        {0},
        {0},
        {0}
    };
      
    float aI[3][1] = { //accel inertial
        { 0}, //x
        { 0}, //y
        { 1}  //z
    };
    
    float mI[3][1] = { //mag inertial (microTesla)
        {23.5595}, //N 
        { 4.9062}, //E 
        {39.7532}  //D 
    };
    
    //Feedback values
    float Kp_a = 2;
    float Kp_m = 2;
    float Ki_a = Kp_a/10.0;
    float Ki_m = Kp_m/10.0;
    
    //time step
    float dT = 1.0/50.0;
    
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
            float a[3][1] = { //accelerometer sensor data (gravity))
                {(ICM20948_ReadAccelX()/16384.0) - 158},
                {(ICM20948_ReadAccelY()/16384.0) +  48},
                {(ICM20948_ReadAccelZ()/16384.0) +  21}
            };
            
            //normalize accelerometer data
            VectorScalarMultiply(1.0/sqrt(pow(a[0][0],2)+pow(a[1][0],2)+pow(a[2][0],2)), a, a);
            VectorPrint(a);
            
            float m[3][1] = { //magnetometer sensor data (microTesla)
                {(ICM20948_ReadMagX()*0.15) - 14231},
                {(ICM20948_ReadMagY()*0.15) +  9444},
                {(ICM20948_ReadMagZ()*0.15) + 32568}
            };
            
            //normalize magnetometer data
            VectorScalarMultiply(1.0/sqrt(pow(m[0][0],2)+pow(m[1][0],2)+pow(m[2][0],2)), m, m);
            VectorPrint(m);
            
            //convert accels to unit norm
            //ahat = (Atilde_A*a) + Btilde_A
            float ahat[3][1] = {};
            VectorMatrixMultiply(a, Atilde_A, ahat); //ahat = Atilde_A*a
            VectorAdd(ahat, Btilde_A, ahat); //ahat = ahat + Btilde_A
            
            //convert mags to unit norm
            //mhat = (Atilde_H*m) + Btilde_H
            float mhat[3][1] = {};
            VectorMatrixMultiply(m, Atilde_H, mhat); //mhat = Atilde_H*m
            VectorAdd(mhat, Btilde_H, mhat); //mhat = mhat + Btilde_H
            
            //align magnetometer to accels
            //mhat_aligned = Rmis*mhat
            VectorMatrixMultiply(mhat, Rmis, mhat);
            
            //convert gyro to rad/s
            float w[3][1] = { // w = gyro(rad/s)
                {(ICM20948_ReadGyroX()/131.0) + 0.4866}, //this division might truncate
                {(ICM20948_ReadGyroY()/131.0) - 0.1444},
                {(ICM20948_ReadGyroZ()/131.0) + 0.0600}
            };
            
            //remove gyro bias
            //w = gyro(rad/s) - bhat;
            float _bhat[3][1] = {}; // -bhat
            VectorScalarMultiply(-1, bhat, _bhat);
            VectorAdd(w, _bhat, w); //w = gyro(rad/s) - bhat
            
            //Feedback
            // wmeas_a = [ab_x]*(RT*aI)
            float wmeas_a[3][1] = {};
            float ab_x[3][3] = {};
            float RT[3][3] = {};
            float RT_aI[3][1] = {};
            //rcross(a, ab_x); //not sure if this is right: ab_x = rcross(a)
            rcross(ahat, ab_x); //ab_x = rcross(ahat)
            MatrixTranspose(R, RT); //RT = transpose(R)
            VectorMatrixMultiply(aI, RT, RT_aI); //RT_aI = RT * aI
            VectorMatrixMultiply(RT_aI, ab_x, wmeas_a); //wmeas_a = [ab_x](RT*aI)
            
            //wmeas_m = [mb_x](RT*mI)
            float wmeas_m[3][1] = {};
            float mb_x[3][3] = {};
            float RT_mI[3][1] = {};
            //rcross(m, mb_x); //mb_x = rcross(m)
            rcross(mhat, mb_x); //mb_x = rcross(mhat)
            VectorMatrixMultiply(mI, RT, RT_mI); //RT_mI = RT * mI
            VectorMatrixMultiply(RT_mI, mb_x, wmeas_m); //wmeas_m = [mb_x](RT*mI)
            
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
            MatrixMultiply(R, R_exp, R); //R = R * R_exp = R * (Rexp(wtotal,dT))
            
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

