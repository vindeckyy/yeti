
#include "ESP.h"
#include "Interfaces.h"
#include "DrawManager.h"
#include <iostream>

#define minimum(a,b)            (((a) < (b)) ? (a) : (b))
#define ccsplayer 35

CEsp esp;
//CVisuals visuals;

float flPlayerAlpha[65];

void CEsp::paint() {
	auto m_local = game::localdata.localplayer();

	if (m_local) {
		for (int i = 0; i < m_pEntityList->GetHighestEntityIndex(); i++) {
			auto m_entity = static_cast<IClientEntity*>(m_pEntityList->GetClientEntity(i));
			if (!m_entity) continue;

			if (m_entity->GetClientClass()->m_ClassID == ccsplayer) {
				if (m_entity->IsDormant() && flPlayerAlpha[i] > 0) flPlayerAlpha[i] -= 5;
				else if (!(m_entity->IsDormant()) && flPlayerAlpha[i] < 255) flPlayerAlpha[i] += 5;
				float alpha = flPlayerAlpha[i];
				game::math.clamp(alpha, 0, 255);

				if (m_entity->IsAlive()) {

					Color plc = Color(get_player_colors(m_entity).r(), get_player_colors(m_entity).g(), get_player_colors(m_entity).b(), alpha);

					Box box; if (!get_box(m_entity, box, visualconfig.player.iEspMode)) {
						player.direction_arrow( m_entity->GetOrigin( ) );
						continue;
					}

					if (!visualconfig.player.bTeammates && m_entity->GetTeamNum() == m_local->GetTeamNum() || m_entity == m_local) continue;

					if (visualconfig.player.iActivationType == 1 && !game::functions.visible(m_local, m_entity, 0)) continue;

					player.paint_player(m_entity, box, plc);
				}
			}
		}

		if ( m_local->IsAlive() && visualconfig.bAntiaimLines ) antiaim_lines( );
	}
}

void CEsp::antiaim_lines( ) {
	auto m_local = game::localdata.localplayer( );

	static float line_length = 80.f;

	Vector lby, fake, real;
	Vector start, end, forward, start_2d, end_2d;

	lby = Vector( 0.f, m_local->GetLowerBodyYaw( ), 0.f );
	fake = Vector( 0.f, game::globals.aa_line_data.fake_angle, 0.f );
	real = Vector( 0.f, game::globals.aa_line_data.real_angle, 0.f );

	start = m_local->GetOrigin( );
	game::math.angle_vectors( lby, forward );
	forward *= line_length;
	end = start + forward;

	if ( !game::functions.world_to_screen( start, start_2d ) || !game::functions.world_to_screen( end, end_2d ) )
		return;

	draw.line( start_2d.x, start_2d.y, end_2d.x, end_2d.y, Color( 0, 255, 0, 255 ) );

	game::math.angle_vectors( fake, forward );
	forward *= line_length;
	end = start + forward;

	if ( !game::functions.world_to_screen( start, start_2d ) || !game::functions.world_to_screen( end, end_2d ) )
		return;

	draw.line( start_2d.x, start_2d.y, end_2d.x, end_2d.y, Color( 255, 0, 0, 255) );

	game::math.angle_vectors( real, forward );
	forward *= line_length;
	end = start + forward;

	if ( !game::functions.world_to_screen( start, start_2d ) || !game::functions.world_to_screen( end, end_2d ) )
		return;

	draw.line( start_2d.x, start_2d.y, end_2d.x, end_2d.y, Color( 255, 255, 0, 255) );
}

