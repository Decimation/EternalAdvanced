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
