#include "Legitbot.h"

CLegitbot legitbot;

void CLegitbot::run(CUserCmd* m_pcmd, bool& send_packet)
{
	auto m_local = game::localdata.localplayer();

	if (m_local && m_local->IsAlive())
	{
		sync();

		if (!active) return;

		IClientEntity* m_target = nullptr;
		bool find_target = true;

		auto m_weapon = m_local->GetWeapon();
		if (m_weapon)
		{
			if (m_weapon->GetAmmoInClip() == 0 || m_weapon->IsKnife()) return;
			sync();
		}
		else return;

		if (!game::functions.can_shoot()) return;

		if (locked && target_id >= 0 && hitbox >= 0)
		{
			m_target = m_pEntityList->GetClientEntity(target_id);
			if (m_target  && viable(m_target))
			{
				sync();
				if (hitbox >= 0)
				{
					Vector ViewOffset = m_target->GetOrigin() + m_target->GetViewOffset();
					Vector View; m_pEngine->GetViewAngles(View);
					View += m_local->localPlayerExclusive()->GetAimPunchAngle() * recoil;
					float nFoV = distance(ViewOffset, View, m_target, hitbox);
					if (nFoV < fov)
						find_target = false;
				}
			}
		}
		if (find_target)
		{
			target_id = 0;
			m_target = nullptr;
			hitbox = -1;
			target_id = get_target();
			if (target_id >= 0) m_target = m_pEntityList->GetClientEntity(target_id);
			else
			{
				m_target = nullptr;
				hitbox = -1;
			}
		}
		sync();
		if (target_id >= 0 && m_target)
		{
			sync();
			if (keypress)
			{
				if (key >= 0 && !GUI.GetKeyState(key))
				{
					target_id = -1;
					m_target = nullptr;
					hitbox = -1;
					return;
				}
			}

			Vector point;
			if (multi_hitbox) point = game::functions.get_hitbox_location(m_target, besthitbox);
			else point = game::functions.get_hitbox_location(m_target, hitbox);
			if (aim(point, m_pcmd))
			{
				if (autofire && !(m_pcmd->buttons & IN_ATTACK))
				{
					m_pcmd->buttons |= IN_ATTACK;
				}
			}
		}
	}
}

bool CLegitbot::viable(IClientEntity* m_player)
{
	auto m_local = game::localdata.localplayer();
	if (!m_player) return false;
	if (m_player->IsDormant()) return false;
	if (!m_player->IsAlive()) return false;
	if (m_player->GetIndex() == m_local->GetIndex()) return false;

	ClientClass *pClientClass = m_player->GetClientClass();
	player_info_t pinfo;

	if (pClientClass->m_ClassID != (int)CSGOClassID::CCSPlayer) return false;
	if (!m_pEngine->GetPlayerInfo(m_player->GetIndex(), &pinfo)) return false;
	if (m_player->GetTeamNum() == m_local->GetTeamNum()) return false;
	if (m_player->m_bGunGameImmunity()) return false;

	return true;
}

