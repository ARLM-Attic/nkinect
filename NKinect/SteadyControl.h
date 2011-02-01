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
	public ref class SteadyControl : public OpenNiControl {
		private:
			DateTime LastFired;

		public:
			delegate void SteadyDelegate();
			delegate void Callback(XnFloat fVelocity, void *pUserCxt);

			property int Duration;

			SteadyDelegate^ ControlEvent;
			Callback^ SteadyCB;

			SteadyControl(int duration, SteadyDelegate^ func) : OpenNiControl(ControlType::Steady) {
				XnMControl = new XnVSteadyDetector();
				SteadyCB = gcnew Callback(this, &SteadyControl::Steady);

				((XnVSteadyDetector*) XnMControl)->RegisterSteady(OpenNiContext, (XnVSteadyDetector::SteadyCB) Marshal::GetFunctionPointerForDelegate(SteadyCB).ToPointer());

				ControlEvent = func;
				LastFired = DateTime::Now;
				Duration = duration;
			}

		private:
			void Steady(XnFloat fVelocity, void *pUserCxt) {
				if (ControlEvent == nullptr || DateTime::Now.Subtract(LastFired).TotalSeconds < Duration)
					return;

				ControlEvent();
				LastFired = DateTime::Now;
			}
	};
}