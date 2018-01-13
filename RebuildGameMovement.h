#pragma once
#include "Interfaces.h"

class RebuildGameMovement
{

public:

	RebuildGameMovement(void);

	void SetAbsOrigin(IClientEntity *entity, const Vector &vec);
	void FullWalkMove(IClientEntity *entity);
	void CheckVelocity(IClientEntity *entity);
	void FinishGravity(IClientEntity *entity);
	void StepMove(IClientEntity *entity, Vector &vecDestination, trace_t &trace);
	int ClipVelocity(Vector &in, Vector &normal, Vector &out, float overbounce);
	int TryPlayerMove(IClientEntity *entity, Vector *pFirstDest, trace_t *pFirstTrace);
	void Accelerate(IClientEntity *entity, Vector &wishdir, float wishspeed, float accel);
	void Friction(IClientEntity *entity);
	void AirAccelerate(IClientEntity *entity, Vector &wishdir, float wishspeed, float accel);
	void AirMove(IClientEntity *entity);
	void WalkMove(IClientEntity *entity);
	void CheckFalling(IClientEntity *entity);
	void StartGravity(IClientEntity *entity);
	void TracePlayerBBox(const Vector &start, const Vector &end, unsigned int fMask, int collisionGroup, trace_t &pm, IClientEntity *entity);
};