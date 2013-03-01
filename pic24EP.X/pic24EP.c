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
#include "i2c.h"
#include "9axis.h"
#include "spi.h"


int main( void ){
//SPI communication Test Code
    configSPICommunication();
    _TRISB6 = OUTPUT;
    _TRISB5 = OUTPUT;
    _RB6 = LOW;
    _RB5 = LOW;
    while(1){
        _RB5 = ~_RB5;
        if(SPImsg == EN){
            SPImsg = DIS;
            if(spiReadVal==0xAA){
                _RB6=HIGH;
            }
            if(spiReadVal==0x7F){
                _RB6=LOW;
            }
            spiReadVal = 0;
        }//end if
    }//end while

    //TRISB = 0;
    //PORTB = 0;
    //Initialize the Ultrasonic which is setup to run on PIN12-RP20
    //See the Ultrasonic.c file to check setup.
//    initUltra();
//    initTemperature();
//    initServo();
//    setGlobalTemp();

/**
 * Test Signal to ensure PIC operation when testing
 * 
 */
//    init9axis();
//    initServo();
   // int magArr[3];
   // int accArr[3];
   // int gyrArr[3];
//    double heading;
//    _TRISB6 = OUTPUT;
//    _RB6 = 0;
//    while(1){
//        heading = getHeading();
//        if (heading<=.1 && heading>=-.1)
//            _RB6=1;
//        else
//            _RB6=0;
//        float h2s =(pi + heading)*900/pi;
//        posServo((int)h2s);
//    }//end while
//
//    _RB5=0;
//    int i=0;
//    int theTempC=0;
//    int theTempF=0;
//    int theTempK=0;
//    short step=1;
//    int pos=0;
//    posServo(0);
//    while(1)
//    {
//        for(i=0;i<10000;i++){
//            Nop();
//        }//end for
//
//        theTempC = readTemperature(CEL);
//        theTempF = readTemperature(FAR);
//        theTempK = readTemperature(KEL);
//
//
//        pos=pos+step;
//        posServo(pos);
//        if(pos == servo_STEPS){
//            step=-step;
//        }else if(pos == 0){
//            step=-step;
//        }//endif
//
//    };//end while
//    return 1;
}//end main