void CLegitbot::sync()
{
	auto m_local = game::localdata.localplayer();
	auto m_weapon = m_local->GetWeapon();

	if (!m_weapon) return;

	LegitConfig config = legitconfig;

	if (m_weapon->m_bIsPistol())
	{
		active = config.pistol.bActivated;
		smoothing = config.pistol.flSmoothing;
		fov = config.pistol.flFOV * 2;
		recoil = config.pistol.flRecoil;
		int speeds[] = { 20, 15, 10, 5, 0 };
		delay = speeds[config.pistol.iReactionTime];
		keypress = config.pistol.iAutoFireMode;
		key = config.pistol.iAutoFireKey;
		autofire = config.pistol.bAutoFire;

		switch (config.pistol.iAutoFireTarget)
		{
		case 0:
			hitbox = ((int)CSGOHitboxID::Head);
			multi_hitbox = false;
			break;
		case 1:
			hitbox = ((int)CSGOHitboxID::Neck);
			multi_hitbox = false;
			break;
		case 2:
			hitbox = ((int)CSGOHitboxID::Chest);
			multi_hitbox = false;
			break;
		case 3:
			hitbox = ((int)CSGOHitboxID::Stomach);
			multi_hitbox = false;
			break;
		case 4:
			hitbox = ((int)CSGOHitboxID::Pelvis);
			multi_hitbox = false;
			break;
		case 5:
			hitbox = ((int)CSGOHitboxID::LeftLowerArm);
			multi_hitbox = false;
			break;
		case 6:
			hitbox = ((int)CSGOHitboxID::RightLowerArm);
			multi_hitbox = false;
			break;
		case 7:
			hitbox = ((int)CSGOHitboxID::LeftShin);
			multi_hitbox = false;
			break;
		case 8:
			hitbox = ((int)CSGOHitboxID::RightShin);
			multi_hitbox = false;
			break;
		}
	}
	else if (m_weapon->m_bIsSmg())
	{
		active = config.smg.bActivated;
		smoothing = config.smg.flSmoothing;
		fov = config.smg.flFOV * 2;
		recoil = config.smg.flRecoil;
		int speeds[] = { 20, 15, 10, 5, 0 };
		delay = speeds[config.smg.iReactionTime];
		keypress = config.smg.iAutoFireMode;
		key = config.smg.iAutoFireKey;
		autofire = config.smg.bAutoFire;

		switch (config.smg.iAutoFireTarget)
		{
		case 0:
			hitbox = ((int)CSGOHitboxID::Head);
			multi_hitbox = false;
			break;
		case 1:
			hitbox = ((int)CSGOHitboxID::Neck);
			multi_hitbox = false;
			break;
		case 2:
			hitbox = ((int)CSGOHitboxID::Chest);
			multi_hitbox = false;
			break;
		case 3:
			hitbox = ((int)CSGOHitboxID::Stomach);
			multi_hitbox = false;
			break;
		case 4:
			hitbox = ((int)CSGOHitboxID::Pelvis);
			multi_hitbox = false;
			break;
		case 5:
			hitbox = ((int)CSGOHitboxID::LeftLowerArm);
			multi_hitbox = false;
			break;
		case 6:
			hitbox = ((int)CSGOHitboxID::RightLowerArm);
			multi_hitbox = false;
			break;
		case 7:
			hitbox = ((int)CSGOHitboxID::LeftShin);
			multi_hitbox = false;
			break;
		case 8:
			hitbox = ((int)CSGOHitboxID::RightShin);
			multi_hitbox = false;
			break;
		}
	}
	else if (m_weapon->m_bIsShotgun())
	{
		active = config.shotgun.bActivated;
		smoothing = config.shotgun.flSmoothing;
		fov = config.shotgun.flFOV * 2;
		recoil = config.shotgun.flRecoil;
		int speeds[] = { 20, 15, 10, 5, 0 };
		delay = speeds[config.shotgun.iReactionTime];
		keypress = config.shotgun.iAutoFireMode;
		key = config.shotgun.iAutoFireKey;
		autofire = config.shotgun.bAutoFire;

		switch (config.shotgun.iAutoFireTarget)
		{
		case 0:
			hitbox = ((int)CSGOHitboxID::Head);
			multi_hitbox = false;
			break;
		case 1:
			hitbox = ((int)CSGOHitboxID::Neck);
			multi_hitbox = false;
			break;
		case 2:
			hitbox = ((int)CSGOHitboxID::Chest);
			multi_hitbox = false;
			break;
		case 3:
			hitbox = ((int)CSGOHitboxID::Stomach);
			multi_hitbox = false;
			break;
		case 4:
			hitbox = ((int)CSGOHitboxID::Pelvis);
			multi_hitbox = false;
			break;
		case 5:
			hitbox = ((int)CSGOHitboxID::LeftLowerArm);
			multi_hitbox = false;
			break;
		case 6:
			hitbox = ((int)CSGOHitboxID::RightLowerArm);
			multi_hitbox = false;
			break;
		case 7:
			hitbox = ((int)CSGOHitboxID::LeftShin);
			multi_hitbox = false;
			break;
		case 8:
			hitbox = ((int)CSGOHitboxID::RightShin);
			multi_hitbox = false;
			break;
		}
	}
	else if (m_weapon->m_bIsSniper())
	{
		active = config.sniper.bActivated;
		smoothing = config.sniper.flSmoothing;
		fov = config.sniper.flFOV * 2;
		recoil = config.sniper.flRecoil;
		int speeds[] = { 20, 15, 10, 5, 0 };
		delay = speeds[config.sniper.iReactionTime];
		keypress = config.sniper.iAutoFireMode;
		key = config.sniper.iAutoFireKey;
		autofire = config.sniper.bAutoFire;

		switch (config.sniper.iAutoFireTarget)
		{
		case 0:
			hitbox = ((int)CSGOHitboxID::Head);
			multi_hitbox = false;
			break;
		case 1:
			hitbox = ((int)CSGOHitboxID::Neck);
			multi_hitbox = false;
			break;
		case 2:
			hitbox = ((int)CSGOHitboxID::Chest);
			multi_hitbox = false;
			break;
		case 3:
			hitbox = ((int)CSGOHitboxID::Stomach);
			multi_hitbox = false;
			break;
		case 4:
			hitbox = ((int)CSGOHitboxID::Pelvis);
			multi_hitbox = false;
			break;
		case 5:
			hitbox = ((int)CSGOHitboxID::LeftLowerArm);
			multi_hitbox = false;
			break;
		case 6:
			hitbox = ((int)CSGOHitboxID::RightLowerArm);
			multi_hitbox = false;
			break;
		case 7:
			hitbox = ((int)CSGOHitboxID::LeftShin);
			multi_hitbox = false;
			break;
		case 8:
			hitbox = ((int)CSGOHitboxID::RightShin);
			multi_hitbox = false;
			break;
		}
	}
	else
	{
		active = config.rifle.bActivated;
		smoothing = config.rifle.flSmoothing;
		fov = config.rifle.flFOV * 2;
		recoil = config.rifle.flRecoil;
		int speeds[] = { 20, 15, 10, 5, 0 };
		delay = speeds[config.rifle.iReactionTime];
		keypress = config.rifle.iAutoFireMode;
		key = config.rifle.iAutoFireKey;
		autofire = config.rifle.bAutoFire;

		switch (config.rifle.iAutoFireTarget)
		{
		case 0:
			hitbox = ((int)CSGOHitboxID::Head);
			multi_hitbox = false;
			break;
		case 1:
			hitbox = ((int)CSGOHitboxID::Neck);
			multi_hitbox = false;
			break;
		case 2:
			hitbox = ((int)CSGOHitboxID::Chest);
			multi_hitbox = false;
			break;
		case 3:
			hitbox = ((int)CSGOHitboxID::Stomach);
			multi_hitbox = false;
			break;
		case 4:
			hitbox = ((int)CSGOHitboxID::Pelvis);
			multi_hitbox = false;
			break;
		case 5:
			hitbox = ((int)CSGOHitboxID::LeftLowerArm);
			multi_hitbox = false;
			break;
		case 6:
			hitbox = ((int)CSGOHitboxID::RightLowerArm);
			multi_hitbox = false;
			break;
		case 7:
			hitbox = ((int)CSGOHitboxID::LeftShin);
			multi_hitbox = false;
			break;
		case 8:
			hitbox = ((int)CSGOHitboxID::RightShin);
			multi_hitbox = false;
			break;
		}
	}
}

