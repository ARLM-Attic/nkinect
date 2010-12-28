#pragma once
#include "BaseKinect.h"
#include "MouseCoordinatesEventArgs.h"

using namespace System::Drawing;
using namespace AForge::Imaging;
using namespace AForge::Imaging::Filters;
using namespace AForge;
using namespace AForge::Math::Geometry;

namespace NKinect {
	public ref class MouseTracker {
		private:
			void ThresholdImageUpdated(Object^ sender, CameraImageEventArgs^ e) {
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

				if (blobs->Length != 1)
					return;

				if (Enabled)
					MovementDetected(this, gcnew MouseCoordinatesEventArgs(GetCoordinates(blobs[0]->CenterOfGravity)));
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