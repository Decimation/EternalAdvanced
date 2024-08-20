// ReSharper disable CppInconsistentNaming
#include "dllmain.hpp"

idUsercmdGenLocalSendBtnPressMB_t p_idUsercmdGenLocalSendBtnPressMB_t        = nullptr;
idUsercmdGenLocalSendBtnPressMB_t p_idUsercmdGenLocalSendBtnPressMB_t_Target = nullptr;

RenderSprite_t p_RenderSprite_t = nullptr;
RenderSprite_t p_RenderSprite_t_Target = nullptr;

idPlayerFovLerp_t p_idPlayerFovLerp_t = nullptr;
idPlayerFovLerp_t p_idPlayerFovLerp_t_Target = nullptr;

StartSync_t p_StartSync_t = nullptr;
StartSync_t p_StartSync_t_Target = nullptr;

SyncEnd_t p_SyncEnd_t = nullptr;
SyncEnd_t p_SyncEnd_t_Target = nullptr;


customAnimSmth_t p_customAnimSmth_t = nullptr;
customAnimSmth_t p_customAnimSmth_t_Target = nullptr;

idHUDMenu_CurrencyConfirmation_t p_idHUDMenu_CurrencyConfirmation_t = nullptr;
idHUDMenu_CurrencyConfirmation_t p_idHUDMenu_CurrencyConfirmation_t_Target = nullptr;

idMenu_Update_t p_idMenu_Update_t = nullptr;
idMenu_Update_t p_idMenu_Update_t_Target = nullptr;

getFovTargetVal_t p_getFovTargetVal_t = nullptr;
getFovTargetVal_t p_getFovTargetVal_t_Target = nullptr;

SelectWeaponForSelectionGroup_t p_SelectWeaponForSelectionGroup_t = nullptr;
SelectWeaponForSelectionGroup_t p_SelectWeaponForSelectionGroup_t_Target = nullptr;

isKeyPressed_t p_isKeyPressed_t = nullptr;
isKeyPressed_t p_isKeyPressed_t_Target = nullptr;

BindsStrSet_t p_BindsStrSet_t = nullptr;
BindsStrSet_t p_BindsStrSet_t_Target = nullptr;

idHUD_Reticle_SetActiveReticle_t p_idHUD_Reticle_SetActiveReticle_t = nullptr;
idHUD_Reticle_SetActiveReticle_t p_idHUD_Reticle_SetActiveReticle_t_Target = nullptr;

convertIdDeclUIColorToidColor_t p_convertIdDeclUIColorToidColor_t = nullptr;
convertIdDeclUIColorToidColor_t p_convertIdDeclUIColorToidColor_t_Target = nullptr;

setSpriteInstanceColor_t p_setSpriteInstanceColor_t = nullptr;
setSpriteInstanceColor_t p_setSpriteInstanceColor_t_Target = nullptr;

printOutlinedString_t p_printOutlinedString_t = nullptr;
printOutlinedString_t p_printOutlinedString_t_Target = nullptr;



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



// intereception user inputs (keyboard, controller...however this will not register controller analog inputs which is a problem. a hook is usefull but a typedef to this func would be good so we can call it when necessary and send fake inputs as this func is only trigger when user inputs smth. May be we could hook what is calling it(?)
__int64 __fastcall idUsercmdGenLocalSendBtnPressMB_Hook(__int64      idUsercmdGenLocal_a1, unsigned int deviceNumMB_a2,
                                                        id::keyNum_t keyNum_t_a3, unsigned __int8       isDown_a4)
{
	static int             lastKeyNum_t_a3 = -1;
	static unsigned __int8 lastIsDown_a4   = 0;


	//todo this works, not using it atm but we might use it in the incoming rewrite of the mod
	/*if (keyNum_t_a3 != lastKeyNum_t_a3) {
		logInfo("keyNum_t_a3 has changed to: 0x%X (isDown_a4: %d)", keyNum_t_a3, isDown_a4);
		lastKeyNum_t_a3 = keyNum_t_a3;
	}

	if (isDown_a4 != lastIsDown_a4) {
		logInfo("isDown_a4 has changed to: %d",  isDown_a4);
		lastIsDown_a4 = isDown_a4;
	}*/


	auto rv = p_idUsercmdGenLocalSendBtnPressMB_t(idUsercmdGenLocal_a1, deviceNumMB_a2, keyNum_t_a3, isDown_a4);


	auto x =
		std::format("{} {} {} {} -> {}", idUsercmdGenLocal_a1, deviceNumMB_a2, (int) keyNum_t_a3, isDown_a4, rv);
	g_logfile << x << '\n';
	g_logfile.flush();

	return rv;
}

