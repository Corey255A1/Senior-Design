/**
 * -- Temperature Reader --
 * Initializes the AD channel so that it can be used with the temperature
 * sensors voltage output.
 * Reads in the value of the temperature sensor on the AD channel.
 *
 *
 */

#include "temperature.h"
#include "globals.h"
#include <p24EP32MC202.h>


/**
 * Sets up the AD channel for reading by the PIC. The temperature
 * sensor outputs a voltage that can be read by the read function.
 */
void initTemperature( void ){
    TEMPERATUREport = INPUT;
    TEMPCON1 = 0;
    TEMPCON2 = 0;
    TEMPCON3 = 0;
    TEMPCON4 = 0;

    TEMPCON1bits.ADON = 0; //Turn off AD
    TEMPCON1bits.ADSIDL = 0; //AD on in idle mode
    TEMPCON1bits.AD12B = 0; //10-bit mode
    TEMPCON1bits.FORM = 0; //integer output
    TEMPCON1bits.SSRC = 0b111; //auto-convert mode
    TEMPCON1bits.ASAM = 0;// sampling begins by setting SAMP bit
    TEMPCON3bits.SAMC = 31; // Auto Sample Time set to 31
    TEMPCHSbits.CH0SA = TEMPCHAN; //Set AD channel to Temperature Channel
    TEMPCON1bits.ADON = 1; //Turn on AD

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
    TEMPCON1bits.SAMP = 1; // Set the Sample Bit High to start read
    while(!TEMPCON1bits.DONE){}; // Wait for the sample and conversion
    TEMPCON1bits.DONE=0; // Reset the done bit
    int ad = TEMPBUF0;
    switch(d){
        case FAR: return (ad * VINx100) / BIT_10;
        case CEL: return ((((ad * VINx100) / BIT_10)- 32)*5)/9;
        case KEL: (return ((((ad * VINx100) / BIT_10)- 32)*5)/9)-271;
        default: return TEMPBUF0; // Store the result of the buffer to global
    }//endswitch
}//endread


/**
 * Does a read temperature converts the AD value to a temperature
 * and sets the global_temp value
 */
void setGlobalTemp( void ){
    //TO-DO Decide if we want a double precision
    //here or if just ints will work
    global_temp = (readTemperature(FAR) * VINx100) / BIT_10; // Remember Int div
}//endGlobalTemp

/**
 * Fahrenheit to Celsius Converter
 * @param fahrenheit
 * @return celsuis
 */
int f2c (int f){
    return ((f - 32)*5)/9;
}//end f2c
