#pragma once

#include <windows.h>
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
#include "XnVNite.h"
#include "OpenNiControl.h"
#include "cv.h"
#include "UserSkeleton.h"

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
	public ref class UserHandler {
		private:
			delegate void UserCallback(UserGenerator& generator, XnUserID nId, void* pCookie);
			delegate void CalibrationStartDelegate(SkeletonCapability& capability, XnUserID nId, void* pCookie);
			delegate void CalibrationEndDelegate(SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie);
			delegate void PoseDelegate(PoseDetectionCapability& pose, const XnChar* strPose, XnUserID user, void* pCookie);
			UserGenerator* UserGen;

		public:
			delegate void SkeletonCallback(UserSkeleton^ skeletonInfo);
			SkeletonCallback^ ControlFunc;

			UserCallback^ NewUserCB;
			UserCallback^ LostUserCB;
			CalibrationStartDelegate^ CalibrationStartCB;
			CalibrationEndDelegate^ CalibrationEndCB;
			PoseDelegate^ PoseStartCB;
			PoseDelegate^ PoseEndCB;

			UserHandler(UserGenerator* gen, SkeletonCallback^ func) {
				UserGen = gen;

				NewUserCB = gcnew UserCallback(this, &UserHandler::NewUser);
				LostUserCB = gcnew UserCallback(this, &UserHandler::LostUser);
				CalibrationStartCB = gcnew CalibrationStartDelegate(this, &UserHandler::CalibrationStarted);
				CalibrationEndCB = gcnew CalibrationEndDelegate(this, &UserHandler::CalibrationEnded);
				PoseStartCB = gcnew PoseDelegate(this, &UserHandler::PoseStart);
				PoseEndCB = gcnew PoseDelegate(this, &UserHandler::PoseEnd);

				XnCallbackHandle hUserCallbacks;
				XnCallbackHandle hCalibrationCallbacks;
				XnCallbackHandle hPoseCallbacks;

				UserGen->RegisterUserCallbacks((UserGenerator::UserHandler) Marshal::GetFunctionPointerForDelegate(NewUserCB).ToPointer(), (UserGenerator::UserHandler) Marshal::GetFunctionPointerForDelegate(LostUserCB).ToPointer(), NULL, hUserCallbacks);
				UserGen->GetSkeletonCap().RegisterCalibrationCallbacks((SkeletonCapability::CalibrationStart) Marshal::GetFunctionPointerForDelegate(CalibrationStartCB).ToPointer(),
																	   (SkeletonCapability::CalibrationEnd) Marshal::GetFunctionPointerForDelegate(CalibrationEndCB).ToPointer(),
																	   NULL, hCalibrationCallbacks);
				UserGen->GetPoseDetectionCap().RegisterToPoseCallbacks((PoseDetectionCapability::PoseDetection) Marshal::GetFunctionPointerForDelegate(PoseStartCB).ToPointer(),
																	   (PoseDetectionCapability::PoseDetection) Marshal::GetFunctionPointerForDelegate(PoseEndCB).ToPointer(), NULL, hPoseCallbacks);

				UserGen->GetSkeletonCap().SetSmoothing(0.1);
				UserGen->GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

				if (func != nullptr)
					ControlFunc = func;
			}

			void XN_CALLBACK_TYPE NewUser(UserGenerator& generator, XnUserID nId, void* pCookie) {
				Console::WriteLine("New user detected: " + nId);

				UserGen->GetPoseDetectionCap().StartPoseDetection("Psi", nId);
			}

			void XN_CALLBACK_TYPE LostUser(UserGenerator& generator, XnUserID nId, void* pCookie) {
				Console::WriteLine("User lost: " + nId);
			}

			void XN_CALLBACK_TYPE CalibrationStarted(SkeletonCapability& capability, XnUserID nId, void* pCookie) {
				Console::WriteLine("Calibration started for user: " + nId);
			}

			void XN_CALLBACK_TYPE CalibrationEnded(SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie) {
				if (bSuccess) {
					Console::WriteLine("Calibration successful for user: " + nId);
					
					UserGen->GetSkeletonCap().StartTracking(nId);

					Console::WriteLine("Now tracking user " + nId);
				} else {
					Console::WriteLine("Calibration failed for user: " + nId);

					UserGen->GetPoseDetectionCap().StartPoseDetection("Psi", nId);
				}
			}

			void XN_CALLBACK_TYPE PoseStart(PoseDetectionCapability& pose, const XnChar* strPose, XnUserID nId, void* pCookie) {
				Console::WriteLine("Pose start, user: " + nId);

				UserGen->GetPoseDetectionCap().StopPoseDetection(nId);
				UserGen->GetSkeletonCap().RequestCalibration(nId, true);
			}

			void XN_CALLBACK_TYPE PoseEnd(PoseDetectionCapability& pose, const XnChar* strPose, XnUserID nId, void* pCookie) {
				Console::WriteLine("Pose detected, user: " + nId);
			}

			void ManageUsers() {
				if (ControlFunc == nullptr)
					return;

				XnUInt16 nUsers = UserGen->GetNumberOfUsers();

				if (nUsers == 0)
					return;

				XnUserID* users = new XnUserID[nUsers];
				UserGen->GetUsers(users, nUsers);

				for (int i = 0; i < nUsers; i++) {
					if (!UserGen->GetSkeletonCap().IsTracking(users[i]))
						continue;

					ControlFunc(gcnew UserSkeleton(&UserGen->GetSkeletonCap(), users[i]));
				}

				delete[] users;
			}
	};
}