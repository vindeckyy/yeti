#pragma once

#include "Hooks.h"

class CLegitbot
{
public:
	void run(CUserCmd* m_pcmd, bool& send_packet);
private:
	int get_target();
	bool viable(IClientEntity* m_player);
	float distance(Vector offset, Vector view, IClientEntity* m_player, int hitbox);
	bool aim(Vector point, CUserCmd *m_pcmd);

	void sync();

	bool locked = false;
	int target_id = -1;
	int hitbox = -1;
	Vector point;
	bool multi_hitbox;
	float smart_aim;
	int besthitbox;

	bool active;
	float smoothing;
	float fov;
	float recoil;
	int delay;
	bool keypress;
	int key;
	bool autofire;
};
extern CLegitbot legitbot;