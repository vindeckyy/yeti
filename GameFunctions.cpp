#include "GameFunctions.h"

/*bool CGameFunctions::can_shoot()
{
	auto m_local = m_pEntityList->GetClientEntity(m_pEngine->GetLocalPlayer());
	if (!m_local) return false;

	auto m_weapon = m_local->GetWeapon();

	if (!m_local)
		return false;

	if (!m_weapon)
		return false;

	float flServerTime = m_local->GetTickBase() * m_pGlobals->interval_per_tick;

	return (!(m_weapon->GetNextPrimaryAttack() > flServerTime));
}

void CGameFunctions::add_shot(CUserCmd* m_pcmd, int& shots, bool has_target, IClientEntity* m_target)
{
	auto m_local = m_pEntityList->GetClientEntity(m_pEngine->GetLocalPlayer());
	auto m_weapon = m_local->GetWeapon();
	if (m_weapon)
	{
		if (!m_weapon->IsGrenade() && !m_weapon->IsKnife() && !m_weapon->IsC4() && m_weapon->GetAmmoInClip() > 0 && has_target)
		{
			if (can_shoot() && m_pcmd->buttons & IN_ATTACK)
			{
				shots += 1;

				CPlayer* m_player = plist.get_player(m_target->GetIndex());
				if (!m_player->is_empty)
				{
					m_player->resolver_data.missed_shots += 1;
					m_player->resolver_data.shots += 1;
				}
			}
		}
	}
}*/