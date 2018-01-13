#pragma once

#include "SDK.h"

class IGameMovement
{
public:
	void ProcessMovement(IClientEntity *pPlayer, void *pMove)
	{
		typedef void(__thiscall* ProcessMovementFn)(void*, IClientEntity*, void*);
		return call_vfunc<ProcessMovementFn>(this, 1)(this, pPlayer, pMove);
	}
};