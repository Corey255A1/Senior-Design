/**
 * --Servo Control--
 * Initialize with the Fast PWM Mode for the Servo
 * The servo in this case has been put on the PWM2L port
 * not super general but was more for testing
 *
 */
#include "servo.h"
#include <p24EP32MC202.h>
#include "globals.h"

void initServo( void ){

    _TRISB13 = OUTPUT;//make PWM servo port an output.
    PTCONbits.PTEN = 0;//Turn of the PWM Module
    PTCONbits.SYNCPOL = 0;//SYNCO output is active high?
    PTCONbits.SYNCOEN = 1;//Enable SYNCO output - Sync with master time
    PTCON2 = 3; //set to 1:8 prescaler
    PTPER = 20000;//master PWM period
    IOCON2=0;//clear
    IOCON1=0;//clear
    IOCON2bits.PENL = 1; //turn on PWM2L
    IOCON2bits.POLL = 0; //make PWM2L active high
    PWMCON2 =0; //clear PWMCON2
    PWMCON2bits.CAM=0; //EDGE aligned mode
    PHASE2 = 0;// 0 phase
    PDC2 = 18000;//initial duty cycle
    DTR2=0;//no dead zone
    PTCONbits.PTEN = 1;//enable the PWM
}//end init

/**
 * Position the servo with a value between 0-1800
 * @param pos - servo position
 */

void posServo( int pos ){
    if(pos<=servo_STEPS && pos>=0)
        PDC2 = servo_MIN + pos;
    else
        PDC2 = servo_MAX;
}//end posServo
