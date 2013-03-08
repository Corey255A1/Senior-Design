#include <p24EP32MC202.h>

//-----------------------------------------------------------------------------
//  Function Defines
//-----------------------------------------------------------------------------
void configOutputCompare(void);
void configDevicePins(void);
void configInputCaptures();
void __attribute__((__interrupt__, auto_psv)) _IC1Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC2Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC3Interrupt(void);
void __attribute__((__interrupt__, auto_psv)) _IC4Interrupt(void);

//-----------------------------------------------------------------------------
//  Variable Defines
//-----------------------------------------------------------------------------
#define DRIVER_PERIOD_US    10
#define CLK_PERIOD          0.135
#define DRIVE_EN            _RB11
#define OUTPUT              0
#define INPUT               1
#define DIGITAL             0
#define EN                  1
#define DISABLE             0
#define CLEAR               0
#define SPEEDM1             OC1R
#define SPEEDM2             OC2R
#define M1FWD               _RA0
#define M1REV               _RA1
#define M2FWD               _RB0
#define M2REV               _RB1

#define M1FDBCKA_RPIPORT    47
#define M1FDBCKA_RBPORT     _RB15
#define M1FDBCKA_TRISREG    TRISBbits.TRISB15

#define M1FDBCKB_RPIPORT    46
#define M1FDBCKB_RBPORT     _RB14
#define M1FDBCKB_TRISREG    TRISBbits.TRISB14

#define M2FDBCKA_RPIPORT    45
#define M2FDBCKA_RBPORT     _RB13
#define M2FDBCKA_TRISREG    TRISBbits.TRISB13

#define M2FDBCKB_RPIPORT    44
#define M2FDBCKB_RBPORT     _RB12
#define M2FDBCKB_TRISREG    TRISBbits.TRISB12

//-----------------------------------------------------------------------------
//  Global Declarations.
//-----------------------------------------------------------------------------
int OC1clkT         = 100;      // Set a period
int spiReadVal      = 0;        // Message we read from Master
char msgQueued      = CLEAR;    // Let's us know if a message needs handled.
char M1FdBckASet    = 0;        // Whether or not M1 feedback A has arrived
char M1FdBckBSet    = 0;        // Whether or not M1 feedback B has arrived
char M2FdBckASet    = 0;        // Whether or not M2 feedback A has arrived
char M2FdBckBSet    = 0;        // Whether or not M2 feedback B has arrived