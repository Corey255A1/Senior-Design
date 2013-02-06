/*
 * File:   SlaveDriveControl.c
 * Author: Nicholas
 *
 * Created on January 30, 2013, 3:20 PM
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
void configOutputCompare(void);
int OC1clkT = DRIVER_PERIOD_US/CLK_PERIOD;
//int OC1DCyc = OC1clkT * 0.5;

//=============================================================================
//  Function:       main()
//  Description:    Main loop for the PIC. Purpose is to send appropriate
//                  PWM to the motor driver based on input received from
//                  the master PIC.
//  Arguments:      - void
//  Return:         - int (Should never return)
//  Algorithm:
//=============================================================================
int main(void) {

    ANSELBbits.ANSB0 = 0;
    TRISBbits.TRISB0 = 0;
    configOutputCompare();

    PORTBbits.RB0 = 1;
    while(1)
    {
        int i;
        for (i = 0; i < 20000; ++i)
        {
            Nop();
        }
        PORTBbits.RB0 = ~PORTBbits.RB0;
    }
    return 0;
}

//=============================================================================
//  Function:       configOutputCompare()
//  Description:    Configure the Output Compare (OC1) that we will be using
//                  to send the PWMs to the motor drivers.
//  Arguments:      - void
//  Return:         - void
//  Algorithm:      - none
//=============================================================================
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
    OC1CON2bits.SYNCSEL = 0b01101;  // Period Control to OC1RS
    OC1RS               = OC1clkT;  // Set period of OC1
    OC1R                = 30;//OC1DCyc;     // Set duty duration of OC1

    //-------------------------------------------------------------------------
    //  Ready to turn TMR3 on.
    //-------------------------------------------------------------------------
    T3CONbits.TON = 1;
}
