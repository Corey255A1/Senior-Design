/* 
 * File:   main.cpp
 * @author Design Team 12
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
#include "MessageBuilder.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define NUM_THREADS     5

void MoveForward(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char M1speed, unsigned char M2speed, unsigned char cDistance);
void GetUltSensVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void GetMotorVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void StopMoving(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void GetCompassVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void SpinRobot(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nHeading);
SerialComm serialPort;
ColorTracking colorTracker;

char cDebug = FALSE;

enum DEMO_STATE {
    
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
    GO_TO_SINK,
    HARD_CODE,
    HARD_END
};

//enum DEMO_STATE
//{
//    INITIALIZE,
//    WAIT_FOR_TONE
//};

enum DST_PT {
    DST_PT1,
    DST_PT2,
    DST_PT3
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
    
    
    colorTracker.RunColorTracking(cDebug);
    
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
    bool bGetLowShelf = FALSE;
    bool bGetUpperShelf = FALSE;
    float frgSoundSamps[NUM_SECONDS * SAMPLE_RATE];
    double dblFreqSamp;
    long lRightMotorCount;
    long lLeftMotorCount;
    int nFrontUltDist;
    int nBackUltDist;
    int nRightFrontUltDist;
    int nRightBackUltDist;
    int nLeftFrontUltDist;
    int nLeftBackUltDist;
    double curHeading;
    
    //-------------------------------------------------------------------------
    //  Function specific variables - Objects
    //-------------------------------------------------------------------------
    pthread_t thread;
    string sLogFilePath = "/home/robowaiter/Desktop/logfile2.txt";
    string sLogMsg;
    enum DEMO_STATE state = INITIALIZE;
    enum DST_PT dstPt = DST_PT1;
    SoundRecorder soundRecorder;
    SoundFFT soundFFT;
    TheMap theMap;
    COORDINATES roboCoord;
    
    
    
    while (1)
    {
        switch (state)
        {
            case INITIALIZE:
                theMap.display();
                StopMoving(uszCommOutMsg, uszCommInMsg);
                //-----------------------------------------------------------------
                //  Open file hand for log output.
                //-----------------------------------------------------------------
                OpenLogFile(sLogFilePath);

                //-----------------------------------------------------------------
                //  Attempt to open serial port for Master Pic communication.
                //-----------------------------------------------------------------
                nRC = serialPort.connect();

                //-----------------------------------------------------------------
                //  Check to make sure the serial port opened successfully.
                //-----------------------------------------------------------------
                if (nRC != SUCCESS)
                {
                    sLogMsg = "Communication Error: Failed to open serial communication:" + IntToString(nRC) + ". Exiting...\n";
                    WriteToLogFile(sLogMsg);
                    exit(-1);
                }

                //-----------------------------------------------------------------
                //  Create and let the color tracking thread start running.
                //-----------------------------------------------------------------

                sLogMsg = "Creating thread...\n";
                WriteToLogFile(sLogMsg);
                nRC = pthread_create(&thread, NULL, ColorTrackingThread, (void *)t);

                //-----------------------------------------------------------------
                //  Check to make sure thread executed successfully.
                //-----------------------------------------------------------------

                if (nRC)
                {
                    sLogMsg = "Thread Error: Bad return code on thread creation: " + IntToString(nRC) + ". Exiting...\n";
                    exit(-1);
                }
                
                state= WAIT_FOR_TONE;
                //state = HARD_CODE;
                break;

            case WAIT_FOR_TONE:

                //-----------------------------------------------------------------
                //  Sound Processing
                //-----------------------------------------------------------------
                soundRecorder.record(frgSoundSamps);
                dblFreqSamp = soundFFT.getFreq(frgSoundSamps);
                printf("Tone: %f\n", dblFreqSamp);
                
                //-----------------------------------------------------------------
                //  If the higher frequency is heard, then RoboWaiter needs to get
                //  the plate on the lower shelf, and the state needs to advance
                //  to SCAN_FOR_POS...
                //-----------------------------------------------------------------
                if ((dblFreqSamp <= LOW_SHELF_CLEAN_FREQ+FREQ_THRESH) && (dblFreqSamp > LOW_SHELF_CLEAN_FREQ-FREQ_THRESH))
                {
                    bGetUpperShelf  = FALSE;
                    bGetLowShelf    = TRUE;
                    
                }

                //-----------------------------------------------------------------
                //  ... Else if the lower frequency is heard, then RoboWaiter needs 
                //  to get the plate on the lower shelf, and the state needs to 
                //  advance to SCAN_FOR_POS...
                //-----------------------------------------------------------------
                else if ((dblFreqSamp <= UPPER_SHELF_FREQ+FREQ_THRESH) && (dblFreqSamp > UPPER_SHELF_FREQ-FREQ_THRESH))
                {
                    bGetLowShelf    = FALSE;
                    bGetUpperShelf  = TRUE;
                    state = HARD_CODE;
                }

                //-----------------------------------------------------------------
                //  ... Else remain at the current state.
                //-----------------------------------------------------------------
                else
                {
                    state = state;
                }

                break;

            case HARD_CODE:
            {
                int nHeadingGetVal;
                int freakingCount = 0;
                
                ////********
                //      TEST STOPMOVING
//                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, 100);
//                StopMoving(uszCommOutMsg, uszCommInMsg);
                ////********
                // ARM TEST
                // Position "1"
//                BuildArmSet(uszCommOutMsg, 1, 0);
//                WR_SET_ARM();
//                
//                do
//                {
//                    BuildArmGet(uszCommOutMsg);
//                    WR_GET_ARM();
//                    armTest = BytesToInt(uszCommInMsg, ucArmMSB, ucArmLSB);
//                }
//                while (armTest != 1);
                
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, 100);
                
                GetCompassVals(uszCommOutMsg, uszCommInMsg);
                nHeadingGetVal = BytesToInt(uszCommInMsg, ucCompassMSB, ucCompassLSB);
                int n90Turn = (int) ((M_PI/2) * COMPASS_DIVISOR);
                SpinRobot(uszCommOutMsg, uszCommInMsg, nHeadingGetVal + n90Turn);

                MOTOR_STOP();
                if (freakingCount >= 4)
                {
                        state = HARD_END;
                }
                freakingCount++;
                break; 
            }
            case HARD_END:
                MOTOR_STOP();
                MOTOR_STOP();
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
    }
    
    //-------------------------------------------------------------------------
    //  Cleanup
    //-------------------------------------------------------------------------
    serialPort.closeConnection();
    //pthread_exit(NULL);
    CloseLogFile();
    return 0;
}

/**
 * Function to move the robot forward a specified number of centimeters.
 * @param puszCommOutMsg buffer to store the output message to write.
 * @param puszCommInMsg buffer to store the input message.
 * @param M1speed speed of the right motor.
 * @param M2speed speed of the left motor.
 * @param cDistance distance we want the robot to travel.
 */
