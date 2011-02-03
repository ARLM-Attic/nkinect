#pragma once

#include <windows.h>
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
#include "XnVNite.h"
#include "OpenNiControl.h"
#include "cv.h"
#include "Vector3.h"

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
	public ref class UserSkeleton {
		public:
			property XnUserID UserId;
			property Vector3^ Forward;
			property Vector3^ Up;
			// ---- Joints ----
			property Vector3^ Head;
			property Vector3^ Neck;
			property Vector3^ Torso;
			property Vector3^ Waist;
			property Vector3^ LeftCollar;
			property Vector3^ LeftShoulder;
			property Vector3^ LeftElbow;
			property Vector3^ LeftWrist;
			property Vector3^ LeftHand;
			property Vector3^ LeftFingertip;
			property Vector3^ RightCollar;
			property Vector3^ RightShoulder;
			property Vector3^ RightElbow;
			property Vector3^ RightWrist;
			property Vector3^ RightHand;
			property Vector3^ RightFingertip;
			property Vector3^ LeftHip;
			property Vector3^ LeftKnee;
			property Vector3^ LeftAnkle;
			property Vector3^ LeftFoot;
			property Vector3^ RightHip;
			property Vector3^ RightKnee;
			property Vector3^ RightAnkle;
			property Vector3^ RightFoot;

			UserSkeleton(SkeletonCapability* skeleton, XnUserID userId) {
				UserId = userId;

				XnSkeletonJointTransformation* joints = new XnSkeletonJointTransformation[25];

				for (int i = 1; i < 25; i++)
					skeleton->GetSkeletonJoint(userId, (XnSkeletonJoint) i, joints[i]);

				Head = gcnew Vector3(joints[XN_SKEL_HEAD]);
				Neck = gcnew Vector3(joints[XN_SKEL_NECK]);
				Torso = gcnew Vector3(joints[XN_SKEL_TORSO]);
				Waist = gcnew Vector3(joints[XN_SKEL_WAIST]);
				LeftCollar = gcnew Vector3(joints[XN_SKEL_LEFT_COLLAR]);
				LeftShoulder = gcnew Vector3(joints[XN_SKEL_LEFT_SHOULDER]);
				LeftElbow = gcnew Vector3(joints[XN_SKEL_LEFT_ELBOW]);
				LeftWrist = gcnew Vector3(joints[XN_SKEL_LEFT_WRIST]);
				LeftHand = gcnew Vector3(joints[XN_SKEL_LEFT_HAND]);
				LeftFingertip = gcnew Vector3(joints[XN_SKEL_LEFT_FINGERTIP]);
				RightCollar = gcnew Vector3(joints[XN_SKEL_RIGHT_COLLAR]);
				RightShoulder = gcnew Vector3(joints[XN_SKEL_RIGHT_SHOULDER]);
				RightElbow = gcnew Vector3(joints[XN_SKEL_RIGHT_ELBOW]);
				RightWrist = gcnew Vector3(joints[XN_SKEL_RIGHT_WRIST]);
				RightHand = gcnew Vector3(joints[XN_SKEL_RIGHT_HAND]);
				RightFingertip = gcnew Vector3(joints[XN_SKEL_RIGHT_FINGERTIP]);
				LeftHip = gcnew Vector3(joints[XN_SKEL_LEFT_HIP]);
				LeftKnee = gcnew Vector3(joints[XN_SKEL_LEFT_KNEE]);
				LeftAnkle = gcnew Vector3(joints[XN_SKEL_LEFT_ANKLE]);
				LeftFoot = gcnew Vector3(joints[XN_SKEL_LEFT_FOOT]);
				RightHip = gcnew Vector3(joints[XN_SKEL_RIGHT_HIP]);
				RightKnee = gcnew Vector3(joints[XN_SKEL_RIGHT_KNEE]);
				RightAnkle = gcnew Vector3(joints[XN_SKEL_RIGHT_ANKLE]);
				RightFoot = gcnew Vector3(joints[XN_SKEL_RIGHT_FOOT]);

				delete[] joints;
			}
	};
}