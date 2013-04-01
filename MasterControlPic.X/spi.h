/* 
 * File:   spi.h
 * Author: Corey Wunderlich
 *
 * Created on February 28, 2013, 6:15 PM
 */
#include <p24FJ128GB106.h>

/**
 * These are the defines for SPI chip select lines
 */
#define MOTOR_DRIVER 1
#define MOTOR_DRIVERportEN _TRISD1=0
#define STEPPER_DRIVER1 2
#define STEPPER_DRIVER1portEN _TRISD2=0
#define STEPPER_DRIVER2 3
#define STEPPER_DRIVER2portEN _TRISD3=0
#define STEPPER_DRIVER3 4
#define STEPPER_DRIVER3portEN _TRISD4=0
#define SENSOR_BOARD 5
#define SENSOR_BOARDportEN _TRISD5=0
#define S_ULTRAS 6
#define S_ULTRASportEN _TRISD6=0
#define FB_ULTRAS 7
#define FB_ULTRASportEN _TRISD7=0

void initSPI(void);
int writeSPI1(int);

