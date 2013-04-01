#include <p24FJ128GA010.h>
#include "spi.h"
#include "communication.h"
#include "globals.h"
#include "../Global_PIC/spiMessages.h"
#include "lcd.h"
int writeSPI_master(int slave,int msg){
    PORTG = PORTG & ~(1<<slave);
     int feedback;
//     _Bool retry=1;
//     int retry_cnt=0;
//     while(retry && retry_cnt<20){
//        retry=0;
      feedback = writeSPI1(msg);
//        if(feedback == DATA_ERROR){
//            retry=1;
//            retry_cnt++;
//            msDelay(5);
//            continue;
//        }
//     }
    PORTG = PORTG | (1<<slave);
     return feedback;
}

void writeSlave(int slave, int mem, int value){
    writeSPI_master(slave,WRITE_DATA);
    msDelay(20);
    writeSPI_master(slave,mem);
    msDelay(20);
    writeSPI_master(slave,value);
}

int readSlave(int slave, int toRead){
        int msg=0;
        writeSPI_master(slave,READ_DATA);
         msDelay(20);
        writeSPI_master(slave,toRead);
         msDelay(20);
        msg=writeSPI_master(slave,READ);
        return msg;
}