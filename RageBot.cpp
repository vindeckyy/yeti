
#include "RageBot.h"
#include "DrawManager.h"
#include "Autowall.h"
#include <iostream>
#include "Resolver.h"

#define M_PI_F ((float)(M_PI))

CRagebot ragebot;

void CRagebot::run(CUserCmd *pCmd, bool& bSendPacket)
{
	if (m_pEngine->IsConnected() && m_pEngine->IsInGame())
	{ 
		auto m_local = game::localdata.localplayer();
		if (!m_local || !m_local->IsAlive()) return;

		auto m_weapon = m_local->GetWeapon();
		if (m_weapon)
		{
			aa.Manage(pCmd, bSendPacket);

			if (m_weapon->GetAmmoInClip())
			{
				if (ragebotconfig.bEnabled)
				{
					if (m_local->IsAlive())
					{
						aimbot.aimbotted_in_current_tick = false;
						if (game::functions.can_shoot()) {
							aimbot.DoAimbot(pCmd, bSendPacket);
						}
						if (!aimbot.aimbotted_in_current_tick) aimbot.auto_revolver(pCmd);

						RemoveRecoil(pCmd);
					}
				}
			}
			else
			{
				pCmd->buttons &= ~IN_ATTACK;
				pCmd->buttons |= IN_RELOAD;
			}

			if (game::globals.Target)
			{
				if (ragebotconfig.bAutostop)
				{
					if (m_local->GetVelocity().Length() > 0)
					{
						if (!(ragebotconfig.iAutostopType))
						{
							pCmd->forwardmove = 0.f;
							pCmd->sidemove = 0.f;
						}
						else if (ragebotconfig.iAutostopType == 1)
						{
							Vector velocity = m_local->GetVelocity();
							Vector direction = velocity.Angle();
							float speed = velocity.Length();

							direction.y = pCmd->viewangles.y - direction.y;

							Vector negated_direction = direction.Forward() * -speed;

							pCmd->forwardmove = negated_direction.x;
							pCmd->sidemove = negated_direction.y;
						}
					}

					if ((pCmd->buttons & IN_FORWARD))
						pCmd->buttons &= ~IN_FORWARD;

					if ((pCmd->buttons & IN_LEFT))
						pCmd->buttons &= ~IN_LEFT;

					if ((pCmd->buttons & IN_RIGHT))
						pCmd->buttons &= ~IN_RIGHT;

					if ((pCmd->buttons & IN_BACK))
						pCmd->buttons &= ~IN_BACK;

					if ((pCmd->buttons & IN_JUMP))
						pCmd->buttons &= ~IN_JUMP;
				}
			}
		}
		if (miscconfig.bAntiUntrusted)
			game::math.normalize_vector(pCmd->viewangles);
	}
}
bool CanWallbang(Vector& EndPos, float &Damage)
{

	if (CanHit(EndPos, &Damage))
	{
		return true;
	}
	return false;
}
Vector GetBestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* m_local = game::localdata.localplayer();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	m_pTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}
void VectorAngles1(const Vector& forward, Vector &angles) {
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

float hitchance() {
	auto m_local = game::localdata.localplayer();
	auto m_weapon = m_local->GetWeapon();

	float hitchance = 101;
	if (!m_weapon) return 0;
	if (ragebotconfig.bHitchance && ragebotconfig.flHitchanceAmt > 1)
	{
		float inaccuracy = m_weapon->GetCone();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;
}

void CRagebot::RemoveRecoil(CUserCmd* pCmd)
{
	if (!ragebotconfig.bRemoveRecoil) return;

	IClientEntity* m_local = game::localdata.localplayer();
	if (m_local)
	{
		Vector AimPunch = m_local->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			auto weapon_recoil_scale = m_pCVar->FindVar(XorStr("weapon_recoil_scale"));
			*(int*)((DWORD)&weapon_recoil_scale->fnChangeCallback + 0xC) = 0;
			pCmd->viewangles -= AimPunch * weapon_recoil_scale->GetFloat();
			game::math.normalize_vector(pCmd->viewangles);
		}
	}
}

int Aimbot::GetTargetFOV()
{
	auto m_local = game::localdata.localplayer();

	int target = -1;
	float mfov = ragebotconfig.flMaxFov;

	Vector viewoffset = m_local->GetOrigin() + m_local->GetViewOffset();
	Vector view; m_pEngine->GetViewAngles(view);

	for (int i = 0; i < m_pEntityList->GetMaxEntities(); i++)
	{
		IClientEntity* pEntity = m_pEntityList->GetClientEntity(i);

		if (IsViable(pEntity))
		{
			int newhb = HitScan(pEntity);
			if (newhb >= 0)
			{
				float fov = FovToPlayer(viewoffset, view, pEntity, 0);
				CPlayer* Player = plist.get_player(i);
				if (fov < mfov || (Player->Priority && fov <= 180))
				{
					mfov = fov;
					target = i;
				}
			}
		}
	}

	return target;
}

int Aimbot::GetTargetDistance()
{
	auto m_local = game::localdata.localplayer();

	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = m_local;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; m_pEngine->GetViewAngles(View);

	for (int i = 0; i < m_pEntityList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = m_pEntityList->GetClientEntity(i);
		if (IsViable(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				CPlayer* Player = plist.get_player(i);
				if ((Distance < minDist || Player->Priority) && fov < ragebotconfig.flMaxFov)
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}

	return target;
}

int Aimbot::GetTargetHealth()
{
	auto m_local = game::localdata.localplayer();

	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = m_local;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; m_pEngine->GetViewAngles(View);

	for (int i = 0; i < m_pEntityList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = m_pEntityList->GetClientEntity(i);
		if (IsViable(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				CPlayer* Player = plist.get_player(i);
				if ((Health < minHealth || Player->Priority) && fov < ragebotconfig.flMaxFov)
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}

	return target;
}

bool Aimbot::IsViable(IClientEntity* pEntity)
{
	auto m_local = game::localdata.localplayer();
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != m_local->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && m_pEngine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != m_local->GetTeamNum() || ragebotconfig.bFriendlyFire)
			{
				if (!pEntity->m_bGunGameImmunity())
				{
					CPlayer* Player = plist.get_player(pEntity->GetIndex());
					if (!Player->Friendly)
						return true;
				}
			}
		}
	}

	return false;
}

void Aimbot::auto_revolver(CUserCmd* m_pcmd) {
	if (ragebotconfig.iAutoFireRevolverMode == 1) {
		auto m_local = game::localdata.localplayer();
		auto m_weapon = m_local->GetWeapon();

		if (m_weapon) {
			if (*m_weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER) {
				m_pcmd->buttons |= IN_ATTACK;
				float flPostponeFireReady = m_weapon->GetFireReadyTime();
				if (flPostponeFireReady > 0 && flPostponeFireReady - 1 < m_pGlobals->curtime) {
					m_pcmd->buttons &= ~IN_ATTACK;
				}
			}
		}
	}
}

float Aimbot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;
	Vector Angles = View;
	Vector Origin = ViewOffSet;
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);
	game::math.angle_vectors(Angles, Forward);
	Vector AimPos = game::functions.get_hitbox_location(pEntity, aHitBox);
	game::math.vector_subtract(AimPos, Origin, Delta);
	game::math.normalize(Delta, Delta);
	FLOAT DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int Aimbot::HitScan(IClientEntity* m_entity)
{
	IClientEntity* m_local = game::localdata.localplayer();
	CPlayer* m_player = plist.get_player( m_entity->GetIndex());
	std::vector<int> hitboxes;
	std::vector<int> baim_hitboxes;
	bool AWall = ragebotconfig.bAutoWall;
	int hbox = ragebotconfig.iAutoFireHitbox;

	bool bRevert = true;

	for (int i = 0; i < hitscanconfig.hitboxes.size(); i++)
	{
		if (hitscanconfig.hitboxes[i].bselected)
			bRevert = false;
	}

	std::vector<MultiBoxItem> custom_hitbones = hitscanconfig.hitboxes;

	baim_hitboxes.push_back((int)CSGOHitboxID::UpperChest);
	baim_hitboxes.push_back((int)CSGOHitboxID::Chest);
	baim_hitboxes.push_back((int)CSGOHitboxID::LowerChest);
	baim_hitboxes.push_back((int)CSGOHitboxID::Stomach);

	bool canseebody = false;
	float bodydmg;

	for (auto HitBoxID : baim_hitboxes)
	{
		Vector Point = game::functions.get_hitbox_location(m_entity, HitBoxID);
		if (CanHit(Point, &bodydmg))
		{
			canseebody = true;
		}
	}

	if (bRevert)
	{
		switch (hbox)
		{
		case 0: hitboxes.push_back((int)CSGOHitboxID::Head);
		case 1: hitboxes.push_back((int)CSGOHitboxID::Neck);
		case 2: hitboxes.push_back((int)CSGOHitboxID::Chest);
		case 3: hitboxes.push_back((int)CSGOHitboxID::Stomach);
		case 4: hitboxes.push_back((int)CSGOHitboxID::Pelvis);
		case 5: hitboxes.push_back((int)CSGOHitboxID::LeftLowerArm);
		case 6: hitboxes.push_back((int)CSGOHitboxID::RightLowerArm);
		case 7: hitboxes.push_back((int)CSGOHitboxID::LeftShin);
		case 8: hitboxes.push_back((int)CSGOHitboxID::RightShin);
		}
	}
	else
	{
		if (custom_hitbones[0].bselected)
			hitboxes.push_back((int)CSGOHitboxID::Head);

		if (custom_hitbones[1].bselected)
			hitboxes.push_back((int)CSGOHitboxID::Neck);

		if (custom_hitbones[2].bselected)
			hitboxes.push_back((int)CSGOHitboxID::NeckLower);

		if (custom_hitbones[3].bselected)
			hitboxes.push_back((int)CSGOHitboxID::UpperChest);

		if (custom_hitbones[4].bselected)
			hitboxes.push_back((int)CSGOHitboxID::Chest);

		if (custom_hitbones[5].bselected)
			hitboxes.push_back((int)CSGOHitboxID::LowerChest);

		if (custom_hitbones[6].bselected)
			hitboxes.push_back((int)CSGOHitboxID::Stomach);

		if (custom_hitbones[7].bselected)
			hitboxes.push_back((int)CSGOHitboxID::Pelvis);

		if (custom_hitbones[8].bselected)
			hitboxes.push_back((int)CSGOHitboxID::LeftUpperArm);

		if (custom_hitbones[9].bselected)
			hitboxes.push_back((int)CSGOHitboxID::LeftLowerArm);

		if (custom_hitbones[10].bselected)
			hitboxes.push_back((int)CSGOHitboxID::LeftHand);

		if (custom_hitbones[11].bselected)
			hitboxes.push_back((int)CSGOHitboxID::RightUpperArm);

		if (custom_hitbones[12].bselected)
			hitboxes.push_back((int)CSGOHitboxID::RightLowerArm);

		if (custom_hitbones[13].bselected)
			hitboxes.push_back((int)CSGOHitboxID::RightHand);

		if (custom_hitbones[14].bselected)
			hitboxes.push_back((int)CSGOHitboxID::LeftThigh);

		if (custom_hitbones[15].bselected)
			hitboxes.push_back((int)CSGOHitboxID::LeftShin);

		if (custom_hitbones[16].bselected)
			hitboxes.push_back((int)CSGOHitboxID::LeftFoot);

		if (custom_hitbones[17].bselected)
			hitboxes.push_back((int)CSGOHitboxID::RightThigh);

		if (custom_hitbones[18].bselected)
			hitboxes.push_back((int)CSGOHitboxID::RightShin);

		if (custom_hitbones[19].bselected)
			hitboxes.push_back((int)CSGOHitboxID::RightFoot);
	}

	for (auto HitBoxID : hitboxes)
	{
		if ((HitBoxID == 0 || HitBoxID == 1 || HitBoxID == 2) && canseebody) {
			if (ragebotconfig.bPreferBaim)
				if (m_entity->GetVelocity().Length2D() < 5 || !(m_entity->GetFlags() & FL_ONGROUND))
					continue;
			if (ragebotconfig.bBaimWithAwp && ragebotconfig.iBaimAwpMode && *m_local->GetWeapon()->GetItemDefinitionIndex() == WEAPON_AWP)
				if (m_entity->GetVelocity().Length2D() < 5 || !(m_entity->GetFlags() & FL_ONGROUND))
					continue;
			if (ragebotconfig.bBaimWithScout && ragebotconfig.iBaimScoutMode && *m_local->GetWeapon()->GetItemDefinitionIndex() == WEAPON_SSG08)
				if (m_entity->GetVelocity().Length2D() < 5 || !(m_entity->GetFlags() & FL_ONGROUND))
					continue;
			if (ragebotconfig.bBaimWithAwp && !ragebotconfig.iBaimAwpMode && *m_local->GetWeapon()->GetItemDefinitionIndex() == WEAPON_AWP)
				continue;
			if (ragebotconfig.bBaimWithScout && !ragebotconfig.iBaimScoutMode && *m_local->GetWeapon()->GetItemDefinitionIndex() == WEAPON_SSG08)
				continue;
			if (ragebotconfig.flBaimOnX)
			{
				if (m_entity->GetHealth() <= ragebotconfig.flBaimOnX)
					continue;
			}
			if (ragebotconfig.bBaimIfDeadly)
			{
				if (m_entity->GetHealth() <= bodydmg)
					continue;
			}
		}

		if (AWall) {
			Vector Point = game::functions.get_hitbox_location(m_entity, HitBoxID);
			float dmg = 0.f;
			if (CanHit(Point, &dmg )) {
				if ( dmg >= ragebotconfig.flMinDmg) {
					return HitBoxID;
				}
			}
		} else {
			if ( game::functions.visible( m_local, m_entity, HitBoxID ) )
				return HitBoxID;
		}
	}

	return -1;
}

bool Aimbot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket, IClientEntity* pEntity)
{
	bool m_return = true;
	if (point.Length() == 0) return false;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	game::math.calculate_angle(src, point, angles);
	game::math.normalize_vector(angles);

	IsLocked = true;

	float m_fov = FovToPlayer( src, game::globals.m_last_angle_both, m_pEntityList->GetClientEntity(TargetID), 0);

	if (ragebotconfig.bAimstep)
	{
		Vector m_delta = angles - game::globals.m_last_angle_both;
		float m_dist = 30.5; // or aimstep value from menu
		game::math.normalize_vector(m_delta);
		if (m_fov > m_dist)
		{
			game::math.normalize(m_delta, m_delta);
			m_delta *= m_dist;
			angles = game::globals.m_last_angle_both + m_delta;
			m_return = false;
		} else m_return = true;
	} else m_return = true;

	game::math.normalize_vector(angles);

	pCmd->viewangles = angles;

	if (!ragebotconfig.iAimbotMode) {
		m_pEngine->SetViewAngles(angles);
	}

	return m_return;
}

