#include "Chams.h"
#include <fstream>

Chams* chams;

Chams::Chams()
{
	std::ofstream("csgo\\materials\\yeti_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$reflectivity" "[1 1 1]"
}
)#";
	std::ofstream("csgo\\materials\\yeti_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$reflectivity" "[1 1 1]"
}
)#";
	std::ofstream("csgo\\materials\\yeti_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\yeti_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	materialRegular = m_pMaterialSystem->FindMaterial("yeti_regular", TEXTURE_GROUP_MODEL);
	materialRegularIgnoreZ = m_pMaterialSystem->FindMaterial("yeti_ignorez", TEXTURE_GROUP_MODEL);
	materialFlatIgnoreZ = m_pMaterialSystem->FindMaterial("yeti_flat_ignorez", TEXTURE_GROUP_MODEL);
	materialFlat = m_pMaterialSystem->FindMaterial("yeti_flat", TEXTURE_GROUP_MODEL);
}

Chams::~Chams()
{
	std::remove(XorStr("csgo\\materials\\yeti_regular.vmt"));
	std::remove(XorStr("csgo\\materials\\yeti_ignorez.vmt"));
	std::remove(XorStr("csgo\\materials\\yeti_flat.vmt"));
	std::remove(XorStr("csgo\\materials\\yeti_flat_ignorez.vmt"));
}

void Chams::override_material(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba)
{
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else {
		if (ignoreZ)
			material = materialRegularIgnoreZ;
		else
			material = materialRegular;
	}


	if (glass) {
		material = materialFlat;
		material->AlphaModulate(0.45f);
	}
	else {
		material->AlphaModulate(
			rgba.a() / 255.0f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	m_pModelRender->ForcedMaterialOverride(material);
}

void Chams::override_material(bool ignoreZ, bool flat, const float alpha)
{
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = materialFlatIgnoreZ;
		else
			material = materialFlat;
	}
	else {
		if (ignoreZ)
			material = materialRegularIgnoreZ;
		else
			material = materialRegular;
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
	material->AlphaModulate(alpha / 255.0f);

	m_pModelRender->ForcedMaterialOverride(material);
}