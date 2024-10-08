// ReSharper disable CppInconsistentNaming
#include "dllmain.hpp"

#include "idHooks.h"


#pragma region Proxy
struct msimg32_dll
{
	HMODULE dll;
	FARPROC oAlphaBlend;
	FARPROC oDllInitialize;
	FARPROC oGradientFill;
	FARPROC oTransparentBlt;
	FARPROC ovSetDdrawflag;
} msimg32;

extern "C" {
	FARPROC PA = nullptr;
	int     runASM();

	void fAlphaBlend()
	{
		PA = msimg32.oAlphaBlend;
		runASM();
	}

	void fDllInitialize()
	{
		PA = msimg32.oDllInitialize;
		runASM();
	}

	void fGradientFill()
	{
		PA = msimg32.oGradientFill;
		runASM();
	}

	void fTransparentBlt()
	{
		PA = msimg32.oTransparentBlt;
		runASM();
	}

	void fvSetDdrawflag()
	{
		PA = msimg32.ovSetDdrawflag;
		runASM();
	}
}

void setupFunctions()
{
	msimg32.oAlphaBlend     = GetProcAddress(msimg32.dll, "AlphaBlend");
	msimg32.oDllInitialize  = GetProcAddress(msimg32.dll, "DllInitialize");
	msimg32.oGradientFill   = GetProcAddress(msimg32.dll, "GradientFill");
	msimg32.oTransparentBlt = GetProcAddress(msimg32.dll, "TransparentBlt");
	msimg32.ovSetDdrawflag  = GetProcAddress(msimg32.dll, "vSetDdrawflag");
}
#pragma endregion


bool Init()
{
	bool ret = true;
	if (!id::InitId()) {
	}
	if (!ea::InitHooks()) {
	}

ret:
	return ret;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	//Initialize MinHook.


	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			InitLog();
		// DisableThreadLibraryCalls(hModule);
			g_logfile << std::format("DllMain {}", (void*) hModule) << '\n';
			g_logfile << "Dll attach" << '\n';
			if (ea::GetCallingModuleName().ends_with("idTechLauncher.exe")) {
				g_logfile << "Exiting" << '\n';
				FreeLibrary(msimg32.dll);
				CloseLog();
				return TRUE;
			}

			char path[MAX_PATH];
			GetWindowsDirectory(path, sizeof(path));

		// Example: "\\System32\\version.dll"
			strcat_s(path, "\\System32\\msimg32.dll");
			msimg32.dll = LoadLibrary(path);
			setupFunctions();
			Init();
		//CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Initialise), (PVOID)hModule, NULL, NULL);

		// Add here your code, I recommend you to create a thread
			break;
		case DLL_PROCESS_DETACH:

			g_logfile << "Detaching" << '\n';
			FreeLibrary(msimg32.dll);
			CloseLog();
			break;
	}

ret:
	// g_logfile << "DllMain return" << '\n';

	return TRUE;
}
