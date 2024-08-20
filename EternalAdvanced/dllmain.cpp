// ReSharper disable CppInconsistentNaming
#include "dllmain.hpp"

#include "idHooks.h"


template <typename T>
inline MH_STATUS MH_CreateHookEx(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

template <typename T>
inline MH_STATUS MH_CreateHookApiEx(
	LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHookApi(pszModule, pszProcName, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}


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


int init()
{
	int ret = 0;

	id::init();

	inithooks();
ret:
	return ret;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	//Initialize MinHook.


	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		init_log();
	// DisableThreadLibraryCalls(hModule);
		g_logfile << std::format("DllMain {}", (void*) hModule) << '\n';
		g_logfile << "Dll attach" << '\n';
		if (GetCallingModuleName().ends_with("idTechLauncher.exe")) {
			g_logfile << "Exiting" << '\n';
			FreeLibrary(msimg32.dll);
			close_log();
			return TRUE;
		}

		char path[MAX_PATH];
		GetWindowsDirectory(path, sizeof(path));

	// Example: "\\System32\\version.dll"
		strcat_s(path, "\\System32\\msimg32.dll");
		msimg32.dll = LoadLibrary(path);
		setupFunctions();
		init();
	//CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Initialise), (PVOID)hModule, NULL, NULL);

	// Add here your code, I recommend you to create a thread
		break;
	case DLL_PROCESS_DETACH:

		g_logfile << "Detaching" << '\n';
		FreeLibrary(msimg32.dll);
		close_log();
		break;
	}

ret:
	// g_logfile << "DllMain return" << '\n';

	return TRUE;
}
