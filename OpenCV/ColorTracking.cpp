/* 
 * File:   ColorTracking.cpp
 * Author: Dan
 *
 * Adapted from https://github.com/liquidmetal/AI-Shack--Tracking-with-OpenCV/blob/master/TrackColour.cpp
 * 
 * Track colors with circles. Send signal out to say color was detected.
 * 
 * Created on February 5, 2013, 1:27 PM
 */

#include <stdlib.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>

IplImage* GetRedThresholdedImage(IplImage* img)
{
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);

    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);

    // Red on phone
    cvInRangeS(imgHSV, cvScalar(0, 175, 200), cvScalar(25, 225, 255), imgThreshed);

    cvReleaseImage(&imgHSV);

    return imgThreshed;
}

IplImage* GetBlueThresholdedImage(IplImage* img)
{
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);

    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);

    // Red on phone
    cvInRangeS(imgHSV, cvScalar(0, 175, 200), cvScalar(25, 225, 255), imgThreshed);
    
    // Blue on phone
    cvInRangeS(imgHSV, cvScalar(75, 225, 225), cvScalar(125 ,255, 255), imgThreshed);

    cvReleaseImage(&imgHSV);

    return imgThreshed;
}

void addObjectToVideo (IplImage *image, CvPoint pos)
{
    cvDrawCircle(image, pos, 15, CV_RGB(0, 255, 255), -1);
    
    cvShowImage("video", image);
}

int main(int argc, char** argv)
{
	// Initialize capturing live feed from the camera
	CvCapture* capture = 0;
	capture = cvCaptureFromCAM( CV_CAP_ANY );	

	// Couldn't get a device? Throw an error and quit
	if(!capture)
    {
        fprintf( stderr, "ERROR: Frame is NULL \n");
        getchar();
        return -1;
    }

    // The two windows we'll be using
    cvNamedWindow("video");
    cvNamedWindow("thresh");

	// An infinite loop
	while(true)
    {
		// Will hold a frame captured from the camera
		IplImage* frame = 0;
		frame = cvQueryFrame(capture);

		// If we couldn't grab a frame... quit
        if(!frame)
            break;

		// Holds the thresholded image (threshold = white, rest = black)
		IplImage* imgThresh = GetRedThresholdedImage(frame);
        IplImage* imgBlueThresh = GetBlueThresholdedImage(frame);
        
        cvAdd(imgThresh, imgBlueThresh, imgThresh);
        
		// Calculate the moments to estimate the position of the ball
		CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
		cvMoments(imgThresh, moments, 1);

		// The actual moment values
		double moment10 = cvGetSpatialMoment(moments, 1, 0);
		double moment01 = cvGetSpatialMoment(moments, 0, 1);
		double area = cvGetCentralMoment(moments, 0, 0);

		// Holding the last and current ball positions
		static int posX = 0;
		static int posY = 0;

		int lastX = posX;
		int lastY = posY;

		posX = moment10/area;
		posY = moment01/area;

		// Print it out for debugging purposes
		fprintf( stderr, "ERROR: Frame is NULL \n");

		// We want to draw a line only if its a valid position
		if(lastX>0 && lastY>0 && posX>0 && posY>0)
		{
            // First two will draw lines (one is an actually line, other is a line of circles)
            //cvLine(imgScribble, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,255,255), 5);
            //cvDrawCircle(imgScribble, cvPoint(posX, posY), 15, CV_RGB(0, 255, 255), -1);
            
            // Just add a circle to the frame when it finds the color
            addObjectToVideo(frame, cvPoint(posX, posY));
		}

		// Add the scribbling image and the frame... and we get a combination of the two
		//cvAdd(frame, imgScribble, frame);
		cvShowImage("thresh", imgThresh);
		cvShowImage("video", frame);

		// Wait for a keypress
		int c = cvWaitKey(10);
		if(c!=-1)
		{
			// If pressed, break out of the loop
            break;
		}

		// Release the thresholded image... we need no memory leaks.. please
		cvReleaseImage(&imgThresh);

        delete moments;
    }

	// We're done using the camera. Other applications can now use it
	cvReleaseCapture(&capture);
        
    return 0;
}