void CEsp::CVisualsPlayer::draw_box(IClientEntity* m_entity, Box box, Color color) {
	float alpha = flPlayerAlpha[m_entity->GetIndex()];

	if (visualconfig.player.bBoxFill && visualconfig.player.flBoxFillOpacity > 0) {
		Color color_fill = Color(color.r(), color.g(), color.b(), (alpha / 255) * visualconfig.player.flBoxFillOpacity * 2.55);
		draw.rect(box.x, box.y, box.w, box.h, color_fill);
	}

	if (visualconfig.player.bBox) {
		if (!visualconfig.player.iBoxType) {
			draw.outline(box.x, box.y, box.w, box.h, color);
			if (visualconfig.player.bBoxOutlines) {
				draw.outline(box.x - 1, box.y - 1, box.w + 2, box.h + 2, Color(21, 21, 21, alpha));
				draw.outline(box.x + 1, box.y + 1, box.w - 2, box.h - 2, Color(21, 21, 21, alpha));
			}
		}
		else if (visualconfig.player.iBoxType == 1) {

			float width_corner = box.w / 4;
			float height_corner = width_corner;

			if (visualconfig.player.bBoxOutlines) {
				draw.rect(box.x - 1, box.y - 1, width_corner + 2, 3, Color(21, 21, 21, alpha));
				draw.rect(box.x - 1, box.y - 1, 3, height_corner + 2, Color(21, 21, 21, alpha));

				draw.rect((box.x + box.w) - width_corner - 1, box.y - 1, width_corner + 2, 3, Color(21, 21, 21, alpha));
				draw.rect(box.x + box.w - 1, box.y - 1, 3, height_corner + 2, Color(21, 21, 21, alpha));

				draw.rect(box.x - 1, box.y + box.h - 4, width_corner + 2, 3, Color(21, 21, 21, alpha));
				draw.rect(box.x - 1, (box.y + box.h) - height_corner - 4, 3, height_corner + 2, Color(21, 21, 21, alpha));

				draw.rect((box.x + box.w) - width_corner - 1, box.y + box.h - 4, width_corner + 2, 3, Color(21, 21, 21, alpha));
				draw.rect(box.x + box.w - 1, (box.y + box.h) - height_corner - 4, 3, height_corner + 3, Color(21, 21, 21, alpha));
			}

			draw.rect(box.x, box.y, width_corner, 1, color);
			draw.rect(box.x, box.y, 1, height_corner, color);

			draw.rect((box.x + box.w) - width_corner, box.y, width_corner, 1, color);
			draw.rect(box.x + box.w, box.y, 1, height_corner, color);

			draw.rect(box.x, box.y + box.h - 3, width_corner, 1, color);
			draw.rect(box.x, (box.y + box.h) - height_corner - 3, 1, height_corner, color);

			draw.rect((box.x + box.w) - width_corner, box.y + box.h - 3, width_corner, 1, color);
			draw.rect(box.x + box.w, (box.y + box.h) - height_corner - 3, 1, height_corner + 1, color);
		}
	}
}

void CEsp::CVisualsPlayer::draw_health(IClientEntity* m_entity, Box box) {
	if (visualconfig.player.bHealth) {
		float alpha = flPlayerAlpha[m_entity->GetIndex()];
		int player_health = m_entity->GetHealth() > 100 ? 100 : m_entity->GetHealth();
		
		if (player_health) {
			int color[3] = { 0, 0, 0 };

			if (player_health >= 85) {
				color[0] = 83; color[1] = 200; color[2] = 84;
			} else if (player_health >= 70) {
				color[0] = 107; color[1] = 142; color[2] = 35;
			} else if (player_health >= 55) {
				color[0] = 173; color[1] = 255; color[2] = 47;
			} else if (player_health >= 40) {
				color[0] = 255; color[1] = 215; color[2] = 0;
			} else if (player_health >= 25) {
				color[0] = 255; color[1] = 127; color[2] = 80;
			} else if (player_health >= 10) {
				color[0] = 205; color[1] = 92; color[2] = 92;
			} else if (player_health >= 0) {
				color[0] = 178; color[1] = 34; color[2] = 34;
			}

			if (visualconfig.player.bBoxOutlines)
				draw.outline(box.x - 7, box.y - 1, 4, box.h + 2, Color(21, 21, 21, alpha));

			int health_height = player_health * box.h / 100;
			int add_space = box.h - health_height;

			Color hec = Color(color[0], color[1], color[2], alpha);

			draw.rect(box.x - 6, box.y, 2, box.h, Color(21, 21, 21, alpha));
			draw.rect(box.x - 6, box.y + add_space, 2, health_height, hec);

			if (visualconfig.player.bHealthText && player_health < 100) {
				RECT text_size = draw.get_text_size(std::to_string(player_health).c_str(), draw.fonts.esp_small);
				draw.text(box.x - 5 - (text_size.right / 2), box.y + add_space - (text_size.bottom / 2), std::to_string(player_health).c_str(), draw.fonts.esp_small, Color(255, 255, 255, alpha));
			}
		}
	}
}

void CEsp::CVisualsPlayer::draw_armor(IClientEntity* m_entity, Box box) {
	if (visualconfig.player.bArmor) {
		int player_armor = m_entity->ArmorValue() > 100 ? 100 : m_entity->ArmorValue();
		float alpha = flPlayerAlpha[m_entity->GetIndex()];

		if (player_armor) {
			Color arc = Color(visualconfig.player.cArmorColor, alpha);

			if (visualconfig.player.bBoxOutlines)
				draw.outline(box.x - 1, box.y + box.h + 2, box.w + 2, 4, Color(21, 21, 21, alpha));

			int armor_width = player_armor * box.w / 100;

			draw.rect(box.x, box.y + box.h + 3, box.w, 2, Color(21, 21, 21, alpha));
			draw.rect(box.x, box.y + box.h + 3, armor_width, 2, arc);
		}
	}
}

