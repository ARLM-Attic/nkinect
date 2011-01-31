#pragma once
#include <windows.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "BaseKinect.h"
#include "MouseCoordinatesEventArgs.h"

using namespace System;
using namespace System::Drawing;

namespace NKinect {
	public ref class MouseTracker {
		private:
			property Point^ LastCoordinates;
			property int SameCoordinateCount;
			IplImage* colorImage;
			IplImage* greyImage;
			CvMemStorage* gStorage;

			float cvDistancePoints(CvPoint p1, CvPoint p2) {
				int dx = p1.x - p2.x;
				int dy = p1.y - p2.y;

				return sqrt((float) dx * dx + dy * dy);
			}

			void ThresholdImageUpdated(Object^ sender, CameraImageEventArgs^ e) {
				int				totalFound	= 0;
				int				fingers		= 0;
				int				val			= 0;
				CvSeq*			contours;

				if (colorImage == nullptr) {
					colorImage	= cvCreateImage(cvSize(640, 480), 8, 3);
					greyImage	= cvCreateImage(cvGetSize(colorImage), 8, 1);
					gStorage	= cvCreateMemStorage(0);
				} 

				for (int i = 0; i < 640 * 480 * 3; i++)
					colorImage->imageData[i] = e->RawBytes[i];

				cvSaveImage("C:\\TestCvImage.jpg", colorImage);
				e->CameraImage->Save("C:\\TestNETImage.jpg");

				cvCvtColor(colorImage, greyImage, CV_BGR2GRAY);

				cvThreshold(greyImage, greyImage, val, 255, CV_THRESH_BINARY);
				cvFindContours(greyImage, gStorage, &contours);
				cvSaveImage("c:\\testOutput.jpg", greyImage);
				cvZero(greyImage);

				if (!contours)
					return;

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
						totalFound++;
						CvMemStorage		*store		 = cvCreateMemStorage(0);
						CvSeq				*hull		 = cvConvexHull2(contours, 0, CV_CLOCKWISE, 0);
						CvSeq				*defects	 = cvConvexityDefects(contours, hull, store);
						CvConvexityDefect	*defectArray = (CvConvexityDefect *) malloc(sizeof(CvConvexityDefect) * defects->total);
						CvMemStorage		*minStorage	 = cvCreateMemStorage(0);

						CvBox2D box = cvMinAreaRect2(contours, minStorage);
						cvCvtSeqToArray(defects, defectArray);

						cvCircle(colorImage, cvPoint(box.center.x, box.center.y), 3, CV_RGB(200, 0, 200), 2);

						for (int i = 0; i < defects->total; i++) {
							CvPoint startPoint	= *(defectArray[i].start);
							CvPoint depthPoint	= *(defectArray[i].depth_point);
							CvPoint endPoint	= *(defectArray[i].end);

							if (cvDistancePoints(startPoint, depthPoint) <= 10)
								continue;

							cvCircle(colorImage, cvPoint(startPoint.x, startPoint.y), 2, CV_RGB(0, 255, 0), 5);
							fingers++;
						}
					}
				}

				if (totalFound != 1)
					return;

				Point^ newCoords = gcnew Point(); //GetCoordinates(blobs[0]->CenterOfGravity);

				MovementDetected(this, gcnew MouseCoordinatesEventArgs(newCoords));

				// if (Enabled)
				//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, newCoords->X * 65535 / ScreenSize->Width, newCoords->Y * 65535 / ScreenSize->Height, 0, 0);

				if (System::Math::Abs(newCoords->X - LastCoordinates->X) < 10 && System::Math::Abs(newCoords->Y - LastCoordinates->Y) < 10)
					SameCoordinateCount++;

				LastCoordinates = newCoords;

				if (SameCoordinateCount > 50) {
					ClickDetected(this, gcnew MouseCoordinatesEventArgs(newCoords));
					// if (Enabled)
					//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

					ResetCounter();
				}

				BitmapDataArray^ newImage = gcnew BitmapDataArray(true);
				newImage->Reset();
				newImage->CopyCvImage(colorImage);
				newImage->End();

				//cvReleaseImage(greyImage);
				//cvReleaseImage(&colorImage);

				MouseViewChanged(this, gcnew CameraImageEventArgs(newImage));
			}

			void ResetCounter() {
				LastCoordinates = gcnew Point();
				SameCoordinateCount = 0;
			}

			//Point^ GetCoordinates(IntPoint^ pnt) {
			//	return gcnew Point(pnt->X * ScreenSize->Height / 640, pnt->Y * ScreenSize->Width / 480);
			//}
		public:
			event EventHandler<MouseCoordinatesEventArgs^>^ MovementDetected;
			event EventHandler<MouseCoordinatesEventArgs^>^ ClickDetected;
			event EventHandler<CameraImageEventArgs^>^ MouseViewChanged;

			property System::Drawing::Rectangle^ ScreenSize;
			property BaseKinect^ Kinect;
			property bool Enabled;

			MouseTracker(BaseKinect^ kinect) {
				Kinect = kinect;

				Kinect->ThresholdDepthImageUpdated += gcnew EventHandler<CameraImageEventArgs^>(this, &MouseTracker::ThresholdImageUpdated);

				ScreenSize = System::Windows::Forms::Screen::PrimaryScreen->Bounds;
				LastCoordinates = gcnew Point();
			}

			~MouseTracker() {
				Stop();
			}

			!MouseTracker() {
				Stop();
			}

			void Start() {
				Kinect->Start();
			}

			void Stop() {
				Kinect->Stop();
			}
	};
}