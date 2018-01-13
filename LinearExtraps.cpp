#include "LinearExtraps.h"

LinearExtrapolations linear_extraps;

void LinearExtrapolations::run()
{
	if (resolverconfig.bLinearExtraps)
	{
		auto m_local = game::localdata.localplayer();
		for (int i = 1; i < m_pGlobals->maxClients; i++)
		{
			auto m_entity = m_pEntityList->GetClientEntity(i);
			if (!m_entity || m_entity == m_local || m_entity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !m_entity->IsAlive()) continue;

			CPlayer* m_player = plist.get_player(i);
			if (!m_player->is_valid_player) continue;

			float simtime_delta = m_entity->m_flSimulationTime() - m_entity->m_flOldSimulationTime();
			int choked_ticks = game::math.clamp(TIME_TO_TICKS(simtime_delta), 1, 15);

			if (m_player->last_origin.Length() != m_entity->GetOrigin().Length())
				m_player->last_origin = m_entity->GetOrigin();

			float delta_distance = (m_entity->GetOrigin() - m_player->last_origin).LengthSqr();
			if (delta_distance > 4096.f)
			{
				Vector velocity_per_tick = m_entity->GetVelocity() * m_pGlobals->interval_per_tick;
				auto new_origin = m_entity->GetOrigin() + (velocity_per_tick * choked_ticks);
				m_entity->SetAbsOriginal(new_origin);
			}
		}
	}
}