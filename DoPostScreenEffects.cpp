#include "Hooks.h"
#include "ESP.h"

int __stdcall Hooks::Hooked_DoPostScreenEffects(int a1)
{
	auto m_local = game::localdata.localplayer();

	if (visualconfig.player.bGlow) esp.glow.paint();

	return g_fnOriginalDoPostScreenEffects(m_pClientMode, a1);
}