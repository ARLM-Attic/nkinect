#pragma once
#include "BitmapDataArray.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::IO;

namespace NKinect {
	public ref class CameraImageEventArgs : public EventArgs {
		public:
			property Bitmap^ CameraImage;
			property PUSHORT RawBytes;

			CameraImageEventArgs(BitmapDataArray^ bmp) {
				CameraImage = bmp->Bmp;
				RawBytes = bmp->NativeArray;
			}
			
			CameraImageEventArgs(Bitmap^ bmp, PUSHORT ary) {
				CameraImage = bmp;
				RawBytes = ary;
			}
	};
}
