#pragma once

#include "Hooks.h"

class Chams {
public:
	void override_material(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba);
	void override_material(bool ignoreZ, bool flat, const float alpha);

	Chams();
	~Chams();

	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
};

extern Chams* chams;