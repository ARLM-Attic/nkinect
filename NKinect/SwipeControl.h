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
	public ref class SwipeControl : public OpenNiControl {
		public:
			delegate void SwipeDelegate();
			delegate void Callback(XnFloat fVelocity, XnFloat fAngle, void *pUserCxt);

			Callback^ SwipeUpCB;
			Callback^ SwipeDownCB;
			Callback^ SwipeRightCB;
			Callback^ SwipeLeftCB;

			SwipeDelegate^ ControlEventSwipeUp;
			SwipeDelegate^ ControlEventSwipeDown;
			SwipeDelegate^ ControlEventSwipeRight;
			SwipeDelegate^ ControlEventSwipeLeft;

			SwipeControl(SwipeDelegate^ up, SwipeDelegate^ down, SwipeDelegate^ right, SwipeDelegate^ left) : OpenNiControl(ControlType::Swipe) {
				XnMControl = new XnVSwipeDetector();

				SwipeUpCB = gcnew Callback(this, &SwipeControl::SwipeUp);
				SwipeDownCB = gcnew Callback(this, &SwipeControl::SwipeDown);
				SwipeRightCB = gcnew Callback(this, &SwipeControl::SwipeRight);
				SwipeLeftCB = gcnew Callback(this, &SwipeControl::SwipeLeft);

				((XnVSwipeDetector*) XnMControl)->RegisterSwipeUp(OpenNiContext, (XnVSwipeDetector::SwipeCB) Marshal::GetFunctionPointerForDelegate(SwipeUpCB).ToPointer());
				((XnVSwipeDetector*) XnMControl)->RegisterSwipeDown(OpenNiContext, (XnVSwipeDetector::SwipeCB) Marshal::GetFunctionPointerForDelegate(SwipeDownCB).ToPointer());
				((XnVSwipeDetector*) XnMControl)->RegisterSwipeRight(OpenNiContext, (XnVSwipeDetector::SwipeCB) Marshal::GetFunctionPointerForDelegate(SwipeRightCB).ToPointer());
				((XnVSwipeDetector*) XnMControl)->RegisterSwipeLeft(OpenNiContext, (XnVSwipeDetector::SwipeCB) Marshal::GetFunctionPointerForDelegate(SwipeLeftCB).ToPointer());

				ControlEventSwipeUp = up;
				ControlEventSwipeDown = down;
				ControlEventSwipeRight = right;
				ControlEventSwipeLeft = left;
			}

		private:
			void SwipeUp(XnFloat fVelocity, XnFloat fAngle, void *pUserCxt) {
				if (ControlEventSwipeUp == nullptr)
					return;

				ControlEventSwipeUp();
			}

			void SwipeDown(XnFloat fVelocity, XnFloat fAngle, void *pUserCxt) {
				if (ControlEventSwipeDown == nullptr)
					return;

				ControlEventSwipeDown();
			}

			void SwipeRight(XnFloat fVelocity, XnFloat fAngle, void *pUserCxt) {
				if (ControlEventSwipeRight == nullptr)
					return;

				ControlEventSwipeRight();
			}

			void SwipeLeft(XnFloat fVelocity, XnFloat fAngle, void *pUserCxt) {
				if (ControlEventSwipeLeft == nullptr)
					return;

				ControlEventSwipeLeft();
			}
	};
}