// ReSharper disable CppInconsistentNaming
#pragma once
#include <fstream>
#include <array>
#include <Windows.h>
// #include "RPCInterface/mhclient.h"

#include <iostream>
#include <format>
#include "MinHook.h"
#include "sigscanner.h"
#include <iomanip>
#include <vector>

#include <iostream>
#include <Psapi.h>
#include "Memory.h"
#include "Logging.h"

void EnumerateProcessModules();
int  init();

static const char IdUsercmdGenLocalSendBtnPressFpSig[] =
	"48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 20 44 8B B1 ? ? ? ? 8B EA";


typedef __int64 (__fastcall*idUsercmdGenLocalSendBtnPressMB_t)(__int64         idUsercmdGenLocal_a1,
															   unsigned int    deviceNumMB_a2,
															   int             keyNum_t_a3,
															   unsigned __int8 isDown_a4);


__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64         idUsercmdGenLocal_a1,
														unsigned int    deviceNumMB_a2,
														int             keyNum_t_a3,
														unsigned __int8 isDown_a4);
