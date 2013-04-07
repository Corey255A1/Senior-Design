/*
 *  Code for Side ultrasonics
 *
 *  Dan
 *
 */

#include <p24EP32MC202.h>
#include "sideUltrasonic.h"
#include "globalsTemp.h"
#include "spi.h"
#include "configs.h"
#include <stdbool.h>
#include <math.h>

#define ULTRA_LEFT_FRONT_DISTANCE SLAVEData.outData[0]
#define ULTRA_LEFT_REAR_DISTANCE SLAVEData.outData[1]
#define ULTRA_RIGHT_FRONT_DISTANCE SLAVEData.outData[2]
#define ULTRA_RIGHT_REAR_DISTANCE SLAVEData.outData[3]

double temp = 21; // degrees C
double Cair = 0;

double timerPeriod = 2e-6;

double leftLeftLength = 0;
double leftRightLength = 0;

double rightLeftLength = 0;
double rightRightLength = 0;

double sideLength = 11.1;   // needs changed, but will be same for both

bool foundLeftRear = false;
bool foundLeftFront = false;

bool foundRightFront = false;
bool foundRightRear = false;

short u5_edge = RISE;
long u5_time = 0;

int u5_time_i;
int u5_time_f;

short u6_edge = RISE;
long u6_time = 0;

int u6_time_i;
int u6_time_f;

short u7_edge = RISE;
long u7_time = 0;

int u7_time_i;
int u7_time_f;

short u8_edge = RISE;
long u8_time = 0;

int u8_time_i;
int u8_time_f;

void initSideUltras( void ){
    U5_RPOreg = OC1port; // set ultra1 RPO register to OC1 output
    U6_RPOreg = OC2port;

    U7_RPOreg = OC1port;
    U8_RPOreg = OC2port;

    U5_RBreg = INPUT; // Set ultra1 Tris RB register to input mode
    U6_RBreg = INPUT;
    U7_RBreg = INPUT;
    U8_RBreg = INPUT;
    
    RPINR7bits.IC1R = U5_RPIport; // set IC1 input to ultra1 RP input
    RPINR7bits.IC2R = U6_RPIport;
    RPINR8bits.IC3R = U7_RPIport;
    RPINR8bits.IC4R = U8_RPIport;

    //Setup OCM timers
    TMR3 = 0;   // clear
    T3CONbits.TON = 0; // turn off

    T3CONbits.TCKPS = 0b01; // set prescaler to 1:8
    OC1CON1 = 0; // clear control registers
    OC1CON2 = 0;

    OC1CON1bits.OCTSEL = 0x001; // set to TMR3
    OC1CON1bits.OCM = 0b110; // set to edge-aligned PWM
    OC1CON2bits.SYNCSEL = 0x1F; // set period control to OC1RS

    OC1RS = 35000; // set period of OC1
    OC1R = 15000; // set duration of OC1

    OC2CON1 = 0; // clear control registers
    OC2CON2 = 0;

    OC2CON1bits.OCTSEL = 0x001; // set to TMR3
    OC2CON1bits.OCM = 0b110; // set to edge-aligned PWM
    OC2CON2bits.SYNCSEL = 0x1F; // set period control to O2RS

    OC2RS = 30000; // set period of OC1
    OC2R = 10000; // set duration of OC1

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

double convertToDistance(double time){
    // take in time and convert to distance
    double distance = 0;

    // S = Cair * time (S = distance traveled)
    // S/2 = distance to object
    distance = (Cair*time) / 2;

    return distance;
}

double findObject(double leftLength, double rightLength){
    // one edge is global_u1_time, other is global_u2_time

    // perform law of cosines, let u1 = a, u2 = b, and base = c
    double preAngleA = (rightLength * rightLength + sideLength * sideLength - leftLength * leftLength) / (2 * rightLength * sideLength);
    double angleA = acos(preAngleA) * 180 / pi;

    double preAngleB = (leftLength * leftLength + sideLength * sideLength - rightLength * rightLength) / (2 * leftLength * sideLength);
    double angleB = acos(preAngleB) * 180 / pi;

    double angleDiff = angleA - angleB;

    // find distance to the object
    double baseDistance = leftLength * (sin(angleA * pi / 180));

    // DEBUGGING
    // if we are too close, then we need to backup/turn
    /*
    if (baseDistance > 40)
    {
    }
    */

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

void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void)
{
    _IC2IF = 0;

    if((u5_edge == RISE) && (U5_RBIport == HIGH)){
        u5_time_i = IC1BUF;
        u5_edge = FALL;

        ULTRA_RIGHT_FRONT_DISTANCE = 0;

        foundRightFront = false;
    }else{
        u5_time_f = IC1BUF;
        u5_edge = RISE;
        u5_time = u5_time_i - u5_time_f;

        rightLeftLength = convertToDistance(u5_time * timerPeriod);

        ULTRA_RIGHT_FRONT_DISTANCE = (int)rightLeftLength;

        foundRightFront = true;
    }
}

void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void)
{
    _IC2IF = 0;

    if((u6_edge == RISE) && (U6_RBIport == HIGH)){
        u6_time_i = IC2BUF;
        u6_edge = FALL;

        ULTRA_RIGHT_REAR_DISTANCE = 0;

        foundRightRear = false;
    }else{
        u6_time_f = IC2BUF;
        u6_edge = RISE;
        u6_time = u6_time_i - u6_time_f;

        rightRightLength = convertToDistance(u6_time * timerPeriod);

        ULTRA_RIGHT_REAR_DISTANCE = (int)rightRightLength;

        foundRightRear = true;
    }
}

void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void)
{
    _IC3IF = 0;

    if((u7_edge == RISE) && (U7_RBIport == HIGH)){
        u7_time_i = IC2BUF;
        u7_edge = FALL;

        ULTRA_LEFT_FRONT_DISTANCE = 0;

        foundLeftFront = false;
    }else{
        u7_time_f = IC2BUF;
        u7_edge = RISE;
        u7_time = u7_time_i - u7_time_f;

        leftRightLength = convertToDistance(u7_time * timerPeriod);

        ULTRA_LEFT_FRONT_DISTANCE = (int)leftRightLength;

        foundLeftFront = true;
    }
}

void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void)
{
    _IC4IF = 0;

    if((u8_edge == RISE) && (U8_RBIport == HIGH)){
        u8_time_i = IC4BUF;
        u8_edge = FALL;

        ULTRA_LEFT_REAR_DISTANCE = 0;

        foundLeftRear = false;
    }else{
        u8_time_f = IC4BUF;
        u8_edge = RISE;
        u8_time = u8_time_i - u8_time_f;

        leftLeftLength = convertToDistance(u8_time * timerPeriod);

        ULTRA_LEFT_REAR_DISTANCE = (int)leftLeftLength;

        foundLeftRear = true;
    }
}

int main()
{
    initSideUltras();
    configSPICommunication();
    Cair = 33150 + (60 * temp);
    while(1)
    {
        if (foundLeftRear && foundLeftFront)
        {
            findObject(leftLeftLength, leftRightLength);
            foundLeftRear = false;
            foundLeftFront = false;
        }

        if (foundRightFront && foundRightRear)
        {
            findObject(rightLeftLength, rightRightLength);
            foundRightFront = false;
            foundRightRear = false;
        }
    }

    return 0;
}
