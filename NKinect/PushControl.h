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
	public ref class PushControl : public OpenNiControl {
		public:
			delegate void PushDelegate();
			delegate void Callback(XnFloat fVelocity, XnFloat fAngle, void *UserCxt);

			PushDelegate^ ControlEvent;
			Callback^ PushCB;
			
			PushControl(PushDelegate^ func) : OpenNiControl(ControlType::Push) {
				XnMControl = new XnVPushDetector();
				PushCB = gcnew Callback(this, &PushControl::Push);

				((XnVPushDetector*) XnMControl)->RegisterPush(OpenNiContext, (XnVPushDetector::PushCB) Marshal::GetFunctionPointerForDelegate(PushCB).ToPointer());

				ControlEvent = func;
			}

			void Push(XnFloat fVelocity, XnFloat fAngle, void *UserCxt) {
				if (ControlEvent == nullptr)
					return;

				ControlEvent();
			}
	};
}