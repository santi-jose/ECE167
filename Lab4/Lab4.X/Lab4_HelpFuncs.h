#ifndef LAB4_HELPFUNCS_H
#define LAB4_HELPFUNCS_H

/**
 * @file
 *
 * @section DESCRIPTION
 *
 * This file implements the conversion of MATLAB functions provided for Lab4
 * in C
 *
 */

/*
 * DCM2Euler extracts Euler angles from DCM.  
 * @param: M, pointer to a 3x3 float array. y, p, and r pointers to float variables.
 * @return: none
 */
void DCM2Euler(float M[3][3], float Eul[3][1]);//float* y, float* p, float* r);

/*
 * rcross forms the skew symmetric x-product matrix of a 3x1 vector.  
 * @param: R, pointer to a 3x1 float array. RC, pointer to a 3x3  float array.
 * @return: none
 */
void rcross(float R[3][1], float RC[3][3]);

/*
 * Rexp returns the exponential Rodrigues parameter form of
 * the integration that keeps R on SO(3). Requires a time step and 
 * rotation rate   
 * @param: Rotation rate, w, pointer to a 3x1 float array. 
 * Time step, deltaT, a float variable. R_exp, exponential Rodrigues parameter 
 * form, pointer to 3x3 float array.
 * @return: none
 */
void Rexp(float w[3][1], float deltaT, float R_exp[3][3]);

/*
 * IntegrateOpenLoop integrates the gyros to attitude DCM.  
 * @param: Rminus, pointer to a 3x3 float array. G, pointer to a 3x1  float array.
 * deltaT, float variable. Rplus, pointer to a 3x3 float array.
 * @return: none
 */
void IntegrateOpenLoop(float Rminus[3][3], float G[3][1], float deltaT, float Rplus[3][3]);

/*
 * IntegrateClosedLoop integrates the gyros to attitude DCM.  
 * @param: Rminus, pointer to a 3x3 float array. G, pointer to a 3x1  float array.
 * deltaT, float variable. Rplus, pointer to a 3x3 float array.
 * @return: none
 */
void IntegrateClosedLoop(float Rm[3][3], float Bm[3][1], float g[3][1], float m[3][1], 
                         float a[3][1], float mI[3][1], float aI[3][1], float dT,
                         float Rp[3][3], float Bp[3][1]);

/*
 * DCMfromTriad implements a solution to Wahba's problem based in the TRIAD algorithm
 * to give you an estimate of the DCM from two non-collinear vector measurements.  
 * @param: m, 3x1 float vector of mags in body coordinates.
 * a, 3x1 float vector of gravity in body coordinates.
 * mI, 3x1 float vector of inertial magnetic reference vector
 * aI, 3x1 float vector of gravity reference vector (accelInertial)
 * R, 3x3 float matrix, DCM estimate
 * @return: none
 */
void DCMfromTriad(float m[3][1], float a[3][1], float mI[3][1], float aI[3][1], float R[3][3]);

#endif // LAB4_HELPFUNCS_H
