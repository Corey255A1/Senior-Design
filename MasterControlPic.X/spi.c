/**
 * == SPI MASTER ==
 * This provides functions to communicate over SPI
 *
 */

#include "../../Senior-Design/Global_PIC/spiMessages.h"
#include <p24FJ128GB106.h>
#include "spi.h"


/**
 * Setup the SPI ports for communication
 */
void initSPI(void){
    //Setup all of the Slave Select Lines
    MOTOR_DRIVERportEN;
    SENSOR_BOARDportEN;
    STEPPER_DRIVER1portEN;
    STEPPER_DRIVER2portEN;
    STEPPER_DRIVER3portEN;
    S_ULTRASportEN;
    FB_ULTRASportEN;
    
    //Set the SPI ports
    RPOR1bits.RP2R = 8;

    RPOR1bits.RP3R = 7;
    RPINR20bits.SDI1R = 4;

    //Setup SPI
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.MODE16 = 1;
    SPI1STATbits.SPIROV = 0;
    SPI1STATbits.SPIEN = 1;

    //Set Slave selects to HIGH
    LATD=LATD | (1<<MOTOR_DRIVER);
    LATD=LATD | (1<<SENSOR_BOARD);
    LATD=LATD | (1<<STEPPER_DRIVER1);
    LATD=LATD | (1<<STEPPER_DRIVER2);
    LATD=LATD | (1<<STEPPER_DRIVER3);
    LATD=LATD | (1<<S_ULTRAS);
    LATD=LATD | (1<<FB_ULTRAS);
}//init

/**
 * Take in a byte and write it out spi port
 * @param input - byte to write
 * @return  returns the buffer after sending
 */
int writeSPI1(int input) {
    //TO-DO Write code to pick the slaves and add parameter
    SPI1BUF = input;
    while(!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}//write
