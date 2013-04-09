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
void GetSensorVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void GetMotorVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
void StopMoving(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg);
SerialComm serialPort;

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
    
    ColorTracking colorTracker;
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
                
                //state= WAIT_FOR_TONE;
                state = HARD_CODE;
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
                int coreySucksPenis;
                int danIsASissy;
                int mikeSmellsBad;
                int nickIsADumbShit;
                int nReturnHeading;
                double compHeading;
                double updateHeading;
                int freakingCount = 0;
                int shit;
                int fuck;
                

                //MoveForward(uszCommOutMsg, uszCommInMsg, ucSpeed4, ucSpeed4, 100);
                
                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucForward, ucSpeed4, 100);
                WR_SET_MOTOR();
                
                do
                {
                    BuildSensGet(uszCommOutMsg, ucUltSel);
                    WR_GET_SENS();
                    
                    BuildMotorGet(uszCommOutMsg);
                    WR_GET_MOTOR();
                    

                    shit = BytesToLong(uszCommInMsg, ucLeftWheelMSB1, ucLeftWheelMSB2, ucLeftWheelLSB1, ucLeftWheelLSB2);
                    fuck = BytesToLong(uszCommInMsg, ucRightWheelMSB1, ucRightWheelMSB2, ucRightWheelLSB1, ucRightWheelLSB2);
                    
                }
                while ((shit < ((double)100 / CM_TO_PULSES)) && (fuck < (double) 100 / CM_TO_PULSES));
                
                MOTOR_STOP();
                
                
                
                // Spinning 90 degrees
                BuildSensGet(uszCommOutMsg, ucCompassSel);
                WR_GET_SENS();
                nReturnHeading = BytesToInt(uszCommInMsg, ucCompassMSB, ucCompassLSB);
                compHeading = (((double) nReturnHeading)  / COMPASS_DIVISOR);                
                updateHeading = compHeading + (M_PI / 2);
                SPIN_BOT_CCLK();
                
                if (updateHeading > (2*M_PI))
                {
                    updateHeading -= 2*M_PI;
                }
                
                do
                {
                    BuildSensGet(uszCommOutMsg, ucCompassSel);
                    WR_GET_SENS();

                    nReturnHeading = BytesToInt(uszCommInMsg, ucCompassMSB, ucCompassLSB);
                    compHeading = (((double) nReturnHeading)  / COMPASS_DIVISOR);
                    
                    BuildMotorGet(uszCommOutMsg);
                    WR_GET_MOTOR();
//                    shit = BytesToLong(uszCommInMsg, ucLeftWheelMSB1, ucLeftWheelMSB2, ucLeftWheelLSB1, ucLeftWheelLSB2);
//                    fuck = BytesToLong(uszCommInMsg, ucRightWheelMSB1, ucRightWheelMSB2, ucRightWheelLSB1, ucRightWheelLSB2); 
                    
//                    if ((shit >= ((double)95 / CM_TO_PULSES)) || (fuck >= (double) 95 / CM_TO_PULSES))
//                    {
//                        SPIN_BOT_CCLK();
//                    }
                    
//                    BuildSensGet(uszCommOutMsg, ucUltSel);
//                    WR_GET_SENS();
//                    
//                    coreySucksPenis = BytesToInt(uszCommInMsg, ucUltLeftFrontMSB, ucUltLeftFrontLSB);
//                    danIsASissy = BytesToInt(uszCommInMsg, ucUltLeftBackMSB, ucUltLeftBackLSB);
//                    mikeSmellsBad = BytesToInt(uszCommInMsg, ucUltRightFrontMSB, ucUltRightFrontLSB);
//                    nickIsADumbShit = BytesToInt(uszCommInMsg, ucUltRightBackMSB, ucUltRightBackLSB);
//                    nFrontUltDist = BytesToInt(uszCommInMsg, ucUltFrontMSB, ucUltFrontLSB);
//                    nBackUltDist = BytesToInt(uszCommInMsg, ucUltBackMSB, ucUltBackLSB);
                    
                    
                    
//                    if (nFrontUltDist < 10)
//                    {
//                        MOTOR_STOP();
//                        break;
//                    }
                    
                    
//                    
//                    BuildMotorGet(uszCommOutMsg);
//                    WR_GET_MOTOR();
////                    
////
//                    shit = BytesToLong(uszCommInMsg, ucLeftWheelMSB1, ucLeftWheelMSB2, ucLeftWheelLSB1, ucLeftWheelLSB2);
//                    fuck = BytesToLong(uszCommInMsg, ucRightWheelMSB1, ucRightWheelMSB2, ucRightWheelLSB1, ucRightWheelLSB2);
                    
                }
                //while ((shit < ((double)43 / CM_TO_PULSES)) && (fuck < (double) 43 / CM_TO_PULSES));
                while (!theMap.checkCompassHeading(compHeading, updateHeading));
                MOTOR_STOP();
                //state = WAIT_FOR_TONE;
                //state = SCAN_FOR_POS;
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
            {
                int nRightFrontDist;
                int nRightBackDist;

                //-----------------------------------------------------------------
                //  Build ultrasonic messages.
                //-----------------------------------------------------------------
                BuildSensGet(uszCommOutMsg, ucUltSel);
                WR_GET_SENS();

                //-----------------------------------------------------------------
                //  Find Ultrasonic distances to wall.
                //-----------------------------------------------------------------
                nRightFrontDist = BytesToInt(uszCommInMsg, ucUltRightFrontMSB, ucUltRightFrontLSB);
                nRightBackDist = BytesToInt(uszCommInMsg, ucUltRightBackMSB, ucUltRightBackLSB);

                //-----------------------------------------------------------------
                //  If the front right ultrasonic is farther away we turn right...
                //-----------------------------------------------------------------
                if (nRightFrontDist > nRightBackDist)
                {
//                    BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed4, ucForward, ucSpeed4);
                }

                //-----------------------------------------------------------------
                //  ... Else if the front right ultrasonic is close turn left
                //-----------------------------------------------------------------
                else if (nRightFrontDist < nRightBackDist)
                {
                //    BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucReverse, ucSpeed4);
                }

                //-----------------------------------------------------------------
                //  ... Else we good.
                //-----------------------------------------------------------------
                else
                {
                    double heading;

                    //-------------------------------------------------------------
                    //  Get the distance from the back of the robot to the wall.
                    //-------------------------------------------------------------
                    int nBackDist = BytesToInt(uszCommInMsg, ucUltBackMSB, ucUltBackLSB);

                    //-------------------------------------------------------------
                    //  Get the compass value for our heading offset.
                    //-------------------------------------------------------------
                    BuildSensGet(uszCommOutMsg, ucCompassSel);
                    WR_GET_SENS();
                    heading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
                    theMap.orientNorth(heading / ((double) COMPASS_DIVISOR));
                    theMap.setHeading(heading / ((double) COMPASS_DIVISOR));

                    //-------------------------------------------------------------
                    //  Update the map with the robot's current position.
                    //-------------------------------------------------------------
                    roboCoord.x = nBackDist + CENTER_OFFSET;
                    roboCoord.y = nRightFrontDist + CENTER_OFFSET;
                    theMap.setLocation(roboCoord);

                    //-------------------------------------------------------------
                    //  Go to the next state (Find fridge trigger)
                    //-------------------------------------------------------------
                    theMap.destPt1.x = (int) theMap.getX();
                    dstPt = DST_PT1;
                    state = FIND_FRIDGE_TRIGGER;
                    break;
                }
                
                //-------------------------------------------------------------
                //  Write the serial port with the set motor command built 
                //  from above
                //-------------------------------------------------------------
                WR_SET_MOTOR();

                //-------------------------------------------------------------
                //  Check to make sure if we received a return ACK, if we did
                //  not then just send it again...
                //-------------------------------------------------------------
                if (!CheckAck(uszCommInMsg))
                {
                    MOTOR_STOP();
                    break;
                }
                
                //-------------------------------------------------------------
                //  ... Else continue and monitor the stepper count of the turn
                //-------------------------------------------------------------
                else
                {
                    //---------------------------------------------------------
                    //  While the the robot has not spinned enough. Keep
                    //  writing set commands.
                    //---------------------------------------------------------
//                    BuildMotorGet(uszCommOutMsg);
//                    do 
//                    {
//                        WR_GET_MOTOR();
//                    } 
//                    while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < 100);
//
//                    //---------------------------------------------------------
                    //  Once we move enough, stop the robot.
                    //---------------------------------------------------------
                    MOTOR_STOP();

                    //---------------------------------------------------------
                    //  Check to make sure the stop command was received.
                    //---------------------------------------------------------
                    if (!CheckAck(uszCommInMsg))
                    {
                        //-----------------------------------------------------
                        //  Send stop again
                        //-----------------------------------------------------
                        MOTOR_STOP();
                        break;
                    }
                }
                
                break;
            }
                
            case FIND_FRIDGE_TRIGGER:
            {
                double newHeading;
                double curHeading;
                char inRange = FALSE;
                
                //-------------------------------------------------------------
                //  We have set locations on the path to get to the door
                //  trigger, so go to each one sequentually
                //-------------------------------------------------------------
                switch (dstPt)
                {
                    //---------------------------------------------------------
                    //  Go to destination 1
                    //---------------------------------------------------------
                    case DST_PT1:
                        
                        //-----------------------------------------------------
                        //  Determine heading.
                        //-----------------------------------------------------
                        newHeading = theMap.determineHeading(theMap.destPt1);

                        //-----------------------------------------------------
                        //  While the compass heading is not matched with our
                        //  desired heading at destination point 1, we want to
                        //  keep spinning the robot.
                        //-----------------------------------------------------
//                        BuildSensGet(uszCommOutMsg, ucCompassSel);
//                        do 
//                        {
//                            WR_GET_SENS();
//                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
//                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
//                            
//                        }
//                        while (!theMap.checkCompassHeading(curHeading));
//                        MOTOR_STOP();
                        
                        //-----------------------------------------------------
                        //  Need to go forward to finally move towards the
                        //  destination point 1.
                        //-----------------------------------------------------
                        //BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
                        WR_SET_MOTOR();
                        
                        //-----------------------------------------------------
                        //  As we move forward to get to the destination, we
                        //  do the following...
                        //-----------------------------------------------------
//                        do
//                        {
//                            //  See if we are running into anything
////                            BuildSensGet(uszCommOutMsg, ucUltSel);
////                            WR_GET_SENS();
////                            
////                            //-------------------------------------------------
////                            //  If we are going to run into anything then stop.
////                            //-------------------------------------------------
////                            if (BytesToInt(uszCommInMsg, ucUltFrontMSB, ucUltFrontLSB) < ULT_STOP_THRESH)
////                            {
////                                MOTOR_STOP();
////                                
////                                //---------------------------------------------
////                                //  Check to make sure if we received a return 
////                                //  ACK, if we did not then just send it again.
////                                //---------------------------------------------
////                                if (!CheckAck(uszCommInMsg))
////                                {
////                                    MOTOR_STOP();
////                                    break;
////                                }
////                            }
//                            
//                            
//                            //  Make sure we keep traveling strait.
//                            
//                            BuildSensGet(uszCommOutMsg, ucCompassSel);
//                            WR_GET_SENS();
//                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
//                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
//                            int nAdj = theMap.checkHeadingDeviation(curHeading);
//                           
//                            //  Adjust right, left, or keep going strait
//                            if (nAdj == ADJ_RIGHT)
//                            {
//                                  BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed6, ucForward, ucSpeed5);
//                                  WR_SET_MOTOR();
//                            }
//                            else if (nAdj == ADJ_LEFT)
//                            {
//                                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed6);
//                                WR_SET_MOTOR();
//                            }
//                            else 
//                            {
//                                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//                                WR_SET_MOTOR();
//                            }
//                            
//                           
//                            
//                            
//                            //  Get the steps again
//                            BuildMotorGet(uszCommOutMsg);
//                            WR_GET_MOTOR();
//                            
//                            //-------------------------------------------------------------
//                            //  Check to make sure if we received a return ACK, if we did
//                            //  not then just send it again...
//                            //-------------------------------------------------------------
//                            if (!CheckAck(uszCommInMsg))
//                            {
//                                MOTOR_STOP();
//                                break;
//                            }
//                            
//                            
//                            
//                        }
//                        while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < theMap.getStepCount());
                        
                        MOTOR_STOP();
                        
                        //dstPt = DST_PT2;
                        break;
                        
                }
                
                state = DETECT_FRIDGE_IR;
                break;
            }
            case DETECT_FRIDGE_IR:
            { 
                break;
            }
            case OPEN_DOOR:
                
                // Measure distance and parallelism from bottom wall
                // Adjust as usual to be parallel (and get distance to bot wall)
                // Measure distance to fridge
                // Isolate location and update our map for precision
                // drive backwards to get to the bottom sensor position
                //      coordinate about (90, 62-63O
                // See if we detect floor LED.
                // Go to LINE_UP_WITH_FRIDGE
                break;

            case LINE_UP_WITH_FRIDGE:
                // This state is probably not needed, because we can do it in
                // OPEN_DOOR
                // go to GET_PLATE_FRIDGE
                state = GET_PLATE_FRIDGE;
                break;

            case GET_PLATE_FRIDGE:
                
                //  Go forward a certain distance.
                //  Adjust to maintain a steady path as usual
                //  Once a certain distance from the fridge stop 
                //  Extend arm
                //  Open the gripper.
                //  Go forward more
                //  Close the gripper.
                //  back up
                //  Pull back arm
                // go to CLOSE_DOOR
                break;

            case CLOSE_DOOR:
                // Back up back to switch spot again. (should be strait back)
                // Sense the LED on the floor.
                // Turn on LED on robot to show door is closed.
                // go to GO_TO_TABLE
                
                break;

            case GO_TO_TABLE:
                
                // go to PLACE_PLATE
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


