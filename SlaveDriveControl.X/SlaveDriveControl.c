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

long mFeedWidths[100];
int mWidthCount = 0;
long mFeedRunSum = 0;
long mFeedRunAvg = 0;

int main(void) {

    configDevicePins();
    configOutputCompare();
    configSPICommunication();
    configInputCaptures();
    char forwardDirM1;
    char speedM1;
    char forwardDirM2;
    char speedM2;
    char distance;

    DRIVE_EN        = EN;
    SPEEDM1         = 75;
    SPEEDM2         = 75;
    msgQueued       = EN;
    M1FWD           = 1;
    M1REV           = 0;
    M2FWD           = 1;
    M2REV           = 0;
  
    while(1)
    {
        //---------------------------------------------------------------------
        //  If a message needs decoding...
        //---------------------------------------------------------------------
        //if (curConfig != INMSG)
        if (SPEEDM1 == 20)
        {
            curMasterConfig = INMSG;
                    
            //-----------------------------------------------------------------
            //  Here we can parse the speed and direction from the
            //  message received.
            //
            //  Bits (3-0) are the bits for direction on Motor 1
            //  Bits (7-4) are the bits for speed on Motor 1
            //  Bits (11-8) are the bits for direction on Motor 2
            //  Bits (15-12) are the bits for speed on Motor 2
            //-----------------------------------------------------------------
            forwardDirM1    = 0x000F & curMasterConfig;
            speedM1         = (0x00F0 & curMasterConfig) >> 4;
            forwardDirM2    = (0x0F000 & curMasterConfig) >> 8;
            speedM2         = (0xF0000 & curMasterConfig) >> 12;
            //distance =


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
    //  Check to see if the M1-A input capture came first.
    //-------------------------------------------------------------------------
    if (M1FdBckASet)
    {

    }
    if ((M1FdBckAEdge == RISE) && (M1FDBCKA_RBPORT == HIGH))
    {
        M1FdBckAStart_t = IC1BUF;   // Capture the start time from IC1 buffer
        M1FdBckAEdge    = FALL;     // Next interrupt occurs on falling edge
    }
    else
    {
        M1FdBckAEnd_t   = IC1BUF;   // Capture the end time from IC1 buffer
        M1FdBckAEdge    = RISE;     // Next interrupt occurs on rising edge
        ++M1FdBckA_SampCount;            // increase the sample count
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
        ++M1FdBckB_SampCount;       // increase the sample count

        if ((M1FdBckB_SampCount % 100) == 0)
        {
            OUTMSG_M1 = M1FdBckB_SampCount;
        }
        
        //---------------------------------------------------------------------
        //  I noticed that the value returned from the feedback sensors didn't
        //  really come to a steady average till about the 10th sample, or
        //  pulse width measurement. Therefore, when monitoring the feedback
        //  I'm only  going to worry about the tentht captured pulse, and
        //  measure that to get a more acurate reading.
        //---------------------------------------------------------------------
        if (M1FdBckB_SampCount == SAMPNUM)
        {
            M1FdBckB_SampCount = 1; // reset the samp counter
            M1FdBckB_t  = M1FdBckBEnd_t - M1FdBckBStart_t;  // Calculate PW

            //-----------------------------------------------------------------
            //  Go through the process of trying to detect the speed value
            //  based on the pulse width returned from the feedback using
            //  threshold detection
            //
            //      - Clear the bits (8-5) at the beginning to prep them for
            //        being modified once the speed has been found.
            //-----------------------------------------------------------------
            curDriveState &= 0xFF0F;    // Just clear the bits (8-5)
            if (M1FdBckB_t == 0)
            {
                // Speed 0
                curDriveState &= 0xFF0F;    // Just clear the bits (8-5)
            }
            else if ((M1FdBckB_t > 0)&& (M1FdBckB_t < MSpeed1_2_Thresh))
            {
                // speed 1
                curDriveState |= 0x0010;    // OR with 1

            }
            else if ((M1FdBckB_t > MSpeed1_2_Thresh) && (M1FdBckB_t < MSpeed2_3_Thresh) )
            {
                // speed 2
                curDriveState |= 0x0020;    // OR with 2
            }
            else if ((M1FdBckB_t > MSpeed2_3_Thresh) && (M1FdBckB_t < MSpeed3_4_Thresh) )
            {
                // Speed 3
                curDriveState |= 0x0030;    // OR with 3
            }
            else if ((M1FdBckB_t > MSpeed3_4_Thresh) && (M1FdBckB_t < MSpeed4_5_Thresh) )
            {
                // Speed 4
                curDriveState |= 0x0040;    // OR with 4
            }
            else if ((M1FdBckB_t > MSpeed4_5_Thresh) && (M1FdBckB_t < MSpeed5_6_Thresh) )
            {
                // Speed 5
                curDriveState |= 0x0050;    // OR with 5
            }
            else if ((M1FdBckB_t > MSpeed5_6_Thresh) && (M1FdBckB_t < MSpeed6_7_Thresh) )
            {
                // Speed 6
                curDriveState |= 0x0060;    // OR with 6
            }
            else if ((M1FdBckB_t > MSpeed6_7_Thresh) && (M1FdBckB_t < MSpeed7_8_Thresh) )
            {
                // Speed 7
                curDriveState |= 0x0070;    // OR with 7
            }
            else if ((M1FdBckB_t > MSpeed7_8_Thresh) && (M1FdBckB_t < MSpeed8_9_Thresh) )
            {
                // speed 8
                curDriveState |= 0x0080;    // OR with 8
            }
            else if ((M1FdBckB_t > MSpeed8_9_Thresh) && (M1FdBckB_t < MSpeed9_10_Thresh) )
            {
                // Speed 9
                curDriveState |= 0x0090;    // OR with 9
            }
            else
            {
                // Speed 10
                curDriveState |= 0x00A0;    // OR with A
            }
        }
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
    //-------------------------------------------------------------------------
    //  Check to see if the M1-A input capture came first.
    //-------------------------------------------------------------------------
    if (M2FdBckASet)
    {

    }
    if ((M2FdBckAEdge == RISE) && (M2FDBCKA_RBPORT == HIGH))
    {
        M2FdBckAStart_t = IC3BUF;   // Capture the start time from IC3 buffer
        M2FdBckAEdge    = FALL;     // Next interrupt occurs on falling edge
    }
    else
    {
        M2FdBckAEnd_t   = IC3BUF;   // Capture the end time from IC3 buffer
        M2FdBckAEdge    = RISE;     // Next interrupt occurs on rising edge
        ++M2FdBckA_SampCount;            // increase the sample count
    }
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
        M2FdBckBStart_t = IC4BUF;   // Capture the start time from IC4 buffer
        M2FdBckBEdge    = FALL;     // Next interrupt occurs on falling edge
    }
    //-------------------------------------------------------------------------
    //  ... else we are looking at the end (falling edge) of the pulse.
    //-------------------------------------------------------------------------
    else
    {
        M2FdBckBEnd_t   = IC4BUF;   // Capture the end time from IC4 buffer
        M2FdBckBEdge    = RISE;     // Next interrupt occurs on rising edge
        ++M2FdBckB_SampCount;       // increase the sample count

        if ((M2FdBckB_SampCount % 100) == 0)
        {
            OUTMSG_M2 = M2FdBckB_SampCount;
        }
        //---------------------------------------------------------------------
        //  I noticed that the value returned from the feedback sensors didn't
        //  really come to a steady average till about the 10th sample, or
        //  pulse width measurement. Therefore, when monitoring the feedback
        //  I'm only  going to worry about the tentht captured pulse, and
        //  measure that to get a more acurate reading.
        //---------------------------------------------------------------------
        if (M2FdBckB_SampCount == SAMPNUM)
        {
            M2FdBckB_SampCount = 1; // reset the samp counter
            M2FdBckB_t  = M2FdBckBEnd_t - M2FdBckBStart_t;  // Calculate PW

            //-----------------------------------------------------------------
            //  Go through the process of trying to detect the speed value
            //  based on the pulse width returned from the feedback using
            //  threshold detection
            //
            //      - Clear the bits (8-5) at the beginning to prep them for
            //        being modified once the speed has been found.
            //-----------------------------------------------------------------
            curDriveState &= 0x0FFF;    // Just clear the bits (8-5)
            if (M2FdBckB_t == 0)
            {
                // Speed 0
                curDriveState &= 0x0FFF;    // Just clear the bits (8-5)
            }
            else if ((M2FdBckB_t > 0)&& (M2FdBckB_t < MSpeed1_2_Thresh))
            {
                // speed 1
                curDriveState |= 0x1000;    // OR with 1

            }
            else if ((M2FdBckB_t > MSpeed1_2_Thresh) && (M2FdBckB_t < MSpeed2_3_Thresh) )
            {
                // speed 2
                curDriveState |= 0x2000;    // OR with 2
            }
            else if ((M2FdBckB_t > MSpeed2_3_Thresh) && (M2FdBckB_t < MSpeed3_4_Thresh) )
            {
                // Speed 3
                curDriveState |= 0x3000;    // OR with 3
            }
            else if ((M2FdBckB_t > MSpeed3_4_Thresh) && (M2FdBckB_t < MSpeed4_5_Thresh) )
            {
                // Speed 4
                curDriveState |= 0x4000;    // OR with 4
            }
            else if ((M2FdBckB_t > MSpeed4_5_Thresh) && (M2FdBckB_t < MSpeed5_6_Thresh) )
            {
                // Speed 5
                curDriveState |= 0x5000;    // OR with 5
            }
            else if ((M2FdBckB_t > MSpeed5_6_Thresh) && (M2FdBckB_t < MSpeed6_7_Thresh) )
            {
                // Speed 6
                curDriveState |= 0x6000;    // OR with 6
            }
            else if ((M2FdBckB_t > MSpeed6_7_Thresh) && (M2FdBckB_t < MSpeed7_8_Thresh) )
            {
                // Speed 7
                curDriveState |= 0x7000;    // OR with 7
            }
            else if ((M2FdBckB_t > MSpeed7_8_Thresh) && (M2FdBckB_t < MSpeed8_9_Thresh) )
            {
                // speed 8
                curDriveState |= 0x8000;    // OR with 8
            }
            else if ((M2FdBckB_t > MSpeed8_9_Thresh) && (M2FdBckB_t < MSpeed9_10_Thresh) )
            {
                // Speed 9
                curDriveState |= 0x9000;    // OR with 9
            }
            else
            {
                // Speed 10
                curDriveState |= 0xA000;    // OR with A
            }
        }
    }
}