#pragma once

#include "Hooks.h"

class FakeWalk
{
public:
	void run(CUserCmd* m_pcmd, bool& _packet);
};
extern FakeWalk fakewalk;