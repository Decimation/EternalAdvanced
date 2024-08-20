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

int init();

DEFINE_FUNCTION_POINTER2(__int64, idUsercmdGenLocalSendBtnPressMB,
                         __int64 idUsercmdGenLocal_a1,
                         unsigned int deviceNumMB_a2,
                         id::keyNum_t keyNum_t_a3,
                         unsigned __int8 isDown_a4)

static const char IdUsercmdGenLocalSendBtnPressFpSig[] =
	"48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC 20 44 8B B1 ? ? ? ? 8B EA";


DEFINE_FUNCTION_POINTER2(char, RenderSprite,
    void* idSwf_a1,
    __int64 a2,
    void* spriteInstancePtr_a3,
    _int64 a4,
    unsigned int a5,
    char a6)

    static const char RenderSpriteFpSig[] = "40 55 56 57 41 55 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 89 4D 00";


 DEFINE_FUNCTION_POINTER2(void*, idPlayerFovLerp, __int64 idPlayer_a1, float lerpFOV_a2, float a3, char a4)
 
     static const char idPlayerFovLerpFpSig[] = "40 53 48 83 EC 20 48 8B 05 ? ? ? ? 48 8B D9 83 78 08 5A";


 DEFINE_FUNCTION_POINTER2(void, StartSync, __int64 idSyncEntity_a1, __int64 idDeclAnimWebPTR_a2, __int64 idPlayer_a3, float a4, __int64 a5)

     static const char StartSyncFpSig[] = "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 44 0F 29 8C 24";


 DEFINE_FUNCTION_POINTER2(char, SyncEnd, __int64 a1, __int64 a2, int a3, unsigned int a4, unsigned __int8 a5)

    static const char syncEndFpSig[] = "48 8B C4 57 41 55 41 56";


 DEFINE_FUNCTION_POINTER2(char, customAnimSmth, __int64 idHands_a1, const char* str_a2, int a3, int a4, int a5)
     static const char skipCustomAnimFpSig[] = "44 89 4C 24 ? 44 89 44 24 ? 48 89 54 24 ? 53 55";


 //__int64 __fastcall idHUDMenu_CurrencyConfirmation_t_Hook(__int64 idHUDMenu_CurrencyConfirmation_a1, __int64 struct_a2)
 DEFINE_FUNCTION_POINTER2(__int64, idHUDMenu_CurrencyConfirmation, __int64 idHUDMenu_CurrencyConfirmation_a1, __int64 struct_a2)
     static const char idHUDMenu_CurrencyConfirmationSmthFpSig[] = "4C 8B DC 55 49 8D AB ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 89 5B 18 49 89 73 F0 48 8B F1";


 DEFINE_FUNCTION_POINTER2(__int64, idMenu_Update, __int64 idMenu, __int64 a2)
     static const char idMenu_UpdateSig[] = "40 55 56 41 56 48 8B EC 48 83 EC 50 4C 8B F2";


 //float(__fastcall* getFovTargetValMB)(idWeapon* idWeaponAddr_a1);
 DEFINE_FUNCTION_POINTER2(float, getFovTargetVal, void* idWeaponPtr_a1)
     static const char GetFovTargetValSig[] = "48 83 EC 38 48 83 B9 58 01 00 00 00";


 //typedef char(__fastcall* SelectWeaponForSelectionGroup)(__int64 a1, int a2);
 DEFINE_FUNCTION_POINTER2(char, SelectWeaponForSelectionGroup, __int64 a1, int a2)
     static const char idPLayerSelectWeaponForSelectionGroupSig[] = "89 54 24 10 55 41 54 41 56 48 83 EC 60";


 


/*typedef __int64 (__fastcall*idUsercmdGenLocalSendBtnPressMB_t)(__int64         idUsercmdGenLocal_a1,
                                                               unsigned int    deviceNumMB_a2,
                                                               int             keyNum_t_a3,
                                                               unsigned __int8 isDown_a4);


__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64         idUsercmdGenLocal_a1,
                                                        unsigned int    deviceNumMB_a2,
                                                        int             keyNum_t_a3,
                                                        unsigned __int8 isDown_a4);*/

