/* 
 * @file    QEI.c
 * @brief   Quadrature Encoder sensing module
 * @author  Jose Santiago (jlsantia)
 * @date    4/25/2021
 * @detail  This module uses the change notify peripheral to interface to a quadrature encoder. 
 *          If you are unfamiliar with change notify please read the lab appendix before beginning.
 *          The peripheral is configured to generate an interrupt on any change in input of pins RD6
 *          or RD7 (pins 36 and 37 on the Uno32). The interrupt will not tell you which pin changed
 *          states so you will need to implement a simple state machine to handle the transitions and
 *          keep track of the encoder count.
 *          NOTE: Encoder A and B must be input to pins 36 and 37
 * 
 */
/* ************************************************************************** */

#include <xc.h>
#include <BOARD.h>

#include <QEI.h>
#include <stdio.h>

#define A PORTDbits.RD6 //pin 36
#define B PORTDbits.RD7 //pin 37

typedef enum{
    ONE,
    TWO,
    THREE,
    FOUR
}STATE;

STATE S;
static int QEI_count; //keep track of the count during the encoder's rotation

/**
 * @function QEI_Init(void)
 * @param none
 * @brief  Enables the Change Notify peripheral and sets up the interrupt, anything
 *         else that needs to be done to initialize the module. 
 * @return SUCCESS or ERROR (as defined in BOARD.h)
*/
char QEI_Init(void){
    // INIT Change notify
    CNCONbits.ON = 1; // Change Notify On
    CNENbits.CNEN15 = 1; //enable one phase
    CNENbits.CNEN16 = 1; //enable other phase
    int temp = PORTD; // this is intentional to ensure a interrupt occur immediately upon enabling
    IFS1bits.CNIF = 0; // clear interrupt flag
    IPC6bits.CNIP = 1; //set priority
    IPC6bits.CNIS = 3; // and sub priority
    IEC1bits.CNIE = 1; // enable change notify
    
    //add stuff
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;
    QEI_count = 0; //set QEI_Count to 0
    S = THREE; 
    return SUCCESS;
}

/**
 * @function QEI_GetPosition(void) 
 * @param none
 * @brief This function returns the current count of the Quadrature Encoder in ticks.      
*/
int QEI_GetPosition(void){
    return QEI_count; //return QEI count value
}

/**
 * @Function QEI_ResetPosition(void) 
 * @param  none
 * @return none
 * @brief  Resets the encoder such that it starts counting from 0.
*/
void QEI_ResetPosition(){
    QEI_count = 0; //set count back to 0
    S = THREE; //reset state to THREE
    IFS1bits.CNIF; //clear interrupt flag
}

/*
 * @Function QEI_SM(enum DIR)
 * @param enum DIR
 * @return none
 * @brief State machine for directional states of encoder
 */
void QEI_SM(void){
    switch(S){
        case ONE:
            if(A && ~B){ //forward
                S = TWO;
            }
            if(~A && B){ //backward
                S = FOUR;
            }
            break;
        case TWO:
            if(A && B){ //forward
                QEI_count++; //increment when we go back to both A&B high
                S = THREE;
            }
            if(~A && ~B){ //backward
                S = ONE;
            }
            break;
        case THREE:
            if(~A && B){ //forward
                S = FOUR;
            }
            if(A && ~B){ //backward
                S = TWO;
            }
            break;
        case FOUR:
            if(~A && ~B){ //forward
                S = ONE;
            }
            if(A && B){ //backward
                QEI_count--; //decrement when we go back to both A&B high
                S = THREE;
            }
            break;
        default:
            printf("Error: Encoder State should never default\n");
            break;
    }
}


void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    IFS1bits.CNIF = 0;
    
    //call QEI_SM
    QEI_SM();
}


/* *****************************************************************************
 End of File
 */
