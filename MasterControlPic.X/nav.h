/* 
 * File:   nav.h
 * Author: student
 *
 * Created on April 11, 2013, 12:49 AM
 */

//#define COMPASS_FIXED_POINT 8196
//#define TWO_PI 51496
//#define ONE_PI 25748
//#define COMPASS_ERROR 2000
#define GYRO_ZAXIS 3
#define ACC_SPEED 0.01
#define GYRO_ERROR 0.4
extern double accDegrees;

void initGyroAcc( void );
void setGyro ( int gyro );
void resetGyroAccum(void);

