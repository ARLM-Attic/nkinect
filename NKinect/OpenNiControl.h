#pragma once

#include <windows.h>
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
#include "XnVNite.h"
#include "OpenNiControl.h"

using namespace xn;
using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Drawing::Imaging;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace NKinect {
	private enum ControlType {
		Slider2D,
		Slider1D,
		Swipe,
		Wave,
		Circle,
		Push,
		Steady
	};

	public ref class OpenNiControl {
		private:
			property ControlType Type;

		protected:

		public:
			static Context* OpenNiContext;
			XnVPointControl* XnMControl;

			OpenNiControl(ControlType type) {
				Type = type;
			}

			~OpenNiControl() {
				delete XnMControl;
			}

			!OpenNiControl() {
				delete XnMControl;
			}
	};
}
