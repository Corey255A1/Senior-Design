/**
 * -- General Functionality Testing Main --
 * Testing the PIC24EP's which will be used for the Slaves
 *
 *
 */

#include "configs.h"
#include <p24EP32MC202.h>
#include "ultrasonic.h"
#include "temperature.h"
#include "servo.h"
#include "globals.h"


int main( void ){
    TRISB = 0;
    PORTB = 0;

    //Initialize the Ultrasonic which is setup to run on PIN12-RP20
    //See the Ultrasonic.c file to check setup.
    initUltra();
    initTemperature();
    initServo();
    setGlobalTemp();

/**
 * Test Signal to ensure PIC operation when testing
 * 
 */
    _RB5=0;
    int i=0;
    int theTempC=0;
    int theTempF=0;
    int theTempK=0;
    short step=1;
    int pos=0;
    posServo(0);
    while(1)
    {
        for(i=0;i<10000;i++){
            Nop();
        }//end for
        
        theTempC = readTemperature(CEL);
        theTempF = readTemperature(FAR);
        theTempK = readTemperature(KEL);


        pos=pos+step;
        posServo(pos);
        if(pos == servo_STEPS){
            step=-step;
        }else if(pos == 0){
            step=-step;
        }//endif

    };//end while
    return 1;
}//end main
