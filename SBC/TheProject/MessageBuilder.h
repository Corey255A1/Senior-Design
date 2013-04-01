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

    //-------------------------------------------------------------------------
    // Functions
    //-------------------------------------------------------------------------
    void ResetMessage(unsigned char* puszmMgBuff);
    void BuildMotorSet(unsigned char* puszMsgBuff, unsigned char ucMD1, unsigned char ucMS1, unsigned char ucMD2, unsigned char ucMS2);
    void BuildMotorGet(unsigned char* puszMsgBuff);
    void BuildArmSet(unsigned char* puszMsgBuff, unsigned char ucX, unsigned char ucY);
    void BuildArmGet(unsigned char* puszMsgBuff);
    void BuildSensGet(unsigned char* puszMsgBuff, unsigned char ucSensorSelect);
    
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
    
    //-------------------------------------------------------------------------
    // Packet Sizes
    //-------------------------------------------------------------------------
    extern const unsigned char ucSetMotorPacketSize;
    extern const unsigned char ucGetMotorPacketSize;
    extern const unsigned char ucSetArmPacketSize;
    extern const unsigned char ucGetArmPacketSize;
    extern const unsigned char ucGetSensorPacketSize;


#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGEBUILDER_H */

