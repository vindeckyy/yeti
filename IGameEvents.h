#pragma once

#include "SDK.h"

class IGameEvent {
public:
	const char* GetName() {
		return call_vfunc<const char*(__thiscall *)(void*)>(this, 1)(this);
	}

	int GetInt(const char* szKeyName, int nDefault = 0) {
		return call_vfunc<int(__thiscall *)(void*, const char*, int)>(this, 6)(this, szKeyName, nDefault);
	}

	const char* GetString(const char* szKeyName) {
		return call_vfunc<const char*(__thiscall *)(void*, const char*, int)>(this, 9)(this, szKeyName, 0);
	}

	void SetString(const char* szKeyName, const char* szValue) {
		return call_vfunc<void(__thiscall *)(void*, const char*, const char*)>(this, 16)(this, szKeyName, szValue);
	}
};

class IGameEventListener
{
public:
	virtual	~IGameEventListener(void) {};

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent(IGameEvent *event) = 0;
	int m_nDebugID;
	virtual int GetEventDebugID(void) { return m_nDebugID; }
};

class IGameEventManager2 {
public:
	bool AddListener(IGameEventListener *listener, const char *name, bool bServerSide)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, IGameEventListener*, const char*, bool);
		return call_vfunc<OriginalFn>(this, 3)(this, listener, name, bServerSide);
	}

	bool FindListener(IGameEventListener *listener, const char *name)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, IGameEventListener*, const char*);
		return call_vfunc<OriginalFn>(this, 4)(this, listener, name);
	}

	bool FireEventClientSide(IGameEvent *event)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, IGameEvent*);
		return call_vfunc<OriginalFn>(this, 8)(this, event);
	}
};