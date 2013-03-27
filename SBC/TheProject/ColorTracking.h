/* 
 * File:   ColorTracking.h
 * Author: robo-waiter
 *
 * Created on January 6, 2007, 2:21 PM
 */

#ifndef COLORTRACKING_H
#define	COLORTRACKING_H

#include <stdlib.h>
#include <stdio.h>
#include "opencv2/opencv.hpp"


class ColorTracking
{
    
public:
    IplImage* GetRedThresholdedImage(IplImage* img);
    IplImage* GetBlueThresholdedImage(IplImage* img);
    void addObjectToVideo (IplImage *image, CvPoint pos);
    ColorTracking();
    ~ColorTracking();
    int RunColorTracking();
};

#endif	/* COLORTRACKING_H */

