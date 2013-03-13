/**
 * --Stepper Control--
 * Handles the stepper motor initialization
 * and also provides functions to move the stepper
 * a specified amount
 * Also can set the direction and speed of the stepper
 *
 * --Corey Wunderlich
 *
 */

#include "stepper.h"
#include <p24EP32MC202.h>
#include "globals.h"

unsigned int local_stepCount=0;
unsigned int local_stepper_speed=50000;

/**
 * Enables all of the stepper motor phases
 * PWM1 and PWM2 both have a high and low
 * Set them up in complementary modes
 * Set them up 90degrees out of phase
 */
void initStepper( void ){
    _TRISB15 = OUTPUT;//make PWM servo port an output.
    _TRISB14 = OUTPUT;//make PWM servo port an output.
    _TRISB13 = OUTPUT;//make PWM servo port an output.
    _TRISB12 = OUTPUT;//make PWM servo port an output.
    PTCONbits.PTEN = 0;//Turn of the PWM Module
    PTCONbits.SYNCPOL = 0;//SYNCO output is active high?
    PTCONbits.SYNCOEN = 1;//Enable SYNCO output - Sync with master time
    PTCON2 = 0; //set to 1:1 prescaler
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
    PTPER = local_stepper_speed;//master PWM period
    PHASE1 = 0;//PHASE1
    PHASE2 = local_stepper_speed/4;//PHASE2 is set to a quater of the speed 90deg
    PDC2=PDC1 = local_stepper_speed/2;
    DTR1=DTR2=0;//no dead zone
    PWMCON2bits.TRGIEN=1;//Turn on Interrupt for PWM2
    IFS5bits.PWM2IF=0;//Clear PWM2 flag
    IEC5bits.PWM2IE=1;//Enable the Interrupts
}
/**
 * The interrupt that catches and clears the PWM enable flag
 * It also decrements the stepCount and clears the PWM
 */
void __attribute__((__interrupt__, auto_psv)) _PWM2Interrupt(void){
    local_stepCount--;
    if(local_stepCount<=0)
        PTCONbits.PTEN = 0;//disable the PWM
    IFS5bits.PWM2IF=0;
}

/**
 * Main code setable of the speed
 * Sets the local speed and the PERIOD
 * and Duty cycles
 * @param speed -- the amount of timer counts
 */
void stepperSpeed(unsigned int speed){
    local_stepper_speed=speed;
       PTPER = speed;//master PWM period
       PDC2=PDC1 = speed/2;
}

/**
 * Sets up the PHASE1 and PHASE2 for corresponding
 * to the direction that is to be set
 * @param steps -- the amount of steps to go
 * @param dir -- the Direction to step
 */
void takeSteps(unsigned int steps, char dir){
    if(dir==FWD){
      PHASE1 = 0;
      PHASE2 = local_stepper_speed/4;
    }else if(dir==REV){
      PHASE1 = local_stepper_speed/4;
      PHASE2 =  0;
    }
   local_stepCount=steps;
   PTCONbits.PTEN = 1;//enable the PWM
}
