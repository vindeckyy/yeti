#include "Hooks.h"
#include "ESP.h"
//#include "BulletImpacts.h"
#include "DamageIndicators.h"

void __fastcall Hooks::Hooked_PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static unsigned int panel_zoom = 0;
	if (!panel_zoom) {
		if (strstr(m_pPanels->GetName(vguiPanel), XorStr("HudZoom")))
			panel_zoom = vguiPanel;
	}
	else if (vguiPanel == panel_zoom) {
		if (visualconfig.bRemoveWeaponScope)
			return;
	}


	g_fnOriginalPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int OverlayerPan = 0;
	static bool FoundPanel = false;

	PCHAR panel_name = (PCHAR)m_pPanels->GetName(vguiPanel);

	if (!FoundPanel)
	{
		if (strstr(panel_name, XorStr("MatSystemTopPanel")))
		{
			OverlayerPan = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (OverlayerPan == vguiPanel)
	{
		if ( m_pEngine->IsConnected( ) && m_pEngine->IsInGame( ) )
		{
			IClientEntity* m_local = game::localdata.localplayer( );

			if ( m_local )
			{
			//	bullet_impacts.paint( );
				esp.paint( );

				if (visualconfig.bHitmarkers)
					hitmarkers.Draw();
				if (visualconfig.bRemoveWeaponScope && game::localdata.localplayer()->IsScoped())
				{
					int width, height;
					m_pEngine->GetScreenSize(width, height);

					Color line_color = Color(19, 19, 19, 255);

					if (game::globals.end_removed_scope_red > m_pGlobals->curtime)
						line_color = Color(255, 10, 10);

					draw.rect(width / 2, 0, 1, height, line_color);
					draw.rect(0, height / 2, width, 1, line_color);
				}
				crosshair.paint();
				damage_indicators.paint();

				static bool disabled_postprocessing = false;

				if (visualconfig.bDisablePostProcessing && !disabled_postprocessing)
				{
					auto svcheats = m_pCVar->FindVar("sv_cheats");
					auto svcheatsspoof = new SpoofedConvar(svcheats);
					svcheatsspoof->SetInt(1);

					m_pEngine->ClientCmd_Unrestricted("mat_postprocess_enable 0");
					disabled_postprocessing = true;
				}

				if (!visualconfig.bDisablePostProcessing && disabled_postprocessing)
				{
					m_pEngine->ClientCmd_Unrestricted("mat_postprocess_enable 1");
					disabled_postprocessing = false;
				}

				if (resolverconfig.bLowerbodyIndicator)
				{
					int width, height;
					m_pEngine->GetScreenSize(width, height);

					int aa_type;

					if (game::localdata.localplayer()->GetVelocity().Length() < 1)
						aa_type = antiaimconfig.stagnant.iRealYaw;
					else
						aa_type = antiaimconfig.moving.iRealYaw;

					
					if (m_local->IsAlive()) {
						if (m_local->is_grounded() && m_local->GetVelocity().Length2D() > 0.1)
							game::globals.lby_update_end_time = m_pGlobals->curtime;

						if (game::globals.lby_update_end_time < m_pGlobals->curtime) {
							draw.text(10, height - (height / 12), "LBY", draw.fonts.indicator, Color(130, 241, 13));
						} else {
							draw.text(10, height - (height / 12), "LBY", draw.fonts.indicator, Color(255, 102, 102));
						}
					}
				}

				resolver.draw_developer_data();
			}
		}

		plist.update();
		Menu::DoFrame();
		settings.Update();
		wallmodulator.set_modulations();
		skinchanger.update_settings();
		game::globals.forecolor = Color(miscconfig.cMenuForecolor[0], miscconfig.cMenuForecolor[1], miscconfig.cMenuForecolor[2], 255);
	}
}