// this is used to prevent the rendering of certain hud elements so that the custom ice grenade icon can be drawn with a clean background
char __fastcall RenderSprite_Hook(
	void* idSwf_a1,
	__int64 a2,
	void* spriteInstance_a3,
	__int64 a4,
	unsigned int a5,
	char a6) {


	/*if (!modSettings::getIsUseDedicatedNadeKeys()) {
		return p_RenderSprite_Original(idSwf_a1, a2, spriteInstance_a3, a4, a5, a6);
	}

	if (spriteInstance_a3) {

		int fullPathHash = spriteInstance_a3->fullPathHash;

		if (fullPathHash == equipmentArrowSpriteId) {
			return 0;
		}

		else if (fullPathHash == equipmentBackerSpriteId) {
			return 0;
		}	

	}*/

	return p_RenderSprite_t(idSwf_a1, a2, spriteInstance_a3, a4, a5, a6);
}


// this is used to change the FOV when player is locked in specific animations like a glory kill or interacting with specific object like inserting a battery in the container.
void* __fastcall idPlayerFovLerp_Hook(__int64 idPlayer_a1, float lerpFOV_a2, float a3, char a4) {

	/*if (modSettings::getIsOverideInteractionFOV()) {
		lerpFOV_a2 = modSettings::getInteractionFOV();
	}*/

	return p_idPlayerFovLerp_t(idPlayer_a1, lerpFOV_a2, a3, a4);
}


// this is used to identify if a battery=>socket anim starts so we can change the gamespeed to speedup animation.
void __fastcall StartSync_t_Hook(__int64 idSyncEntity_a1, __int64 idDeclAnimWebPTR_a2, __int64 idPlayer_a3, float a4, __int64 a5) {

	/*if (modSettings::getIsSpeedUpBatterySocketAnimation() && !MemHelper::isBadReadPtr((void*)idDeclAnimWebPTR_a2)) {
		idResource* res = (idResource*)*(__int64*)idDeclAnimWebPTR_a2;
		std::string declName = res->name.str;
		if (declName == "animweb/interact/hub_battery_socket/hub_battery_socket_syncanimweb") {
			logInfo("StartSync_t_Hook batter interaction detected !");
			idCmd::setGameSpeed(gameSpeed_K::X2_Speed);
		}
		else {
			logInfo("StartSync_t_Hook: found sync animation we're not speeding up (atm) declName: %s ", declName.c_str());
		}
	}*/
	return p_StartSync_t(idSyncEntity_a1, idDeclAnimWebPTR_a2, idPlayer_a3, a4, a5);
}

// triggered when a custom sync animation end. 
char __fastcall syncEnd_Hook(__int64 a1, __int64 a2, int a3, unsigned int a4, unsigned __int8 a5) {

	/*logInfo("syncEnd_Hook: reseting timescale....");
	idCmd::setGameSpeed(gameSpeed_K::defaultSpeed);*/

	return p_SyncEnd_t(a1, a2, a3, a4, a5);
}

// triggered when a specific kind of custom animation (when finding secrets which locks the player in place) is triggered but
char __fastcall customAnimSmth_t_Hook(__int64 idHands_a1, const char* str_a2, int a3, int a4, int a5) {

	/*if (modSettings::getIsSkipCustomAnimations()) {
		return 0;
	}	*/

	return p_customAnimSmth_t(idHands_a1, str_a2, a3, a4, a5);
}


// used to prevent to auto dismiss the currency confirmation menu (dismisses the  'put battery in socket ?' dialog)
__int64 __fastcall idHUDMenu_CurrencyConfirmation_t_Hook(__int64 idHUDMenu_CurrencyConfirmation_a1, __int64 struct_a2) {

	/*if (modSettings::getIsDisableBatterySocketPopUp()) {
		idHudEventManager::send_HUD_EVENT_CURRENCY_CONFIRM((idHUDElement*)idHUDMenu_CurrencyConfirmation_a1);
	}*/

	return p_idHUDMenu_CurrencyConfirmation_t(idHUDMenu_CurrencyConfirmation_a1, struct_a2);
}



