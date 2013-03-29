/* 
 * File:   SoundFFT.h
 * @author Design Team 12
 *
 * Created on March 17, 2013, 11:50 PM
 * 
 * Header for the SoundFFT class.In the scope of the design, this class can be
 * used to process and anaylize a provided audio stream in order to identify
 * key frequency components.
 */

//-----------------------------------------------------------------------------
//  Include directories.
//-----------------------------------------------------------------------------
#ifndef SOUNDFFT_H
#define	SOUNDFFT_H
#include "SoundRecorder.h"

//-----------------------------------------------------------------------------
//  Class variable and function declearations.
//-----------------------------------------------------------------------------
class SoundFFT {
public:
    SoundFFT();
    SoundFFT(const SoundFFT& orig);
    virtual ~SoundFFT();
    double getFreq(float[NUM_SECONDS * SAMPLE_RATE]);
private:

};

#endif	/* SOUNDFFT_H */

