/* 
 * File:   MessageBuilder.cpp
 * @author Design Team 12
 *
 * Created on March 26, 2013, 1:23 PM
 */

#include "MessageBuilder.h"
#include "string.h"

//-----------------------------------------------------------------------------
// Componenet Select
//-----------------------------------------------------------------------------
const unsigned char ucMotor             = 'M';
const unsigned char ucArm               = 'A';
const unsigned char ucSensor            = 'S';
    
//-----------------------------------------------------------------------------
// Serial Port Request Options.
//-----------------------------------------------------------------------------
const unsigned char ucSet               = 'S';
const unsigned char ucGet               = 'G';

//-----------------------------------------------------------------------------
// Sensor Select
//-----------------------------------------------------------------------------
const unsigned char ucTempSensSel       = 'T';
const unsigned char ucWhiteLEDSel       = 'W';
const unsigned char ucInfLEDSel         = 'I';
const unsigned char ucCompassSel        = 'C';
const unsigned char ucUltSel            = 'U';
const unsigned char ucAllDevSel         = 'A';

//-----------------------------------------------------------------------------
// Speed Select Constants
//-----------------------------------------------------------------------------
const unsigned char ucSpeed0            = 0;
const unsigned char ucSpeed1            = 1;
const unsigned char ucSpeed2            = 2;
const unsigned char ucSpeed3            = 3;
const unsigned char ucSpeed4            = 4;
const unsigned char ucSpeed5            = 5;
const unsigned char ucSpeed6            = 6;
const unsigned char ucSpeed7            = 7;
const unsigned char ucSpeed8            = 8;
const unsigned char ucSpeed9            = 9;
const unsigned char ucSpeed10           = 10;

//-----------------------------------------------------------------------------
// Direction Select Constants
//-----------------------------------------------------------------------------
const unsigned char ucForward           = 'F';
const unsigned char ucReverse           = 'R';

//-----------------------------------------------------------------------------
// Message Terminator
//-----------------------------------------------------------------------------
const unsigned char ucMsgStartSym       = '!';

//-------------------------------------------------------------------------
// Data Size Constants
//-------------------------------------------------------------------------
const unsigned char ucSetMotorDataSize  = 6;
const unsigned char ucGetMotorDataSize  = 2;
const unsigned char ucSetArmDataSize    = 4;
const unsigned char ucGetArmDataSize    = 2;
const unsigned char ucGetSensorDataSize = 3;

//-------------------------------------------------------------------------
// Packet Sizes
//-------------------------------------------------------------------------
const unsigned char ucSetMotorPacketSize        = 8;
const unsigned char ucGetMotorPacketSize        = 4;
const unsigned char ucSetArmPacketSize          = 6;
const unsigned char ucGetArmPacketSize          = 4;
const unsigned char ucGetSensorPacketSize       = 5;

/**
 * Fills the whole buffer with only null (0) values.
 * @param msgBuff - Buffer to hold message
 */
void ResetMessage(unsigned char* msgBuff)
{
    int msgByteCount = strlen(reinterpret_cast <const char*>(msgBuff));
    memset(msgBuff, 0, msgByteCount);
}

/**
 * Buils the message for a command to set the speed of both motors.
 * @param puszMsgBuff - Buffer to store the message
 * @param ucMD1 - Direction of Motor 1
 * @param ucMS1 - Speed of Motor 1
 * @param ucMD2 - Direction of Motor 2
 * @param ucMS2 - Speed of Motor 2
 */
void BuildMotorSet(unsigned char* puszMsgBuff, unsigned char ucMD1, unsigned char ucMS1, unsigned char ucMD2, unsigned char ucMS2)
{
    puszMsgBuff[0]      = ucMsgStartSym;
    puszMsgBuff[1]      = ucSetMotorDataSize;
    puszMsgBuff[2]      = ucSet;
    puszMsgBuff[3]      = ucMotor;
    puszMsgBuff[4]      = ucMD1;
    puszMsgBuff[5]      = ucMS1;
    puszMsgBuff[6]      = ucMD2;
    puszMsgBuff[7]      = ucMS2;
    
}

/**
 * Builds the message for a command to get the state of both motors.
 * @param puszMsgBuff - Buffer to hold the message
 */
void BuildMotorGet(unsigned char* puszMsgBuff)
{
    puszMsgBuff[0] = ucMsgStartSym;
    puszMsgBuff[1] = ucGetMotorDataSize;
    puszMsgBuff[2] = ucGet;
    puszMsgBuff[3] = ucMotor;
}

/**
 * Builds the message for a command to set the position of the arm.
 * @param puszMsgBuff - Buffer to hold the message.
 * @param ucX - X position of the arm.
 * @param ucY - Y position of the arm.
 */
void BuildArmSet(unsigned char* puszMsgBuff, unsigned char ucX, unsigned char ucY)
{
    puszMsgBuff[0] = ucMsgStartSym;
    puszMsgBuff[1] = ucSetArmDataSize;
    puszMsgBuff[2] = ucSet;
    puszMsgBuff[3] = ucArm;
    puszMsgBuff[4] = ucX;
    puszMsgBuff[5] = ucY;
}

/**
 * Builds a message to get the current state of the arm.
 * @param puszMsgBuff - Buffer to hold the message
 */
void BuildArmGet(unsigned char* puszMsgBuff)
{
    puszMsgBuff[0] = ucMsgStartSym;
    puszMsgBuff[1] = ucGetArmDataSize;
    puszMsgBuff[2] = ucGet;
    puszMsgBuff[3] = ucArm;
    
}

/**
 * Builds the message to get sensor data .
 * @param puszMsgBuff - Buffer to hold the message.
 * @param ucSensorSelect - Character to select which sensor to querey.
 */
void BuildSensGet(unsigned char* puszMsgBuff, unsigned char ucSensorSelect)
{
    puszMsgBuff[0] = ucMsgStartSym;
    puszMsgBuff[1] = ucGetSensorDataSize;
    puszMsgBuff[2] = ucGet;
    puszMsgBuff[3] = ucSensor;
    puszMsgBuff[4] = ucSensorSelect;
    
}

