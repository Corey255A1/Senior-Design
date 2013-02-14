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
//      SS1 (pin 4)     -   Slave Select (SPI)
//      SCK1(pin 16)    -   Shift Clock Input (SPI)
//      SDI1(pin 18)    -   Serial Data Input (SPI)
//      RP42(pin 24)    -   Input Caption from DC Motor Feedback
//
//  Output Digital Pins:
//      RB1 (pin 5)     -   Motor Driver Enable
//      RP38(pin 15)    -   PWM Signal
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


//-----------------------------------------------------------------------------
//  Defines
//-----------------------------------------------------------------------------
#define DRIVER_PERIOD_US    10
#define CLK_PERIOD          0.135
#define DRIVE_EN            _RB1
#define OUTPUT              0
#define INPUT               1
#define DIGITAL             0
#define EN                  1
#define DISABLE             0
#define CLEAR               0

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
    T3CONbits.TON   = DISABLE;  // Turn off TMR3
    T3CONbits.TCKPS = 0b00;     // Set Pre-scalar to 1:8

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC1)
    //-------------------------------------------------------------------------
    OC1CON1             = CLEAR;    // Clear out first configuration register
    OC1CON2             = CLEAR;    // Clear out second configuration register
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
    SPI1CON1bits.MODE16 = 1;        // Communication is word-wide (16-bits)
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
    IFS0bits.SPI1IF = CLEAR;// Clear the interrupt flag.
    IEC0bits.SPI1IE = EN;    // Enable interrupt.
}

/**
 * Reads the buffer for any type of communications from the master PIC.
 * 
 * @return void
 */
int readSPI(void)
{

    return 1;
}

/**
 * Setup the pin configurations (I/O)
 *
 * @return void
 */
void configDevicePins(void)
{
    //-------------------------------------------------------------------------
    //  RB1 Setup.
    //-------------------------------------------------------------------------
    ANSELBbits.ANSB1 = DIGITAL; // Change RB1 pin to digital
    TRISBbits.TRISB1 = OUTPUT;  // Change RB1 pin to output.

    //-------------------------------------------------------------------------
    //
    //-------------------------------------------------------------------------
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
    DRIVE_EN = EN;

    int SPIVal;
    while(1)
    {
        //---------------------------------------------------------------------
        //
        //---------------------------------------------------------------------
        SPIVal = readSPI();

        
    }
    return 0;
}
