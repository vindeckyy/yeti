#include "Resolver.h"
#include "Hooks.h"

CResolver resolver;

void CResolver::draw_developer_data() {
	if (resolverconfig.bResolverDebug) {
		int pos_y = 30;
		char buffer_shots[128];
		float shots = game::globals.Shots % 4;
		sprintf_s(buffer_shots, "Shots: %1.0f", shots);
		draw.text(4, pos_y, buffer_shots, draw.fonts.menu_bold, Color(255, 255, 255));
		pos_y += 10;

		char buffer_chokedticks[128];
		float choked_ticks = game::globals.choked_ticks;
		sprintf_s(buffer_chokedticks, "Choked ticks: %1.0f", choked_ticks);
		draw.text(4, pos_y, buffer_chokedticks, draw.fonts.menu_bold, Color(255, 255, 255));
		pos_y += 10;

		if (game::globals.UserCmd) {
			char buffer_realyaw[128];
			static float real_yaw = 0;
			if (!game::globals.SendPacket) real_yaw = game::globals.UserCmd->viewangles.y;
			sprintf_s(buffer_realyaw, "Real yaw: %1.0f", real_yaw);
			draw.text(4, pos_y, buffer_realyaw, draw.fonts.menu_bold, Color(255, 255, 255));
			pos_y += 10;

			char buffer_fakeyaw[128];
			static float fake_yaw = 0;
			if (game::globals.SendPacket) fake_yaw = game::globals.UserCmd->viewangles.y;
			sprintf_s(buffer_fakeyaw, "Fake yaw: %1.0f", fake_yaw);
			draw.text(4, pos_y, buffer_fakeyaw, draw.fonts.menu_bold, Color(255, 255, 255));
			pos_y += 20;
		}
	}
}

