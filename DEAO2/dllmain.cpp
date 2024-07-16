#include <Windows.h>

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
FARPROC PA = 0;
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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		char path[MAX_PATH];
		GetWindowsDirectory(path, sizeof(path));

	// Example: "\\System32\\version.dll"
		strcat_s(path, "\\add manual path\\msimg32.dll");
		msimg32.dll = LoadLibrary(path);
		setupFunctions();

	// Add here your code, I recommend you to create a thread
		break;
	case DLL_PROCESS_DETACH:
		FreeLibrary(msimg32.dll);
		break;
	}
	return 1;
}
