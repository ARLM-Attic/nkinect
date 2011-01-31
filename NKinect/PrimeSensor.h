#pragma once
#pragma once
#include "CLNUIDevice.h"
#include "BaseKinect.h"
#include "Accelerometer.h"

using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Text;
using namespace ManagedNite;

namespace NKinect {
	private ref class PrimeSensor : public BaseKinect {
		private:
			Thread^ RefreshThread;
			Thread^ AccelerometerThread;
			bool Running;
			PUSHORT RawDepth;
			PDWORD RawPixels;
			array<array<double>^>^ Depths;
			bool Disposed;
			XnMOpenNIContext^ Context;
			XnMSessionManager^ SessionManager;
			XnMSelectableSlider2D^ Slider2D;

		public:
			PrimeSensor() {
				Context = gcnew XnMOpenNIContext();
				Context->Init();

				SessionManager = gcnew XnMSessionManager(Context, "Wave", "RaiseHand");
				Slider2D = gcnew XnMSelectableSlider2D(1920, 1200);
				Slider2D->ItemHovered += gcnew EventHandler<SelectableSlider2DHoverEventArgs^>(this, &PrimeSensor::Slider2DItemHovered);

				SessionManager->AddListener(Slider2D);
			}

			void Slider2DItemHovered(Object^ sender, SelectableSlider2DHoverEventArgs^ e) {
				Console::WriteLine(e->X + ", " + e->Y);
			}

		protected:
			virtual void UpdateAccelerometer() override {
			}

			virtual void FlipDepth() override {
			}

			virtual void DownloadImages() override {
				while (Running) {
					Context->Update();
					SessionManager->Update(Context);
				}
			}

		public:
			virtual void Start() override {
				Running = true;

				RefreshThread = gcnew Thread(gcnew ThreadStart(this, &PrimeSensor::DownloadImages));
				RefreshThread->IsBackground = true;
				RefreshThread->Start();

				if (!accelerometerUpdateEnabled)
					return;

				AccelerometerThread = gcnew Thread(gcnew ThreadStart(this, &PrimeSensor::UpdateAccelerometer));
				AccelerometerThread->IsBackground = true;
				AccelerometerThread->Start();
			}

			virtual void Stop() override {
				if (Disposed)
					return;

				Running = false;

				if (RefreshThread != nullptr)
					RefreshThread->Join(1000);

				if (AccelerometerThread != nullptr)
					AccelerometerThread->Join(1000);

				delete RawDepth;
				delete RawPixels;

				Disposed = true;
			}

			virtual void SetLed(LedColor color) override {
			}

			virtual void SetPosition(short pos) override {
			}

			virtual void ExportPLY(String^ path) override {
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
			}
		};
}
