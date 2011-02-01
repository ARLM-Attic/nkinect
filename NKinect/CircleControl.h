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
	public ref class CircleControl : public OpenNiControl {
		private:
			DateTime LastFired;

		public:
			delegate void CircleDelegate();
			delegate void Callback(XnFloat fTimes, XnBool bConfident, const XnVCircle *pCircle, void *pUserCxt);
			
			CircleDelegate^ ControlEvent;
			Callback^ CircleCB;

			CircleControl(CircleDelegate^ func) : OpenNiControl(ControlType::Circle) {
				XnMControl = new XnVCircleDetector();
				CircleCB = gcnew Callback(this, &CircleControl::Circle);

				((XnVCircleDetector*) XnMControl)->RegisterCircle(OpenNiContext, (XnVCircleDetector::CircleCB) Marshal::GetFunctionPointerForDelegate(CircleCB).ToPointer());

				ControlEvent = func;
				LastFired = DateTime::Now;
			}

			void Circle(XnFloat fTimes, XnBool bConfident, const XnVCircle *pCircle, void *pUserCxt) {
				if (ControlEvent == nullptr || DateTime::Now.Subtract(LastFired).TotalSeconds < 5)
					return;

				ControlEvent();
				LastFired = DateTime::Now;
			}
	};
}