#include "idHooks.h"

__int64 __fastcall ea::idUsercmdGenLocalSendBtnPressMB_Hook(__int64 idUsercmdGenLocal_a1, unsigned int deviceNumMB_a2,
															id::keyNum_t keyNum_t_a3, unsigned __int8 isDown_a4)
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


	auto rv = p_idUsercmdGenLocalSendBtnPressMB(idUsercmdGenLocal_a1, deviceNumMB_a2, keyNum_t_a3, isDown_a4);

	// auto rv = g_x.orig(idUsercmdGenLocal_a1, deviceNumMB_a2, keyNum_t_a3, isDown_a4);

	auto xy =
		std::format("{} {} {} {} -> {}", idUsercmdGenLocal_a1, deviceNumMB_a2, (int) keyNum_t_a3, isDown_a4, rv);
	g_logfile << xy << '\n';
	g_logfile.flush();

	return rv;
}
