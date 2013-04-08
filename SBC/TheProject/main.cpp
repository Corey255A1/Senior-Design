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
    
    //-------------------------------------------------------------------------
    //  Function specific variables - Objects
    //-------------------------------------------------------------------------
    pthread_t thread;
    SerialComm serialPort;
    string sLogFilePath = "/home/robowaiter/Desktop/logfile2.txt";
    string sLogMsg;
    //enum STATE state = WAIT_FOR_TONE;
    enum DEMO_STATE state = INITIALIZE;
    enum DST_PT dstPt = DST_PT1;
    SoundRecorder soundRecorder;
    SoundFFT soundFFT;
    TheMap theMap;
    COORDINATES roboCoord;
    //roboCoord.x = 50;
    //roboCoord.y = 50;
    //theMap.setLocation(roboCoord);
    //enum STATE state = WAIT_FOR_TONE;
    
    
    
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

//                sLogMsg = "Creating thread...\n";
//                WriteToLogFile(sLogMsg);
//                nRC = pthread_create(&thread, NULL, ColorTrackingThread, (void *)t);

//                sLogMsg = "Creating thread...\n";
//                WriteToLogFile(sLogMsg);
//                nRC = pthread_create(&thread, NULL, ColorTrackingThread, (void *)t);

                //-----------------------------------------------------------------
                //  Check to make sure thread executed successfully.
                //-----------------------------------------------------------------

//                if (nRC)
//                {
//                    sLogMsg = "Thread Error: Bad return code on thread creation: " + IntToString(nRC) + ". Exiting...\n";
//                    exit(-1);
//                }
               

//                if (nRC)
//                {
//                    sLogMsg = "Thread Error: Bad return code on thread creation: " + IntToString(nRC) + ". Exiting...\n";
//                    exit(-1);
//                }

                MOTOR_STOP();
                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucForward, ucSpeed4);
                WR_SET_MOTOR();
                
                long shit;
                int coreySucksPenis;
                int danIsASissy;
                do
                {
                    BuildSensGet(uszCommOutMsg, ucUltSel);
                    WR_GET_SENS();
                    
                    coreySucksPenis = BytesToInt(uszCommInMsg, ucUltLeftFrontMSB, ucUltLeftFrontLSB);
                    danIsASissy = BytesToInt(uszCommInMsg, ucUltLeftBackMSB, ucUltLeftBackLSB);

                    
                    
                    
                    
                    
                    BuildMotorGet(uszCommOutMsg);
                    WR_GET_MOTOR();
                    
                    shit = BytesToLong(uszCommInMsg, ucRightWheelMSB1, ucRightWheelMSB2, ucRightWheelLSB1, ucRightWheelLSB2);
                    
                }
                while (shit < ((double)24.5 / CM_TO_PULSES));
                
                MOTOR_STOP();
                //state = WAIT_FOR_TONE;
                //state = SCAN_FOR_POS;
                MOTOR_STOP();
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
//                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//                WR_SET_MOTOR();
//                
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < 85);
//                
//                MOTOR_STOP(); 
//                state = HARD_END;
//                break;
//                
                
                
                //---------------------------------------------------------
                //  While the the robot has not spinned enough. Keep
                //  writing set commands.
                //---------------------------------------------------------
                // Spin left and go strait
