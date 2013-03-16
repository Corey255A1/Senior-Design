/* 
 * File:   stepper.h
 * Author: Corey Wunderlich
 *
 * Created on March 11, 2013, 4:25 PM
 */

extern int global_stepsInProgress;
void initStepper( void );
void stepperSpeed(unsigned int);
void takeSteps(unsigned int,char);