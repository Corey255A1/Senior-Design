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
#include "../Global_PIC/spiMessages.h"
#define WAIT_FOR_MSG 0x11

enum CARDINAL {NORTH, EAST, SOUTH, WEST};
int main( void ){
    i2c_Init();
    init9axis();
//SPI communication Test Code
    configSPICommunication();
    _TRISB4 = OUTPUT;
    _RB4 = LOW;
    int STATE=WAIT_FOR_MSG;
    double heading=0.3;
    enum CARDINAL dir = NORTH;
    SPI1BUF = DATA_RDY;
    while(1){
        switch(STATE){
            case WAIT_FOR_MSG:
                if(SPI1STATbits.SPIRBF){
                    switch(SPI1BUF){
                        case WRITE_DATA:
                            //SPI1BUF = WRITE_DATA;
                            STATE = WRITE_DATA;break;
                        case READ_DATA:
                            //SPI1BUF = READ_DATA;
                            STATE = READ_DATA;break;
                    }//endswitch
                    //SPI_msg_rdy = DIS;//clear out msg_rdy to receive next
                }//end if
                else{
                    if(f_UPDATE_9AXIS){
                        SPI1BUF = DATA_PROCESSING;
                        heading = updateHeading();
                        if(heading>-.75 && heading<.75){
                            dir = NORTH;
                        }else if (heading>-2.25 && heading<=-.75){
                            dir = EAST;
                        }else if (heading<2.25 && heading>=.75){
                            dir = WEST;
                        }else if ((heading<3.2 && heading >=2.25)||(heading>-3.2 && heading <=-2.25)){
                            dir = SOUTH;
                        }//endif
                        f_UPDATE_9AXIS=0;
                        SPI1BUF = DATA_RDY;
                    }//end IF
                }//end else

                break;//break formsg
            case WRITE_DATA:
                while(!SPI1STATbits.SPIRBF);
                    switch(SPI1BUF){
                        case OUT_ON: SPI1BUF = OUT_ON; _RB4 = HIGH;break;
                        case OUT_OFF: SPI1BUF = OUT_OFF;_RB4 = LOW;break;
                        case END_TRANSMISSION:
                            SPI1BUF = DATA_RDY;
                            STATE = WAIT_FOR_MSG;break;
                        default: SPI1BUF = DATA_RDY; STATE = WAIT_FOR_MSG;break;
                    }//end switch
                    //SPI_msg_rdy = DIS;
                break;//end writedata
            case READ_DATA:
                    while(!SPI1STATbits.SPIRBF);
                    switch(SPI1BUF){
                        case GET_STATE:
                            switch(dir){
                                case NORTH: SPI1BUF='N';break;
                                case SOUTH: SPI1BUF='S';break;
                                case EAST: SPI1BUF='E';break;
                                case WEST: SPI1BUF='W';break;
                            }
                            while(SPI1STATbits.SPITBF);
                            break;
                        case READ: SPI1BUF=0; break; //TO_DO increment through slave transmit buffer to be implemented
                        case END_TRANSMISSION:SPI1BUF = DATA_RDY;
                                              STATE = WAIT_FOR_MSG;break;
                        default: SPI1BUF = DATA_RDY; STATE = WAIT_FOR_MSG;break;
                    }//end switch
                    //SPI_msg_rdy = DIS;
        }//endcase
        SPI1STATbits.SPIROV=CLEAR;
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
