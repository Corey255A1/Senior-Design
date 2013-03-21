/**
 * -- Global Header --
 * Any global value used should be declared with in this
 * file so that they can be found quickly and easily
 * Declarations are also found with in this header file
 * Making it easy to know where stuff is defined.
 */
#include <p24EP32MC202.h>
#define RISE 1
#define FALL 0
#define INPUT 1
#define OUTPUT 0
#define OC1port 0b010000
#define HIGH 1
#define LOW 0
#define VINx100 330
#define BIT_10 1023
#define LOCKED 1
#define UNLOCKED 0
#define DISABLE 0
#define CLEAR 0
#define EN 1
#define DIS 0
#define pi 3.141592653589

//-----------------------------------------------------------------------------
//  Variable Defines for Slave Drive Control
//-----------------------------------------------------------------------------
#define DIGITAL             0
#define DRIVER_PERIOD_US    10

//-----------------------------------------------------------------------------
//  Defines for drive system.
//-----------------------------------------------------------------------------
#define CLK_PERIOD          0.135
#define DRIVE_EN            _RB11
#define SPEEDM1             OC1R
#define SPEEDM2             OC2R
#define M1FWD               LATAbits.LATA1
#define M1REV               LATAbits.LATA0
#define M2FWD               LATAbits.LATA4
#define M2REV               LATBbits.LATB4
#define MMSG                0
#define DRIVESTATUS         0
#define INMSG               SLAVEData.inData[MMSG]
#define OUTMSG              SLAVEData.outData[DRIVESTATUS]

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

extern short global_u1_edge;
extern long global_u1_time;

extern short global_u2_edge;
extern long global_u2_time;

extern int global_temp;
