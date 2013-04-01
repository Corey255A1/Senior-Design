/**
 * == Communication ==
 * Provides functions to read and write our slave PICs according to the
 * determined protocol
 * --Corey
 */
#include <p24FJ128GB106.h>
#include "spi.h"
#include "communication.h"
#include "globals.h"
#include "../../Senior-Design/Global_PIC/spiMessages.h"

/**
 * This function writes a message to the selected slave
 * It takes care of the appropriate chip select
 * @param slave -- the slave to be communicated with
 * @param msg -- the message to be sent
 * @return -- the message sent back from the slave
 */
int writeSPI_master(int slave,int msg){
    PORTD = PORTD & ~(1<<slave);
    int feedback;
    feedback = writeSPI1(msg);
    PORTD = PORTD | (1<<slave);
    return feedback;
}

/**
 * This function writes and communicates the selected slave
 * @param slave -- slave to communicate with
 * @param mem -- the location on the slave to write
 * @param value -- the value to write
 */
void writeSlave(int slave, int mem, int value){
    writeSPI_master(slave,WRITE_DATA);
    msDelay(20);
    writeSPI_master(slave,mem);
    msDelay(20);
    writeSPI_master(slave,value);
}

/**
 * This sets up the slave for read mode and reads a value
 * for the select memory location on the slave
 * @param slave -- the pic to communicate with
 * @param toRead -- the location of which to read
 * @return
 */
int readSlave(int slave, int toRead){
    int msg=0;
    writeSPI_master(slave,READ_DATA);
    msDelay(20);
    writeSPI_master(slave,toRead);
    msDelay(20);
    msg=writeSPI_master(slave,READ);
    return msg;
}