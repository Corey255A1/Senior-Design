#include "spi.h"
#include "communication.h"
#include "globals.h"
#include "../Global_PIC/spiMessages.h"
#include "lcd.h"
void writeSPI_master(char msg){
     char feedback;
     _Bool retry=1;
     int retry_cnt=0;
     while(retry && retry_cnt<20){
        retry=0;
        putLCD(msg);
        feedback = writeSPI1(msg);
        putLCD(feedback);

        if(feedback !=msg){
            retry=1;
            retry_cnt++;
            msDelay(200);
            continue;
        }
     }
}

void writeSlave(char msg){
    writeSPI_master(WRITE_DATA);
    cmdLCD(0b00000010); //return home??
    writeSPI_master(msg);
    cmdLCD(0b00000010); //return home??
    writeSPI_master(END_TRANSMISSION);
    cmdLCD(0b00000010); //return home??
//        _Bool retry=1;
//        int retry_cnt=0;
//        char feedback;
//        while(retry && retry_cnt<20){
//            cmdLCD(0b00000010); //return home??
//            retry=0;
//            feedback = writeSPI1(WRITE_DATA);
//            putLCD(feedback);
//            if(feedback !=DATA_RDY && feedback != DATA_ERROR){
//                retry=1;
//                retry_cnt++;
//                msDelay(200);
//                continue;
//            }
//            msDelay(1);
//            feedback = writeSPI1(msg);
//            putLCD(feedback);
//            if(feedback!=WRITE_DATA){
//                retry=1;
//                retry_cnt++;
//                msDelay(200);
//                continue;
//            }
//            msDelay(1);
//            feedback = writeSPI1(END_TRANSMISSION);
//            putLCD(feedback);
//            if(feedback!=msg){
//                retry=1;
//                retry_cnt++;
//                msDelay(200);
//                continue;
//            }
//            msDelay(1);
//        }//endwhile
}

char readSlave(char toRead){
        char msg=0;
        writeSPI_master(READ_DATA);
        writeSPI_master(toRead);
        msg = writeSPI1(READ);
        writeSPI_master(END_TRANSMISSION);
//        _Bool retry=1;
//        int retry_cnt=0;
//
//        char feedback;
//
//        while(retry && retry_cnt<20){
//            cmdLCD(0b00000010); //return home??
//            retry=0;
//            feedback = writeSPI1(READ_DATA);
//            putLCD(feedback);
//            if(feedback !=DATA_RDY && feedback != DATA_ERROR){
//                    retry=1;
//                    retry_cnt++;
//                    msDelay(100);
//                    continue;
//            }
//            msDelay(1);
//            feedback = writeSPI1(toRead);
//            putLCD(feedback);
//            if(feedback !=READ_DATA){
//                    retry=1;
//                    retry_cnt++;
//                    msDelay(100);
//                    continue;
//            }
//            msDelay(1);
//            msg = writeSPI1(READ);
//            msDelay(1);
//            feedback=writeSPI1(END_TRANSMISSION);
//            putLCD(feedback);
//            if(feedback!=0){
//                    retry=1;
//                    retry_cnt++;
//                    msDelay(200);
//                    continue;
//            }
//        }//endwhile
        return msg;
}