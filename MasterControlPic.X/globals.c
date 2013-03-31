#include "globals.h"
#include <p24FJ128GB106.h>


void msDelay(unsigned int ms){
		unsigned long i;
		for(i=(unsigned long)(ms+1)*SCALE;i>0;i-=1) Nop();
}

void _10usDelay(unsigned char _10us){
    int i;
    for(i=_10us*uSCALE; i>1;i--); Nop();
}