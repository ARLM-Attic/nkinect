#pragma once
#include "cv.h"

using namespace System;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

namespace NKinect {
	public ref class CameraImageEventArgs : public EventArgs {
		public:
			property Bitmap^ CameraImage;

			CameraImageEventArgs(IplImage* cvImg) {
				PixelFormat pxl = PixelFormat::DontCare;

				if (cvImg->nChannels == 3)
					pxl = PixelFormat::Format24bppRgb;
				else
					pxl = PixelFormat::Format8bppIndexed;

				CameraImage = gcnew Bitmap(cvImg->width, cvImg->height, cvImg->widthStep, pxl, (IntPtr) cvImg->imageData);

				if (cvImg->nChannels != 1)
					return;

				ColorPalette^ palette = CameraImage->Palette;
				array<Color>^ entries = palette->Entries;

				if (entries->Length == 0)
					entries = gcnew array<Color>(256);

				for (int i = 0; i < 256; i++)
					entries[i] = Color::FromArgb(i, i, i);
			}
	};
}
