#pragma once

#include "Interfaces.h"

class CResolverData
{
public:
	int index = 0;
	Vector realAngles, oldAngles;
public:
	CResolverData(int _index, Vector _realAngles, Vector _oldAngles)
	{
		this->index = _index;
		this->realAngles = _realAngles;
		this->oldAngles = _oldAngles;
	}
};

class CResolver
{
public:
	std::vector<CResolverData> corrections;
	void draw_developer_data();
	void add_corrections();
	void apply_corrections(CUserCmd* m_pcmd);
};
extern CResolver resolver;