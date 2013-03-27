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
    // Function specific variables
    //-------------------------------------------------------------------------
    pthread_t thread;
    int rc;
    long t = 0;
    string logFilePath = "/home/robowaiter/Desktop/logfile2.txt";
    string testing;
    string logMsg;
    
    //OpenLogFile(logFilePath);

    
    //CloseLogFile();
    
    SerialComm serComm;
    
    rc = serComm.connect();
    
    if (rc != SUCCESS)
    {
        return rc;
    }
    
    unsigned char commOutMsg[BUFF_SIZE+1] = "EMMA WATSON!";
    
    unsigned char commInMsg[BUFF_SIZE+1] = "";
    int byteCount;
    while (1)
    {
        serComm.WritePort(commOutMsg);
        byteCount = serComm.ReadPort(commInMsg);
    }
    serComm.closeConnection();
    
    
    //-------------------------------------------------------------------------
    //  Create and let the color tracking thread start running.
    //-------------------------------------------------------------------------
//    printf("Creating thread...\n");
//    rc = pthread_create(&thread, NULL, ColorTrackingThread, (void *)t);
//    
//    //-------------------------------------------------------------------------
//    //  Check to make sure thread executed successfully.
//    //-------------------------------------------------------------------------
//    if (rc)
//    {
//        printf("ERROR: Bad return code on thread creation: %d\n", rc);
//        exit(-1);
//    }
//    
//    //-------------------------------------------------------------------------
//    //  Main work loop. Loop exit depends on completion of competition trial.
//    //-------------------------------------------------------------------------
//    while(1)
//    {
//        
//    }
//    
//    //-------------------------------------------------------------------------
//    //  Cleanup
//    //-------------------------------------------------------------------------
//    pthread_exit(NULL);
    return 0;
}

