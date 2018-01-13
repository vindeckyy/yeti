#include "RebuildGameMovement.h"

RebuildGameMovement::RebuildGameMovement(void)
{

}

void RebuildGameMovement::SetAbsOrigin(IClientEntity *player, const Vector &vec)
{
	if (player && player->GetMoveType() == MOVETYPE_WALK)
	{
		trace_t pm;
		TracePlayerBBox(vec, vec, MASK_PLAYERSOLID, 8, pm, player);
		if (pm.startsolid || pm.allsolid || pm.fraction != 1.0f)
		{
			//player will get stuck, lets not?
			return;
		}
	}

	player->GetAbsOriginal() = vec;
}

int RebuildGameMovement::ClipVelocity(Vector &in, Vector &normal, Vector &out, float overbounce)
{
	return 0;
}

int RebuildGameMovement::TryPlayerMove(IClientEntity *player, Vector *pFirstDest, trace_t *pFirstTrace)
{
	return 0;
}

void RebuildGameMovement::Accelerate(IClientEntity *player, Vector &wishdir, float wishspeed, float accel)
{

}

void RebuildGameMovement::AirAccelerate(IClientEntity *player, Vector &wishdir, float wishspeed, float accel)
{

}

void RebuildGameMovement::AirMove(IClientEntity *player)
{

}

void RebuildGameMovement::StepMove(IClientEntity *player, Vector &vecDestination, trace_t &trace)
{

}

void RebuildGameMovement::TracePlayerBBox(const Vector &start, const Vector &end, unsigned int fMask, int collisionGroup, trace_t& pm, IClientEntity *player)
{
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = reinterpret_cast<void*>(player);

	ray.Init(start, end, player->collisionProperty()->GetMins(), player->collisionProperty()->GetMaxs());
	
	m_pTrace->TraceRay(ray, fMask, &filter, &pm);
}

void RebuildGameMovement::WalkMove(IClientEntity *player)
{

}

void RebuildGameMovement::FinishGravity(IClientEntity *player)
{

}

void RebuildGameMovement::FullWalkMove(IClientEntity *player)
{
	// ~ deleted parts cause pub release

	StartGravity(player);

	// Fricion is handled before we add in any base velocity. That way, if we are on a conveyor, 
	// we don't slow when standing still, relative to the conveyor.
	if (player->GetFlags() & FL_ONGROUND)
	{
		player->GetVelocity()[2] = 0.0;
		Friction(player);
	}

	// Make sure velocity is valid.
	CheckVelocity(player);

	if (player->GetFlags() & FL_ONGROUND)
	{
		WalkMove(player);
	}
	else
	{
		AirMove(player);  // Take into account movement when in air.
	}

	// Make sure velocity is valid.
	CheckVelocity(player);

	// Add any remaining gravitational component.
	FinishGravity(player);


	// If we are on ground, no downward velocity.
	if (player->GetFlags() & FL_ONGROUND)
	{
		player->GetVelocity()[2] = 0;
	}

	CheckFalling(player);
}

void RebuildGameMovement::Friction(IClientEntity *player)
{

}


void RebuildGameMovement::CheckFalling(IClientEntity *player)
{

}

const int nanmask = 255 << 23;
#define	IS_NAN(x) (((*(int *)&x)&nanmask)==nanmask)
void RebuildGameMovement::CheckVelocity(IClientEntity *player)
{

}
void RebuildGameMovement::StartGravity(IClientEntity *player)
{

}