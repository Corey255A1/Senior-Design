/*
 * File:   serialcomm.h
 * Author: Corey
 *
 * Created on March 25, 2013, 2:35 PM
 */

#define SET 'S'
#define GET 'G'
#define DCMOTOR 'M'
#define SENSORS 'S'
#define ARM 'A'
#define TERM '!'
#define TEMP 'T'
#define WLED 'W'
#define COMPASS 'C'
#define ULTRAS 'U'
#define ALLSENS 'A'
#define IRLED 'I'
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

