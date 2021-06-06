/*
 * File:   MatrixMath.c
 * Author: Jose Luis Santiago jlsantia@ucsc.edu
 *
 * Created on July 15, 2018, 11:59 PM
 */

//include necessary libraries
#include "MatrixMath.h"
#include <stdio.h>
#include <stdlib.h>

//user macros
#define TRUE 1
#define FALSE 0

//function takes pointer to matrix and prints it
void MatrixPrint(float mat[3][3])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < 3; i++) {
        printf("--------------------------------\n");
        fflush(stdout);
        printf("||");
        for (j = 0; j < 3; j++) {
            fflush(stdout);
            printf("%8.4f", (double) mat[i][j]); //print single element of matrix
            fflush(stdout);
            printf("||");
        }
        printf("\n");
    }
    printf("--------------------------------");
    printf("\n");
    fflush(stdout);
}


void VectorPrint(float vec[3][1]){
    int i;
    for(i = 0; i < 3; i++){
        printf("------------\n");
        fflush(stdout);
        printf("||");
        fflush(stdout);
        printf("%8.4f",(double)vec[i][0]);
        fflush(stdout);
        printf("||");
        printf("\n");
    }
    printf("------------\n");
    fflush(stdout);
}

//Function takes two matrix pointers for arguments and returns 1 if they're equal, 0 otherwise
int MatrixEquals(float mat1[3][3], float mat2[3][3])
{
    //nesting for loops to iterate through every matrix data entry
    int i;
    int j;
    for (i = 0; i < 3; i++) { //iterate through rows
        for (j = 0; j < 3; j++) { //iterate through columns
            if (abs(mat1[i][j] - mat2[i][j]) > FP_DELTA) { //check for equality accounting for round error
                return FALSE;
            }
        }
    }
    return TRUE;
}

//Function adds two matrices together and returns pointer to resultant matrix
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    //nesting for loops to iterate through every matrix data entry
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j]; //add matrices
        }
    }
}

void VectorAdd(float vec1[3][1], float vec2[3][1], float result[3][1]){
    int i;
    for(i = 0; i < 3; i++){
        result[i][0] = vec1[i][0] + vec2[i][0];
    }
}

//Function performs matrix multiplication returning pointer to resultant matrix
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3])
{
    int i;
    int j;
    int k;
    //nest for loops to iterate through matrices
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            float sum = 0;
            for (k = 0; k < 3; k++) {
                sum = (mat1[i][k] * mat2[k][j]) + sum;
                //result[i][j] = (mat1[i][k] * mat2[k][j]) + result[i][j]; //perform and store matrix multiplication into resultant matrix 
            }
            result[i][j] = sum;
        }
    }
}

void VectorMatrixMultiply(float vec[3][1], float mat[3][3], float result[3][1]){
    int i;
    int j;
    for(i = 0; i < 3; i++){ //iterate through rows
        float sum = 0;
        for(j = 0; j < 3; j++){ //iterate through columns
            sum = (vec[j][0]*mat[i][j]) + sum;
        }
        result[i][0] = sum;
    }
}

//Function performs scalar addition on Matrix
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3])
{
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = x + mat[i][j]; //perform scalar addition
        }
    }
}

void VectorScalarAdd(float x, float vec[3][1], float result[3][1]){
    int i;
    for(i = 0; i < 3; i++){
        result[i][0] = x + vec[i][0]; //scalar addition
    }
}

//function performs scalar multiplication on matrix
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3])
{
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            result[i][j] = x * mat[i][j]; //perform scalar multiplication
        }
    }
}

//function performs scalar multiplication on vector
void VectorScalarMultiply(float x, float vec[3][1], float result[3][1]){
    int i;
    for(i = 0; i < 3; i++){
        result[i][0] = x * vec[i][0];
    }
}

//function returns the trace of a matrix (sum of diagonals)
float MatrixTrace(float mat[3][3])
{
    int i;
    int j;
    float result;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j) {
                result = mat[i][j] + result; //store sum of diagonals into result
            }
        }
    }
    return result;
}

//function performs transpose of matrix
void MatrixTranspose(float mat[3][3], float result[3][3])
{
    int i;
    int j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (i == j) { //if it is on the diagonal set it equal to result matrix corresponding diagonal
                result[i][j] = mat[i][j];
            } else { //else switch row and column indexing
                result[j][i] = mat[i][j];
            }
        }
    }
}

//function performs
void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2])
{
    int row;
    int column;
    int m;
    int n;
    for (row = 0; row < 3; row++) {
        for (column = 0; column < 3; column++) {
            while ( row != i || column != j) {
                if (m > 1) {
                    m = 0;
                    n++;
                }
                if (n > 1) {
                    n = 0;
                }
                result[m][n] = mat[row][column];
                m++;
            }
        }
    }
}

