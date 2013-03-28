/*
 * File:   sideUltrasonic.h
 * Author: Dan
 *
 * Created on March 13, 2013, 11:50 PM
 */

#include <p24EP32MC202.h>

#define U5_RPOreg RPOR4bits.RP43R   // output right front
#define U6_RPOreg RPOR4bits.RP42R   // output right rear

#define U7_RPOreg RPOR1bits.RP36R   // output left front
#define U8_RPOreg RPOR1bits.RP37R   // output left rear

#define U5_RPIport 47 // This is the RPI number which reads the ultra pulse for right front
#define U5_RBIport _RB15
#define U5_RBreg TRISBbits.TRISB15

#define U6_RPIport 46 // This is the RPI number which reads the ultra pulse for right rear
#define U6_RBIport _RB14
#define U6_RBreg TRISBbits.TRISB14

#define U7_RPIport 45 // This is the RPI number which reads the ultra pulse for left front
#define U7_RBIport _RB13
#define U7_RBreg TRISBbits.TRISB13

#define U8_RPIport 44 // This is the RPI number which reads the ultra pulse for left rear
#define U8_RBIport _RB12
#define U8_RBreg TRISBbits.TRISB12

// init for sides
// sides also have IC1 interrupt
// sides also have IC2 interrupt
// sides also have IC3 interrupt
// sides also have IC4 interrupt
// sides also have getDistance method
// sides also have findObject method

void initSideUltras(void);
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void);
double convertToDistance(double time);
double findObject(double leftPulse, double rightPulse);
