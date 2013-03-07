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
//void writeSPI_slave(char);
enum SPI_STATES  {SPI_RDY, SPI_WriteSlot, SPI_WriteData, SPI_ReadSlot, SPI_ReadData};
typedef struct {
    enum SPI_STATES State;
    char MsgRecv;
    int TxBuffer;
    int RxBuffer;
} SPISTATUS;
typedef struct {
    int outData[255];
    int inData[255];
} SLAVESPIDATA;
extern SPISTATUS SPISTATUSbits;
extern SLAVESPIDATA SLAVEData;
