#pragma once

#include <windows.h>
#include "LedColor.h"
#include "Library.h"
#include "Accelerometer.h"
#include "AccelerometerEventArgs.h"
#include "CameraImageEventArgs.h"
#include "DepthEventArgs.h"
#include "Unit.h"

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
		public:
			event EventHandler<DepthEventArgs^>^			DepthsCalculated;
			event EventHandler<AccelerometerEventArgs^>^	AccelerometerUpdated;
			event EventHandler<CameraImageEventArgs^>^		ImageUpdated;
			event EventHandler<CameraImageEventArgs^>^		DepthImageUpdated;
			event EventHandler<CameraImageEventArgs^>^		ThresholdDepthImageUpdated;
			event EventHandler<CameraImageEventArgs^>^		ThresholdColorImageUpdated;

			property String^	MotorSerialNumber;
			property int		MinDistanceThreshold;
			property int		MaxDistanceThreshold;
			property Unit		DistanceUnit;

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

