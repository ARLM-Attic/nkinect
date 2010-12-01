#pragma once
using namespace System;

namespace NKinect {
	public ref class DepthEventArgs : public EventArgs  {
		public:
			property array<array<DOUBLE>^>^ Depths;

			DepthEventArgs(array<array<DOUBLE>^>^ values) {
				Depths = gcnew array<array<double>^>(640);
			
				for (int i = 0; i < 640; i++) {
					Depths[i] = gcnew array<double>(480);
			
					for (int j = 0; j < 480; j++)
						Depths[i][j] = values[i][j];
				}
			}
	};
}
