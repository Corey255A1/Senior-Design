/*
 *  Main functions for front Ultrasonic code
 *
 *  Dan Pramik
 */

#include <p24EP32MC202.h>
#include "frontUltrasonic.h"
#include "globalsTemp.h"
#include "spi.h"
#include "configs.h"
#include <stdbool.h>
#include <math.h>

#define ULTRA_FRONT_DISTANCE SLAVEData.outData[0]
#define ULTRA_BACK_DISTANCE SLAVEData.outData[1]

double temp = 21; // degrees C
double Cair = 0;

double leftLength = 0;
double rightLength = 0;

double frontLength = 0;
double backLength = 0;

double baseLength = 7.3; //cm

double angle = 0;

double timerPeriod = 2e-6;
double maxPulse = 12500;

bool leftFound = false;
bool rightFound = false;

short u4_edge = RISE;
long u4_time = 0;

int u4_time_i;
int u4_time_f;

bool backClose = false;

void initFrontUltras( void ){
    _TRISB7 = OUTPUT;

    U1_RPOreg = OC1port; // set ultra1 RPO register to OC1 output
    U4_RPOreg = OC2port;

    U2_RBreg = INPUT; // Set ultra1 Tris RB register to input mode
    U3_RBreg = INPUT;
    U4_RBreg = INPUT;
    RPINR7bits.IC1R = U2_RPIport; // set IC1 input to ultra2 RP input - Front left
    RPINR7bits.IC2R = U3_RPIport; // set IC2 input to ultra3 RP input - Front right
    RPINR8bits.IC3R = U4_RPIport; // set IC3 input to ultra4 RP input - Back

    //Setup OCM timers
    TMR3 = 0;   // clear
    T3CONbits.TON = 0; // turn off

    T3CONbits.TCKPS = 0b01; // set prescaler to 1:8

    // Output for Front
    OC1CON1 = 0; // clear control registers
    OC1CON2 = 0;

    OC1CON1bits.OCTSEL = 0x001; // set to TMR3
    OC1CON1bits.OCM = 0b110; // set to edge-aligned PWM
    OC1CON2bits.SYNCSEL = 0x1F; // set period control to OC1RS

    OC1RS = 30000; // set period of OC1 - This gives us a period of ~68ms
    OC1R = 15000; // set duration of OC1 -

    IPC0bits.OC1IP = 1;
    IFS0bits.OC1IF = 0;

    // Output for Back
    OC2CON1 = 0;
    OC2CON2 = 0;

    OC2CON1bits.OCTSEL = 0x001; // set to TMR3
    OC2CON1bits.OCM = 0b110; // edge-aligned
    OC2CON2bits.SYNCSEL = 0x1F; // period control to OC2RS

    OC2RS = 50000;
    OC2R = 5000;

    // setup input capture module 1
    IC1CON1bits.ICM = 0x000; // turn off
    IC1TMR = 0; // clear IC1TMR
    IC1CON1 = 0; // clear config1
    IC1CON2 = 0;

    IPC0bits.IC1IP = 1; // Turn on IC1
    IFS0bits.IC1IF = 0; // clear IC1 interrupt status flag

    IC1CON1bits.ICTSEL = 0b000; // set IC1 to TMR3
    IC1CON2bits.SYNCSEL = 0b00001; // set IC1 to synchronize with OC1

    // setup the input capture module 2
    IC2CON1bits.ICM = 0x000; // turn off
    IC2TMR = 0; // clear IC1TMR
    IC2CON1 = 0; // clear config1
    IC2CON2 = 0;

    IPC1bits.IC2IP = 1;
    IFS0bits.IC2IF = 0;

    IC2CON1bits.ICTSEL = 0b000; // IC2 to TMR3
    IC2CON2bits.SYNCSEL= 0b00001; // set IC2 to synchronize with OC1

    // setup input capture module 3 - back
    IC3CON1bits.ICM = 0x000;
    IC3TMR = 0;
    IC3CON1 = 0;
    IC3CON2 = 0;

    IPC9bits.IC3IP = 1;
    IFS2bits.IC3IF = 0;

    IC3CON1bits.ICTSEL = 0b000; // IC3 to TMR3
    IC3CON1bits.ICI = 0; // interrupt every capture

    // turn all on
    IC1CON1bits.ICM = 0b011; // IC1 for rising edge
    IEC0bits.IC1IE = 1; // IC1 interrupts
    IC2CON1bits.ICM = 0b011; // IC2 for rising edge
    IEC0bits.IC2IE = 1; // IC2 interrupts
    IC3CON1bits.ICM = 0b001; // IC3 for every edge
    IEC2bits.IC3IE = 1; // IC3 interrupts

    IEC0bits.OC1IE = 1;
    
    T3CONbits.TON = 1;  // TMR3
} // end init


