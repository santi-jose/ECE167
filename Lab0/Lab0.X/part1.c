/* 
 * File:   part1.c
 * Author: Jose (jlsantia@ucsc.edu)
 *
 * Created on April 1, 2021, 1:46 AM
 */

#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h" 
#include "serial.h"

/*
 * 
 */
int main(void)
{
    BOARD_Init();
    char m[] = "Hello World!\n";
    int i;
    for(i = 0; i < 13; i++)
    {
        PutChar(m[i]);
    }
    
    //printf("Hello World!\n");
    
    
    
    BOARD_End(); //handles cleanup of system
    while(1); //if embedded we never exit
}