__int64 __fastcall idMenu_UpdateHook(__int64 idMenu, __int64 a2) {

	//ImGuiManager::setIsInitFlag(true);

	//PlayerStateChecker::updateLastMenuRefresh(); //! this is a hacky and not so good way to know that the game has been (may be) initialized	

	return p_idMenu_Update_t(idMenu, a2);
}


// this is used to get the currently equipped idWeapon of local player. (might be a way to get it from somewhere else but at least this is very reliable)
//float __fastcall getFovTargetValMB_Hook(idWeapon* idWeaponObj_a1) {
float __fastcall getFovTargetValMB_Hook(void* idWeaponPtr_a1) {
	//logDebug("SelectWeaponForSelectionGroupHook");

	//idWeaponManager::acquirreCurrentIdWeapon(idWeaponPtr_a1);
	
	return p_getFovTargetVal_t(idWeaponPtr_a1);
}


// this select the weapon of the player when using direct keybinds (meaning not when using weapon wheel or other mean to change weapon) and this is way to overwrite this weapon if the game missed a key press which happens often and can be very frustrating for the player.
char __fastcall SelectWeaponForSelectionGroupHook(__int64 a1, int weaponIndex_a2) {	

	/*if (modSettings::isImprovedWeaponSwitching()) {
		switcher.updateLastWeapKeyPress(weaponIndex_a2);
	}*/

	return p_SelectWeaponForSelectionGroup_t(a1, weaponIndex_a2);
}


// this checks only the keys bound to an action, this is used to have dedicated grenade keys and also to check the last key weapon key pressed so can correct it if the current equipped weapon is not what it should be.
bool __fastcall isKeyPressedHook(__int64 ptr, __int64 btnEnum) {

	// can not fill the func cause a lot of data structure are not in id.h yet

	return p_isKeyPressed_t(ptr, btnEnum);

}


//used to ovewrite the string in the Slayer control strings so that if user uses dedicated grenade keys, they will be correctly labeled as so.
__int64 __fastcall BindsStrSetHook(__int64* a1, unsigned char* a2) {

	/*if (!modSettings::getIsUseDedicatedNadeKeys()) {
		return pBindsStrSet(a1, a2);
	}

	if (PlayerStateChecker::isInMenus()) {
		bindLabelChanger.overwriteDynamicBindLabels(a2, lang.getLocalizedBindStringData());
	}*/
	return p_BindsStrSet_t(a1, a2);
}


