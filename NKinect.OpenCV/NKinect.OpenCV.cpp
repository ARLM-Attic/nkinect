// NKinect.OpenCV.cpp : Defines the entry point for the console application.

//

#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <windows.h>
#include "CLNUIDevice.h"

int _tmain(int argc, _TCHAR *argv[]) {
	CLNUIMotor motor = CreateNUIMotor();
	SetNUIMotorLED(motor, 0);

	CLNUICamera camera = CreateNUICamera();
	StartNUICamera(camera);

	PBYTE rawData = (PBYTE) malloc(640 * 480 * 3);
	GetNUICameraColorFrameRGB24(camera, rawData);

	IplImage *kinectDepthImage = cvCreateImage(cvSize(640, 480), 8, 3);
	cvSetData(kinectDepthImage, rawData, kinectDepthImage->widthStep);

	cvNamedWindow("NKinect OpenCV Tests", CV_WINDOW_AUTOSIZE);

	while (true) {
		GetNUICameraColorFrameRGB24(camera, rawData, 250);
		cvSetData(kinectDepthImage, rawData, kinectDepthImage->widthStep);
		cvShowImage("NKinect OpenCV Tests", kinectDepthImage);

		char c = cvWaitKey(26);

		if (c == 27)
			break;
	}

	cvDestroyWindow("NKinect OpenCV Tests");
	cvReleaseImageHeader(&kinectDepthImage);

	return 0;
}