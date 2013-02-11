/**
 * File:   SlaveDriveControl.c
 * Created on January 30, 2013, 3:20 PM
 * <p>
 * This code will be burned onto the two (x2) slave PICs that are used to
 * control the both (left & right) DC motors for the Robo Waiter. This PIC will
 * not drive the motor directly, but instead drive an H-Bridge (motor driver),
 * which will handle and encapsulate the control of the DC motor.
 *
 * @author Nicholas Fragiskatos (Senior Design Team #12 - Robo Waiter)
 */

//=============================================================================
//================= Pin Configuration for PIC24EP32MC202 ======================
//=============================================================================
//
//  Input Digital Pins:
//      RB0 (pin 4)     -   Forward Signal
//      RB1 (pin 5)     -   Reverse Signal
//      RB2 (pin 6)     -   Speed
//
//  Output Digital Pins:
//      RB15(pin 26)    -   Forward/Back Signal
//      OSC1(pin 9)     -   PWM Signal
//
//=============================================================================

//=============================================================================
//  Function:
//  Description:
//  Arguments:
//  Return:
//  Algorithm:
//=============================================================================

//-----------------------------------------------------------------------------
//  Include files.
//-----------------------------------------------------------------------------
#include <p24EP32MC202.h>
#include "Configuration.h"


//-----------------------------------------------------------------------------
//  Defines
//-----------------------------------------------------------------------------
#define DRIVER_PERIOD_US    10
#define CLK_PERIOD          0.135

//-----------------------------------------------------------------------------
//  Function declarations.
//-----------------------------------------------------------------------------
int main(void);
//void configOutputCompare(void);
//void configSPICommunication(void);
int OC1clkT = DRIVER_PERIOD_US/CLK_PERIOD;
//int OC1DCyc = OC1clkT * 0.5;

/**
 * Configure the Output Compare (OC1) that will be used to send the PWMs to the
 * motor drivers.
 *
 * @return void
 */
void configOutputCompare(void)
{
    //-------------------------------------------------------------------------
    //  Setup Output Compare Timer (Timer 3)
    //-------------------------------------------------------------------------
    TMR3            = 0;        // Clear TMR3
    T3CONbits.TON   = 0;        // Turn off TMR3
    T3CONbits.TCKPS = 0b00;     // Set Pre-scalar to 1:8

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC1)
    //-------------------------------------------------------------------------
    OC1CON1             = 0;        // Clear out first configuration register
    OC1CON2             = 0;        // Clear out second configuration register
    OC1CON1bits.OCTSEL  = 0b001;    // Set TMR3 as the source timer.
    OC1CON1bits.OCM     = 0b110;    // Edge aligned PWM mode.
    OC1CON2bits.SYNCSEL = 0x1F;     // Period Control to OC1RS
    OC1RS               = OC1clkT;  // Set period of OC1
    OC1R                = 60;//OC1DCyc;     // Set duty duration of OC1
    RPOR2bits.RP38R     = 0b010000;

    //-------------------------------------------------------------------------
    //  Ready to turn TMR3 on.
    //-------------------------------------------------------------------------
    T3CONbits.TON = 1;
}

/**
 * Configure the Serial Peripheral Interface (SPI) module of the PIC to act as
 * a slave.
 *
 * @return void
 */
void configSPICommunication(void)
{
    //-------------------------------------------------------------------------
    //  Clear the SPI Buffer register
    //-------------------------------------------------------------------------
    SPI1BUF = 0;    // Clear the SPI buffer.

    //-------------------------------------------------------------------------
    //  Configure interrupts for the interface
    //-------------------------------------------------------------------------
    IFS0bits.SPI1IF = 0;    // Clear the interrupt flag
    IEC0bits.SPI1IE = 0;    // Disable interrupt (for now)

    //-------------------------------------------------------------------------
    //  Configure SPI1CON1 register.
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    //  Configure SPI1STAT register.
    //-------------------------------------------------------------------------
}

/**
 * Main loop for the PIC. Purpose is to send appropriate PWMs to the motor
 * drivers based on input received from the master PIC.
 *
 * @return int (But this should never happen)
 */
int main(void) {

    ANSELBbits.ANSB0 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB6 = 0;
    //TRISBbits.TRISB1 = 0;
    configOutputCompare();
    configSPICommunication();


    PORTBbits.RB0 = 1;
    //PORTBbits.RB1 = 1;
    //int i;
    while(1)
    {
        /*
        for (i = 0; i < 30000; ++i)
        {
            Nop();
        }
        OC1R = OC1R + 15;

        if (OC1R >= 60)
        {
            OC1R = 15;
        }
        PORTBbits.RB0 = ~PORTBbits.RB0;
         */
    }
    return 0;
}
