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
#include "stepper.h"
#include "../Global_PIC/spiMessages.h"

enum CARDINAL {NORTH, EAST, SOUTH, WEST};
enum STATES {NO_MESSAGE, MESSAGE_RECV, WRITE_MODE, READ_MODE, PROCESS_DATA};
//Local Variables
char data;
enum CARDINAL local_dir = NORTH;
enum STATES local_state = NO_MESSAGE;
double heading=0;

//char getData(){
//
//    return rdata;
//}
//void doWork(){
//
//}

int main( void ){
//    i2c_Init();
//    init9axis();
//    SLAVEData.inData[LED_OUT]=0;
//    SLAVEData.outData[COMPASS_HEADING]='C';
//    SPI1BUF = DATA_RDY;
//    while(1){
//       if(f_UPDATE_9AXIS){
//            heading = updateHeading();
//            if(heading>-.75 && heading<.75){
//                local_dir = NORTH;
//            }else if (heading>-2.25 && heading<=-.75){
//                local_dir = EAST;
//            }else if (heading<2.25 && heading>=.75){
//                local_dir = WEST;
//            }else if ((heading<3.2 && heading >=2.25)||(heading>-3.2 && heading <=-2.25)){
//                local_dir = SOUTH;
//            }//endif
//            f_UPDATE_9AXIS=0;
//          char rdata=0;
//          switch(local_dir){
//            case NORTH: rdata='N';break;
//            case SOUTH: rdata='S';break;
//            case EAST: rdata='E';break;
//            case WEST: rdata='W';break;
//        }
//          SLAVEData.outData[COMPASS_HEADING]=rdata;
//        }//end IF
//       _RB4 = SLAVEData.inData[LED_OUT];
//    }//end while

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
    //SPI communication Test Code
//    configSPICommunication();
//    //initTemperature();
//    _TRISB4 = OUTPUT;
//    _RB4 = HIGH;
//    while(1)
//    {
//        // SLAVEData.outData[CURRENT_TEMPERATURE] = readTemperature(FAR);
//         _RB4 = SLAVEData.inData[LED_OUT];
//    };//end while
//    return 1;
    PORTB = 0;
    initServo();
    posServo(1000);
    while(1){};
}//end main
