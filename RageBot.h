
#pragma once

#include "Settings.h"
#include "Autowall.h"

struct Pitch_AntiAims
{
	void down(float& angle);
	void fake_down(float& angle);
	void up(float& angle);
	void fake_up(float& angle);
	void random(float& angle);
};

struct Yaw_AntiAims
{
	bool lby_just_updated;
	void sideways(float& angle);
	void backwards(float& angle);
	void crooked(float& angle);
	void jitter(float& angle, CUserCmd* m_pcmd);
	void swap(float& angle);
	void rotate(float& angle);
	void lowerbody(float& angle);
	void corruption( float& angle );
};

class AntiAim
{
public:

	Pitch_AntiAims pitches;
	Yaw_AntiAims yaws;
	int AntiAimType;
	float fakeAngles;

	float curtime_fixed(CUserCmd* ucmd);
	bool next_lby_update_func(CUserCmd* m_pcmd, const float yaw_to_break );

	void Manage(CUserCmd* pCmd, bool& bSendPacket);
	void PitchOverrideTick(CUserCmd* pCmd);
	void RealYawOverride(CUserCmd* pCmd);
	void FakeYawOverride(CUserCmd* pCmd);

	bool freestanding(CUserCmd* m_pcmd, bool packet);
};

class Aimbot
{
public:

	// Targetting
	int GetTargetFOV();
	int GetTargetDistance();
	int GetTargetHealth();
	bool IsViable(IClientEntity* pEntity);
	void auto_revolver(CUserCmd* m_pcmd);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity* pEntity);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket, IClientEntity* pEntity);

	// Functionality
	void DoAimbot(CUserCmd *pCmd, bool &bSendPacket);
	void DoNoRecoil(CUserCmd *pCmd);

	// Aimbot
	bool IsLocked;
	int TargetID;
	int HitBox;
	bool aimbotted_in_current_tick;
};

class CRagebot
{
public:
	void Init();
	void Draw();
	void run(CUserCmd *pCmd, bool& bSendPacket);
private:
	AntiAim aa;
	Aimbot aimbot;

	void RemoveRecoil(CUserCmd* pCmd);
};
extern CRagebot ragebot;