#pragma once

#include <windows.h>
#include "CameraImageEventArgs.h"
#include "DepthEventArgs.h"
#include "Unit.h"
#include "BitmapDataArray.h"

using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Text;
using namespace ManagedNite;

namespace NKinect {
	private ref class Sensor {
		private:
			Thread^ RefreshThread;
			bool Running;
			array<array<double>^>^ Depths;
			bool Disposed;
			XnMOpenNIContext^ Context;
			XnMSessionManager^ SessionManager;
			XnMSelectableSlider2D^ Slider2D;
			XnMPushDetector^ PushDetector;
			XnMSteadyDetector^ SteadyDetector;

			property Point^ Coordinates;

		public:
			Sensor() {
				Context = gcnew XnMOpenNIContext();
				Context->Init();

				SessionManager = gcnew XnMSessionManager(Context, "Wave", "RaiseHand");

				Slider2D = gcnew XnMSelectableSlider2D(1920, 1200);
				Slider2D->ItemHovered += gcnew EventHandler<SelectableSlider2DHoverEventArgs^>(this, &Sensor::Slider2DItemHovered);
				
				PushDetector = gcnew XnMPushDetector();
				PushDetector->Push += gcnew EventHandler<PushDetectorEventArgs^>(this, &Sensor::PushDetected);

				SteadyDetector = gcnew XnMSteadyDetector();
				SteadyDetector->Steady += gcnew EventHandler<SteadyEventArgs^>(this, &Sensor::SteadyHand);

				SessionManager->AddListener(Slider2D);
				SessionManager->AddListener(PushDetector);
				SessionManager->AddListener(SteadyDetector);

				Coordinates = gcnew Point();
			}

			void SteadyHand(Object^ sender, SteadyEventArgs^ e) {
				Console::WriteLine("Steady hand detected @ " + Coordinates->X + ", " + Coordinates->Y);
				
				/*mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);*/
			}

			void PushDetected(Object^ sender, PushDetectorEventArgs^ e) {
				Console::WriteLine("Push detected @ " + Coordinates->X + ", " + Coordinates->Y);

				/*mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);*/
			}

			void Slider2DItemHovered(Object^ sender, SelectableSlider2DHoverEventArgs^ e) {
				Console::WriteLine(e->X + ", " + e->Y);

				SetCursorPos(Coordinates->X, Coordinates->Y);

				Coordinates->X = e->X;
				Coordinates->Y = 1200 - e->Y;
			}

		protected:
			void DownloadImages() {
				while (Running) {
					Context->Update();
					SessionManager->Update(Context);
				}
			}

		public:
			void Start() {
				Running = true;

				RefreshThread = gcnew Thread(gcnew ThreadStart(this, &Sensor::DownloadImages));
				RefreshThread->IsBackground = true;
				RefreshThread->Start();
			}

			void Stop() {
				if (Disposed)
					return;

				Running = false;

				if (RefreshThread != nullptr)
					RefreshThread->Join(1000);

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
