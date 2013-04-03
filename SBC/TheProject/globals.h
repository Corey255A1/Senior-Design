/* 
 * File:   globals.h
 * @author Design Team 12
 *
 * Created on March 26, 2013, 4:36 AM
 * 
 * General global declaration and definitions for any variables and helper
 * functions that are deemed necessary.
 */

//-----------------------------------------------------------------------------
//  Include directories
//-----------------------------------------------------------------------------
#include <string>
#include <string.h>
#include <sstream>
using namespace std;

//-----------------------------------------------------------------------------
//  General definitions.
//-----------------------------------------------------------------------------
#define FAIL                    0
#define FALSE                   0
#define TRUE                    1
#define SUCCESS                 1
#define FAIL_OPEN_SERIAL        -1
#define FAIL_NO_TERM_REF        -2
#define FAIL_GET_ATTRIB         -3
#define BUFF_SIZE               100
#define LOW_SHELF_CLEAN_FREQ    3800
#define UPPER_SHELF_FREQ        2500
#define COMPASS_DIVISOR         8192
#define CENTER_OFFSET           15
#define PULSES_TO_CM            100
#define COMPASS_ERROR           0.07
#define ULT_STOP_THRESH         10
#define ADJ_LEFT                1
#define ADJ_RIGHT               2
#define NO_ADJ                  3

#define MOTOR_STOP() BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed0, ucReverse, ucSpeed0); serialPort.ZeroWritePort(uszCommOutMsg, ucSetMotorPacketSize); serialPort.ReadPort(uszCommInMsg)
#define WR_SERIAL() serialPort.WritePort(uszCommOutMsg); serialPort.ReadPort(uszCommInMsg)
#define SPIN_BOT_CLK() BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed4, ucForward, ucSpeed4); serialPort.WritePort(uszCommOutMsg); serialPort.ReadPort(uszCommInMsg)
#define SPIN_BOT_CCLK() BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucReverse, ucSpeed4); serialPort.WritePort(uszCommOutMsg); serialPort.ReadPort(uszCommInMsg)
//-----------------------------------------------------------------------------
//  Helper function declarations.
//-----------------------------------------------------------------------------
#ifndef GLOBALS_H
#define	GLOBALS_H


string IntToString(int nNumber);
void UnCharCat(unsigned char* puszDestBuff, unsigned char* puszSourceBuff, int nDestBuffSize);
int BytesToInt(unsigned char* puszBuff, char pos1, char pos2);
char CheckAck(unsigned char* puszBuff);
#endif	/* GLOBALS_H */

