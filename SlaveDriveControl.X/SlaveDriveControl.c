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
#include "../pic24EP_spibeta.X/spi.h"
#include "ConfigPeriphs.h"
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
    configInputCaptures();
    char forwardDirM1;
    char speedM1;
    char forwardDirM2;
    char speedM2;

    DRIVE_EN        = EN;
    SPEEDM1         = 0;
    SPEEDM2         = 0;
    msgQueued       = EN;
    M1FWD           = 1;
    M1REV           = 0;
    M2FWD           = 0;
    M2REV           = 0;
  
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
        }
    }
    return 0;
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
    _IC1IF = 0;
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
    _IC2IF = 0; // Clear the interrupt flag

    //-------------------------------------------------------------------------
    //  If we are looking at the beginning (rising edge) of the pulse...
    //-------------------------------------------------------------------------
    if ((M1FdBckBEdge == RISE) && (M1FDBCKB_RBPORT == HIGH))
    {
        M1FdBckBStart_t = IC2BUF;   // Capture the start time from IC2 buffer
        M1FdBckBEdge    = FALL;     // Next interrupt occurs on falling edge
    }
    //-------------------------------------------------------------------------
    //  ... else we are looking at the end (falling edge) of the pulse.
    //-------------------------------------------------------------------------
    else
    {
        M1FdBckBEnd_t   = IC2BUF;   // Capture the end time from IC2 buffer
        M1FdBckBEdge    = RISE;     // Next interrupt occurs on rising edge
        M1FdBckB_t      = M1FdBckAEnd_t - M1FdBckAStart_t;  // Calculate PW

        
    }
}

/**
 * Trips when an input shows up on port 24 (RPI45)
 *
 * For the capture of Motor 2 A feedback (motor direction)
 */
void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void)
{
    _IC3IF = 0;
    // Get Direction of motor
}

/**
 * Trips when an input shows up on port 24 (RPI45)
 *
 * For the capture of Motor 2 B feedback (motor speed)
 */
void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void)
{
    _IC4IF = 0; // Clear the interrupt flag
    
    //-------------------------------------------------------------------------
    //  If we are looking at the beginning (rising edge) of the pulse...
    //-------------------------------------------------------------------------
    if ((M2FdBckBEdge == RISE) && (M2FDBCKB_RBPORT == HIGH))
    {
        M2FdBckBStart_t = IC4BUF;
        M2FdBckBEdge    = FALL;
    }
    else
    {
        M2FdBckBEnd_t   = IC4BUF;
        M2FdBckBEdge    = RISE;
        M2FdBckB_t      = M2FdBckBEnd_t - M2FdBckBStart_t;
    }
}