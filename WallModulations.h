#pragma once

#include "SDK.h"
#include "Settings.h"

class WallModulator
{
public:
	void set_modulations();
	void set_skybox();

	std::vector<IMaterial*> materials_world;
	std::vector<IMaterial*> materials_props;
};

extern WallModulator wallmodulator;