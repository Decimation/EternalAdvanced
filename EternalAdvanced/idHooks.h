// Author: Deci | Project: EternalAdvanced | Name: idHooks.h
// Date: 2024/08/20 @ 10:08:19

// ReSharper disable CppClangTidyClangDiagnosticMicrosoftCast
// ReSharper disable CppClangTidyPerformanceNoIntToPtr
// ReSharper disable CppInconsistentNaming
#pragma once
#include <map>

#include "Memory.h"
#include "Logging.h"
#include "id.h"
#include "MinHook.h"

namespace ea
{
DEFINE_FUNCTION_POINTER2(__int64, idUsercmdGenLocalSendBtnPressMB,
                         __int64 idUsercmdGenLocal_a1,
                         unsigned int deviceNumMB_a2,
                         id::keyNum_t keyNum_t_a3,
                         unsigned __int8 isDown_a4);

static const char IdUsercmdGenLocalSendBtnPressFpSig[] =
	"48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 20 44 8B B1 ? ? ? ? 8B EA";


inline bool InitHooks()
{
	bool ret1                                = false;
	p_idUsercmdGenLocalSendBtnPressMB_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(
		PatternScan(id::DE_EXE_MODULE, IdUsercmdGenLocalSendBtnPressFpSig));

	g_logfile << "@" << std::hex << p_idUsercmdGenLocalSendBtnPressMB_Target << "\n";

	if (MH_Initialize() != MH_OK) {
		ret1 = false;
		goto ret;
	}

	// g_x.init();

	/*for (auto ix = 0; ix < 8; ix++)
		g_logfile << "byte: " << std::hex <<
			(int) reinterpret_cast<uint8_t*>(p_idUsercmdGenLocalSendBtnPressMB_t_Target)[ix] << " ";
			*/

	// p_idUsercmdGenLocalSendBtnPressMB_t_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(p_idOrigFunc);

	// Create a hook for MessageBoxW, in disabled state.
	if (MH_CreateHook(p_idUsercmdGenLocalSendBtnPressMB_Target, &idUsercmdGenLocalSendBtnPressMB_Hook,
	                  reinterpret_cast<void**>(&p_idUsercmdGenLocalSendBtnPressMB))) {
		g_logfile << "failed to create hook";
		ret1 = false;
		goto ret;
	}

	// Enable the hook for MessageBoxW.
	if (MH_EnableHook(nullptr) != MH_OK) {
		g_logfile << "failed to enable hook";
		ret1 = false;
		goto ret;
	}

ret:
	return ret1;
}


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


/*__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64      idUsercmdGenLocal_a1, unsigned int deviceNumMB_a2,
														id::keyNum_t keyNum_t_a3, unsigned __int8       isDown_a4);*/


/*
template <class TFunction, TFunction* f>
class my_class
{
public:
	const char* sig;
	TFunction*   orig;
	TFunction*   hook;
	TFunction*   target;

	my_class(const char* sig)
		: sig(sig),
		  hook(f)
	{
	}

	bool init()
	{
		target = reinterpret_cast<TFunction*>(PatternScan(id::DE_EXE_MODULE, sig));
		g_logfile << std::format("{} {} {} -> {}", (void*) orig, (void*) hook, (void*) target,
								 typeid(TFunction*).name());
		if (MH_CreateHook(target, &hook,
						  reinterpret_cast<void**>(&orig))) {
			g_logfile << "failed to create hook";
			return false;
		}
		return true;
	}
};

inline my_class<decltype(idUsercmdGenLocalSendBtnPressMB_Hook), idUsercmdGenLocalSendBtnPressMB_Hook> g_x{
	IdUsercmdGenLocalSendBtnPressFpSig

};
*/
}
