#include "Hooks.h"

bool __fastcall Hooks::Hooked_FireEventClientSide(void* ecx, void* edx, IGameEvent* pEvent) {

	if (!pEvent)
		return g_fnOriginalFireEventClientSide(ecx, pEvent);

	auto m_local = game::localdata.localplayer();

	if (!strcmp(pEvent->GetName(), "round_start")) {
		if (visualconfig.player.bDormant) {
			for (int i = 0; i < 99; i++) {
				CPlayer* player = plist.get_player(i);
				if (player->is_valid_player)
					player->dormancy_data.initialized = false;
			}
		}
	}

	if (!strcmp(pEvent->GetName(), "player_death"))
		skinchanger.apply_killcon(pEvent);

	if (!strcmp(pEvent->GetName(), "game_newmap")) {
		if (g_ViewModelCFG.size() >= 1)
			g_ViewModelCFG.clear();
		skinchanger.set_viewmodel();
		game::globals.Target = nullptr;
		game::globals = CGameGlobals();
	}

	return g_fnOriginalFireEventClientSide(ecx, pEvent);
}