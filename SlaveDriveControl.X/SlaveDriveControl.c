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
//============================= SPI Protocol ==================================
//=============================================================================
//
//  The SPI communication is word-wide (16-bits or 2 characters)
//
//  I'm assuming the master is going to send characters (C1 and C2)
//
//      - C1 will be used to set the speed of the motor (ie, the duty cycle).
//          - There will be an arbitrary total of 10 different speeds
//              - Speed 0 corresponds with no movement. (0% Duty Cycle)
//              - Speed 9 corresponds with fastest movement (90% Duty Cycle)
//      - C2 will be used as a forward or reverse.
//          - The forward or reverse character will be used

//=============================================================================
//================= Pin Configuration for PIC24EP32MC202 ======================
//=============================================================================
//
//  Input Digital Pins:
//      MCLR (pin 1)    -   Programming Pin (Master Clear)
//      PGEC1(pin 6)    -   Programming Pin
//      PGED1(pin 7)    -   Programming Pin
//      SCK1(pin 16)    -   Shift Clock Input (SPI)
//      SDI1(pin 18)    -   Serial Data Input (SPI)
//      SS11(pin 21)    -   Slave Select (SPI)
//      RB11(pin 22)    -   H-Bridge Enable
//      RPI44(pin 23)   -   Input Capture Motor 2B
//      RPI45(pin 24)   -   Input Capture Motor 2A
//      RPI46(pin 25)   -   Input Capture Motor 1B
//      RPI47(pin 26)   -   Input Capture Motor 1A
//
//  Output Digital Pins:
//      RA2 (pin 2)     -   H-Bridge 1 Forward Enable
//      RA3 (pin 3)     -   H-Bridge 1 Reverse Enable
//      RB4 (pin 4)     -   H-Bridge 2 Forward Enable
//      RA4 (pin 5)     -   H-Bridge 2 Reverse Enable
//      RP37(pin14)     -   OC1 PWM Output
//      RP38(pin 15)    -   OC2 PWM Output
//      SDO1(pin 17)    -   Serial Data Output (SPI)
//
//  Voltages and Ground:
//      Vss (pin 8)
//      Vdd (pin 13)
//      Vss (pin 19)
//      Vcap(pin 20)
//      AVss(pin 27)d
//      AVdd(pin 28)
//
//
//      ---------------------------
//      |1                      28|
//      |                         |
//      |2                      27|
//      |                         |
//      |3                      26|
//      |                         |
//      |4                      25|
//      |                         |
//      |5                      24|
//      |                         |
//      |6                      23|
//      |                         |
//      |7                      22|
//      |                         |
//      |8                      21|
//      |                         |
//      |9                      20|
//      |                         |
//      |10                     19|
//      |                         |
//      |11                     18|
//      |                         |
//      |12                     17|
//      |                         |
//      |13                     16|
//      |                         |
//      |14                     15|
//      |-------------------------|
//=============================================================================

//-----------------------------------------------------------------------------
//  Include files.
//-----------------------------------------------------------------------------
#include <p24EP32MC202.h>
#include "SlaveDriveControl.h"
#include "Configuration.h"
#include "globals.h"
#include "../pic24EP_spibeta.X/spi.h"

/**
 * Main loop for the PIC. Purpose is to send appropriate PWMs to the motor
 * drivers based on input received from the master PIC.
 *
 * @return int (But this should never happen)
 */
