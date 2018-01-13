#pragma once

#include "Interfaces.h"
#include "Settings.h"
#include "ClientRecvProps.h"

#include <unordered_map>

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

#define	LIFE_ALIVE 0

class CBaseViewModel : public IClientUnknown, public IClientRenderable, public IClientNetworkable {
public:
	inline int GetModelIndex() {
		return *(int*)((DWORD)this + 0x254);
	}
	inline DWORD GetOwner() {
		return *(PDWORD)((DWORD)this + 0x29BC);
	}
	inline DWORD GetWeapon() {
		return *(PDWORD)((DWORD)this + 0x29B8);
	}
	inline void SetWeaponModel(const char* Filename, IClientUnknown* Weapon) {
		return getvfunc<void(__thiscall*)(void*, const char*, IClientUnknown*)>(this, 242)(this, Filename, Weapon);
	}
};

struct SkinStruct {
	int nItemDefinitionIndex = 0;
	int nFallbackPaintKit = 0;
	int nFallbackSeed = 0;
	int nFallbackStatTrak = -1;
	int nEntityQuality = 0;
	float flFallbackWear = 0.1f;
	char* szWeaponName = nullptr;
};

extern std::unordered_map<int, SkinStruct>  g_SkinCFG;
extern std::unordered_map<int, const char*> g_ViewModelCFG;
extern std::unordered_map<const char*, const char*> g_KillIconCfg;

class SkinChanger
{
public:
	void update_settings();
	void set_skins();
	void set_viewmodel();
	bool apply_skins(CBaseAttributableItem* pWeapon, int nWeaponIndex);
	bool apply_viewmodel(IClientEntity* pLocal, CBaseAttributableItem* pWeapon, int nWeaponIndex);
	bool apply_killcon(IGameEvent* pEvent);
};

extern SkinChanger skinchanger;