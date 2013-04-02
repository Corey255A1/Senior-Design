/* 
 * File:   SerialComm.h
 * @author Design Team 12
 *
 * Created on December 31, 2006, 7:33 PM
 * 
 * Header for the SerialComm class. This class is used to handle opening,
 * writing to, reading from, and closing a serial port. Within the scope of
 * the project, this enables serial communication between the SBC and the
 * master PIC.
 */

#ifndef SERIALCOMM_H

//-----------------------------------------------------------------------------
//  Include directories.
//-----------------------------------------------------------------------------
#define	SERIALCOMM_H
#include <stdio.h>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>
#include <string>  /* String function definitions */
#include <string.h>

using namespace std;

enum PACKET_TYPE {START, DATA_SIZE, DATA_MINE, JUNK};
//-----------------------------------------------------------------------------
//  Class variable and function declarations.
//-----------------------------------------------------------------------------
class SerialComm {
    
public:
    int connect(void);
    void closeConnection(void);
    SerialComm();
    SerialComm(const SerialComm& orig);
    virtual ~SerialComm();
    void WritePort(unsigned char* puszWriteBuff);
    int ReadPort(unsigned char* puszReadBuff);
    void ZeroWritePort(unsigned char* puszWriteBuff, char buffSize);
    
private:
    int ConfigCommPort();
    int commPort;
    struct termios config;
    string sLogMsg;
    enum PACKET_TYPE packetType;
    
};

#endif	/* SERIALCOMM_H */

