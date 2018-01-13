#include "Hooks.h"

#include "ESP.h"
#include "LagCompensation.h"

CLagcompensation lagcompensation;



void  __stdcall Hooks::Hooked_FrameStageNotify(ClientFrameStage_t curStage) {

	IClientEntity* m_local = game::localdata.localplayer();

	Vector vecAimPunch;
	Vector vecViewPunch;

	Vector* pAimPunch = nullptr;
	Vector* pViewPunch = nullptr;


	if (m_pEngine->IsConnected() && m_pEngine->IsInGame()) {

		if (curStage == FRAME_RENDER_START) {
			static auto linegoesthrusmoke = game::search.pattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
			static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);

			static std::vector< const char* > smoke_materials = {
				"particle/vistasmokev1/vistasmokev1_smokegrenade",
				"particle/vistasmokev1/vistasmokev1_emods",
				"particle/vistasmokev1/vistasmokev1_emods_impactdust",
				"particle/vistasmokev1/vistasmokev1_fire",
			};

			for (auto matName : smoke_materials) {
				IMaterial* mat = m_pMaterialSystem->FindMaterial(matName, "Other textures");
				mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, visualconfig.bRemoveSmoke);
			}


			if (visualconfig.bRemoveSmoke) {
				*(int*)(smokecout) = 0;
			}

			if (m_local && m_local->IsAlive()) {
				for (int i = 1; i < m_pGlobals->maxClients; i++) {
					auto ent = m_pEntityList->GetClientEntity(i);
					if (!ent || ent == m_local || ent->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !ent->IsAlive()) continue;


					*(int*)((uintptr_t)ent + 0xA30) = m_pGlobals->framecount;
					*(int*)((uintptr_t)ent + 0xA28) = 0;
				}

				auto thirdperson = *(bool*)(reinterpret_cast<uintptr_t>(m_pInput) + 0xA5);
				if (thirdperson)
					*(Vector*)((DWORD)m_local + 0x31C8) = game::globals.visualized_angle;

				if (visualconfig.bNoVisualRecoil) {
					pAimPunch = m_local->AimPunch();
					pViewPunch = m_local->ViewPunch();

					vecAimPunch = *pAimPunch;
					vecViewPunch = *pViewPunch;

					*pAimPunch = Vector(0, 0, 0);
					*pViewPunch = Vector(0, 0, 0);
				}
			}
		}

		wallmodulator.set_skybox();

		if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			if (m_local && m_local->IsAlive()) {

				if (skinconfig.bEnabled) {
					UINT *hWeapons = (UINT*)((DWORD)m_local + 0x2DE8);
					if (hWeapons) {
						player_info_t pLocalInfo;
						m_pEngine->GetPlayerInfo(m_pEngine->GetLocalPlayer(), &pLocalInfo);

						for (int i = 0; hWeapons[i]; i++) {
							CBaseAttributableItem* pWeapon = (CBaseAttributableItem*)m_pEntityList->GetClientEntityFromHandle((HANDLE)hWeapons[i]);

							if (!pWeapon)
								continue;

							int nWeaponIndex = *pWeapon->GetItemDefinitionIndex();

							if (g_ViewModelCFG.find(pWeapon->GetModelIndex()) != g_ViewModelCFG.end())
								pWeapon->SetModelIndex(m_pModelInfo->GetModelIndex(g_ViewModelCFG[pWeapon->GetModelIndex()]));

							skinchanger.apply_viewmodel(m_local, pWeapon, nWeaponIndex);

							if (pLocalInfo.m_nXuidLow != *pWeapon->GetOriginalOwnerXuidLow())
								continue;

							if (pLocalInfo.m_nXuidHigh != *pWeapon->GetOriginalOwnerXuidHigh())
								continue;

							skinchanger.apply_skins(pWeapon, nWeaponIndex);

							*pWeapon->GetAccountID() = pLocalInfo.m_nXuidLow;
						}
					}
				}

				if (resolverconfig.bDisableInterp) {
					for (int i = 0; i < m_pGlobals->maxClients; i++) {
						auto m_entity = m_pEntityList->GetClientEntity(i);
						if (!m_entity || m_entity == m_local || m_entity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !m_entity->IsAlive()) continue;

						lagcompensation.disable_interpolation(m_entity);
					}
				}

				resolver.add_corrections();
			}
		}
	}

	g_fnOriginalFrameStageNotify(curStage);

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		backtracking->update(m_pGlobals->tickcount);
	}

	if (visualconfig.bNoVisualRecoil) {
		if (curStage == ClientFrameStage_t::FRAME_RENDER_START) {
			if (pAimPunch && pViewPunch) {
				*pAimPunch = vecAimPunch;
				*pViewPunch = vecViewPunch;
			}
		}
	}
}