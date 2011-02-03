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
	public ref class Vector3 {
		public:
			property double X;
			property double Y;
			property double Z;

			Vector3(double x, double y, double z) {
				X = x;
				Y = y;
				Z = z;
			}

			Vector3(XnSkeletonJointTransformation joint) {
				X = joint.position.position.X;
				Y = joint.position.position.Y;
				Z = joint.position.position.Z;
			}
	};

	public ref class ColorVector3 : public Vector3 {
		public:
			property short R;
			property short G;
			property short B;

			ColorVector3(XnPoint3D point, XnRGB24Pixel color) : Vector3(point.X, point.Y, point.Z) {
				R = color.nRed;
				G = color.nGreen;
				B = color.nBlue;
			}
	};
}