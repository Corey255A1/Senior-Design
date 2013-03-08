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
    int currentTemp;
    while(1){
        writeSlave(SLAVE1,LED_OUT,0x1);
        msDelay(10);
        writeSlave(SLAVE1,LED_OUT,0x0);
        msDelay(10);
        putLCD(readSlave(SLAVE2,CURRENT_TEMPERATURE));
        msDelay(10);
        cmdLCD(0b00000010); //return home??
//        putLCD('A');
//        writeSlave(SLAVE1,LED_OUT,0x1);
//        putLCD('B');
//        writeSlave(SLAVE1,LED_OUT,0x0);
//        putLCD('C');
//        putLCD(readSlave(SLAVE1,CURRENT_TEMPERATURE));
//        cmdLCD(0b00000010); //return home??
    };
    return (EXIT_SUCCESS);
}

