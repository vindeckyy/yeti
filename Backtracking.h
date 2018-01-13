#pragma once

#include "Hooks.h"
#include "Settings.h"
#include <deque>

struct lby_record
{
	int tick_count;
	float lby;
	Vector head_position;
};
struct backtrack_data
{
	float simtime;
	Vector hitbox_pos;
};

class CBacktracking
{
	int latest_tick;
	bool is_valid_tick(int tick);
	void update_record(int i);
public:
	lby_record records[64];
	bool run_lby_backtrack(CUserCmd* m_pcmd, Vector& aim_point);
	void update(int tick_count);
	void legit_backtrack(CUserCmd* m_pcmd);
};

extern backtrack_data head_positions[64][12];

extern CBacktracking* backtracking;