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
				cam = CreateNUICamera();
				motor = CreateNUIMotor();

				RawDepth = new USHORT[640 * 480];
				RawPixels = new DWORD[640 * 480];

				Depths = gcnew array<array<double>^>(640);

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
			}

			virtual void DownloadImages() override {
				while (Running) {
					GetNUICameraColorFrameRGB32(cam, RawPixels, 500);
					GetNUICameraDepthFrameRAW(cam, RawDepth, 0);

					SetLed(Red);

					Bitmap^		 RgbImage	= gcnew Bitmap(640, 480, PixelFormat::Format32bppPArgb);
					Bitmap^		 GrayImage	= gcnew Bitmap(640, 480, PixelFormat::Format32bppPArgb);
					Bitmap^		 ThresImage	= gcnew Bitmap(640, 480, PixelFormat::Format32bppPArgb);
					Bitmap^		 RgbTImage	= gcnew Bitmap(640, 480, PixelFormat::Format32bppPArgb);

					BitmapData^	 rgbData	= RgbImage->LockBits(System::Drawing::Rectangle(0, 0, RgbImage->Width, RgbImage->Height), ImageLockMode::WriteOnly, RgbImage->PixelFormat);
					BitmapData^	 grayData	= GrayImage->LockBits(System::Drawing::Rectangle(0, 0, GrayImage->Width, GrayImage->Height), ImageLockMode::WriteOnly, RgbImage->PixelFormat);
					BitmapData^	 thresData	= ThresImage->LockBits(System::Drawing::Rectangle(0, 0, ThresImage->Width, ThresImage->Height), ImageLockMode::WriteOnly, RgbImage->PixelFormat);
					BitmapData^	 rgbTData	= RgbTImage->LockBits(System::Drawing::Rectangle(0, 0, ThresImage->Width, ThresImage->Height), ImageLockMode::WriteOnly, RgbImage->PixelFormat);
					
					array<Byte>^ ary		= gcnew array<Byte>(640 * 480 * 4);
					array<Byte>^ gsAry		= gcnew array<Byte>(640 * 480 * 4);
					array<Byte>^ thrAry		= gcnew array<Byte>(640 * 480 * 4);
					array<Byte>^ rgbTAry	= gcnew array<Byte>(640 * 480 * 4);

					for (int i = 0, y = 0, idx = 0; y < 480; y++) {
						for (int x = 0; x < 640; x++, i++, idx += 4) {
							Depths[x][y]	= DisparityToDistance(RawDepth[i] & 0x07FF);

							DWORD	color	= RawPixels[641 + i];
							int		gray	= DisparityToGrayscale(RawDepth[i] & 0x07FF);
							int		thresh	= Depths[x][y] >= MinDistanceThreshold && Depths[x][y] <= MaxDistanceThreshold ? gray : 0x00;

							byte	blue	= (color & 0x000000FF);
							byte	green	= (color & 0x0000FF00) >> 8;
							byte	red		= (color & 0x00FF0000) >> 16;

							ary[idx]		= blue;
							ary[idx + 1]	= green;
							ary[idx + 2]	= red;
							ary[idx + 3]	= 0xFF;

							gsAry[idx]		= gray;
							gsAry[idx + 1]	= gray;
							gsAry[idx + 2]	= gray;
							gsAry[idx + 3]	= 0xFF;

							thrAry[idx]		= thresh;
							thrAry[idx + 1]	= thresh;
							thrAry[idx + 2]	= thresh;
							thrAry[idx + 3]	= 0xFF;

							rgbTAry[idx]	= Depths[x][y] >= MinDistanceThreshold && Depths[x][y] <= MaxDistanceThreshold ? blue : 0x00;;
							rgbTAry[idx + 1]= Depths[x][y] >= MinDistanceThreshold && Depths[x][y] <= MaxDistanceThreshold ? green : 0x00;;
							rgbTAry[idx + 2]= Depths[x][y] >= MinDistanceThreshold && Depths[x][y] <= MaxDistanceThreshold ? red : 0x00;;
							rgbTAry[idx + 3]= 0xFF;
						}
					}

					Marshal::Copy(ary, 0, rgbData->Scan0, 640 * 480 * 4);
					Marshal::Copy(gsAry, 0, grayData->Scan0, 640 * 480 * 4);
					Marshal::Copy(thrAry, 0, thresData->Scan0, 640 * 480 * 4);
					Marshal::Copy(rgbTAry, 0, rgbTData->Scan0, 640 * 480 * 4);

					RgbImage->UnlockBits(rgbData);
					GrayImage->UnlockBits(grayData);
					ThresImage->UnlockBits(thresData);
					RgbTImage->UnlockBits(rgbTData);

					DepthsCalculated(this, gcnew DepthEventArgs(Depths));
					ImageUpdated(this, gcnew CameraImageEventArgs(RgbImage));
					DepthImageUpdated(this, gcnew CameraImageEventArgs(GrayImage));
					ThresholdDepthImageUpdated(this, gcnew CameraImageEventArgs(ThresImage));
					ThresholdColorImageUpdated(this, gcnew CameraImageEventArgs(RgbTImage));

					SetLed(Orange);
				}
			}

		public:
			virtual void Start() override {
				StartNUICamera(cam);
				SetLed(Orange);

				Running = true;

				RefreshThread = gcnew Thread(gcnew ThreadStart(this, &CLKinect::DownloadImages));
				RefreshThread->IsBackground = true;
				RefreshThread->Start();

				AccelerometerThread = gcnew Thread(gcnew ThreadStart(this, &CLKinect::UpdateAccelerometer));
				AccelerometerThread->IsBackground = true;
				AccelerometerThread->Start();
			}

			virtual void Stop() override {
				if (Disposed)
					return;

				Running = false;

				SetPosition(0);
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
