/**
 * -- Temperature Header --
 * 
 * This header masks the AD channels so that if they change
 * the code can be easily changed by just changing these definitions
 *
 * Created on February 4, 2013, 1:21 PM
 */
#include <p24EP32MC202.h>
#include "globals.h"

#define FAR 'F'
#define CEL 'C'
#define KEL 'K'

#define TEMPERATUREport ANSELAbits.ANSA0
#define TEMP_ADBUF 0


void initTemperature( void );
int readTemperature( char );
int f2c( int );
