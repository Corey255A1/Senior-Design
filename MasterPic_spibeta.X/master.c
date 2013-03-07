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
        putLCD('A');
        writeSlave(1,LED_OUT,0x1);
        msDelay(5);
        putLCD('B');
        writeSlave(1,LED_OUT,0x0);
        msDelay(5);
        putLCD('C');
        msDelay(5);
        putLCD(readSlave(1,CURRENT_TEMPERATURE));
        msDelay(5);
        cmdLCD(0b00000010); //return home??
    };
    return (EXIT_SUCCESS);
}

