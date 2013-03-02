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
#include "communication.h"
#include "../Global_PIC/spiMessages.h"


/*
 * 
 */
int main( void ) {
    initPMP();
    initLCD(7);
    initSPI();
    char spi_recv;
    while(1){
        writeSlave(OUT_ON);
        //putLCD('a');
        msDelay(5);
        writeSlave(OUT_OFF);
        //putLCD('b');
        //msDelay(5);
        spi_recv = readSlave(GET_STATE);
        putLCD(spi_recv & 0xFF);
        msDelay(100);
        cmdLCD(0b00000010); //return home??


//        putLCD(spi_recv & 0xFF);
//        cmdLCD(0b00000010); //return home??
//        msDelay(1000);
//        putLCD('c');
//        while(writeSPI1(WRITE_DATA)!= DATA_RDY){msDelay(5);}
//        msDelay(5);
//        spi_recv = writeSPI1(OUT_ON);
//        msDelay(5);
//        writeSPI1(END_TRANSMISSION);
//        msDelay(5);
//        while(writeSPI1(WRITE_DATA)!= DATA_RDY){msDelay(5);}
//        msDelay(5);
//        spi_recv = writeSPI1(OUT_OFF);
//        if(spi_recv == 0xFF){
//            writeLCD("ASS");
//        }
//        msDelay(5);
//        writeSPI1(END_TRANSMISSION);
//        msDelay(1000);
//        while(writeSPI1(READ_DATA)!=DATA_RDY){msDelay(5);}
//        msDelay(5);
//        writeSPI1(GET_STATE);
//        msDelay(5);
//        spi_recv = writeSPI1(READ);
//        cmdLCD(0b00000010); //return home??
//        if(spi_recv)
//            putLCD(spi_recv & 0xFF);
//        writeSPI1(END_TRANSMISSION);
//        msDelay(1000);

    };
    return (EXIT_SUCCESS);
}

