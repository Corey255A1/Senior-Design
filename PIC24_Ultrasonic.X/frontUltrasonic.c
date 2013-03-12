/*
 *  Main functions for front Ultrasonic code
 *
 *  Dan Pramik
 */

#include <p24EP32MC202.h>
#include <math.h>
#include "frontUltrasonic.h"
#include "globalsTemp.h"

short global_front1_edge = RISE;
long global_front1_time = 0;
short global_front2_edge = RISE;
long global_front2_time = 0;

// going to be static in end, this is just a placeholder
int baseLength = 10;

int front1_time_i;
int front1_time_f;
int front2_time_i;
int front2_time_f;

void initFrontUltras( void ){
    _TRISB7 = OUTPUT;
    _TRISB8 = OUTPUT;

    U1_RPOreg = OC1port; // set ultra1 RPO register to OC1 output
    U2_RPOreg = OC1port; // set ultra2 pulse out to OC1

    U1_RBreg = INPUT; // Set ultra1 Tris RB register to input mode
    U2_RBreg = INPUT;
    RPINR7bits.IC1R = U1_RPIport; // set IC1 input to ultra1 RP input
    RPINR7bits.IC2R = U2_RPIport;

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
    IC2CON1bits.ICM = 0x000; // turn off
    IC2TMR = 0; // clear IC1TMR
    IC2CON1 = 0; // clear config1
    IC2CON2 = 0;

    IPC1bits.IC2IP = 1;
    IFS0bits.IC2IF = 0;

    IC2CON1bits.ICTSEL = 0b000; // IC2 to TMR3
    IC2CON1bits.ICI = 0;    // interrupt every capture

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
    if((global_front1_edge == RISE) && (U1_RBIport == HIGH)){
        front1_time_i = IC1BUF;
        global_front1_edge = FALL;
    }else{
        front1_time_f = IC1BUF;
        global_front1_edge = RISE;
        global_front1_time = front1_time_f - front1_time_i;
    }

    if (global_front1_time<300)
        _RB7 = HIGH;
    else
        _RB7 = LOW;
}

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{
    _IC2IF = 0;
    if((global_front2_edge == RISE) && (U2_RBIport == HIGH)){
        front2_time_i = IC2BUF;
        global_front2_edge = FALL;
    }else{
        front2_time_f = IC2BUF;
        global_front2_edge = RISE;
        global_front2_time = front2_time_f - front2_time_i;
    }
    if (global_front2_time<300)
        _RB8 = HIGH;
    else
        _RB8 = LOW;
}

double convertToDistance(int time){
    // take in time and convert to distance - need conversion still
    return 10.0;
}

// return angle to turn
double findObject(void){
    // one edge is global_u1_time, other is global_u2_time

    // perform law of cosines, let u1 = a, u2 = b, and base = c
    double a = convertToDistance(global_front1_time);
    double b = convertToDistance(global_front2_time);
    double c = baseLength;

    double preAngleA = (b * b + c * c - a * a) / (2 * b * c);
    double angleA = acos(preAngleA);

    double preAngleB = (a * a + c * c - b * b) / (2 * a * c);
    double angleB = acos(preAngleB);

    double angleDiff = angleA - angleB;

    // if the angles are less than 5 degrees apart consider it straight ahead
    if (fabs(angleDiff) <= 5 ){
        return 45.0;    // simulate turning 45 degrees
    }
    else {
        return angleDiff;   // angle diff will represent how much it needs to turn
    }

    // still needs conditions if facing the fridge/etc
    // need to find the distance to the object
}

int main()
{
    return 0;
}
