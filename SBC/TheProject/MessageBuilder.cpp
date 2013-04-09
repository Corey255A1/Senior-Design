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
const unsigned char ucForward           = 0;
const unsigned char ucReverse           = 1;

//-----------------------------------------------------------------------------
// Message Terminator
//-----------------------------------------------------------------------------
const unsigned char ucMsgStartSym       = '!';

//-----------------------------------------------------------------------------
// Data Size Constants
//-----------------------------------------------------------------------------
const unsigned char ucSetMotorDataSize  = 7;
const unsigned char ucGetMotorDataSize  = 2;
const unsigned char ucSetArmDataSize    = 4;
const unsigned char ucGetArmDataSize    = 2;
const unsigned char ucGetSensorDataSize = 3;

//-----------------------------------------------------------------------------
// Packet Sizes
//-----------------------------------------------------------------------------
const unsigned char ucSetMotorPacketSize        = 9;
const unsigned char ucGetMotorPacketSize        = 4;
const unsigned char ucSetArmPacketSize          = 6;
const unsigned char ucGetArmPacketSize          = 4;
const unsigned char ucGetSensorPacketSize       = 5;

//-----------------------------------------------------------------------------
// Array Message Positions.
//-----------------------------------------------------------------------------
const unsigned char ucMsgIdentifier     = 0;

// Ultrasonic Definitions
const unsigned char ucUltLeftFrontMSB     = 1;
const unsigned char ucUltLeftFrontLSB     = 2;

const unsigned char ucUltLeftBackMSB      = 3;
const unsigned char ucUltLeftBackLSB      = 4;

const unsigned char ucUltRightFrontMSB    = 5;
const unsigned char ucUltRightFrontLSB    = 6;

const unsigned char ucUltRightBackMSB     = 7;
const unsigned char ucUltRightBackLSB     = 8;

const unsigned char ucUltFrontMSB         = 9;
const unsigned char ucUltFrontLSB         = 10;

const unsigned char ucUltBackMSB          = 11;
const unsigned char ucUltBackLSB          = 12;

// Wheel Definitions
const unsigned char ucLeftWheelMSB1     = 1;
const unsigned char ucLeftWheelMSB2     = 2;
const unsigned char ucLeftWheelLSB1     = 3;
const unsigned char ucLeftWheelLSB2     = 4;

const unsigned char ucRightWheelMSB1    = 5;
const unsigned char ucRightWheelMSB2    = 6;
const unsigned char ucRightWheelLSB1    = 7;
const unsigned char ucRightWheelLSB2    = 8;



// Compass Definitions
const unsigned char ucCompassMSB        = 1;
const unsigned char ucCompassLSB        = 2;

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
void BuildMotorSet(unsigned char* puszMsgBuff, unsigned char ucMD1, unsigned char ucMS1, unsigned char ucMD2, unsigned char ucMS2, unsigned char distance)
{
    puszMsgBuff[0]      = ucMsgStartSym;
    puszMsgBuff[1]      = ucSetMotorDataSize;
    puszMsgBuff[2]      = ucSet;
    puszMsgBuff[3]      = ucMotor;
    puszMsgBuff[4]      = ucMS2;
    puszMsgBuff[5]      = ucMD2;
    puszMsgBuff[6]      = ucMS1;
    puszMsgBuff[7]      = ucMD1;
    puszMsgBuff[8]      = distance;
    
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

