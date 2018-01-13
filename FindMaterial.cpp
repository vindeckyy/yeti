#include "Hooks.h"

IMaterial* __fastcall Hooks::Hooked_FindMaterial(void* pThis, void*, const char* pMaterialName, const char* pTextureGroup, bool complain, const char* pComplainPrefix)
{
	IMaterial* pMaterial = g_pMaterialSystemHook->GetOriginal<IMaterial*(__thiscall*)(void*, const char*, const char*, bool, const char*)>(84)(pThis, pMaterialName, pTextureGroup, complain, pComplainPrefix);
	if (pMaterial && pMaterialName && pTextureGroup)
	{
		if (strstr(pTextureGroup, "World"))
			wallmodulator.materials_world.push_back(pMaterial);
		if (strstr(pTextureGroup, "StaticProp"))
			wallmodulator.materials_props.push_back(pMaterial);
	}
	return pMaterial;
}