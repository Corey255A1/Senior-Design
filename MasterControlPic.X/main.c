/* 
 * File:   main.c
 * Author: Corey
 *
 * Created on March 26, 2013, 7:57 PM
 */

#include <p24FJ128GB106.h>
#include "configs.h"
#include "serialcomm.h"
#include "globals.h"
#include "string.h"
#include "stdlib.h"
#include "communication.h"
#include "motordrive.h"
#include "spi.h"
#include "A2D.h"
#include "temperature.h"
#include "nav.h"
#include "../../Senior-Design/Global_PIC/spiMessages.h"
#define COMPASS_FIXED 8192
/*
 * This is the MCP main; Basically all it does is spin inside it's
 * while loop and wait for a serial message to parse and take
 * appropriate actions
 */
//int FAKE_HEADING = (int)((double)3.14*(8192));
unsigned int headingRaw;
//unsigned int headingSet;
//double heading;
int radSet=0;
int gyro_Z=0;

//Take in the accumulator value and the value to compare too
//return whether or not the acc val lies with in the intended value
int compareGyro(double acc, int compRad){
    double theDeg = ((double)compRad/(double)COMPASS_FIXED * 180)/(double)pi;
    if(acc<0){
        acc = -acc;
    }//if
    if((acc>theDeg-GYRO_ERROR) && (acc<theDeg+GYRO_ERROR)){
        return 1;
    }
    else{
        return 0;
    }
}//endcompare

