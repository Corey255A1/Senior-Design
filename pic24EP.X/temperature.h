/**
 * -- Temperature Header --
 * 
 * This header masks the AD channels so that if they change
 * the code can be easily changed by just changing these definitions
 *
 * Created on February 4, 2013, 1:21 PM
 */
#include <p24EP32MC202.h>

#define FAR 'F'
#define CEL 'C'
#define KEL 'K'

#define TEMPERATUREport ANSELAbits.ANSA0
#define TEMPCON1 AD1CON1
#define TEMPCON2 AD1CON2
#define TEMPCON3 AD1CON3
#define TEMPCON4 AD1CON4

#define TEMPCON1bits AD1CON1bits
#define TEMPCON2bits AD1CON2bits
#define TEMPCON3bits AD1CON3bits
#define TEMPCON4bits AD1CON4bits
#define TEMPCSSL AD1CSSL
#define TEMPCHS AD1CHS0
#define TEMPCHSbits AD1CHS0bits
#define TEMPBUF0 ADC1BUF0
#define TEMPCHAN 0 //AD0


void initTemperature( void );
void setGlobalTemp( void );
int readTemperature( char );
int f2c( int )

