/* 
 * File:   spi.h
 * Author: Corey Wunderlich
 *
 * Created on February 28, 2013, 6:15 PM
 */
#include <p24FJ128GA010.h>
#define SLAVE1 12
#define SLAVE1portEN _TRISG12=0
#define SLAVE2 13
#define SLAVE2portEN _TRISG13=0
#define MOTOR_DRIVER 14
#define MOTOR_DRIVERportEN _TRISG14=0
#define SERVO_DRIVER 15
#define SERVO_DRIVERportEN _TRISG15=0
void initSPI(void);
int writeSPI1(int);

