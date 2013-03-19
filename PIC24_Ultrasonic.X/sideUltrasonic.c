/*
 *  Code for Side ultrasonics
 *
 *  Dan
 *
 */


#include <p24EP32MC202.h>
#include "sideUltrasonic.h"
#include "globalsTemp.h"
#include <stdbool.h>
#include <math.h>

unsigned leftFrontPulse = 0;
unsigned leftRearPulse = 0;

unsigned rightFrontPulse = 0;
unsigned rightRearPulse = 0;

unsigned sideLength = 10;   // needs changed, but will be same for both

void initSideUltras( void ){
    U4_RPOreg = OC1port; // set ultra1 RPO register to OC1 output
    U5_RPOreg = OC1port;
    
    U6_RPOreg = OC1port;
    U7_RPOreg = OC1port;

    U4_RBreg = INPUT; // Set ultra1 Tris RB register to input mode
    U5_RBreg = INPUT;
    U6_RBreg = INPUT;
    U7_RBreg = INPUT;
    RPINR7bits.IC1R = U4_RPIport; // set IC1 input to ultra1 RP input
    RPINR7bits.IC2R = U5_RPIport;
    RPINR8bits.IC3R = U6_RPIport;
    RPINR8bits.IC4R = U7_RPIport;

    //Setup OCM timers
    TMR3 = 0;   // clear
    T3CONbits.TON = 0; // turn off

    T3CONbits.TCKPS = 0b01; // set prescaler to 1:8
    OC1CON1 = 0; // clear control registers
    OC1CON2 = 0;

    OC1CON1bits.OCTSEL = 0x001; // set to TMR3
    OC1CON1bits.OCM = 0b110; // set to edge-aligned PWM
    OC1CON2bits.SYNCSEL = 0x1F; // set period control to OC1RS

    OC1RS = 30000; // set period of OC1
    OC1R = 10000; // set duration of OC1

    // setup input capture module 1
    IC1CON1bits.ICM = 0x000; // turn off
    IC1TMR = 0; // clear IC1TMR
    IC1CON1 = 0; // clear config1
    IC1CON2 = 0;

    IPC0bits.IC1IP = 1; // Turn on IC1
    IFS0bits.IC1IF = 0; // clear IC1 interrupt status flag

    IC1CON1bits.ICTSEL = 0b000; // set IC1 to TMR3
    IC1CON1bits.ICI = 0;    // interrupt every capture

    // setup the input capture module 2
    IC2CON1bits.ICM = 0x000; // turn off
    IC2TMR = 0; // clear IC1TMR
    IC2CON1 = 0; // clear config1
    IC2CON2 = 0;

    IPC1bits.IC2IP = 1;
    IFS0bits.IC2IF = 0;

    IC2CON1bits.ICTSEL = 0b000; // IC2 to TMR3
    IC2CON1bits.ICI = 0;    // interrupt every capture

    // setup the input capture module 3
    IC3CON1bits.ICM = 0x000; // turn off
    IC3TMR = 0; // clear IC1TMR
    IC3CON1 = 0; // clear config1
    IC3CON2 = 0;

    IPC9bits.IC3IP = 1;
    IFS2bits.IC3IF = 0;

    IC3CON1bits.ICTSEL = 0b000; // IC3 to TMR3
    IC3CON1bits.ICI = 0;    // interrupt every capture

    // setup theinput capture module 4
    IC4CON1bits.ICM = 0x000; // turn off
    IC4TMR = 0; // clear IC1TMR
    IC4CON1 = 0; // clear config1
    IC4CON2 = 0;

    IPC9bits.IC4IP = 1;
    IFS2bits.IC4IF = 0;

    IC4CON1bits.ICTSEL = 0b000; // IC4 to TMR3
    IC4CON1bits.ICI = 0;    // interrupt every capture

    // turn all on
    IC1CON1bits.ICM = 0b001; // IC1 for every edge
    IEC0bits.IC1IE = 1; // IC1 interrupts
    IC2CON1bits.ICM = 0b001; // IC2 for every edge
    IEC0bits.IC2IE = 1; // IC2 interrupts
    IC3CON1bits.ICM = 0b001; // IC3 for every edge
    IEC2bits.IC3IE = 1; // IC3 interrupts
    IC4CON1bits.ICM = 0b001; // IC4 for every edge
    IEC2bits.IC4IE = 1; // IC4 interrupts
    
    T3CONbits.TON = 1;  // TMR3 ON
} // end init

/*
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{

}

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{

}

void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void)
{

}

void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void)
{

}
*/