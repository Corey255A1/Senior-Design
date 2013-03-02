#include "spi.h"
#include "communication.h"
#include "globals.h"
#include "../Global_PIC/spiMessages.h"
#include "lcd.h"
void writeSlave(char msg){
        _Bool retry=1;
        int retry_cnt=0;
        while(retry && retry_cnt<20){
            retry=0;
            if(writeSPI1(WRITE_DATA)!=DATA_RDY){
                retry=1;
                retry_cnt++;
                msDelay(200);
                continue;
            }
            msDelay(1);
            writeSPI1(msg);
            msDelay(1);
            if(writeSPI1(END_TRANSMISSION)!=msg){
                retry=1;
                retry_cnt++;
                msDelay(200);
                continue;
            }
            msDelay(1);
        }//endwhile
}

char readSlave(char toRead){
        _Bool retry=1;
        int retry_cnt=0;
        char msg=0;
        while(retry && retry_cnt<20){
            retry=0;
            if(writeSPI1(READ_DATA)!=DATA_RDY){
                    retry=1;
                    retry_cnt++;
                    msDelay(100);
                    continue;
            }
            msDelay(1);
            writeSPI1(toRead);
            msDelay(1);
            msg = writeSPI1(READ);
            msDelay(1);
            if(writeSPI1(END_TRANSMISSION)!=0){
                    retry=1;
                    retry_cnt++;
                    msDelay(200);
                    continue;
            }
        }//endwhile
        return msg;
}