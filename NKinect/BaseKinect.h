#pragma once

#include <windows.h>
#include "LedColor.h"
#include "Library.h"
#include "Accelerometer.h"
#include "AccelerometerEventArgs.h"
#include "CameraImageEventArgs.h"
#include "DepthEventArgs.h"
#include "Unit.h"
#include "BitmapDataArray.h"

#define NKINECT_VERSION 0.1

using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace NKinect {
	public ref class BaseKinect abstract {
		protected:
			EventHandler<AccelerometerEventArgs^>^	accelerometerUpdated;
			EventHandler<CameraImageEventArgs^>^	imageUpdated;
			EventHandler<CameraImageEventArgs^>^	depthImageUpdated;
			EventHandler<CameraImageEventArgs^>^	thresholdDepthImageUpdated;
			EventHandler<CameraImageEventArgs^>^	thresholdColorImageUpdated;

			bool	accelerometerUpdateEnabled;
			bool	imageUpdateEnabled;
			bool	depthImageUpdateEnabled;
			bool	thresholdColorImageUpdateEnabled;
			bool	thresholdDepthImageUpdateEnabled;
		public:
			#pragma region Event definitions
			event EventHandler<DepthEventArgs^>^ DepthsCalculated;

			event EventHandler<AccelerometerEventArgs^>^ AccelerometerUpdated {
				void add(EventHandler<AccelerometerEventArgs^>^ e) {
					accelerometerUpdated += e;

					accelerometerUpdateEnabled = true;
				}

				void remove(EventHandler<AccelerometerEventArgs^>^ e) {
					accelerometerUpdated -= e;
				}

				void raise(Object^ obj, AccelerometerEventArgs^ e) {
					if (accelerometerUpdated != nullptr)
						accelerometerUpdated(obj, e);
				}
			}

			event EventHandler<CameraImageEventArgs^>^ ImageUpdated {
				void add(EventHandler<CameraImageEventArgs^>^ e) {
					imageUpdated += e;

					imageUpdateEnabled = true;
				}

				void remove(EventHandler<CameraImageEventArgs^>^ e) {
					imageUpdated -= e;
				}

				void raise(Object^ obj, CameraImageEventArgs^ e) {
					if (imageUpdated != nullptr)
						imageUpdated(obj, e);
				}
			}

			event EventHandler<CameraImageEventArgs^>^ DepthImageUpdated {
				void add(EventHandler<CameraImageEventArgs^>^ e) {
					depthImageUpdated += e;

					depthImageUpdateEnabled = true;
				}

				void remove(EventHandler<CameraImageEventArgs^>^ e) {
					depthImageUpdated -= e;
				}

				void raise(Object^ obj, CameraImageEventArgs^ e) {
					if (depthImageUpdated != nullptr)
						depthImageUpdated(obj, e);
				}
			}

			event EventHandler<CameraImageEventArgs^>^ ThresholdDepthImageUpdated {
				void add(EventHandler<CameraImageEventArgs^>^ e) {
					thresholdDepthImageUpdated += e;

					thresholdDepthImageUpdateEnabled = true;
				}

				void remove(EventHandler<CameraImageEventArgs^>^ e) {
					thresholdDepthImageUpdated -= e;
				}

				void raise(Object^ obj, CameraImageEventArgs^ e) {
					if (thresholdDepthImageUpdated != nullptr)
						thresholdDepthImageUpdated(obj, e);
				}
			}

			event EventHandler<CameraImageEventArgs^>^ ThresholdColorImageUpdated {
				void add(EventHandler<CameraImageEventArgs^>^ e) {
					thresholdColorImageUpdated += e;

					thresholdColorImageUpdateEnabled = true;
				}

				void remove(EventHandler<CameraImageEventArgs^>^ e) {
					thresholdColorImageUpdated -= e;
				}

				void raise(Object^ obj, CameraImageEventArgs^ e) {
					if (thresholdColorImageUpdated != nullptr)
						thresholdColorImageUpdated(obj, e);
				}
			}
			#pragma endregion

			property String^	MotorSerialNumber;
			property int		MinDistanceThreshold;
			property int		MaxDistanceThreshold;
			property Unit		DistanceUnit;
			property bool		Mirrored;

			BaseKinect() {
				MinDistanceThreshold = 0;
				MaxDistanceThreshold = 9999;

				DistanceUnit = Centimeters;
			}

			!BaseKinect() {
				Stop();
			}

			~BaseKinect() {
				Stop();
			}

		protected:
			double DisparityToDistance(short val) {
				return (double) (100.00 / (-0.00307 * val + 3.33));
			}

			int DisparityToGrayscale(short val) {
				return (2048 * 256) / (2048 - val);
			}

			virtual void FlipDepth()			 = 0;
			virtual void UpdateAccelerometer()	 = 0;
			virtual void DownloadImages()		 = 0;
		public:
			virtual void Start()				 = 0;
			virtual void Stop()					 = 0;
			virtual void SetLed(LedColor color)  = 0;
			virtual void SetPosition(short pos)  = 0;
			virtual void ExportPLY(String^ path) = 0;

			double GetPreferredUnit(double dist) {
				switch (DistanceUnit) {
					case Meters:		dist = dist * 0.01;
						break;

					case Inches:		dist = dist * 0.393700787;
						break;

					case Feet:			dist = dist * 0.032808399;
						break;
				}

				return dist;
			}
	};
}

