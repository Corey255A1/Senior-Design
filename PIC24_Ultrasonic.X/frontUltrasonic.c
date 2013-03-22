/*
 *  Main functions for front Ultrasonic code
 *
 *  Dan Pramik
 */

#include <p24EP32MC202.h>
#include "frontUltrasonic.h"
#include "globalsTemp.h"
#include <stdbool.h>
#include <math.h>

double temp = 21; // degrees C
unsigned Cair = 0;

double outputDuration = 1500;

unsigned leftPulse = 0;
unsigned rightPulse = 0;

unsigned baseLength = 10; //cm, NEEDS UPDATED

unsigned angle = 0;

bool leftFound = false;
bool rightFound = false;

/*
int global_temp = 15;   // test
int i = 0;

short global_front1_edge = RISE;
long global_front1_time = 0;
short global_front2_edge = RISE;
long global_front2_time = 0;

// going to be static in end, this is just a placeholder
unsigned baseLength = 10;
unsigned a = 0;
unsigned b = 0;

int front1_time_i;
int front1_time_f;
int front2_time_i;
int front2_time_f;

bool leftFound = false;
bool rightFound = false;

unsigned angle = 0;
unsigned echoWidth = 0;
*/


void initFrontUltras( void ){
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
    OC1R = outputDuration; // set duration of OC1

    // setup input capture module 1
    IC1CON1bits.ICM = 0x000; // turn off
    IC1TMR = 0; // clear IC1TMR
    IC1CON1 = 0; // clear config1
    IC1CON2 = 0;

    IPC0bits.IC1IP = 1; // Turn on IC1
    IFS0bits.IC1IF = 0; // clear IC1 interrupt status flag

    IC1CON1bits.ICTSEL = 0b000; // set IC1 to TMR3
    IC1CON2bits.SYNCSEL = 0b00001; // set IC1 to synchronize with OC1
    //IC1CON1bits.ICI = 0;    // interrupt every capture

    // setup the input capture module 2
    IC2CON1bits.ICM = 0x000; // turn off
    IC2TMR = 0; // clear IC1TMR
    IC2CON1 = 0; // clear config1
    IC2CON2 = 0;

    IPC1bits.IC2IP = 1;
    IFS0bits.IC2IF = 0;

    IC2CON1bits.ICTSEL = 0b000; // IC2 to TMR3
    IC2CON2bits.SYNCSEL= 0b00001; // set IC2 to synchronize with OC1
    //IC2CON1bits.ICI = 0;    // interrupt every capture

    // turn all on
    IC1CON1bits.ICM = 0b011; // IC1 for rising edge
    IEC0bits.IC1IE = 1; // IC1 interrupts
    IC2CON1bits.ICM = 0b011; // IC2 for rising edge
    IEC0bits.IC2IE = 1; // IC2 interrupts
    
    IEC0bits.OC1IE = 1; // OC1 interrupts
    T3CONbits.TON = 1;  // TMR3
} // end init


void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    _IC1IF = 0;
    
    leftPulse = IC1BUF;

    unsigned lengthTemp = convertToDistance(leftPulse - outputDuration);

    if (lengthTemp > 20)
        leftFound = false;
    else
        leftFound = true;
    
    /*
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

    a = convertToDistance(global_front1_time);

    leftFound = true;
    */
    /*
    if (global_front1_time<300)
        _RB7 = HIGH;
    else
        _RB7 = LOW;
    */
}

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{
    _IC2IF = 0;
    
    rightPulse = IC2BUF;

    unsigned lengthTemp = convertToDistance(rightPulse - outputDuration);

    if (lengthTemp > 20)
        rightFound = false;
    else
        rightFound = true;
    /*
    static enum {PingEchoLow, PingEchoHigh}PingState = PingEchoHigh;
    
    static unsigned timeStart, timeStop;

    switch(PingState)
    {
        case PingEchoHigh:
            PingState = PingEchoLow;
            timeStart = IC2BUF;
            IC2CON1 = 0x0000; // disable IC2 module
            IC2CON1 = 0x0002; // enable on falling edge
            break;
        case PingEchoLow:
            PingState = PingEchoHigh;
            timeStop = IC2BUF;
            echoWidth = timeStop - timeStart;
            IC2CON1 = 0x0000; // disable IC2 module
            IC2CON1 = 0x0003;
    }
    */

    /*
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

    b = convertToDistance(global_front2_time);

    rightFound = true;
    */
    /*
    if (global_front2_time<300)
        _RB8 = HIGH;
    else
        _RB8 = LOW;
    */
}

unsigned convertToDistance(double time){
    // take in time and convert to distance
    unsigned distance = 0;

    // S = Cair * time (S = distance traveled)
    // S/2 = distance to object
    distance = (unsigned)(Cair>>1)*time;

    return distance;
}

//// return angle to turn
double findObject(void){
    // one edge is global_u1_time, other is global_u2_time

    // perform law of cosines, let u1 = a, u2 = b, and base = c
    unsigned leftLength = convertToDistance(leftPulse);
    unsigned rightLength = convertToDistance(rightPulse);
    unsigned c = baseLength;

    double preAngleA = (rightLength * rightLength + c * c - leftLength * leftLength) / (2 * rightLength * c);
    double angleA = acos(preAngleA) * 180 / pi;

    double preAngleB = (leftLength * leftLength + c * c - rightLength * rightLength) / (2 * leftLength * c);
    double angleB = acos(preAngleB) * 180 / pi;

    double angleDiff = angleA - angleB;

    // if the angles are less than 5 degrees apart consider it straight ahead
    if (fabs(angleDiff) <= 5 ){
        return 45;    // simulate turning 45 degrees
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
    Cair = 33150 + 60 * temp;
    while (1)
    {
        if (leftFound && rightFound) {
            angle = findObject();
            Nop();
            Nop();
        }
    }

    return 0;
}
