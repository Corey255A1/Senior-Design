/* 
 * File:   MessageBuilder.h
 * @author Design Team 12
 *
 * Created on March 26, 2013, 1:23 PM
 */

#ifndef MESSAGEBUILDER_H
#define	MESSAGEBUILDER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define GYRO_GET_MSB    1
#define GYRO_GET_LSB    2

    //-------------------------------------------------------------------------
    // Functions
    //-------------------------------------------------------------------------
    void ResetMessage(unsigned char* puszmMgBuff);
    void BuildMotorSet(unsigned char* puszMsgBuff, unsigned char ucMD1, unsigned char ucMS1, unsigned char ucMD2, unsigned char ucMS2, unsigned char distance, unsigned char ucUseHeading, int nHeading);
    void BuildMotorGet(unsigned char* puszMsgBuff);
    void BuildArmSet(unsigned char* puszMsgBuff, int nLink1Rad, int nLink2Rad);
    void BuildArmGet(unsigned char* puszMsgBuff);
    void BuildSensGet(unsigned char* puszMsgBuff, unsigned char ucSensorSelect);
    void BuildClawSet(unsigned char* puszMsgBuff, unsigned char ucClawPos);
    void BuildShortArmSet(unsigned char* puszMsgBuff, int nLink2Rad);
    void BuildLongArmSet(unsigned char* puszMsgBuff, int nLink1Rad);
    
    //-------------------------------------------------------------------------
    //  Message Terminator
    //-------------------------------------------------------------------------
    extern const unsigned char ucMsgStartSym;
    
    //-------------------------------------------------------------------------
    // Componenet Select
    //-------------------------------------------------------------------------
    extern const unsigned char ucMotor;
    extern const unsigned char ucArm;
    extern const unsigned char ucSensor;
    extern const unsigned char ucClaw;
    extern const unsigned char ucShortArm;
    extern const unsigned char ucLongArm;
    
    //-------------------------------------------------------------------------
    // Serial Port Request Options.
    //-------------------------------------------------------------------------
    extern const unsigned char ucSet;
    extern const unsigned char ucGet;
    
    //-------------------------------------------------------------------------
    // Sensor Select Constants.
    //-------------------------------------------------------------------------
    extern const unsigned char ucTempSensSel;
    extern const unsigned char ucWhiteLEDSel;
    extern const unsigned char ucInfLEDSel;
    extern const unsigned char ucCompassSel;
    extern const unsigned char ucUltSel;
    extern const unsigned char ucAllDevSel;
    extern const unsigned char ucGyroSel;
    
    //-------------------------------------------------------------------------
    // Speed Select Constant.
    //-------------------------------------------------------------------------
    extern const unsigned char ucSpeed0;
    extern const unsigned char ucSpeed1;
    extern const unsigned char ucSpeed2;
    extern const unsigned char ucSpeed3;
    extern const unsigned char ucSpeed4;
    extern const unsigned char ucSpeed5;
    extern const unsigned char ucSpeed6;
    extern const unsigned char ucSpeed7;
    extern const unsigned char ucSpeed8;
    extern const unsigned char ucSpeed9;
    extern const unsigned char ucSpeed10;
    
    //-------------------------------------------------------------------------
    // Direction Select.
    //-------------------------------------------------------------------------
    extern const unsigned char ucForward;
    extern const unsigned char ucReverse;
    
    //-------------------------------------------------------------------------
    // Data Size Constants
    //-------------------------------------------------------------------------
    extern const unsigned char ucSetMotorDataSize;
    extern const unsigned char ucGetMotorDataSize;
    extern const unsigned char ucSetArmDataSize;
    extern const unsigned char ucGetArmDataSize;
    extern const unsigned char ucGetSensorDataSize;
    extern const unsigned char ucSetClawDataSize;
    extern const unsigned char ucSetLongArmDataSize;
    extern const unsigned char ucSetShortArmDataSize;
    
    //-------------------------------------------------------------------------
    // Packet Sizes
    //-------------------------------------------------------------------------
    extern const unsigned char ucSetMotorPacketSize;
    extern const unsigned char ucGetMotorPacketSize;
    extern const unsigned char ucSetArmPacketSize;
    extern const unsigned char ucGetArmPacketSize;
    extern const unsigned char ucGetSensorPacketSize;
    extern const unsigned char ucSetClawPacketSize;
    extern const unsigned char ucSetLongArmPacketSize;
    extern const unsigned char ucSetShortArmPacketSize;
    
    //-----------------------------------------------------------------------------
    // Array Message Positions.
    //-----------------------------------------------------------------------------
    extern const unsigned char ucMsgIdentifier;
    
    extern const unsigned char ucUltLeftFrontMSB;
    extern const unsigned char ucUltLeftFrontLSB;
    extern const unsigned char ucUltLeftBackMSB;
    extern const unsigned char ucUltLeftBackLSB;
    extern const unsigned char ucUltRightFrontMSB;
    extern const unsigned char ucUltRightFrontLSB;
    extern const unsigned char ucUltRightBackMSB;
    extern const unsigned char ucUltRightBackLSB;
    extern const unsigned char ucUltFrontMSB;
    extern const unsigned char ucUltFrontLSB;
    extern const unsigned char ucUltBackMSB;
    extern const unsigned char ucUltBackLSB;
    
    extern const unsigned char ucLeftWheelMSB1;
    extern const unsigned char ucLeftWheelMSB2;
    extern const unsigned char ucLeftWheelLSB1;
    extern const unsigned char ucLeftWheelLSB2;
    extern const unsigned char ucRightWheelMSB1;
    extern const unsigned char ucRightWheelMSB2;
    extern const unsigned char ucRightWheelLSB1;
    extern const unsigned char ucRightWheelLSB2;
   
    extern const unsigned char ucArmMSB;
    extern const unsigned char ucArmLSB;
    
    extern const unsigned char ucCompassMSB;
    extern const unsigned char ucCompassLSB;
    
    extern const unsigned char ucClawOpenPos;
    extern const unsigned char ucClawClosedPos;


#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGEBUILDER_H */

