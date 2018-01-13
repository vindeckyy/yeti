#pragma once

#include "Interfaces.h"

class CGameMath
{
public:
	float random_float(float min, float max)
	{
		typedef float(*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle(XorStr("vstdlib.dll")), XorStr("RandomFloat"));
		return m_RandomFloat(min, max);
	}
	int random_seed(int seed) {
		typedef int(__cdecl* RandomSeed_t)(int iSeed);
		static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle(XorStr("vstdlib.dll")), XorStr("RandomSeed"));
		return m_RandomSeed(seed);
	}
	void normalize_float(float& input)
	{
		while (input < 180)
			input += 180;
		while (input > 180)
			input -= 180;
	}
	void normalize_vector(Vector& input)
	{
		while (input.y <= -180) input.y += 360;
		while (input.y > 180) input.y -= 360;
		while (input.x <= -180) input.x += 360;
		while (input.x > 180) input.x -= 360;


		if (input.x > 89.0) input.x = 89;
		if (input.x < -89.0) input.x = -89;
		if (input.y < -180) input.y = -179.999;
		if (input.y > 180) input.y = 179.999;

		input.z = 0;
	}
	void normalize(Vector &vIn, Vector &vOut)
	{
		float flLen = vIn.Length();
		if (flLen == 0) {
			vOut.Init(0, 0, 1);
			return;
		}
		flLen = 1 / flLen;
		vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
	}
	void angle_vectors(const Vector &angles, Vector& forward)
	{
		Assert(s_bMathlibInitialized);
		Assert(forward);

		float	sp, sy, cp, cy;

		sy = sin(DEG2RAD(angles[1]));
		cy = cos(DEG2RAD(angles[1]));

		sp = sin(DEG2RAD(angles[0]));
		cp = cos(DEG2RAD(angles[0]));

		forward.x = cp*cy;
		forward.y = cp*sy;
		forward.z = -sp;
	}
	void vector_transform(const Vector in1, float in2[3][4], Vector &out)
	{
		out[0] = DotProduct(in1, Vector(in2[0][0], in2[0][1], in2[0][2])) + in2[0][3];
		out[1] = DotProduct(in1, Vector(in2[1][0], in2[1][1], in2[1][2])) + in2[1][3];
		out[2] = DotProduct(in1, Vector(in2[2][0], in2[2][1], in2[2][2])) + in2[2][3];
	}
	void vector_angles(const Vector &vecForward, Vector &vecAngles)
	{
		Vector vecView;
		if (vecForward[1] == 0.f && vecForward[0] == 0.f)
		{
			vecView[0] = 0.f;
			vecView[1] = 0.f;
		}
		else
		{
			vecView[1] = atan2(vecForward[1], vecForward[0]) * 180.f / 3.14159265358979323846f;

			if (vecView[1] < 0.f)
				vecView[1] += 360.f;

			vecView[2] = sqrt(vecForward[0] * vecForward[0] + vecForward[1] * vecForward[1]);

			vecView[0] = atan2(vecForward[2], vecView[2]) * 180.f / 3.14159265358979323846f;
		}

		vecAngles[0] = -vecView[0];
		vecAngles[1] = vecView[1];
		vecAngles[2] = 0.f;
	}
	void vector_angles(Vector &forward, Vector &up, Vector &angle)
	{
		Vector left = CrossProduct(up, forward);
		left.NormalizeInPlace();

		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f)
		{
			angle.x = atan2f(-forward.z, forwardDist) * 180 / ((float)(3.14159265358979323846264338327950288419));
			angle.y = atan2f(forward.y, forward.x) * 180 / ((float)(3.14159265358979323846264338327950288419));

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angle.z = atan2f(left.z, upZ) * 180 / ((float)(3.14159265358979323846264338327950288419));
		}
		else
		{
			angle.x = atan2f(-forward.z, forwardDist) * 180 / ((float)(3.14159265358979323846264338327950288419));
			angle.y = atan2f(-left.x, left.y) * 180 / ((float)(3.14159265358979323846264338327950288419));
			angle.z = 0;
		}
	}
	inline vec_t vector_normalize(Vector& v)
	{
		vec_t l = v.Length();

		if (l != 0.0f)
		{
			v /= l;
		}
		else
		{
			v.x = v.y = 0.0f; v.z = 1.0f;
		}

		return l;
	}
	void inline sincos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}
	void angle_vectors(const Vector &angles, Vector *forward) {
		float sp, sy, cp, cy;

		sincos(DEG2RAD(angles[1]), &sy, &cy);
		sincos(DEG2RAD(angles[0]), &sp, &cp);

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}
	void angle_vectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
	{
		float sr, sp, sy, cr, cp, cy;

		sincos(DEG2RAD(angles[1]), &sy, &cy);
		sincos(DEG2RAD(angles[0]), &sp, &cp);
		sincos(DEG2RAD(angles[2]), &sr, &cr);

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}
	void angle_clamp(CUserCmd* pCmd, Vector oang)
	{
		normalize_vector(pCmd->viewangles);
		if (pCmd->viewangles.z != 0.0f)
			pCmd->viewangles.z = 0.00;

		if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
		{
			normalize_vector(pCmd->viewangles);
			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				pCmd->viewangles = oang;
		}
	}
	void calculate_angle(Vector src, Vector dst, Vector &angles)
	{
		Vector delta = src - dst;
		double hyp = delta.Length2D();
		angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
		angles.x = (atan(delta.z / hyp) * 57.295779513082f);
		angles[2] = 0.0f;
		if (delta.x >= 0.0) angles.y += 180.0f;
	}
	FORCEINLINE void vector_subtract(const Vector& a, const Vector& b, Vector& c)
	{
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}
	bool is_close(float value1, float value2, float max_difference)
	{
		float buffered[2] = { value1, value2 };

		if (buffered[0] < 180) buffered[0] += 360;
		if (buffered[0] > 180) buffered[0] -= 360;
		if (buffered[1] < 180) buffered[1] += 360;
		if (buffered[1] > 180) buffered[1] -= 360;

		if ((buffered[0] - buffered[1]) >= -max_difference && (buffered[0] - buffered[1]) <= max_difference)
			return true;

		return false;
	}
	void matrix_set_column(const Vector &in, int column, matrix3x4_t& out)
	{
		out.m_flMatVal[0][column] = in.x;
		out.m_flMatVal[1][column] = in.y;
		out.m_flMatVal[2][column] = in.z;
	}
	void angle_matrix(Vector& angles, const Vector& position, matrix3x4_t& matrix)
	{
		angle_matrix(angles, matrix);
		matrix_set_column(position, 3, matrix);
	}
	template<class T, class U>
	T clamp(T in, U low, U high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

private:
	void angle_matrix(Vector& angles, matrix3x4_t& matrix)
	{
		float angle;
		float sr, sp, sy, cr, cp, cy;

		angle = angles.x * (6.283185f / 360);
		sp = sin(angle);
		cp = cos(angle);

		angle = angles.y * (6.283185f / 360);
		sy = sin(angle);
		cy = cos(angle);

		angle = angles.z * (6.283185f / 360);
		sr = sin(angle);
		cr = cos(angle);

		matrix.m_flMatVal[0][0] = cp * cy;
		matrix.m_flMatVal[1][0] = cp * sy;

		matrix.m_flMatVal[2][0] = -sp;
		matrix.m_flMatVal[0][1] = sr * sp * cy + cr * -sy;

		matrix.m_flMatVal[1][1] = sr * sp * sy + cr * cy;
		matrix.m_flMatVal[2][1] = sr * cp;

		matrix.m_flMatVal[0][2] = (cr * sp * cy + -sr * -sy);
		matrix.m_flMatVal[1][2] = (cr * sp * sy + -sr * cy);

		matrix.m_flMatVal[2][2] = cr * cp;

		matrix.m_flMatVal[0][3] = 0.0;
		matrix.m_flMatVal[1][3] = 0.0;
		matrix.m_flMatVal[2][3] = 0.0;
	}
};