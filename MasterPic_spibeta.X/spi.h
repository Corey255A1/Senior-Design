/* 
 * File:   spi.h
 * Author: Corey Wunderlich
 *
 * Created on February 28, 2013, 6:15 PM
 */
#include <p24FJ128GA010.h>
#define SLAVE1 12
#define SLAVE1portEN _TRISD12=0
#define SLAVE2 10
#define SLAVE2portEN _TRISD10=0
void initSPI(void);
int writeSPI1(int);

