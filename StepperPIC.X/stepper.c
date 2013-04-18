/**
 * --Stepper Control--
 * Handles the stepper motor initialization
 * and also provides functions to move the stepper
 * a specified amount
 * Also can set the direction and speed of the stepper
 *
 * --Corey Wunderlich
 *
 */

#include "stepper.h"
#include <p24EP32MC202.h>
#include "globals.h"
#include "A2D.h"
#include <math.h>

int local_stepCount=0;
int global_stepsInProgress=0;
unsigned int local_stepper_speed=50000;
double local_X=5, local_Y=5, local_R=0;

/**
 * Enables all of the stepper motor phases
 * PWM1 and PWM2 both have a high and low
 * Set them up in complementary modes
 * Set them up 90degrees out of phase
 */
void initStepper( void ){
    _TRISB15 = 0;//make PWM servo port an output.
    _TRISB14 = 0;//make PWM servo port an output.
    _TRISB13 = 0;//make PWM servo port an output.
    _TRISB12 = 0;//make PWM servo port an output.
    _TRISB11=0;//Gonna use this as the enable for now
    _RB15=0;
    _RB14=0;
    _RB13=0;
    _RB12=0;
    _RB11=0;

    PTCONbits.PTEN = 0;//Turn of the PWM Module
    PTCONbits.SYNCPOL = 0;//SYNCO output is active high?
    PTCONbits.SYNCOEN = 1;//Enable SYNCO output - Sync with master time
    PTCON2 = 0; //set to 1:1 prescaler
    IOCON2=0;//clear
    IOCON1=0;//clear
    IOCON2bits.PENL = 1; //turn on PWM2L
    IOCON2bits.PENH = 1; //turn on PWM2H
    IOCON1bits.PENL = 1; //turn on PWM1L
    IOCON1bits.PENH = 1; //turn on PWM1H
    IOCON2bits.POLL = 0; //make PWM2L active high
    IOCON2bits.POLH = 0; //make PWM2H active high
    IOCON1bits.POLL = 0; //make PWM1L active high
    IOCON1bits.POLH = 0; //make PWM1H active high
    IOCON2bits.PMOD=0;//Complementary
    IOCON1bits.PMOD=0;//Complementary
    PWMCON2 =0; //clear PWMCON2
    PWMCON2bits.CAM=0; //EDGE aligned mode
    PWMCON1 =0; //clear PWMCON2
    PWMCON1bits.CAM=0; //EDGE aligned mode
    PTPER = local_stepper_speed;//master PWM period
    PHASE1 = 0;//PHASE1
    PHASE2 = local_stepper_speed/4;//PHASE2 is set to a quater of the speed 90deg
    PDC2=PDC1 = local_stepper_speed/2;
    DTR1=DTR2=0;//no dead zone
    PWMCON2bits.TRGIEN=1;//Turn on Interrupt for PWM2
    IFS5bits.PWM2IF=0;//Clear PWM2 flag
    IEC5bits.PWM2IE=1;//Enable the Interrupts
}
/**
 * The interrupt that catches and clears the PWM enable flag
 * It also decrements the stepCount and clears the PWM
 */
void __attribute__((__interrupt__, auto_psv)) _PWM2Interrupt(void){
    global_stepsInProgress=local_stepCount;
    if(local_stepCount<=0){
        PTCONbits.PTEN = 0;//disable the PWM
        _RB11=0;
    }
    local_stepCount-=4;
    IFS5bits.PWM2IF=0;
}

/**
 * Main code setable of the speed
 * Sets the local speed and the PERIOD
 * and Duty cycles
 * @param speed -- the amount of timer counts
 */
void stepperSpeed(unsigned int speed){
    local_stepper_speed=speed;
       PTPER = speed;//master PWM period
       PDC2=PDC1 = speed/2;
}

/**
 * Sets up the PHASE1 and PHASE2 for corresponding
 * to the direction that is to be set
 * @param steps -- the amount of steps to go
 * @param dir -- the Direction to step
 */
