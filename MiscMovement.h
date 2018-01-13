#pragma once

#include "Hooks.h"

class Misc_Movement
{
public:
	void run(CUserCmd* m_pcmd, Vector& strafer_angle);
	void cstrafe_start(CUserCmd* m_pcmd);
	void cstrafe_run(CUserCmd* m_pcmd);

	void blockbot( CUserCmd* m_pcmd );

	class CStrafeData {
	public:
		float right_movement;
		bool active;
		float angle;
	};
	CStrafeData cdata;
};
extern Misc_Movement movement_hacks;