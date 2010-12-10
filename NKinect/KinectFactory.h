#pragma once
#include "CLNUIDevice.h"
#include "BaseKinect.h"
#include "CLKinect.h"

using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace NKinect {
	public ref class KinectFactory abstract sealed {
		static int CreatedKinects = 0;

		public:
			static BaseKinect^ GetKinect() {
				// TODO: Check for libfreenect, autodetect library.

				/*if (CreatedKinects >= GetNUIDeviceCount())
					throw gcnew ArgumentException("Too many Kinect objects created.");*/

				return gcnew CLKinect(0);
			}
	};
}
