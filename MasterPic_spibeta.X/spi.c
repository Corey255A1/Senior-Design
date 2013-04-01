#include "../Global_PIC/spiMessages.h"
#include <p24FJ128GA010.h>
#include "spi.h"
/**
 * SPI MASTER communication setup
 * This will communicate with the slave pics
 *
 */



/**
 * Setup the SPI ports for communication
 */
void initSPI(void){
    COMPASSportEN;
    STEPPER_DRIVERportEN;
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.MODE16 = 1;
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 1;
    LATG=LATG | (1<<COMPASS);
    LATG=LATG | (1<<STEPPER_DRIVER);
    LATG=LATG | (1<<MOTOR_DRIVER);
}//init

/**
 * Take in a byte and write it out spi port
 * @param input - byte to write
 * @return  returns the buffer after sending
 */
int writeSPI1(int input) {
    //TO-DO Write code to pick the slaves and add parameter
    SPI1BUF = input;
    while(!_SPIRBF);
    return SPI1BUF;
}//write
