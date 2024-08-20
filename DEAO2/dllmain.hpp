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


 //bool __fastcall isKeyPressedHook(__int64 ptr, __int64 btnEnum)
 DEFINE_FUNCTION_POINTER2(bool, isKeyPressed, __int64 ptr, __int64 btnEnum)
     static const char isKeyPressedSig[] = "33 C0 44 8B C0 38 81 A1 00 00 00 75 07 4C 8B 81 A8 00 00 00 4C 85 C2 74 11";


 //typedef __int64(__fastcall* BindsStrSet)(__int64* a1, unsigned char* a2);
 DEFINE_FUNCTION_POINTER2(__int64, BindsStrSet, __int64* a1, unsigned char* a2)
     static const char pBindsStrSetSig[] = "48 89 5C 24 18 48 89 74 24 20 41 56 48 83 EC 50 0F B6 02";


 //typedef void(__fastcall* idHUD_Reticle_SetActiveReticle)(idHUD_Reticle* a1, unsigned int a2, idDeclWeaponReticle* a3, unsigned __int8 a4);
 DEFINE_FUNCTION_POINTER2(void, idHUD_Reticle_SetActiveReticle, void* idHUD_ReticlePtr_a1, unsigned int a2, void* idDeclWeaponReticlePtr_a3, unsigned __int8 a4)
     static const char setActiveReticleSig[] = "41 54 41 55 41 57 48 81 EC D0 02 00 00 48 8B 05";


 //typedef idColor* (__fastcall* convertIdDeclUIColorToidColor)(__int64 idDeclUIColor_a1, idColor* idColor_a2, int colorId_a3);
 DEFINE_FUNCTION_POINTER2(void*, convertIdDeclUIColorToidColor, __int64 idDeclUIColor_a1, void* idColor_a2, int colorId_a3)
     static const char convertIdDeclUIColorToidColorTargetSig[] = "41 81 F8 0C 01 00 00";


 //typedef void(__fastcall* setSpriteInstanceColor)(__int64 idSWFSpriteInstance_a1, unsigned int namedColorId_a2);
 DEFINE_FUNCTION_POINTER2(void, setSpriteInstanceColor, __int64 idSWFSpriteInstance_a1, unsigned int namedColorId_a2)
    static const char setSpriteInstanceSig[] = "48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 8B F2 48 8B F9";


 DEFINE_FUNCTION_POINTER2(__int64, printOutlinedString, __int64 idRenderModelGui_a1PtrToPtr, int a2, __int64 a3, int a4, __int64 a5, char* a6, int a7, int a8)
     static const char printOutlinedStringMBSig[] = "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 41 56 41 57 48 81 EC ? ? ? ? F3 0F 10 05 ? ? ? ? 4C 8B F9";


 //!!!!!!!  Im not sure how to handle this one as the calling convention is different so i'd rather let you know. BUT WE NEED IT: !!!!!!!!!!!!!!!!!!!!!!!!

 static const char idLibPrintfSig[] = "48 89 4C 24 08 48 89 54 24 10 4C 89 44 24 18 4C 89 4C 24 20 48 83 EC 28 BA 01 00 00 00";

 ////! __int64 idLib::Printf_35F240(const char *a1, ...)
 //typedef void(__cdecl* IdLib_Printf)(const char* a1, ...);
 //IdLib_Printf  pIdLib_Printf = nullptr;
 //IdLib_Printf  pIdLib_PrintfTarget;


 //void __cdecl IdLib_PrintfHook(const char* format, ...) {

 //    va_list args;
 //    va_start(args, format);

 //    char buffer[2048];
 //    int result = vsnprintf(buffer, sizeof(buffer), format, args); // Store the result of vsnprintf

 //    va_end(args);  // Call va_end after using va_list

 //    globalVariableString = std::string(buffer);
 //    logInfo("id console: %s", globalVariableString.c_str());

 //    return pIdLib_Printf(buffer);
 //   
 //}





/*typedef __int64 (__fastcall*idUsercmdGenLocalSendBtnPressMB_t)(__int64         idUsercmdGenLocal_a1,
                                                               unsigned int    deviceNumMB_a2,
                                                               int             keyNum_t_a3,
                                                               unsigned __int8 isDown_a4);


__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64         idUsercmdGenLocal_a1,
                                                        unsigned int    deviceNumMB_a2,
                                                        int             keyNum_t_a3,
                                                        unsigned __int8 isDown_a4);*/

