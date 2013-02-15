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


int main( void ){
//    TRISB = 0;
//    PORTB = 0;

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
    char tempB;
    i2c_Init();
    i2c_ResetBus();
    i2c_Start(mag_i2c_addr,I2C_WRITE);
    i2c_Write(0x00);
    i2c_Write(0x18);
    i2c_Write(0x01);
    i2c_Write(0x60);
    i2c_ResetBus();

    i2c_Start(acc_i2c_addr,I2C_WRITE);
    i2c_Write(0x20);
    i2c_Write(0x37);
    i2c_ResetBus();

    i2c_Start(gyr_i2c_addr,I2C_WRITE);
    i2c_Write(0x16);
    i2c_Write(0x18);
    i2c_ResetBus();

    i2c_Start(mag_i2c_addr,I2C_WRITE);
    i2c_Write(0x03);
    i2c_Restart(mag_i2c_addr,I2C_READ);
    tempB = i2c_Read();
    i2c_ResetBus();

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
