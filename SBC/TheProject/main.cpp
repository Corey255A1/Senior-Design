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
#include "TheMap.h"
#include "SoundRecorder.h"
#include "SoundFFT.h"

using namespace std;

#define NUM_THREADS     5

enum STATE {
    INITIALIZE,
    WAIT_FOR_TONE,
    SCAN_FOR_POS,
    FIND_FRIDGE_TRIGGER,
    DETECT_FRIDGE_IR,
    OPEN_DOOR,
    LINE_UP_WITH_FRIDGE,
    GET_PLATE_FRIDGE,
    CLOSE_DOOR,
    GO_TO_TABLE,
    PLACE_PLATE,
    RETURN_START,
    GET_PLATE_TABLE,
    GO_TO_SINK    
};

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
    unsigned char uszCommOutMsg[BUFF_SIZE+1] = {0};
    unsigned char uszCommInMsg[BUFF_SIZE+1] = {0};
    int nRC;
    long t = 0;
    
    //-------------------------------------------------------------------------
    //  Function specific variables - Objects
    //-------------------------------------------------------------------------
    pthread_t thread;
    SerialComm serialPort;
    string sLogFilePath = "/home/robowaiter/Desktop/logfile2.txt";
    string sLogMsg;
    enum STATE state = INITIALIZE;
    SoundRecorder soundRecorder;
    
    switch (state)
    {
        case INITIALIZE:
            //-------------------------------------------------------------------------
            //  Open file hand for log output.
            //-------------------------------------------------------------------------
//            OpenLogFile(sLogFilePath);

            //-------------------------------------------------------------------------
            //  Attempt to open serial port for Master Pic communication.
            //-------------------------------------------------------------------------
//            nRC = serialPort.connect();

            //-------------------------------------------------------------------------
            //  Check to make sure the serial port opened successfully.
            //-------------------------------------------------------------------------
//            if (nRC != SUCCESS)
//            {
//                sLogMsg = "Communication Error: Failed to open serial communication:" + IntToString(rc) + ". Exiting...\n";
//                WriteToLogFile(sLogMsg);
//                exit(-1);
//            }

            //-------------------------------------------------------------------------
            //  Create and let the color tracking thread start running.
            //-------------------------------------------------------------------------
//            sLogMsg = "Creating thread...\n";
//            WriteToLogFile(sLogMsg);
            //nRC = pthread_create(&thread, NULL, ColorTrackingThread, (void *)t);

            //-------------------------------------------------------------------------
            //  Check to make sure thread executed successfully.
            //-------------------------------------------------------------------------
//            if (nRC)
//            {
//                sLogMsg = "Thread Error: Bad return code on thread creation: " + IntToString(rc) + ". Exiting...\n";
//                exit(-1);
//            }
            
//            state = WAIT_FOR_TONE;
            break;
            
        case WAIT_FOR_TONE:
            //-------------------------------------------------------------------------
            //  Sound Processing
            //-------------------------------------------------------------------------
            
            break;
            
        case SCAN_FOR_POS:
            break;
            
        case FIND_FRIDGE_TRIGGER:
            
            state = DETECT_FRIDGE_IR;
            break;
            
        case DETECT_FRIDGE_IR:
            break;
            
        case OPEN_DOOR:
            break;
            
        case LINE_UP_WITH_FRIDGE:
            state = GET_PLATE_FRIDGE;
            break;
            
        case GET_PLATE_FRIDGE:
            break;
            
        case CLOSE_DOOR:
            break;
            
        case GO_TO_TABLE:
            break;
            
        case PLACE_PLATE:
            break;
            
        case RETURN_START:
            break;
            
        case GET_PLATE_TABLE:
            break;
            
        case GO_TO_SINK:
            break;
            
        default:
            break;
    }
    
    
    
    
    //-------------------------------------------------------------------------
    //  Determine Location
    //-------------------------------------------------------------------------
    
    
    
    
    int tempCount = 0;
    float pi = 3.14;
    unsigned char piFixed = pi * 32;
    
    uszCommOutMsg[0] = 'G';
    uszCommOutMsg[1] = 'S';
    //commOutMsg[2] = piFixed;
    uszCommOutMsg[2] = '!';
    int tempSize = sizeof(uszCommOutMsg);
    UnCharCat(uszCommOutMsg, uszCommOutMsg, tempSize);
    //int temp = strlen(reinterpret_cast <const char*>(commOutMsg));
    
    //commOutMsg[2] = piFixed;
    //-------------------------------------------------------------------------
    //  Main Working loop. Exit still has yet to be determined.
    //-------------------------------------------------------------------------
    while (1)
    {
            serialPort.WritePort(uszCommOutMsg);
            
            serialPort.ReadPort(uszCommInMsg);
        
        //---------------------------------------------------------------------
        //  Gather camera information.
        //---------------------------------------------------------------------
        
        //---------------------------------------------------------------------
        //  Write or Query PIC information.
        //---------------------------------------------------------------------
        
        ++tempCount;
    }
    serialPort.closeConnection();
    
    
    
    //-------------------------------------------------------------------------
    //  Cleanup
    //-------------------------------------------------------------------------
    //pthread_exit(NULL);
    CloseLogFile();
    return 0;
}

