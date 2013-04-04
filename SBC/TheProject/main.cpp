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

using namespace std;

#define NUM_THREADS     5

char cDebug = FALSE;

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
<<<<<<< HEAD
    enum STATE state = WAIT_FOR_TONE;
=======
    enum STATE state = INITIALIZE;
    enum DST_PT dstPt = DST_PT1;
>>>>>>> d47833d6d4dcdc31e8e37f2f05c568c0c210ded2
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
    //            sLogMsg = "Creating thread...\n";
    //            WriteToLogFile(sLogMsg);
                //nRC = pthread_create(&thread, NULL, ColorTrackingThread, (void *)t);

                //-----------------------------------------------------------------
                //  Check to make sure thread executed successfully.
                //-----------------------------------------------------------------
    //            if (nRC)
    //            {
    //                sLogMsg = "Thread Error: Bad return code on thread creation: " + IntToString(rc) + ". Exiting...\n";
    //                exit(-1);
    //            }

                state = WAIT_FOR_TONE;
                //state = SCAN_FOR_POS;
                break;

            case WAIT_FOR_TONE:

                //-----------------------------------------------------------------
                //  Sound Processing
                //-----------------------------------------------------------------
                soundRecorder.record(frgSoundSamps);
                dblFreqSamp = soundFFT.getFreq(frgSoundSamps);

                //-----------------------------------------------------------------
                //  If the higher frequency is heard, then RoboWaiter needs to get
                //  the plate on the lower shelf, and the state needs to advance
                //  to SCAN_FOR_POS...
                //-----------------------------------------------------------------
                if ((dblFreqSamp <= LOW_SHELF_CLEAN_FREQ+100) && (dblFreqSamp > LOW_SHELF_CLEAN_FREQ-100))
                {
                    bGetUpperShelf  = FALSE;
                    bGetLowShelf    = TRUE;
                    state = SCAN_FOR_POS;
                }

                //-----------------------------------------------------------------
                //  ... Else if the lower frequency is heard, then RoboWaiter needs 
                //  to get the plate on the lower shelf, and the state needs to 
                //  advance to SCAN_FOR_POS...
                //-----------------------------------------------------------------
                else if ((dblFreqSamp <= UPPER_SHELF_FREQ+100) && (dblFreqSamp > UPPER_SHELF_FREQ-100))
                {
                    bGetLowShelf    = FALSE;
                    bGetUpperShelf  = TRUE;
                    state = SCAN_FOR_POS;
                }

                //-----------------------------------------------------------------
                //  ... Else remain at the current state.
                //-----------------------------------------------------------------
                else
                {
                    state = state;
                }

                break;

            case SCAN_FOR_POS:
            {
                int nRightFrontDist;
                int nRightBackDist;

                //-----------------------------------------------------------------
                //  Build ultrasonic messages.
                //-----------------------------------------------------------------
                BuildSensGet(uszCommOutMsg, ucUltSel);
                WR_SERIAL();

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
                    WR_SERIAL();
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
                WR_SERIAL();

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
                    BuildMotorGet(uszCommOutMsg);
                    do 
                    {
                        WR_SERIAL();
                    } 
                    while (BytesToInt(uszCommInMsg, ucLeftWheelMSB, ucLeftWheelLSB) < 100);

                    //---------------------------------------------------------
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

                        if (theMap.spinDirection(newHeading) == CLKWISE)
                        {
                            SPIN_BOT_CLK();
                        }
                        else
                        {
                            SPIN_BOT_CCLK();
                        }

                        BuildSensGet(uszCommOutMsg, ucCompassSel);
                        do 
                        {
                            WR_SERIAL();
                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
                            
                        }
                        while (!theMap.checkCompassHeading(curHeading));
                        
                        MOTOR_STOP();
                        
                        dstPt = DST_PT2;
                        break;
                        
                    case DST_PT2:
                        
                        //-----------------------------------------------------
                        //  Determine heading.
                        //-----------------------------------------------------
                        newHeading = theMap.determineHeading(theMap.destPt2);
                        
                        if (theMap.spinDirection(newHeading) == CLKWISE)
                        {
                            SPIN_BOT_CLK();
                        }
                        else
                        {
                            SPIN_BOT_CCLK();
                        }

                        BuildSensGet(uszCommOutMsg, ucCompassSel);
                        do 
                        {
                            WR_SERIAL();
                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
                            
                        }
                        while (!theMap.checkCompassHeading(curHeading));
                        
                        MOTOR_STOP();
                        
                        dstPt = DST_PT3;
                        break;
                        
                    case DST_PT3:
                        
                        //-----------------------------------------------------
                        //  Determine heading.
                        //-----------------------------------------------------
                        newHeading = theMap.determineHeading(theMap.destPt3);
                        
                        if (theMap.spinDirection(newHeading) == CLKWISE)
                        {
                            SPIN_BOT_CLK();
                        }
                        else
                        {
                            SPIN_BOT_CCLK();
                        }

                        BuildSensGet(uszCommOutMsg, ucCompassSel);
                        do 
                        {
                            WR_SERIAL();
                            curHeading = ((int)(uszCommInMsg[ucCompassMSB] << 8)) | uszCommInMsg[ucCompassLSB];
                            curHeading = curHeading / ((double) COMPASS_DIVISOR);
                            
                        }
                        while (!theMap.checkCompassHeading(curHeading));
                        
                        MOTOR_STOP();
                        
                        dstPt = DST_PT3;
                        break;
                }
                
                state = DETECT_FRIDGE_IR;
                break;
            }
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