void MoveForward(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char M1speed, unsigned char M2speed, unsigned char cDistance)
{
    long lLeftWheelCount;
    long lRightWheelCount;
    
    //-------------------------------------------------------------------------
    //  Build message to move forward, then execute command.
    //-------------------------------------------------------------------------
    BuildMotorSet(puszCommOutMsg, ucForward, M1speed, ucForward, M2speed, cDistance, 0, 0);
    serialPort.WritePort(puszCommOutMsg, ucSetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
      
    //-------------------------------------------------------------------------
    // Keep moving until one of the motors reaches the required count.
    //-------------------------------------------------------------------------
    do
    {
        //-------------------------------------------------------------------------
        //  Get updated motor information.
        //-------------------------------------------------------------------------
        GetMotorVals(puszCommOutMsg, puszCommInMsg);


        lLeftWheelCount = BytesToLong(puszCommInMsg, ucLeftWheelMSB1, ucLeftWheelMSB2, ucLeftWheelLSB1, ucLeftWheelLSB2);
        lRightWheelCount = BytesToLong(puszCommInMsg, ucRightWheelMSB1, ucRightWheelMSB2, ucRightWheelLSB1, ucRightWheelLSB2);

    }
    while ((lLeftWheelCount < ((double)cDistance / CM_TO_PULSES)) && (lRightWheelCount < (double) cDistance / CM_TO_PULSES));

    //-------------------------------------------------------------------------
    //  Stop moving.
    //-------------------------------------------------------------------------
    StopMoving(puszCommOutMsg, puszCommInMsg);
}

/**
 * Gets Ultrasonic sensor information
 * @param puszCommOutMsg buffer used to store the output message to write.
 * @param puszCommInMsg buffer used to store the received message from querey
 */
void GetUltSensVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildSensGet(puszCommOutMsg, ucUltSel);
    serialPort.WritePort(puszCommOutMsg, ucGetSensorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

/**
 * Gets the current step values for the motors.
 * @param puszCommOutMsg buffer used to store the output message to write.
 * @param puszCommInMsg buffer used to store the received message from querey.
 */
void GetMotorVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildMotorGet(puszCommOutMsg);
    serialPort.WritePort(puszCommOutMsg, ucGetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

/**
 * Command to stop moving the robot
 * @param puszCommOutMsg buffer used to store the output message to write.
 * @param puszCommInMsg buffer used to store the received message.
 */
void StopMoving(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildMotorSet(puszCommOutMsg, ucForward, ucSpeed0, ucReverse, ucSpeed0, 0, 0, 0); 
    serialPort.WritePort(puszCommOutMsg, ucSetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

/**
 * Spins the robot either clockwise or counter-clockwise
 * @param puszCommOutMsg buffer used to store the output message to write.
 * @param puszCommInMsg buffer used to store the received message.
 * @param nHeading the heading that we want to turn to.
 */
void SpinRobot(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nHeading)
{
    int nLastHeading;
    
    //-------------------------------------------------------------------------
    //  Build message to move forward, then execute command.
    //-------------------------------------------------------------------------
    BuildMotorSet(puszCommOutMsg, ucForward, ucSpeed4, ucReverse, ucSpeed4, 100, 1, nHeading);
    serialPort.WritePort(puszCommOutMsg, ucSetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
         
    //-------------------------------------------------------------------------
    // Keep moving until we reach the required heading.
    //-------------------------------------------------------------------------
    do
    {
        //-------------------------------------------------------------------------
        //  Get updated sensor information.
        //-------------------------------------------------------------------------
        GetCompassVals(puszCommOutMsg, puszCommInMsg);
        nLastHeading = BytesToInt(puszCommInMsg, ucCompassMSB, ucCompassLSB);
        
        if ((nLastHeading >= (nHeading - COMPASS_THRESH)) && (nLastHeading <= (nHeading + COMPASS_THRESH)))
        {
            break;
        }

    }
    while ( nLastHeading != nHeading);

    //-------------------------------------------------------------------------
    //  Stop moving.
    //-------------------------------------------------------------------------
    StopMoving(puszCommOutMsg, puszCommInMsg);
}

/**
 * Function to get the values from the compass.
 * @param puszCommOutMsg buffer to store the message to write.
 * @param puszCommInMsg buffer to store the received message from querey.
 */
void GetCompassVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildSensGet(puszCommOutMsg, ucCompassSel);
    serialPort.WritePort(puszCommOutMsg, ucGetSensorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}