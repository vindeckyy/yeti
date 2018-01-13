#include "Ragebot.h"
#include <chrono>

bool is_viable_target(IClientEntity* pEntity)
{
	IClientEntity* m_local = game::localdata.localplayer();
	if (!pEntity) return false;
	if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer) return false;
	if (pEntity == m_local) return false;
	if (pEntity->GetTeamNum() == m_local->GetTeamNum()) return false;
	if (pEntity->m_bGunGameImmunity()) return false;
	if (!pEntity->IsAlive() || pEntity->IsDormant()) return false;
	return true;
}

void normalize_angle(float& flAngle)
{
	if (std::isnan(flAngle)) flAngle = 0.0f;
	if (std::isinf(flAngle)) flAngle = 0.0f;

	float flRevolutions = flAngle / 360;

	if (flAngle > 180 || flAngle < -180)
	{
		if (flRevolutions < 0)
			flRevolutions = -flRevolutions;

		flRevolutions = round(flRevolutions);

		if (flAngle < 0)
			flAngle = (flAngle + 360 * flRevolutions);
		else
			flAngle = (flAngle - 360 * flRevolutions);
	}
}

void Pitch_AntiAims::down(float& angle)
{
	angle = 89.f;
}
void Pitch_AntiAims::fake_down(float& angle)
{
	angle = -179.990005f;
}
void Pitch_AntiAims::up(float& angle)
{
	angle = -89.f;
}
void Pitch_AntiAims::fake_up(float& angle)
{
	angle = -270.f;
}
void Pitch_AntiAims::random(float& angle)
{
	angle = game::math.random_float(-89, 89);
}
void Yaw_AntiAims::sideways(float& angle)
{
	angle += 90;
}
void Yaw_AntiAims::backwards(float& angle)
{
	angle -= 180;
}
void Yaw_AntiAims::crooked(float& angle)
{
	angle += 145;
}
void Yaw_AntiAims::jitter(float& angle, CUserCmd* m_pcmd)
{
	static bool flip = false; flip = !flip;
	float range = antiaimconfig.flJitterRange / 2;
	if (!flip)
		angle += 180 - range;
	else
		angle -= 180 - range;
}
void Yaw_AntiAims::swap(float& angle)
{
	static bool flip = true;

	if (flip) angle += 90.0f;
	else angle -= 90.0f;

	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < .75)
		return;
	flip = !flip;
	start_t = clock();
}
void Yaw_AntiAims::rotate(float& angle)
{
	angle += m_pGlobals->curtime * (antiaimconfig.flRotateSpeed * 1000);
	normalize_angle(angle);
}
void Yaw_AntiAims::corruption(float& angle)
{
	long currentTime_ms = std::chrono::duration_cast< std::chrono::seconds >( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( );
	static long timeStamp = currentTime_ms;

	timeStamp = currentTime_ms;

	switch ( timeStamp % 8 )
	{
	case 1: angle - 170 + rand( ) % ( ( 90 - 1 ) + 1 ) + 1;  break;
	case 2: angle -= 180;  break;
	case 3: angle -= 170 + rand( ) % ( ( 180 - 90 ) + 1 ) + 1;  break;
	case 4: angle -= 180;  break;
	case 5: angle -= 170 + rand( ) % ( ( ( -90 ) - ( -180 ) ) + 1 ) + 1;  break;
	case 6: angle -= 180;  break;
	case 7: angle -= 170 + rand( ) % ( ( ( -1 ) - ( -90 ) ) + 1 ) + 1;  break;
	case 8: angle -= 180;  break;
	}
}
void Yaw_AntiAims::lowerbody(float& angle)
{
	auto m_local = game::localdata.localplayer();
	angle = m_local->GetLowerBodyYaw( );
}
enum ADAPTIVE_SIDE {
	ADAPTIVE_UNKNOWN,
	ADAPTIVE_LEFT,
	ADAPTIVE_RIGHT
};

float AntiAim::curtime_fixed( CUserCmd* ucmd ) {
	auto local_player = game::localdata.localplayer( );
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if ( !g_pLastCmd || g_pLastCmd->hasbeenpredicted ) {
		g_tick = local_player->GetTickBase( );
	}
	else {
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * m_pGlobals->interval_per_tick;
	return curtime;
}

bool AntiAim::next_lby_update_func( CUserCmd* m_pcmd, const float yaw_to_break ) {
	auto m_local = game::localdata.localplayer( );

	if ( m_local ) {
		static float last_attempted_yaw;
		static float old_lby;
		static float next_lby_update_time;
		const float current_time = curtime_fixed( m_pcmd ); // Fixes curtime to the frame so it breaks perfectly every time if delta is in range

		if ( old_lby != m_local->GetLowerBodyYaw( ) && last_attempted_yaw != m_local->GetLowerBodyYaw( ) ) {
			old_lby = m_local->GetLowerBodyYaw( );
			if ( m_local->GetVelocity( ).Length2D( ) < 0.1 ) {
				auto latency = ( m_pEngine->GetNetChannelInfo( )->GetAvgLatency( FLOW_INCOMING ) + m_pEngine->GetNetChannelInfo( )->GetAvgLatency( FLOW_OUTGOING ) );
				next_lby_update_time = current_time + 1.1f;
			}
		}

		if ( m_local->GetVelocity( ).Length2D( ) < 0.1 ) {
			if ( ( next_lby_update_time < current_time ) && m_local->GetFlags( ) & FL_ONGROUND ) {
				last_attempted_yaw = yaw_to_break;
				next_lby_update_time = current_time + 1.1f;
				return true;
			}
		}
	}

	return false;
}

void AntiAim::Manage(CUserCmd* pCmd, bool& bSendPacket)
{
	static int ChokedPackets = -1;
	auto m_local = game::localdata.localplayer();
	auto m_weapon = m_local->GetWeapon();
	if (!m_local)
		return;

	if (m_weapon->IsGrenade())
		return;

	if (m_weapon->IsKnife() && pCmd->buttons & IN_ATTACK || m_weapon->IsKnife() && pCmd->buttons & IN_ATTACK2)
		return;

	if (m_weapon->IsC4() && pCmd->buttons & IN_ATTACK)
		return;

	if (pCmd->buttons & IN_USE)
		return;

	if (m_local->GetMoveType() == 8 || m_local->GetMoveType() == 9)
		return;

	if (ChokedPackets < 1 && m_local->IsAlive() && pCmd->buttons & IN_ATTACK && game::functions.can_shoot() && !m_weapon->IsKnife() && !m_weapon->IsGrenade())
		bSendPacket = false;
	else
	{
		if (m_local->IsAlive())
		{
			int MoveType = m_local->GetMoveType();

			if (antiaimconfig.bDormantCheck)
			{
				bool dormant = true;
				for (int i = 1; i < m_pGlobals->maxClients; i++)
				{
					IClientEntity* ent = m_pEntityList->GetClientEntity(i);
					if (!ent || ent->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || ent->GetTeamNum() == m_local->GetTeamNum() || !ent->IsAlive()) continue;
					if (ent->IsDormant() == false)
						dormant = false;
				}

				if (dormant)
					return;
			}

			PitchOverrideTick(pCmd);

			if (!game::globals.fakelag)
				bSendPacket = pCmd->command_number % 2;
			
			if (!freestanding(pCmd, bSendPacket)) {
				if (bSendPacket) {
					FakeYawOverride(pCmd);
				} else {
					RealYawOverride(pCmd);
				}

				bool clean_up;
				if (game::localdata.localplayer()->GetVelocity().Length() < 6)
					clean_up = antiaimconfig.stagnant.bCleanUp;
				else
					clean_up = antiaimconfig.moving.bCleanUp;

				if (clean_up)
				{
					static float last_fake;
					static float last_real;

					if (bSendPacket)
						last_fake = pCmd->viewangles.y;
					else
						last_real = pCmd->viewangles.y;

					if (game::math.is_close(last_real, last_fake, 35) && !bSendPacket)
						pCmd->viewangles.y -= 90;
				}
			}

			if ( !bSendPacket && antiaimconfig.bLbyBreaker ) {
				if ( next_lby_update_func( pCmd, pCmd->viewangles.y + antiaimconfig.flLbyDelta ) ) {
					pCmd->viewangles.y += antiaimconfig.flLbyDelta;
				}
			}
		}
		ChokedPackets = -1;
	}
}

void AntiAim::PitchOverrideTick(CUserCmd* pCmd)
{
	float pitch;

	int type;

	if (game::localdata.localplayer()->GetVelocity().Length() < 6)
		type = antiaimconfig.stagnant.iRealPitch;
	else
		type = antiaimconfig.moving.iRealPitch;

	if (type == 1) pitches.down(pitch);
	else if (type == 3) pitches.fake_down(pitch);
	else if (type == 4) pitches.up(pitch);
	else if (type == 5) pitches.fake_up(pitch);
	else if (type == 5) pitches.random(pitch);
	else return;

	pCmd->viewangles.x = pitch;
}

void AntiAim::RealYawOverride(CUserCmd* pCmd)
{
	float yaw = 0;

	if (antiaimconfig.iRealYawDirection == 0)
		yaw = pCmd->viewangles.y;
	else if (antiaimconfig.iRealYawDirection == 1)
		yaw = 0;
	else if (antiaimconfig.iRealYawDirection == 2)
	{
		auto m_local = game::localdata.localplayer();
		int CurrentTarget = 0;
		float LastDistance = 999999999999.0f;

		for (int i = 1; i < 65; i++)
		{
			auto pEntity = static_cast<IClientEntity*>(m_pEntityList->GetClientEntity(i));
			if (is_viable_target(pEntity))
			{
				float CurrentDistance = (pEntity->GetOrigin() - m_local->GetOrigin()).Length();
				if (!CurrentTarget || CurrentDistance < LastDistance)
				{
					CurrentTarget = i;
					LastDistance = CurrentDistance;
				}
			}
		}

		if (!CurrentTarget)
			yaw = pCmd->viewangles.y;
		else
		{
			auto pEntity = static_cast<IClientEntity*>(m_pEntityList->GetClientEntity(CurrentTarget));
			Vector LookAtAngle = (pEntity->GetOrigin() - m_local->GetOrigin()).Angle();
			yaw = LookAtAngle.y;
		}
	}

	int type;

	if (game::localdata.localplayer()->GetVelocity().Length() < 6)
		type = antiaimconfig.stagnant.iRealYaw;
	else
		type = antiaimconfig.moving.iRealYaw;

	if (type == 1) yaws.sideways(yaw);
	else if (type == 2) yaws.backwards(yaw);
	else if (type == 3) yaws.crooked(yaw);
	else if (type == 4) yaws.jitter(yaw, pCmd);
	else if (type == 5) yaws.swap(yaw);
	else if (type == 6) yaws.rotate(yaw);
	else if (type == 7) yaws.lowerbody(yaw);
	else if ( type == 8 ) yaws.corruption( yaw );
	else return;

	if (game::localdata.localplayer()->GetVelocity().Length() < 6)
		yaw += antiaimconfig.stagnant.flRealYawOffset;
	else
		yaw += antiaimconfig.moving.flRealYawOffset;

	pCmd->viewangles.y = yaw;
}

void AntiAim::FakeYawOverride( CUserCmd* pCmd )
{
	float yaw = 0;

	if ( antiaimconfig.iRealYawDirection == 0 )
		yaw = pCmd->viewangles.y;
	else if ( antiaimconfig.iRealYawDirection == 1 )
		yaw = 0;
	else if ( antiaimconfig.iRealYawDirection == 2 )
	{
		auto m_local = game::localdata.localplayer( );
		int CurrentTarget = 0;
		float LastDistance = 999999999999.0f;

		for ( int i = 1; i < 65; i++ )
		{
			auto pEntity = static_cast< IClientEntity* >( m_pEntityList->GetClientEntity( i ) );
			if ( is_viable_target( pEntity ) )
			{
				float CurrentDistance = ( pEntity->GetOrigin( ) - m_local->GetOrigin( ) ).Length( );
				if ( !CurrentTarget || CurrentDistance < LastDistance )
				{
					CurrentTarget = i;
					LastDistance = CurrentDistance;
				}
			}
		}

		if ( !CurrentTarget )
			yaw = pCmd->viewangles.y;
		else
		{
			auto pEntity = static_cast< IClientEntity* >( m_pEntityList->GetClientEntity( CurrentTarget ) );
			Vector LookAtAngle = ( pEntity->GetOrigin( ) - m_local->GetOrigin( ) ).Angle( );
			yaw = LookAtAngle.y;
		}
	}

	int type;

	if ( game::localdata.localplayer( )->GetVelocity( ).Length( ) < 6 )
		type = antiaimconfig.stagnant.iFakeYaw;
	else
		type = antiaimconfig.moving.iFakeYaw;

	if ( type == 1 ) yaws.sideways( yaw );
	else if ( type == 2 ) yaws.backwards( yaw );
	else if ( type == 3 ) yaws.crooked( yaw );
	else if ( type == 4 ) yaws.jitter( yaw, pCmd );
	else if ( type == 5 ) yaws.swap( yaw );
	else if ( type == 6 ) yaws.rotate( yaw );
	else if ( type == 7 ) yaws.lowerbody( yaw );
	else if ( type == 8 ) yaws.corruption( yaw );
	else return;

	if ( game::localdata.localplayer( )->GetVelocity( ).Length( ) < 6 )
		yaw += antiaimconfig.stagnant.flFakeYawOffset;
	else
		yaw += antiaimconfig.moving.flFakeYawOffset;

	pCmd->viewangles.y = yaw;
}

bool AntiAim::freestanding( CUserCmd* m_pcmd, bool packet )
{
	IClientEntity* m_local = game::localdata.localplayer( );

	if ( antiaimconfig.edge.iWallDtc == 1 && m_local->GetVelocity( ).Length( ) < 300 ) {

		auto fov_to_player = [ ] ( Vector view_offset, Vector view, IClientEntity* m_entity, int hitbox )
		{
			CONST FLOAT MaxDegrees = 180.0f;
			Vector Angles = view;
			Vector Origin = view_offset;
			Vector Delta( 0, 0, 0 );
			Vector Forward( 0, 0, 0 );
			game::math.angle_vectors( Angles, Forward );
			Vector AimPos = game::functions.get_hitbox_location( m_entity, hitbox );
			game::math.vector_subtract( AimPos, Origin, Delta );
			game::math.normalize( Delta, Delta );
			FLOAT DotProduct = Forward.Dot( Delta );
			return ( acos( DotProduct ) * ( MaxDegrees / PI ) );
		};

		int target = -1;
		float mfov = 50;

		Vector viewoffset = m_local->GetOrigin( ) + m_local->GetViewOffset( );
		Vector view; m_pEngine->GetViewAngles( view );

		for ( int i = 0; i < m_pGlobals->maxClients; i++ ) {
			IClientEntity* m_entity = m_pEntityList->GetClientEntity( i );

			if ( is_viable_target( m_entity ) ) {

				float fov = fov_to_player( viewoffset, view, m_entity, 0 );
				if ( fov < mfov ) {
					mfov = fov;
					target = i;
				}
			}
		}

		Vector at_target_angle;

		if ( target ) {
			auto m_entity = m_pEntityList->GetClientEntity( target );

			if ( is_viable_target( m_entity ) ) {
				Vector head_pos_screen;

				if ( game::functions.world_to_screen( m_entity->GetHeadPos( ), head_pos_screen ) ) {

					float pitch = m_pcmd->viewangles.x;
					int type = antiaimconfig.edge.iRealPitch;
					if ( type == 1 ) pitches.down( pitch );
					else if ( type == 3 ) pitches.fake_down( pitch );
					else if ( type == 4 ) pitches.up( pitch );
					else if ( type == 5 ) pitches.fake_up( pitch );
					else if ( type == 5 ) pitches.random( pitch );
					m_pcmd->viewangles.x = pitch;

					float yaw = m_pcmd->viewangles.y;
					type = packet ? antiaimconfig.edge.iFakeYaw : antiaimconfig.edge.iRealYaw;
					if ( type == 1 ) yaws.sideways( yaw );
					else if ( type == 2 ) yaws.backwards( yaw );
					else if ( type == 3 ) yaws.crooked( yaw );
					else if ( type == 4 ) yaws.jitter( yaw, m_pcmd );
					else if ( type == 5 ) yaws.swap( yaw );
					else if ( type == 6 ) yaws.rotate( yaw );
					if ( game::localdata.localplayer( )->GetVelocity( ).Length( ) < 6 ) yaw += antiaimconfig.stagnant.flRealYawOffset;
					else yaw += antiaimconfig.moving.flRealYawOffset;
					m_pcmd->viewangles.y = yaw;

					game::math.calculate_angle( m_local->GetOrigin( ), m_entity->GetOrigin( ), at_target_angle );
					at_target_angle.x = 0;

					Vector src3D, dst3D, forward, right, up, src, dst;
					float back_two, right_two, left_two;
					trace_t tr;
					Ray_t ray, ray2, ray3, ray4, ray5;
					CTraceFilter filter;

					const Vector to_convert = at_target_angle;
					game::math.angle_vectors( to_convert, &forward, &right, &up );

					filter.pSkip = m_local;
					src3D = m_local->GetEyePosition( );
					dst3D = src3D + ( forward * 384 ); //Might want to experiment with other numbers, incase you don't know what the number does, its how far the trace will go. Lower = shorter.

					ray.Init( src3D, dst3D );
					m_pTrace->TraceRay( ray, MASK_SHOT, &filter, &tr );
					back_two = ( tr.endpos - tr.startpos ).Length( );

					ray2.Init( src3D + right * 35, dst3D + right * 35 );
					m_pTrace->TraceRay( ray2, MASK_SHOT, &filter, &tr );
					right_two = ( tr.endpos - tr.startpos ).Length( );

					ray3.Init( src3D - right * 35, dst3D - right * 35 );
					m_pTrace->TraceRay( ray3, MASK_SHOT, &filter, &tr );
					left_two = ( tr.endpos - tr.startpos ).Length( );

					if ( left_two > right_two ) {
						m_pcmd->viewangles.y -= 90;
						//Body should be right
					}
					else if ( right_two > left_two ) {
						m_pcmd->viewangles.y += 90;
						//Body should be left
					}
					//if (packet) m_pcmd->viewangles.y += 180;
					return true;
				}
			}
		}
	}
	return false;
}