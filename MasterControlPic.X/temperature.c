/**
 * -- Temperature Reader --
 * Initializes the AD channel so that it can be used with the temperature
 * sensors voltage output.
 * Reads in the value of the temperature sensor on the AD channel.
 *
 *--Corey Wunderlich
 */

#include "temperature.h"
#include <p24FJ128GB106.h>
#include "globals.h"
#include "A2D.h"

int global_temp = 0;

/**
 * Sets up the AD channel for reading by the PIC. The temperature
 * sensor outputs a voltage that can be read by the read function.
 * be sure to explicitly initialize the AD channel
 */
void initTemperature( void ){
    TEMPERATUREport = 0;//set the AD channel to Analog just incase
}//endinit



/**
 * Reads in the temperature by setting Sample bit and waiting for the done bit
 * It is setup this way because we only need to call it once per run
 * since Temperature can be thought of as being relatively constant
 * Read the value into global_temp so that it can be used by the ultrasonic
 * sensors in the distance calculations
 * @param a character that can choose the return value
 * F - fahrenheit
 * C - Celcius
 * K - Kelvin - for fun
 * R - raw AD value
 * @return the temperature AD value
 */
int readTemperature( char d ){
    int ad = readADC(TEMP_ADBUF);
    long t;
    switch(d){
        case FAR:
            t = ((long) ad * VINx100) / BIT_10;
            return (int) t;
        case CEL:
            t =(((((long) ad * VINx100) / BIT_10)- 32)*5)/9;
            return (int) t;
        case KEL: 
            t = ((((((long) ad * VINx100) / BIT_10)- 32)*5)/9)+271;
            return t;
        default: return ad; // Store the result of the buffer to global
    }//endswitch
}//endread



/**
 * Fahrenheit to Celsius Converter
 * @param fahrenheit
 * @return celsuis
 */
int f2c (int f){
    return ((f - 32)*5)/9;
}//end f2c
