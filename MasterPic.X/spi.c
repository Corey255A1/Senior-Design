/**
 * SPI MASTER communication setup
 * This will communicate with the slave pics
 *
 */


#include "../Global_PIC/spiMessages.h"
#include <p24FJ128GA010.h>

/**
 * Setup the SPI ports for communication
 */
void initSPI(void){
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.MODE16 = 0;
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 1;
    PORTD=0x0000;
    TRISDbits.TRISD12=0;
}//init

/**
 * Take in a byte and write it out spi port
 * @param input - byte to write
 * @return  returns the buffer after sending
 */
char writeSPI1(int input) {
    //TO-DO Write code to pick the slaves and add parameter
    SPI1BUF = 0x00FF & input;
    while(!_SPIRBF);
    return 0x00FF & SPI1BUF;
}//write
