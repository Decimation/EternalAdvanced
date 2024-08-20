// ReSharper disable CppInconsistentNaming
#include "dllmain.hpp"

idUsercmdGenLocalSendBtnPressMB_t p_idUsercmdGenLocalSendBtnPressMB_t        = nullptr;
idUsercmdGenLocalSendBtnPressMB_t p_idUsercmdGenLocalSendBtnPressMB_t_Target = nullptr;

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

__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64      idUsercmdGenLocal_a1, unsigned int deviceNumMB_a2,
                                                        id::keyNum_t keyNum_t_a3, unsigned __int8       isDown_a4)
{
	static int             lastKeyNum_t_a3 = -1;
	static unsigned __int8 lastIsDown_a4   = 0;

	//todo this works, not using it atm but we might use it in the incoming rewrite of the mod
	/*if (keyNum_t_a3 != lastKeyNum_t_a3) {
		logInfo("keyNum_t_a3 has changed to: 0x%X (isDown_a4: %d)", keyNum_t_a3, isDown_a4);
		lastKeyNum_t_a3 = keyNum_t_a3;
	}

	if (isDown_a4 != lastIsDown_a4) {
		logInfo("isDown_a4 has changed to: %d",  isDown_a4);
		lastIsDown_a4 = isDown_a4;
	}*/


	auto rv = p_idUsercmdGenLocalSendBtnPressMB_t(idUsercmdGenLocal_a1, deviceNumMB_a2, keyNum_t_a3, isDown_a4);


	auto x =
		std::format("{} {} {} {} -> {}", idUsercmdGenLocal_a1, deviceNumMB_a2, (int) keyNum_t_a3, isDown_a4, rv);
	g_logfile << x << '\n';
	g_logfile.flush();

	return rv;
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

	p_idUsercmdGenLocalSendBtnPressMB_t_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(
		PatternScan(id::DE_EXE_MODULE, IdUsercmdGenLocalSendBtnPressFpSig));

	g_logfile << "@" << std::hex << p_idUsercmdGenLocalSendBtnPressMB_t_Target << "\n";

	if (MH_Initialize() != MH_OK) {
		ret = 1;
		goto ret;
	}

	for (auto ix = 0; ix < 8; ix++)
		g_logfile << "byte: " << std::hex <<
			(int) reinterpret_cast<uint8_t*>(p_idUsercmdGenLocalSendBtnPressMB_t_Target)[ix] << " ";

	// p_idUsercmdGenLocalSendBtnPressMB_t_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(p_idOrigFunc);

	// Create a hook for MessageBoxW, in disabled state.
	if (MH_CreateHook(p_idUsercmdGenLocalSendBtnPressMB_t_Target, &idUsercmdGenLocalSendBtnPressMB_Hook,
	                  reinterpret_cast<void**>(&p_idUsercmdGenLocalSendBtnPressMB_t))) {
		g_logfile << "failed to create hook";
		ret = 1;
		goto ret;
	}

	// Enable the hook for MessageBoxW.
	if (MH_EnableHook(NULL) != MH_OK) {
		g_logfile << "failed to enable hook";
		ret = 1;
		goto ret;
	}
ret:
	return ret;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	//Initialize MinHook.


	switch (ul_reason_for_call) {
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_ATTACH:
		init_log();

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
