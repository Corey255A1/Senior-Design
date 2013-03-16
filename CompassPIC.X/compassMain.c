/* 
 * File:   compassMain.c
 * Author: Corey Wunderlich
 * Description: Basically main for the compass, continuously polls the I2C
 * compass and stores the calculated heading value into the SPI send/receive
 * buffer for processing by the master pic. 
 *
 * Created on March 15, 2013, 5:26 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "configs.h"
#include "9axis.h"
#include "globals.h"
#include "i2c.h"
#include "spi.h"
#include "../Global_PIC/spiMessages.h"

/*
 * Defines
 */
#define HEADING SLAVEData.outData[0] // The port that will be read by mcp
#define FIXEDPOINT_13BIT 8192 // the Shifting value to pass the radians value
                              // as a fixed point integer.
int main( void ) {
    //i2c_Init();
    //init9axis();
    configSPICommunication();
    double heading_d;
    //Use 1 sign bit and 2 integer bits and therefore 13 fraction bits it goes from -pi->pi
    //This means say 3.14 << 14bits
    while(1){
       heading_d = -3.1412;//updateHeading();
       HEADING = (int) (heading_d*FIXEDPOINT_13BIT); //cast as an integer after shifting 14
    }//end while
    return (EXIT_SUCCESS);
}

