/**
 * == SerialComm ==
 * This provides functions to setup and use the serial port (UART)
 * It receives messages in the Receive interrupt and waits for
 * the ! terminated message.
 *
 * --Corey
 */
#include <P24FJ128GB106.h>
#include "string.h"
#include "globals.h"
#include "serialcomm.h"

#define READMSG msg; newMsg=0;
char lastchar;
int ch=0;
SERIALMESSAGE RXMessage;

/**
 * Here the UART1 Receive Interrupt gets the message and stores upto 20
 * characters. It breaks and indicates message received when it receives an
 * ! (exclamation point)
 */
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(){
    lastchar = U1RXREG;
    U1STAbits.UTXEN = 1;
    U1TXREG = lastchar;// uncomment for echoing commands
    if(!RXMessage.Received){
        RXMessage.Msg[ch]=lastchar;
        if(lastchar!='!'){
            ch++;
        }else{
            RXMessage.Length=ch;
            RXMessage.Received = 1;
            ch=0;
        };
    }//endif
    IFS0bits.U1RXIF = 0;
}

/**
 * Setups UART1 and puts
 * :Receive Pin on RP12
 * :Transmit Pin on RP11
 * It then sets up for 38460 baud
 * 
 */
void initSerial1 ( void ){
    RPINR18bits.U1RXR = 12; //
    RPOR5bits.RP11R = 3;
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

/**
 * This takes in a character array (msg) of (size)
 * and loops through and transmits each byte
 * @param msg - the message buffer
 * @param size - the number of elements
 */
void txSerial1(char msg[],int size){
    int i;
    U1STAbits.UTXEN = 1;
    for(i=0;i<size;i++){
        U1TXREG = msg[i];
        while(!U1STAbits.TRMT){};
    }//end for
}//tx

/**
 * This takes in a string (msg) and sends it over the line
 * @param msg - the message buffer (null terminated)
 *
 */
void txSerial1_str(char msg[]){
    int length = strlen(msg);
    int i;
    U1STAbits.UTXEN = 1;
    for(i=0;i<length;i++){
        U1TXREG = msg[i];
        while(!U1STAbits.TRMT){};
    }//end for
}//tx