/*
 * File:   serialcomm.h
 * Author: Corey
 *
 * Created on March 25, 2013, 2:35 PM
 */

#define BYTECOUNT 0
#define GSHEADER 1
#define DEVICEHEADER 2
#define SENSEHEADER 3

#define SET 'S'
#define GET 'G'
#define DCMOTOR 'M'
#define SENSORS 'S'
#define HEADING 'H'
#define ARM 'A'
#define TERM '!'
#define TEMP 'T'
#define WLED 'W'
#define COMPASS 'C'
#define GYRO 'Y'
#define ULTRAS 'U'
#define ALLSENS 'A'
#define IRLED 'I'
#define SEND_ACK txSerial1("!3ACK",5)
typedef struct{
    char Msg[20];
    char Length;
    unsigned Received:1;
}SERIALMESSAGE;
extern SERIALMESSAGE RXMessage;
void initSerial1( void );
//void initSerial2( void );
void txSerial1(char[],int);
void txSerial1_str(char[]);
//void txSerial2(char[],int);

