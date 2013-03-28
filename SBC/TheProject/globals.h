/* 
 * File:   globals.h
 * Author: robowaiter
 *
 * Created on March 26, 2013, 4:36 AM
 */

#include <string>
#include <string.h>
#include <sstream>
using namespace std;

#define FAIL                    0
#define FALSE                   0
#define TRUE                    1
#define SUCCESS                 1
#define FAIL_OPEN_SERIAL        -1
#define FAIL_NO_TERM_REF        -2
#define FAIL_GET_ATTRIB         -3
#define BUFF_SIZE               100



#ifndef GLOBALS_H
#define	GLOBALS_H


string IntToString(int number);
void uCharCat(unsigned char* destBuff, unsigned char* sourceBuff, int destBuffSize);

#endif	/* GLOBALS_H */

