#pragma once
using namespace System;

namespace NKinect {
	public ref class KinectException : public Exception {
		public:
			KinectException(String^ message) : Exception(message) {
			}

			static KinectException^ NotFound			 = gcnew KinectException("No Kinect detected. Please verify that it's connected and that you have CL NUI / OpenKinect installed.");
			static KinectException^ RequiredDllsNotFound = gcnew KinectException("Required DLLs not found. Please verify that CLNUIDevice.dll is in the executable directory.");
	};
}