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
//      SS1!(pin 21)    -   Slave Select (SPI)
//      RB11(pin 22)    -   H-Bridge Enable
//      RPI44(pin 23)   -   Input Capture Motor 2B
//      RPI45(pin 24)   -   Input Capture Motor 2A
//      RPI46(pin 25)   -   Input Capture Motor 1B
//      RPI47(pin 26)   -   Input Capture Motor 1A
//
//  Output Digital Pins:
//      RA0 (pin 2)     -   H-Bridge 1 Forward Enable
//      RA1 (pin 3)     -   H-Bridge 1 Reverse Enable
//      RB0 (pin 4)     -   H-Bridge 2 Forward Enable
//      RB1 (pin 5)     -   H-Bridge 2 Reverse Enable
//      RP37(pin14)     -   OC1 PWM Output
//      RP38(pin 15)    -   OC2 PWM Output
//      SDO1(pin 17)    -   Serial Data Output (SPI)
//
//  Voltages and Ground:
//      Vss (pin 8)
//      Vdd (pin 13)
//      Vss (pin 19)
//      Vcap(pin 20)
//      AVss(pin 27)
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
#include "Configuration.h"
#include "globals.h"
#include "../pic24EP.X/spi.h"



//-----------------------------------------------------------------------------
//  Defines
//-----------------------------------------------------------------------------
#define DRIVER_PERIOD_US    10
#define CLK_PERIOD          0.135
#define DRIVE_EN            _RB11
#define OUTPUT              0
#define INPUT               1
#define DIGITAL             0
#define EN                  1
#define DISABLE             0
#define CLEAR               0
#define SPEEDM1             OC1R
#define SPEEDM2             OC2R
#define M1FWD               _RA0
#define M1REV               _RA1
#define M2FWD               _RB0
#define M2REV               _RB1

//-----------------------------------------------------------------------------
//  Global Declarations.
//-----------------------------------------------------------------------------
int OC1clkT = DRIVER_PERIOD_US/CLK_PERIOD;  // Set a period
int spiReadVal = 0;                        // Message we read from Master
char msgQueued = CLEAR;                     // Let's us know if a message
                                            // needs handled.

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
    ANSELAbits.ANSA0 = DIGITAL; // Change RA0 pin to digital
    ANSELAbits.ANSA1 = DIGITAL; // Change RA1 pin to digital
    ANSELBbits.ANSB0 = DIGITAL; // Change RB0 pin to digital
    ANSELBbits.ANSB1 = DIGITAL; // Change RB1 pin to digital

    TRISAbits.TRISA0 = OUTPUT;  // Change RA0 pin to output
    TRISAbits.TRISA1 = OUTPUT;  // Change RA1 pin to output
    TRISBbits.TRISB0 = OUTPUT;  // Change RB0 pin to output
    TRISBbits.TRISB1 = OUTPUT;  // Change RB1 pin to output
    TRISBbits.TRISB11= OUTPUT;  // Change RB11 pin to output
}

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
    char forwardDirM1;
    char speedM1;
    char forwardDirM2;
    char speedM2;

    DRIVE_EN = EN;
    SPEEDM1 = 70;
    SPEEDM2 = 0;
    msgQueued = EN;

    spiReadVal = 0x00AF;
    while(1)
    {
        
        //---------------------------------------------------------------------
        //  If a message needs decoding...
        //---------------------------------------------------------------------
        if (msgQueued)
        {
            //-----------------------------------------------------------------
            //  Here we can parse the speed and direction from the
            //  message received.
            //
            //  Bits (3-0) are the bits for direction on Motor 1
            //  Bits (7-4) are the bits for speed on Motor 1
            //  Bits (11-8) are the bits for direction on Motor 2
            //  Bits (15-12) are the bits for speed on Motor 2
            //-----------------------------------------------------------------
            forwardDirM1    = 0x000F & spiReadVal;
            speedM1         = (0x00F0 & spiReadVal) >> 4;
            forwardDirM2    = (0x0F000 & spiReadVal) >> 8;
            speedM2         = (0xF0000 & spiReadVal) >> 12;


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
            if (forwardDirM1 == 0x0F)
            {
                //-------------------------------------------------------------
                //  - Disable the reverse signal for Motor 1. (Safe Check)
                //  - Enable the forward signal for Motor 1.
                //-------------------------------------------------------------
                M1REV = DISABLE;
                M1FWD = EN;

            //-----------------------------------------------------------------
            //  ... Else Motor 1 is in reverse...
            //-----------------------------------------------------------------
            } else {
                //-------------------------------------------------------------
                //  - Disable the forward signal for Motor 1. (Safe Check)
                //  - Enable the reverse signal for Motor 1.
                //-------------------------------------------------------------
                M1FWD = DISABLE;
                M1REV = EN;
            }

            //-----------------------------------------------------------------
            //  If Motor 2 is going forward...
            //-----------------------------------------------------------------
            if (forwardDirM2 == 0x0F)
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
