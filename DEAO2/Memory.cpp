#include "Memory.h"

std::vector<char> PatternToByte(const char* pattern)
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

DWORD64 ModulePatternScan(std::string scanFriendlyName, const char* signature)
{
	g_logfile << ("ModulePatternScan") << '\n';

	MODULEINFO mInfo;
	HMODULE    hModule = GetModuleHandleA(DE_EXE_NAME.c_str());

	if (hModule == nullptr) {
		g_logfile << std::format("GetModuleHandleA failed to find the module: %s returning", DE_EXE_NAME.c_str()) << '\n';
		return 0; // Module not found, return an appropriate value (0 in this case).
	}

	K32GetModuleInformation(GetCurrentProcess(), hModule, &mInfo, sizeof(MODULEINFO));
	DWORD64 base         = (DWORD64) mInfo.lpBaseOfDll;
	DWORD64 sizeOfImage  = (DWORD64) mInfo.SizeOfImage;
	auto    patternBytes = PatternToByte(signature);

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
			g_logfile << std::format("PatternScan Success for {} Found Addr: {}", scanFriendlyName.c_str(),
								   (void*) result) << '\n';
			return result;
		}
	}

	g_logfile << std::format("!!! PatternScan failed for: {}", scanFriendlyName.c_str()) << '\n';
	return 0;
}

DWORD64 PatternScan(const char* szModule, const char* signature)
{
	g_logfile << ("PatternScan") << '\n';

	MODULEINFO mInfo;
	HMODULE    hModule = GetModuleHandleA(szModule);

	if (hModule == nullptr) {
		g_logfile << std::format("GetModuleHandleA failed to find the module: %s returning", szModule) << '\n';
		return 0; // Module not found, return an appropriate value (0 in this case).
	}

	K32GetModuleInformation(GetCurrentProcess(), hModule, &mInfo, sizeof(MODULEINFO));
	DWORD64 base         = (DWORD64) mInfo.lpBaseOfDll;
	DWORD64 sizeOfImage  = (DWORD64) mInfo.SizeOfImage;
	auto    patternBytes = PatternToByte(signature);

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
			g_logfile << std::format("PatternScan Success. Found Addr: {}", (void*) result) << '\n';
			return result;
		}
	}

	g_logfile << std::format("!!! PatternScan failed !!!") << '\n';
	return 0;
}
