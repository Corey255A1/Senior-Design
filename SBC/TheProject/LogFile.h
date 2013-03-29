/* 
 * File:   LogFile.h
 * @author Design Team 12
 *
 * Created on March 26, 2013, 5:48 AM
 * 
 * Header for the log file handling functions.This class can be used to open,
 * write to, and close log files for debugging or any general runtime data
 * mining.
 */

//-----------------------------------------------------------------------------
//  Include directories.
//-----------------------------------------------------------------------------
#ifndef LOGFILE_H
#define	LOGFILE_H
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

//-----------------------------------------------------------------------------
//  Variable and function declearations.
//-----------------------------------------------------------------------------
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

