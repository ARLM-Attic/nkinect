#pragma once
using namespace System;
using namespace System::Threading;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Text;

namespace NKinect {
	public ref class BitmapDataArray {
		public:
			Bitmap^			Bmp;
			BitmapData^		Data;
			array<Byte>^	ByteArray;
			PUSHORT			NativeArray;

			BitmapDataArray(bool isEnabled) {
				if (!isEnabled)
					return;

				ByteArray = gcnew array<Byte>(640 * 480 * 3);
				NativeArray = new USHORT[640 * 480 * 3];
			}

			!BitmapDataArray() {
				delete[] NativeArray;
			}

			void Reset() {
				if (ByteArray == nullptr || NativeArray == nullptr)
					return;

				Bmp = gcnew Bitmap(640, 480, PixelFormat::Format24bppRgb);
				Data = Bmp->LockBits(System::Drawing::Rectangle(0, 0, Bmp->Width, Bmp->Height), ImageLockMode::WriteOnly, Bmp->PixelFormat);
			}

			void SetBytes(int idx, byte first, byte second, byte third) {
				if (ByteArray == nullptr)
					return;

				ByteArray[idx]			= first;
				ByteArray[idx + 1]		= second;
				ByteArray[idx + 2]		= third;

				NativeArray[idx]		= first;
				NativeArray[idx + 1]	= second;
				NativeArray[idx + 2]	= third;
			}

			void End() {
				if (ByteArray == nullptr)
					return;

				Marshal::Copy(ByteArray, 0, Data->Scan0, 640 * 480 * 3);
				Bmp->UnlockBits(Data);
			}

			void Mirror() {
				if (Bmp == nullptr)
					return;

				Bmp->RotateFlip(RotateFlipType::RotateNoneFlipX);
			}
		};
}
