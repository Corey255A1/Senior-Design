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
char MSGSTART=0;
int EXPECTEDLENGTH;
/**
 * Here the UART1 Receive Interrupt gets the message and stores upto 20
 * characters. It breaks and indicates message received when it receives an
 * ! (exclamation point)
 */
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(){
    lastchar = U2RXREG;
    U2STAbits.UTXEN = 1;
//    U1TXREG = lastchar;// uncomment for echoing commands
    if(!RXMessage.Received){
        RXMessage.Msg[ch]=lastchar;
        //IF we know it is the first byte of our message is a ! then we
        //know a legit message is arriving
        if((MSGSTART==0) && (lastchar=='!') && (ch==0)){
            MSGSTART = 1;
        }//endif

        //We then receive a byte after the message has started
        //This byte we know is the message length after the current byte
        else if((MSGSTART==1) && (ch==0)){
            EXPECTEDLENGTH = lastchar;
            ch++;
        }//endelse

        //We have received more bytes and we have not reached our length
        //just increment amount of bytes received
        else if((MSGSTART==1) && (ch>0) && (ch<EXPECTEDLENGTH)){
            ch++;
        }//endelse

        //We have reached the number of expected bytes
        //Therefore reset message start and byte count and
        //indicate that we have received a message for processing
        else if(((MSGSTART==1) && (ch>=EXPECTEDLENGTH))){
            RXMessage.Length=ch-1;
            MSGSTART = 0;
            ch=0;
            RXMessage.Received = 1;
        };
    }//endif
    IFS1bits.U2RXIF = 0;
}

/**
 * Setups UART1 and puts
 * :Receive Pin on RP12
 * :Transmit Pin on RP11
 * It then sets up for 38460 baud
 * 
 */
void initSerial1 ( void ){
    RPINR19bits.U2RXR = 12; //
    RPOR5bits.RP11R = 5;
    U2MODEbits.UARTEN = 0;//disable;
    U2MODE = 0;//clear that bitch
    U2STA = 0;// clea this bitch
    U2BRG = 103; // set baud to 38461
    U2MODEbits.BRGH = 0; //this needs to be a 1 for High mode
    U2STAbits.UTXEN = 1; // enable transmit
    IFS1bits.U2RXIF = 0;//clear interupt flag
    IEC1bits.U2RXIE = 1;//enable interrupt
    U2MODEbits.UARTEN = 1;//enable UART
}

/**
 * This takes in a character array (msg) of (size)
 * and loops through and transmits each byte
 * @param msg - the message buffer
 * @param size - the number of elements
 */
void txSerial1(char msg[],int size){
    int i;
    U2STAbits.UTXEN = 1;
    for(i=0;i<size;i++){
        U2TXREG = msg[i];
        while(!U2STAbits.TRMT){};
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
    U2STAbits.UTXEN = 1;
    for(i=0;i<length;i++){
        U2TXREG = msg[i];
        while(!U2STAbits.TRMT){};
    }//end for
}//tx