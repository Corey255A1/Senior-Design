#include <p24EP32MC202.h>
#include "i2c.h"

int i2c_state;


void i2c_Init( void ){
    	int temp;
        I2C2CONbits.I2CEN = 0; // Disable I2C Mode
	I2C2BRG = 30; //BRG=0x27 for Fosc=8Mhz,SCK 100kHz
	I2C2CONbits.DISSLW = 1; // Disable slew rate control
	IFS3bits.MI2C2IF = 0; // Clear Interrupt
        I2C2CONbits.I2CEN=1; // start module; configure SDA & SCK
        I2C2CONbits.A10M=0; //set 07-bit address mode
	temp = I2C2RCV;
                int i;
	for(i=0;i<5000;i++)
                 Nop();
}
void i2c_ResetBus(void)
{
	//initiate stop bit
	I2C2CONbits.PEN = 1;
	//wait for hardware clear of stop bit
	while (I2C2CONbits.PEN);
	I2C2CONbits.RCEN = 0;
	IFS3bits.MI2C2IF = 0; // Clear Interrupt
	I2C2STATbits.IWCOL = 0;
	I2C2STATbits.BCL = 0;
        int i;
	for(i=0;i<5000;i++)
                 Nop();
}
void i2c_Write(char data){
    while (I2C2STATbits.TBF){};
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = data;
    while (I2C2STATbits.TRSTAT){};
    Nop();Nop();
}

void i2c_Start(char addr, char rw){
    I2C2CONbits.ACKDT = 0;
    int i=0;
    for(i=0;i<1000;i++)
        Nop();
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN){};
    i2c_Write(addr | rw);

}

unsigned char i2c_Read(void){
    I2C2CONbits.RCEN=1;
    while(!I2C2STATbits.RBF){};
    I2C2CONbits.ACKEN=1;
    return (unsigned) I2C2RCV;
}
unsigned char i2c_ReadFinal(void){
    I2C2CONbits.RCEN=1;
    while(!I2C2STATbits.RBF){};
    return (unsigned) I2C2RCV;
}



void i2c_Restart(char addr, char rw)
{
	I2C2CONbits.RSEN = 1; //Initiate restart condition
	Nop();
	//wait for automatic clear before proceding
	while (I2C2CONbits.RSEN){};
        while (I2C2STATbits.TBF){};
        IFS3bits.MI2C2IF = 0;
        I2C2TRN = addr | rw;
        while (I2C2STATbits.TRSTAT){};
}//end restart

void i2c_Ack( void ){
  I2C2CONbits.ACKEN=1;
}