//                SPIN_BOT_CCLK();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                } 
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double) 22.42 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                MOTOR_STOP();
//                
//                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//                WR_SET_MOTOR();
//                
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)90 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                MOTOR_STOP();
//                
//                
//                // Spin right and move forward
//                SPIN_BOT_CLK();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                } 
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)22.42 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                MOTOR_STOP();
//                
//                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//                WR_SET_MOTOR();
//                
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)200 / PULSES_TO_CM));
//                MOTOR_STOP();
//                 MOTOR_STOP();
//                
//                // Spin Right and move forward
//                SPIN_BOT_CLK();
//                
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)22.42 / PULSES_TO_CM));
//                
//                
//                MOTOR_STOP();
//                                MOTOR_STOP();
//                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//                WR_SET_MOTOR();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)88 / PULSES_TO_CM));
//                MOTOR_STOP();
//                
//                // Spin left and reverse?
//                SPIN_BOT_CCLK();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)22.42 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                               MOTOR_STOP();
//                BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed5, ucReverse, ucSpeed5);
//                WR_SET_MOTOR();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)35 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                               MOTOR_STOP(); 
//                // Set the Arm
//                if (bGetLowShelf)
//                {
//                    BuildArmSet(uszCommOutMsg, 0,0);
//                }
//                else 
//                {
//                    BuildArmSet(uszCommOutMsg, 0,1);
//                }
//                WR_SET_ARM();
//                
//                // Move forward to fridge
//                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed2, ucForward, ucSpeed2);
//                WR_SET_MOTOR();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)43 / PULSES_TO_CM));
//                MOTOR_STOP();
//                  MOTOR_STOP();              
//                // Backup to close fridge
//                BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed5, ucReverse, ucSpeed5);
//                WR_SET_MOTOR();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)43 / PULSES_TO_CM));
//                MOTOR_STOP();
//                        MOTOR_STOP();        
//                // Spin Left
//                SPIN_BOT_CCLK();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)22.42 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                 MOTOR_STOP();               
//                BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed5, ucReverse, ucSpeed5);
//                WR_SET_MOTOR();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)35 / PULSES_TO_CM));
//                MOTOR_STOP();
//                             MOTOR_STOP();   
//                // Spin left
//                SPIN_BOT_CCLK();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)22.42 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                               MOTOR_STOP(); 
//                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//                WR_SET_MOTOR();
//                BuildMotorGet(uszCommInMsg);
//                do
//                {
//                    WR_GET_MOTOR();
//                }
//                while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < ((double)22.42 / PULSES_TO_CM));
//                
//                MOTOR_STOP();
//                                MOTOR_STOP();
                state = HARD_END;
                break;
                
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
                    BuildMotorSet(uszCommOutMsg, ucReverse, ucSpeed4, ucForward, ucSpeed4);
                }

                //-----------------------------------------------------------------
                //  ... Else if the front right ultrasonic is close turn left
                //-----------------------------------------------------------------
                else if (nRightFrontDist < nRightBackDist)
                {
                    BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucReverse, ucSpeed4);
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

                        //  Determine spin direction.
                        if (theMap.spinDirection(newHeading) == CLKWISE)
                        {
                            SPIN_BOT_CLK();
                        }
                        else
                        {
                            SPIN_BOT_CCLK();
                        }

                        //-----------------------------------------------------
                        //  While the compass heading is not matched with our
                        //  desired heading at destination point 1, we want to
                        //  keep spinning the robot.
                        //-----------------------------------------------------
                        BuildSensGet(uszCommOutMsg, ucCompassSel);
                        do 
                        {
                            WR_GET_SENS();
                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
                            
                        }
                        while (!theMap.checkCompassHeading(curHeading));
                        MOTOR_STOP();
                        
                        //-----------------------------------------------------
                        //  Need to go forward to finally move towards the
                        //  destination point 1.
                        //-----------------------------------------------------
                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
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
                        
                    case DST_PT2:
                        
                        //-----------------------------------------------------
                        //  Determine heading.
                        //-----------------------------------------------------
                        newHeading = theMap.determineHeading(theMap.destPt2);
                        
                        //  Determine spin direction.
                        if (theMap.spinDirection(newHeading) == CLKWISE)
                        {
                            SPIN_BOT_CLK();
                        }
                        else
                        {
                            SPIN_BOT_CCLK();
                        }

                        //-----------------------------------------------------
                        //  While the compass heading is not matched with our
                        //  desired heading at destination point 1, we want to
                        //  keep spinning the robot.
                        //-----------------------------------------------------
                        BuildSensGet(uszCommOutMsg, ucCompassSel);
                        do 
                        {
                            WR_GET_SENS();
                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
                            
                        }
                        while (!theMap.checkCompassHeading(curHeading));
                        MOTOR_STOP();
                        
                        //-----------------------------------------------------
                        //  Need to go forward to finally move towards the
                        //  destination point 1.
                        //-----------------------------------------------------
                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);

                        WR_SET_MOTOR();

                        //

                        
                        //-----------------------------------------------------
                        //  As we move forward to get to the destination, we
                        //  do the following...
                        //-----------------------------------------------------