void CEsp::CVisualsPlayer::draw_name(IClientEntity* m_entity, Box box) {
	if (visualconfig.player.bPlayerNames) {
		float alpha = flPlayerAlpha[m_entity->GetIndex()];

		player_info_t player_info;
		if (m_pEngine->GetPlayerInfo(m_entity->GetIndex(), &player_info)) {
			RECT name_size = draw.get_text_size(player_info.name, draw.fonts.esp);
			draw.text(box.x + (box.w / 2) - (name_size.right / 2), box.y - 14, player_info.name, draw.fonts.esp, Color(225, 225, 225, alpha));
		}
	}
}

void CEsp::CVisualsPlayer::draw_weapon(IClientEntity* m_entity, Box box) {
	float alpha = flPlayerAlpha[m_entity->GetIndex()];

	int bottom_pos = 0;
	bool wa_enabled = false;
	CBaseCombatWeapon* weapon = m_entity->GetWeapon();

	if (weapon) {
		int player_armor = m_entity->ArmorValue() > 100 ? 100 : m_entity->ArmorValue();
		bool armor = visualconfig.player.bArmor && player_armor;
		if (visualconfig.player.bWeaponNames || visualconfig.player.bPlayerAmmo) {
			char buffer[128];
			char* format = "";
			if (visualconfig.player.bWeaponNames && !visualconfig.player.bPlayerAmmo) {
				format = XorStr("%s");
				sprintf_s(buffer, format, weapon->GetGunName());
				wa_enabled = true;
			} else if (!visualconfig.player.bWeaponNames && visualconfig.player.bPlayerAmmo) {
				format = XorStr("%1.0f");
				float ammo = weapon->GetAmmoInClip();
				sprintf_s(buffer, format, ammo);
				wa_enabled = true;
			} else if (visualconfig.player.bWeaponNames && visualconfig.player.bPlayerAmmo) {
				format = XorStr("%s | %1.0f");
				float ammo = weapon->GetAmmoInClip();
				sprintf_s(buffer, format, weapon->GetGunName(), ammo);
				wa_enabled = true;
			}

			RECT size = draw.get_text_size(buffer, draw.fonts.esp_extra);
			draw.text(box.x + (box.w / 2) - (size.right / 2), box.y + box.h + (armor ? 5 : 2), buffer, draw.fonts.esp_extra, Color(225, 225, 225, alpha));
			bottom_pos += 1;
		}
	}
}

void CEsp::CVisualsPlayer::draw_hit_angle(IClientEntity* m_entity, Box box) {
	if (visualconfig.player.bHitAngle) {
		float alpha = flPlayerAlpha[m_entity->GetIndex()];
		CPlayer* m_player = plist.get_player(m_entity->GetIndex());
		RECT size = draw.get_text_size(XorStr("HA"), draw.fonts.esp_extra );

		bool draw_scoped = (visualconfig.player.bScoped && m_entity->IsScoped( ));
		draw.text( ( box.x + box.w ) + 3, box.y + (draw_scoped ? 13 : 0), XorStr("HA"), draw.fonts.esp_extra, m_player->resolver_data.has_hit_angle ? Color(55, 255, 55, alpha) : Color(255, 255, 255, alpha));
	}
}

void CEsp::CVisualsPlayer::draw_skeletons(IClientEntity* m_entity, Box box) {
	if (visualconfig.player.bSkeletons) {
		float alpha = flPlayerAlpha[m_entity->GetIndex()];
		studiohdr_t* studio_hdr = m_pModelInfo->GetStudioModel(m_entity->GetModel());

		if (!studio_hdr)
			return;

		Vector vParent, vChild, sParent, sChild;

		for (int j = 0; j < studio_hdr->numbones; j++) {
			mstudiobone_t* pBone = studio_hdr->GetBone(j);

			if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1)) {
				vChild = m_entity->GetBonePos(j);
				vParent = m_entity->GetBonePos(pBone->parent);

				if (game::functions.world_to_screen(vParent, sParent) && game::functions.world_to_screen(vChild, sChild)) {
					draw.line(sParent[0], sParent[1], sChild[0], sChild[1], Color(visualconfig.player.cSkeletonsColor, alpha));
				}
			}
		}
	}
}

