#include "Hooks.h"

void Hooks::Proxy_Knife(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	static int default_t = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_default_t.mdl"));
	static int default_ct = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_default_ct.mdl"));
	static int iBayonet = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_bayonet.mdl"));
	static int iButterfly = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_butterfly.mdl"));
	static int iFlip = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_flip.mdl"));
	static int iGunGame = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_gg.mdl"));
	static int iGut = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_gut.mdl"));
	static int iKarambit = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_karam.mdl"));
	static int iM9Bayonet = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_m9_bay.mdl"));
	static int iHuntsman = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_tactical.mdl"));
	static int iFalchion = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_falchion_advanced.mdl"));
	static int iDagger = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_push.mdl"));
	static int iBowie = m_pModelInfo->GetModelIndex(XorStr("models/weapons/v_knife_survival_bowie.mdl"));

	IClientEntity* pLocal = m_pEntityList->GetClientEntity(m_pEngine->GetLocalPlayer());
	if (pLocal)
	{
		if (skinconfig.bOverrideKnife && skinconfig.iKnife)
		{
			if (pLocal->IsAlive() && (pData->m_Value.m_Int == default_t ||
				pData->m_Value.m_Int == default_ct))
			{
				switch (skinconfig.iKnife)
				{
				case 0: break;
				case 1: pData->m_Value.m_Int = iBayonet; break;
				case 2: pData->m_Value.m_Int = iM9Bayonet; break;
				case 3: pData->m_Value.m_Int = iButterfly; break;
				case 4: pData->m_Value.m_Int = iFlip; break;
				case 5: pData->m_Value.m_Int = iGut; break;
				case 6: pData->m_Value.m_Int = iKarambit; break;
				case 7: pData->m_Value.m_Int = iHuntsman; break;
				case 8: pData->m_Value.m_Int = iFalchion; break;
				case 9: pData->m_Value.m_Int = iDagger; break;
				case 10: pData->m_Value.m_Int = iBowie; break;
				}
			}
		}
	}

	*(int*)((DWORD)pOut) = pData->m_Value.m_Int;
}