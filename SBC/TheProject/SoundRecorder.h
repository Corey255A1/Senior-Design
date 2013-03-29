/* 
 * File:   SoundRecorder.h
 * @author Design Team 12
 *
 * Created on March 17, 2013, 11:08 PM
 * 
 * Header for the SoundRecorder class. In the scope of the design, this class
 * can be used in order to handle recording external audio from the 
 * surrounding environment.
 */

#ifndef SOUNDRECORDER_H

//-----------------------------------------------------------------------------
//  Variable definitions.
//-----------------------------------------------------------------------------
#define	SOUNDRECORDER_H
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_SECONDS     (2)
#define NUM_CHANNELS    (1)
#define DITHER_FLAG     (0) /**/
#define PA_SAMPLE_TYPE  paFloat32
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"

//-----------------------------------------------------------------------------
//  Include directories
//-----------------------------------------------------------------------------
#include <portaudio.h>


//-----------------------------------------------------------------------------
//  Class variable and function declarations.
//-----------------------------------------------------------------------------
class SoundRecorder {
public:
    SoundRecorder();
    SoundRecorder(const SoundRecorder& orig);
    virtual ~SoundRecorder();
    void record(float[NUM_SECONDS * SAMPLE_RATE]);
private:
    PaStreamParameters inputParameters;
    PaStream *stream;
    float *recordedSamples;
    int i;
    int totalFrames;
    int numSamples;
    int numBytes;

};

#endif	/* SOUNDRECORDER_H */

