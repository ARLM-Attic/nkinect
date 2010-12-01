#pragma once
using namespace System;

namespace NKinect {
	public ref class AccelerometerEventArgs : public EventArgs {
		public:
			property short X;
			property short Y;
			property short Z;

			AccelerometerEventArgs(short x, short y, short z) {
				X = x;
				Y = y;
				Z = z;
			}
	};
}