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
#include "globals.h"


int main( void ){
    TRISB = 0;
    PORTB = 0;

    //Initialize the Ultrasonic which is setup to run on PIN12-RP20
    //See the Ultrasonic.c file to check setup.
    initUltra();
    initTemperature();

/**
 * Test Signal to ensure PIC operation when testing
 * 
 */
    _RB5=0;
    int i=0;
    int theTempC;
    int theTempF;
    setGlobalTemp();
    while(1)
    {
        for(i=0;i<10000;i++){
            Nop();
        }//end for
        if(global_u1_time>0x400){
            _RB5 = HIGH;
           theTempF = readTemperature();
        }
        else
            _RB5 = LOW;
    };//end while
    return 1;
}//end main
