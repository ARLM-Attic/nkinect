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

			property String^	MotorSerialNumber;
			property int		MinDistanceThreshold;
			property int		MaxDistanceThreshold;
			property int		MinDisparityThreshold;
			property int		MaxDisparityThreshold;
			property Unit		DistanceUnit;

			BaseKinect() {
				MinDistanceThreshold = 0;
				MaxDistanceThreshold = 9999;

				MinDisparityThreshold = 0;
				MaxDisparityThreshold = 1023;

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
				double baseCalc = (double) (100.00 / (-0.00307 * val + 3.33));

				switch (DistanceUnit) {
					case Centimeters:	break;

					case Meters:		baseCalc * 0.01;
										break;

					case Inches:		baseCalc * 0.393700787;
										break;

					case Feet:			baseCalc * 0.032808399;
										break;
				}

				return (baseCalc < MinDistanceThreshold || baseCalc > MaxDistanceThreshold) ? 0.00 : baseCalc;
			}

			int DisparityToGrayscale(short val) {
				return (val < MinDisparityThreshold || val > MaxDisparityThreshold) ? 0 : (2048 * 256) / (2048 - val);
			}

			virtual void UpdateAccelerometer()	 = 0;
			virtual void DownloadImages()		 = 0;

		public:
			virtual void Start()				 = 0;
			virtual void Stop()					 = 0;
			virtual void SetLed(LedColor color)  = 0;
			virtual void SetPosition(short pos)  = 0;
			virtual void ExportPLY(String^ path) = 0;
	};
}

