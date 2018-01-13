#pragma once

#include "SDK.h"
#include "MiscDefinitions.h"

class IMoveHelper
{
public:
	void SetHost(IClientEntity* host) {
		typedef void(__thiscall *OrigFn)(void*, IClientEntity*);
		call_vfunc<OrigFn>(this, 1)(this, host);
	}
};