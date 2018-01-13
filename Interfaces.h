
#pragma once

#include "CommonIncludes.h"
#include "SDK.h"
#include "CInput.h"
#include "Prediction.h"
#include "XorStr.hpp"
#include "MoveHelper.h"
#include "IClientMode.h"
#include "glow_outline_effect.h"
#include "IVEffects.h"
#include "CClientState.h"

extern IBaseClientDLL* m_pClient;
extern IVEngineClient* m_pEngine;
extern IPanel* m_pPanels;
extern IClientEntityList* m_pEntityList;
extern ISurface* m_pSurface;
extern IClientMode* m_pClientMode;
extern CGlobalVarsBase* m_pGlobals;
extern IPrediction* m_pPrediction;
extern IMaterialSystem* m_pMaterialSystem;
extern IVRenderView* m_pRenderView;
extern IVModelRender* m_pModelRender;
extern IVModelInfo* m_pModelInfo;
extern IEngineTrace* m_pTrace;
extern IPhysicsSurfaceProps* m_pPhysProps;
extern ICVar* m_pCVar;
extern CInput* m_pInput;
extern IGameEventManager2* m_pGameEventManager;
extern IGameMovement* m_pGameMovement;
extern IMoveHelper* m_pMoveHelper;
extern CClientState* m_ClientState;
extern CGlowObjectManager* m_pGlowObjManager;
extern IVEffects* m_pEffects;

class CInterfaces
{
public:
	class InterfacesHead;
	class InterfaceNode;

	class InterfacesHead
	{
	public:
		InterfaceNode* HeadNode; //0x0000 
	};//Size=0x0040

	class InterfaceNode
	{
	public:
		void* fncGet; //0x0000 
		char* pName; //0x0004 
		InterfaceNode* pNext; //0x0008 
	};//Size=0x001C

	class InterfaceManager
	{
	public:
		InterfaceManager(std::string strModule);
		InterfaceManager();

		void Setup(std::string strModule);
		void* GetInterface(std::string strName);

	private:
		std::string strModuleName;
		HMODULE ModuleBase;
		InterfacesHead* pIntHead;
	};
public:
	void initialise();
};
extern CInterfaces interfaces;