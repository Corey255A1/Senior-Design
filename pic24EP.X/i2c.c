#include <p24EP32MC202.h>

int i2c_state;

void __attribute__((__interrupt__, auto_psv)) _MI2C1Interrupt(void){
    IFS1bits.MI2C1IF = 0;
}
void i2c_Init( void ){
    	int temp;
	I2C1BRG = 0x27; //BRG=0x27 for Fosc=8Mhz,SCK 100kHz
	I2C1CONbits.I2CEN = 0; // Disable I2C Mode
	I2C1CONbits.DISSLW = 1; // Disable slew rate control
	IFS1bits.MI2C1IF = 0; // Clear Interrupt
	I2C1CONbits.I2CEN=1; // start module; configure SDA & SCK
	I2C1CONbits.A10M=0; //set 07-bit address mode
	temp = I2C1RCV;
}
void i2c_ResetBus(void)
{
	int x = 0;
	//initiate stop bit
	I2C1CONbits.PEN = 1;
	//wait for hardware clear of stop bit
	while (I2C1CONbits.PEN)
	{
		Nop();Nop();Nop();Nop();
		x ++;
		if (x > 20) break;
	}
	I2C1CONbits.RCEN = 0;
	IFS1bits.MI2C1IF = 0; // Clear Interrupt
	I2C1STATbits.IWCOL = 0;
	I2C1STATbits.BCL = 0;
	Nop();	Nop();	Nop();	Nop();	Nop();	Nop();	Nop();
}

void i2c_Start(char addr, char rw){
    I2C1CONbits.ACKDT = 0;
    I2C1CONbits.SEN = 1;
    Nop();	Nop();	Nop();	Nop();	Nop();	Nop();	Nop();
    int x;
    	while (I2C1CONbits.SEN)
	{
		Nop();Nop();
		x++;
		if (x > 20) break;
		Nop();Nop();Nop();
	}
    while (I2C1STATbits.TBF){}
    IFS1bits.MI2C1IF = 0;
    I2C1TRN = addr & rw;
    while (I2C1STATbits.TRSTAT){};

}

char i2c_Read(void){
    I2C1CONbits.RCEN =1;
    while(!I2C1STATbits.RBF){};
    return I2C2RCV;
}

void i2c_Write(char data){
    I2C1TRN = data;
    while (I2C1STATbits.TRSTAT){};
}

void i2c_Restart(void)
{
	int x = 0;
	I2C1CONbits.RSEN = 1; //Initiate restart condition
	Nop();
	//wait for automatic clear before proceding
	while (I2C1CONbits.RSEN)
	{
		Nop();
		x++;
		if (x > 20) break;
		Nop();
		Nop();
	}
}//end restart