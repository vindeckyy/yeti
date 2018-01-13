#include "Hooks.h"
#include "DamageIndicators.h"

void CHookedEvents::FireGameEvent(IGameEvent *event) {
	const char* szEventName = event->GetName();

	if (!strcmp(szEventName, "player_hurt")) {
		if (m_pEngine->GetPlayerForUserID(event->GetInt("attacker")) == m_pEngine->GetLocalPlayer()) {
			int userid = m_pEngine->GetPlayerForUserID(event->GetInt("userid"));

			auto m_local = game::localdata.localplayer();
			IClientEntity* m_entity = m_pEntityList->GetClientEntity(userid);

			if (visualconfig.bRemoveWeaponScope) {
				game::globals.end_removed_scope_red = m_pGlobals->curtime + .4;
			}

			if (visualconfig.bDamageIndicators) {
				DamageIndicator_t DmgIndicator;
				DmgIndicator.iDamage = event->GetInt("dmg_health");
				DmgIndicator.Player = m_entity;
				DmgIndicator.flEraseTime = m_local->GetTickBase() * m_pGlobals->interval_per_tick + 3.f;
				DmgIndicator.bInitialized = false;

				damage_indicators.data.push_back(DmgIndicator);
			}

			if (event->GetInt("hitgroup") == 1) {
				if (m_entity) {
					CPlayer* m_player = plist.get_player(m_entity->GetIndex());
					if (m_player->is_valid_player) {
						m_player->resolver_data.has_hit_angle = true;
						m_player->resolver_data.last_hit_angle = *m_entity->GetEyeAngles();
						m_player->resolver_data.missed_shots = 0;
						m_player->resolver_data.shots = 0;
					}
				}
			}
		}
	}
	if (!strcmp(event->GetName(), "player_death")) {
		int userid = m_pEngine->GetPlayerForUserID(event->GetInt("userid"));

		IClientEntity* m_entity = m_pEntityList->GetClientEntity(userid);

		if (m_entity) {
			CPlayer* m_player = plist.get_player(m_entity->GetIndex());
			if (m_player->is_valid_player) {
				m_player->resolver_data.has_hit_angle = false;
				m_player->resolver_data.last_hit_angle = Vector(0, 0, 0);
				m_player->resolver_data.missed_shots = 0;
				m_player->resolver_data.shots = 0;
			}
		}
	}
}

void CHookedEvents::RegisterSelf() {
	m_nDebugID = 42;
	m_pGameEventManager->AddListener(this, "player_hurt", false);
	m_pGameEventManager->AddListener(this, "player_death", false);
}

void Hooks::initialize_events() {
	if (!m_pGameEventManager->FindListener(&hooked_events, "player_hurt"))
		hooked_events.RegisterSelf();
}