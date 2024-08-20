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


DWORD64 PatternScan(const char* szModule, const char* signature)
{
	HMODULE hModule = GetModuleHandleA(szModule);
	return PatternScan(hModule, signature);
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

DWORD64 PatternScan(HMODULE hModule, const char* signature)
{
	MODULEINFO mInfo;

	if (hModule == nullptr) {
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
			return result;
		}
	}

	return 0;
}



__int64 FindPtrFromRelativeOffset(uintptr_t instructionStartAddress, const int instructionOffset, const int nextInstructionOffset) {

	g_logfile << "FindPtrFromRelativeOffset: " << std::hex << instructionStartAddress << "\n";
	
	__int64 relativeOffsetAddr = instructionStartAddress + instructionOffset;
	HANDLE hProcess = GetCurrentProcess();
	uint8_t buffer[4]; //! reading 4 cause it's an 4 bytes offset.	
	SIZE_T bytesRead;
	//! int and not uint as the offset has to be signed
	int relativeOffsetValue = 0;
	if (ReadProcessMemory(hProcess, (LPCVOID)relativeOffsetAddr, buffer, sizeof(buffer), &bytesRead)) {
		if (bytesRead == sizeof(buffer)) {			
			int relativeOffsetValue = *reinterpret_cast<int*>(buffer);			
			__int64 nextInstructionAddress = instructionStartAddress + nextInstructionOffset;			
			__int64 ptr = nextInstructionAddress + relativeOffsetValue;
			g_logfile << "FindPtrFromRelativeOffset: found ptr: " << std::hex << ptr << "\n";
			return ptr;
		}
	}
	else {
		g_logfile << "FindPtrFromRelativeOffset: failed for instructionStartAddress:  " << instructionStartAddress << "\n";

	}
	return 0;
}




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
				g_logfile << szModuleName << std::endl;
			}
		}
	}
}