void takeSteps(unsigned int steps, char dir){
    if(dir==REV){
      PHASE1 = 0;
      PHASE2 = local_stepper_speed/4;
    }else if(dir==FWD){
      PHASE1 = local_stepper_speed/4;
      PHASE2 =  0;
    }
   local_stepCount=steps;
   _RB11=1;
   PTCONbits.PTEN = 1;//enable the PWM
}
void posXY(int x, int y){
    double R;
    double L;
#ifdef LONGARM
    double O1;
    double r2;
#endif
#ifdef SHORTARM
    double O2;
    double r1;
#endif
    double delRad;
    _Bool xDone = 0;
    _Bool yDone = 0;
    unsigned int steps;
    unsigned int speed;
    double prespeed;
    char dir;
    int totalSteps=0;
    while(!xDone || !yDone){

        if(local_X>(x+COORDRES)){
            local_X = local_X - COORDRES;
        }
        else if (local_X<(x-COORDRES)){
            local_X = local_X +COORDRES;
        }
        else{
            xDone = 1;
        }

        if(local_Y>(y+COORDRES)){
            local_Y = local_Y - COORDRES;
        }
        else if (local_Y<(y-COORDRES)){
            local_Y = local_Y +COORDRES;
        }
        else{
            yDone = 1;
        }
        R = atan(local_Y/local_X);
        L = local_X/cos(R);
#ifdef SHORTARM
        O2 = acos((SSQR_m_LSQR+(L*L))/(STWO*L));
        r1 = R - O2;
        delRad = local_R - r1;
        if(delRad < 0){
            dir=REV;
            delRad=-delRad;
        }else{
            dir=FWD;
        }
        local_R = r1;
#endif
#ifdef LONGARM
        O1 = acos((LSQR_m_SSQR+L*L)/(LTWO*L));
        r2 = O1 + R;
        delRad = local_R - r2;
        if(delRad < 0){
            dir=REV;
            delRad=-delRad;
        }else{
            dir=FWD;
        }
        local_R=r2;
#endif
        steps = floor(delRad*STEPPERRAD);
        prespeed = (double)RADPERSECTION/delRad;
        speed = floor(MIDSTEP*prespeed);
        totalSteps += steps;
        stepperSpeed(speed);
        takeSteps(steps,dir);
        while(PTCONbits.PTEN);
    }
}//end posXY
void posRad(double rad){
#ifdef LONGARM
    if(rad>LMAXRAD){
        rad = LMAXRAD;
    }
    if(rad<LMINRAD){
        rad = LMINRAD;
    }
#endif
#ifdef SHORTARM
    if(rad>LMAXRAD){
        rad = LMAXRAD;
    }
#endif
    if(rad>local_R+(RAD_ERROR) || rad<local_R-(RAD_ERROR)){
        double temp_r;
        if(local_R<0){
            temp_r = local_R+(2*pi);
        }else{
            temp_r = local_R;
        }
        double delRad = temp_r - rad;
        char dir;
        if(delRad<0){
#ifdef LONGARM
            dir = REV;
#endif
#ifdef SHORTARM
            dir = FWD;
#endif
            delRad = -delRad;
        }else{
#ifdef LONGARM
            dir = FWD;
#endif
#ifdef SHORTARM
            dir = REV;
#endif
        }
        int steps = floor(delRad*STEPPERRAD);
        stepperSpeed(30000);
        takeSteps(steps,dir);
        while(PTCONbits.PTEN);
        local_R = rad;
    }
}

double getAngle ( void ){
    int raw = readADC_single(AN0);
    double AvgAngle = (double) raw * ((double)((3*pi)/2)/ (double)MAXANGLE);
    int i;
    for(i=0;i<20;i++){
        raw = readADC_single(AN0);
        AvgAngle += (double) raw * ((double)((3*pi)/2)/ (double)MAXANGLE);
    }
    AvgAngle = AvgAngle/20;
    local_R = AvgAngle;
    return AvgAngle;
}