// this is used to force disable the game crosshair if user uses the imgui custom crosshair and also to check if slayer is in game playing, as he could be in a cutsene or any other state where user does not have control of the slayer. leaving the commented code for reference. also the actual idHUD_Reticle and idDeclWeaponReticle can not be used here cause id.h doesn't have them.
//void __fastcall idHUD_Reticle_SetActiveReticleHook(idHUD_Reticle* idHUD_Reticle_a1, unsigned int reticleIndex_a2, idDeclWeaponReticle* idDeclWeaponReticle_a3, unsigned __int8 a4) {
void __fastcall idHUD_Reticle_SetActiveReticleHook(void* idHUD_Reticle_a1, unsigned int reticleIndex_a2, void* idDeclWeaponReticle_a3, unsigned __int8 a4) {


	////! 29/4/24: still using this system from previous mod after all as our new customizedWeapon is extremely frustrating to use as some declweapons arbitrarily do not respect the reticule set rule and vary from weapon to weapon so...funk it!
	////return pidHUD_Reticle_SetActiveReticle(idHUD_Reticle_a1, reticleIndex_a2, idDeclWeaponReticle_a3, a4);
	////logInfo("idHUD_Reticle_SetActiveReticleHook Triggered");

	//PlayerStateChecker::updateLastReticleRefresh();

	//idSWFWidget_Hud_Reticle* idSWFWidget_Hud_Reticle_v8 = 0i64;
	////unsigned int activeReticleStyle = *(unsigned int*)(idHUD_Reticle_a1 + 0x330);
	//unsigned int activeReticleStyle = (unsigned int)idHUD_Reticle_a1->activeReticleStyle;

	////logInfo("idHUD_Reticle_SetActiveReticleHook: debug 3");


	//if (reticleIndex_a2 > idDeclWeaponReticle_reticleStyle_t::RETICLE_STYLE_MAX) {
	//	idSWFWidget_Hud_Reticle_v8 = 0i64;
	//}
	//else {
	//	idSWFWidget_Hud_Reticle_v8 = idHUD_Reticle_a1->reticles_ptr[reticleIndex_a2]; //! this is fine.	

	//}

	//CustomCrosshairManager::acquireWeaponCoolDownStatus(idSWFWidget_Hud_Reticle_v8, idDeclWeaponReticle_a3);

	////logInfo("debug:  idSWFWidget_Hud_Reticle_v8 is %p ", idSWFWidget_Hud_Reticle_v8);
	////! idSWFWidget_Hud_Reticle_v8 will be null when the ingame reticle mode is set to None so the rest of this code will not trigger, obviously
	//if (idSWFWidget_Hud_Reticle_v8) {

	//	idSWFSpriteInstance* idSWFSpriteInstance_v17 = idSWFWidgetManager::getBoundSprite(idSWFWidget_Hud_Reticle_v8);

	//	//logInfo("idHUD_Reticle_SetActiveReticleHook: debug 5");

	//	//? this is the mistake we made we got the wrong spriteInstance and as a result, the scale was inconsistent, we were using this....
	//	//idSWFSpriteInstance* idSWFSpriteInstance_v17 = idSWFWidget_Hud_Reticle_v8->reticleSprite;
	//	//? ...instead of this: 
	//	//__int64 idSWFSpriteInstance_v17 = *(__int64*)(idSWFWidget_Hud_Reticle_v8 + 0x18);

	//	//! looks this work now:
	//	idSWFSpriteInstanceManager::setHitMarkerState((idHUD_Reticle*)idHUD_Reticle_a1, modSettings::getIsDisableHitMarker());

	//	//logInfo("idHUD_Reticle_SetActiveReticleHook: debug 6");


	//	//! this is how we manage to change the scale of the crosshair even when it's already displayed
	//	if (idDeclWeaponReticle_a3 && idSWFSpriteInstance_v17) {


	//		//! from logs, even when the dot crosshair is set in the game menu, idDeclWeaponReticle_a3->style will have the value of the current weapon reticle style so we can use that to set crosshair color then?

	//		//logInfo("debug:  idHUD_Reticle_a1->activeReticleDecl: %p and idDeclWeaponReticle_a3: %p activeReticleStyle: %u reticleIndex_a2: %u", idHUD_Reticle_a1->activeReticleDecl, idDeclWeaponReticle_a3, activeReticleStyle, reticleIndex_a2);

	//		//! if the reticle scale needs to be changed cause user wants it changed, we trigger the sprite reload function
	//		if (ReticleScaleManager::updateScale(idDeclWeaponReticle_a3)) {
	//			idSWFSpriteInstanceManager::updateScale(idSWFSpriteInstance_v17, idDeclWeaponReticle_a3->reticleModelScale);
	//		}

	//		//! if game dot:
	//		if (activeReticleStyle == idDeclWeaponReticle_reticleStyle_t::RETICLE_STYLE_DOT) {

	//			if (modSettings::getIsUseImguiDotCrosshair()) {
	//				idSWFWidget_Hud_Reticle_v8->reticleSprite->namedColorId = swfNamedColors_t::SWF_CUSTOM_NAMED_COLOR_INVISIBLE;
	//			}
	//			else {
	//				idSWFWidget_Hud_Reticle_v8->reticleSprite->namedColorId = swfNamedColors_t::SWF_CUSTOM_NAMED_COLOR_DEFAULT;
	//			}

	//			//? doesn't work to force disable dot:
	//			//idHUD_Reticle_a1->activeReticleStyle = idDeclWeaponReticle_reticleStyle_t::RETICLE_STYLE_NONE;

	//			//? doesn't work to force disable dot:
	//			//reticleIndex_a2 = idDeclWeaponReticle_reticleStyle_t::RETICLE_STYLE_NONE;

	//			//idDeclWeaponReticleManager::debugPrintCurrentDeclWeaponReticle(idDeclWeaponReticle_a3);
	//			//! this used to crash because of bad padding in our generated classes
	//			/*ReticleColorManager::updateDotColorV3(idSWFWidget_Hud_Reticle_v8, idDeclWeaponReticle_a3, idSWFSpriteInstance_v17);*/

	//		}
	//	}
	//}
	return p_idHUD_Reticle_SetActiveReticle_t(idHUD_Reticle_a1, reticleIndex_a2, idDeclWeaponReticle_a3, a4);
}



