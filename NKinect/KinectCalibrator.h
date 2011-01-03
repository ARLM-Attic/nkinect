#include "BaseKinect.h"
#include "ImageCalibrator.h"

#pragma once
namespace NKinect {
	public ref class KinectCalibrator {
		private:
			property ImageCalibrator^ ColorImageCalibrator;
			property ImageCalibrator^ DepthImageCalibrator;

			void ImageUpdated(Object ^ sender, CameraImageEventArgs ^ e) {
				BitmapDataArray ^ newImage = gcnew BitmapDataArray(true);
				newImage->Reset();
				newImage->CopyCvImage(ColorImageCalibrator->GetChessboardImage(e));
				newImage->End();

				CalibratedImageUpdated(this, gcnew CameraImageEventArgs(newImage));
			}

		public:
			property BaseKinect ^ Kinect;
			event    EventHandler<CameraImageEventArgs ^> ^ CalibratedImageUpdated;

			KinectCalibrator(BaseKinect ^ kinect) {
				Kinect = kinect;

				ColorImageCalibrator = gcnew ImageCalibrator(7, 6, 20);
				DepthImageCalibrator = gcnew ImageCalibrator(7, 6, 20);

				Kinect->ImageUpdated += gcnew EventHandler<CameraImageEventArgs ^>(this, &KinectCalibrator::ImageUpdated);
			}

			!KinectCalibrator() {
				Stop();
			}

			~KinectCalibrator() {
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