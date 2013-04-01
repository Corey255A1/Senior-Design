#include <p24EP32MC202.h>
#include "globals.h"

//-----------------------------------------------------------------------------
//  Function Defines
//-----------------------------------------------------------------------------
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void);

//-----------------------------------------------------------------------------
//  Global Declarations.
//-----------------------------------------------------------------------------
int curMasterConfig = 0;        // Message we read from Master
int curFdBckState   = 0;        // State from which we are reading from the
                                // motor Hall feedback sensors.
char msgQueued      = CLEAR;    // Let's us know if a message needs handled.
int OC1clkT         = 100;      // Set a period
int curDriveState   = 0;        // Speed and direction of both motors. Bits are
                                // as follows:
                                // M2speed,M2Dir,M1speed,M1Dir. All 4-bits wide

char M1FdBckASet    = 0;        // Whether or not M1 feedback A has arrived
char M1FdBckBSet    = 0;        // Whether or not M1 feedback B has arrived
char M2FdBckASet    = 0;        // Whether or not M2 feedback A has arrived
char M2FdBckBSet    = 0;        // Whether or not M2 feedback B has arrived

short M1FdBckAEdge  = RISE;
short M1FdBckBEdge  = RISE;
short M2FdBckAEdge  = RISE;
short M2FdBckBEdge  = RISE;

long M1FdBckA_t     = 0;
long M1FdBckB_t     = 0;
long M2FdBckA_t     = 0;
long M2FdBckB_t     = 0;

long M1FdBckA_SampCount   = 0;
long M1FdBckB_SampCount   = 0;
long M2FdBckA_SampCount   = 0;
long M2FdBckB_SampCount   = 0;

long M1FdBckA_SampReq   = 0;
long M1FdBckB_SampReq   = 0;
long M2FdBckA_SampReq   = 0;
long M2FdBckB_SampReq   = 0;

long M1FdBckAStart_t = 0;
long M1FdBckAEnd_t   = 0;

long M1FdBckBStart_t = 0;
long M1FdBckBEnd_t   = 0;

long M2FdBckAStart_t = 0;
long M2FdBckAEnd_t   = 0;

long M2FdBckBStart_t = 0;
long M2FdBckBEnd_t   = 0;

//-----------------------------------------------------------------------------
//  I ran some tests by monitoring the pulse width over 100 samples and taking
//  an average. The averages for different speeds are as follows:
//
//      ____________________________
//      |Speed 0     |   0          |
//      |Speed 1     |   46100      |
//      |Speed 2     |   16914      |
//      |Speed 3     |   9783       |
//      |Speed 4     |   6653       |
//      |Speed 5     |   4993       |
//      |Speed 6     |   3988       |
//      |Speed 7     |   3333       |
//      |Speed 8     |   2863       |
//      |Speed 9     |   2521       |
//      |Speed 10    |   2188       |
//      |___________________________|
//
//  From these averages a threshold detection scheme was used to determine the
//  speed of the motor. The thresholds are the exact middle of the speed
//  averages.
//-----------------------------------------------------------------------------
long MSpeed1_2_Thresh   = 31507;
long MSpeed2_3_Thresh   = 13349;
long MSpeed3_4_Thresh   = 8218;
long MSpeed4_5_Thresh   = 5823;
long MSpeed5_6_Thresh   = 4491;
long MSpeed6_7_Thresh   = 3661;
long MSpeed7_8_Thresh   = 3098;
long MSpeed8_9_Thresh   = 2692;
long MSpeed9_10_Thresh  = 2355;