int main( void ) {
    initSerial1();
    configOutputCompare();
    configInputCaptures();
    initSPI();
    initADC();
    initGyroAcc();
//setDistance(75);
useCompass = 1;
//headingSet = 35000;
setMotor(
       (0x0 MOTOR_LEFT_SPEED) |
       (MOTOR_FWD MOTOR_LEFT_DIR) |
       (0x0 MOTOR_RIGHT_SPEED) |
       (MOTOR_FWD MOTOR_RIGHT_DIR)
        );
//while(1){
//        headingRaw = readSlave(SENSOR_BOARD,COMPASS_HEADING);
//        heading = (double) headingRaw / (double) COMPASS_FIXED_POINT;
//        f_Heading = compareCompass(headingRaw,headingSet);
//}
//radSet = (int)((double)(pi/2)*COMPASS_FIXED);
//useCompass = 1;
//setMotor(
//       (0x4 MOTOR_LEFT_SPEED) |
//       (MOTOR_REV MOTOR_LEFT_DIR) |
//       (0x4 MOTOR_RIGHT_SPEED) |
//       (MOTOR_FWD MOTOR_RIGHT_DIR)
//        );
//while(1){
//        gyro_Z = readSlave(SENSOR_BOARD,GYRO_ZAXIS);
//        setGyro(gyro_Z);
//        f_Heading = compareGyro(accDegrees,radSet);
//}//while
resetGyroAccum();
    while(1){
//        headingRaw = readSlave(SENSOR_BOARD,COMPASS_HEADING);
//        heading = (double) headingRaw / (double) COMPASS_FIXED_POINT;
//        f_Heading = compareCompass(headingRaw,headingSet);
        gyro_Z = readSlave(SENSOR_BOARD,GYRO_ZAXIS);
        setGyro(gyro_Z);
        f_Heading = compareGyro(accDegrees,radSet);
        if(RXMessage.Received){
            switch(RXMessage.Msg[GSHEADER]){
                case SET:
                    switch(RXMessage.Msg[DEVICEHEADER]){
                        case DCMOTOR:
                            setDistance(RXMessage.Msg[DEVICEHEADER+5]);
                            useCompass = 0;
                            if(RXMessage.Msg[DEVICEHEADER+6]){
                                radSet = ((0x00FF&RXMessage.Msg[DEVICEHEADER+7])<<8)|(0x00FF&RXMessage.Msg[DEVICEHEADER+8]);
                                useCompass = 1;
                                resetGyroAccum();
                            };
                            currentMotorSetting = (RXMessage.Msg[DEVICEHEADER+3] MOTOR_LEFT_SPEED) |
                                               (RXMessage.Msg[DEVICEHEADER+4] MOTOR_LEFT_DIR) |
                                               (RXMessage.Msg[DEVICEHEADER+1] MOTOR_RIGHT_SPEED) |
                                               (RXMessage.Msg[DEVICEHEADER+2] MOTOR_RIGHT_DIR);
                            setMotor(currentMotorSetting);
                            SEND_ACK;
                            break;
                        case ARM:
                            writeSlave(STEPPER_DRIVER1,SPI_STEPPER_X,RXMessage.Msg[DEVICEHEADER+1]);
                            writeSlave(STEPPER_DRIVER1,SPI_STEPPER_Y,RXMessage.Msg[DEVICEHEADER+2]);
                            writeSlave(STEPPER_DRIVER2,SPI_STEPPER_X,RXMessage.Msg[DEVICEHEADER+1]);
                            writeSlave(STEPPER_DRIVER2,SPI_STEPPER_Y,RXMessage.Msg[DEVICEHEADER+2]);
                            //writeSlave(STEPPER_DRIVER3,SPI_STEPPER_X,RXMessage.Msg[DEVICEHEADER+1]);
                            //writeSlave(STEPPER_DRIVER3,SPI_STEPPER_Y,RXMessage.Msg[DEVICEHEADER+2]);
                            SEND_ACK;
                            break;
                    }//switch
                    break;
                case GET:
                    switch(RXMessage.Msg[DEVICEHEADER]){
                        case DCMOTOR:{
                            char txMSG[11];
                            txMSG[0] = '!';
                            txMSG[1] = 9;
                            txMSG[2] = 'M';
                            txMSG[3] = IC1_COUNTS_BYTE4;
                            txMSG[4] = IC1_COUNTS_BYTE3;
                            txMSG[5] = IC1_COUNTS_BYTE2;
                            txMSG[6] = IC1_COUNTS_BYTE1;
                            txMSG[7] = IC2_COUNTS_BYTE4;
                            txMSG[8] = IC2_COUNTS_BYTE3;
                            txMSG[9] = IC2_COUNTS_BYTE2;
                            txMSG[10] = IC2_COUNTS_BYTE1;
                            txSerial1(txMSG,11);
                            break;}
                        case ARM:{
                         //TO-DO Figure out what to return from the ARM
                         //If this is infact needed
                        }break;
                        case SENSORS:
                            switch(RXMessage.Msg[SENSEHEADER]){
                                case TEMP:{
                                    char txMSG[5];
                                    int tempSPI;
                                    txMSG[0] = '!';
                                    txMSG[1] = 3;
                                    txMSG[2] = 'S';
                                    tempSPI = readTemperature(CEL);
                                    txMSG[3] = (tempSPI&0xFF00)>>8;
                                    txMSG[4] = (tempSPI&0x00FF);
                                    txSerial1(txMSG,5);
                                    break;}
                                case WLED:{
                                    char txMSG[5];
                                    int tempSPI;
                                    txMSG[0] = '!';
                                    txMSG[1] = 3;
                                    txMSG[2] = 'S';
                                    tempSPI = readADC(WLED_AD);
                                    txMSG[3] = (tempSPI&0xFF00)>>8;
                                    txMSG[4] = (tempSPI&0x00FF);
                                    txSerial1(txMSG,5);
                                    break;}
                                case IRLED:{
                                    char txMSG[5];
                                    int tempSPI;
                                    txMSG[0] = '!';
                                    txMSG[1] = 3;
                                    txMSG[2] = 'S';
                                    tempSPI = readADC(ILED_AD);
                                    txMSG[3] = (tempSPI&0xFF00)>>8;
                                    txMSG[4] = (tempSPI&0x00FF);
                                    txSerial1(txMSG,5);
                                    break;}
                                case COMPASS:{
                                    char txMSG[5];
                                    int tempSPI;
                                    txMSG[0] = '!';
                                    txMSG[1] = 3;
                                    txMSG[2] = 'S';
                                    tempSPI = readSlave(SENSOR_BOARD,COMPASS_HEADING);
                                    txMSG[3] = (headingRaw&0xFF00)>>8;
                                    txMSG[4] = (headingRaw&0x00FF);
                                    txSerial1(txMSG,5);
                                    break;}
                                case GYRO:{
                                    double magAccDeg;
                                    if(accDegrees<0){
                                        magAccDeg = -accDegrees;
                                    }else{
                                        magAccDeg = accDegrees;
                                    }
                                    int accRadFixed = (int)(((magAccDeg * pi)/180)*COMPASS_FIXED);
                                    char txMSG[5];
                                    txMSG[0] = '!';
                                    txMSG[1] = 3;
                                    txMSG[2] = 'S';
                                    txMSG[3] = (accRadFixed&0xFF00)>>8;
                                    txMSG[4] = (accRadFixed&0x00FF);
                                    txSerial1(txMSG,5);
                                    break;
                                }
                                case ULTRAS:{
                                    char txMSG[15];
                                    int tempSPI;
                                    txMSG[0] = '!';
                                    txMSG[1] = 13;
                                    txMSG[2] = 'S';
                                    
                                    //LF
                                    tempSPI = readSlave(S_ULTRAS,ULTRA_LEFT_FRONT_DISTANCE);
                                    txMSG[3] = (tempSPI&0xFF00)>>8;
                                    txMSG[4] = (tempSPI&0x00FF);
                                    //LB
                                    tempSPI = readSlave(S_ULTRAS,ULTRA_LEFT_BACK_DISTANCE);
                                    txMSG[5] = (tempSPI&0xFF00)>>8;
                                    txMSG[6] = (tempSPI&0x00FF);
                                    
                                    //RF
                                    tempSPI = readSlave(S_ULTRAS,ULTRA_RIGHT_FRONT_DISTANCE);
                                    txMSG[7] = (tempSPI&0xFF00)>>8;
                                    txMSG[8] = (tempSPI&0x00FF);
                                    //RB
                                    tempSPI = readSlave(S_ULTRAS,ULTRA_RIGHT_BACK_DISTANCE);
                                    txMSG[9] = (tempSPI&0xFF00)>>8;
                                    txMSG[10] = (tempSPI&0x00FF);

                                    //F
                                    tempSPI = readSlave(FB_ULTRAS,ULTRA_FRONT_DISTANCE);
                                    txMSG[11] = (tempSPI&0xFF00)>>8;
                                    txMSG[12] = (tempSPI&0x00FF);
                                    //B
                                    tempSPI = readSlave(FB_ULTRAS,ULTRA_BACK_DISTANCE);
                                    txMSG[13] = (tempSPI&0xFF00)>>8;
                                    txMSG[14] = (tempSPI&0x00FF);

                                    txSerial1(txMSG,15);
                                    break;}
                            }//switch - sensors
                            break;
                    }//switch - device
                    break;
            }//switch - get/set
            RXMessage.Received = 0;
        }//if
       // msDelay(10);
    }//while
    return (0);
}//main

