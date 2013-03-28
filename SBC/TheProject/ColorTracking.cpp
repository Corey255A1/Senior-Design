/* 
 * File:   ColorTracking.cpp
 * Author: Dan
 *
 * 
 * Track colors with circles. Send signal out to say color was detected.
 * 
 * Created on February 5, 2013, 1:27 PM
 */
#include "opencv2/opencv.hpp"
#include "ColorTracking.h"

/**
 * Constructor for the ColorTracking class.
 */
ColorTracking::ColorTracking()
{
    
}

/**
 * Destructor for the ColorTracking class.
 */
ColorTracking::~ColorTracking()
{
    
}

/**
 * Takes in camera image, converts it to HSV value and finds the given
 * threshold (here it's red), and returns the filtered image that matches
 * the threshold value.
 * 
 * @param img
 * @return imgThreshed
 */
IplImage* ColorTracking::GetRedThresholdedImage(IplImage* img)
{
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);

    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);

    // Red LED - seen as white (need to fix)
    cvInRangeS(imgHSV, cvScalar(0, 0, 255), cvScalar(0, 0, 255), imgThreshed);

    cvReleaseImage(&imgHSV);

    return imgThreshed;
}

/**
 * Takes in camera image, converts it to HSV value and finds the given
 * threshold (here it's blue), and returns the filtered image that matches
 * the threshold value.
 * 
 * @param img
 * @return imgThreshed
 */
IplImage* ColorTracking::GetBlueThresholdedImage(IplImage* img)
{
    // Convert the image into an HSV image
    IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, imgHSV, CV_BGR2HSV);

    IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
    
    // Blue on phone
    cvInRangeS(imgHSV, cvScalar(100, 240, 240), cvScalar(140, 255, 255), imgThreshed);

    cvReleaseImage(&imgHSV);

    return imgThreshed;
}

/**
 * Adds marker to the image at the given CvPoint position.
 * 
 * @param image
 * @param pos
 */
void ColorTracking::addObjectToVideo (IplImage *image, CvPoint pos)
{
    cvDrawCircle(image, pos, 15, CV_RGB(0, 255, 255), -1);
}

/**
 *  Draw a line from the middle of frame to the given point
 *  @param Image to draw on
 *  @param Object to draw line to
 *  @param Middle of frame
 */
void drawWidthDiff (IplImage *image, CvPoint object, CvPoint middle)
{
    cvDrawLine(image, object, middle, CV_RGB(0, 255, 0), 2);
}

/**
 *  Draw a circle in the middle of found areas
 *  @param Image to draw on
 *  @param Image to get moments from
 */
void ColorTracking::DrawPoint(IplImage *frame, IplImage *thresh)
{
	// NONE OF THIS IS USED AT THE MOMENT - THIS IS FOR REFERENCE
    // Calculate the moments to estimate the position of the items
    CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
    cvMoments(thresh, moments, 1);

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

    // We want to draw a line only if its a valid position
    if(lastX>0 && lastY>0 && posX>0 && posY>0)
    {
        // Just add a circle to the frame when it finds the color
        addObjectToVideo(frame, cvPoint(posX, posY));

        // find x distance from middle to object
        drawWidthDiff(frame, cvPoint(posX, posY), cvPoint(frame->width / 2, frame->height /2));
    }
    
    delete moments;
}

/**
 * The "Main" of the class. This is the master function that will
 * continuously loop and scan/process images read in from the camera.
 */
