// ReSharper disable CppInconsistentNaming
#include "dllmain.hpp"


HMODULE                           baseAddr;
std::ofstream                     logfile;
idUsercmdGenLocalSendBtnPressMB_t p_idUsercmdGenLocalSendBtnPressMB_t        = nullptr;
idUsercmdGenLocalSendBtnPressMB_t p_idUsercmdGenLocalSendBtnPressMB_t_Target = nullptr;
void*                             p_idOrigFunc                               = nullptr;

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


__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64 idUsercmdGenLocal_a1, unsigned int deviceNumMB_a2,
														int     keyNum_t_a3, unsigned __int8       isDown_a4)
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
	logfile << std::format("{} {} {} {} -> {}", idUsercmdGenLocal_a1, deviceNumMB_a2, keyNum_t_a3, isDown_a4, rv) <<
		'\n';
	logfile.flush();

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

/*
 *
 * [--------------]				[--------------]			[--------------]
 *		PROXY				<---		ORIG			->>>			ADDR SPACE
 * [--------------]				[--------------]			[--------------]
 *
 */


void EnumerateProcessModules()
{
	// Get the handle to the current process
	HANDLE hProcess = GetCurrentProcess();

	// Get the list of module handles for the current process
	HMODULE hModules[1024];
	DWORD   cbNeeded;
	if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
		// Calculate the number of modules
		int numModules = cbNeeded / sizeof(HMODULE);

		// Iterate through the modules
		for (int i = 0; i < numModules; i++) {
			// Get the module file name
			TCHAR szModuleName[MAX_PATH];
			if (GetModuleFileNameEx(hProcess, hModules[i], szModuleName, sizeof(szModuleName) / sizeof(TCHAR))) {
				// Print the module file name
				logfile << szModuleName << std::endl;
			}
		}
	}
}


// Define the pattern_to_byte lambda outside of PatternScan
static auto pattern_to_byte = [](const char* pattern)
{
	auto bytes = std::vector<char>{};
	auto start = const_cast<char*>(pattern);
	auto end   = const_cast<char*>(pattern) + strlen(pattern);

	for (auto current = start; current < end; ++current) {
		if (*current == '?') {
			++current;
			if (*current == '?')
				++current;
			bytes.push_back('\?');
		} else {
			bytes.push_back(strtoul(current, &current, 16));
		}
	}
	return bytes;
};

DWORD64 PatternScan(const char* szModule, const char* signature)
{
	logfile << ("PatternScan") << '\n';

	MODULEINFO mInfo;
	HMODULE    hModule = GetModuleHandleA(szModule);

	if (hModule == nullptr) {
		logfile << std::format("GetModuleHandleA failed to find the module: %s returning", szModule) << '\n';
		return 0; // Module not found, return an appropriate value (0 in this case).
	}

	K32GetModuleInformation(GetCurrentProcess(), hModule, &mInfo, sizeof(MODULEINFO));
	DWORD64 base         = (DWORD64) mInfo.lpBaseOfDll;
	DWORD64 sizeOfImage  = (DWORD64) mInfo.SizeOfImage;
	auto    patternBytes = pattern_to_byte(signature);

	DWORD64 patternLength = patternBytes.size();
	auto    data          = patternBytes.data();

	for (DWORD64 i = 0; i < sizeOfImage - patternLength; i++) {
		bool found = true;
		for (DWORD64 j = 0; j < patternLength; j++) {
			char a = '\?';
			char b = *(char*) (base + i + j);
			found &= data[j] == a || data[j] == b;
		}
		if (found) {
			DWORD64 result = base + i;
			logfile << std::format("PatternScan Success. Found Addr: {}", (void*) result) << '\n';
			return result;
		}
	}

	logfile << std::format("!!! PatternScan failed !!!") << '\n';
	return 0;
}

