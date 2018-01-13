#include "Hooks.h"

using IsRdyFn = void(__cdecl*)();
IsRdyFn IsRdy;

void __stdcall Hooks::Hooked_PlaySound(const char* name)
{
	g_fnOriginalPlaySound(m_pSurface, name);

	// Auto Accept
	if (!strcmp(name, XorStr("!UI/competitive_accept_beep.wav")))
	{
		if (miscconfig.bAutoAccept)
		{
			IsRdy = (IsRdyFn)(game::search.pattern(XorStr("client.dll"), (BYTE*)XorStr("\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x08\x56\x8B\x35\x00\x00\x00\x00\x57\x83\xBE"), XorStr("xxxxxxxxxxxx????xxx")));
			IsRdy();
		}
	}
}