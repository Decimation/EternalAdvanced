// Author: Deci | Project: DEAO2 | Name: Memory.h
// Date: 2024/08/19 @ 12:08:05

#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <format>
#include "Logging.h"
#include <vector>

#define DEFINE_FUNCTION_POINTER(returnType, functionName, ...) \
	typedef returnType(*functionName##_t)(__VA_ARGS__);

#define DEFINE_FUNCTION_POINTER2(returnType, functionName, ...) \
	DEFINE_FUNCTION_POINTER(returnType, functionName, __VA_ARGS__) \
	returnType functionName##_Hook(__VA_ARGS__);

DWORD64           PatternScan(const char* szModule, const char* signature);
DWORD64           PatternScan(HMODULE hModule, const char* signature);
std::vector<char> PatternToByte(const char* pattern);

std::string GetCallingModuleName();
void        EnumerateProcessModules();

