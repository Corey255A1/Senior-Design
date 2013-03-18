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
int curConfig       = 0;        // Message we read from Master
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

int M1FdBckAStart_t = 0;
int M1FdBckAEnd_t   = 0;

int M1FdBckBStart_t = 0;
int M1FdBckBEnd_t   = 0;

int M2FdBckAStart_t = 0;
int M2FdBckAEnd_t   = 0;

int M2FdBckBStart_t = 0;
int M2FdBckBEnd_t   = 0;

