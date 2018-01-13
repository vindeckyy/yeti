#include "DamageIndicators.h"

DamageIndicators damage_indicators;

void DamageIndicators::paint() {
	auto m_local = game::localdata.localplayer();

	float current_time = m_local->GetTickBase() * m_pGlobals->interval_per_tick;

	for (int i = 0; i < data.size(); i++) {
		if (data[i].flEraseTime < current_time) {
			data.erase(data.begin() + i);
			continue;
		}

		if (!data[i].bInitialized) {
			data[i].Position = data[i].Player->GetHeadPos();
			data[i].bInitialized = true;
		}

		if (current_time - data[i].flLastUpdate > 0.0001f) {
			data[i].Position.z -= (0.1f * (current_time - data[i].flEraseTime));
			data[i].flLastUpdate = current_time;
		}

		Vector screen_pos;

		if (game::functions.world_to_screen(data[i].Position, screen_pos)) {
			draw.text(screen_pos.x, screen_pos.y, std::to_string(data[i].iDamage).c_str(), draw.fonts.esp, Color(255, 0, 0, 255));
		}
	}
}