float distance_3d(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

float CLegitbot::distance(Vector offset, Vector view, IClientEntity* m_player, int hitbox)
{
	CONST FLOAT MaxDegrees = 180.0f;
	Vector Angles = view;
	Vector Origin = offset;
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);
	game::math.angle_vectors(Angles, &Forward);
	Vector AimPos = game::functions.get_hitbox_location(m_player, hitbox);
	VectorSubtract(AimPos, Origin, Delta);
	game::math.normalize(Delta, Delta);
	FLOAT DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}

bool CLegitbot::aim(Vector point, CUserCmd *pCmd)
{
	auto m_local = game::localdata.localplayer();
	if (point.Length() == 0) return false;
	Vector angles;
	if (!game::functions.visible(m_local, m_pEntityList->GetClientEntity(target_id), hitbox)) return false;
	Vector src = m_local->GetOrigin() + m_local->GetViewOffset();
	game::math.calculate_angle(src, point, angles);
	game::math.normalize_vector(angles);
	if (recoil)
	{
		Vector AimPunch = m_local->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * 2;
			game::math.normalize_vector(angles);
		}
	}
	locked = true;
	Vector delta = angles - pCmd->viewangles;
	bool return_data = false;
	if (delta.Length() > smoothing)
	{
		game::math.normalize(delta, delta);
		delta *= smoothing;
	}
	else return_data = true;
	pCmd->viewangles += delta;
	m_pEngine->SetViewAngles(pCmd->viewangles);
	return return_data;
}

int CLegitbot::get_target()
{
	sync();
	int target = -1;
	float minFoV = fov;

	IClientEntity* m_local = game::localdata.localplayer();
	Vector offset = m_local->GetOrigin() + m_local->GetViewOffset();
	Vector view; m_pEngine->GetViewAngles(view);
	view += m_local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	for (int i = 1; i < m_pGlobals->maxClients; i++)
	{
		IClientEntity *m_entity = m_pEntityList->GetClientEntity(i);
		if (viable(m_entity))
		{
			if (hitbox >= 0)
			{
				float dist = distance(offset, view, m_entity, 0);
				if (!game::functions.visible(m_local, m_entity, hitbox)) continue;
				if (dist < minFoV)
				{
					minFoV = dist;
					target = i;
				}
			}
		}
	}

	return target;
}