#include "Hooks.h"

void __stdcall Hooks::Hooked_RunCommand(IClientEntity* pPlayer, CUserCmd* pCmd, IMoveHelper* pMoveHelper)
{
	g_pPredictionHook->GetOriginal<RunCommand_t>(19)(m_pPrediction, pPlayer, pCmd, pMoveHelper);

	m_pMoveHelper = pMoveHelper;
}