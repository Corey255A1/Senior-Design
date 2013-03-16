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

#include "../Global_PIC/spiMessages.h"
#include "communication.h"


/*
 * 
 */
int main( void ) {
    initPMP();
    initLCD(7);
    initSPI();
    cmdLCD(0b00000010); //return home??
    int tempStepperCounts[5];
    int heading_fp; // remember this is in fixed point from spi; 2,14
    double heading;
    while(1){
        writeLCD("FWD...");
        writeSlave(STEPPER_DRIVER,SPI_STEPPER_DIR,'F');
        writeSlave(STEPPER_DRIVER,SPI_STEPPER_SPEED,30000);
        writeSlave(STEPPER_DRIVER,SPI_STEPPER_STEPS,1000);
        msDelay(3000);
        writeLCD("REV...");
        writeSlave(STEPPER_DRIVER,SPI_STEPPER_DIR,'R');
        writeSlave(STEPPER_DRIVER,SPI_STEPPER_SPEED,25000);
        writeSlave(STEPPER_DRIVER,SPI_STEPPER_STEPS,1000);
        msDelay(500);
        tempStepperCounts[0] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
        tempStepperCounts[1] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
        tempStepperCounts[2] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
        msDelay(100);
        tempStepperCounts[3] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
        msDelay(100);
        tempStepperCounts[4] = readSlave(STEPPER_DRIVER,SPI_STEPPER_INPROG);
        msDelay(3000);
        heading_fp = readSlave(COMPASS,COMPASS_HEADING);
        heading = ((double) heading_fp) / FIXEDPOINT_13BIT;
        cmdLCD(0b00000010); //return home??
        writeLCD("               ");
        cmdLCD(0b00000010); //return home??
    };
    return (EXIT_SUCCESS);
}

