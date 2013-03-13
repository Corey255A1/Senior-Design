/**
 * --SPI Control--
 * This Is code for SPI SLAVE MODE
 * It will wait for the Clock to start running 
 * and transmit it's buffer along with receive the
 * bits from the MASTER
 * 
 */

#include "globals.h"
#include <p24EP32MC202.h>
#include "spi.h"
#include "../Global_PIC/spiMessages.h"


extern enum SPI_STATES global_spistate;

int SPI_msg;
int SPI_msg_rdy;
int writeSlot=0;
//enum SPI_STATES global_spistate = SPI_RDY;
SPISTATUS SPISTATUSbits;
SLAVESPIDATA SLAVEData;
/**
 * Interrupt Service Routine to handle SPI communication. The ISR will do the
 * following:
 *
 *      1. Read from the SPI buffer.
 *      2. Sets up the the state to either be write or read
 *      3. The sets sets the appropriate send/recv buffer slot
 *      4. The writes or reads the appropriate slot information gathered
 *
 * The SPI state machine steps through each section depending upon a message
 * that has been recieved
 *
 * @return void
 */
void __attribute__((__interrupt__, auto_psv)) _SPI1Interrupt()
{
        //SPISTATUSbits.MsgRecv=1;
        SPISTATUSbits.RxBuffer=SPI1BUF;
        switch(SPISTATUSbits.State){
            case SPI_RDY:
              switch(SPISTATUSbits.RxBuffer){
                case WRITE_DATA:
                    SPISTATUSbits.State = SPI_WriteSlot;
                    SPI1BUF=WRITE_DATA;
                    break;
                case READ_DATA:
                    SPISTATUSbits.State = SPI_ReadSlot;
                    SPI1BUF=READ_DATA;
                    break;
                case 0x5555:
                    SPI1BUF=0x1818;
                    break;
                default:
                    SPI1BUF=0xFFFF;
              }
              break;
            case SPI_WriteSlot:
                writeSlot=SPISTATUSbits.RxBuffer;
                SPI1BUF = SPISTATUSbits.RxBuffer;
                SPISTATUSbits.State = SPI_WriteData;
                break;
            case SPI_WriteData:
                SLAVEData.inData[writeSlot]=SPISTATUSbits.RxBuffer;
                SPI1BUF = DATA_RDY;
                SPISTATUSbits.State = SPI_RDY;
                break;
            case SPI_ReadSlot:
                SPI1BUF=SLAVEData.outData[SPISTATUSbits.RxBuffer];
                SPISTATUSbits.State=SPI_ReadData;
                break;
            case SPI_ReadData:
                SPI1BUF = DATA_RDY;
                SPISTATUSbits.State = SPI_RDY;
                break;
        }//endswtich
    IFS0bits.SPI1IF = CLEAR;    // Clear interrupt flag
}//end interrupt

/**
 * Configure the Serial Peripheral Interface (SPI) module of the PIC to act as
 * a slave.
 *
 * @return void
 */
void configSPICommunication(void)
{
    //-------------------------------------------------------------------------
    //  1. Clear the SPI Buffer register
    //-------------------------------------------------------------------------
    SPI1BUF = CLEAR;    // Clear the SPI buffer.

    //-------------------------------------------------------------------------
    //  2. Initial configuration for the interface interrupts.
    //-------------------------------------------------------------------------
    IFS0bits.SPI1IF = CLEAR;    // Clear the interrupt flag
    IEC0bits.SPI1IE = DISABLE;  // Disable interrupt (for now)

    //-------------------------------------------------------------------------
    //  3. Configure SPI1CON1 register.
    //-------------------------------------------------------------------------
    SPI1CON1bits.DISSCK = 0;       // Internal Serial Clock is enabled.
    SPI1CON1bits.DISSDO = 0;        // SDO1 pin is controlled by the module
    SPI1CON1bits.MODE16 = 1;        // Communication is byte-wide (16-bits)
    SPI1CON1bits.SMP = 0;           // Input data is sampled at the middle of
                                    // data output time.
    SPI1CON1bits.CKE = 1;           // Serial output data changes on transition
                                    // from active clock state to idle clock
                                    // state.
    SPI1CON1bits.SSEN = 1;          // Slave Select 1 (SS1-bar) is used for
                                    // slave mode.
    SPI1CON1bits.CKP = 0;           // Active high clock.
    SPI1CON1bits.MSTEN = DISABLE;   // Master mode disabled.

    //-------------------------------------------------------------------------
    //  4. Configure SPI1STAT register.
    //-------------------------------------------------------------------------
    SPI1STATbits.SPIROV = CLEAR;// No receive overflow has occured
    SPI1STATbits.SPIEN = EN;    // Enable SPI module

    //-------------------------------------------------------------------------
    //  Final configuration for the interface interrupts.
    //-------------------------------------------------------------------------
    SPI1BUF=DATA_RDY;
    IFS0bits.SPI1IF = CLEAR; // Clear the interrupt flag.
    IEC0bits.SPI1IE = EN;    // Enable interrupt.
}