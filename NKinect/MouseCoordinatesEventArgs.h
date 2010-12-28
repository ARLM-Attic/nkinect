#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::IO;

namespace NKinect {
	public ref class MouseCoordinatesEventArgs : public EventArgs {
		public:
			property int X;
			property int Y;

			MouseCoordinatesEventArgs(Point^ coords) {
				X = coords->X;
				Y = coords->Y;
			}
	};
}