//                        do
//                        {
//                            //  See if we are running into anything
//                            BuildSensGet(uszCommOutMsg, ucUltSel);
//
//                            WR_GET_SENS();
//
//                            //
//
//                            
//                            //-------------------------------------------------
//                            //  If we are going to run into anything then stop.
//                            //-------------------------------------------------
//                            if (BytesToInt(uszCommInMsg, ucUltFrontMSB, ucUltFrontLSB) < ULT_STOP_THRESH)
//                            {
//                                MOTOR_STOP();
//                                
//                                //---------------------------------------------
//                                //  Check to make sure if we received a return 
//                                //  ACK, if we did not then just send it again.
//                                //---------------------------------------------
//                                if (!CheckAck(uszCommInMsg))
//                                {
//                                    MOTOR_STOP();
//                                    break;
//                                }
//                            }
//                            
//                            
//                            //  Make sure we keep traveling strait.
//                            
//                            BuildSensGet(uszCommOutMsg, ucCompassSel);
//
//                            WR_GET_SENS();
//
//                            //
//
//                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
//                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
//                            int nAdj = theMap.checkHeadingDeviation(curHeading);
//                           
//                            //  Adjust right, left, or keep going strait
//                            if (nAdj == ADJ_RIGHT)
//                            {
//                                  BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed6, ucForward, ucSpeed5);
//
//                                  WR_SET_MOTOR();
//
//                                  //
//
//                            }
//                            else if (nAdj == ADJ_LEFT)
//                            {
//                                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed6);
//
//                                WR_SET_MOTOR();
//
//                                //
//
//                            }
//                            else 
//                            {
//                                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//
//                                WR_SET_MOTOR();
//
//                                
//
//                            }
//                            
//                           
//                            
//                            
//                            //  Get the steps again
//                            BuildMotorGet(uszCommOutMsg);
//
//                            WR_GET_MOTOR();
//
//                            
//
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
                        
                        dstPt = DST_PT3;
                        break;
                        
                    case DST_PT3:
                        
                        //-----------------------------------------------------
                        //  Determine heading.
                        //-----------------------------------------------------
                        newHeading = theMap.determineHeading(theMap.destPt3);
                        
                        //  Determine spin direction.
                        if (theMap.spinDirection(newHeading) == CLKWISE)
                        {
                            SPIN_BOT_CLK();
                        }
                        else
                        {
                            SPIN_BOT_CCLK();
                        }

                        //-----------------------------------------------------
                        //  While the compass heading is not matched with our
                        //  desired heading at destination point 1, we want to
                        //  keep spinning the robot.
                        //-----------------------------------------------------
                        BuildSensGet(uszCommOutMsg, ucCompassSel);
                        do 
                        {
                            WR_GET_SENS();
                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
                            
                        }
                        while (!theMap.checkCompassHeading(curHeading));
                        MOTOR_STOP();
                        
                        //-----------------------------------------------------
                        //  Need to go forward to finally move towards the
                        //  destination point 1.
                        //-----------------------------------------------------
                        BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);

                        WR_SET_MOTOR();

                        //

                        
                        //-----------------------------------------------------
                        //  As we move forward to get to the destination, we
                        //  do the following...
                        //-----------------------------------------------------
