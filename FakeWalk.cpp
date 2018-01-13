#include "FakeWalk.h"

FakeWalk fakewalk;

void FakeWalk::run(CUserCmd* m_pcmd, bool& _packet)
{
	auto m_local = game::localdata.localplayer();
	if (m_local->GetFlags() & FL_ONGROUND) {
		if (!(m_local->GetFlags() & (int)FL_INWATER)) {
			int movetype = m_local->GetMoveType();
			if (!(movetype == 8 || movetype == 9)) {
				if (miscconfig.bFakeWalk && GetAsyncKeyState(miscconfig.iFakeWalkKey)) {
					auto clientstate = m_ClientState;
					auto netchannelinfo = m_pEngine->GetNetChannelInfo();
					//auto netchannelhandler = g_csgo.m_nch;
					//auto netclient = g_csgo.m_nc;
				}
			}
		}
	}
}