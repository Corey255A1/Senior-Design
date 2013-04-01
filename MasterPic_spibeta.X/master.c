/* 
 * File:   master.c
 * Author: student
 *
 * Created on February 28, 2013, 5:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24FJ128GA010.h>
#include "lcd.h"
#include "spi.h"
#include "globals.h"
#include "serialcomm.h"

#include "../Global_PIC/spiMessages.h"
#include "communication.h"


/*
 * 
 */
int main( void ) {
    initPMP();
    initLCD(7);
    initSerial2();

//    initSPI();
    cmdLCD(0b00000010); //return home??
    //writeLCD("hello");

    writeLCD("Begin");

//    int tempStepperCounts[5];
//    int heading_fp; // remember this is in fixed point from spi; 2,14
//    double heading;
//    int i=0;
    while(1){
        if(newMSG){
          cmdLCD(0b00000010); //return home??
          writeLCD("                ");
          cmdLCD(0b00000010); //return home??
          writeLCD(msg);
          txSerial("ASS!");
          newMSG = 0;
        }
//        writeLCD("FWD...");
//        writeSlave(MOTOR_DRIVER,SPI_MOTOR,(MOTOR_FWD SPI_MOTOR_RIGHT_DIR) |
//                                      (MOTOR_FULL_SPEED SPI_MOTOR_RIGHT_SPEED));
//        writeSlave(STEPPER_DRIVER,SPI_STEPPER_DIR,'F');
//        writeSlave(STEPPER_DRIVER,SPI_STEPPER_SPEED,30000);
//        writeSlave(STEPPER_DRIVER,SPI_STEPPER_STEPS,1500);
//        for(i=1;i<5000;i+=500)
//            writeSlave(STEPPER_DRIVER,SPI_STEPPER_SPEED,30000-i);
   
//        msDelay(1500);
//        writeSlave(MOTOR_DRIVER,SPI_MOTOR,(MOTOR_REV SPI_MOTOR_RIGHT_DIR) |
//                                            (MOTOR_STOP SPI_MOTOR_RIGHT_SPEED));
//        msDelay(1500);
//        writeLCD("REV...");
//        writeSlave(MOTOR_DRIVER,SPI_MOTOR,(MOTOR_REV SPI_MOTOR_RIGHT_DIR) |
//                                      (MOTOR_FULL_SPEED SPI_MOTOR_RIGHT_SPEED));
//        writeSlave(STEPPER_DRIVER,SPI_STEPPER_DIR,'R');
//        writeSlave(STEPPER_DRIVER,SPI_STEPPER_SPEED,25000);
//        writeSlave(STEPPER_DRIVER,SPI_STEPPER_STEPS,1000);
//        msDelay(500);
//        tempStepperCounts[0] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
//        tempStepperCounts[1] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
//        tempStepperCounts[2] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
//        msDelay(100);
//        tempStepperCounts[3] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
//        msDelay(100);
//        tempStepperCounts[4] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
//        msDelay(1500);
//        writeSlave(MOTOR_DRIVER,SPI_MOTOR,(MOTOR_REV SPI_MOTOR_RIGHT_DIR) |
//                                            (MOTOR_STOP SPI_MOTOR_RIGHT_SPEED));
//        msDelay(1500);
////        heading_fp = readSlave(COMPASS,COMPASS_HEADING);
////        heading = ((double) heading_fp) / FIXEDPOINT_13BIT;
//        cmdLCD(0b00000010); //return home??
//        writeLCD("               ");
//        cmdLCD(0b00000010); //return home??
    };
    return (EXIT_SUCCESS);
}

