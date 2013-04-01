#include <p24EP32MC202.h>
#include "globals.h"


void initADC( void ){
    AD1CON1 = 0;
    AD1CON1 = 0;
    AD1CON1 = 0;
    AD1CON1 = 0;
    AD1CON1bits.ADON = 0; //Turn off AD
    AD1CON1bits.ADSIDL = 0; //AD on in idle mode
    AD1CON1bits.AD12B = 0; //10-bit mode
    AD1CON1bits.FORM = 0; //integer output
    AD1CON1bits.SSRC = 0b111; //auto-convert mode
    AD1CON1bits.ASAM = 0;// sampling begins by setting SAMP bit
    AD1CON3bits.SAMC = 31; // Auto Sample Time set to 31
    AD1CON1bits.SIMSAM = 1;// Sample all channels at once
    AD1CON2bits.CHPS = 0b11; //set to sample all channels
    AD1CHS0bits.CH0SA = 3;//Set CH0 to AN3
    AD1CHS123bits.CH123SA=0;//CH1 to AN0
                            //CH2 to AN1
                            //CH3 to AN2
    AD1CON1bits.ADON = 1; //Turn on AD
}

void readADC( int ADRead[4] ){
    AD1CON1bits.SAMP = 1; // Set the Sample Bit High to start read
    while(!AD1CON1bits.DONE){}; // Wait for the sample and conversion
    AD1CON1bits.DONE=0; // Reset the done bit
    ADRead[0] = ADC1BUF0;
    ADRead[1] = ADC1BUF1;
    ADRead[2] = ADC1BUF2;
    ADRead[3] = ADC1BUF3;
}
int readADC_single(char ch){
    int ad[4];
    readADC(ad);
    return ad[ch];
}
