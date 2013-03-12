/* 
 * File:   frontUltrasonic.h
 * Author: Dan
 *
 * Created on March 12, 2013, 5:13 PM
 */


#include <p24EP32MC202.h>

// BROUGHT OVER FROM ULTRASONIC.H FROM MAIN PROJECT
#define U1_RPIport 47 // This is the RPI number which reads the ultra pulse
#define U1_RBIport _RB15
#define U1_RBreg TRISBbits.TRISB15
#define U1_RPOreg RPOR0bits.RP20R

#define U2_RPIport 46 // This is the RPI number which reads the ultra pulse
#define U2_RBIport _RB14
#define U2_RBreg TRISBbits.TRISB14
#define U2_RPOreg RPOR2bits.RP38R // This is the reg that outputs

void initFrontUltras(void);
// THESE ARE OVERLAPPING WITH ULTRASONIC.H
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void);

double convertToDistance(int time);
double findObject(void);
