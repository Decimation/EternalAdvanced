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
#include "id.h"
#include <functional>

bool Init();


/*__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64      idUsercmdGenLocal_a1, unsigned int deviceNumMB_a2,
                                                        id::keyNum_t keyNum_t_a3, unsigned __int8       isDown_a4);

DEFINE_FUNCTION_POINTER4(idUsercmdGenLocalSendBtnPressMB_Hook)*/

/*typedef __int64 (__fastcall*idUsercmdGenLocalSendBtnPressMB_t)(__int64         idUsercmdGenLocal_a1,
                                                               unsigned int    deviceNumMB_a2,
                                                               int             keyNum_t_a3,
                                                               unsigned __int8 isDown_a4);


__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64         idUsercmdGenLocal_a1,
                                                        unsigned int    deviceNumMB_a2,
                                                        int             keyNum_t_a3,
                                                        unsigned __int8 isDown_a4);*/
