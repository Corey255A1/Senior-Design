/* 
 * File:   SoundFFT.cpp
 * @author Design Team 12
 * 
 * Created on March 17, 2013, 11:50 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include "SoundFFT.h"
#include <fftw3.h>
#include "SoundRecorder.h"
#include "LogFile.h"

/**
 * Empty constructor for the SoundFFT class.
 */
SoundFFT::SoundFFT() 
{
    
}

/**
 * Non-empty constructor for the SoundFFT class.
 * @param orig
 */
SoundFFT::SoundFFT(const SoundFFT& orig)
{
    
}

/**
 * Destructor for the SoundFFT class.
 */
SoundFFT::~SoundFFT()
{
    
}

/**
 * Analyzes an array of sound samples to determine the dominate frequency.
 * @param sound - Buffer containing sound samples.
 * @return - Dominate frequency within the recording.
 */
double SoundFFT::getFreq(float sound[NUM_SECONDS * SAMPLE_RATE])
{
    //-------------------------------------------------------------------------
    //  Initialization Variables
    //-------------------------------------------------------------------------
    int N=NUM_SECONDS * SAMPLE_RATE;
    double binsize = ((double)SAMPLE_RATE/(double)N);
    double *inSound = (double *) malloc(sizeof(*inSound)*N);
    int i,maxbin=0;
    
    WriteToLogFile("Analyzing frequency sound sample frequency components.\n");
    
    //-------------------------------------------------------------------------
    //  Convert the input samples to doubles for compatibility
    //-------------------------------------------------------------------------
    for(i=0;i<N;i++){
        inSound[i] = (double) sound[i];
//        printf("\n%f\n",sound[i]);
    }
    
    //-------------------------------------------------------------------------
    //  
    //-------------------------------------------------------------------------
    fftw_complex *outFFT = (fftw_complex*) fftw_malloc(sizeof(*outFFT)*((N/2)+1));
    fftw_plan p = fftw_plan_dft_r2c_1d(N,inSound,outFFT,FFTW_ESTIMATE);
    fftw_execute(p);
    double themax = outFFT[0][0]*outFFT[0][0];
    for(i=0;i<N/2;i++){
        if(outFFT[i][0]*outFFT[i][0]>themax){
            themax=outFFT[i][0]*outFFT[i][0];
            maxbin=i;
        }
    }
    
    fftw_destroy_plan(p);
    fftw_free(outFFT);
    free(inSound);
    printf("%d",maxbin);
    return ((double)maxbin)*binsize;
}

