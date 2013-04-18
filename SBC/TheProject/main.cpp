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
void MoveBackwards(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char M1speed, unsigned char M2speed, unsigned char cDistance);
void GetUltSensVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void GetMotorVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void StopMoving(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void GetCompassVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void SpinRobot(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nSpinDegrees, int nSpin);
void GetGyroVal(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void SetShortArmPos(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nShortArmPos);
void SetLongArmPos(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nLongArmPos);
void SetClawPos(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char ucClawPos);

SerialComm serialPort;
ColorTracking colorTracker;

char cDebug = TRUE;

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
    HARD_END,
    DEMO_ULT,
    DEMO_SPEED,
    DEMO_COURSE,
    DEMO_COLOR,
    DEMO_BOX,
    DEMO_FOLLOW_COLOR,
    DEMO_STOP,
    DEMO_CLAW
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
    int nBoxCount = 0;
    
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
            {
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
                
                StopMoving(uszCommOutMsg, uszCommInMsg);
                //22118 - TOO LOW!!
                // 2.49 - Long Arm is good.
                //SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawClosedPos);
//                SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawOpenPos);
//                SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawClosedPos);
//                int nArmMovingPos;
//                double nRadianLong = 2.7;
//                double nRadianShort = 3.1;
//                
//                nArmMovingPos = (int) (nRadianLong * COMPASS_DIVISOR);
//                SetLongArmPos(uszCommOutMsg, uszCommInMsg, nArmMovingPos);
//                
//                nArmMovingPos = (int) (nRadianShort * COMPASS_DIVISOR);
//                SetShortArmPos(uszCommOutMsg, uszCommInMsg, nArmMovingPos);
                
                //int nlowTurn = (int) ((M_PI/8) * COMPASS_DIVISOR);
                
                // LINE UP SHIT
               
                
                 state= WAIT_FOR_TONE;
                //state = HARD_CODE;
                break;
            }
            case WAIT_FOR_TONE:

                //SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawOpenPos);
                //SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawClosedPos);
                
                //-----------------------------------------------------------------
                //  Sound Processing
                //-----------------------------------------------------------------
                soundRecorder.record(frgSoundSamps);
                dblFreqSamp = soundFFT.getFreq(frgSoundSamps);
                printf("Tone: %f\n", dblFreqSamp);
                
                if ((dblFreqSamp <= DEMO_SPEED_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_SPEED_FREQ - FREQ_THRESH))
                {
                    state = DEMO_SPEED;
                }
                else if ((dblFreqSamp <= DEMO_ULT_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_ULT_FREQ - FREQ_THRESH))
                {
                    state = DEMO_ULT;
                }
                else if ((dblFreqSamp <= DEMO_COURSE_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_COURSE_FREQ - FREQ_THRESH))
                {
                    state = DEMO_COURSE;
                }
                else if ((dblFreqSamp <= DEMO_COLOR_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_COLOR_FREQ - FREQ_THRESH))
                {
                    state = DEMO_COLOR;
                }
                else if ((dblFreqSamp <= DEMO_STOP_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_STOP_FREQ - FREQ_THRESH))
                {
                    state = DEMO_STOP;
                }
                else if ((dblFreqSamp <= DEMO_BOX_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_BOX_FREQ - FREQ_THRESH))
                {
                    state = DEMO_BOX;
                }
                else if ((dblFreqSamp <= DEMO_CLAW_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_CLAW_FREQ - FREQ_THRESH))
                {
                    state = DEMO_CLAW;
                }
                else if ((dblFreqSamp <= DEMO_FOLLOW_FREQ + FREQ_THRESH) && (dblFreqSamp >= DEMO_FOLLOW_FREQ - FREQ_THRESH))
                {
                    state = DEMO_FOLLOW_COLOR;
                }
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
//                BuildArmSet(uszCommOutMsg, RAdian * COMPASS_DIVISOR);
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
                
                int n90Turn = (int) ((M_PI/2) * COMPASS_DIVISOR);
                
                SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CLK);

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
                
            case DEMO_SPEED:
            {
                unsigned char speedDistance = 200;
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed10, ucSpeed10, speedDistance);
                
                state = WAIT_FOR_TONE;
                break;
            }   
            case DEMO_ULT:
            {
                long lLeftWheelCount;
                long lRightWheelCount;
                unsigned char cDistance = 200;
                int nFrontUltReading;
                int nLeftSideUlt;
                int bStopped = FALSE;
                
                //-------------------------------------------------------------------------
                //  Build message to move forward, then execute command.
                //-------------------------------------------------------------------------
                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucForward, ucSpeed4, cDistance, 0, 0);
                serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                serialPort.ReadPort(uszCommInMsg);
                
                //----------------------------------------------------------------
                // Keep moving until one of the motors reaches the required count.
                //----------------------------------------------------------------
                do
                {
                    //---------------------------------------------------------
                    //  Get ultrasonic stuff      
                    //---------------------------------------------------------
                    if (colorTracker.getBlueCount() > 0)
                    {
                        break;
                    }
                    
                    GetUltSensVals(uszCommOutMsg, uszCommInMsg);
                    //nFrontUltReading = BytesToInt(uszCommInMsg, ucUltFrontMSB, ucUltFrontLSB);
                    nLeftSideUlt = BytesToInt(uszCommInMsg, ucUltLeftFrontMSB, ucUltLeftFrontLSB);
//                    
                    if (nLeftSideUlt < 15)
                    {
                        StopMoving(uszCommOutMsg, uszCommInMsg);
                        
                    }
                    else
                    {
                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucForward, ucSpeed4, cDistance, 0, 0);
                        serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                        serialPort.ReadPort(uszCommInMsg);
                    }

                    
