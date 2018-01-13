#include "Hooks.h"

void __fastcall Hooks::Hooked_SceneEnd( void *ecx, void *edx ) {
	g_fnOriginalSceneEnd(ecx, edx);

	auto m_local = game::localdata.localplayer();

	if ( m_local && m_local->IsAlive( ) ) {
		for ( int i = 1; i <= m_pGlobals->maxClients; i++ ) {
			auto m_entity = m_pEntityList->GetClientEntity( i );

			if ( m_entity && m_entity != m_local && m_entity->GetClientClass( )->m_ClassID == ( int )CSGOClassID::CCSPlayer && m_entity->IsAlive( ) && !m_entity->IsDormant( ) ) {
				int teamnum = m_entity->GetTeamNum( );

				bool flat = visualconfig.colored_models.iPlayerChamType == 2;
				bool wireframe = visualconfig.colored_models.iPlayerChamType == 3;
				bool glass = visualconfig.colored_models.iPlayerChamType == 4;

				if ( visualconfig.colored_models.bEnemiesNoVis && teamnum != m_local->GetTeamNum( ) ) {
					Color color = Color( visualconfig.colored_models.cEnemiesNonVisible );
					chams->override_material( true, flat, wireframe, glass, color );
					m_entity->DrawModel( 0x1, m_entity->GetModelInstance( ) );
					m_pModelRender->ForcedMaterialOverride( nullptr );
				}
				else if ( visualconfig.colored_models.bTeammatesNoVis && teamnum == m_local->GetTeamNum( ) ) {
					Color color = Color( visualconfig.colored_models.cTeammatesNonVisible );
					chams->override_material( true, flat, wireframe, glass, color );
					m_entity->DrawModel( 0x1, m_entity->GetModelInstance( ) );
					m_pModelRender->ForcedMaterialOverride( nullptr );
				}

				if ( visualconfig.colored_models.bEnemies && teamnum != m_local->GetTeamNum( ) ) {
					Color color = Color( visualconfig.colored_models.cEnemiesVisible );
					chams->override_material( false, flat, wireframe, glass, color );
					m_entity->DrawModel( 0x1, m_entity->GetModelInstance( ) );
					m_pModelRender->ForcedMaterialOverride( nullptr );
				}
				else if ( visualconfig.colored_models.bTeammates && teamnum == m_local->GetTeamNum( ) ) {
					Color color = Color( visualconfig.colored_models.cTeammatesVisible );
					chams->override_material( false, flat, wireframe, glass, color );
					m_entity->DrawModel( 0x1, m_entity->GetModelInstance( ) );
					m_pModelRender->ForcedMaterialOverride( nullptr );

				}
			}
		}

		if ( visualconfig.bGhostChams ) {
			auto original_angle = *m_local->GetEyeAngles( );
			m_local->SetAngle2( Vector( 0, game::globals.fake_angle.y, 0 ) );
			m_local->UpdateClientSideAnimation( );
			chams->override_material( false, false, false, false, Color( 255, 255, 255, 255 ) );
			m_local->DrawModel( 0x1, m_local->GetModelInstance( ) );
			m_pModelRender->ForcedMaterialOverride( nullptr );
			m_local->SetAngle2( original_angle );
			m_local->UpdateClientSideAnimation( );
		}
	}
}