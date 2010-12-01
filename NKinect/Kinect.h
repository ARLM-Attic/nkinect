// KinectWrapper.h

#pragma once
#include "CLNUIDevice.h"
#include "Accelerometer.h"
#include "AccelerometerEventArgs.h"
#include "CameraImageEventArgs.h"
#include "DepthEventArgs.h"
#define NKINECT_VERSION 0.1

using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace NKinect {
	public enum LedColor : BYTE {
		Off = 0,
		Green = 1,
		Red = 2,
		Orange = 3,
		BlinkGreen = 4,
		BlinkRedOrange = 6,
	};

	public ref class Kinect {
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
			event EventHandler<DepthEventArgs^>^ DepthsCalculated;
			event EventHandler<AccelerometerEventArgs^>^ AccelerometerUpdated;
			event EventHandler<CameraImageEventArgs^>^ ImageUpdated;

			property String^ MotorSerialNumber;

			Kinect() {
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

			!Kinect() {
				Stop();
			}

			~Kinect() {
				Stop();
			}

		private:
			double DisparityToDistance(short val) {
				return /*val >= 1023 ? 0 : Math::Max(*/(double) (100.00 / (-0.00307 * val + 3.33)); /*, 0.00); */
			}

			void UpdateAccelerometer() {
				Accelerometer accel;

				while (Running) {
					GetNUIMotorAccelerometer(motor, accel.X, accel.Y, accel.Z);

					AccelerometerUpdated(this, gcnew AccelerometerEventArgs(accel.X, accel.Y, accel.Z));
					
					Thread::Sleep(100);
				}
			}

			void DownloadImages() {
				while (Running) {
					GetNUICameraColorFrameRGB32(cam, RawPixels, 500);
					GetNUICameraDepthFrameRAW(cam, RawDepth, 0);

					SetLed(Red);

					Bitmap^		 RgbImage	= gcnew Bitmap(640, 480, PixelFormat::Format32bppPArgb);
					BitmapData^	 bmpData	= RgbImage->LockBits(System::Drawing::Rectangle(0, 0, RgbImage->Width, RgbImage->Height), ImageLockMode::WriteOnly, RgbImage->PixelFormat);
					array<Byte>^ ary		= gcnew array<Byte>(640 * 480 * 4);

					for (int i = 0, y = 0, idx = 0; y < 480; y++) {
						for (int x = 0; x < 640; x++, i++, idx += 4) {
							Depths[x][y] = DisparityToDistance(RawDepth[i] & 0x07FF);

							DWORD color  = RawPixels[641 + i];
							byte blue	 = (color & 0x000000FF);
							byte green	 = (color & 0x0000FF00) >> 8;
							byte red	 = (color & 0x00FF0000) >> 16;
							byte alpha	 = (color & 0xFF000000) >> 24;

							ary[idx]	 = blue;
							ary[idx + 1] = green;
							ary[idx + 2] = red;
							ary[idx + 3] = 0xFF;
						}
					}

					Marshal::Copy(ary, 0, bmpData->Scan0, 640 * 480 * 4);

					RgbImage->UnlockBits(bmpData);

					DepthsCalculated(this, gcnew DepthEventArgs(Depths));
					ImageUpdated(this, gcnew CameraImageEventArgs(RgbImage));

					SetLed(Orange);
				}
			}

		public:
			void Start() {
				StartNUICamera(cam);
				SetLed(Orange);

				Running = true;

				RefreshThread = gcnew Thread(gcnew ThreadStart(this, &Kinect::DownloadImages));
				RefreshThread->IsBackground = true;
				RefreshThread->Start();

				AccelerometerThread = gcnew Thread(gcnew ThreadStart(this, &Kinect::UpdateAccelerometer));
				AccelerometerThread->IsBackground = true;
				AccelerometerThread->Start();
			}

			void Stop() {
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

			void SetLed(LedColor color) {
				SetNUIMotorLED(motor, color);
			}

			void SetPosition(short pos) {
				SetNUIMotorPosition(motor, pos);
			}

			void ExportPLY(String^ path) {
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
