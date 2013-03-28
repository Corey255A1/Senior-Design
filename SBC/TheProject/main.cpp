/* 
 * File:   main.cpp
 * Author: robo-waiter
 *
 * Created on December 31, 2006, 7:07 PM
 */

#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "SerialComm.h"
#include "ColorTracking.h"  /* UNIX standard function definitions */
#include "globals.h"
#include "LogFile.h"

using namespace std;

#define NUM_THREADS     5


/**
 * The thread for Robo-Waiter's camera.
 * @param threadID
 * @return void
 */
void* ColorTrackingThread(void *threadid)
{
    long tID = (long) threadid;
    
    printf("Starting Color Tracking Thread with thread id: #%ld\n", tID);
    
    ColorTracking colorTracker;
    colorTracker.RunColorTracking();
    
    pthread_exit(NULL);
    
}

/*
 * 
 */
int main(int argc, char** argv) 
{
    //-------------------------------------------------------------------------
    //  Function specific variables - Native Types
    //-------------------------------------------------------------------------
    unsigned char commOutMsg[BUFF_SIZE+1] = "EMMA WATSON!";
    unsigned char commInMsg[BUFF_SIZE+1] = "";
    int rc;
    long t = 0;
    
    //-------------------------------------------------------------------------
    //  Function specific variables - Objects
    //-------------------------------------------------------------------------
    pthread_t thread;
    SerialComm serialPort;
    string logFilePath = "/home/robowaiter/Desktop/logfile2.txt";
    string logMsg;
    
    //-------------------------------------------------------------------------
    //  Open file hand for log output.
    //-------------------------------------------------------------------------
    OpenLogFile(logFilePath);

    //-------------------------------------------------------------------------
    //  Create and let the color tracking thread start running.
    //-------------------------------------------------------------------------
    logMsg = "Creating thread...\n";
    WriteToLogFile(logMsg);
    //rc = pthread_create(&thread, NULL, ColorTrackingThread, (void *)t);
    
    //-------------------------------------------------------------------------
    //  Check to make sure thread executed successfully.
    //-------------------------------------------------------------------------
//    if (rc)
//    {
//        logMsg = "Thread Error: Bad return code on thread creation: " + IntToString(rc) + ". Exiting...\n";
//        exit(-1);
//    }
    
    //-------------------------------------------------------------------------
    //  Attempt to open serial port for Master Pic communication.
    //-------------------------------------------------------------------------
    rc = serialPort.connect();
    
    //-------------------------------------------------------------------------
    //  Check to make sure the serial port opened successfully.
    //-------------------------------------------------------------------------
    if (rc != SUCCESS)
    {
        logMsg = "Communication Error: Failed to open serial communication:" + IntToString(rc) + ". Exiting...\n";
        WriteToLogFile(logMsg);
        exit(-1);
    }
    
    int byteCount;
    
    //-------------------------------------------------------------------------
    //  Main Working loop. Exit still has yet to be determined.
    //-------------------------------------------------------------------------
    while (1)
    {
        //---------------------------------------------------------------------
        //  Gather camera information.
        //---------------------------------------------------------------------
        
        //---------------------------------------------------------------------
        //  Write or Query PIC information.
        //---------------------------------------------------------------------
        serialPort.WritePort(commOutMsg);
        byteCount = serialPort.ReadPort(commInMsg);
    }
    serialPort.closeConnection();
    
    
    
    //-------------------------------------------------------------------------
    //  Cleanup
    //-------------------------------------------------------------------------
    //pthread_exit(NULL);
    CloseLogFile();
    return 0;
}

