#include "MiscMovement.h"

Misc_Movement movement_hacks;

void Misc_Movement::run(CUserCmd* m_pcmd, Vector& strafer_angle)
{
	auto m_local = game::localdata.localplayer();
	if (!(m_local->GetFlags() & (int)FL_ONGROUND) && m_pcmd->buttons & IN_JUMP) {
		if (!(m_local->GetFlags() & (int)FL_INWATER)) {
			int movetype = m_local->GetMoveType();
			if (!(movetype == 8 || movetype == 9)) {
				if (miscconfig.bAutoJump)
					m_pcmd->buttons &= ~IN_JUMP;

				switch (miscconfig.iAutoStrafeType) {
				case 0:
					m_pcmd->forwardmove = 0.f;
					m_pcmd->sidemove = m_pcmd->mousedx < 0.0f ? -450.f : 450.f;
				case 1:
					if (m_pcmd->mousedx > 1 || m_pcmd->mousedx < -1) {
						m_pcmd->sidemove = m_pcmd->mousedx < 0.0f ? -450.f : 450.f;
					}
					else {
						m_pcmd->forwardmove = 10000.f / m_local->GetVelocity().Length2D();
						m_pcmd->sidemove = (m_pcmd->command_number % 2) == 0 ? -450.f : 450.f;
						if (m_pcmd->forwardmove > 450.f)
							m_pcmd->forwardmove = 450.f;
					}
				}
			}
		}
	}

	blockbot( m_pcmd );
}

void Misc_Movement::cstrafe_run(CUserCmd* m_pcmd)
{
	auto m_local = game::localdata.localplayer();
	Vector velocity = m_local->GetVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed < 45) speed = 45;
	if (speed > 800) speed = 800;

	float final_path = game::functions.get_fraction_props_only(m_local->GetOrigin() + Vector(0, 0, 10), m_local->GetOrigin() + Vector(0, 0, 10) + velocity / 3.0f);
	float delta_angle = cdata.right_movement * fmax((275.0f / speed) * (2.0f / final_path) * (128.0f / (1.7f / m_pGlobals->interval_per_tick)) * 4.20f, 2.0f);
	cdata.angle += delta_angle;

	if (fabs(cdata.angle) >= 360.0f)
	{
		cdata.angle = 0.0f;
		cdata.active = false;
		cdata.right_movement = 0;
	}
	else
	{
		m_pcmd->forwardmove = cos((cdata.angle + 90 * cdata.right_movement) * (M_PI / 180.0f)) * 450.f;
		m_pcmd->sidemove = sin((cdata.angle + 90 * cdata.right_movement) * (M_PI / 180.0f)) * 450.f;
	}
}

void Misc_Movement::cstrafe_start(CUserCmd* m_pcmd)
{
	auto m_local = game::localdata.localplayer();
	cdata.angle = 0;
	cdata.active = true;

	Vector current_view;
	m_pEngine->GetViewAngles(current_view);
	current_view.x = 0;
	Vector forward = QAngle(current_view.x, current_view.y, current_view.z).Direction();
	Vector right = forward.Cross(Vector(0, 0, 1));
	Vector left = Vector(-right.x, -right.y, right.z);

	float left_path = game::functions.get_fraction_props_only(m_local->GetOrigin() + Vector(0, 0, 10), m_local->GetOrigin() + left * 250.0f + Vector(0, 0, 10));
	float right_path = game::functions.get_fraction_props_only(m_local->GetOrigin() + Vector(0, 0, 10), m_local->GetOrigin() + right * 250.0f + Vector(0, 0, 10));

	if (left_path > right_path)
		cdata.right_movement = -1;
	else
		cdata.right_movement = 1;
}

void Misc_Movement::blockbot( CUserCmd* m_pcmd ) {
	auto m_local = game::localdata.localplayer( );

	float bestdist = 250.f;
	int index = -1;

	if ( GetAsyncKeyState( miscconfig.iBlockbotKey ) ) {
		for ( int i = 0; i < m_pGlobals->maxClients; i++ ) {
			auto entity = m_pEntityList->GetClientEntity( i );

			if ( !entity ) continue;
			if ( !entity->IsAlive( ) || entity->IsDormant( ) || entity == m_local ) continue;

			float dist = ( m_local->GetOrigin( ) - entity->GetOrigin( ) ).Length( );

			if ( dist < bestdist ) {
				bestdist = dist;
				index = i;
			}
		}
	}

	if ( index == -1 ) return;

	auto target = m_pEntityList->GetClientEntity( index );

	if ( !target ) return;

	Vector angles;  game::math.calculate_angle( m_local->GetEyePosition( ), target->GetEyePosition( ), angles );
	angles.Normalized( );

	if ( angles.y < 0.0f ) m_pcmd->sidemove = 450.f;
	else if ( angles.y > 0.0f ) m_pcmd->sidemove = -450.f;
}