//idColor* __fastcall convertIdDeclUIColorToidColorHook(__int64 idDeclUIColor_a1, idColor* idColorPtr_a2, int colorId_a3) { //! there is a bak of the old version of this hook in debug.h
// let us change color of hud elements
void* __fastcall convertIdDeclUIColorToidColorHook(__int64 idDeclUIColor_a1, void* idColorPtr_a2, int colorId_a3) { //! 

	
	////! by acequiring this address we always know where the color of a specifi elelment on the hud is. this is useful when user changes profile cause idDeclUIColor_a1 will change in that case and there are many color profiles.
	//GameHudColorsManager::acquireIdDeclUIColorAddr(idDeclUIColor_a1);

	//if (colorId_a3 == swfNamedColors_t::SWF_CUSTOM_NAMED_COLOR_DEFAULT) {
	//	return pconvertIdDeclUIColorToidColor(idDeclUIColor_a1, idColorPtr_a2, colorId_a3);
	//}

	////? would still need a check for the color above old make namedColor and SWF_CUSTOM_NAMED_COLOR_DEFAULT

	////! here in we check if colorId_a3> is not default  and if it's in bound:
	//if ((colorId_a3 > swfNamedColors_t::SWF_CUSTOM_NAMED_COLOR_DEFAULT) && (colorId_a3 <= swfNamedColors_t::SWF_CUSTOM_NAMED_COLOR_INVISIBLE)) {

	//	//logInfo("convertIdDeclUIColorToidColorHook: colorId_a3: %d", colorId_a3);

	//	//? 30/4/24 removing this just 5 min for testing....
	//	*idColorPtr_a2 = *(idColor*)GameHudColorsManager::getCustomIdColor(colorId_a3);
	//	return idColorPtr_a2;

	//}

	return p_convertIdDeclUIColorToidColor_t(idDeclUIColor_a1, idColorPtr_a2, colorId_a3);
}


// actually changing the color of sprites on the hud
void __fastcall setSpriteInstanceColorHook(__int64 idSWFSpriteInstance_a1, unsigned int namedColorId_a2) {
		

	//? this is used to get the addresses of all the sprites instances we are interested in. So at least, the frag icon instances so this should be UNcommented all the time. As those instances addresses are acquired when a level load or when the idCmd::reapplySwfColorsCmd is called.
	/*GameHudColorsManager::acquireMonitoredSpriteInstanceAddr(idSWFSpriteInstance_a1);


	namedColorId_a2 = GameHudColorsManager::getColor(idSWFSpriteInstance_a1, namedColorId_a2);*/
		

	return p_setSpriteInstanceColor_t(idSWFSpriteInstance_a1, namedColorId_a2);
}


// this is the hook used to render the multi layered ice grenade icon
__int64 __fastcall printOutlinedStringMB_hook(
	__int64 idRenderModelGui_a1PtrToPtr,
	int a2,
	__int64 a3,
	int a4,
	__int64 a5,
	char* a6,
	int a7,
	int a8
) {

	//__int64 gui = *(__int64*)idRenderModelGui_a1PtrToPtr;

	//auto iceNadeIconData = CustomIceNadeIconManager::getData();
	//iceNadeIconData.updateMaterials(); //? this could be the answer to the crashing when loading levels....fingers crossed...Might be a bit slower than before cause of matr fetching (?)...Update no more crash indeed so far.

	//if (iceNadeIconData.isRenderingAllowed) {
	//	idRenderModelGuiManager::drawIceIcon(gui, iceNadeIconData);

	//}

	//return hudString::textForidCmd.size();
	
	//! temporary:
	return p_printOutlinedString_t(idRenderModelGui_a1PtrToPtr, a2, a3, a4, a5, a6, a7, a8);
}




