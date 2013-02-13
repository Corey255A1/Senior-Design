/* 
 * File:   i2c.h
 * Author: student
 *
 * Created on February 6, 2013, 4:26 PM
 */

#include <p24EP32MC202.h>
void i2c_Init( void );
void i2c_Start( char, char );
void i2c_ResetBus(void);
char i2c_Read(void);
void i2c_Write(char);
void i2c_Restart(void);

void __attribute__((__interrupt__, auto_psv)) _MI2C1Interrupt(void);