int ColorTracking::RunColorTracking()
{
	// Initialize capturing live feed from the camera
	CvCapture* capture = 0;
	capture = cvCaptureFromCAM( CV_CAP_ANY );	
    
    //cvSetCaptureProperty(capture, CV_CAP_PROP_CONTRAST, 0);

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

        // Create two images and grab red and blue thresholds
		IplImage* imgRedThresh = GetRedThresholdedImage(frame);
        IplImage* imgBlueThresh = GetBlueThresholdedImage(frame);
        
        // turn the thresholded image into a binary image (white and black only)
        IplImage* imgRedBinary = cvCreateImage(cvGetSize(frame), 8, 1);
        cvThreshold(imgRedThresh, imgRedBinary, 10, 255, CV_THRESH_BINARY);
        
        // Setup for getting contours for red only
        CvSeq* redContours;
        CvMemStorage *redStorage = cvCreateMemStorage(0);
        
        // Smooth the image (prevent any edge issues)
        IplImage* imgRedSmooth = cvCreateImage(cvGetSize(frame), 8, 1);
        cvSmooth(imgRedBinary, imgRedSmooth, CV_MEDIAN, 7, 3, 0, 0);
        cvThreshold(imgRedSmooth, imgRedSmooth, 10, 255, CV_THRESH_BINARY);
        
        // gets amount of "white" space found - debugging only
        //int white_count = cvCountNonZero(imgRedThresh);
        
        // Find contours of smooth image, then display contours on regular image
        // Find contours returns the number of contours found
        int redCount = cvFindContours(imgRedSmooth, redStorage, &redContours);
        cvDrawContours(frame, redContours, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), 100, 5);
        
        IplImage* imgBlueBinary = cvCreateImage(cvGetSize(frame), 8, 1);
        cvThreshold(imgBlueThresh, imgBlueBinary, 10, 255, CV_THRESH_BINARY);
        
        // Setup for getting contours for red only
        CvSeq* blueContours;
        CvMemStorage *blueStorage = cvCreateMemStorage(0);
        
        // Smooth the image (prevent any edge issues)
        IplImage* imgBlueSmooth = cvCreateImage(cvGetSize(frame), 8, 1);
        cvSmooth(imgBlueBinary, imgBlueSmooth, CV_MEDIAN, 7, 3, 0, 0);
        cvThreshold(imgBlueSmooth, imgBlueSmooth, 10, 255, CV_THRESH_BINARY);
        
        int blueCount = cvFindContours(imgBlueSmooth, blueStorage, &blueContours);
        cvDrawContours(frame, blueContours, CV_RGB(0, 255, 0), CV_RGB(0, 255, 0), 100, 5);
//        CvMoments *Allmoments = (CvMoments*)malloc(sizeof(CvMoments));
//        
//        for (int i = 0; i < NC; ++i) {
//            Allmoments[i] = moments(contours[i], false);
//            
//            double moment10 = cvGetSpatialMoment(moments, 1, 0);
//            double moment01 = cvGetSpatialMoment(moments, 0, 1);
//            double area = cvGetCentralMoment(moments, 0, 0);
//            
//            int posX = 0;
//            int posY = 0;
//            
//            posX = moment10/area;
//            posY = moment01/area;
//            
//            if (posX>0 && posY>0)
//            {
//                addObjectToVideo(frame, cvPoint(posX, posY));
//            }
//            
//            delete moments;
//        }
//
//        delete moments;
        
        // Add the two thresholded images into one
        cvAdd(imgRedSmooth, imgBlueSmooth, imgRedSmooth); 
        
        // Choose the images we wish to show on the windows
        // This is for debugging only
		cvShowImage("thresh", imgRedSmooth);
		cvShowImage("video", frame);

		// Wait for a keypress
		int c = cvWaitKey(10);
        // if q or Q is pressed, quit
		if(c == 81 || c == 113)
		{
			// If pressed, break out of the loop
            break;
		}
        // if Space is pressed, tell us the number of contours
        else if (c == 32)
        {
            std::cout << "Red Contours found: " << redCount << std::endl;
            std::cout << "Blue Contours found: " << blueCount << std::endl;
        }

		// Release all images and release the memory storage for contours
        // this prevents memory leaks
		cvReleaseImage(&imgRedThresh);
        cvReleaseImage(&imgRedBinary);
        cvReleaseImage(&imgRedSmooth);
        cvReleaseMemStorage(&redStorage);
        cvReleaseImage(&imgBlueThresh);
        cvReleaseImage(&imgBlueBinary);
        cvReleaseImage(&imgBlueSmooth);
        cvReleaseMemStorage(&blueStorage);
    }
    
	// We're done using the camera. Other applications can now use it
	cvReleaseCapture(&capture);
        
    return 0;
}