DWORD64 ModulePatternScan(std::string scanFriendlyName, const char* signature)
{
	logfile << ("ModulePatternScan") << '\n';

	MODULEINFO mInfo;
	HMODULE    hModule = GetModuleHandleA(DE_EXE_NAME.c_str());

	if (hModule == nullptr) {
		logfile << std::format("GetModuleHandleA failed to find the module: %s returning", DE_EXE_NAME.c_str()) << '\n';
		return 0; // Module not found, return an appropriate value (0 in this case).
	}

	K32GetModuleInformation(GetCurrentProcess(), hModule, &mInfo, sizeof(MODULEINFO));
	DWORD64 base         = (DWORD64) mInfo.lpBaseOfDll;
	DWORD64 sizeOfImage  = (DWORD64) mInfo.SizeOfImage;
	auto    patternBytes = pattern_to_byte(signature);

	DWORD64 patternLength = patternBytes.size();
	auto    data          = patternBytes.data();

	for (DWORD64 i = 0; i < sizeOfImage - patternLength; i++) {
		bool found = true;
		for (DWORD64 j = 0; j < patternLength; j++) {
			char a = '\?';
			char b = *(char*) (base + i + j);
			found &= data[j] == a || data[j] == b;
		}
		if (found) {
			DWORD64 result = base + i;
			logfile << std::format("PatternScan Success for {} Found Addr: {}", scanFriendlyName.c_str(),
								   (void*) result) << '\n';
			return result;
		}
	}

	logfile << std::format("!!! PatternScan failed for: {}", scanFriendlyName.c_str()) << '\n';
	return 0;
}

int init()
{
	int ret = 0;

	baseAddr = GetModuleHandle(nullptr);
	logfile << "@" << std::hex << baseAddr << "\n";

	p_idUsercmdGenLocalSendBtnPressMB_t_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(
		ModulePatternScan("test", IdUsercmdGenLocalSendBtnPressFpSig));

	logfile << "@" << std::hex << p_idUsercmdGenLocalSendBtnPressMB_t_Target << "\n";

	if (MH_Initialize() != MH_OK) {
		ret = 1;
		goto ret;
	}

	for (auto ix = 0; ix < 8; ix++)
		logfile << "byte: " << std::hex << (int) reinterpret_cast<uint8_t*>(p_idUsercmdGenLocalSendBtnPressMB_t_Target)[
			ix] << " ";

	// p_idUsercmdGenLocalSendBtnPressMB_t_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(p_idOrigFunc);

	// Create a hook for MessageBoxW, in disabled state.
	if (MH_CreateHook(reinterpret_cast<void**>(p_idUsercmdGenLocalSendBtnPressMB_t_Target),
					  &idUsercmdGenLocalSendBtnPressMB_Hook,
					  reinterpret_cast<void**>(&p_idUsercmdGenLocalSendBtnPressMB_t))) {
		logfile << "failed to create hook";
		ret = 1;
		goto ret;
	}

	// or you can use the new helper function like this.
	//if (MH_CreateHookApiEx(
	//    L"user32", "MessageBoxW", &DetourMessageBoxW, &fpMessageBoxW) != MH_OK)
	//{
	//    return 1;
	//}

	// Enable the hook for MessageBoxW.
	if (MH_EnableHook(NULL) != MH_OK) {
		logfile << "failed to enable hook";
		ret = 1;
		goto ret;
	}
ret:
	return ret;
}

std::string GetCallingModuleName()
{
	HMODULE hModule = GetModuleHandle(nullptr);
	TCHAR   szModuleName[MAX_PATH];

	if (GetModuleFileName(hModule, szModuleName, MAX_PATH) != 0) {
		std::string moduleName(szModuleName);
		return moduleName;
	}
	return "";
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	//Initialize MinHook.

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		logfile = std::ofstream("log.txt", std::ios::app);

		logfile << "Loaded" << '\n';
		if (GetCallingModuleName().ends_with("idTechLauncher.exe")) {
			logfile << "Exiting" << '\n';
			return 1;
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

		logfile << "Detaching" << '\n';
		FreeLibrary(msimg32.dll);
		logfile.flush();
		logfile.close();
		break;
	}

ret:
	logfile << "DllMain return" << '\n';

	return TRUE;
}
