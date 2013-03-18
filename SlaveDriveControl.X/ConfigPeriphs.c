/**
 * File:   ConfigPeriphs.c
 * Created on March 13, 2013, 2:40 PM
 * <p>
 * These are helper functions to setup and configure the slave pic that is used
 * for drive control. As of right now, we have functions to setup the following:
 * <p>
 * - Output Compare
 * - Device Pins
 * - Input Captures
 *
 * @author Nicholas Fragiskatos (Senior Design Team #12 - Robo Waiter)
 */
#include "ConfigPeriphs.h"

/**
 * Configure the Output Compare (OC1) that will be used to send the PWMs to the
 * motor drivers.
 *
 * @return void
 */
void configOutputCompare(void)
{
    //-------------------------------------------------------------------------
    //  Setup Output Compare Timer 1 (Timer 3)
    //-------------------------------------------------------------------------
    TMR3            = 0;        // Clear TMR3
    T3CONbits.TON   = DISABLE;  // Turn off TMR3
    T3CONbits.TCKPS = 0b00;     // Set Pre-scalar to 1:1

    //-------------------------------------------------------------------------
    //  Setup Output Compare Timer 2 (Timer 2)
    //-------------------------------------------------------------------------
    TMR2            = 0;        // Clear TMR2
    T2CONbits.TON   = DISABLE;  // Turn off TMR2
    T2CONbits.TCKPS = 0b00;     // Set Pre-scalar to 1:1

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC1)
    //-------------------------------------------------------------------------
    OC1CON1             = CLEAR;    // Clear out first configuration register
    OC1CON2             = CLEAR;    // Clear out second configuration register
    OC1CON1bits.OCTSEL  = 0b001;    // Set TMR3 as the source timer.
    OC1CON1bits.OCM     = 0b110;    // Edge aligned PWM mode.
    OC1CON2bits.SYNCSEL = 0x1F;     // Period Control to OC1RS
    OC1RS               = CLK_PER;  // Set period of OC1
    OC1R                = 0;        // Set duty duration of OC1
    RPOR1bits.RP37R     = 0b010001; // Maps the OC1 output to the RP37R pin
                                    // (pin 14) on the pick. The 0b010001 is
                                    // defined in the "Output Mapping" section
                                    // of the data sheet.

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC2)
    //-------------------------------------------------------------------------
    OC2CON1             = CLEAR;    // Clear out first configuration register
    OC2CON2             = CLEAR;    // Clear out second configuration register
    OC2CON1bits.OCTSEL  = 0b000;    // Set TMR2 as the source timer.
    OC2CON1bits.OCM     = 0b110;    // Edge aligned PWM mode.
    OC2CON2bits.SYNCSEL = 0x1F;     // Period Control to OC2RS
    OC2RS               = CLK_PER;  // Set period of OC2
    OC2R                = 0;        // Set duty duration of OC2
    RPOR2bits.RP38R     = 0b010000; // Maps the OC2 output to the RP38R pin
                                    // (pin 15) on the pick. The 0b010000 is
                                    // defined in the "Output Mapping" section
                                    // of the data sheet.

    //-------------------------------------------------------------------------
    //  Ready to turn TMR3 and TMR2 on.
    //-------------------------------------------------------------------------
    T3CONbits.TON = EN;
    T2CONbits.TON = EN;
}


/**
 * Setup the pin configurations (I/O)
 *
 * @return void
 */
void configDevicePins(void)
{
    //-------------------------------------------------------------------------
    //  Setup Pins for H-Bridge Direction Control
    //-------------------------------------------------------------------------
    ANSELAbits.ANSA0 = DIGITAL; // Change RA0 pin to digital
    ANSELAbits.ANSA1 = DIGITAL; // Change RA1 pin to digital
    ANSELAbits.ANSA4 = DIGITAL; // Change RA4 pin to digital

    TRISAbits.TRISA0 = OUTPUT;  // Change RA0 pin to output
    TRISAbits.TRISA1 = OUTPUT;  // Change RA1 pin to output
    TRISAbits.TRISA4 = OUTPUT;  // Change RA4 pin to output
    TRISBbits.TRISB4 = OUTPUT;  // Change RB4 pin to output
    TRISBbits.TRISB11= OUTPUT;  // Change RB11 pin to output


}

/**
 * Configure input captures for all four feedback lines coming from the Hall
 * sensor of the two motors (2 feedback lines for each motor)
 * <p>
 * Input Capture 1 is the direction motor 1 is turning (forward or reverse)
 * Input Capture 2 is the speed of motor 1
 * Input Capture 3 is the direction motor 2 is turning (forward or reverse)
 * Input Capture 4 is the speed of motor 2
 */