//                        do
//                        {
//                            //  See if we are running into anything
//                            BuildSensGet(uszCommOutMsg, ucUltSel);
//
//                            WR_GET_SENS();
//
//                           // 
//
//                            
//                            //-------------------------------------------------
//                            //  If we are going to run into anything then stop.
//                            //-------------------------------------------------
//                            if (BytesToInt(uszCommInMsg, ucUltFrontMSB, ucUltFrontLSB) < ULT_STOP_THRESH)
//                            {
//                                MOTOR_STOP();
//                                
//                                //---------------------------------------------
//                                //  Check to make sure if we received a return 
//                                //  ACK, if we did not then just send it again.
//                                //---------------------------------------------
//                                if (!CheckAck(uszCommInMsg))
//                                {
//                                    MOTOR_STOP();
//                                    break;
//                                }
//                            }
//                            
//                            
//                            //  Make sure we keep traveling strait.
//                            
//                            BuildSensGet(uszCommOutMsg, ucCompassSel);
//
//                            WR_GET_SENS();
//
//                            
//
//                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
//                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
//                            int nAdj = theMap.checkHeadingDeviation(curHeading);
//                           
//                            //  Adjust right, left, or keep going strait
//                            if (nAdj == ADJ_RIGHT)
//                            {
//                                  BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed6, ucForward, ucSpeed5);
//
//                                  WR_SET_MOTOR();
//
//                                  
//
//                            }
//                            else if (nAdj == ADJ_LEFT)
//                            {
//                                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed6);
//
//                                WR_SET_MOTOR();
//
//                                
//
//                            }
//                            else 
//                            {
//                                BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed5, ucForward, ucSpeed5);
//
//                                WR_SET_MOTOR();
//
//                                
//
//                            }
//                            
//                           
//                            
//                            
//                            //  Get the steps again
//                            BuildMotorGet(uszCommOutMsg);
//
//                            WR_GET_MOTOR();
//
//                            
//
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
                        
                        dstPt = DST_PT2;
                        break;
                }
                
                state = DETECT_FRIDGE_IR;
                break;
            }
            case DETECT_FRIDGE_IR:
            {
                int nFrontDist;
                
                // Mesaure distance from bottom wall
                BuildSensGet(uszCommOutMsg, ucUltSel);

                WR_GET_SENS();

                


                //-----------------------------------------------------------------
                //  Find Ultrasonic distances to wall.
                //-----------------------------------------------------------------
                nFrontDist = BytesToInt(uszCommInMsg, ucUltFrontMSB, ucUltFrontLSB);
                
                
                // Spin 90 degrees CCW (or whatever amount to point map-north)
                // Detect the Fridge IR emitter.
                
                break;
            }
            case OPEN_DOOR:
                
                // Measure distance and parallelism from bottom wall
                // Adjust as usual to be parallel (and get distance to bot wall)
                // Measure distance to fridge
                // Isolate location and update our map for precision
                // drive backwards to get to the bottom sensor position
                // See if we detect floor LED.
                // Go to LINE_UP_WITH_FRIDGE
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
    //  Determine Location
    //-------------------------------------------------------------------------
    
    
    
    
    int tempCount = 0;
    float pi = 3.14;
    unsigned char piFixed = pi * 32;
    
    
    //BuildMotorSet(uszCommOutMsg, ucForward, ucSpeed4, ucReverse, ucSpeed4);
    //BuildMotorGet(uszCommOutMsg);
    BuildSensGet(uszCommOutMsg, ucCompassSel);
    //unsigned char testChar[5] = "!2GM";
//    BuildSensGet(uszCommOutMsg, ucTempSensSel);
//    BuildMotorGet(uszCommOutMsg);
//    BuildArmSet(uszCommOutMsg, 4, 6);
//    BuildArmGet(uszCommOutMsg);
    //uszCommOutMsg[0] = 'G';
    //uszCommOutMsg[1] = 'S';
    //commOutMsg[2] = piFixed;
    //uszCommOutMsg[2] = '!';
    //int tempSize = sizeof(uszCommOutMsg);
    //UnCharCat(uszCommOutMsg, uszCommOutMsg, tempSize);
    //int temp = strlen(reinterpret_cast <const char*>(commOutMsg));
    
    //commOutMsg[2] = piFixed;
    //-------------------------------------------------------------------------
    //  Main Working loop. Exit still has yet to be determined.
    //-------------------------------------------------------------------------
    while (1)
    {
            //serialPort.WritePort(uszCommOutMsg);
            
            //serialPort.ReadPort(uszCommInMsg);
            
            
            
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

