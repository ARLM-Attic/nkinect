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
	public ref class Slider2DControl : public OpenNiControl {
		public:
			delegate void HoverDelegate(int x, int y);
			delegate void Callback(XnInt32 nXIndex, XnInt32 nYIndex, void *pUserCxt);

			property int Width;
			property int Height;

			HoverDelegate^ ControlEvent;
			Callback^ HoverCB;

			Slider2DControl(int width, int height, HoverDelegate^ func) : OpenNiControl(ControlType::Slider2D) {
				Width = width;
				Height = height;

				XnMControl = new XnVSelectableSlider2D(Width, Height);
				HoverCB = gcnew Callback(this, &Slider2DControl::ItemHovered);

				ControlEvent = func;

				((XnVSelectableSlider2D*) XnMControl)->RegisterItemHover(OpenNiContext, (XnVSelectableSlider2D::ItemHoverCB) Marshal::GetFunctionPointerForDelegate(HoverCB).ToPointer());
			}

			void XN_CALLBACK_TYPE ItemHovered(XnInt32 nXIndex, XnInt32 nYIndex, void *pUserCxt) {
				if (ControlEvent == nullptr)
					return;

				ControlEvent(nXIndex, Height - nYIndex);
			}
	};
};