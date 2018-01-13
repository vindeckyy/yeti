#include "Interfaces.h"
#include "Utilities.h"
#include "Game.h""

CInterfaces interfaces;

IBaseClientDLL* m_pClient = nullptr;
IVEngineClient* m_pEngine = nullptr;
IPanel* m_pPanels = nullptr;
IClientEntityList* m_pEntityList = nullptr;
ISurface* m_pSurface = nullptr;
IClientMode* m_pClientMode = nullptr;
CGlobalVarsBase* m_pGlobals = nullptr;
IPrediction* m_pPrediction = nullptr;
IMaterialSystem* m_pMaterialSystem = nullptr;
IVRenderView* m_pRenderView = nullptr;
IVModelRender* m_pModelRender = nullptr;
IVModelInfo* m_pModelInfo = nullptr;
IEngineTrace* m_pTrace = nullptr;
IPhysicsSurfaceProps* m_pPhysProps = nullptr;
ICVar* m_pCVar = nullptr;
CInput* m_pInput = nullptr;
IGameEventManager2* m_pGameEventManager = nullptr;
IGameMovement* m_pGameMovement = nullptr;
IMoveHelper* m_pMoveHelper = nullptr;
CClientState* m_ClientState = nullptr;
CGlowObjectManager* m_pGlowObjManager = nullptr;
IVEffects* m_pEffects = nullptr;

CInterfaces::InterfaceManager::InterfaceManager()
{
	ModuleBase = 0;
	pIntHead = 0;
}

CInterfaces::InterfaceManager::InterfaceManager(std::string strModule)
{
	ModuleBase = 0;
	pIntHead = 0;
	Setup(strModule);
}

void CInterfaces::InterfaceManager::Setup(std::string strName)
{
	strModuleName = strName;
	ModuleBase = (HMODULE)game::search.wait_on_mod_handle(strName.c_str());

	if (ModuleBase)
	{
		DWORD* pCreateInterfaceFn = (DWORD*)GetProcAddress(ModuleBase, "CreateInterface");

		pCreateInterfaceFn = (DWORD*)((DWORD)pCreateInterfaceFn + 0x4);
		int JmpConv = *(int*)((DWORD)pCreateInterfaceFn + 1);
		pCreateInterfaceFn = (DWORD*)(5 + (DWORD)pCreateInterfaceFn + (DWORD)JmpConv);
		pIntHead = *(InterfacesHead**)((DWORD)pCreateInterfaceFn + 0x6);
	}
}

void* CInterfaces::InterfaceManager::GetInterface(std::string strName)
{
	if (pIntHead)
	{
		DWORD pInt = NULL;
		InterfaceNode* pNode = pIntHead->HeadNode;

		while (pNode)
		{
			std::string nodeName = pNode->pName;
			if (nodeName.find(strName) != std::string::npos)
			{
				DWORD dest = (DWORD)pNode->fncGet;
				__asm
				{
					call dest
					mov pInt, eax
				}
				return (void*)pInt;
			}
			else
			{
				pNode = pNode->pNext;
			}
		}
	}

	return nullptr;
}

/*typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUISurfaceFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;
CreateInterface_t MatFactory = NULL;
CreateInterface_t PhysFactory = NULL;
CreateInterface_t StdFactory = NULL;*/

