#include "servo.h"
#include <p24EP32MC202.h>
#include "globals.h"
void initstepper( void ){
    _TRISB15 = OUTPUT;//make PWM servo port an output.
    _TRISB14 = OUTPUT;//make PWM servo port an output.
    _TRISB13 = OUTPUT;//make PWM servo port an output.
    _TRISB12 = OUTPUT;//make PWM servo port an output.
    PTCONbits.PTEN = 0;//Turn of the PWM Module
    PTCONbits.SYNCPOL = 0;//SYNCO output is active high?
    PTCONbits.SYNCOEN = 1;//Enable SYNCO output - Sync with master time
    PTCON2 = 3; //set to 1:8 prescaler
    PTPER = 20000;//master PWM period
    IOCON2=0;//clear
    IOCON1=0;//clear
    IOCON2bits.PENL = 1; //turn on PWM2L
    IOCON2bits.PENH = 1; //turn on PWM2H
    IOCON1bits.PENL = 1; //turn on PWM1L
    IOCON1bits.PENH = 1; //turn on PWM1H
    IOCON2bits.POLL = 0; //make PWM2L active high
    IOCON2bits.POLH = 0; //make PWM2H active high
    IOCON1bits.POLL = 0; //make PWM1L active high
    IOCON1bits.POLH = 0; //make PWM1H active high
    IOCON2bits.PMOD=0;//Complementary
    IOCON1bits.PMOD=0;//Complementary
    PWMCON2 =0; //clear PWMCON2
    PWMCON2bits.CAM=0; //EDGE aligned mode
    PWMCON1 =0; //clear PWMCON2
    PWMCON1bits.CAM=0; //EDGE aligned mode
    PHASE1 = 0;
    PHASE2 = 5000;// 0 phase
    PDC2=PDC1 = 10000;//initial duty cycle
    DTR1=DTR2=0;//no dead zone
    PTCONbits.PTEN = 1;//enable the PWM
}
