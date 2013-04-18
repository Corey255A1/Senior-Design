/**
 * -- Stepper Main --
 * File:   stepperMain.c
 * Author: Corey Wunderlich
 * Description: This is the main function for converting the spi messages into
 * stepper motor movements. The stepper can be effected by values for speed
 * direction and the amount of steps to take. It will give information back to
 * the mcp with things such as steps left in current motion.
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
#include "A2D.h"
#include "servo.h"
#define RAD_FIXED 8192
//Define the ports for information passing over SPI
//#define STEPPER_SPEED SLAVEData.inData[SPI_STEPPER_SPEED]
//#define STEPPER_DIR SLAVEData.inData[SPI_STEPPER_DIR]
//#define STEPPER_STEPS SLAVEData.inData[SPI_STEPPER_STEPS]
//#define STEPPER_MOVING SLAVEData.outData[SPI_STEPPER_MOVING]
//#define STEPPER_STEPSINPROG SLAVEData.outData[SPI_STEPPER_INPROG]
#define STEPPER_RAD SLAVEData.inData[0]
#define SERVO_POS SLAVEData.inData[1]
#define STEPPER_ENABLED PTCONbits.PTEN //is High (1) when PWM is active
/*
 * 
 */
int main( void ) {
    TRISB=0x0FEF;//For some reason RB4 needs to be an output
    _TRISB5=0;
    initStepper();//intialize stepper
    initADC();
    initServo();
    configSPICommunication();//turn on spi communication
    int currentSpeed=0;//set initial current speed
   // double angle;
    stepperSpeed(55000);
    double stepp;
    stepp = getAngle();
    SERVO_POS = SERVO_MIN;
#ifdef LONGARM
    STEPPER_RAD = (unsigned int)((double)LMAXRAD*(double)RAD_FIXED);
#endif
#ifdef SHORTARM
    STEPPER_RAD = (unsigned int)((double)SHOME*(double)RAD_FIXED);
#endif
    while(1){
//      posRad((double)((unsigned int)22118)/(double)RAD_FIXED);
      posRad((double)((unsigned int)STEPPER_RAD)/(double)RAD_FIXED);
#ifdef SHORTARM
        posServo((unsigned int)SERVO_POS);
#endif
      stepp = getAngle();

    }
//    STEPPER_SPEED=1;
//    while(1){
//        STEPPER_MOVING = 0xAD;
//        if(STEPPER_SPEED>0){
//            takeSteps(100,FWD);
//            while(PTCONbits.PTEN);
//            STEPPER_SPEED=0;
//        }
//
//            int i;
//    int j;
//    while(1){
//        for(i=0;i<165;i++){
//            posServo(i);
//            for(j=0;j<10000;j++){
//                Nop();Nop();
//            }
//        }
//        for(i=165;i>0;i--){
//            posServo(i);
//           for(j=0;j<10000;j++){
//                Nop();Nop();
//            }
//        }
//    }
//        angle = getAngle();
//        posRad(angle);
//       stepperSpeed(25000);
//        takeSteps(2000,FWD);
//        while(PTCONbits.PTEN);
//        posXY(13,8);
//
//        posXY(18,5);
//        posXY(18,15);
         // takeSteps(1000,FWD);
         // while(PTCONbits.PTEN);
//        if(STEPPER_SPEED!=currentSpeed){ // if speed has changed, change it
//            stepperSpeed(STEPPER_SPEED);
//            currentSpeed=STEPPER_SPEED;
//        }
//        //if there are still steps in prog do not start new
//        if(STEPPER_STEPS && !global_stepsInProgress && !STEPPER_ENABLED){
//            takeSteps(STEPPER_STEPS,STEPPER_DIR);
//            STEPPER_STEPS=0;
//            STEPPER_MOVING=1;
//        }
//        if(STEPPER_ENABLED){
//            //set the out spi to the steps in progress
//            STEPPER_STEPSINPROG=global_stepsInProgress;
//        }else{
//            STEPPER_MOVING=0;
//            STEPPER_STEPSINPROG=0;
//        }
//    }//endwhile
    return (EXIT_SUCCESS);
}

