/* 
 * File:   SerialComm.h
 * Author: robo-waiter
 *
 * Created on December 31, 2006, 7:33 PM
 */

#ifndef SERIALCOMM_H
#define	SERIALCOMM_H

#include <stdio.h>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>
#include <string>  /* String function definitions */
#include <string.h>

using namespace std;

class SerialComm {
    
public:
    int connect(void);
    void closeConnection(void);
    SerialComm();
    SerialComm(const SerialComm& orig);
    virtual ~SerialComm();
    void WritePort(unsigned char* writeBuff);
    int ReadPort(unsigned char* readBuff);
    
private:
    int ConfigCommPort();
    int commPort;
    struct termios config;
    string logMsg;
};

#endif	/* SERIALCOMM_H */

