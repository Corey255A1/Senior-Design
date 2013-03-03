/* 
 * File:   spi.h
 * Author: student
 *
 * Created on February 28, 2013, 6:26 PM
 */
/**
 * Configure the Serial Peripheral Interface (SPI) module of the PIC to act as
 * a slave.
 *
 * @return void
 */
extern int SPI_msg;
extern int SPI_msg_rdy;
void configSPICommunication(void);
void writeSPI_slave(char);

