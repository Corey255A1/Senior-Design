/*
 *  Main functions for front Ultrasonic code
 *
 *  Dan Pramik
 */

#include <p24EP32MC202.h>
#include <math.h>
#include "frontUltrasonic.h"
#include "globalsTemp.h"
#include <stdbool.h>

int global_temp = 15;   // test
int i = 0;

short global_front1_edge = RISE;
long global_front1_time = 0;
short global_front2_edge = RISE;
long global_front2_time = 0;

// going to be static in end, this is just a placeholder
unsigned baseLength = 10;
unsigned a = 0.0;
unsigned b = 0.0;

int front1_time_i;
int front1_time_f;
int front2_time_i;
int front2_time_f;

bool leftFound = false;
bool rightFound = false;

unsigned angle = 0.0;

void initFrontUltras( void ){
    _TRISB7 = OUTPUT;
    _TRISB8 = OUTPUT;

    U1_RPOreg = OC1port; // set ultra1 RPO register to OC1 output

    U2_RBreg = INPUT; // Set ultra1 Tris RB register to input mode
    U3_RBreg = INPUT;
    RPINR7bits.IC1R = U2_RPIport; // set IC1 input to ultra1 RP input
    RPINR7bits.IC2R = U3_RPIport;

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
    OC1R = 5000; // set duration of OC1

    // setup input capture module 1
    IC1CON1bits.ICM = 0x000; // turn off
    IC1TMR = 0; // clear IC1TMR
    IC1CON1 = 0; // clear config1
    IC1CON2 = 0;

    IPC0bits.IC1IP = 1; // Turn on IC1
    IFS0bits.IC1IF = 0; // clear IC1 interrupt status flag

    IC1CON1bits.ICTSEL = 0b000; // set IC1 to TMR3
    //IC1CON1bits.ICI = 0;    // interrupt every capture

    // setup the input capture module 2
    IC2CON1bits.ICM = 0x000; // turn off
    IC2TMR = 0; // clear IC1TMR
    IC2CON1 = 0; // clear config1
    IC2CON2 = 0;

    IPC1bits.IC2IP = 1;
    IFS0bits.IC2IF = 0;

    IC2CON1bits.ICTSEL = 0b000; // IC2 to TMR3
    //IC2CON1bits.ICI = 0;    // interrupt every capture

    // turn all on
    IC1CON1bits.ICM = 0b001; // IC1 for every edge
    IEC0bits.IC1IE = 1; // IC1 interrupts
    IC2CON1bits.ICM = 0b001; // IC2 for every edge
    IEC0bits.IC2IE = 1; // IC2 interrupts
    T3CONbits.TON = 1;  // TMR3
} // end init

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    leftFound = false;
    _IC1IF = 0;
    if((global_front1_edge == RISE) && (U2_RBIport == HIGH)){
        front1_time_i = IC1BUF;
        global_front1_edge = FALL;
    }else{
        front1_time_f = IC1BUF;
        global_front1_edge = RISE;
        global_front1_time = front1_time_f - front1_time_i;
    }
    leftFound = true;
    /*
    if (global_front1_time<300)
        _RB7 = HIGH;
    else
        _RB7 = LOW;
    */
}

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{
    rightFound = false;
    _IC2IF = 0;
    if((global_front2_edge == RISE) && (U3_RBIport == HIGH)){
        front2_time_i = IC2BUF;
        global_front2_edge = FALL;
    }else{
        front2_time_f = IC2BUF;
        global_front2_edge = RISE;
        global_front2_time = front2_time_f - front2_time_i;
    }
    rightFound = true;
    /*
    if (global_front2_time<300)
        _RB8 = HIGH;
    else
        _RB8 = LOW;
    */
}

unsigned convertToDistance(long time){
    // take in time and convert to distance
    unsigned distance = 0.0;

    // S = Cair * time (S = distance traveled)
    unsigned Cair = 33150 + 60 * global_temp;

    // S/2 = distance to object
    distance = (Cair * time) / 2;

    return distance;
}

// return angle to turn
unsigned findObject(void){
    // one edge is global_u1_time, other is global_u2_time

    // perform law of cosines, let u1 = a, u2 = b, and base = c
    a = convertToDistance(global_front1_time);
    b = convertToDistance(global_front2_time);
    unsigned c = baseLength;

    unsigned preAngleA = (b * b + c * c - a * a) / (2 * b * c);
    unsigned angleA = acos(preAngleA);

    unsigned preAngleB = (a * a + c * c - b * b) / (2 * a * c);
    unsigned angleB = acos(preAngleB);

    unsigned angleDiff = angleA - angleB;

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
    initFrontUltras();
    while (1)
    {
        // if we have found both signals, find the object
        //if (leftFound && rightFound)
            angle = findObject();
            for (i = 0; i < 10000; ++i)
            {
                Nop();
            }
            i = 0;
    }

    return 0;
}