inline float FastSqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;

	i += 127 << 23;
	i >>= 1;

	return *(float*)&i;
}

#define square( x ) ( x * x )

void ClampMovement(CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;

	float fSpeed = (float)(FastSqrt(square(pCommand->forwardmove) + square(pCommand->sidemove) + square(pCommand->upmove)));
	if (fSpeed <= 0.f)
		return;

	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f; // TO DO: Maybe look trough the leaked sdk for an exact value since this is straight out of my ass...

	if (fSpeed <= fMaxSpeed)
		return;

	float fRatio = fMaxSpeed / fSpeed;

	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}

void Aimbot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* m_local = game::localdata.localplayer();
	Vector Start = m_local->GetViewOffset() + m_local->GetOrigin();
	bool FindNewTarget = true;

	CSWeaponInfo* weapInfo = ((CBaseCombatWeapon*)m_pEntityList->GetClientEntityFromHandle(m_local->GetActiveWeaponHandle()))->GetCSWpnData();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)m_pEntityList->GetClientEntityFromHandle(m_local->GetActiveWeaponHandle());

	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || pWeapon->IsKnife() || pWeapon->IsC4() || pWeapon->IsGrenade())
			return;
	}
	else
		return;


	if (IsLocked && TargetID > -0 && HitBox >= 0)
	{
		pTarget = m_pEntityList->GetClientEntity(TargetID);
		if (pTarget && IsViable(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector viewoff = m_local->GetOrigin() + m_local->GetViewOffset();
				Vector view; m_pEngine->GetViewAngles(view);
				float fov = FovToPlayer(viewoff, view, pTarget, HitBox);
				if (fov < ragebotconfig.flMaxFov)
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;
		game::globals.Shots = 0;

		switch (ragebotconfig.iTargetSelection)
		{
		case 0: TargetID = GetTargetFOV();
		case 1: TargetID = GetTargetDistance();
		case 2: TargetID = GetTargetHealth();
		}

		if (TargetID >= 0)
			pTarget = m_pEntityList->GetClientEntity(TargetID);
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	game::globals.Target = pTarget;

	if (TargetID >= 0 && pTarget)
	{
		HitBox = HitScan(pTarget);

		if (!game::functions.can_shoot())
			return;

		if (ragebotconfig.bAutoFire && ragebotconfig.iAutoFireMode == 1)
		{
			int Key = ragebotconfig.iAutoFireKey;
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		if (ragebotconfig.iAimbotMode == 2 && game::globals.choked_ticks >= 15)
			return;

		Vector point;
		Vector aimpoint = game::functions.get_hitbox_location(pTarget, HitBox);

		point = aimpoint;

		if (ragebotconfig.bAutoScope && pWeapon->m_bIsSniper() && !pWeapon->IsScoped()) pCmd->buttons |= IN_ATTACK2;
		else
		{
			if ( ragebotconfig.bHitchance && hitchance( ) >= ragebotconfig.flHitchanceAmt * 1.5 || !ragebotconfig.bHitchance )
			{
				if ( AimAtPoint( m_local, point, pCmd, bSendPacket, pTarget ) )
				{
					if ( !ragebotconfig.bAutoFire && !( pCmd->buttons & IN_ATTACK ) )
						return;

					game::globals.aimbotting = true;
					if ( ragebotconfig.bAutoFire && ragebotconfig.iAutoFireMode == 0 || ragebotconfig.bAutoFire && ragebotconfig.iAutoFireMode == 1 && ragebotconfig.iAutoFireKey >= 0 && GUI.GetKeyState( ragebotconfig.iAutoFireKey ) )
					{
						pCmd->buttons |= IN_ATTACK;
						aimbotted_in_current_tick = true;
					}
					else
						return;
				}
			}
		}

		if (ragebotconfig.bAccuracyWhileStanding && !(pCmd->buttons & IN_DUCK))
			ClampMovement(pCmd, 81.f);
	}
}