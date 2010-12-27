#pragma once
#include "BaseKinect.h"

using namespace System::Drawing;

namespace NKinect {
	public ref class MouseTracker {
		public:
			property Size^ ScreenSize;
			property BaseKinect^ Kinect;

			MouseTracker(BaseKinect^ kinect) {
				Kinect = kinect;
			}
	};
}