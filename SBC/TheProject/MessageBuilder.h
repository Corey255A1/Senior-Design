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


    // Functions
    void ResetMessage(unsigned char* puszmMgBuff);
    void BuildMotorSet(unsigned char* puszMsgBuff, unsigned char ucMD1, unsigned char ucMS1, unsigned char ucMD2, unsigned char ucMS2);
    void BuildMotorGet(unsigned char* puszMsgBuff);
    void BuildArmSet(unsigned char* puszMsgBuff, unsigned char ucX, unsigned char ucY);
    void BuildArmGet(unsigned char* puszMsgBuff);
    void BuildSensGet(unsigned char* puszMsgBuff, unsigned char ucSensorSelect);
    
    extern const unsigned char ucMsgEndSym;
    
    // Sensor Select Constants.
    extern const unsigned char ucTempSensSel;
    extern const unsigned char ucWhiteLEDSel;
    extern const unsigned char ucInfLEDSel;
    extern const unsigned char ucCompassSel;
    extern const unsigned char ucUltSel;
    extern const unsigned char ucAllDevSel;
    
    // Speed Select Constant.
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
    
    // Direction Select.
    extern const unsigned char ucForward;
    extern const unsigned char ucReverse;


#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGEBUILDER_H */

