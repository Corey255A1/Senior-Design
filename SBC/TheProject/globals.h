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

#include "MessageBuilder.h"
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
#define FREQ_THRESH             200
#define COMPASS_DIVISOR         8192
#define CENTER_OFFSET           15
#define CM_TO_PULSES            (double) 0.0737
#define COMPASS_ERROR           0.25
#define ULT_STOP_THRESH         10
#define ADJ_LEFT                1
#define ADJ_RIGHT               2
#define NO_ADJ                  3
#define COMPASS_THRESH          2000
#define GYRO_THRESH             1000
#define SPIN_CLK                1
#define SPIN_CCLK               2

#define DEMO_ULT_FREQ           2500
#define DEMO_SPEED_FREQ         1200
#define DEMO_COURSE_FREQ        3800
#define DEMO_COLOR_FREQ         4400
#define DEMO_STOP_FREQ          4900
#define DEMO_BOX_FREQ           5400
#define DEMO_FOLLOW_FREQ        800
#define DEMO_CLAW_FREQ          2000


#define MOTOR_STOP() BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed0, ucReverse, ucSpeed0, 0, 0, 0); serialPort.WritePort(uszCommOutMsg, sizeof(uszCommOutMsg)); serialPort.ReadPort(uszCommInMsg)
#define WR_SET_MOTOR() serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); serialPort.ReadPort(uszCommInMsg)
#define WR_GET_MOTOR() serialPort.WritePort(uszCommOutMsg, ucGetMotorPacketSize); serialPort.ReadPort(uszCommInMsg)
#define WR_SET_ARM() serialPort.WritePort(uszCommOutMsg, ucSetArmPacketSize); serialPort.ReadPort(uszCommInMsg)
#define WR_GET_ARM() serialPort.WritePort(uszCommOutMsg, ucGetArmPacketSize); serialPort.ReadPort(uszCommInMsg)
#define WR_GET_SENS() serialPort.WritePort(uszCommOutMsg, ucGetSensorPacketSize); serialPort.ReadPort(uszCommInMsg)
//#define SPIN_BOT_CLK() BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucReverse, ucSpeed4, 95); serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); serialPort.ReadPort(uszCommInMsg)
//#define SPIN_BOT_CCLK() BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed4, ucForward, ucSpeed4, 95); serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); serialPort.ReadPort(uszCommInMsg)
//-----------------------------------------------------------------------------
//  Helper function declarations.
//-----------------------------------------------------------------------------
#ifndef GLOBALS_H
#define	GLOBALS_H


string IntToString(int nNumber);
void UnCharCat(unsigned char* puszDestBuff, unsigned char* puszSourceBuff, int nDestBuffSize);
int BytesToInt(unsigned char* puszBuff, char pos1, char pos2);
char CheckAck(unsigned char* puszBuff);
long BytesToLong(unsigned char* puszBuff, char pos1, char pos2, char pos3, char pos4);

#endif	/* GLOBALS_H */

