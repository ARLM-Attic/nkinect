#pragma once
using namespace System;
using namespace System::Drawing;

namespace NKinect {
	public ref class ImageCalibrator {
		private:
			IplImage*	colorImage;
			CvMat*		image_points;
			CvMat*		object_points;
			CvMat*		point_counts;
			CvMat*		intrinsic_matrix;
			CvMat*		distortion_coeffs;
			CvMat*		object_points2;
			CvMat*		image_points2;
			CvMat*		point_counts2;

			bool		saved;
			int			successes;

		public:
			property int BoardWidth;
			property int BoardHeight;
			property int NumBoards;
			property int BoardTiles;

			ImageCalibrator(int boardWidth, int boardHeight, int numBoards) {
				BoardWidth = boardWidth;
				BoardHeight = boardHeight;
				NumBoards = numBoards;
				BoardTiles = BoardWidth * BoardHeight;
			}

			IplImage* GetChessboardImage(CameraImageEventArgs^ e) {
				if (colorImage == nullptr) {
					colorImage   = cvCreateImage(cvSize(640, 480), 8, 3);
					image_points = cvCreateMat(NumBoards * BoardTiles, 2, CV_32FC1);

					object_points     = cvCreateMat(NumBoards * BoardTiles, 3, CV_32FC1);
					point_counts      = cvCreateMat(NumBoards, 1, CV_32SC1);
					intrinsic_matrix  = cvCreateMat(3, 3, CV_32FC1);
					distortion_coeffs = cvCreateMat(5, 1, CV_32FC1);

					object_points2 = cvCreateMat(NumBoards * BoardTiles, 3, CV_32FC1);
					image_points2  = cvCreateMat(NumBoards * BoardTiles, 2, CV_32FC1);
					point_counts2  = cvCreateMat(NumBoards, 1, CV_32SC1);
				}

				for (int i = 0; i < 640 * 480 * 3; i++)
					colorImage->imageData[i] = e->RawBytes[i];

				if (successes < NumBoards) {
					int    corner_count;
					CvSize board_sz       = cvSize(BoardWidth, BoardHeight);
					CvPoint2D32f *corners = new CvPoint2D32f[BoardTiles];

					int found = cvFindChessboardCorners(colorImage,
						board_sz,
						corners,
						&corner_count,
						CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

					cvDrawChessboardCorners(colorImage, board_sz, corners, corner_count, found);

					if (corner_count == BoardTiles) {
						for (int i = successes * BoardTiles, j = 0; j < BoardTiles; ++i, ++j) {
							CV_MAT_ELEM(*image_points, float, i, 0)  = corners[j].x;
							CV_MAT_ELEM(*image_points, float, i, 1)  = corners[j].y;
							CV_MAT_ELEM(*object_points, float, i, 0) = j / BoardWidth;
							CV_MAT_ELEM(*object_points, float, i, 1) = j % BoardWidth;
							CV_MAT_ELEM(*object_points, float, i, 2) = 0.0f;
						}

						CV_MAT_ELEM(*point_counts, int, successes, 0) = BoardTiles;
						successes++;
					}
				} else if ((successes == NumBoards) && !saved) {
					for (int i = 0; i < successes * BoardTiles; ++i) {
						CV_MAT_ELEM(*image_points2, float, i, 0)  = CV_MAT_ELEM(*image_points, float, i, 0);
						CV_MAT_ELEM(*image_points2, float, i, 1)  = CV_MAT_ELEM(*image_points, float, i, 1);
						CV_MAT_ELEM(*object_points2, float, i, 0) = CV_MAT_ELEM(*object_points, float, i, 0);
						CV_MAT_ELEM(*object_points2, float, i, 1) = CV_MAT_ELEM(*object_points, float, i, 1);
						CV_MAT_ELEM(*object_points2, float, i, 2) = CV_MAT_ELEM(*object_points, float, i, 2);
					}

					for (int i = 0; i < successes; ++i)
						CV_MAT_ELEM(*point_counts2, int, i, 0) = CV_MAT_ELEM(*point_counts, int, i, 0);

					// cvReleaseMat( &object_points );
					// cvReleaseMat( &image_points );
					// cvReleaseMat( &point_counts );

					CV_MAT_ELEM(*intrinsic_matrix, float, 0, 0) = 1.0;
					CV_MAT_ELEM(*intrinsic_matrix, float, 1, 1) = 1.0;

					cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(
						colorImage), intrinsic_matrix, distortion_coeffs, NULL, NULL, CV_CALIB_FIX_ASPECT_RATIO);
					cvSave("C:\\Intrinsics.xml", intrinsic_matrix);
					cvSave("C:\\Distortion.xml", distortion_coeffs);

					saved = true;
				} else if (saved) {
					IplImage *t = cvCloneImage(colorImage);
					IplImage *mapX = cvCreateImage(cvGetSize(colorImage), IPL_DEPTH_32F, 1);
					IplImage *mapY = cvCreateImage(cvGetSize(colorImage), IPL_DEPTH_32F, 1);

					cvInitUndistortMap(intrinsic_matrix, distortion_coeffs, mapX, mapY);
					cvRemap(t, colorImage, mapX, mapY);

					cvReleaseImage(&t);
					cvReleaseImage(&mapX);
					cvReleaseImage(&mapY);
				}

				return colorImage;
			}
	};
}
