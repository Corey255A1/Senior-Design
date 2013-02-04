/* 
 * File:   temperature.h
 * Author: student
 *
 * Created on February 4, 2013, 1:21 PM
 */
#include <p24EP32MC202.h>
#define TEMPERATUREport ANSELAbits.ANSA0
#define TEMPCON1 AD1CON1bits
#define TEMPCON2 AD1CON2bits
#define TEMPCON3 AD1CON3bits
#define TEMPCON4 AD1CON4bits
#define TEMPCSSL AD1CSSL


void initTemperature( void );
int readTemperature();

