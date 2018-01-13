#include "Hooks.h"

void __stdcall Hooks::Hooked_DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t *matrix) {
	auto m_local = game::localdata.localplayer();

	g_pModelRenderHook->GetOriginal<DrawModelExecute_t>(21)(m_pModelRender, ctx, state, info, matrix);
	m_pModelRender->ForcedMaterialOverride(NULL);
}