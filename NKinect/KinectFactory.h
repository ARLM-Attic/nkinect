#pragma once
#include <windows.h>
#include <SetupAPI.h>
#include "Codebase.h"
#include "CLNUIDevice.h"
#include "BaseKinect.h"
#include "CLKinect.h"
#include "MouseTracker.h"
#include "KinectException.h"
#include "KinectCalibrator.h"
#include "PrimeSensor.h"

using namespace System;
using namespace System::IO;
using namespace System::Text;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Drawing::Imaging;
using namespace System::Diagnostics;

namespace NKinect {
	public ref class KinectFactory abstract sealed {
		static int CreatedKinects = 0;

		private:
			static String^ ExecutableDir = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);

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

					if (str->Equals("NUI Motor")) {
						returnValue = Codebase::CodeLabs;

						Debug::WriteLine("Using CodeLaboratories SDK");
					} else if (str->Equals("PrimeSensor Development Kit 5.x")) {
						returnValue = Codebase::OpenNi;

						Debug::WriteLine("Using OpenNI SDK");
					}

					if (buffer) 
						LocalFree(buffer);
				}


				if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS)
					return returnValue;

				SetupDiDestroyDeviceInfoList(hDevInfo);

				return returnValue;
			}

 			static bool RequiredDLLsFound() {
				bool clNuiFound = File::Exists(Path::Combine(ExecutableDir, "CLNUIDevice.dll"));

				return clNuiFound;
			}
		public:
			static BaseKinect^ GetKinect() {
				int codeBase = GetInstalledCodeBase();

				if (!RequiredDLLsFound())
					throw KinectException::RequiredDllsNotFound;

				switch (codeBase) {
					case Codebase::CodeLabs:
						return gcnew CLKinect(0);
					case Codebase::OpenNi:
						return gcnew PrimeSensor();
				}

				throw KinectException::NotFound;
			}
	};
}
