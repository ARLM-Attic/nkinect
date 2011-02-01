#pragma once

#include <windows.h>
#include "CameraImageEventArgs.h"
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
			IplImage* RgbImage;
			IplImage* GrayImage;

		public:
			array<array<double>^>^ Depths;

			delegate void SessionStartDelegate(const XnPoint3D& ptPosition, void* UserCxt);
			delegate void SessionEndDelegate(void* UserCxt);
			delegate void FocusProgressDelegate(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt);

			SessionStartDelegate^ SessionStartCB;
			SessionEndDelegate^ SessionEndCB;
			FocusProgressDelegate^ FocusProgressCB;

			EventHandler<CameraImageEventArgs^>^	ImageUpdated;
			EventHandler<CameraImageEventArgs^>^	DepthImageUpdated;

			Sensor() {
				NodeInfoList list;

				Depths		= gcnew array<array<double>^>(XN_VGA_X_RES);

				for (int i = 0; i < XN_VGA_X_RES; i++)
					Depths[i] = gcnew array<double>(XN_VGA_Y_RES);

				OpenNiContext = new Context();
				OpenNiContext->InitFromXmlFile("Data\\openni.xml");

				DepthGen = new DepthGenerator();
				ImageGen = new ImageGenerator();
				HandsGen = new HandsGenerator();

				SessionStartCB = gcnew SessionStartDelegate(this, &Sensor::SessionStarted);
				SessionEndCB = gcnew SessionEndDelegate(this, &Sensor::SessionEnded);
				FocusProgressCB = gcnew FocusProgressDelegate(this, &Sensor::FocusProgress);

				OpenNiControl::OpenNiContext = OpenNiContext;

				SessionManager = new XnVSessionManager();
				SessionManager->Initialize(OpenNiContext, "Wave", "RaiseHand");
				SessionManager->RegisterSession(OpenNiContext, (XnVSessionListener::OnSessionStartCB) Marshal::GetFunctionPointerForDelegate(SessionStartCB).ToPointer(), (XnVSessionListener::OnSessionEndCB) Marshal::GetFunctionPointerForDelegate(SessionEndCB).ToPointer(), (XnVSessionListener::OnFocusStartDetectedCB) Marshal::GetFunctionPointerForDelegate(FocusProgressCB).ToPointer());
				
				OpenNiContext->EnumerateExistingNodes(list);

				for (NodeInfoList::Iterator it = list.Begin(); it != list.End(); ++it) {
					NodeInfo node = *it;
					Console::WriteLine(node.GetDescription().Type);

					switch (node.GetDescription().Type) {
						case XN_NODE_TYPE_DEVICE:
							break;
						case XN_NODE_TYPE_DEPTH:
							node.GetInstance(*DepthGen);

							break;
						case XN_NODE_TYPE_USER:
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

				OpenNiContext->StartGeneratingAll();
			}

			~Sensor() {
				delete HandsGen;
				delete DepthGen;
				delete ImageGen;
				delete SessionManager;
				delete OpenNiContext;
			}

			!Sensor() {
				delete HandsGen;
				delete DepthGen;
				delete ImageGen;
				delete SessionManager;
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

					/*if (ImageUpdated != nullptr)
						ImageUpdated(this, gcnew CameraImageEventArgs(RgbImage));

					if (DepthImageUpdated != nullptr)
						DepthImageUpdated(this, gcnew CameraImageEventArgs(GrayImage));*/
				}
			}

		public:
			void Start() {
				Running = true;

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

				Disposed = true;
			}

			/*void ExportPLY(String^ path) override {
			int minDistance = -10;
			double scaleFactor = 0.0021;

			TextWriter^ tw = File::CreateText(path);

			StringBuilder^ sb = gcnew StringBuilder();

			sb->AppendLine("ply");
			sb->AppendLine("format ascii 1.0");
			sb->AppendLine(String::Format("comment Created by NKinect v{0}", NKINECT_VERSION));
			sb->AppendLine("element vertex 307200");
			sb->AppendLine("property float x");
			sb->AppendLine("property float y");
			sb->AppendLine("property float z");
			sb->AppendLine("end_header");

			for (int j = 0; j < 480; j++) {
			for (int i = 0; i < 640; i++) {
			double z = Depths[i][j];
			double y = (640 / 2 - j) * (z + minDistance) * scaleFactor;
			double x = (i - 480 / 2) * (z + minDistance) * scaleFactor;

			sb->AppendLine(String::Format("{0}\t{1}\t{2}", x, y, z));
			}
			}

			tw->WriteLine(sb);
			tw->Close();
			}*/
		};
}
