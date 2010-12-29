#pragma once
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

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

			void CopyCvImage(IplImage* image) {
				for (int i = 0; i < image->imageSize; i += 3)
					SetBytes(i, image->imageData[i], image->imageData[i + 1], image->imageData[i + 2]);
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

				for (int row = 0; row < 480; row++)
					for (int col = 0; col < 320; col++)
						SwitchTriplet(row, col, 639 - col);
			}

		private:
			void SwitchTriplet(int row, int src, int dest) {
				int srcIndex = row * 1920 + (src * 3);
				int dstIndex = row * 1920 + (dest * 3);

				SwapByte(srcIndex, dstIndex);
				SwapByte(srcIndex + 1, dstIndex + 1);
				SwapByte(srcIndex + 2, dstIndex + 2);
			}

			void SwapByte(int src, int dest) {
				USHORT temp			= NativeArray[dest];

				NativeArray[dest]	= NativeArray[src];
				NativeArray[src]	= temp;
			}

		};
}
