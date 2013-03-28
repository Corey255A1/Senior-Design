/* 
 * File:   frontUltrasonic.h
 * Author: Dan
 *
 * Created on March 12, 2013, 5:13 PM
 */


#include <p24EP32MC202.h>

// BROUGHT OVER FROM ULTRASONIC.H FROM MAIN PROJECT
#define U1_RPOreg RPOR4bits.RP42R   // output

#define U2_RPIport 47 // This is the RPI number which reads the ultra pulse
#define U2_RBIport _RB15
#define U2_RBreg TRISBbits.TRISB15

#define U3_RPIport 46 // This is the RPI number which reads the ultra pulse
#define U3_RBIport _RB14
#define U3_RBreg TRISBbits.TRISB14

// Back Ultrasonic
#define U4_RPOreg RPOR4bits.RP43R // This is the reg that outputs

#define U4_RPIport 45
#define U4_RBIport _RB13
#define U4_RBreg TRISBbits.TRISB13

void initFrontUltras(void);
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void);

double convertToDistance(double time);
double findObject(void);
