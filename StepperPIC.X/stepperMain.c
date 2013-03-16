/* 
 * File:   stepperMain.c
 * Author: student
 *
 * Created on March 14, 2013, 5:15 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24EP32MC202.h>
#include "spi.h"
#include "stepper.h"
#include "globals.h"
#include "configs.h"
#include "../Global_PIC/spiMessages.h"


#define STEPPER_SPEED SLAVEData.inData[SPI_STEPPER_SPEED]
#define STEPPER_DIR SLAVEData.inData[SPI_STEPPER_DIR]
#define STEPPER_STEPS SLAVEData.inData[SPI_STEPPER_STEPS]
#define STEPPER_MOVING SLAVEData.outData[SPI_STEPPER_MOVING]
#define STEPPER_STEPSINPROG SLAVEData.outData[SPI_STEPPER_INPROG]
#define STEPPER_ENABLED PTCONbits.PTEN
/*
 * 
 */
int main( void ) {
    TRISB=0x0FEF;//For some reason RB4 needs to be an output
    _TRISB5=0;
    initStepper();//intialize stepper
    configSPICommunication();//turn on spi communication
    int currentSpeed=0;//set initial current speed
    while(1){
        if(STEPPER_SPEED!=currentSpeed){ // if speed has changed, change it
            stepperSpeed(STEPPER_SPEED);
            currentSpeed=STEPPER_SPEED;
        }
        //if there are still steps in prog do not start new
        if(STEPPER_STEPS && !global_stepsInProgress && !STEPPER_ENABLED){
            takeSteps(STEPPER_STEPS,STEPPER_DIR);
            STEPPER_STEPS=0;
            STEPPER_MOVING=1;
        }
        if(STEPPER_ENABLED){
            //set the out spi to the steps in progress
            STEPPER_STEPSINPROG=global_stepsInProgress;
        }else{
            STEPPER_MOVING=0;
            STEPPER_STEPSINPROG=0;
        }
    }//endwhile
    return (EXIT_SUCCESS);
}

