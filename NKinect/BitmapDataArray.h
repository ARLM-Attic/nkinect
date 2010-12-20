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

			BitmapDataArray(bool isEnabled) {
				if (!isEnabled)
					return;

				ByteArray = gcnew array<Byte>(640 * 480 * 4);
			}

			void Reset() {
				if (ByteArray == nullptr)
					return;

				Bmp = gcnew Bitmap(640, 480, PixelFormat::Format32bppArgb);
				Data = Bmp->LockBits(System::Drawing::Rectangle(0, 0, Bmp->Width, Bmp->Height), ImageLockMode::WriteOnly, Bmp->PixelFormat);
			}

			void SetBytes(int idx, byte first, byte second, byte third) {
				if (ByteArray == nullptr)
					return;

				ByteArray[idx]		= first;
				ByteArray[idx + 1]	= second;
				ByteArray[idx + 2]	= third;
				ByteArray[idx + 3]	= 0xFF;
			}

			void End() {
				if (ByteArray == nullptr)
					return;

				Marshal::Copy(ByteArray, 0, Data->Scan0, 640 * 480 * 4);
				Bmp->UnlockBits(Data);
			}

			void Mirror() {
				if (Bmp == nullptr)
					return;

				Bmp->RotateFlip(RotateFlipType::RotateNoneFlipX);
			}
		};
}
