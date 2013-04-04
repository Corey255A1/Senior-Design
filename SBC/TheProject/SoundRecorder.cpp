/* 
 * File:   SoundRecorder.cpp
 * @author Design Team 12
 * 
 * Created on March 17, 2013, 11:08 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "SoundRecorder.h"
#include <portaudio.h>
#include "LogFile.h"

/**
 * Empty constructor for the SoundRecorder class.
 */
SoundRecorder::SoundRecorder()
{
    //-------------------------------------------------------------------------
    //  Initialization variables.
    //-------------------------------------------------------------------------
    totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(float);
    Pa_Initialize();
    
    //-------------------------------------------------------------------------
    //  Setup the input parameters structure for the audio stream.
    //
    //          device          - Device index specifying the device to be used
    //                            for the streaming.
    //          channelCount    - Number of channels of sound to be accessed
    //                            by the stream.
    //          sampleFormat    - Sample format (i.e, data type) to be used
    //                            by the stream.
    //          suggestedLatency- Desired latency.
    //
    //          hostApiSpecificStreamInfo - Data structure containing any
    //                                      additional setup information.
    //          
    //-------------------------------------------------------------------------
    
    int devcount = Pa_GetDeviceCount();
    printf("Dev Count: %d\n",devcount);
    inputParameters.device = Pa_GetDefaultInputDevice();
    //inputParameters.device = 11;
    if (inputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default input device.\n");
    }
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
  
}

/**
 * Non-Empty constructor for the SoundRecorder class.
 * @param orig
 */
SoundRecorder::SoundRecorder(const SoundRecorder& orig)
{
    
}

/**
 * Destructor for the sound recorder class.
 */
SoundRecorder::~SoundRecorder()
{
    Pa_Terminate();
}

/**
 * Function used to record sound for a given period of time.
 * @param rSamples - Buffer to store all of the sound samples.
 */
void SoundRecorder::record(float rSamples[NUM_SECONDS * SAMPLE_RATE])
{
    WriteToLogFile("Recording\n");
    
    //-------------------------------------------------------------------------
    //  Here we open an audio stream with the following characteristics:
    //-------------------------------------------------------------------------
    Pa_OpenStream
    (
            &stream,          // Address of the paStream pointer that will
                              // will receive a pointer to the newly opened
                              // stream.
            &inputParameters, // Data structure to customize the input
                              // parameters for the data stream.
            NULL,             // Data structure to customize the output
                              // parameters for the data stream.
            SAMPLE_RATE,      // Sample rate for input/output stream
            FRAMES_PER_BUFFER,// Prefered block granularity for stream
            paClipOff,        // Specific streaming characterstics
            NULL,             // Pointer to callback function to process and
                              // and filling input and output buffers.
            NULL              // Pointer to be passed to the callback function.
    );
    
    //-------------------------------------------------------------------------
    //  - Start the streaming process
    //  - Read the stream
    //  - Close the stream
    //-------------------------------------------------------------------------
    Pa_StartStream( stream );
    Pa_ReadStream( stream, rSamples, totalFrames );
    Pa_CloseStream( stream );
}
