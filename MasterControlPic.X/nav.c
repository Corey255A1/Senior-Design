#include <p24FJ128GB106.h>
#include "nav.h"
int gyroVal;
short resetFlag;
double accDegrees;
void __attribute__((__interrupt__, auto_psv)) _T5Interrupt(void){
    if(resetFlag){
        resetFlag =0;
        accDegrees=0;
    }else{
        accDegrees = accDegrees + ((double)gyroVal*ACC_SPEED);
    }
    _T5IF = 0; // Clear the IC1 interrupt status flag
}

void initGyroAcc( void ){
    //Setup the Output Compare Module timers
    TMR5=0; // Clear out the TMR3
    T5CON = 0;
    T5CONbits.TON = 0; // Turn off TMR3
    T5CONbits.TCKPS=0b01; //Set Prescaler to 1:8
    PR5 = 5000; //10mS per interrupt
    _T5IF = 0; // Clear the IC1 interrupt status flag
    _T5IE = 1; // Enable the IC1 interrupt status flag
    T5CONbits.TON = 1;
}
void setGyro ( int gyro ){
    gyroVal = gyro;
}
void resetGyroAccum(){
    resetFlag = 1;
}

//short compareCompass(unsigned int headingReal, unsigned int headingIntended){
//    long upperLim = (long)headingIntended+(long)COMPASS_ERROR;
//    long lowerLim = (long)headingIntended-(long)COMPASS_ERROR;
//    if (upperLim>TWO_PI){
//        upperLim-=TWO_PI;
//    }
//    if (lowerLim<0){
//        lowerLim+=TWO_PI;
//    }
//    if(upperLim>lowerLim){
//        if(headingReal>upperLim || headingReal<lowerLim){
//            return 0;
//        }//endif
//    }else{
//        if(headingReal>upperLim && headingReal<lowerLim){
//            return 0;
//        }//endif
//    }//endelse
//    return 1;
//}//endcompares