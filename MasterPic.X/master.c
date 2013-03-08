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
    while(1){
        writeSlave(OUT_ON);
        msDelay(5);
        writeSlave(OUT_OFF);
        msDelay(5);
        putLCD(readSlave(GET_STATE));
        cmdLCD(0b00000010); //return home??
        msDelay(5);

    };
    return (EXIT_SUCCESS);
}

