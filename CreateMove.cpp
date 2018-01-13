#include "Hooks.h"
#include "MiscMovement.h"
#include "FakeLag.h"
#include "FakeWalk.h"
#include "Legitbot.h"
#include "LinearExtraps.h"
#include "mac.h"

#define getebp() stack *_bp; __asm mov _bp, ebp;
struct stack { stack *next; };

bool __stdcall Hooks::Hooked_CreateMove(float sample_input_frametime, CUserCmd* pCmd) {

	if (!pCmd->command_number)
		return true;

	getebp();
	byte* send_packet = ((byte*)_bp->next - 0x1C);

	if (m_pEngine->IsConnected() && m_pEngine->IsInGame())
	{
		IClientEntity *m_local = game::localdata.localplayer();
		if (m_pEngine->IsConnected() && m_pEngine->IsInGame() && m_local)
		{
			game::globals.UserCmd = pCmd;

			Vector strafe_angles = pCmd->viewangles;
			Vector unmodified_angle = pCmd->viewangles;

			if (visualconfig.bRemoveParticles)
			{
				ConVar* drawparticles = m_pCVar->FindVar(XorStr("r_drawparticles"));
				*(int*)((DWORD)&drawparticles->fnChangeCallback + 0xC) = 0;
				drawparticles->SetValue(0);
			}
			else
			{
				ConVar* drawparticles = m_pCVar->FindVar(XorStr("r_drawparticles"));
				drawparticles->SetValue(1);
			}

			static bool thirdperson = false;
			static bool setting = visualconfig.bThirdperson;
			static bool last_thirdperson_check;

			static bool check = false;
			POINT cursor = GUI.GetMouse();
			if (GetAsyncKeyState(visualconfig.iThirdpersonToggle))
			{
				if (!check)
					setting = !setting;
				check = true;
			}
			else
				check = false;

			if (visualconfig.bThirdperson != last_thirdperson_check)
			{
				setting = visualconfig.bThirdperson;
				last_thirdperson_check = visualconfig.bThirdperson;
			}

			if (visualconfig.bThirdperson)
			{
				if (setting)
				{
					auto svcheats = m_pCVar->FindVar("sv_cheats");
					auto svcheatsspoof = new SpoofedConvar(svcheats);
					svcheatsspoof->SetInt(1);

					m_pEngine->ClientCmd_Unrestricted("thirdperson");

					thirdperson = true;
				}
			}

			if (!setting && thirdperson)
			{
				m_pEngine->ClientCmd_Unrestricted("firstperson");
				thirdperson = false;
			}

			start_prediction(pCmd);

			movement_hacks.run(pCmd, strafe_angles);

			Vector aim_point;
			backtracking->run_lby_backtrack(pCmd, aim_point);

			linear_extraps.run();

			resolver.apply_corrections(pCmd);

			fakelag.run(pCmd, *(bool*)send_packet);

			fakewalk.run(pCmd, *(bool*)send_packet);

			legitbot.run(pCmd, *(bool*)send_packet);

			ragebot.run(pCmd, *(bool*)send_packet);

			spammers.run(pCmd);

			bool has_target = game::globals.Target ? 1 : 0;
			game::functions.add_shot(pCmd, game::globals.Shots, has_target, game::globals.Target);

			if (miscconfig.bAntiUntrusted)
				game::math.angle_clamp(pCmd, unmodified_angle);
			game::functions.movement_correction(strafe_angles, pCmd);

			game::globals.m_last_angle_both = pCmd->viewangles;

			game::globals.m_last_angle.x = pCmd->viewangles.x;
			if ( !( *send_packet ) ) {
				game::globals.m_last_angle.y = pCmd->viewangles.y;
				game::globals.aa_line_data.real_angle = pCmd->viewangles.y;
			}
			else {
				game::globals.fake_angle = *m_local->GetEyeAngles( );
				game::globals.aa_line_data.fake_angle = pCmd->viewangles.y;
			}

			game::globals.visualized_angle.x = pCmd->viewangles.x;
			if (visualconfig.iVisualizedAngle == 0 && !(*send_packet))
				game::globals.visualized_angle.y = pCmd->viewangles.y;
			else if (visualconfig.iVisualizedAngle == 1 && (*send_packet))
				game::globals.visualized_angle.y = pCmd->viewangles.y;
			else if (visualconfig.iVisualizedAngle == 2)
				game::globals.visualized_angle.y = m_local->GetLowerBodyYaw();

			if (m_ClientState->chokedcommands >= MAX_CHOKE)
				*(bool*)send_packet = true;

			resolver.corrections.clear();
			game::globals.LastAngle = pCmd->viewangles;
			game::globals.aimbotting = false;
			game::globals.SendPacket = *send_packet;
			game::globals.fakelag = false;

		}
	}

	return false;
}