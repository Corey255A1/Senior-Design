/**
 * --Servo Control--
 * Initialize with the Fast PWM Mode for the Servo
 * The servo in this case has been put on the PWM2L port
 * not super general but was more for testing
 *
 * --Corey Wunderlich
 * 
 */
#include "servo.h"
#include <p24EP32MC202.h>
#include "globals.h"

void initServo( void ){

    _TRISB10 = OUTPUT;//make PWM servo port an output.
    RPOR4bits.RP42R = 0b010000;
    //Setup the Output Compare Module timers
    TMR3=0; // Clear out the TMR3
    T3CONbits.TON = 0; // Turn off TMR3
    T3CONbits.TCKPS=0b01; //Set Prescaler to 1:8
    OC1CON1=0; //clear out control regs
    OC1CON2=0; //clear out control regs
    OC1CON1bits.OCTSEL = 0x001; //set to TMR3
    OC1CON1bits.OCM=0b110; //set to EDGE-ALIGNED PWM
    OC1CON2bits.SYNCSEL = 0x1F; //set 87eriod control to OC1RS
    OC1RS = 10000; //Set the Period of OC1
    OC1R = 1040; //set the Duration of OC1
    T3CONbits.TON = 1;
}//end init

/**
 * Position the servo with a value between 0-1800
 * @param pos - servo position
 */
void posServo( int pos ){
    if((SERVO_MIN + pos)<SERVO_MAX){
        OC1R = SERVO_MIN + pos;
    }
}//end posServo
