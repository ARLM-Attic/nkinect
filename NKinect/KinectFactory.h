#pragma once
#include <windows.h>
#include <SetupAPI.h>
#include "Codebase.h"
#include "CLNUIDevice.h"
#include "BaseKinect.h"
#include "CLKinect.h"
#include "MouseTracker.h"
#include "KinectException.h"

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Windows::Forms;
using namespace System::Drawing::Imaging;

namespace NKinect {
	public ref class KinectFactory abstract sealed {
		static int CreatedKinects = 0;

		private:
			static int GetInstalledCodeBase() {
				HDEVINFO hDevInfo = SetupDiGetClassDevs(0, 0, 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);
				SP_DEVINFO_DATA DeviceInfoData;
				int returnValue = Codebase::None;

				if (hDevInfo == INVALID_HANDLE_VALUE)
					return returnValue;

				DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

				for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++) {
					DWORD DataT;
					LPTSTR buffer = NULL;
					DWORD buffersize = 0;

					while (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC, &DataT, (PBYTE) buffer, buffersize, &buffersize)) {
						if (GetLastError() ==  ERROR_INSUFFICIENT_BUFFER) {
							if (buffer) 
								LocalFree(buffer);

							buffer = (LPTSTR) LocalAlloc(LPTR, buffersize * 2);
						} else
							break;
					}

					String^ str = gcnew String(buffer);

					if (str->Equals("NUI Motor"))
						returnValue = Codebase::CodeLabs;

					if (buffer) 
						LocalFree(buffer);
				}


				if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS)
					return returnValue;

				SetupDiDestroyDeviceInfoList(hDevInfo);

				return returnValue;
			}

		public:
			static BaseKinect^ GetKinect() {
				int codeBase = GetInstalledCodeBase();

				switch (codeBase) {
					case Codebase::CodeLabs:
						return gcnew CLKinect(0);
				}

				MessageBox::Show("No Kinect detected.\n\nPlease verify that it's connected and that you have CL NUI / OpenKinect installed.", "NKinect", MessageBoxButtons::OK, MessageBoxIcon::Error);
				throw gcnew KinectException("No Kinect detected. Please verify that it's connected and that you have CL NUI / OpenKinect installed.");
			}
	};
}
