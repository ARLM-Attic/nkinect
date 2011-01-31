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
using namespace System::Collections::Generic;
using namespace ManagedNite;

namespace NKinect {
	private enum ControlType {
		Slider2D,
		Slider1D,
		Swipe,
		Wave,
		Circle,
		Push,
		Steady
	};

	public ref class OpenNiControl {
		private:
			property ControlType Type;

		protected:

		public:
			XnMPointControl^ XnMControl;

			OpenNiControl(ControlType type) {
				Type = type;
			}
	};

	public ref class WaveControl : public OpenNiControl {
		public:
			delegate void WaveDelegate();
			WaveDelegate^ ControlEvent;

			WaveControl(WaveDelegate^ func) : OpenNiControl(ControlType::Wave) {
				XnMControl = gcnew XnMWaveDetector();

				((XnMWaveDetector^) XnMControl)->Wave += gcnew EventHandler(this, &WaveControl::Wave);

				ControlEvent = func;
			}

			void Wave(Object^ sender, EventArgs^ e) {
				if (ControlEvent == nullptr)
					return;

				ControlEvent();
			}
	};

	public ref class CircleControl : public OpenNiControl {
		public:
			delegate void CircleDelegate();
			CircleDelegate^ ControlEvent;

			CircleControl(CircleDelegate^ func) : OpenNiControl(ControlType::Circle) {
				XnMControl = gcnew XnMCircleDetector();

				((XnMCircleDetector^) XnMControl)->Circle += gcnew EventHandler<CircleEventArgs^>(this, &CircleControl::Circle);

				ControlEvent = func;
			}

			void Circle(Object^ sender, CircleEventArgs^ e) {
				if (ControlEvent == nullptr)
					return;

				ControlEvent();
			}
	};

	public ref class SwipeControl : public OpenNiControl {
		public:
			delegate void SwipeDelegate();
			SwipeDelegate^ ControlEventSwipeUp;
			SwipeDelegate^ ControlEventSwipeDown;
			SwipeDelegate^ ControlEventSwipeRight;
			SwipeDelegate^ ControlEventSwipeLeft;

			SwipeControl(SwipeDelegate^ up, SwipeDelegate^ down, SwipeDelegate^ right, SwipeDelegate^ left) : OpenNiControl(ControlType::Swipe) {
				XnMControl = gcnew XnMSwipeDetector();

				((XnMSwipeDetector^) XnMControl)->SwipeUp += gcnew EventHandler<SwipeDetectorEventArgs^>(this, &SwipeControl::SwipeUp);
				((XnMSwipeDetector^) XnMControl)->SwipeDown += gcnew EventHandler<SwipeDetectorEventArgs^>(this, &SwipeControl::SwipeDown);
				((XnMSwipeDetector^) XnMControl)->SwipeRight += gcnew EventHandler<SwipeDetectorEventArgs^>(this, &SwipeControl::SwipeRight);
				((XnMSwipeDetector^) XnMControl)->SwipeLeft += gcnew EventHandler<SwipeDetectorEventArgs^>(this, &SwipeControl::SwipeLeft);

				ControlEventSwipeUp = up;
				ControlEventSwipeDown = down;
				ControlEventSwipeRight = right;
				ControlEventSwipeLeft = left;
			}

		private:
			void SwipeUp(Object^ sender, SwipeDetectorEventArgs^ e) {
				if (ControlEventSwipeUp == nullptr)
					return;

				ControlEventSwipeUp();
			}

			void SwipeDown(Object^ sender, SwipeDetectorEventArgs^ e) {
				if (ControlEventSwipeDown == nullptr)
					return;

				ControlEventSwipeDown();
			}

			void SwipeRight(Object^ sender, SwipeDetectorEventArgs^ e) {
				if (ControlEventSwipeRight == nullptr)
					return;

				ControlEventSwipeRight();
			}

			void SwipeLeft(Object^ sender, SwipeDetectorEventArgs^ e) {
				if (ControlEventSwipeLeft == nullptr)
					return;

				ControlEventSwipeLeft();
			}
	};

	public ref class SteadyControl : public OpenNiControl {
		private:
			DateTime LastFired;

		public:
			delegate void SteadyDelegate();
			SteadyDelegate^ ControlEvent;
			property int Duration;

			SteadyControl(int duration, SteadyDelegate^ func) : OpenNiControl(ControlType::Steady) {
				XnMControl = gcnew XnMSteadyDetector();

				((XnMSteadyDetector^) XnMControl)->Steady += gcnew EventHandler<SteadyEventArgs^>(this, &SteadyControl::Steady);

				ControlEvent = func;
				LastFired = DateTime::Now;
				Duration = duration;
			}

		private:
			void Steady(Object^ sender, SteadyEventArgs^ e) {
				if (ControlEvent == nullptr || DateTime::Now.Subtract(LastFired).TotalSeconds < Duration)
					return;

				ControlEvent();
				LastFired = DateTime::Now;
			}
	};

	public ref class PushControl : public OpenNiControl {
		public:
			delegate void PushDelegate();
			PushDelegate^ ControlEvent;
			
			PushControl(PushDelegate^ func) : OpenNiControl(ControlType::Push) {
				XnMControl = gcnew XnMPushDetector();

				((XnMPushDetector^) XnMControl)->Push += gcnew EventHandler<PushDetectorEventArgs^>(this, &PushControl::Push);

				ControlEvent = func;
			}

			void Push(Object^ sender, PushDetectorEventArgs^ e) {
				if (ControlEvent == nullptr)
					return;

				ControlEvent();
			}
	};

	public ref class Slider2DControl : public OpenNiControl {
		public:
			delegate void HoverDelegate(int x, int y);
			HoverDelegate^ ControlEvent;
			property int Width;
			property int Height;

			Slider2DControl(int width, int height, HoverDelegate^ func) : OpenNiControl(ControlType::Slider2D) {
				Width = width;
				Height = height;

				XnMControl = gcnew XnMSelectableSlider2D(Width, Height);
				((XnMSelectableSlider2D^) XnMControl)->ItemHovered += gcnew EventHandler<SelectableSlider2DHoverEventArgs^>(this, &Slider2DControl::ItemHovered);

				ControlEvent = func;
			}

			void ItemHovered(Object^ sender, SelectableSlider2DHoverEventArgs^ e) {
				if (ControlEvent == nullptr)
					return;

				ControlEvent(e->X, Height - e->Y);
			}
	};

	public ref class Win32Helper {
		public:
			static void SendLeftMouseClick() {
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}

			static void SendRightMouseClick() {
				mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			}
	};

	public ref class Sensor {
		private:
			Thread^ UpdateThread;
			bool Running;
			array<array<double>^>^ Depths;
			bool Disposed;
			XnMOpenNIContext^ Context;
			XnMSessionManager^ SessionManager;

		public:
			EventHandler^ SessionStarted;

			Sensor() {
				Context = gcnew XnMOpenNIContext();
				Context->Init();

				Context->SetSmoothing(0.1);
				SessionManager = gcnew XnMSessionManager(Context, "Wave", "RaiseHand");
			}

			void AddControl(OpenNiControl^ control) {
				SessionManager->AddListener(control->XnMControl);
			}

		protected:
			void UpdateSensor() {
				while (Running) {
					Context->Update();
					SessionManager->Update(Context);
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
