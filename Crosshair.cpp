#include "Crosshair.h"

Crosshair crosshair;

void Crosshair::paint() {
	if (visualconfig.bSpreadCrosshair) {
		auto m_local = game::localdata.localplayer();
	
		if (m_local && m_local->IsAlive()) {
			auto m_weapon = m_local->GetWeapon();
			if (!m_weapon || m_weapon->IsKnife()) return;

			int xs, ys;
			m_pEngine->GetScreenSize(xs, ys);
			xs /= 2; ys /= 2;

			auto accuracy = m_weapon->GetCone() * 550.f;
			draw.filled_circle(xs, ys, 30, accuracy, Color(27, 27, 27, 140));
		}
	}

	if (visualconfig.bPenetrationReticle) {
		auto m_local = game::localdata.localplayer();

		if (m_local && m_local->IsAlive()) {
			float damage;

			int xs, ys;
			m_pEngine->GetScreenSize(xs, ys);
			xs /= 2; ys /= 2;

			draw.outlined_rect(xs - 2, ys - 2, 5, 5, Color(27, 27, 27), trace_autowallable(damage) ? Color(130, 241, 13) : Color(255, 102, 102));
		}
	}
}