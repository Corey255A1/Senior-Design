/* 
 * File:   SoundFFT.h
 * Author: corey
 *
 * Created on March 17, 2013, 11:50 PM
 */

#ifndef SOUNDFFT_H
#define	SOUNDFFT_H
#include "SoundRecorder.h"

class SoundFFT {
public:
    SoundFFT();
    SoundFFT(const SoundFFT& orig);
    virtual ~SoundFFT();
    double getFreq(float[NUM_SECONDS * SAMPLE_RATE]);
private:

};

#endif	/* SOUNDFFT_H */