void MoveForward(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg, unsigned char M1speed, unsigned char M2speed, unsigned char cDistance)
{
    long lLeftWheelCount;
    long lRightWheelCount;
    
    //  Build message to move forward, then execute command.
    BuildMotorSet(puszCommOutMsg, M1speed, ucForward, M2speed, ucForward, cDistance);
    serialPort.WritePort(puszCommOutMsg, ucSetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
         
    // Keep moving until one of the motors reaches the required count.
    do
    {
        //  Get updated sensor information.
        GetSensorVals(puszCommOutMsg, puszCommInMsg);

        //  Get updated motor information.
        GetMotorVals(puszCommOutMsg, puszCommInMsg);


        lLeftWheelCount = BytesToLong(puszCommInMsg, ucLeftWheelMSB1, ucLeftWheelMSB2, ucLeftWheelLSB1, ucLeftWheelLSB2);
        lRightWheelCount = BytesToLong(puszCommInMsg, ucRightWheelMSB1, ucRightWheelMSB2, ucRightWheelLSB1, ucRightWheelLSB2);

    }
    while ((lLeftWheelCount < ((double)cDistance / CM_TO_PULSES)) && (lRightWheelCount < (double) cDistance / CM_TO_PULSES));

    //  Stop moving.
    StopMoving(puszCommOutMsg, puszCommInMsg);
}

void GetSensorVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildSensGet(puszCommOutMsg, ucUltSel);
    serialPort.WritePort(puszCommOutMsg, ucGetSensorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

void GetMotorVals(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildMotorGet(puszCommOutMsg);
    serialPort.WritePort(puszCommOutMsg, ucGetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}

void StopMoving(unsigned char* puszCommOutMsg, unsigned char* puszCommInMsg)
{
    BuildMotorSet(puszCommOutMsg, ucForward, ucSpeed0, ucReverse, ucSpeed0, 0); 
    serialPort.WritePort(puszCommOutMsg, ucSetMotorPacketSize); 
    serialPort.ReadPort(puszCommInMsg);
}