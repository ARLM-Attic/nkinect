#pragma once

#include <windows.h>
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
#include "XnVNite.h"
#include "OpenNiControl.h"
#include "cv.h"
#include "UserHandler.h"
#include "BitmapFromIplImage.h"

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
	public ref class Sensor {
		private:
			Thread^ UpdateThread;
			bool Running;
			bool Disposed;
			Context* OpenNiContext;
			XnVSessionManager* SessionManager;
			HandsGenerator* HandsGen;
			DepthGenerator* DepthGen;
			ImageGenerator* ImageGen;
			UserGenerator* UserGen;
			IplImage* RgbImage;
			IplImage* GrayImage;
			UserHandler^ Users;

			void Init() {
				NodeInfoList list;

				Depths = gcnew array<array<double>^>(XN_VGA_X_RES);

				for (int i = 0; i < XN_VGA_X_RES; i++)
					Depths[i] = gcnew array<double>(XN_VGA_Y_RES);

				OpenNiContext = new Context();
				OpenNiContext->InitFromXmlFile("Data\\openni.xml");

				DepthGen = new DepthGenerator();
				ImageGen = new ImageGenerator();
				HandsGen = new HandsGenerator();
				UserGen = new UserGenerator();

				SessionStartCB = gcnew SessionStartDelegate(this, &Sensor::SessionStarted);
				SessionEndCB = gcnew SessionEndDelegate(this, &Sensor::SessionEnded);
				FocusProgressCB = gcnew FocusProgressDelegate(this, &Sensor::FocusProgress);

				OpenNiControl::OpenNiContext = OpenNiContext;

				SessionManager = new XnVSessionManager();
				SessionManager->Initialize(OpenNiContext, "Wave,Click", "RaiseHand");
				SessionManager->RegisterSession(OpenNiContext, (XnVSessionListener::OnSessionStartCB) Marshal::GetFunctionPointerForDelegate(SessionStartCB).ToPointer(), (XnVSessionListener::OnSessionEndCB) Marshal::GetFunctionPointerForDelegate(SessionEndCB).ToPointer(), (XnVSessionListener::OnFocusStartDetectedCB) Marshal::GetFunctionPointerForDelegate(FocusProgressCB).ToPointer());

				OpenNiContext->EnumerateExistingNodes(list);

				for (NodeInfoList::Iterator it = list.Begin(); it != list.End(); ++it) {
					NodeInfo node = *it;

					switch (node.GetDescription().Type) {
					case XN_NODE_TYPE_DEVICE:
						break;
					case XN_NODE_TYPE_DEPTH:
						node.GetInstance(*DepthGen);

						break;
					case XN_NODE_TYPE_USER:
						node.GetInstance(*UserGen);

						break;
					case XN_NODE_TYPE_IMAGE:
						node.GetInstance(*ImageGen);

						break;
					case XN_NODE_TYPE_IR:
						break;
					case XN_NODE_TYPE_AUDIO:
						break;
					case XN_NODE_TYPE_PLAYER:
						break;
					case XN_NODE_TYPE_GESTURE:
						break;
					case XN_NODE_TYPE_SCENE:
						break;
					case XN_NODE_TYPE_HANDS:
						node.GetInstance(*HandsGen);

						break;
					}
				}

				RgbImage = cvCreateImage(cvSize(XN_VGA_X_RES, XN_VGA_Y_RES), IPL_DEPTH_8U, 3);
				GrayImage = cvCreateImage(cvSize(XN_VGA_X_RES, XN_VGA_Y_RES), IPL_DEPTH_8U, 1);

				HandsGen->SetSmoothing(0.1);
				DepthGen->GetFrameSyncCap().FrameSyncWith(*ImageGen);
				DepthGen->GetAlternativeViewPointCap().SetViewPoint(*ImageGen);
			}

		public:
			array<array<double>^>^ Depths;

			delegate void SessionStartDelegate(const XnPoint3D& ptPosition, void* UserCxt);
			delegate void SessionEndDelegate(void* UserCxt);
			delegate void FocusProgressDelegate(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt);
			delegate void BitmapCallback(Bitmap^ bmp);

			SessionStartDelegate^ SessionStartCB;
			SessionEndDelegate^ SessionEndCB;
			FocusProgressDelegate^ FocusProgressCB;
			BitmapCallback^ RgbFunc;
			BitmapCallback^ DepthFunc;

			Sensor() {
				Init();

				Users = gcnew UserHandler(UserGen, nullptr);
			}

			Sensor(UserHandler::SkeletonCallback^ skeletonCallback, BitmapCallback^ rgb, BitmapCallback^ depth) {
				Init();

				Users = gcnew UserHandler(UserGen, skeletonCallback);
				RgbFunc = rgb;
				DepthFunc = depth;
			}

			~Sensor() {
				delete HandsGen;
				delete DepthGen;
				delete ImageGen;
				delete OpenNiContext;
			}

			!Sensor() {
				delete HandsGen;
				delete DepthGen;
				delete ImageGen;
				delete OpenNiContext;
			}

			void AddControl(OpenNiControl^ control) {
				SessionManager->AddListener(control->XnMControl);
			}

			void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt) {
			}

			void XN_CALLBACK_TYPE SessionStarted(const XnPoint3D& ptPosition, void* UserCxt) {
			}

			void XN_CALLBACK_TYPE SessionEnded(void* UserCxt) {
			}
	
		protected:
			void UpdateSensor() {
				cvZero(RgbImage);
				cvZero(GrayImage);
				Bitmap^ rBmp;
				Bitmap^ gBmp;

				while (Running) {
					OpenNiContext->WaitAndUpdateAll();
					SessionManager->Update(OpenNiContext);

					const XnDepthPixel* depthMap = DepthGen->GetDepthMap();
					const XnRGB24Pixel* imageMap = ImageGen->GetRGB24ImageMap();

					for (int i = 0, y = 0; y < XN_VGA_Y_RES; y++) {
						for (int x = 0; x < XN_VGA_X_RES; x++, i++) {
							Depths[x][y] = depthMap[i];

 							((unsigned char*) RgbImage->imageData)[(y * XN_VGA_X_RES + x) * 3 + 0] = imageMap[y * XN_VGA_X_RES + x].nBlue;
 							((unsigned char*) RgbImage->imageData)[(y * XN_VGA_X_RES + x) * 3 + 1] = imageMap[y * XN_VGA_X_RES + x].nGreen;
 							((unsigned char*) RgbImage->imageData)[(y * XN_VGA_X_RES + x) * 3 + 2] = imageMap[y * XN_VGA_X_RES + x].nRed; 						

							((unsigned char*) GrayImage->imageData)[(y * XN_VGA_X_RES + x)] = depthMap[y * XN_VGA_X_RES + x];
						}
					}

					if (DepthFunc != nullptr)
						DepthFunc(BitmapFromIplImage::Convert(GrayImage));

					if (RgbFunc != nullptr)
						RgbFunc(BitmapFromIplImage::Convert(RgbImage));

					Users->ManageUsers();
				}
			}

		public:
			void Start() {
				Running = true;

				OpenNiContext->StartGeneratingAll();

				UpdateThread = gcnew Thread(gcnew ThreadStart(this, &Sensor::UpdateSensor));
				UpdateThread->IsBackground = true;
				UpdateThread->Start();
			}

			void Stop() {
				if (Disposed)
					return;

				Running = false;

				if (UpdateThread != nullptr)
					UpdateThread->Join(1000);

				OpenNiContext->StopGeneratingAll();
				OpenNiContext->Shutdown();

				Disposed = true;
			}

			void ExportPLY(String^ path) {
				int minDistance = -10;
				double scaleFactor = 0.0021;

				TextWriter^ tw = File::CreateText(path);

				StringBuilder^ sb = gcnew StringBuilder();

				sb->AppendLine("ply");
				sb->AppendLine("format ascii 1.0");
				sb->AppendLine("comment Created by nKinect");
				sb->AppendLine("element vertex 307200");
				sb->AppendLine("property float x");
				sb->AppendLine("property float y");
				sb->AppendLine("property float z");
				sb->AppendLine("end_header");

				for (int j = 0; j < XN_VGA_Y_RES; j++) {
					for (int i = 0; i < XN_VGA_X_RES; i++) {
						double z = Depths[i][j];
						double y = (XN_VGA_X_RES / 2 - j) * (z + minDistance) * scaleFactor;
						double x = (i - XN_VGA_Y_RES / 2) * (z + minDistance) * scaleFactor;

						sb->AppendLine(String::Format("{0}\t{1}\t{2}", x, y, z));
					}
				}

				tw->WriteLine(sb);
				tw->Close();
			}
		};
}
