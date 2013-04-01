#include <p24FJ128GA010.h>
#include "lcd.h"
#include "string.h"
#include "globals.h"
char lastchar;
char msg[15]=" ";
int ch=0;
int newMSG = 0;
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(){
    lastchar = U1RXREG;
    IFS0bits.U1RXIF = 0;
}
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(){
    lastchar = U2RXREG;
    if(!newMSG){
        msg[ch]=lastchar;
        if(lastchar!='!'){
            ch++;
        }else{
            ch=0;
            newMSG = 1;
        };
    }//endif
    IFS1bits.U2RXIF = 0;
}
void initSerial1 ( void ){
    U1MODEbits.UARTEN = 0;//disable;
    U1MODE = 0;//clear that bitch
    U1STA = 0;// clea this bitch
    U1BRG = 25; // set baud to 38461
    U1MODEbits.BRGH = 1; //this needs to be a 1 for High mode
    U1STAbits.UTXEN = 1; // enable transmit
    IFS0bits.U1RXIF = 0;//clear interupt flag
    IEC0bits.U1RXIE = 1;//enable interrupt
    U1MODEbits.UARTEN = 1;//enable UART
}
void initSerial2 ( void ){
    U2MODEbits.UARTEN = 0;//disable;
    U2MODE = 0;//clear that bitch
    U2STA = 0;// clea this bitch
    U2BRG = 25; // set baud to 38461
    U2MODEbits.BRGH = 1; //this needs to be a 1 for High mode
    U2STAbits.UTXEN = 1; // enable transmit
    IEC1bits.U2RXIE = 1;//enable interrupt
    IFS1bits.U2RXIF = 0;//clear interupt flag
    U2MODEbits.UARTEN = 1;//enable UART
}

void txSerial(char msg[]){
    int length = strlen(msg);
    int i;
    U2STAbits.UTXEN = 1;
    for(i=0;i<length;i++){
        U2TXREG = msg[i];
        while(!U2STAbits.TRMT){};
    }//end for
}//tx