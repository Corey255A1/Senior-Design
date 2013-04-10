/* 
 * File:   ColorTracking.h
 * @author Design Team 12
 *
 * Created on January 6, 2007, 2:21 PM
 */

#ifndef COLORTRACKING_H

//-----------------------------------------------------------------------------
//  Include directories.
//-----------------------------------------------------------------------------

#define	COLORTRACKING_H
#include <stdlib.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"

//-----------------------------------------------------------------------------
//  Class variable and function declearations.
//-----------------------------------------------------------------------------
class ColorTracking
{
    
public:
    IplImage* GetRedThresholdedImage(IplImage* img);
    IplImage* GetBlueThresholdedImage(IplImage* img);
    void addObjectToVideo (IplImage *image, CvPoint pos, CvScalar color, int thickness);
    int drawWidthDiff (IplImage *image, CvPoint object, CvPoint middle);
    void DrawPoint(IplImage *frame, IplImage *thresh);
    int getRedCount(void);
    int getRedTurn(void);
    int getBlueCount(void);
    int getBlueTurn(void);
    ColorTracking();
    ~ColorTracking();
    int RunColorTracking(bool debug);
};

#endif	/* COLORTRACKING_H */
