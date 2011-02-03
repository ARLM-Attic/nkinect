#pragma once

#include <windows.h>
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
#include "XnVNite.h"
#include "OpenNiControl.h"
#include "cv.h"

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
	public ref class BitmapFromIplImage {
		public:
			static Bitmap^ Convert(IplImage* cvImg) {
				PixelFormat pxl = cvImg->nChannels == 3 ? PixelFormat::Format24bppRgb : PixelFormat::Format8bppIndexed;
				Bitmap^ bmp = gcnew Bitmap(cvImg->width, cvImg->height, cvImg->widthStep, pxl, (System::IntPtr) cvImg->imageData);

				if (pxl == PixelFormat::Format8bppIndexed) {
					ColorPalette^ palette = bmp->Palette;
					array<Color>^ entries = palette->Entries;

					for (int i = 0; i < 256; ++i)
						entries[i] = Color::FromArgb(i, i, i);

					bmp->Palette = palette;
				}

				return bmp;
			}
	};
}