#pragma region Proxy
struct msimg32_dll
{
	HMODULE dll;
	FARPROC oAlphaBlend;
	FARPROC oDllInitialize;
	FARPROC oGradientFill;
	FARPROC oTransparentBlt;
	FARPROC ovSetDdrawflag;
} msimg32;

extern "C" {
	FARPROC PA = nullptr;
	int     runASM();

	void fAlphaBlend()
	{
		PA = msimg32.oAlphaBlend;
		runASM();
	}

	void fDllInitialize()
	{
		PA = msimg32.oDllInitialize;
		runASM();
	}

	void fGradientFill()
	{
		PA = msimg32.oGradientFill;
		runASM();
	}

	void fTransparentBlt()
	{
		PA = msimg32.oTransparentBlt;
		runASM();
	}

	void fvSetDdrawflag()
	{
		PA = msimg32.ovSetDdrawflag;
		runASM();
	}
}

void setupFunctions()
{
	msimg32.oAlphaBlend     = GetProcAddress(msimg32.dll, "AlphaBlend");
	msimg32.oDllInitialize  = GetProcAddress(msimg32.dll, "DllInitialize");
	msimg32.oGradientFill   = GetProcAddress(msimg32.dll, "GradientFill");
	msimg32.oTransparentBlt = GetProcAddress(msimg32.dll, "TransparentBlt");
	msimg32.ovSetDdrawflag  = GetProcAddress(msimg32.dll, "vSetDdrawflag");
}
#pragma endregion


