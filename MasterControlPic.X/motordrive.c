
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
int IC_GO_COUNTS = 0;
short useCompass = 0;
unsigned short f_Heading=0;
unsigned int currentMotorSetting;

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
        if(!useCompass){
            if(IC2_COUNT>=IC_GO_COUNTS || IC1_COUNT>=IC_GO_COUNTS){
                currentMotorSetting=0;
                setMotor(currentMotorSetting);
                IC_GO_COUNTS = 0;
            }//endif
        }//endif
        else{
            if(f_Heading){
                currentMotorSetting=0;
                setMotor(currentMotorSetting);
                IC_GO_COUNTS = 0;
            }//endif
        }//endelse
    }//endelse
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
        if(!useCompass){
            if(IC2_COUNT>=IC_GO_COUNTS || IC1_COUNT>=IC_GO_COUNTS){
                currentMotorSetting=0;
                setMotor(currentMotorSetting);
            }//endif
        }//endif
        else{
            if(f_Heading){
                currentMotorSetting=0;
                setMotor(currentMotorSetting);
            }//endif
        }//endelse
    }
}//endic2

void setDistance(int dist){
    IC_GO_COUNTS = (int)((double) dist / (double) CM_TO_IC_COUNTS);
}

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
    T3CON           = 0;
    T3CONbits.TON   = DISABLE;  // Turn off TMR3
    T3CONbits.TCKPS = 0b01;     // Set Pre-scalar to 1:8

    //-------------------------------------------------------------------------
    //  Setup Output Compare (OC1)
    //-------------------------------------------------------------------------
    OC1CON1             = CLEAR;    // Clear out first configuration register
    OC1CON2             = CLEAR;    // Clear out second configuration register
    OC1CON1bits.OCTSEL  = 0b001;    // Set TMR3 as the source timer.
    OC1CON1bits.OCM     = 0b110;    // Edge aligned PWM mode.
    OC1CON2bits.SYNCSEL = 0x1F;     // Period Control to OC1RS
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
    OC2CON2bits.SYNCSEL = 0x1F;     // Period Control to OC
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
        forwardDirM2    = (0x0F00 & control) >> 8;
        speedM2         = (0xF000 & control) >> 12;
        if(control != 0){
            IC1_COUNT =0;
            IC2_COUNT =0;
        }

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
            SPEEDM1 = M1Speed;
            SPEEDM2 = M2Speed;
 }//endsetmotor

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
    TRISGbits.TRISG6    = INPUT;            // Set pin to input
    RPINR7bits.IC1R     = 21; // Set IC input to RPI input
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
    TRISCbits.TRISC14    = INPUT;            // Set pin to input
    RPINR7bits.IC2R     = 37; // Set IC input to RPI input
    IC2TMR              = 0;                // Clear the IC2 Timer
    IC2CON1             = 0;                // Clear the config 1 register
    IC2CON2             = 0;                // Clear the config 2 register
    IPC1bits.IC2IP      = 1;                // Set IC2 Interrupt Priority to 1
    IFS0bits.IC2IF      = 0;                // Clear the IC2 interrupt flag
    IC2CON1bits.ICTSEL  = 0b010;            // Set IC2 to TMR4
    IC2CON1bits.ICI     = 0;                // Interrupt on every capture
    IC2CON1bits.ICM     = 0x001;            // Capture on every edge trigger


    //-------------------------------------------------------------------------
    //  Turn the stuff on
    //-------------------------------------------------------------------------
    IEC0bits.IC1IE  = 1;    // Enable IC1 interrupts
    IEC0bits.IC2IE  = 1;    // Enable IC2 interrupts
    T4CONbits.TON   = 1;    // Turn on Timer 4
}