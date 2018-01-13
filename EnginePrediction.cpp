#include "EnginePrediction.h"
#include "NetVars.h"
#include "XorStr.hpp"
#include "Game.h""

BYTE bMoveData[200];

void start_prediction(CUserCmd* pCmd)
{
	auto m_local = game::localdata.localplayer();
	if (m_pMoveHelper && m_local->IsAlive())
	{
		float curtime = m_pGlobals->curtime;
		float frametime = m_pGlobals->frametime;
		int iFlags = m_local->GetFlags();

		m_pGlobals->curtime = (float)m_local->GetTickBase() * m_pGlobals->interval_per_tick;
		m_pGlobals->frametime = m_pGlobals->interval_per_tick;

		m_pMoveHelper->SetHost(m_local);

		m_pPrediction->SetupMove(m_local, pCmd, nullptr, bMoveData);
		m_pGameMovement->ProcessMovement(m_local, bMoveData);
		m_pPrediction->FinishMove(m_local, pCmd, bMoveData);

		m_pMoveHelper->SetHost(0);

		m_pGlobals->curtime = curtime;
		m_pGlobals->frametime = frametime;
		*m_local->GetPointerFlags() = iFlags;
	}
}