void __attribute__((__interrupt__, auto_psv)) _OC1Interrupt(void)
{
    _OC1IF = 0;

    // Reset everything
    ULTRA_FRONT_DISTANCE = 0;

    leftFound = false;
    rightFound = false;
}

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    _IC1IF = 0;

    if (!leftFound)
    {
        leftLength = convertToDistance(IC1BUF * timerPeriod);

        leftFound = true;
    }
}

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{
    _IC2IF = 0;

    if (!rightFound)
    {
        rightLength = convertToDistance(IC2BUF * timerPeriod);

        rightFound = true;
    }
}

void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void)
{
    _IC3IF = 0;
    
    if((u4_edge == RISE) && (U4_RBIport == HIGH)){
        u4_time_i = IC1BUF;
        u4_edge = FALL;

        ULTRA_BACK_DISTANCE = 0;

        backClose = false;
    }else{
        u4_time_f = IC1BUF;
        u4_edge = RISE;
        u4_time = u4_time_i - u4_time_f;
        // once we have the time, convert to distance
        backLength = (400 * u4_time) / maxPulse;

        ULTRA_BACK_DISTANCE = (int)backLength;

        if (backLength < 5)
        {
            backClose = true;
        }
        else
        {
            backClose = false;
        }
    }
}

double convertToDistance(double time){
    // take in time and convert to distance
    double distance = 0;

    // S = Cair * time (S = distance traveled)
    // S/2 = distance to object
    distance = (Cair*time) / 2;

    return distance;
}

//// return angle to turn
double findObject(void){
    // one edge is global_u1_time, other is global_u2_time

    // perform law of cosines, let u1 = a, u2 = b, and base = c
    double preAngleA = (rightLength * rightLength + baseLength * baseLength - leftLength * leftLength) / (2 * rightLength * baseLength);
    double angleA = acos(preAngleA) * 180 / pi;

    double preAngleB = (leftLength * leftLength + baseLength * baseLength - rightLength * rightLength) / (2 * leftLength * baseLength);
    double angleB = acos(preAngleB) * 180 / pi;

    double angleDiff = angleA - angleB;

    // find distance to the object
    frontLength = leftLength * (sin(angleA * pi / 180));

    ULTRA_FRONT_DISTANCE = (int)frontLength;

    // DEBUGGING ONLY
    // if we are too close, then we need to backup/turn
    /*
    if (frontLength < 30)
    {
        _RB7 = HIGH;
    }
    else
    {
        _RB7 = LOW;
    }
    */

    // if the angles are less than 5 degrees apart consider it straight ahead
    if (fabs(angleDiff) <= 5 ){
        return 45;    // simulate turning 45 degrees
    }
    else {
        return angleDiff;   // angle diff will represent how much it needs to turn
    }
}

int main()
{
    initFrontUltras();
    configSPICommunication();
    Cair = 33150 + (60 * temp);
    while (1)
    {
        if (leftFound && rightFound) {
            angle = findObject();
        }

        if (backClose) {
            // tell robot to not backup
            // set flag that can't backup or something
        }
    }

    return 0;
}