//char* briand = "Briand";
//if(strncmp(msg,briand,strlen(briand))==0){
//    txSerial1_str("\r\nHe a baller. Gets Badass bitches :^)\r\n");
//}else if(strncmp(msg,"Does Dan Suck?!",15)==0){
//    txSerial1_str("\r\nNo, he is dastardly, but cool. :^)\r\n");
//}else if(strncmp(msg,"Does Corey Suck?!",17)==0){
//    txSerial1str("\r\nNo he is cool.\r\n");
//}else if(strncmp(msg,"Does Nick Suck?!",16)==0){
//    txSerial1_str("\r\nHe is a Greek bastard!!\r\n");
//}else if(strncmp(msg,"clear!",6)==0){
//    txSerial1_str("\f");
//}else if(strncmp(msg,"SA",2)==0){
//    writeSlave(STEPPER_DRIVER,0,msg[2]);
//}else if(strncmp(msg,"GS",2)==0){
//    data = readSlave(STEPPER_DRIVER,0);
//    char data1 = (char)((0xFF00&data)>>8);
//    char data2 = (char)(0xFF&data);
//    char d[4] = {'F',data1,data2,'!'};
//    data=0;
//    int sizeofmyd =sizeof(d);
//    txSerial1(d,sizeofmyd);
//}else{
//    txSerial1_str("\r\n");
//    txSerial1_str(msg);
//    txSerial1_str("\r\n");
//}