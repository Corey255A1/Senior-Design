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


/*
 * 
 */
int main( void ) {
    initPMP();
    initLCD(7);
    initSPI();
    writeLCD("Dan is sexy");
    while(1){
        writeSPI1(0xAA);
        msDelay(500);
        writeSPI1(0x7F);
        msDelay(500);
    };
    return (EXIT_SUCCESS);
}

