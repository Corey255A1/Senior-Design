/* 
 * File:   LogFile.h
 * Author: robowaiter
 *
 * Created on March 26, 2013, 5:48 AM
 */

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#ifndef LOGFILE_H
#define	LOGFILE_H

#ifdef	__cplusplus
extern "C" {
#endif
    // Variables
    extern int logFD;
    extern string logFilePath;
    extern bool logOpen;
    
    
    // Functions
    int OpenLogFile(string path);
    int CloseLogFile();
    string GetLogFilePath();
    void WriteToLogFile(string data);

#ifdef	__cplusplus
}
#endif

#endif	/* LOGFILE_H */

