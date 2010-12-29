// NKinect.OpenCV.cpp : main project file.

#include "stdafx.h"
#include <windows.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace System;

IplImage *g_image = NULL;

float cvDistancePoints(CvPoint p1, CvPoint p2) {
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;

	return sqrt((float) dx * dx + dy * dy);
}

void on_trackbar(int val) {
	IplImage     *g_gray    = NULL;
	IplImage     *c_image   = NULL;
	CvMemStorage *g_storage = NULL;

	if (g_storage == NULL) {
		g_gray  = cvCreateImage(cvGetSize(g_image), 8, 1);

		g_storage = cvCreateMemStorage(0);
	} else
		cvClearMemStorage(g_storage);

	CvSeq *contours;
	cvCvtColor(g_image, g_gray, CV_BGR2GRAY);
	c_image = cvCloneImage(g_image);

	cvThreshold(g_gray, g_gray, val, 255, CV_THRESH_BINARY);
	cvFindContours(g_gray, g_storage, &contours);
	cvZero(g_gray);


	if (contours)
		for (; contours != 0; contours = contours->h_next) {
			bool openHand = false;

			if ((contours->total >= 100) && (contours->total < 300)) {
				Console::WriteLine("Closed hand");

				openHand = false;
			} else if (contours->total >= 300) {
				Console::WriteLine("Open hand");

				openHand = true;
			} else
				continue;

			if (openHand) {
				CvMemStorage *store            = cvCreateMemStorage(0);
				CvSeq *hull                    = cvConvexHull2(contours, 0, CV_CLOCKWISE, 0);
				CvSeq *defects                 = cvConvexityDefects(contours, hull, store);
				CvConvexityDefect *defectArray = (CvConvexityDefect *) malloc(sizeof(CvConvexityDefect) * defects->total);
				CvMemStorage *minStorage	   = cvCreateMemStorage(0);

				CvBox2D box = cvMinAreaRect2(contours, minStorage);
				cvCvtSeqToArray(defects, defectArray);

				cvCircle(c_image, cvPoint(box.center.x, box.center.y), 3, CV_RGB(200, 0, 200), 2);
				cvEllipse(c_image, cvPoint(box.center.x, box.center.y), cvSize(box.size.height / 2, box.size.width/ 2), box.angle, 0, 360, CV_RGB(220, 0, 220), 1, 8, 0);

				for (int i = 0; i < defects->total; i++) {
					CvPoint startPoint	= *(defectArray[i].start);
					CvPoint depthPoint	= *(defectArray[i].depth_point);
					CvPoint endPoint	= *(defectArray[i].end);

					if (cvDistancePoints(startPoint, depthPoint) <= 10)
						continue;

					float dist = cvDistancePoints(startPoint, endPoint);
					Console::WriteLine(dist);

					cvCircle(c_image, cvPoint(startPoint.x, startPoint.y), 2, CV_RGB(0, 255, 0), 5);
					/*if (dist >= 38)
					else {
						if (endPoint.y >= startPoint.y)
							cvCircle(c_image, cvPoint(endPoint.x, endPoint.y), 2, CV_RGB(0, 255, 255), 5);
						else
							cvCircle(c_image, cvPoint(startPoint.x, startPoint.y), 2, CV_RGB(0, 255, 0), 5);
					}*/
				}
			}

			cvDrawContours(g_gray, contours, cvScalarAll(255), cvScalarAll(255), 100);
		}

		cvShowImage("Contours", g_gray);
		cvShowImage("Colour", c_image);
}

int main(array<System::String ^> ^ args) {
	g_image = cvLoadImage("C:\\TestBlobs3.png");
	cvNamedWindow("Original", 1);
	cvShowImage("Original", g_image);

	cvNamedWindow("Contours", 1);
	cvNamedWindow("Colour", 1);

	on_trackbar(0);
	cvWaitKey();

	cvDestroyAllWindows();

	return 0;
}