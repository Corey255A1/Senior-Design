/* 
 * File:   MessageBuilder.h
 * Author: robowaiter
 *
 * Created on March 26, 2013, 1:23 PM
 */

#ifndef MESSAGEBUILDER_H
#define	MESSAGEBUILDER_H

#ifdef	__cplusplus
extern "C" {
#endif


    //Functions
    //void BuildDriveMessage();   // Drive Message
    //void BuildArmMessage();     // Arm Message
    //Sensor Message
    
    void ResetMessage(unsigned char* msgBuff);


#ifdef	__cplusplus
}
#endif

#endif	/* MESSAGEBUILDER_H */

