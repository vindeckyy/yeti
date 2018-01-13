#include "WallModulations.h"

WallModulator wallmodulator;

void WallModulator::set_modulations() {
	static bool nightmode_performed = false, nightmode_lastsetting;

	if (!m_pEngine->IsConnected() || !m_pEngine->IsInGame()) {
		if (nightmode_performed) {
			materials_props.clear();
			materials_world.clear();
			nightmode_performed = false;
		}
		return;
	}

	auto m_local = game::localdata.localplayer();

	if (!m_local) {
		if (nightmode_performed)
			nightmode_performed = false;
		return;
	}

	if (nightmode_lastsetting != visualconfig.bNightMode) {
		nightmode_lastsetting = visualconfig.bNightMode;
		nightmode_performed = false;
	}

	if (!nightmode_performed) {
		static ConVar* r_DrawSpecificStaticProp = m_pCVar->FindVar("r_DrawSpecificStaticProp");
		r_DrawSpecificStaticProp->nFlags &= ~FCVAR_CHEAT;

		static ConVar* fog_override = m_pCVar->FindVar( "fog_override" );
		static SpoofedConvar* fog_override_spoofed = new SpoofedConvar( fog_override );

		static ConVar* fog_enable = m_pCVar->FindVar( "fog_enable" );
		static SpoofedConvar* fog_enable_spoofed = new SpoofedConvar( fog_enable );

		for (MaterialHandle_t i = m_pMaterialSystem->FirstMaterial(); i != m_pMaterialSystem->InvalidMaterial(); i = m_pMaterialSystem->NextMaterial(i)) {
			IMaterial *pMaterial = m_pMaterialSystem->GetMaterial(i);

			if (!pMaterial || pMaterial->IsErrorMaterial())
				continue;

			if (strstr(pMaterial->GetTextureGroupName(), "World")) {
				if (visualconfig.bNightMode) {
					pMaterial->ColorModulate(0.18, 0.18, 0.15);
				}
				else {
					pMaterial->ColorModulate(1, 1, 1);
				}
			} else if (strstr(pMaterial->GetTextureGroupName(), "StaticProp")) {
				if (visualconfig.bNightMode) {
					pMaterial->ColorModulate(0.38, 0.38, 0.38);
					r_DrawSpecificStaticProp->SetValue(1);
				} else {
					pMaterial->ColorModulate(1, 1, 1);
					r_DrawSpecificStaticProp->SetValue(0);
				}
			}

			if ( visualconfig.bNightMode ) {
				fog_override_spoofed->SetInt( 1 );
				fog_enable_spoofed->SetInt( 0 );
			} else {
				fog_override_spoofed->SetInt( 0 );
				fog_enable_spoofed->SetInt( 1 );
			}
		}
		
		nightmode_performed = true;
	}
}

void WallModulator::set_skybox()
{
	static bool bPerfomed = false, bLastSetting;

	if (!m_pEngine->IsConnected() || !m_pEngine->IsInGame())
	{
		if (bPerfomed) {
			bPerfomed = false;
		}
		return;
	}

	auto m_local = game::localdata.localplayer();

	if (!m_local)
	{
		if (bPerfomed)
			bPerfomed = false;
		return;
	}

	if (bLastSetting != visualconfig.bNightMode)
	{
		bLastSetting = visualconfig.bNightMode;
		bPerfomed = false;
	}

	if (!bPerfomed)
	{
		static ConVar* sv_skyname = m_pCVar->FindVar("sv_skyname");
		sv_skyname->nFlags &= ~FCVAR_CHEAT;

		if (visualconfig.bNightMode)
			sv_skyname->SetValueChar("sky_csgo_night02");
		else
			sv_skyname->SetValueChar("vertigoblue_hdr");

		bPerfomed = true;
	}
}