#pragma once

#include "GameLocaldata.h"
#include "GameMath.h"
#include "GameSearch.h"
#include "Playerlist.h"
#include "Menu.h"

class CGameFunctions
{
	CGameMath math;
	CGameSearch search;
public:
	void movement_correction(Vector& wish_angle, CUserCmd* m_pcmd)
	{
		/*Vector vMove = Vector(pCmd->forwardmove, pCmd->sidemove, 0.0f);
		float flSpeed = vMove.Length();
		Vector qMove;
		math.vector_angles(vMove, qMove);
		float normalized = fmod(pCmd->viewangles.y + 180.f, 360.f) - 180.f;
		float normalizedx = fmod(pCmd->viewangles.x + 180.f, 360.f) - 180.f;
		float flYaw = DEG2RAD((normalized - oang.y) + qMove.y);
		if (normalizedx >= 90.0f || normalizedx <= -90.0f) pCmd->forwardmove = -cos(flYaw) * flSpeed;
		else pCmd->forwardmove = cos(flYaw) * flSpeed;
		pCmd->sidemove = sin(flYaw) * flSpeed;*/

		Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
		auto viewangles = m_pcmd->viewangles;
		viewangles.Normalized();

		math.angle_vectors(wish_angle, &view_fwd, &view_right, &view_up);
		math.angle_vectors(viewangles, &cmd_fwd, &cmd_right, &cmd_up);

		float v8 = sqrtf((view_fwd.x * view_fwd.x) + (view_fwd.y * view_fwd.y));
		float v10 = sqrtf((view_right.x * view_right.x) + (view_right.y * view_right.y));
		float v12 = sqrtf(view_up.z * view_up.z);

		Vector norm_view_fwd((1.f / v8) * view_fwd.x, (1.f / v8) * view_fwd.y, 0.f);
		Vector norm_view_right((1.f / v10) * view_right.x, (1.f / v10) * view_right.y, 0.f);
		Vector norm_view_up(0.f, 0.f, (1.f / v12) * view_up.z);

		float v14 = sqrtf((cmd_fwd.x * cmd_fwd.x) + (cmd_fwd.y * cmd_fwd.y));
		float v16 = sqrtf((cmd_right.x * cmd_right.x) + (cmd_right.y * cmd_right.y));
		float v18 = sqrtf(cmd_up.z * cmd_up.z);

		Vector norm_cmd_fwd((1.f / v14) * cmd_fwd.x, (1.f / v14) * cmd_fwd.y, 0.f);
		Vector norm_cmd_right((1.f / v16) * cmd_right.x, (1.f / v16) * cmd_right.y, 0.f);
		Vector norm_cmd_up(0.f, 0.f, (1.f / v18) * cmd_up.z);

		float v22 = norm_view_fwd.x * m_pcmd->forwardmove;
		float v26 = norm_view_fwd.y * m_pcmd->forwardmove;
		float v28 = norm_view_fwd.z * m_pcmd->forwardmove;
		float v24 = norm_view_right.x * m_pcmd->sidemove;
		float v23 = norm_view_right.y * m_pcmd->sidemove;
		float v25 = norm_view_right.z * m_pcmd->sidemove;
		float v30 = norm_view_up.x * m_pcmd->upmove;
		float v27 = norm_view_up.z * m_pcmd->upmove;
		float v29 = norm_view_up.y * m_pcmd->upmove;

		m_pcmd->forwardmove = ((((norm_cmd_fwd.x * v24) + (norm_cmd_fwd.y * v23)) + (norm_cmd_fwd.z * v25))
			+ (((norm_cmd_fwd.x * v22) + (norm_cmd_fwd.y * v26)) + (norm_cmd_fwd.z * v28)))
			+ (((norm_cmd_fwd.y * v30) + (norm_cmd_fwd.x * v29)) + (norm_cmd_fwd.z * v27));
		m_pcmd->sidemove = ((((norm_cmd_right.x * v24) + (norm_cmd_right.y * v23)) + (norm_cmd_right.z * v25))
			+ (((norm_cmd_right.x * v22) + (norm_cmd_right.y * v26)) + (norm_cmd_right.z * v28)))
			+ (((norm_cmd_right.x * v29) + (norm_cmd_right.y * v30)) + (norm_cmd_right.z * v27));
		m_pcmd->upmove = ((((norm_cmd_up.x * v23) + (norm_cmd_up.y * v24)) + (norm_cmd_up.z * v25))
			+ (((norm_cmd_up.x * v26) + (norm_cmd_up.y * v22)) + (norm_cmd_up.z * v28)))
			+ (((norm_cmd_up.x * v30) + (norm_cmd_up.y * v29)) + (norm_cmd_up.z * v27));

		m_pcmd->forwardmove = math.clamp(m_pcmd->forwardmove, -450.f, 450.f);
		m_pcmd->sidemove = math.clamp(m_pcmd->sidemove, -450.f, 450.f);
		m_pcmd->upmove = math.clamp(m_pcmd->upmove, -320.f, 320.f);
	}
	bool visible(IClientEntity* local, IClientEntity* entity, int BoneID)
	{
		static trace_t trace;
		static IClientEntity* ent_cpy;

		if (BoneID < 0) return false;

		ent_cpy = entity;
		Vector start = local->GetOrigin() + local->GetViewOffset();
		Vector end = get_hitbox_location(entity, 0);

		trace_line(start, end, MASK_SOLID, local, 0, &trace);

		if (trace.m_pEnt == ent_cpy)
		{
			return true;
		}

		if (trace.fraction == 1.0f)
		{
			return true;
		}

		return false;
	}
	void print_to_chat(const char *text)
	{
		char buffer[250];
		sprintf_s(buffer, "say \"%s\"", text);
		m_pEngine->ClientCmd_Unrestricted(buffer);
	}
	bool can_shoot()
	{
		auto m_local = m_pEntityList->GetClientEntity(m_pEngine->GetLocalPlayer());
		if (!m_local) return false;

		auto m_weapon = m_local->GetWeapon();

		if (!m_local)
			return false;

		if (!m_weapon)
			return false;

		float flServerTime = m_local->GetTickBase() * m_pGlobals->interval_per_tick;

		return (m_weapon->GetNextPrimaryAttack() <= flServerTime);
	}
	Vector get_hitbox_location(IClientEntity* obj, int hitbox_id) {
		matrix3x4 bone_matrix[ 128 ];

		if ( obj->SetupBones( bone_matrix, 128, 0x00000100, 0.0f ) ) {
			if ( obj->GetModel( ) ) {
				auto studio_model = m_pModelInfo->GetStudioModel( obj->GetModel( ) );
				if ( studio_model ) {
					auto hitbox = studio_model->GetHitboxSet( 0 )->GetHitbox( hitbox_id );
					if ( hitbox ) {
						auto min = Vector{ }, max = Vector{ };

						math.vector_transform( hitbox->bbmin, bone_matrix[ hitbox->bone ], min );
						math.vector_transform( hitbox->bbmax, bone_matrix[ hitbox->bone ], max );

						return ( min + max ) / 2.0f;
					}
				}
			}
		}
		return Vector{ };
	}
	void trace_line(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
	{
		typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
		static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)search.signature("client.dll", "55 8B EC 83 E4 F0 83 EC 7C 56 52");
		TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
	}
	bool trace_to_exit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
	{
		typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
		static TraceToExitFn TraceToExit = (TraceToExitFn)search.signature(XorStr("client.dll"), XorStr("55 8B EC 83 EC 30 F3 0F 10 75"));

		if (!TraceToExit)
			return false;

		return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
	}
	bool is_breakable_ent(IClientEntity *m_entity)
	{

		typedef bool(__thiscall *isBreakbaleEntityFn)(IClientEntity*);
		static isBreakbaleEntityFn IsBreakableEntity;

		if (!IsBreakableEntity)
			IsBreakableEntity = (isBreakbaleEntityFn)search.pattern(XorStr("client.dll"), (BYTE*)(XorStr("\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68")), XorStr("xxxxxxxxxxx"));

		return IsBreakableEntity(m_entity);
	}
	void clip_trace_to_player(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
	{
		static DWORD dwAddress = search.signature("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10");

		if (!dwAddress)
			return;

		_asm
		{
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dwAddress
			ADD		ESP, 0xC
		}
	}
	std::string get_time()
	{
		time_t current_time;
		struct tm *time_info;
		static char timeString[10];
		time(&current_time);
		time_info = localtime(&current_time);
		strftime(timeString, sizeof(timeString), XorStr("%I:%M%p"), time_info);
		return timeString;
	}
	bool screen_transform(const Vector& point, Vector& screen)
	{
		const matrix3x4& w2sMatrix = m_pEngine->WorldToScreenMatrix();
		screen.x = w2sMatrix[0][0] * point.x + w2sMatrix[0][1] * point.y + w2sMatrix[0][2] * point.z + w2sMatrix[0][3];
		screen.y = w2sMatrix[1][0] * point.x + w2sMatrix[1][1] * point.y + w2sMatrix[1][2] * point.z + w2sMatrix[1][3];
		screen.z = 0.0f;

		float w = w2sMatrix[3][0] * point.x + w2sMatrix[3][1] * point.y + w2sMatrix[3][2] * point.z + w2sMatrix[3][3];

		if (w < 0.001f) {
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	}
	bool world_to_screen(const Vector &origin, Vector &screen)
	{
		if (!screen_transform(origin, screen)) {
			int iScreenWidth, iScreenHeight;
			m_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

			screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
			screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;

			return true;
		}
		return false;
	}
	void set_clan_tag(const char* tag, const char* clan_name)
	{
		static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(search.signature(XorStr("engine.dll"), XorStr("53 56 57 8B DA 8B F9 FF 15")));
		pSetClanTag(tag, clan_name);
	}
	void set_name(const char* name)
	{
		ConVar* nameConvar = m_pCVar->FindVar(XorStr("name"));
		*(int*)((DWORD)&nameConvar->fnChangeCallback + 0xC) = NULL;
		nameConvar->SetValueChar(name);
	}
	void add_shot(CUserCmd* m_pcmd, int& shots, bool has_target, IClientEntity* m_target)
	{
		auto m_local = m_pEntityList->GetClientEntity(m_pEngine->GetLocalPlayer());
		auto m_weapon = m_local->GetWeapon();
		if (m_weapon)
		{
			if (!m_weapon->IsGrenade() && !m_weapon->IsKnife() && !m_weapon->IsC4() && m_weapon->GetAmmoInClip() > 0 && has_target)
			{
				if (can_shoot() && m_pcmd->buttons & IN_ATTACK)
				{
					shots += 1;

					CPlayer* m_player = plist.get_player(m_target->GetIndex());
					if (m_player->is_valid_player)
					{
						m_player->resolver_data.missed_shots += 1;
						m_player->resolver_data.shots += 1;
					}
				}
			}
		}
	}
	float get_fraction_props_only(Vector start, Vector end)
	{
		Ray_t ray;
		CGameTrace tr;
		CTraceFilter filter;

		ray.Init(start, end);
		m_pTrace->TraceRay(ray, MASK_SOLID, &filter, &tr);

		return tr.fraction;
	}
};