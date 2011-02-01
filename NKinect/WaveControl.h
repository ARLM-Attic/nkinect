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
	public ref class WaveControl : public OpenNiControl {
		private:
			DateTime LastFired;

		public:
			delegate void WaveDelegate();
			delegate void Callback(void *pUserCxt);
			
			Callback^ WaveCB;
			WaveDelegate^ ControlEvent;

			WaveControl(WaveDelegate^ func) : OpenNiControl(ControlType::Wave) {
				XnMControl = new XnVWaveDetector();
				WaveCB = gcnew Callback(this, &WaveControl::Wave);

				((XnVWaveDetector*) XnMControl)->RegisterWave(OpenNiContext, (XnVWaveDetector::WaveCB) Marshal::GetFunctionPointerForDelegate(WaveCB).ToPointer());

				ControlEvent = func;
				LastFired = DateTime::Now;
			}

			void Wave(void *pUserCxt) {
				if (ControlEvent == nullptr || DateTime::Now.Subtract(LastFired).TotalSeconds < 5)
					return;

				ControlEvent();
				LastFired = DateTime::Now;
			}
	};
}