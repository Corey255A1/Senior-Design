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
#include "spi.h"
#include "A2D.h"
#include "temperature.h"
#include "../../Senior-Design/Global_PIC/spiMessages.h"
/*
 * This is the MCP main; Basically all it does is spin inside it's
 * while loop and wait for a serial message to parse and take
 * appropriate actions
 */
int main( void ) {
    initSerial1();
    initSPI();
    initADC();
    while(1){
        if(RXMessage.Received){
            switch(RXMessage.Msg[GSHEADER]){
                case SET:
                    switch(RXMessage.Msg[DEVICEHEADER]){
                        case DCMOTOR:
                            writeSlave(MOTOR_DRIVER,
                                   SPI_MOTOR,
                                   (RXMessage.Msg[DEVICEHEADER+1] SPI_MOTOR_LEFT_SPEED) |
                                   (RXMessage.Msg[DEVICEHEADER+2] SPI_MOTOR_LEFT_DIR) |
                                   (RXMessage.Msg[DEVICEHEADER+3] SPI_MOTOR_RIGHT_SPEED) |
                                   (RXMessage.Msg[DEVICEHEADER+4] SPI_MOTOR_RIGHT_DIR));
                            break;
                        case ARM:
                            writeSlave(STEPPER_DRIVER1,SPI_STEPPER_X,RXMessage.Msg[DEVICEHEADER+1]);
                            writeSlave(STEPPER_DRIVER1,SPI_STEPPER_Y,RXMessage.Msg[DEVICEHEADER+2]);
                            writeSlave(STEPPER_DRIVER2,SPI_STEPPER_X,RXMessage.Msg[DEVICEHEADER+1]);
                            writeSlave(STEPPER_DRIVER2,SPI_STEPPER_Y,RXMessage.Msg[DEVICEHEADER+2]);
                            //writeSlave(STEPPER_DRIVER3,SPI_STEPPER_X,RXMessage.Msg[DEVICEHEADER+1]);
                            //writeSlave(STEPPER_DRIVER3,SPI_STEPPER_Y,RXMessage.Msg[DEVICEHEADER+2]);
                            break;
                    }//switch
                    break;
                case GET:
                    switch(RXMessage.Msg[DEVICEHEADER]){
                        case DCMOTOR:{
                            char txMSG[7];
                            int tempSPI;
                            txMSG[0] = '!';
                            txMSG[1] = 5;
                            txMSG[2] = 'M';
                            tempSPI = readSlave(MOTOR_DRIVER,SPI_MOTOR1_STATUS);//to-do
                            txMSG[3] = (tempSPI&0xFF00)>>8;
                            txMSG[4] = (tempSPI&0x00FF);
                            tempSPI = readSlave(MOTOR_DRIVER,SPI_MOTOR2_STATUS);//to-do
                            txMSG[5] = (tempSPI&0xFF00)>>8;
                            txMSG[6] = (tempSPI&0x00FF);
                            txSerial1(txMSG,7);
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
                                    tempSPI = readSlave(COMPASS,COMPASS_HEADING);
                                    txMSG[3] = (tempSPI&0xFF00)>>8;
                                    txMSG[4] = (tempSPI&0x00FF);
                                    txSerial1(txMSG,5);
                                    break;}
                                case ULTRAS:{
                                    char txMSG[5];
                                    int tempSPI;
                                    txMSG[0] = '!';
                                    txMSG[1] = 3;
                                    txMSG[2] = 'S';
                                    tempSPI = readSlave(ULTRAS,ULTRA_FRONT_DISTANCE);
                                    txMSG[3] = (tempSPI&0xFF00)>>8;
                                    txMSG[4] = (tempSPI&0x00FF);
                                    txSerial1(txMSG,5);
                                    break;}
                            }//switch - sensors
                            break;
                    }//switch - device
                    break;
            }//switch - get/set
            RXMessage.Received = 0;
        }//if
        msDelay(10);
    }//while
    return (0);
}//main

//char* briand = "Briand";
//if(strncmp(msg,briand,strlen(briand))==0){
//    txSerial1_str("\r\nHe a baller. Gets Badass bitches :^)\r\n");
//}else if(strncmp(msg,"Does Dan Suck?!",15)==0){
//    txSerial1_str("\r\nNo, he is dastardly, but cool. :^)\r\n");
//}else if(strncmp(msg,"Does Corey Suck?!",17)==0){
//    txSerial1_str("\r\nNo he is cool.\r\n");
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