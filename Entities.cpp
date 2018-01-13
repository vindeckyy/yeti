#include "Entities.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "XorStr.hpp"

bool CBaseCombatWeapon::m_bIsReloading()
{
	static auto inReload = *(uint32_t*)(game::search.signature("client.dll", "C6 87 ? ? ? ? ? 8B 06 8B CE FF 90" + 2));
	return *(bool*)((uintptr_t)this + inReload);
}

CBaseCombatWeapon* IClientEntity::GetWeapon()
{
	static int iOffset = GET_NETVAR("DT_BaseCombatCharacter", "m_hActiveWeapon");
	ULONG pWeepEhandle = *(PULONG)((DWORD)this + iOffset);
	return (CBaseCombatWeapon*)(m_pEntityList->GetClientEntityFromHandle((HANDLE)pWeepEhandle));
}

bool IClientEntity::m_bGunGameImmunity()
{
	static int m_bGunGameImmunity = GET_NETVAR("DT_CSPlayer", "m_bGunGameImmunity");
	return *(bool*)((DWORD)this + m_bGunGameImmunity);
}

float IClientEntity::m_flSimulationTime()
{
	static int m_flSimulationTime = 0x264;
	return *(float*)((DWORD)this + m_flSimulationTime);
}

float IClientEntity::m_flOldSimulationTime()
{
	static int m_flSimulationTime = 0x264;
	return *(float*)((DWORD)this + (m_flSimulationTime + 0x4));
}

int IClientEntity::m_iFOV()
{
	static int m_iFOV = 0x31D4;
	return *(float*)((DWORD)this + m_iFOV);
}

int IClientEntity::m_iDefaultFOV()
{
	static int m_iDefaultFOV = 0x330C;
	return *(float*)((DWORD)this + m_iDefaultFOV);
}

Vector* IClientEntity::GetEyeAngles()
{
	return (Vector*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_angEyeAngles"));
}

float IClientEntity::GetLowerBodyYaw()
{
	return *(float*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget"));
}

Vector* IClientEntity::GetHeadRotation()
{
	return (Vector*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_angRotation"));
}

int IClientEntity::GetObserverMode()
{
	return *(int*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_iObserverMode"));
}

char* IClientEntity::GetLastPlaceName()
{
	return (char*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_szLastPlaceName"));
}

int IClientEntity::GetMoveType()
{
	return *(int*)((DWORD)this + 0x258);
}

float IClientEntity::GetMaxSpeed()
{
	return *(float*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_flMaxspeed"));
}

bool IClientEntity::is_grounded()
{
	return this->GetFlags() & FL_ONGROUND;
}

bool IClientEntity::IsInAir()
{
	return !(this->GetFlags() & FL_ONGROUND);
}

bool IClientEntity::SetupBones2(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	*(int*)((uintptr_t) this + GET_NETVAR(XorStr("DT_CSPlayer"), "m_nForceBone") + 0x20) = 0;
	DWORD m_dOcclusionArray = *(uintptr_t*)(game::search.signature("client.dll", "A1 ? ? ? ? 83 EC 30 56 57 8B F9") + 0x1);
	*(int*)((uintptr_t) this + *(uintptr_t*)(game::search.signature("client.dll", "A1 ? ? ? ? 83 EC 30 56 57 8B F9") + 0xE)) = reinterpret_cast<int*>(m_dOcclusionArray)[1];
	reinterpret_cast<void(__fastcall*)(void*)>(game::search.signature("client.dll", "80 3D ? ? ? ? 00 74 16 A1 ? ? ? ? ? C7 81"))(this);
	__asm
	{
		mov edi, this
		lea ecx, dword ptr ds : [edi + 0x4]
		mov edx, dword ptr ds : [ecx]
		push currentTime
		push boneMask
		push nMaxBones
		push pBoneToWorldOut
		call dword ptr ds : [edx + 0x34]
	}
}

Vector IClientEntity::GetVecOrigin()
{
	return *(Vector*)((DWORD)this + 0x134);
}

int IClientEntity::GetChokedPackets()
{
	if (m_flSimulationTime() > m_flOldSimulationTime())
		return TIME_TO_TICKS(fabs(m_flSimulationTime() - m_flOldSimulationTime()));
	return 0;
}

Vector* IClientEntity::ViewPunch()
{
	static int m_viewPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_viewPunchAngle"));
	return GetFieldPointer<Vector>(m_viewPunchAngle);
}

Vector* IClientEntity::AimPunch()
{
	static int m_aimPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_aimPunchAngle"));
	return GetFieldPointer<Vector>(m_aimPunchAngle);
}
Vector IClientEntity::GetNetworkedOrigin()
{
	static int offset = 0x140 - sizeof(Vector);
	return GetFieldValue< Vector >(offset);
}
float IClientEntity::GetCycle()
{
	return *(float*)((DWORD)this + GET_NETVAR(XorStr("CBaseAnimating"), XorStr("m_flCycle")));
}
int IClientEntity::GetSequence()
{
	return *(int*)((DWORD)this + GET_NETVAR(XorStr("CBaseAnimating"), XorStr("m_nSequence")));
}
float IClientEntity::GetPoseParams(int idx)
{
	return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + GET_NETVAR(XorStr("CBaseAnimating"), XorStr("m_flPoseParameter")) + sizeof(float) * idx);
}
void IClientEntity::SetAbsOriginal(Vector origin)
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
	static SetAbsOriginFn SetAbsOrigin;

	if (!SetAbsOrigin)
		SetAbsOrigin = (SetAbsOriginFn)(game::search.pattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));

	SetAbsOrigin(this, origin);
}
void IClientEntity::SetAbsAngles(Vector angle)
{
	using SetAbsAnglesFn = void(__thiscall*)(void*, const Vector &angle);
	static SetAbsAnglesFn SetAbsAngles;

	if (!SetAbsAngles)
		SetAbsAngles = (SetAbsAnglesFn)(game::search.pattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

	SetAbsAngles(this, angle);
}

void IClientEntity::SetAngle2( Vector wantedang ) {
	typedef void( __thiscall* oSetAngle )( void*, const Vector & );
	static oSetAngle _SetAngle = ( oSetAngle )( ( uintptr_t )game::search.signature( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) );
	_SetAngle( this, wantedang );
}