void CInterfaces::initialise( )
{
	InterfaceManager ClientManager( "client.dll" );
	InterfaceManager EngineManager( "engine.dll" );
	InterfaceManager VGUI2Manager( "vgui2.dll" );
	InterfaceManager MatSurfaceManager( "vguimatsurface.dll" );
	InterfaceManager MatSystemManager( "materialsystem.dll" );
	InterfaceManager PhysManager( "vphysics.dll" );
	InterfaceManager StdLibManager( "vstdlib.dll" );

	m_pClient = ( IBaseClientDLL* )ClientManager.GetInterface( "VClient0" );
	m_pEngine = ( IVEngineClient* )EngineManager.GetInterface( "VEngineClient0" );
	m_pPanels = ( IPanel* )VGUI2Manager.GetInterface( "VGUI_Panel" );
	m_pEntityList = ( IClientEntityList* )ClientManager.GetInterface( "VClientEntityList" );
	m_pSurface = ( ISurface* )MatSurfaceManager.GetInterface( "VGUI_Surface" );
	m_pPrediction = ( IPrediction* )ClientManager.GetInterface( "VClientPrediction" );
	m_pMaterialSystem = ( IMaterialSystem* )MatSystemManager.GetInterface( "VMaterialSystem0" );
	m_pRenderView = ( IVRenderView* )EngineManager.GetInterface( "VEngineRenderView0" );
	m_pModelRender = ( IVModelRender* )EngineManager.GetInterface( "VEngineModel0" );
	m_pModelInfo = ( IVModelInfo* )EngineManager.GetInterface( "VModelInfoClient0" );
	m_pTrace = ( IEngineTrace* )EngineManager.GetInterface( "EngineTraceClient" );
	m_pPhysProps = ( IPhysicsSurfaceProps* )PhysManager.GetInterface( "VPhysicsSurfaceProps" );
	m_pCVar = ( ICVar* )StdLibManager.GetInterface( "VEngineCvar" );
	m_pGameEventManager = ( IGameEventManager2* )EngineManager.GetInterface( "GAMEEVENTSMANAGER002" );
	m_pEffects = ( IVEffects* )EngineManager.GetInterface( "VEngineEffects001" );
	m_pGameMovement = ( IGameMovement* )ClientManager.GetInterface( "GameMovement" );
	m_pClientMode = **( IClientMode*** )( ( *( DWORD** )m_pClient )[ 10 ] + 0x5 );
	m_pInput = *( CInput** )( ( *( DWORD** )m_pClient )[ 15 ] + 0x1 );
	m_pGlobals = **reinterpret_cast< CGlobalVarsBase*** >( ( *reinterpret_cast< DWORD** >( m_pClient ) )[ 0 ] + 0x1B );

	m_ClientState = **( CClientState*** )( game::search.signature( "engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3" ) + 1 );

	m_pGlowObjManager = *( CGlowObjectManager** )( game::search.signature( "client.dll", "0F 11 05 ? ? ? ? 83 C8 01" ) + 3 );

	/*EngineFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Engine, "CreateInterface");
	ClientFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Client, "CreateInterface");
	VGUI2Factory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUI2, "CreateInterface");
	VGUISurfaceFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VGUISurface, "CreateInterface");
	MatFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Material, "CreateInterface");
	PhysFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::VPhysics, "CreateInterface");
	StdFactory = (CreateInterface_t)GetProcAddress((HMODULE)Offsets::Modules::Stdlib, "CreateInterface");

	char* CHLClientInterfaceName = (char*)game::search.text_pattern("client.dll", "VClient0");
	char* VGUI2PanelsInterfaceName = (char*)game::search.text_pattern("vgui2.dll", "VGUI_Panel0");
	char* VGUISurfaceInterfaceName = (char*)game::search.text_pattern("vguimatsurface.dll", "VGUI_Surface0");
	char* EntityListInterfaceName = (char*)game::search.text_pattern("client.dll", "VClientEntityList0");
	char* EngineDebugThingInterface = (char*)game::search.text_pattern("engine.dll", "VDebugOverlay0");
	char* EngineClientInterfaceName = (char*)game::search.text_pattern("engine.dll", "VEngineClient0");
	char* ClientPredictionInterface = (char*)game::search.text_pattern("client.dll", "VClientPrediction0");
	char* MatSystemInterfaceName = (char*)game::search.text_pattern("materialsystem.dll", "VMaterialSystem0");
	char* EngineRenderViewInterface = (char*)game::search.text_pattern("engine.dll", "VEngineRenderView0");
	char* EngineModelRenderInterface = (char*)game::search.text_pattern("engine.dll", "VEngineModel0");
	char* EngineModelInfoInterface = (char*)game::search.text_pattern("engine.dll", "VModelInfoClient0");
	char* EngineTraceInterfaceName = (char*)game::search.text_pattern("engine.dll", "EngineTraceClient0");
	char* PhysPropsInterfaces = (char*)game::search.text_pattern("client.dll", "VPhysicsSurfaceProps0");
	char* VEngineCvarName = (char*)game::search.text_pattern("engine.dll", "VEngineCvar00");
	char* SInputSystem = (char*)game::search.text_pattern("inputsystem.dll", "InputSystemVersion001");
	char* GameEventMangerName = (char*)game::search.text_pattern("engine.dll", "GAMEEVENTSMANAGER002");

	m_pClient = (IBaseClientDLL*)ClientFactory(CHLClientInterfaceName, NULL);
	m_pEngine = (IVEngineClient*)EngineFactory(EngineClientInterfaceName, NULL);
	m_pPanels = (IPanel*)VGUI2Factory(VGUI2PanelsInterfaceName, NULL);
	m_pEntityList = (IClientEntityList*)ClientFactory(EntityListInterfaceName, NULL);
	m_pSurface = (ISurface*)VGUISurfaceFactory(VGUISurfaceInterfaceName, NULL);
	m_pPrediction = (IPrediction*)ClientFactory(ClientPredictionInterface, NULL);
	m_pMaterialSystem = (IMaterialSystem*)MatFactory(MatSystemInterfaceName, NULL);
	m_pModelRender = (IVModelRender*)EngineFactory(EngineModelRenderInterface, NULL);
	m_pTrace = (IEngineTrace*)EngineFactory(EngineTraceInterfaceName, NULL);
	m_pPhysProps = (IPhysicsSurfaceProps*)PhysFactory(PhysPropsInterfaces, NULL);
	m_pCVar = (ICVar*)StdFactory(VEngineCvarName, NULL);
	m_pGameEventManager = (IGameEventManager2*)EngineFactory(GameEventMangerName, NULL);
	m_pRenderView = (IVRenderView*)EngineFactory(EngineRenderViewInterface, NULL);
	m_pModelInfo = (IVModelInfo*)EngineFactory(EngineModelInfoInterface, NULL);
	m_pClientMode = **(IClientMode***)((*(DWORD**)m_pClient)[10] + 0x5);*/
}