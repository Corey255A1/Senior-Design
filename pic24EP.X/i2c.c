#include <p24EP32MC202.h>
void initI2C( void ){

    	int temp;
	I2C1BRG = 0x27; //BRG=0x27 for Fosc=8Mhz,SCK 100kHz
	I2C1CONbits.I2CEN = 0; // Disable I2C Mode
	I2C1CONbits.DISSLW = 1; // Disable slew rate control
	IFS1bits.MI2C1IF = 0; // Clear Interrupt
	I2C1CONbits.I2CEN=1; // start module; configure SDA & SCK
	I2C1CONbits.A10M=0; //set 07-bit address mode
	temp = I2C1RCV;
}
