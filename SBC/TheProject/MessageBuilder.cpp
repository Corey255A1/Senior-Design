/* 
 * File:   MessageBuilder.cpp
 * @author Design Team 12
 *
 * Created on March 26, 2013, 1:23 PM
 */

#include "MessageBuilder.h"
#include "string.h"

const unsigned char ucTempSensSel  = 'T';
const unsigned char ucWhiteLEDSel  = 'W';
const unsigned char ucInfLEDSel    = 'I';
const unsigned char ucCompassSel   = 'C';
const unsigned char ucUltSel       = 'U';
const unsigned char ucAllDevSel    = 'A';

const unsigned char ucSpeed0    = 0;
const unsigned char ucSpeed1    = 1;
const unsigned char ucSpeed2    = 2;
const unsigned char ucSpeed3    = 3;
const unsigned char ucSpeed4    = 4;
const unsigned char ucSpeed5    = 5;
const unsigned char ucSpeed6    = 6;
const unsigned char ucSpeed7    = 7;
const unsigned char ucSpeed8    = 8;
const unsigned char ucSpeed9    = 9;
const unsigned char ucSpeed10   = 10;

const unsigned char ucForward   = 'F';
const unsigned char ucReverse   = 'R';

const unsigned char ucMsgEndSym = '!';

void ResetMessage(unsigned char* msgBuff)
{
    int msgByteCount = strlen(reinterpret_cast <const char*>(msgBuff));
    memset(msgBuff, 0, msgByteCount);
}

void BuildMotorSet(unsigned char* puszMsgBuff, unsigned char ucMD1, unsigned char ucMS1, unsigned char ucMD2, unsigned char ucMS2)
{
    puszMsgBuff[0]      = 'S';
    puszMsgBuff[1]      = 'M';
    puszMsgBuff[2]      = ucMD1;
    puszMsgBuff[3]      = ucMS1;
    puszMsgBuff[4]      = ucMD2;
    puszMsgBuff[5]      = ucMS2;
    puszMsgBuff[6]      = ucMsgEndSym;
}

void BuildMotorGet(unsigned char* puszMsgBuff)
{
    puszMsgBuff[0] = 'G';
    puszMsgBuff[1] = 'M';
    puszMsgBuff[2] = ucMsgEndSym;
}

void BuildArmSet(unsigned char* puszMsgBuff, unsigned char ucX, unsigned char ucY)
{
    puszMsgBuff[0] = 'S';
    puszMsgBuff[1] = 'A';
    puszMsgBuff[2] = ucX;
    puszMsgBuff[3] = ucY;
    puszMsgBuff[4] = ucMsgEndSym;
}

void BuildArmGet(unsigned char* puszMsgBuff)
{
    puszMsgBuff[0] = 'G';
    puszMsgBuff[1] = 'A';
    puszMsgBuff[2] = ucMsgEndSym;
}

void BuildSensGet(unsigned char* puszMsgBuff, unsigned char ucSensorSelect)
{
    puszMsgBuff[0] = 'G';
    puszMsgBuff[1] = 'S';
    puszMsgBuff[2] = ucSensorSelect;
    puszMsgBuff[3] = ucMsgEndSym;
}