int main(void) {

    configDevicePins();
    configOutputCompare();
    configSPICommunication();
    //configInputCaptures();
    char forwardDirM1;
    char speedM1;
    char forwardDirM2;
    char speedM2;

    DRIVE_EN        = EN;
    SPEEDM1         = 70;
    SPEEDM2         = 0;
    msgQueued       = EN;
    int curConfig   = INMSG;
    M1FWD           = 1;
    M1REV           = 1;
    M2FWD           = 1;
    M2REV           = 1;

    while(1)
    {


        //---------------------------------------------------------------------
        //  If a message needs decoding...
        //---------------------------------------------------------------------
        if (curConfig != INMSG)
        {
            curConfig = INMSG;
                    
            //-----------------------------------------------------------------
            //  Here we can parse the speed and direction from the
            //  message received.
            //
            //  Bits (3-0) are the bits for direction on Motor 1
            //  Bits (7-4) are the bits for speed on Motor 1
            //  Bits (11-8) are the bits for direction on Motor 2
            //  Bits (15-12) are the bits for speed on Motor 2
            //-----------------------------------------------------------------
            forwardDirM1    = 0x000F & curConfig;
            speedM1         = (0x00F0 & curConfig) >> 4;
            forwardDirM2    = (0x0F000 & curConfig) >> 8;
            speedM2         = (0xF0000 & curConfig) >> 12;


            //-----------------------------------------------------------------
            //  The speed is based on the percentage of the duty cycle.
            //
            //      Speed 0: 0% (i.e, not moving)
            //      Speed 1: 10%
            //      Speed 2: 20%
            //      Speed 3: 30%
            //      Speed 4: 40%
            //      Speed 5: 50%
            //      Speed 6: 60%
            //      Speed 7: 70%
            //      Speed 8: 80%
            //      Speed 9: 90%
            //      Speed 10: 100% (Max Speed)
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            //  If Motor 1 is going forward...
            //-----------------------------------------------------------------
            if (forwardDirM1 == 0x01)
            {
                //-------------------------------------------------------------
                //  - Disable the reverse signal for Motor 1. (Safe Check)
                //  - Enable the forward signal for Motor 1.
                //-------------------------------------------------------------
                M1REV = 0;
                M1REV = 0;
                M1FWD = 1;

            //-----------------------------------------------------------------
            //  ... Else Motor 1 is in reverse...
            //-----------------------------------------------------------------
            } else {
                //-------------------------------------------------------------
                //  - Disable the forward signal for Motor 1. (Safe Check)
                //  - Enable the reverse signal for Motor 1.
                //-------------------------------------------------------------
                M1FWD = 0;
                M1FWD = 0;
                M1REV = 1;
            }

            //-----------------------------------------------------------------
            //  If Motor 2 is going forward...
            //-----------------------------------------------------------------
            if (forwardDirM2 == 0x01)
            {
                //-------------------------------------------------------------
                //  - Disable the reverse signal for Motor 2. (Safe Check)
                //  - Enable the forward signal for Motor 2.
                //-------------------------------------------------------------
                M2REV = DISABLE;
                M2FWD = EN;

            //-----------------------------------------------------------------
            //  ... Else Motor 2 is in reverse...
            //-----------------------------------------------------------------
            } else {
                //-------------------------------------------------------------
                //  - Disable the forward signal for Motor 2. (Safe Check)
                //  - Enable the reverse signal for Motor 2.
                //-------------------------------------------------------------
                M2FWD = DISABLE;
                M2REV = EN;
            }

            //-------------------------------------------------------------
            //  Set the speed of both motors.
            //-------------------------------------------------------------
            SPEEDM1 = (speedM1*OC1clkT)/10;
            SPEEDM2 = (speedM2*OC1clkT)/10;
            //++spiReadVal;
            //msgQueued = CLEAR;
        }


    }
    return 0;
}

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
    OC1RS               = OC1clkT;  // Set period of OC1
    OC1R                = 0;        // Set duty duration of OC1
    RPOR1bits.RP37R     = 0b010000; // Maps the OC1 output to the RP37R pin
                                    // (pin 15) on the pick. The 0b010000 is
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
    OC2RS               = OC1clkT;  // Set period of OC2
    OC2R                = 0;        // Set duty duration of OC2
    RPOR2bits.RP38R     = 0b010001; // Maps the OC2 output to the RP38R pin
                                    // (pin 15) on the pick. The 0b010001 is
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
    //ANSELAbits.ANSA0 = DIGITAL; // Change RA0 pin to digital
    //ANSELAbits.ANSA1 = DIGITAL; // Change RA1 pin to digital
    //ANSELBbits.ANSB1 = DIGITAL; // Change RB1 pin to digital

    //TRISAbits.TRISA0 = OUTPUT;  // Change RA0 pin to output
    //TRISAbits.TRISA1 = OUTPUT;  // Change RA1 pin to output
    //TRISBbits.TRISB1 = OUTPUT;  // Change RB1 pin to output
    //TRISBbits.TRISB4 = OUTPUT;  // Change RB4 pin to output
    TRISBbits.TRISB11= OUTPUT;  // Change RB11 pin to output



    ANSELAbits.ANSA4 = DIGITAL; // Change RA4 pin to digital

    TRISAbits.TRISA2 = OUTPUT;  // Change RA2 pin to output
    TRISAbits.TRISA3 = OUTPUT;  // Change RA3 pin to output
    TRISAbits.TRISA4 = OUTPUT;  // Change RA4 pin to output
    TRISBbits.TRISB4 = OUTPUT;  // Change RB4 pin to output


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

/**
 * Trips when an input shows up on port 26 (RPI47)
 *
 * For the capture of Motor 1 A feedback (motor direction)
 * <p>
 * The pulses for direction are 90 degrees out of phase from the speed pulses.
 * If the speed pulses arrive before the direction pulses, then the motor is
 */
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    //-------------------------------------------------------------------------
    //  Check to see if the M1-B input capture came first.
    //-------------------------------------------------------------------------
    if (M1FdBckBSet)
    {

    }
    // Get Direction of motor
}

/**
 * Trips when an input shows up on port 25 (RPI46)
 *
 * For the capture of Motor 1 B feedback (motor speed)
 */
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{
    // Get speed of motor
}

/**
 * Trips when an input shows up on port 24 (RPI45)
 *
 * For the capture of Motor 2 A feedback (motor direction)
 */
void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void)
{
    // Get Direction of motor
}

/**
 * Trips when an input shows up on port 24 (RPI45)
 *
 * For the capture of Motor 2 B feedback (motor speed)
 */
void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void)
{
    // Get speed of motor
}