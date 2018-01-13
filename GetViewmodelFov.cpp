#include "Hooks.h"

float __stdcall Hooks::Hooked_GetViewmodelFov()
{
	float fov = Hooks::g_pClientModeHook->GetOriginal<Hooks::GetViewmodelFov_t>(35)();
	if (m_pEngine->IsInGame() && m_pEngine->IsConnected())
	{
		auto localplayer = game::localdata.localplayer();
		if (localplayer && localplayer->IsAlive())
		{
			//if (visualconfig.flFieldOfView)
				//fov += visualconfig.flFieldOfView;
		}
	}
	return fov;
}