void CEsp::CVisualsPlayer::draw_snaplines(IClientEntity* m_entity, Box box) {
	if (visualconfig.player.bSnapLines) {
		float alpha = flPlayerAlpha[m_entity->GetIndex()];
		if (box.x >= 0 && box.y >= 0) {
			int width = 0;
			int height = 0;

			Vector to = Vector(box.x + (box.w / 2), box.y + box.h, 0);
			m_pEngine->GetScreenSize(width, height);

			Vector From((width / 2), (height / 2), 0);
			draw.line(From.x, From.y, to.x, to.y, Color(visualconfig.player.cSnaplinesColor, alpha));
		}
	}
}

void CEsp::CVisualsPlayer::draw_hitbones(IClientEntity* m_entity) {
	if (visualconfig.player.bHitbones) {
		float alpha = flPlayerAlpha[m_entity->GetIndex()];
		for (int i = 0; i < 19; i++) {
			Vector screen_position;
			Vector hitbone_pos = game::functions.get_hitbox_location(m_entity, i);
			if (game::functions.world_to_screen(hitbone_pos, screen_position)) {
				draw.rect(screen_position.x, screen_position.y, 4, 4, Color(21, 21, 21, alpha));
				draw.rect(screen_position.x + 1, screen_position.y + 1, 2, 2, Color(visualconfig.player.cHitbonesColor, alpha));
			}
		}
	}
}

void CEsp::CVisualsPlayer::draw_scoped(IClientEntity* m_entity, Box box) {
	if (visualconfig.player.bScoped) {
		float alpha = flPlayerAlpha[m_entity->GetIndex()];

		if (m_entity->IsScoped()) {
			draw.text((box.x + box.w) + 3, box.y, "SCOPED", draw.fonts.esp_extra, Color(255, 99, 71, alpha));
		}
	}
}

void CEsp::CVisualsPlayer::direction_arrow( const Vector& origin ) {
	if ( visualconfig.player.bDirectionArrow ) {
		constexpr float radius = 360.0f;
		int width, height;
		m_pEngine->GetScreenSize( width, height );

		Vector vRealAngles;
		m_pEngine->GetViewAngles( vRealAngles );

		Vector vForward, vRight, vUp( 0.0f, 0.0f, 1.0f );

		game::math.angle_vectors( vRealAngles, &vForward, NULL, NULL );

		vForward.z = 0.0f;
		VectorNormalize( vForward );
		CrossProduct( vUp, vForward, vRight );

		float flFront = DotProduct( origin, vForward );
		float flSide = DotProduct( origin, vRight );
		float flXPosition = radius * -flSide;
		float flYPosition = radius * -flFront;

		float rotation = game::globals.UserCmd->viewangles.y;

		rotation = atan2( flXPosition, flYPosition ) + M_PI;
		rotation *= 180.0f / M_PI;

		float flYawRadians = -( rotation )* M_PI / 180.0f;
		float flCosYaw = cos( flYawRadians );
		float flSinYaw = sin( flYawRadians );

		flXPosition = ( int )( ( width / 2.0f ) + ( radius * flSinYaw ) );
		flYPosition = ( int )( ( height / 2.0f ) - ( radius * flCosYaw ) );

		draw.rect( flXPosition, flYPosition, 10, 10, Color( visualconfig.player.cDirectionArrowColor ) );
	}
}

void CEsp::CGlow::shutdown() {
	for (auto i = 0; i < m_pGlowObjManager->size; i++) {
		auto& glow_object = m_pGlowObjManager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<IClientEntity*>(glow_object.m_pEntity);

		if (glow_object.IsUnused())
			continue;

		if (!entity || entity->IsDormant())
			continue;

		glow_object.m_flGlowAlpha = 0.0f;
	}
}

void CEsp::CGlow::paint()
{
	auto m_local = game::localdata.localplayer();
	for (auto i = 0; i < m_pGlowObjManager->size; i++) {
		auto glow_object = &m_pGlowObjManager->m_GlowObjectDefinitions[i];

		IClientEntity *m_entity = glow_object->m_pEntity;

		if (!glow_object->m_pEntity || glow_object->IsUnused())
			continue;

		if (m_entity->GetClientClass()->m_ClassID == 35) {
			if (m_entity->GetTeamNum() == m_local->GetTeamNum() && !visualconfig.player.bTeammates || visualconfig.player.bTeammates) continue;

			bool m_visible = game::functions.visible(m_local, m_entity, 0);
			float m_flRed = visualconfig.player.cGlowColor[0], m_flGreen = visualconfig.player.cGlowColor[1], m_flBlue = visualconfig.player.cGlowColor[2];
			bool m_teammate = m_entity->GetTeamNum() == m_local->GetTeamNum();

			glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
			glow_object->m_flGlowAlpha = (visualconfig.player.flGlowOpacity * 2.55) / 255;
			glow_object->m_bRenderWhenOccluded = true;
			glow_object->m_bRenderWhenUnoccluded = false;
		}
	}
}