void configInputCaptures()
{
    //-------------------------------------------------------------------------
    //  Setup Input Compare Timer (Timer 4) for all 4 Input Compares
    //-------------------------------------------------------------------------
    TMR4            = 0;        // Clear TMR4
    T4CONbits.TON   = DISABLE;  // Turn off TMR4
    T4CONbits.TCKPS = 0b00;     // Set Pre-scalar to 1:1

    //-------------------------------------------------------------------------
    //  Input Capture 1 (Motor 1 Feedback-A)
    //-------------------------------------------------------------------------
    M1FDBCKA_TRISREG    = INPUT;            // Set pin to input
    RPINR7bits.IC1R     = M1FDBCKA_RPIPORT; // Set IC input to RPI input
    IC1TMR              = 0;                // Clear the IC1 Timer
    IC1CON1             = 0;                // Clear the config 1 register
    IC1CON2             = 0;                // Clear the config 2 register
    IPC0bits.IC1IP      = 1;                // Set IC1 Interrupt Priority to 1
    IFS0bits.IC1IF      = 0;                // Clear the IC1 interrupt flag
    IC1CON1bits.ICTSEL  = 0b010;            // Set IC1 to TMR4
    IC1CON1bits.ICI     = 0;                // Interrupt on every capture
    IC1CON1bits.ICM     = 0x001;            // Capture on every edge trigger

    //-------------------------------------------------------------------------
    //  Input Capture 2 (Motor 1 Feedback-B)
    //-------------------------------------------------------------------------
    M1FDBCKB_TRISREG    = INPUT;            // Set pin to input
    RPINR7bits.IC2R     = M1FDBCKB_RPIPORT; // Set IC input to RPI input
    IC2TMR              = 0;                // Clear the IC2 Timer
    IC2CON1             = 0;                // Clear the config 1 register
    IC2CON2             = 0;                // Clear the config 2 register
    IPC1bits.IC2IP      = 1;                // Set IC2 Interrupt Priority to 1
    IFS0bits.IC2IF      = 0;                // Clear the IC2 interrupt flag
    IC2CON1bits.ICTSEL  = 0b010;            // Set IC2 to TMR4
    IC2CON1bits.ICI     = 0;                // Interrupt on every capture
    IC2CON1bits.ICM     = 0x001;            // Capture on every edge trigger

    //-------------------------------------------------------------------------
    //  Input Capture 3 (Motor 2 Feedback-A)
    //-------------------------------------------------------------------------
    M2FDBCKA_TRISREG    = INPUT;            // Set pin to input
    RPINR8bits.IC3R     = M2FDBCKA_RPIPORT; // Set IC input to RPI input
    IC3TMR              = 0;                // Clear the IC3 Timer
    IC3CON1             = 0;                // Clear the config 1 register
    IC3CON2             = 0;                // Clear the config 2 register
    IPC9bits.IC3IP      = 1;                // Set IC3 Interrupt Priority to 1
    IFS2bits.IC3IF      = 0;                // Clear the IC3 interrupt flag
    IC3CON1bits.ICTSEL  = 0b010;            // Set IC3 to TMR4
    IC3CON1bits.ICI     = 0;                // Interrupt on every capture
    IC3CON1bits.ICM     = 0x001;            // Capture on every edge trigger

    //-------------------------------------------------------------------------
    //  Input Capture 4 (Motor 2 Feedback-B)
    //-------------------------------------------------------------------------
    M2FDBCKB_TRISREG    = INPUT;            // Set pin to input
    RPINR8bits.IC4R     = M2FDBCKA_RPIPORT; // Set IC input to RPI input
    IC4TMR              = 0;                // Clear the IC3 Timer
    IC4CON1             = 0;                // Clear the config 1 register
    IC4CON2             = 0;                // Clear the config 2 register
    IPC9bits.IC4IP      = 1;                // Set IC4 Interrupt Priority to 1
    IFS2bits.IC4IF      = 0;                // Clear the IC4 interrupt flag
    IC4CON1bits.ICTSEL  = 0b010;            // Set IC4 to TMR4
    IC4CON1bits.ICI     = 0;                // Interrupt on every capture
    IC4CON1bits.ICM     = 0x001;            // Capture on every edge trigger

    //-------------------------------------------------------------------------
    //  Turn the stuff on
    //-------------------------------------------------------------------------
    IEC0bits.IC1IE  = 1;    // Enable IC1 interrupts
    IEC0bits.IC2IE  = 1;    // Enable IC2 interrupts
    IEC2bits.IC3IE  = 1;    // Enable IC3 interrupts
    IEC2bits.IC4IE  = 1;    // Enable IC4 interrupts
    T4CONbits.TON   = 1;    // Turn on Timer 4
}