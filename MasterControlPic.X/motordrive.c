
#include <p24FJ128GB106.h>
#include "motordrive.h"
#include "globals.h"


/**
 * Trips when an input shows up on port 25 (RPI46)
 *
 * For the capture of Motor 1 B feedback (motor speed)
 */
int IC1_PULSE_STATE=LOW;
int IC2_PULSE_STATE=LOW;
long IC1_COUNT=0;
long IC2_COUNT=0;
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    _IC1IF = 0; // Clear the interrupt flag

    //-------------------------------------------------------------------------
    //  If we are looking at the beginning (rising edge) of the pulse...
    //-------------------------------------------------------------------------
    if (IC1_PULSE_STATE == RISE)
    {
        IC1_PULSE_STATE    = FALL;     // Next interrupt occurs on falling edge
    }
    //-------------------------------------------------------------------------
    //  ... else we are looking at the end (falling edge) of the pulse.
    //-------------------------------------------------------------------------
    else
    {
        IC1_PULSE_STATE    = RISE;     // Next interrupt occurs on rising edge
        ++IC1_COUNT;       // increase the sample count
    }
}//endic1

/**
 * Trips when an input shows up on port 24 (RPI45)
 *
 * For the capture of Motor 2 B feedback (motor speed)
 */

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{
    _IC2IF = 0; // Clear the interrupt flag

    //-------------------------------------------------------------------------
    //  If we are looking at the beginning (rising edge) of the pulse...
    //-------------------------------------------------------------------------
    if (IC2_PULSE_STATE == RISE)
    {
        IC2_PULSE_STATE    = FALL;     // Next interrupt occurs on falling edge
    }
    //-------------------------------------------------------------------------
    //  ... else we are looking at the end (falling edge) of the pulse.
    //-------------------------------------------------------------------------
    else
    {
        IC2_PULSE_STATE    = RISE;     // Next interrupt occurs on rising edge
        ++IC2_COUNT;       // increase the sample count
    }
}//endic2

void configOutputCompare(void)
{
    AD1PCFGLbits.PCFG6=1;
    AD1PCFGLbits.PCFG7=1;
    _TRISB6=0;
    _TRISB7=0;
    //-------------------------------------------------------------------------
    //  Setup Output Compare Timer 1 (Timer 3)
    //-------------------------------------------------------------------------
    TMR3            = 0;        // Clear TMR3
    T3CONbits.TON   = DISABLE;  // Turn off TMR3
    T3CONbits.TCKPS = 0b01;     // Set Pre-scalar to 1:8

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC1)
    //-------------------------------------------------------------------------
    OC1CON1             = CLEAR;    // Clear out first configuration register
    OC1CON2             = CLEAR;    // Clear out second configuration register
    OC1CON1bits.OCTSEL  = 0b001;    // Set TMR3 as the source timer.
    OC1CON1bits.OCM     = 0b110;    // Edge aligned PWM mode.
    OC1CON2bits.SYNCSEL = 0b01101;     // Period Control to OC1RS
    OC1RS               = CLK_PER;  // Set period of OC1 ... Here we want a
                                    // period of 4000 micro seconds.
    OC1R                = MSTOP_PULSE;        // Set duty duration of OC1
    RPOR3bits.RP6R     = 18;        // Maps the OC1 output to the RP37R pin
                                    // (pin 14) on the pick. The 0b010001 is
                                    // defined in the "Output Mapping" section
                                    // of the data sheet.

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC2)
    //-------------------------------------------------------------------------
    OC2CON1             = CLEAR;    // Clear out first configuration register
    OC2CON2             = CLEAR;    // Clear out second configuration register
    OC2CON1bits.OCTSEL  = 0b001;    // Set TMR2 as the source timer.
    OC2CON1bits.OCM     = 0b110;    // Edge aligned PWM mode.
    OC2CON2bits.SYNCSEL = 0b01101;     // Period Control to OC2RS
    OC2RS               = CLK_PER;  // Set period of OC1 ... Here we want a
                                    // period of 4000 micro seconds
    OC2R                = MSTOP_PULSE;        // Set duty duration of OC1
    RPOR3bits.RP7R     = 19;        // Maps the OC2 output to the RP38R pin
                                    // (pin 15) on the pick. The 0b010000 is
                                    // defined in the "Output Mapping" section
                                    // of the data sheet.

    //-------------------------------------------------------------------------
    //  Ready to turn TMR3 and TMR2 on.
    //-------------------------------------------------------------------------
    T3CONbits.TON = 1;
}

void setMotor(int control){
        char forwardDirM1;
        char speedM1;
        char forwardDirM2;
        char speedM2;
        int M1Speed;
        int M2Speed;
        forwardDirM1    = 0x000F & control;
        speedM1         = (0x00F0 & control) >> 4;
        forwardDirM2    = (0x0F000 & control) >> 8;
        speedM2         = (0xF0000 & control) >> 12;
        IC1_COUNT =0;
        IC2_COUNT =0;

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
                M1Speed = MSTOP_PULSE + (speedM1*SPEEDMULT);

            //-----------------------------------------------------------------
            //  ... Else Motor 1 is in reverse...
            //-----------------------------------------------------------------
            } else {
                //-------------------------------------------------------------
                //  - Disable the forward signal for Motor 1. (Safe Check)
                //  - Enable the reverse signal for Motor 1.
                //-------------------------------------------------------------
                M1Speed = MSTOP_PULSE - (speedM1*SPEEDMULT);
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
                M2Speed = MSTOP_PULSE + (speedM2*SPEEDMULT);

            //-----------------------------------------------------------------
            //  ... Else Motor 2 is in reverse...
            //-----------------------------------------------------------------
            } else {
                //-------------------------------------------------------------
                //  - Disable the forward signal for Motor 2. (Safe Check)
                //  - Enable the reverse signal for Motor 2.
                //-------------------------------------------------------------
                M2Speed = MSTOP_PULSE - (speedM2*SPEEDMULT);
            }

            //-----------------------------------------------------------------
            //  Set the speed of both motors.
            //-----------------------------------------------------------------
            SPEEDM1 = 750;
            SPEEDM2 = M2Speed;
 }//endsetmotor