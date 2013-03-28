/* 
 * File:   SoundRecorder.cpp
 * Author: corey
 * 
 * Created on March 17, 2013, 11:08 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "SoundRecorder.h"
#include <portaudio.h>


SoundRecorder::SoundRecorder() {
    totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(float);
    Pa_Initialize();
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default input device.\n");
    }
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

}

SoundRecorder::SoundRecorder(const SoundRecorder& orig) {
}

SoundRecorder::~SoundRecorder() {
    Pa_Terminate();
}
void SoundRecorder::record(float rSamples[NUM_SECONDS * SAMPLE_RATE]){
    printf("RECORDING\n");
    Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              NULL, /* no callback, use blocking API */
              NULL ); /* no callback, so no callback userData */
    Pa_StartStream( stream );
    Pa_ReadStream( stream, rSamples, totalFrames );
    Pa_CloseStream( stream );
}
