#include "BaseKinect.h"

#pragma once
namespace NKinect {
	public ref class KinectCalibrator {
		private:
			IplImage* colorImage;
			IplImage* greyImage;

			void ImageUpdated(Object^ sender, CameraImageEventArgs^ e) {
				if (colorImage == nullptr) {
					colorImage	= cvCreateImage(cvSize(640, 480), 8, 3);
					greyImage	= cvCreateImage(cvGetSize(colorImage), 8, 1);
				} 

				for (int i = 0; i < 640 * 480 * 3; i++)
					colorImage->imageData[i] = e->RawBytes[i];

				cvCvtColor(colorImage, greyImage, CV_BGR2GRAY);

				int board_w  = 7; // Board width in squares
				int board_h  = 6; // Board height
				int	n_boards = 8; // Number of boards
				int        board_n  = board_w * board_h;
				CvSize     board_sz = cvSize(board_w, board_h);
				CvCapture *capture  = cvCreateCameraCapture(0);
				CvPoint2D32f *corners = new CvPoint2D32f[board_n];
				int corner_count;

				int found = cvFindChessboardCorners(colorImage, board_sz, corners, &corner_count, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

				cvDrawChessboardCorners(colorImage, board_sz, corners, corner_count, found);

				BitmapDataArray^ newImage = gcnew BitmapDataArray(true);
				newImage->Reset();
				newImage->CopyCvImage(colorImage);
				newImage->End();

				CalibratedImageUpdated(this, gcnew CameraImageEventArgs(newImage));
			}
		public:
			property BaseKinect^ Kinect;
			event EventHandler<CameraImageEventArgs^>^ CalibratedImageUpdated;

			KinectCalibrator(BaseKinect^ kinect) {
				Kinect = kinect;

				Kinect->ImageUpdated += gcnew EventHandler<CameraImageEventArgs^>(this, &KinectCalibrator::ImageUpdated);
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

