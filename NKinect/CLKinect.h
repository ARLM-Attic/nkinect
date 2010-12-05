#pragma once
#include "CLNUIDevice.h"
#include "BaseKinect.h"

using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace NKinect {
	private ref class CLKinect : public BaseKinect {
		private:
			CLNUICamera cam;
			CLNUIMotor motor;
			Thread^ RefreshThread;
			Thread^ AccelerometerThread;
			bool Running;
			PUSHORT RawDepth;
			PDWORD RawPixels;
			array<array<double>^>^ Depths;
			bool Disposed;

		public:
			CLKinect() {
				cam			= CreateNUICamera();
				motor		= CreateNUIMotor();

				RawDepth	= new USHORT[640 * 480];
				RawPixels	= new DWORD[640 * 480];

				Depths		= gcnew array<array<double>^>(640);

				for (int i = 0; i < 640; i++)
					Depths[i] = gcnew array<double>(480);

				SetLed(Green);
				SetPosition(0);

				MotorSerialNumber = gcnew String(GetNUIMotorSerial(motor));
			}

		protected:
			virtual void UpdateAccelerometer() override {
				Accelerometer accel;

				while (Running) {
					GetNUIMotorAccelerometer(motor, accel.X, accel.Y, accel.Z);

					AccelerometerUpdated(this, gcnew AccelerometerEventArgs(accel.X, accel.Y, accel.Z));

					Thread::Sleep(100);
				}

				SetLed(Green);
			}

			virtual void DownloadImages() override {
				BitmapDataArray^ RgbImage	= gcnew BitmapDataArray(imageUpdateEnabled);
				BitmapDataArray^ GrayImage	= gcnew BitmapDataArray(depthImageUpdateEnabled);
				BitmapDataArray^ ThresImage = gcnew BitmapDataArray(thresholdDepthImageUpdateEnabled);
				BitmapDataArray^ RgbTImage	= gcnew BitmapDataArray(thresholdColorImageUpdateEnabled);

				while (Running) {
					GetNUICameraColorFrameRGB32(cam, RawPixels, 250);
					GetNUICameraDepthFrameRAW(cam, RawDepth, 0);

					SetLed(Red);

					RgbImage->Reset();
					GrayImage->Reset();
					ThresImage->Reset();
					RgbTImage->Reset();

					for (int i = 0, y = 0, idx = 0; y < 480; y++) {
						for (int x = 0; x < 640; x++, i++, idx += 4) {
							Depths[x][y]	= DisparityToDistance(RawDepth[i]);

							DWORD	color	= RawPixels[641 + i];
							int		gray	= DisparityToGrayscale(RawDepth[i]);
							bool	valid	= Depths[x][y] >= MinDistanceThreshold && Depths[x][y] <= MaxDistanceThreshold;
							int		thresh	= valid ? gray : 0x00;

							byte	blue	= (color & 0x000000FF);
							byte	green	= (color & 0x0000FF00) >> 8;
							byte	red		= (color & 0x00FF0000) >> 16;

							if (imageUpdateEnabled)
								RgbImage->SetBytes(idx, blue, green, red, 0xFF);

							if (depthImageUpdateEnabled)
								GrayImage->SetBytes(idx, gray, gray, gray, 0xFF);

							if (thresholdDepthImageUpdateEnabled)
								ThresImage->SetBytes(idx, thresh, thresh, thresh, 0xFF);

							if (thresholdDepthImageUpdateEnabled)
								RgbTImage->SetBytes(idx, valid ? blue : 0x00, valid ? green : 0x00, valid ? red : 0x00, 0xFF);
						}
					}

					RgbImage->End();
					GrayImage->End();
					ThresImage->End();
					RgbTImage->End();

					DepthsCalculated(this, gcnew DepthEventArgs(Depths));

					if (imageUpdateEnabled)
						ImageUpdated(this, gcnew CameraImageEventArgs(RgbImage->Bmp));

					if (depthImageUpdateEnabled)
						DepthImageUpdated(this, gcnew CameraImageEventArgs(GrayImage->Bmp));

					if (thresholdDepthImageUpdateEnabled)
						ThresholdDepthImageUpdated(this, gcnew CameraImageEventArgs(ThresImage->Bmp));

					if (thresholdDepthImageUpdateEnabled)
						ThresholdColorImageUpdated(this, gcnew CameraImageEventArgs(RgbTImage->Bmp));

					SetLed(Orange);
				}

				SetLed(Green);
			}

		public:
			virtual void Start() override {
				StartNUICamera(cam);
				SetLed(Orange);

				Running = true;

				RefreshThread = gcnew Thread(gcnew ThreadStart(this, &CLKinect::DownloadImages));
				RefreshThread->IsBackground = true;
				RefreshThread->Start();

				if (!accelerometerUpdateEnabled)
					return;

				AccelerometerThread = gcnew Thread(gcnew ThreadStart(this, &CLKinect::UpdateAccelerometer));
				AccelerometerThread->IsBackground = true;
				AccelerometerThread->Start();
			}

			virtual void Stop() override {
				if (Disposed)
					return;

				Running = false;

				SetPosition(0);

				if (RefreshThread != nullptr)
					RefreshThread->Join(1000);

				if (AccelerometerThread != nullptr)
					AccelerometerThread->Join(1000);

				SetLed(Green);

				StopNUICamera(cam);
				DestroyNUICamera(cam);
				DestroyNUIMotor(motor);

				delete RawDepth;
				delete RawPixels;

				Disposed = true;
			}

			virtual void SetLed(LedColor color) override {
				SetNUIMotorLED(motor, color);
			}

			virtual void SetPosition(short pos) override {
				SetNUIMotorPosition(motor, pos);
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
