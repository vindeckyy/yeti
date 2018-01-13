#include "Hooks.h"

CPlist plist = CPlist();
double PlistArray[102][200];

RecvVarProxyFn oRecvLowerBodyTarget = NULL;

namespace Hooks
{
	CHookedEvents hooked_events;

	VFTableHook* g_pClientModeHook;
	VFTableHook* g_pMaterialSystemHook;
	VFTableHook* g_pModelRenderHook;
	VFTableHook* g_pPredictionHook;
	VFTableHook* g_pMatSurfaceHook;
	VFTableHook* g_pPanelHook;
	VFTableHook* g_pClientHook;
	VFTableHook* g_pEngineHook;
	VFTableHook* g_pGameEventHook;
	VFTableHook* g_pSurfaceHook;
	VFTableHook* g_pRenderViewHook;

	CreateMove_t g_fnOriginalCreateMove;
	PaintTraverse_t g_fnOriginalPaintTraverse;
	DrawModelExecute_t g_fnOriginalDrawModelExecute;
	FrameStageNotify_t g_fnOriginalFrameStageNotify;
	FireEventClientSide_t g_fnOriginalFireEventClientSide;
	OverrideView_t g_fnOriginalOverrideView;
	IsPlayingTimeDemo_t g_fnOriginalPlayingTimeDemo;
	PlaySound_t g_fnOriginalPlaySound;
	RecvVarProxyFn g_fnSequenceProxyFn = NULL;
	DoPostScreenEffects_t g_fnOriginalDoPostScreenEffects;
	SceneEnd_t g_fnOriginalSceneEnd;

	void Initialise()
	{
		g_pClientModeHook = new VFTableHook(m_pClientMode);
		g_pModelRenderHook = new VFTableHook(m_pModelRender);
		g_pMaterialSystemHook = new VFTableHook(m_pMaterialSystem);
		g_pPredictionHook = new VFTableHook(m_pPrediction);
		g_pPanelHook = new VFTableHook(m_pPanels);
		g_pClientHook = new VFTableHook(m_pClient);
		g_pEngineHook = new VFTableHook(m_pEngine);
		g_pGameEventHook = new VFTableHook(m_pGameEventManager);
		g_pSurfaceHook = new VFTableHook(m_pSurface);
		g_pRenderViewHook = new VFTableHook(m_pRenderView);

		enum FontFlags
		{
			FONTFLAG_NONE,
			FONTFLAG_ITALIC = 0x001,
			FONTFLAG_UNDERLINE = 0x002,
			FONTFLAG_STRIKEOUT = 0x004,
			FONTFLAG_SYMBOL = 0x008,
			FONTFLAG_ANTIALIAS = 0x010,
			FONTFLAG_GAUSSIANBLUR = 0x020,
			FONTFLAG_ROTARY = 0x040,
			FONTFLAG_DROPSHADOW = 0x080,
			FONTFLAG_ADDITIVE = 0x100,
			FONTFLAG_OUTLINE = 0x200,
			FONTFLAG_CUSTOM = 0x400,
			FONTFLAG_BITMAP = 0x800,
		};

		draw.fonts.menu = m_pSurface->FontCreate();
		m_pSurface->SetFontGlyphSet(draw.fonts.menu, XorStr("Verdana"), 12, 400, 0, 0, (int)FONTFLAG_ANTIALIAS);

		draw.fonts.menu_bold = m_pSurface->FontCreate();
		m_pSurface->SetFontGlyphSet(draw.fonts.menu_bold, XorStr("Verdana"), 12, 700, 0, 0, (int)FONTFLAG_ANTIALIAS | (int)FONTFLAG_OUTLINE | (int)FONTFLAG_DROPSHADOW);

		draw.fonts.font_icons = m_pSurface->FontCreate();
		m_pSurface->SetFontGlyphSet(draw.fonts.font_icons, XorStr("Untitled1"), 50, 400, 0, 0, (int)FONTFLAG_ANTIALIAS | (int)FONTFLAG_DROPSHADOW);

		draw.fonts.esp = m_pSurface->FontCreate( );
		m_pSurface->SetFontGlyphSet( draw.fonts.esp, XorStr( "Arial" ), 11, 700, 0, 0, FONTFLAG_OUTLINE );

		draw.fonts.esp_extra = m_pSurface->FontCreate();
		m_pSurface->SetFontGlyphSet(draw.fonts.esp_extra, XorStr("Arial"), 10, 700, 0, 0, (int)FONTFLAG_OUTLINE );

		draw.fonts.esp_small = m_pSurface->FontCreate();
		m_pSurface->SetFontGlyphSet(draw.fonts.esp_small, XorStr("Verdana"), 11, 230, 0, 0, FONTFLAG_OUTLINE);

		draw.fonts.indicator = m_pSurface->FontCreate();
		m_pSurface->SetFontGlyphSet(draw.fonts.indicator, XorStr("Verdana"), 24, 700, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

		chams = new Chams;

		g_fnOriginalPaintTraverse = g_pPanelHook->Hook(41, (PaintTraverse_t)Hooked_PaintTraverse);
		g_fnOriginalCreateMove = g_pClientModeHook->Hook(24, (CreateMove_t)Hooked_CreateMove);
		g_fnOriginalOverrideView = g_pClientModeHook->Hook(18, (OverrideView_t)Hooked_OverrideView);
		g_fnOriginalDoPostScreenEffects = g_pClientModeHook->Hook(44, (DoPostScreenEffects_t)Hooked_DoPostScreenEffects);
		g_fnOriginalFireEventClientSide = g_pGameEventHook->Hook(9, (FireEventClientSide_t)Hooked_FireEventClientSide);
		g_fnOriginalFrameStageNotify = g_pClientHook->Hook(36, (FrameStageNotify_t)Hooked_FrameStageNotify);
		g_fnOriginalPlayingTimeDemo = g_pEngineHook->Hook(85, (IsPlayingTimeDemo_t)Hooked_IsPlayingTimeDemo);
		g_fnOriginalPlaySound = g_pSurfaceHook->Hook(82, (PlaySound_t)Hooked_PlaySound);
		g_fnOriginalSceneEnd = g_pRenderViewHook->Hook(9, (SceneEnd_t)Hooked_SceneEnd);
		g_pModelRenderHook->Hook(21, (DrawModelExecute_t)Hooked_DrawModelExecute);
		g_pClientModeHook->Hook(35, (GetViewmodelFov_t)Hooked_GetViewmodelFov);
		g_pMaterialSystemHook->Hook(84, Hooked_FindMaterial);
		g_pPredictionHook->Hook(19, (RunCommand_t)Hooked_RunCommand);
		initialize_events();

		for (ClientClass* pClass = m_pClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
			if (!strcmp(pClass->m_pNetworkName, "DT_CSPlayer")) {
				RecvTable* pClassTable = pClass->m_pRecvTable;

				for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
					RecvProp* pProp = &pClassTable->m_pProps[nIndex];

					if (!pProp || strcmp(pProp->m_pVarName, "m_flLowerBodyYawTarget"))
						continue;

					pProp->m_ProxyFn = (RecvVarProxyFn)LowerbodyProxy;

					break;
				}

				break;
			}
		}
	}
	void UndoHooks()
	{
	}
}