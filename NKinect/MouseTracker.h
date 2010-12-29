#pragma once
#include <windows.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "BaseKinect.h"
#include "MouseCoordinatesEventArgs.h"

using namespace System;
using namespace AForge;
using namespace System::Drawing;
using namespace AForge::Imaging;
using namespace AForge::Imaging::Filters;
using namespace AForge::Math::Geometry;

namespace NKinect {
	public ref class MouseTracker {
		private:
			property Point^ LastCoordinates;
			property int SameCoordinateCount;

			void ThresholdImageUpdated(Object^ sender, CameraImageEventArgs^ e) {
				if (!Enabled)
					return;

				Grayscale^ gray = gcnew Grayscale(0.2125, 0.7154, 0.0721);
				Bitmap^ grayImage = gray->Apply(e->CameraImage);

				BlobCounter^ bc = gcnew BlobCounter();
				BlobsFiltering^ filter = gcnew BlobsFiltering();
				GrahamConvexHull^ hull = gcnew GrahamConvexHull();

				bc->FilterBlobs = true;
				bc->MinWidth = 5;
				bc->MinHeight = 5;
				bc->ObjectsOrder = ObjectsOrder::Size;

				filter->CoupledSizeFiltering = true;
				filter->MinWidth = 70;
				filter->MinHeight = 70;

				filter->ApplyInPlace(grayImage);
				bc->ProcessImage(grayImage);

				array<Blob^>^ blobs = bc->GetObjectsInformation();

				if (blobs->Length != 1) {
					ResetCounter();

					return;
				}

				Point^ newCoords = GetCoordinates(blobs[0]->CenterOfGravity);

				MovementDetected(this, gcnew MouseCoordinatesEventArgs(newCoords));
				mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, newCoords->X * 65535 / ScreenSize->Width, newCoords->Y * 65535 / ScreenSize->Height, 0, 0);

				if (System::Math::Abs(newCoords->X - LastCoordinates->X) < 10 && System::Math::Abs(newCoords->Y - LastCoordinates->Y) < 10)
					SameCoordinateCount++;

				LastCoordinates = newCoords;

				if (SameCoordinateCount > 50) {
					ClickDetected(this, gcnew MouseCoordinatesEventArgs(newCoords));
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

					ResetCounter();
				}
			}

			void ResetCounter() {
				LastCoordinates = gcnew Point();
				SameCoordinateCount = 0;
			}

			Point^ GetCoordinates(IntPoint^ pnt) {
				return gcnew Point(pnt->X * ScreenSize->Height / 640, pnt->Y * ScreenSize->Width / 480);
			}
		public:
			event EventHandler<MouseCoordinatesEventArgs^>^ MovementDetected;
			event EventHandler<MouseCoordinatesEventArgs^>^ ClickDetected;

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