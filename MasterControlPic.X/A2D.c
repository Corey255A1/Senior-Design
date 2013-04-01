

#include <p24FJ128GB106.h>
#include "globals.h"


void initADC( void ){
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CON1bits.ADON = 0; //Turn off AD
    AD1CON1bits.ADSIDL = 0; //AD on in idle mode
    AD1CON1bits.FORM = 0; //integer output
    AD1CON1bits.SSRC = 0b111; //auto-convert mode
    AD1CON1bits.ASAM = 0;// sampling begins by setting SAMP bit
    AD1CON3bits.SAMC = 31; // Auto Sample Time set to 31
    AD1CHS0bits.CH0SA = 0;//Set CH0 to AN0
    AD1CON1bits.ADON = 1; //Turn on AD
}

int readADC(char ch){
    AD1CHS0bits.CH0SA = ch;//Set CH0 to AN0
    AD1CON1bits.SAMP = 1; // Set the Sample Bit High to start read
    while(!AD1CON1bits.DONE){}; // Wait for the sample and conversion
    AD1CON1bits.DONE=0; // Reset the done bit
    return ADC1BUF0;
}
