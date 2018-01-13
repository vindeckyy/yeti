#include "Hooks.h"

void Hooks::LowerbodyProxy(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float flLby = pData->m_Value.m_Float;
	game::math.normalize_float(flLby);

	IClientEntity* m_entity = (IClientEntity*)pStruct;

	if (m_entity->GetIndex() == m_pEngine->GetLocalPlayer()) {
		auto m_local = game::localdata.localplayer();

		game::globals.lby_update_end_time = m_pGlobals->curtime + 0.2f;
	}

	*(float*)(pOut) = pData->m_Value.m_Float;
}