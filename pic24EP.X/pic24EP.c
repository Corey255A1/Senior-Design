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

enum CARDINAL {NORTH, EAST, SOUTH, WEST};
enum STATES {NO_MESSAGE, MESSAGE_RECV, WRITE_MODE, READ_MODE, PROCESS_DATA};
//Local Variables
char data;
enum CARDINAL local_dir = NORTH;
enum STATES local_state = NO_MESSAGE;
double heading=0;

char getData(){
    char rdata=0;
      switch(local_dir){
        case NORTH: rdata='N';break;
        case SOUTH: rdata='S';break;
        case EAST: rdata='E';break;
        case WEST: rdata='W';break;
    }
    return rdata;
}
void doWork(){
   if(f_UPDATE_9AXIS){
        heading = updateHeading();
        if(heading>-.75 && heading<.75){
            local_dir = NORTH;
        }else if (heading>-2.25 && heading<=-.75){
            local_dir = EAST;
        }else if (heading<2.25 && heading>=.75){
            local_dir = WEST;
        }else if ((heading<3.2 && heading >=2.25)||(heading>-3.2 && heading <=-2.25)){
            local_dir = SOUTH;
        }//endif
        f_UPDATE_9AXIS=0;
    }//end IF 
}

int main( void ){
    i2c_Init();
    init9axis();
//SPI communication Test Code
    configSPICommunication();
    _TRISB4 = OUTPUT;
    _RB4 = LOW;


    SPI1BUF = DATA_RDY;
    while(1){
        switch(local_state){
            case NO_MESSAGE:
                if(SPI1STATbits.SPIRBF){
                    local_state = MESSAGE_RECV;
                }//end if
                else{
                    local_state = PROCESS_DATA;
                }//end else
                break;//break formsg
            case PROCESS_DATA:
                writeSPI_slave(DATA_PROCESSING);
                doWork();
                writeSPI_slave(DATA_RDY);
                local_state = NO_MESSAGE;
                break;
            case MESSAGE_RECV:
                switch(SPI1BUF){
                    case WRITE_DATA:
                        writeSPI_slave(WRITE_DATA);
                        local_state = WRITE_MODE;
                        break;
                    case READ_DATA:
                        writeSPI_slave(READ_DATA);
                        local_state = READ_MODE;
                        break;
                    default:
                        writeSPI_slave(DATA_ERROR);
                        local_state = NO_MESSAGE;
                        break;
                    }//endswitch
                break;
            case WRITE_MODE:
                while(!SPI1STATbits.SPIRBF);
                    switch(SPI1BUF){
                        case OUT_ON:
                            writeSPI_slave(OUT_ON);
                            _RB4 = HIGH;
                            break;
                        case OUT_OFF:
                            writeSPI_slave(OUT_OFF);
                            _RB4 = LOW;
                            break;
                        case END_TRANSMISSION:
                            writeSPI_slave(END_TRANSMISSION);
                            local_state=NO_MESSAGE;
                            break;
                        default: 
                            writeSPI_slave(DATA_ERROR);
                            local_state = NO_MESSAGE;
                            break;
                    }//end switch
                break;//end writedata
            case READ_MODE:
                while(!SPI1STATbits.SPIRBF);
                    switch(SPI1BUF){
                        case GET_STATE:
                            writeSPI_slave(GET_STATE);
                            writeSPI_slave('A');
                            break;
                        case READ: 
                            writeSPI_slave(0);
                            break; //TO_DO increment through slave transmit buffer to be implemented
                        case END_TRANSMISSION:
                            writeSPI_slave(END_TRANSMISSION);
                            local_state=NO_MESSAGE;
                            break;
                        default:
                            writeSPI_slave(DATA_ERROR);
                            local_state = NO_MESSAGE;
                            break;
                    }//end switch
            }//endcase

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
