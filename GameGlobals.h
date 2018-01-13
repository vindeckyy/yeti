#pragma once

#include "Hooks.h"

class CGameGlobals { 
public:
	Vector m_last_angle;
	Vector visualized_angle;
	Vector fake_angle;
	Vector m_last_angle_both;
	Vector LastAngle;
	CUserCmd* UserCmd;
	IClientEntity* Target;
	int Shots;
	int choked_ticks;
	float frametime;
	Color forecolor = Color(255, 165, 0, 255);
	float fov;
	bool fakelag;
	bool aimbotting;
	bool SendPacket;
	bool breaker_initialized = false;
	float lby_update_end_time;
	float end_removed_scope_red;
	class aa_line_angles {
	public:
		float real_angle, fake_angle;
	};
	aa_line_angles aa_line_data;
};