//                    if (nFrontUltReading < 15 && nFrontUltReading > 0)
//                    {
//                        int n90Turn = (int) ((M_PI/2) * COMPASS_DIVISOR);
//                        unsigned char ucNewDist = 50;
//                        StopMoving(uszCommOutMsg, uszCommInMsg);
//                        SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CCLK);
//                        MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, ucNewDist);
//                        SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CLK);
//                        MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, ucNewDist);
//                        SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CLK);
//                        MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, ucNewDist);
//                        SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CCLK);
//                        lLeftWheelCount = 0;
//                        lRightWheelCount = 0;
//                    }

                }
                while ((lLeftWheelCount < ((double)cDistance / CM_TO_PULSES)) && (lRightWheelCount < (double) cDistance / CM_TO_PULSES));

                //-------------------------------------------------------------
                //  Stop moving.
                //-------------------------------------------------------------
                StopMoving(uszCommOutMsg, uszCommInMsg);
                
                state = WAIT_FOR_TONE;
                break;
            }
            case DEMO_FOLLOW_COLOR:
            {
                unsigned char cAdjDistance = 5;
                while (colorTracker.getBlueCount() == 0)
                {
                    if (colorTracker.getRedTurn() == 1)        // left
                    {
                        BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed3, ucForward, ucSpeed3, cAdjDistance, 0, 0);
                        serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                        serialPort.ReadPort(uszCommInMsg);
                        //SpinRobot(uszCommOutMsg, uszCommInMsg, nlowTurn, SPIN_CCLK);
                    }
                    else if (colorTracker.getRedTurn() == 2)
                    {
                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed3, ucReverse, ucSpeed3, cAdjDistance, 0, 0);
                        serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                        serialPort.ReadPort(uszCommInMsg);
                        //SpinRobot(uszCommOutMsg, uszCommInMsg, nlowTurn, SPIN_CLK);
                    }
                    else
                    {
                        StopMoving(uszCommOutMsg, uszCommInMsg);
                    }
                    
                    if (colorTracker.getBlueCount() > 0)
                    {
                        break;
                    }
                }
                
                state = WAIT_FOR_TONE;
                break;
            }
            case DEMO_COURSE:
            {
                int n90Turn = (int) ((M_PI/2) * COMPASS_DIVISOR);
                int firstMove = 25;
                int secondMove = 158;
                int thirdMove = 122;
                int fourthMove = 92;
                unsigned char cAdjDistance = 5;
                unsigned char cReverseDist = 10;
                int nArmMovingPos;
                double dRadian = 2.7;
                
                // spin and move 1
                SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CCLK);
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, firstMove);
                
                // spin and Open Claw
                SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CLK);
                //SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawOpenPos);
                
                // Set arm position and move forward
                //nArmMovingPos = (int) (dRadian * COMPASS_DIVISOR);
                //SetLongArmPos(uszCommOutMsg, uszCommInMsg, nArmMovingPos);
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, secondMove);
                
                // Line up!!!
                while (colorTracker.getRedCount() > 0)
                {
                    if (colorTracker.getRedTurn() == 1)        // left
                    {
                        BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed3, ucForward, ucSpeed3, cAdjDistance, 0, 0);
                        serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                        serialPort.ReadPort(uszCommInMsg);
                        //SpinRobot(uszCommOutMsg, uszCommInMsg, nlowTurn, SPIN_CCLK);
                    }
                    else if (colorTracker.getRedTurn() == 2)
                    {
                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed3, ucReverse, ucSpeed3, cAdjDistance, 0, 0);
                        serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                        serialPort.ReadPort(uszCommInMsg);
                        //SpinRobot(uszCommOutMsg, uszCommInMsg, nlowTurn, SPIN_CLK);
                    }
                    else
                    {
                        StopMoving(uszCommOutMsg, uszCommInMsg);
                    }
                }
                
                // Move forward and grab claw
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, 10);
                //SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawClosedPos);
                
                // Reverse that stuff
                BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed4, ucReverse, ucSpeed4, 10, 0, 0);
                serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                serialPort.ReadPort(uszCommInMsg);
                
                // spin and move 3
                SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CCLK);
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, thirdMove);
                
                // spin and move 4
                SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CCLK);
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, fourthMove);
                
                SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawOpenPos);
                
                //state = HARD_END;
                state = WAIT_FOR_TONE;
                break;
            }
            case DEMO_COLOR:
            {
                long lLeftWheelCount;
                long lRightWheelCount;
                unsigned char cDistance = 200;
                int redCount = 0;
                int blueCount = 0;
                int bStopped = FALSE;

                //-------------------------------------------------------------------------
                //  Build message to move forward, then execute command.
                //-------------------------------------------------------------------------
                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucForward, ucSpeed4, cDistance, 0, 0);
                serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
                serialPort.ReadPort(uszCommInMsg);

                //----------------------------------------------------------------
                // Keep moving until one of the motors reaches the required count.
                //----------------------------------------------------------------
                do
                {
                    //---------------------------------------------------------
                    //  Get Color Count      
                    //---------------------------------------------------------
                    //redCount = colorTracker.getRedCount();
                    blueCount = colorTracker.getBlueCount();
                    printf("\nBlue Count: %d\n", blueCount);
                    
//                    if (redCount > 0)
//                    {
//                        StopMoving(uszCommOutMsg, uszCommInMsg);
//                        redCount = 0;
//                        bStopped = TRUE;
//                    }
                    
//                    if (blueCount > 0 && bStopped == TRUE)
//                    {
//                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucForward, ucSpeed4, cDistance, 0, 0);
//                        serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize); 
//                        serialPort.ReadPort(uszCommInMsg);
//                        blueCount = 0;
//                        bStopped = FALSE;
//                    }
                    
                    if (blueCount == 0 && bStopped == TRUE)
                    {
                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucForward, ucSpeed4, cDistance, 0, 0);
                        serialPort.WritePort(uszCommOutMsg, ucSetMotorPacketSize);
                        serialPort.ReadPort(uszCommInMsg);
                        bStopped = FALSE;
                    }
                    else if (blueCount > 0)
                    {
                        StopMoving(uszCommOutMsg, uszCommInMsg);
                        bStopped = TRUE;
                    }
                    
                    //---------------------------------------------------------
                    //  Get updated motor information.
                    //---------------------------------------------------------
                    GetMotorVals(uszCommOutMsg, uszCommInMsg);
                    lLeftWheelCount = BytesToLong(uszCommInMsg, ucLeftWheelMSB1, ucLeftWheelMSB2, ucLeftWheelLSB1, ucLeftWheelLSB2);
                    lRightWheelCount = BytesToLong(uszCommInMsg, ucRightWheelMSB1, ucRightWheelMSB2, ucRightWheelLSB1, ucRightWheelLSB2);

                }
                while ((lLeftWheelCount < ((double)cDistance / CM_TO_PULSES)) && (lRightWheelCount < (double) cDistance / CM_TO_PULSES));

                //-------------------------------------------------------------
                //  Stop moving.
                //-------------------------------------------------------------
                StopMoving(uszCommOutMsg, uszCommInMsg);
                
                state = WAIT_FOR_TONE;
                break;
            }
            
            case DEMO_BOX:
            {
                int n90Turn = (int) ((M_PI/2) * COMPASS_DIVISOR);
                unsigned char speedDistance = 100;
                
                MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, speedDistance);
                SpinRobot(uszCommOutMsg, uszCommInMsg, n90Turn, SPIN_CLK);
                
                if (nBoxCount >= 4)
                {
                        state = WAIT_FOR_TONE;
                        nBoxCount = 0;
                }
                nBoxCount++;
            }   
                break;
            case DEMO_STOP:
                
                MOTOR_STOP();
                MOTOR_STOP();
                break;
                
            case DEMO_CLAW:
                SetClawPos(uszCommOutMsg, uszCommInMsg, ucClawClosedPos);
                state = WAIT_FOR_TONE;
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
    long lLeftWheelCount = 0;
    long lRightWheelCount = 0;
    
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
    BuildMotorSet(puszCommOutMsg, ucReverse, ucSpeed0, ucReverse, ucSpeed0, 50, 0, 0); 
    serialPort.WritePort(puszCommOutMsg, ucSetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

/**
 * Spins the robot either clockwise or counter-clockwise
 * @param puszCommOutMsg buffer used to store the output message to write.
 * @param puszCommInMsg buffer used to store the received message.
 * @param nHeading the heading that we want to turn to.
 */
void SpinRobot(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nSpinDegrees, int nSpin)
{
    int nLastHeading;
    int nDegreesTurned;
    
    //-------------------------------------------------------------------------
    //  Build message to move forward, then execute command.
    //-------------------------------------------------------------------------
    if (nSpin == SPIN_CLK)
    {
        BuildMotorSet(puszCommOutMsg, ucForward, ucSpeed3, ucReverse, ucSpeed4, 100, 1, nSpinDegrees);
    }
    else if (nSpin == SPIN_CCLK)
    {
        BuildMotorSet(puszCommOutMsg, ucReverse, ucSpeed4, ucForward, ucSpeed3, 100, 1, nSpinDegrees);
    }
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
        GetGyroVal(puszCommOutMsg, puszCommInMsg);
        nDegreesTurned = BytesToInt(puszCommInMsg, GYRO_GET_MSB, GYRO_GET_LSB);
        
        if ((nDegreesTurned >= (nSpinDegrees - GYRO_THRESH)) && (nDegreesTurned <= (nSpinDegrees + GYRO_THRESH)))
        {
            break;
        }

    }
    while (nDegreesTurned != nSpinDegrees);

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

void GetGyroVal(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildSensGet(puszCommOutMsg, ucGyroSel);
    serialPort.WritePort(puszCommOutMsg, ucGetSensorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

void MoveForwardColor(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char M1speed, unsigned char M2speed, unsigned char cDistance)
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
        //---------------------------------------------------------------------
        //  Get Color Count      
        //---------------------------------------------------------------------
        
        //---------------------------------------------------------------------
        //  Get updated motor information.
        //---------------------------------------------------------------------
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

void SetClawPos(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char ucClawPos)
{
    BuildClawSet(puszCommOutMsg, ucClawPos);
    serialPort.WritePort(puszCommOutMsg, ucSetClawPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

void SetLongArmPos(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nLongArmPos)
{
    BuildLongArmSet(puszCommOutMsg, nLongArmPos);
    serialPort.WritePort(puszCommOutMsg, ucSetLongArmPacketSize);
    serialPort.ReadPort(puszCommInMsg);
}

void SetShortArmPos(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, int nShortArmPos)
{
    BuildLongArmSet(puszCommOutMsg, nShortArmPos);
    serialPort.WritePort(puszCommOutMsg, ucSetShortArmPacketSize);
    serialPort.ReadPort(puszCommInMsg);
}

void MoveBackwards(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char M1speed, unsigned char M2speed, unsigned char cDistance)
{
    long lLeftWheelCount = 0;
    long lRightWheelCount = 0;
    
    //-------------------------------------------------------------------------
    //  Build message to move forward, then execute command.
    //-------------------------------------------------------------------------
    BuildMotorSet(puszCommOutMsg, ucReverse, M1speed, ucReverse, M2speed, cDistance, 0, 0);
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