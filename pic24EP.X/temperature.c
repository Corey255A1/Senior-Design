/**
 * Temperature Reader
 *
 */

#include "temperature.h"
#include <p24EP32MC202.h>
void initTemperature( void ){
    TEMPERATUREport = 1;
    //TO-DO Ooopss this is a mistake
    TEMPCON1 = 0;
    TEMPCON2 = 0;
    TEMPCON3 = 0;
    TEMPCON4 = 0;
    //END MISTAKES

    
    TEMPCON1.ADON = 0; //Turn off AD
    TEMPCON1.ADSIDL = 0; //AD on in idle mode
    TEMPCON1.AD12B = 0; //10-bit mode
    TEMPCON1.FORM = 0; //integer output
    TEMPCON1.SSRC = 0b111; //auto-convert mode
    TEMPCON1.ASAM = 0;// sampling begins by setting SAMP bit
    TEMPCON3.SAMC = 31; // Auto Sample Time set to 31
    TEMPCON3.ADCS = 5; //Conversion Clock select set to 5
    //TO-DO FINISH THE INITIALIZATIONS FOR THIS - CW




}//endinit
int readTemperature(){

}//endread
