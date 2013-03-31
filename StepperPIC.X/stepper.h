/* 
 * File:   stepper.h
 * Author: Corey Wunderlich
 *
 * Created on March 11, 2013, 4:25 PM
 */

#define DEGPERSTEP (.067)
#define RADPERSTEP (.00117)
#define STEPPERDEG (14.916)
#define STEPPERRAD (854.66)
#define MAXSTEPSPERSEC (1970)
#define MAXSECSPERDEG (.007)
#define MAXSECSPERRAD (2.5)
#define MAXSECSPERSTEP (.0006)
#define LARMINCH 8.5
#define SARMINCH 7
#define LARMCM 21.59
#define SARMCM 17.78
#define STWO 35.56
#define LTWO 43.18
#define LSQR 466.1281
#define SSQR 316.1284
#define LSQR_m_SSQR 149.9997
#define SSQR_m_LSQR -149.9997
#define COORDRES .5
#define MIDSTEP 40000
#define RADPERSECTION .03
#define MAXANGLE 500


extern int global_stepsInProgress;
void initStepper( void );
double getAngle( void );
void stepperSpeed(unsigned int);
void takeSteps(unsigned int,char);
void posXY(int,int);
void posRad(double);