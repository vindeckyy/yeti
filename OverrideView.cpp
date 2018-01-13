#include "Hooks.h"
#include "ESP.h"

void __stdcall Hooks::Hooked_OverrideView(CViewSetup* view_setup) {
	if (m_pEngine->IsConnected() && m_pEngine->IsInGame()) {
		IClientEntity* m_local = m_pEntityList->GetClientEntity(m_pEngine->GetLocalPlayer());
		game::globals.fov = view_setup->fov;
		if ( m_local && !m_local->IsScoped() ) {
			view_setup->fov += visualconfig.flFieldOfView;
		}
	}

	g_fnOriginalOverrideView(view_setup);
}