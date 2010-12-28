#pragma once
using namespace System;

namespace NKinect {
	public ref class KinectException : public Exception {
		public:
			KinectException(String^ message) : Exception(message) {
			}
	};
}