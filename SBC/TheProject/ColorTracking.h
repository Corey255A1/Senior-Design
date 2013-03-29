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
    void addObjectToVideo (IplImage *image, CvPoint pos);
    void drawWidthDiff (IplImage *image, CvPoint object, CvPoint middle);
    void DrawPoint(IplImage *frame, IplImage *thresh);
    ColorTracking();
    ~ColorTracking();
    int RunColorTracking();
};

#endif	/* COLORTRACKING_H */

