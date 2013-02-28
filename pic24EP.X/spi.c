#include "globals.h"
#include <p24EP32MC202.h>

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
    SPI1CON1bits.DISSCK = EN;       // Internal Serial Clock is enabled.
    SPI1CON1bits.DISSDO = 0;        // SDO1 pin is controlled by the module
    SPI1CON1bits.MODE16 = 0;        // Communication is byte-wide (8-bits)
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
    IFS0bits.SPI1IF = CLEAR; // Clear the interrupt flag.
    IEC0bits.SPI1IE = EN;    // Enable interrupt.
}
