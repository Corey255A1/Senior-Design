/* 
 * File:   frontUltrasonic.c
 * Author: Dan
 *
 * Created on February 28, 2013, 6:35 PM
 */

#include <p24EP32MC202.h>
#include "ultrasonic.h"
#include "globals.h"
#include <math.h>

short global_u1_edge = RISE;
long global_u1_time = 0;
short global_u2_edge = RISE;
long global_u2_time = 0;
short global_u3_edge = RISE;
long global_u3_time = 0;

int u1_time_i;
int u1_time_f;
int u2_time_i;
int u2_time_f;
int u3_time_i;
int u3_time_f;

void init_ultra( void ){
    _TRISB7 = OUTPUT;
    _TRISB8 = OUTPUT;

    U1_RPOreg = OC1port; // set ultra1 RPO register to OC1 output
    U2_RPOreg = OC1port; // set ultra2 pulse out to OC1
    U3_RPOreg = OC1port; // set ultra3 pulse out to OC1

    U1_RBreg = INPUT; // Set ultra1 Tris RB register to input mode
    U2_RBreg = INPUT;
    U3_RBreg = INPUT;
    RPINR7bits.IC1R = U1_RPIport; // set IC1 input to ultra1 RP input
    RPINR7bits.IC2R = U2_RPIport;
    RPINR8bits.IC3R = U3_RPIport;

    //Setup OCM timers
    TMR3 = 0;   // clear
    T3CONbits.TON = 0; // turn off

    T3CONbits.TCKPS = 0b01; // set prescaler to 1:8
    OC1CON1 = 0; // clear control registers
    OC1CON2 = 0;

    OC1CON1bits.OCTSEL = 0x001; // set to TMR3
    OC1CON1bits.OCM = 0b110; // set to edge-aligned PWM
    OC1CON2bits.SYNCSEL = 0x1F; // set period control to OC1RS

    OC1RS = 50000; // set period of OC1
    OC1R = 5000; // set duration of OC1

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
    IC2CON1bits.ICM = 0x000;
    IC2TMR = 0;
    IC2CON1 = 0;
    IC2CON2 = 0;

    IPC1bits.IC2IP = 1;
    IFS0bits.IC2IF = 0;

    IC2CON1bits.ICTSEL = 0b000;
    IC2CON1bits.ICI = 0;

    // turn all on
    IC1CON1bits.ICM = 0x001; // IC1 for every edge
    IEC0bits.IC1IE = 1; // IC1 interrupts
    IC2CON1bits.ICM = 0x001; // IC2 for every edge
    IEC0bits.IC2IE = 1; // IC2 interrupts
    T3CONbits.TON = 1;  // TMR3
} // end init

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
    }

    if (global_u1_time<300)
        _RB7 = HIGH;
    else
        _RB7 = LOW;
}

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
    }
    if (global_u2_time<300)
        _RB8 = HIGH;
    else
        _RB8 = LOW;
}

void __attribute__((__interrupt__,, auto_psv)) _IC3Interrupt(void)
{
    _IC3IF = 0;
    if ((global_u2_edge == RISE) && (U2_RBIport == HIGH)){
        u3_time_i = IC3BUF;
        global_u2_edge = FALL;
    }else{
        u3_time_f = IC2BUF;
        global_u3_edge = RISE;
        global_u3_time = u3_time_f - u3_time_i;
    }
    if (global_u3_time < 300)
        _RB8 = HIGH;
    else
        _RB8 = LOW;
}