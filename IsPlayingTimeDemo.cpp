#include "Hooks.h"

bool __fastcall Hooks::Hooked_IsPlayingTimeDemo(void* thisptr, void* edx) {
	bool bResult = g_fnOriginalPlayingTimeDemo(thisptr);
	
	return g_fnOriginalPlayingTimeDemo(thisptr);
}