void CResolver::add_corrections() {
	if (m_pEngine->IsInGame() && m_pEngine->IsConnected()) {

		IClientEntity* m_local = game::localdata.localplayer();

		for (int i = 1; i < m_pGlobals->maxClients; i++) {

			auto m_entity = m_pEntityList->GetClientEntity(i);
			if (!m_entity || m_entity == m_local || m_entity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !m_entity->IsAlive()) continue;

			CPlayer* m_player = plist.get_player(i);
			m_player->entity = m_entity;

			bool enable_resolver_y = resolverconfig.bAntiAimCorrection;

			int resolvermode_y = resolverconfig.bAntiAimCorrection ? 1 : 0;
			if (m_player->ForceYaw && m_player->ForceYaw_Yaw) {
				resolvermode_y = m_player->ForceYaw_Yaw;
				enable_resolver_y = true;
			}

			Vector* m_angles = m_entity->GetEyeAngles();
			Vector at_target_angle;

			game::math.calculate_angle(m_entity->GetOrigin(), m_local->GetOrigin(), at_target_angle);
			game::math.normalize_vector(at_target_angle);
			if (enable_resolver_y) {
				if (resolvermode_y == 1) {
					if ( m_entity->GetVelocity( ).Length2D( ) > .1 ) {
						m_player->resolver_data.newer_stored_lby = m_entity->GetLowerBodyYaw( );
					}

					float simtime_delta = m_entity->m_flSimulationTime( ) - m_player->resolver_data.time_at_update;

					if ( m_entity->GetVelocity( ).Length2D( ) > 36 ) {
						m_player->resolver_data.stored_lby = m_entity->GetLowerBodyYaw( );
						m_player->resolver_data.stored_lby_two = m_entity->GetLowerBodyYaw( );
					}

					if ( m_player->resolver_data.old_lowerbody_yaws != m_entity->GetLowerBodyYaw( ) ) {
						m_player->resolver_data.old_yaw_deltas = m_entity->GetLowerBodyYaw( ) - m_player->resolver_data.old_lowerbody_yaws;
						m_player->resolver_data.old_lowerbody_yaws = m_entity->GetLowerBodyYaw( );
						m_player->resolver_data.time_at_update = m_entity->m_flSimulationTime( );

						if ( m_entity->GetVelocity( ).Length2D( ) > 0.1f && ( m_entity->GetFlags( ) & FL_ONGROUND ) ) {
							m_player->resolver_data.temp = m_entity->GetLowerBodyYaw( );
							m_player->resolver_data.old_lowerbody_yaws = m_entity->GetLowerBodyYaw( );
						}
						else {
							m_player->resolver_data.temp = m_player->resolver_data.old_lowerbody_yaws;
						}
					}
					else {
						m_player->resolver_data.temp = m_entity->GetLowerBodyYaw( ) - m_player->resolver_data.old_yaw_deltas;
					}

					float fixed_resolve = m_player->resolver_data.temp;

					if ( m_entity->GetVelocity( ).Length2D( ) > 36 ) {
						m_angles->y = m_entity->GetLowerBodyYaw( );
					} else if ( simtime_delta > 1.525f ) {
						if ( simtime_delta > 1.525f && simtime_delta < 2.25f ) {
							switch ( m_player->resolver_data.shots % 2 ) {
							case 0:m_angles->y = m_player->resolver_data.stored_lby; break;
							case 1:m_angles->y = m_player->resolver_data.newer_stored_lby; break;
							}
						} else if ( simtime_delta > 2.25f && simtime_delta < 2.95f ) {
							m_angles->y = m_entity->GetLowerBodyYaw( );
						} else if ( simtime_delta > 2.95f ) {
							switch ( m_player->resolver_data.shots % 3 ) {
							case 0:m_angles->y = m_player->resolver_data.stored_lby; break;
							case 1:m_angles->y = m_entity->GetLowerBodyYaw( ); break;
							case 2:m_angles->y = m_player->resolver_data.newer_stored_lby; break;
							}
						}
					}
					else if ( simtime_delta <= 1.525f && simtime_delta > 1.125f ) {
						m_angles->y = m_entity->GetLowerBodyYaw( );
					} else {
						if ( simtime_delta <= .57f ) {
							m_player->resolver_data.resolved_yaw = m_entity->GetLowerBodyYaw( );
						} else {
							if ( simtime_delta <= .1 ) {
								m_angles->y = m_entity->GetLowerBodyYaw( );
							} else if ( ( fabs( m_entity->GetLowerBodyYaw( ) - m_player->resolver_data.stored_lby ) ) >= 65 ) {
								if ( ( fabs( m_player->resolver_data.newer_stored_lby - m_player->resolver_data.stored_lby ) ) >= 55 ) {
									m_angles->y = m_player->resolver_data.stored_lby;
								} else {
									switch ( m_player->resolver_data.shots % 2 ) {
									case 0: m_angles->y = m_player->resolver_data.newer_stored_lby; break;
									case 1:	m_angles->y = m_entity->GetLowerBodyYaw( ) + 180; break;
									}
								}
							} else {
								m_angles->y = m_entity->GetLowerBodyYaw( );
							}
						}
						m_angles->y = m_player->resolver_data.resolved_yaw;
					}
				}
				else if (resolvermode_y == 2) m_angles->y = m_entity->GetEyeAngles()->y;
				else if (resolvermode_y == 3) m_angles->y = at_target_angle.y - 90;
				else if (resolvermode_y == 4) m_angles->y = at_target_angle.y + 90;
				else if (resolvermode_y == 5) m_angles->y = at_target_angle.y + 180;
			}

			if (m_player->resolver_data.has_hit_angle) m_angles->y = m_player->resolver_data.last_hit_angle.y;

			if ( resolverconfig.bResolverOverride && GetAsyncKeyState( resolverconfig.iResolverOverrideKey ) ) {
				Vector pos_enemy;
				Vector local_target_angle;
				if ( game::functions.world_to_screen( m_entity->GetOrigin( ), pos_enemy ) ) {
					game::math.calculate_angle( m_local->GetOrigin( ), m_entity->GetOrigin( ), local_target_angle );

					POINT mouse = GUI.GetMouse( );
					float delta = mouse.x - pos_enemy.x;

					if ( delta < 0 ) m_angles->y = local_target_angle.y + 90;
					else m_angles->y = local_target_angle.y - 90;
				}
				
			}

			if (m_player->resolver_data.has_hit_angle && m_player->resolver_data.missed_shots >= 2) {
				m_player->resolver_data.has_hit_angle = false;
				m_player->resolver_data.last_hit_angle = Vector(0, 0, 0);
			}

			if (m_player->resolver_data.missed_shots >= 8) {
				m_player->resolver_data.has_hit_angle = false;
				m_player->resolver_data.last_hit_angle = Vector(0, 0, 0);
				m_player->resolver_data.shots = 0;
				m_player->resolver_data.missed_shots = 0;
			}

			if (!miscconfig.bAntiUntrusted && m_entity == game::globals.Target)
				m_angles->x = game::globals.Shots % 5 == 4 ? -90.f : 90.f;

			if (m_player->ForcePitch && m_player->ForcePitch_Pitch) {
				float angles[] = { -90.f, 90.f };

				m_angles->x = angles[m_player->ForcePitch_Pitch + 1];
			}

			corrections.push_back(CResolverData(i, *m_angles, *m_entity->GetEyeAngles()));
		}
	}
}

void CResolver::apply_corrections(CUserCmd* m_pcmd) {
	if (m_pEngine->IsInGame() && m_pEngine->IsConnected() && game::localdata.localplayer()) {
		for each (CResolverData current in resolver.corrections) {
			IClientEntity* ent = (IClientEntity*)m_pEntityList->GetClientEntity(current.index);
			if (!ent || ent == game::localdata.localplayer() || ent->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !ent->IsAlive()) continue;
			ent->GetEyeAngles()->y = current.realAngles.y;
			ent->GetEyeAngles()->x = current.realAngles.x;
		}
	}
}