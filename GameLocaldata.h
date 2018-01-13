#pragma once

#include "Interfaces.h"
#include "GameFunctions.h"

#define CONVERT_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / m_pGlobals->interval_per_tick ) )
#define MAX_CHOKE 15

class CGameLocalData {
public:
	float last_update_time;
public:
	bool next_lby_update() {
		return false;
	}

	IClientEntity* localplayer() {
		return m_pEntityList->GetClientEntity(m_pEngine->GetLocalPlayer());
	}
};