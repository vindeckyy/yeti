#include "Backtracking.h"

#include "Backtracking.h"

void CBacktracking::update(int tick_count) {
	if (!resolverconfig.bBacktracking)
		return;

	latest_tick = tick_count;
	for (int i = 0; i < 64; i++) {
		update_record(i);
	}
}

bool CBacktracking::is_valid_tick(int tick) {
	int delta = latest_tick - tick;
	float deltaTime = delta * m_pGlobals->interval_per_tick;
	return (fabs(deltaTime) <= 0.2f);
}

void CBacktracking::update_record(int i) {
	IClientEntity* m_entity = m_pEntityList->GetClientEntity(i);
	if (m_entity && m_entity->IsAlive() && !m_entity->IsDormant()) {
		float lby = m_entity->GetLowerBodyYaw();
		if (lby != records[i].lby) {
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].head_position = game::functions.get_hitbox_location(m_entity, 0);
		}
	} else {
		records[i].tick_count = 0;
	}
}

bool CBacktracking::run_lby_backtrack(CUserCmd* m_pcmd, Vector& aim_point) {
	if (resolverconfig.bBacktracking) {
		for (int i = 0; i < m_pGlobals->maxClients; i++) {
			auto m_entity = m_pEntityList->GetClientEntity(i);
			auto m_local = game::localdata.localplayer();
			if (!m_entity || m_entity == m_local || m_entity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !m_entity->IsAlive()) continue;
			if (is_valid_tick(records[i].tick_count)) {
				aim_point = records[i].head_position;
				m_pcmd->tick_count = records[i].tick_count;
				return true;
			}
		}
	}
	return false;
}

CBacktracking* backtracking = new CBacktracking();
backtrack_data head_positions[64][12];