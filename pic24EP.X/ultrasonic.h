/**
 * -- Ultrasonic Header --
 * These Defines masks the choises that I made so that if the port number
 * changes al I have to do is change these definitions
 */
#include <p24EP32MC202.h>
#define U1_RPIport 47 // This is the RPI number which reads the ultra pulse
#define U1_RBIport _RB15
#define U1_RBreg TRISBbits.TRISB15
#define U1_RPOreg RPOR0bits.RP20R

#define U2_RPIport 46 // This is the RPI number which reads the ultra pulse
#define U2_RBIport _RB14
#define U2_RBreg TRISBbits.TRISB14
#define U2_RPOreg RPOR2bits.RP38R // This is the reg that outputs

void initUltra( void );
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void);
