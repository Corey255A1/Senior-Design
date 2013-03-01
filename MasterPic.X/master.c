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


/*
 * 
 */
int main( void ) {
    initPMP();
    initLCD(7);
    initSPI();
    int spi_recv;
    while(1){
        writeSPI1(WRITE_DATA);
        msDelay(1);
        spi_recv = writeSPI1(OUT_ON);
        if(spi_recv == 0xFF){
            writeLCD("ASS");
        }
        msDelay(5);
        writeSPI1(END_TRANSMISSION);
        msDelay(5);
        writeSPI1(WRITE_DATA);
        msDelay(5);
        spi_recv = writeSPI1(OUT_OFF);
        if(spi_recv == 0xFF){
            writeLCD("ASS");
        }
        msDelay(5);
        writeSPI1(END_TRANSMISSION);
        msDelay(5);
    };
    return (EXIT_SUCCESS);
}

