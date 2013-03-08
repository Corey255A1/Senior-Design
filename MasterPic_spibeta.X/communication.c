#include "spi.h"
#include "communication.h"
#include "globals.h"
#include "../Global_PIC/spiMessages.h"
#include "lcd.h"
int writeSPI_master(int slave,int msg){
    PORTD = PORTD & (0<<slave);
     char feedback;
     _Bool retry=1;
     int retry_cnt=0;
     while(retry && retry_cnt<20){
        retry=0;
        feedback = writeSPI1(msg);
        if(feedback == DATA_ERROR){
            retry=1;
            retry_cnt++;
            msDelay(5);
            continue;
        }
     }
     PORTD = PORTD | (1<<slave);
     return feedback;
}

void writeSlave(int slave, int mem, int value){
    writeSPI_master(slave,WRITE_DATA);
    writeSPI_master(slave,mem);
    writeSPI_master(slave,value);
}

int readSlave(int slave, int toRead){
        char msg=0;
        writeSPI_master(slave,READ_DATA);
        writeSPI_master(slave,toRead);
        msg=writeSPI_master(slave,READ);
        return msg;
}