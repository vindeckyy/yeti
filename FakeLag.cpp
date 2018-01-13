#include "FakeLag.h"
#include "Backtracking.h"

CFakeLag fakelag;

void CFakeLag::run(CUserCmd* m_pcmd, bool& send_packet)
{
	auto m_local = game::localdata.localplayer();

	if (!m_local || !m_local->IsAlive())
		return;

	bool disable_while_shooting = miscconfig.bFlagDisableWhileShooting;
	int activation_type = miscconfig.iFlagActivationType;
	int flag_mode = miscconfig.iFlagType;
	int max_choked_packets = miscconfig.flFlagLimit + 1;

	if (!miscconfig.bFlagEnabled || !miscconfig.iFlagType)
		return;

	if (max_choked_packets > 15) max_choked_packets = 15;

	if (miscconfig.bFlagDisableWhileShooting && (m_pcmd->buttons & IN_ATTACK) && game::functions.can_shoot()) return;
	if (miscconfig.iFlagActivationType == 1 && m_local->GetVelocity().Length() < 3) return;
	if (miscconfig.iFlagActivationType == 2 && (m_local->GetFlags() & FL_ONGROUND)) return;

	static int tick; tick++;

	if (flag_mode == 1)
	{
		send_packet = !(tick % max_choked_packets);
	}
	else if (flag_mode == 2)
	{
		static int factor = 7;
		send_packet = !(tick % factor);
		bool m_switch = false;
		if (send_packet)
		{
			int m_choke = tick % max_choked_packets + 1;
			int m_stepdown;

			if (m_choke >= max_choked_packets)
				m_switch = !m_switch;

			m_stepdown = 15 - m_choke;

			if (m_switch && m_choke > 8)
				m_stepdown + 6;

			factor = m_switch ? m_stepdown : m_choke;

			if (factor >= 15)
				factor = 14;
		}
	}
	else if (flag_mode == 3)
	{
		static int factor = 7;
		send_packet = !(tick % factor);
		float flVelocity = m_local->GetVelocity().Length2D() * m_pGlobals->interval_per_tick;
		if (send_packet)
			factor = game::math.clamp(static_cast<int>(std::ceil(69.f / flVelocity)), 1, max_choked_packets);
	}

	game::globals.fakelag = true;
}