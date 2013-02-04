/**
 * -- Ultrasonic Sensor Reader --
 * This gets the Output Compare module set up to generate
 * A pulse to be used with the Ultrasonic sensor
 * It also sets up the input capture module to be
 * used to capture the return pulse from the ultrasonic
 * To do this it requires the use of a interrupt routine
 *
 */

#include <p24EP32MC202.h>
#include "ultrasonic.h"
#include "globals.h"

//Local Variable Declarations
int u1_time_i;
int u1_time_f;
int u2_time_i;
int u2_time_f;

/**
 * This function takes of initializing all of the stuff required to setup
 * The ultrasonic sensors
 */
void initUltra( void ){
   /*Unused
    IC1CON2bits.ICTRIG=0; //Trigger mode
    IC1CON2bits.SYNCSEL=0b01101;//TMR3 used to sync IC1
    */

    //Setup the Ports correctly
    U1_RPOreg=OC1port; // SET Ultra1 RPO register to OC1 output
    U2_RPOreg=OC1port; //Set Ultra2 pulse out to OC1

    U1_RBreg = INPUT; // Set Ultra1 Tris RB register to Input mode
    U2_RBreg = INPUT;
    RPINR7bits.IC1R = U1_RPIport; //set IC1 input to Ultra1 RP input **5V Tolerant
    RPINR7bits.IC2R = U2_RPIport; //set IC2 input to Ultra2 RP input **5V Tolerant
    //Setup the Output Compare Module timers
    TMR3=0; // Clear out the TMR3
    T3CONbits.TON = 0; // Turn off TMR3
    T3CONbits.TCKPS=0b01; //Set Prescaler to 1:8
    OC1CON1=0; //clear out control regs
    OC1CON2=0; //clear out control regs
    OC1CON1bits.OCTSEL = 0x001; //set to TMR3
    OC1CON1bits.OCM=0b110; //set to EDGE-ALIGNED PWM
    OC1CON2bits.SYNCSEL = 0x1F; //set period control to OC1RS
    OC1RS = 50000; //Set the Period of OC1
    OC1R = 5000; //set the Duration of OC1

    //Setup the Input Capture module 1
    IC1CON1bits.ICM = 0x000; //Turn off Input Capture
    IC1TMR = 0; //clear IC1TMR
    IC1CON1=0; //clear config1
    IC1CON2=0; //clear config2
    IPC0bits.IC1IP = 1; //Turn on IC1
    IFS0bits.IC1IF = 0; // Clear the IC1 interrupt status flag 
    IC1CON1bits.ICTSEL=0b000; //set IC1 to TMR3
    IC1CON1bits.ICI=0; //interrupet on every capture

    //Setup the Input Capture module 2
    IC2CON1bits.ICM = 0x000; //Turn off Input Capture
    IC2TMR = 0; //clear IC2TMR
    IC2CON1=0; //clear config1
    IC2CON2=0; //clear config2
    IPC1bits.IC2IP = 1; //Turn on IC2
    IFS0bits.IC2IF = 0; // Clear the IC2 interrupt status flag
    IC2CON1bits.ICTSEL=0b000; //set IC2 to TMR3
    IC2CON1bits.ICI=0; //interrupet on every capture

    //Turn everything ON
    IC1CON1bits.ICM = 0x001; //Turn on IC1 for every edge trigger
    IEC0bits.IC1IE = 1; // Enable IC1 interrupts
    IC2CON1bits.ICM = 0x001; //Turn on IC2 for every edge trigger
    IEC0bits.IC2IE = 1; // Enable IC2 interrupts
    T3CONbits.TON = 1; //turn on TMR3

}//end init


/**
 * On the occurance of an Input Capture interrupt
 * This routine saves the value of IC1Buff only if
 * we are on the rising edge and that U1 port is high
 */
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{

    _IC1IF = 0;
    if((global_u1_edge == RISE) && (U1_RBIport == HIGH)){
        u1_time_i = IC1BUF;
        global_u1_edge = FALL;
    }else{
        u1_time_f = IC1BUF;
        global_u1_edge = RISE;
        global_u1_time = u1_time_f - u1_time_i;
    }//endif
}

/**
 * On the occurance of an Input Capture interrupt
 * This routine saves the value of IC2Buff only if
 * we are on the rising edge and that U2 port is high
 */
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{

    _IC2IF = 0;
    if((global_u2_edge == RISE) && (U2_RBIport == HIGH)){
        u2_time_i = IC2BUF;
        global_u2_edge = FALL;
    }else{
        u2_time_f = IC2BUF;
        global_u2_edge = RISE;
        global_u2_time = u2_time_f - u2_time_i;
    }//endif
}