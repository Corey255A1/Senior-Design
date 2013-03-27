#include "LogFile.h"
#include "globals.h"
#include <stdio.h>   /* Standard input/output definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <string.h>
#include <time.h>


int logFD = 0;
string logFilePath = "none";
bool logOpen = FALSE;
ofstream logFileStream;

/**
 * Open a log file for writing to.
 * @return SUCCESS or FAIL
 */
int OpenLogFile(string path)
{
    logFilePath = path;
    //-------------------------------------------------------------------------
    //  Open file with append mode.
    //-------------------------------------------------------------------------
    logFileStream.open(path.c_str(), ios::app);
    
    //-------------------------------------------------------------------------
    //  If the file successfully opened, then set the flag for the other
    //  functions to know that the file is open..
    //-------------------------------------------------------------------------
    if (logFileStream.is_open())
    {
        logOpen = TRUE;
    }
    //-------------------------------------------------------------------------
    //  .. Else return FAIL.
    //-------------------------------------------------------------------------
    else
    {
        return FAIL;
    }
    
    //-------------------------------------------------------------------------
    //  If flow reaches here, then everything is good.
    //-------------------------------------------------------------------------
    return SUCCESS;
}

/**
 * Closes the log file (if open)
 * @return SUCCESS on graceful close, and FAIL it the file is still open.
 */
int CloseLogFile()
{
    //-------------------------------------------------------------------------
    //  If the log file is open
    //
    //          - Change file open status flag to FALSE.
    //          - Make the call to close the file.
    //-------------------------------------------------------------------------
    if (logOpen)
    {
        logOpen = FALSE;
        logFileStream.close();
    }
    
    //-------------------------------------------------------------------------
    //  Check to make sure the file closed successfully.
    //-------------------------------------------------------------------------
    if (logFileStream.is_open())
    {
        return FAIL;
    }
    
    return SUCCESS;
}

/**
 * Gets the path of the log file that is currently set for writing.
 * @return the literal path used for the log file.
 */
string GetLogFilePath()
{
    return logFilePath;
}

/**
 * Write data to the log file. (Output formatting is done by user caller)
 * @param data Buffer of information to write to the file.
 */
void WriteToLogFile(string data)
{
    
    //-------------------------------------------------------------------------
    //  Check to make sure the log file is open, then write.
    //-------------------------------------------------------------------------
    if (logOpen)
    {
        logFileStream << data;
    }
}