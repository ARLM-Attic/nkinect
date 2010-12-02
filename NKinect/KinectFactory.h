#pragma once
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
		public:
			static BaseKinect^ GetKinect() {
				// TODO: Check for libfreenect, autodetect library.

				return gcnew CLKinect();
			}
	};
}
