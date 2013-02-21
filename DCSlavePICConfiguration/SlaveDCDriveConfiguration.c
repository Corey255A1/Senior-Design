#include <p24EP32MC202.h>



/**
 * Configure the Output Compare (OC1) that will be used to send the PWMs to the
 * motor drivers.
 *
 * @return void
 */
int configOutputCompare(int clkPeriod, int clkDutyCycle)
{
    //-------------------------------------------------------------------------
    //  Setup Output Compare Timer (Timer 3)
    //-------------------------------------------------------------------------
    TMR3            = 0;        // Clear TMR3
    T3CONbits.TON   = 0;  // Turn off TMR3
    T3CONbits.TCKPS = 0b00;     // Set Pre-scalar to 1:1

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC1)
    //-------------------------------------------------------------------------
    OC1CON1             = 0;            // Clear out first configuration
                                        // register
    OC1CON2             = 0;            // Clear out second configuration
                                        // register
    OC1CON1bits.OCTSEL  = 0b001;        // Set TMR3 as the source timer.
    OC1CON1bits.OCM     = 0b110;        // Edge aligned PWM mode.
    OC1CON2bits.SYNCSEL = 0x1F;         // Period Control to OC1RS
    OC1RS               = clkPeriod;    // Set period of OC1
    OC1R                = clkDutyCycle; // Set duty duration of OC1
    RPOR2bits.RP38R     = 0b010000; // Maps the OC1 output to the RP38R pin
                                    // (pin 15) on the pick. The 0b010000 is
                                    // defined in the "Output Mapping" section
                                    // of the data sheet.

    //-------------------------------------------------------------------------
    //  Ready to turn TMR3 on.
    //-------------------------------------------------------------------------
    T3CONbits.TON = 1;

    return 1;
}
