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

#define DEFINE_FUNCTION_POINTER4(function) \
	DEFINE_FUNCTION_POINTER3(function, function##_t)

DWORD64           PatternScan(const char* szModule, const char* signature);
DWORD64           PatternScan(HMODULE hModule, const char* signature);
std::vector<char> PatternToByte(const char* pattern);

std::string GetCallingModuleName();
void        EnumerateProcessModules();


// General template
template <typename Ret, bool Void = std::is_void<Ret>::value, typename... Args>
class Fnc
{
	Ret (&fnc_)(Args...);

public:
	Fnc(Ret (&fnc)(Args...)) : fnc_(fnc)
	{
	}

	Ret operator()(Args... args) { return fnc_(args...); }
};

// Void return specialized template
template <typename Ret, typename... Args>
class Fnc<Ret, true, Args...>
{
	void (&fnc_)(Args...);

public:
	Fnc(void (&fnc)(Args...)) : fnc_(fnc)
	{
	}

	void operator()(Args... args) { (fnc_)(args...); }
};

// Fnc helper function
template <class Ret, bool Void = std::is_void<Ret>::value, class... Args>
const Fnc<Ret, Void, Args...> make_fnc(Ret (&fnc)(Args...))
{
	return Fnc<Ret, Void, Args...>(fnc);
}
