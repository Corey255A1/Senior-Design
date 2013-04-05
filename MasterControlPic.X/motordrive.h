/* 
 * File:   motordrive.h
 * Author: dt12
 *
 * Created on April 5, 2013, 7:23 AM
 */
#include <p24FJ128GB106.h>
#define SPEEDM1     OC1R
#define SPEEDM2     OC2R
#define SPEEDMULT   25
#define MSTOP_PULSE 750
#define CLK_PER 1500

extern long IC1_COUNT;
extern long IC2_COUNT;
#define IC1_COUNTS_BYTE1 ((IC1_COUNT&0xFF000000)>>24)
#define IC1_COUNTS_BYTE2 ((IC1_COUNT&0xFF000000)>>16)
#define IC1_COUNTS_BYTE3 ((IC1_COUNT&0xFF000000)>>8)
#define IC1_COUNTS_BYTE4 ((IC1_COUNT&0xFF000000))

#define IC2_COUNTS_BYTE1 ((IC2_COUNT&0xFF000000)>>24)
#define IC2_COUNTS_BYTE2 ((IC2_COUNT&0xFF000000)>>16)
#define IC2_COUNTS_BYTE3 ((IC2_COUNT&0xFF000000)>>8)
#define IC2_COUNTS_BYTE4 ((IC2_COUNT&0xFF000000))

void configOutputCompare(void);
void setMotor(int);