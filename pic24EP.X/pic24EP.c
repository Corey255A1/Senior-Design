#include "configs.h"
#include <p24EP32MC202.h>
#include "ultrasonic.h"
#include "globals.h"

/**
 * Testing the PIC24EP's which will be used for the Slaves
 *
 *
 */

int main( void ){
    TRISB = 0;
    PORTB = 0;

    //Initialize the Ultrasonic which is setup to run on PIN12-RP20
    //See the Ultrasonic.c file to check setup.
    initUltra();

/**
 * Test Signal to ensure PIC operation when testing
 * 
 */
    _RB5=0;
    int i=0;
    while(1)
    {
        for(i=0;i<10000;i++){
            Nop();
        }//end for
        if(global_u1_time>0x400)
            _RB5 = HIGH;
        else
            _RB5 = LOW;
    };//end while
    return 1;
}//end main
