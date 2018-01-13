

#pragma once

#include "MiscDefinitions.h"
#include "MiscDefinitions.h"

class CGlobalVarsBase
{
public:
	float    realtime;
	int      framecount;
	float    absoluteframetime;
	float    absoluteframestarttimestddev;
	float    curtime;
	float    frametime;
	int      maxClients;
	int      tickcount;
	float    interval_per_tick;
	float    interpolation_amount;
	int      simTicksThisFrame;
	int      network_protocol;
	void*    pSaveData;
	bool     m_bClient;
	int      nTimestampNetworkingBase;
	int      nTimestampRandomizeWindow;
};

class IBaseClientDLL
{
public:

	ClientClass* GetAllClasses(void)
	{
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
		return call_vfunc<OriginalFn>(this, 8)(this);
	}
};