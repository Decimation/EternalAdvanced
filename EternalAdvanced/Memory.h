// Author: Deci | Project: DEAO2 | Name: Memory.h
// Date: 2024/08/19 @ 12:08:05

#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <format>
#include "Logging.h"
#include <vector>
#include "ProcessModule.h"

#define DEFINE_FUNCTION_POINTER(returnType, functionName, ...) \
	typedef returnType(*functionName##_t)(__VA_ARGS__);

/*#define DEFINE_FUNCTION_POINTER2(returnType, functionName, ...) \
	DEFINE_FUNCTION_POINTER(returnType, functionName, __VA_ARGS__) \
	returnType functionName##_Hook(__VA_ARGS__);*/

#define DECLARE_FTYPE(functionName) \
	functionName##_t p_##functionName;		\
	functionName##_t p_##functionName##_Target;

#define DEFINE_FUNCTION_POINTER2(returnType, functionName, ...) \
	DEFINE_FUNCTION_POINTER(returnType, functionName, __VA_ARGS__) \
	returnType functionName##_Hook(__VA_ARGS__); \
	inline functionName##_t p_##functionName;		\
	inline functionName##_t p_##functionName##_Target;

/*#define DEFINE_FUNCTION_POINTER2(returnType, functionName, ...) \
	DEFINE_FUNCTION_POINTER(returnType, functionName, __VA_ARGS__) \
	returnType functionName##_Hook(__VA_ARGS__); \
	functionName##_t p_##functionName; \
	functionName##_t p_##functionName##_Target*/


/*#define DEFINE_FUNCTION_POINTER3(function, name) \
	typedef decltype(&(function)) (name); \
	name p_##name;		\
	name p_##name##_Target;*/

namespace ea
{
DWORD64           PatternScan(HMODULE hModule, const char* signature);
DWORD64           PatternScan(const char* szModule, const char* signature);
std::vector<char> PatternToByte(const char* pattern);

std::string                GetCallingModuleName();
std::vector<ProcessModule> EnumerateProcessModules();
__int64 FindPtrFromRelativeOffset(uintptr_t instructionStartAddress, const int instructionOffset,
								  const int nextInstructionOffset);
}
