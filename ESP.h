

#pragma once

#include "Settings.h"
#include "Vector4D.h"

struct Box
{
	int x = -1, y = -1, w = -1, h = -1;
};

class CEsp {
private:
	Color get_player_colors(IClientEntity* m_entity) {
		int team_num = m_entity->GetTeamNum();
		auto m_local = game::localdata.localplayer();
		bool visible = game::functions.visible(m_local, m_entity, (int)CSGOHitboxID::Head);

		return Color(visualconfig.player.cBoxColor, 255);
	}

	float dot_product_t(const float* a, const float* b) {
		return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
	}

	void vector_transform_a(const float *in1, const matrix3x4& in2, float *out) {
		out[0] = dot_product_t(in1, in2[0]) + in2[0][3];
		out[1] = dot_product_t(in1, in2[1]) + in2[1][3];
		out[2] = dot_product_t(in1, in2[2]) + in2[2][3];
	}

	inline void vector_transform_z(const Vector& in1, const matrix3x4 &in2, Vector &out) {
		vector_transform_a(&in1.x, in2, &out.x);
	}

	bool get_box(IClientEntity* m_entity, Box& box, bool dynamic) {
		DWORD m_rgflCoordinateFrame = ( DWORD )0x470 - 0x30;
		const matrix3x4& trnsf = *( matrix3x4* )( ( DWORD )m_entity + ( DWORD )m_rgflCoordinateFrame );

		Vector  vOrigin, min, max, sMin, sMax, sOrigin,
			flb, brt, blb, frt, frb, brb, blt, flt;

		float left, top, right, bottom;

		vOrigin = m_entity->GetOrigin( );
		min = m_entity->collisionProperty( )->GetMins( );
		max = m_entity->collisionProperty( )->GetMaxs( );

		if ( !dynamic ) {
			min += vOrigin;
			max += vOrigin;
		}

		Vector points[ ] = { Vector( min.x, min.y, min.z ),
			Vector( min.x, max.y, min.z ),
			Vector( max.x, max.y, min.z ),
			Vector( max.x, min.y, min.z ),
			Vector( max.x, max.y, max.z ),
			Vector( min.x, max.y, max.z ),
			Vector( min.x, min.y, max.z ),
			Vector( max.x, min.y, max.z ) };

		Vector vector_transformed[ 8 ];

		if ( dynamic )
		{
			for ( int i = 0; i < 8; i++ )
			{
				vector_transform_z( points[ i ], trnsf, vector_transformed[ i ] );
				points[ i ] = vector_transformed[ i ];
			}
		}
		if ( !game::functions.world_to_screen( points[ 3 ], flb ) || !game::functions.world_to_screen( points[ 5 ], brt )
			|| !game::functions.world_to_screen( points[ 0 ], blb ) || !game::functions.world_to_screen( points[ 4 ], frt )
			|| !game::functions.world_to_screen( points[ 2 ], frb ) || !game::functions.world_to_screen( points[ 1 ], brb )
			|| !game::functions.world_to_screen( points[ 6 ], blt ) || !game::functions.world_to_screen( points[ 7 ], flt ) )
			return false;

		Vector arr[ ] = { flb, brt, blb, frt, frb, brb, blt, flt };
		left = flb.x;
		top = flb.y;
		right = flb.x;
		bottom = flb.y;

		for ( int i = 1; i < 8; i++ ) {
			if ( left > arr[ i ].x )
				left = arr[ i ].x;
			if ( bottom < arr[ i ].y )
				bottom = arr[ i ].y;
			if ( right < arr[ i ].x )
				right = arr[ i ].x;
			if ( top > arr[ i ].y )
				top = arr[ i ].y;
		}

		box.x = left;
		box.y = top;
		box.w = right - left;
		box.h = bottom - top;

		return true;
	}

	class CVisualsPlayer {
	public:
		void direction_arrow( const Vector &vec_delta );

		void paint_player(IClientEntity* m_entity, Box box, Color color) {
			draw_box(m_entity, box, color);
			draw_hitbones(m_entity);
			draw_health(m_entity, box);
			draw_armor(m_entity, box);
			draw_name(m_entity, box);
			draw_weapon(m_entity, box);
			draw_hit_angle(m_entity, box);
			draw_skeletons(m_entity, box);
			draw_snaplines(m_entity, box);
			draw_scoped(m_entity, box);
		}
	private:
		void draw_box(IClientEntity* m_entity, Box box, Color color);
		void draw_health(IClientEntity* m_entity, Box box);
		void draw_armor(IClientEntity* m_entity, Box box);
		void draw_name(IClientEntity* m_entity, Box box);
		void draw_weapon(IClientEntity* m_entity, Box box);
		void draw_hit_angle(IClientEntity* m_entity, Box box);
		void draw_skeletons(IClientEntity* m_entity, Box box);
		void draw_snaplines(IClientEntity* m_entity, Box box);
		void draw_hitbones(IClientEntity* m_entity);
		void draw_scoped(IClientEntity* m_entity, Box box);
	};
	CVisualsPlayer player;

	class CGlow
	{
	public:
		void shutdown();
		void paint();
	};

	void antiaim_lines( );

public:
	void paint();
	CGlow glow;
};

extern CEsp esp;

/*class CVisuals
{
public:
	class CGlow
	{
	public:
		void shutdown();
		void paint();
	};
public:
	void paint();

	CGlow glow;
private:
	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, bool dynamic, Box& box);

	void DrawEntity(IClientEntity* entity, const char* string, Color color);
};

extern CVisuals visuals;*/