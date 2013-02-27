#include <p24EP32MC202.h>
#include "i2c.h"

int i2c_state;


void i2c_Init( void ){
    	int temp;
        I2C1CONbits.I2CEN = 0; // Disable I2C Mode
	I2C1BRG = 30; //BRG=0x27 for Fosc=8Mhz,SCK 100kHz
	I2C1CONbits.DISSLW = 1; // Disable slew rate control
	IFS1bits.MI2C1IF = 0; // Clear Interrupt
        I2C1CONbits.I2CEN=1; // start module; configure SDA & SCK
        I2C1CONbits.A10M=0; //set 07-bit address mode
	temp = I2C1RCV;
                int i;
	for(i=0;i<5000;i++)
                 Nop();
}
void i2c_ResetBus(void)
{
	//initiate stop bit
	I2C1CONbits.PEN = 1;
	//wait for hardware clear of stop bit
	while (I2C1CONbits.PEN);
	I2C1CONbits.RCEN = 0;
	IFS1bits.MI2C1IF = 0; // Clear Interrupt
	I2C1STATbits.IWCOL = 0;
	I2C1STATbits.BCL = 0;
        int i;
	for(i=0;i<5000;i++)
                 Nop();
}
void i2c_Write(char data){
    while (I2C1STATbits.TBF){};
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = data;
    while (I2C1STATbits.TRSTAT){};
    Nop();Nop();
}

void i2c_Start(char addr, char rw){
    I2C1CONbits.ACKDT = 0;
    int i=0;
    for(i=0;i<1000;i++)
        Nop();
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN){};
    i2c_Write(addr | rw);

}

unsigned char i2c_Read(void){
    I2C1CONbits.RCEN=1;
    while(!I2C1STATbits.RBF){};
    I2C1CONbits.ACKEN=1;
    return (unsigned) I2C1RCV;
}
unsigned char i2c_ReadFinal(void){
    I2C1CONbits.RCEN=1;
    while(!I2C1STATbits.RBF){};
    return (unsigned) I2C1RCV;
}



void i2c_Restart(char addr, char rw)
{
	I2C1CONbits.RSEN = 1; //Initiate restart condition
	Nop();
	//wait for automatic clear before proceding
	while (I2C1CONbits.RSEN){};
        while (I2C1STATbits.TBF){};
        IFS1bits.MI2C1IF = 0;
        I2C1TRN = addr | rw;
        while (I2C1STATbits.TRSTAT){};
}//end restart

void i2c_Ack( void ){
  I2C1CONbits.ACKEN=1;
}