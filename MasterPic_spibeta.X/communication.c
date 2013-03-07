#include "spi.h"
#include "communication.h"
#include "globals.h"
#include "../Global_PIC/spiMessages.h"
#include "lcd.h"
int writeSPI_master(int msg){
     char feedback;
     _Bool retry=1;
     int retry_cnt=0;
     while(retry && retry_cnt<20){
        retry=0;
        feedback = writeSPI1(msg);
        if(feedback == DATA_PROCESSING){
            retry=1;
            retry_cnt++;
            msDelay(5);
            continue;
        }
     }
     return feedback;
}

void writeSlave(char slave, char mem, char value){
    writeSPI_master(WRITE_DATA);
    writeSPI_master(mem);
    writeSPI_master(value);
}

char readSlave(char slave, char toRead){
        char msg=0;
        writeSPI_master(READ_DATA);
        writeSPI_master(toRead);
        msg=writeSPI_master(READ);
        return msg;
}