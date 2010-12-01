#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::IO;

namespace NKinect {
	public ref class CameraImageEventArgs : public EventArgs {
		public:
			property Image^ CameraImage;

			CameraImageEventArgs(Bitmap^ bmp) {
				CameraImage = bmp;
			}
	};
}