int init()
{
	int ret = 0;

	id::init();

	p_idUsercmdGenLocalSendBtnPressMB_t_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(
		PatternScan(id::DE_EXE_MODULE, IdUsercmdGenLocalSendBtnPressFpSig));
	g_logfile << "p_idUsercmdGenLocalSendBtnPressMB_t_Target " << std::hex << p_idUsercmdGenLocalSendBtnPressMB_t_Target << "\n";


	p_RenderSprite_t_Target = reinterpret_cast<RenderSprite_t>(
		PatternScan(id::DE_EXE_MODULE, RenderSpriteFpSig));	
	g_logfile << "p_RenderSprite_t_Target " << std::hex << p_RenderSprite_t_Target << "\n";



	p_idPlayerFovLerp_t_Target = reinterpret_cast<idPlayerFovLerp_t>(
		PatternScan(id::DE_EXE_MODULE, idPlayerFovLerpFpSig));
	g_logfile << "p_idPlayerFovLerp_t_Target " << std::hex << p_idPlayerFovLerp_t_Target << "\n";


	p_StartSync_t_Target = reinterpret_cast<StartSync_t>(
		PatternScan(id::DE_EXE_MODULE, StartSyncFpSig));
	g_logfile << "p_StartSync_t_Target " << std::hex << p_StartSync_t_Target << "\n";


	p_SyncEnd_t_Target = reinterpret_cast<SyncEnd_t>(
		PatternScan(id::DE_EXE_MODULE, syncEndFpSig));
	g_logfile << "p_SyncEnd_t_Target " << std::hex << p_SyncEnd_t_Target << "\n";


	p_customAnimSmth_t_Target = reinterpret_cast<customAnimSmth_t>(
		PatternScan(id::DE_EXE_MODULE, skipCustomAnimFpSig));
	g_logfile << "p_SyncEnd_t_Target " << std::hex << p_SyncEnd_t_Target << "\n";


	p_idHUDMenu_CurrencyConfirmation_t_Target = reinterpret_cast<idHUDMenu_CurrencyConfirmation_t>(
		PatternScan(id::DE_EXE_MODULE, idHUDMenu_CurrencyConfirmationSmthFpSig));
	g_logfile << "p_idHUDMenu_CurrencyConfirmation_t_Target " << std::hex << p_idHUDMenu_CurrencyConfirmation_t_Target << "\n";


	p_idMenu_Update_t_Target = reinterpret_cast<idMenu_Update_t>(
		PatternScan(id::DE_EXE_MODULE, idMenu_UpdateSig));
	g_logfile << "p_idMenu_Update_t_Target " << std::hex << p_idMenu_Update_t_Target << "\n";


	p_getFovTargetVal_t_Target = reinterpret_cast<getFovTargetVal_t>(
		PatternScan(id::DE_EXE_MODULE, GetFovTargetValSig));
	g_logfile << "p_getFovTargetVal_t_Target " << std::hex << p_getFovTargetVal_t_Target << "\n";


	p_SelectWeaponForSelectionGroup_t_Target = reinterpret_cast<SelectWeaponForSelectionGroup_t>(
		PatternScan(id::DE_EXE_MODULE, idPLayerSelectWeaponForSelectionGroupSig));
	g_logfile << "p_SelectWeaponForSelectionGroup_t_Target " << std::hex << p_SelectWeaponForSelectionGroup_t_Target << "\n";


	p_isKeyPressed_t_Target = reinterpret_cast<isKeyPressed_t>(
		PatternScan(id::DE_EXE_MODULE, isKeyPressedSig));
	g_logfile << "p_isKeyPressed_t_Target " << std::hex << p_isKeyPressed_t_Target << "\n";


	p_BindsStrSet_t_Target = reinterpret_cast<BindsStrSet_t>(
		PatternScan(id::DE_EXE_MODULE, pBindsStrSetSig));
	g_logfile << "p_BindsStrSet_t_Target " << std::hex << p_BindsStrSet_t_Target << "\n";


	p_idHUD_Reticle_SetActiveReticle_t_Target = reinterpret_cast<idHUD_Reticle_SetActiveReticle_t>(
		PatternScan(id::DE_EXE_MODULE, setActiveReticleSig));
	g_logfile << "p_idHUD_Reticle_SetActiveReticle_t_Target " << std::hex << p_idHUD_Reticle_SetActiveReticle_t_Target << "\n";

	p_convertIdDeclUIColorToidColor_t_Target = reinterpret_cast<convertIdDeclUIColorToidColor_t>(
		PatternScan(id::DE_EXE_MODULE, convertIdDeclUIColorToidColorTargetSig));
	g_logfile << "p_convertIdDeclUIColorToidColor_t_Target " << std::hex << p_convertIdDeclUIColorToidColor_t_Target << "\n";


	p_setSpriteInstanceColor_t_Target = reinterpret_cast<setSpriteInstanceColor_t>(
		PatternScan(id::DE_EXE_MODULE, setSpriteInstanceSig));
	g_logfile << "p_setSpriteInstanceColor_t_Target " << std::hex << p_setSpriteInstanceColor_t_Target << "\n";


	p_printOutlinedString_t_Target  = reinterpret_cast<printOutlinedString_t>(
		PatternScan(id::DE_EXE_MODULE, printOutlinedStringMBSig));







	if (MH_Initialize() != MH_OK) {
		ret = 1;
		goto ret;
	}

	for (auto ix = 0; ix < 8; ix++)
		g_logfile << "byte: " << std::hex <<
			(int) reinterpret_cast<uint8_t*>(p_idUsercmdGenLocalSendBtnPressMB_t_Target)[ix] << " ";

	// p_idUsercmdGenLocalSendBtnPressMB_t_Target = reinterpret_cast<idUsercmdGenLocalSendBtnPressMB_t>(p_idOrigFunc);

	// Create a hook for MessageBoxW, in disabled state.
	if (MH_CreateHook(p_idUsercmdGenLocalSendBtnPressMB_t_Target, &idUsercmdGenLocalSendBtnPressMB_Hook,
	                  reinterpret_cast<void**>(&p_idUsercmdGenLocalSendBtnPressMB_t))) {
		g_logfile << "failed to create hook idUsercmdGenLocalSendBtnPressMB_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_RenderSprite_t_Target, &RenderSprite_Hook, reinterpret_cast<void**>(&p_RenderSprite_t))) {
		g_logfile << "failed to create hook RenderSprite_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_idPlayerFovLerp_t_Target, &idPlayerFovLerp_Hook, reinterpret_cast<void**>(&p_idPlayerFovLerp_t))) {
		g_logfile << "failed to create hook idPlayerFovLerp_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_StartSync_t_Target, &StartSync_t_Hook, reinterpret_cast<void**>(&p_StartSync_t))) {
		g_logfile << "failed to create hook StartSync_t_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_SyncEnd_t_Target, &syncEnd_Hook, reinterpret_cast<void**>(&p_SyncEnd_t))) {
		g_logfile << "failed to create hook syncEnd_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_customAnimSmth_t_Target, &customAnimSmth_t_Hook, reinterpret_cast<void**>(&p_customAnimSmth_t))) {
		g_logfile << "failed to create hook customAnimSmth_t_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_idHUDMenu_CurrencyConfirmation_t_Target, &idHUDMenu_CurrencyConfirmation_t_Hook, reinterpret_cast<void**>(&p_idHUDMenu_CurrencyConfirmation_t))) {
		g_logfile << "failed to create hook idHUDMenu_CurrencyConfirmation_t_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_idMenu_Update_t_Target, &idMenu_UpdateHook, reinterpret_cast<void**>(&p_idMenu_Update_t))) {
		g_logfile << "failed to create hook idMenu_UpdateHook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_getFovTargetVal_t_Target, &getFovTargetValMB_Hook, reinterpret_cast<void**>(&p_getFovTargetVal_t))) {
		g_logfile << "failed to create hook getFovTargetValMB_Hook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_SelectWeaponForSelectionGroup_t_Target, &SelectWeaponForSelectionGroupHook, reinterpret_cast<void**>(&p_SelectWeaponForSelectionGroup_t))) {
		g_logfile << "failed to create hook SelectWeaponForSelectionGroupHook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_isKeyPressed_t_Target, &isKeyPressedHook, reinterpret_cast<void**>(&p_isKeyPressed_t))) {
		g_logfile << "failed to create hook isKeyPressedHook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_BindsStrSet_t_Target, &BindsStrSetHook, reinterpret_cast<void**>(&p_BindsStrSet_t))) {
		g_logfile << "failed to create hook BindsStrSetHook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_idHUD_Reticle_SetActiveReticle_t_Target, &idHUD_Reticle_SetActiveReticleHook, reinterpret_cast<void**>(&p_idHUD_Reticle_SetActiveReticle_t))) {
		g_logfile << "failed to create hook idHUD_Reticle_SetActiveReticleHook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_convertIdDeclUIColorToidColor_t_Target, &convertIdDeclUIColorToidColorHook, reinterpret_cast<void**>(&p_convertIdDeclUIColorToidColor_t))) {
		g_logfile << "failed to create hook convertIdDeclUIColorToidColorHook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_setSpriteInstanceColor_t_Target, &setSpriteInstanceColorHook, reinterpret_cast<void**>(&p_setSpriteInstanceColor_t))) {
		g_logfile << "failed to create hook setSpriteInstanceColorHook";
		ret = 1;
		goto ret;
	}

	if (MH_CreateHook(p_printOutlinedString_t_Target, &printOutlinedStringMB_hook, reinterpret_cast<void**>(&p_printOutlinedString_t))) {
		g_logfile << "failed to create hook printOutlinedStringMB_hook";
		ret = 1;
		goto ret;
	}






	// Enable the hook for MessageBoxW.
	if (MH_EnableHook(NULL) != MH_OK) {
		g_logfile << "failed to enable hook";
		ret = 1;
		goto ret;
	}
ret:
	return ret;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	//Initialize MinHook.


	switch (ul_reason_for_call) {
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_ATTACH:
		init_log();

		g_logfile << std::format("DllMain {}", (void*) hModule) << '\n';
		g_logfile << "Dll attach" << '\n';
		if (GetCallingModuleName().ends_with("idTechLauncher.exe")) {
			g_logfile << "Exiting" << '\n';
			FreeLibrary(msimg32.dll);
			close_log();
			return TRUE;
		}

		char path[MAX_PATH];
		GetWindowsDirectory(path, sizeof(path));

	// Example: "\\System32\\version.dll"
		strcat_s(path, "\\System32\\msimg32.dll");
		msimg32.dll = LoadLibrary(path);
		setupFunctions();
		init();

	// Add here your code, I recommend you to create a thread
		break;
	case DLL_PROCESS_DETACH:

		g_logfile << "Detaching" << '\n';
		FreeLibrary(msimg32.dll);
		close_log();
		break;
	}

ret:
	// g_logfile << "DllMain return" << '\n';

	return TRUE;
}
