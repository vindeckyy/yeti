#pragma once

#include "Interfaces.h"
#include "CRC32.h"
#include "Menu.h"
#include <iostream>

extern double PlistArray[ 102 ][ 200 ];

class CPlayer
{
public:
	void Add_Player( IClientEntity *Ent )
	{
		entity = Ent;
		if ( !entity )
			index = -1;
		else
			index = entity->GetIndex( );
		m_pEngine->GetPlayerInfo( index, info );
		name = info->name;
		is_valid_player = true;
	}

	void Reset_Player( )
	{
		is_valid_player = false;
		name = "unamed";
		selected = false;
		resolver_data.last_hit_angle = Vector( 0, 0, 0 );
		resolver_data.has_hit_angle = false;
		resolver_data.missed_shots = 0;
		resolver_data.shots = 0;
		resolver_data.time_at_update = 0;
		resolver_data.stored_lby = 0;
		resolver_data.temp = 0;
		resolver_data.old_lowerbody_yaws = 0;
		resolver_data.resolved_yaw = 0;
		resolver_data.old_yaw_deltas = 0;
		resolver_data.newer_stored_lby = 0;
		resolver_data.stored_lby_two = 0;
		resolver_data.lowerbody_is_updated = false;
		possible_spinspeed = 0;
		dormancy_data.initialized = false;
		dormancy_data.last_health = 0;
		dormancy_data.last_armor = 0;
		dormancy_data.last_ammo = 0;
		dormancy_data.last_team = 0;
		dormancy_data.last_visible = false;

		last_origin = Vector( 0, 0, 0 );

		Priority = false;
		Friendly = false;
		PreferBodyAim = false;
		ForcePitch = false;
		ForcePitch_Pitch = 0;
		ForceYaw = false;
		ForceYaw_Yaw = 0;

		aimbot_bone = 0;

		if ( index != -1 ) {
			for ( int i = 0; i < 199; i++ )
			{
				PlistArray[ index ][ i ] = 0;
			}
		}
		index = -1;
	}
public:
	int index = -1;
	IClientEntity* entity;
	player_info_t* info = new player_info_t;
	char* name = "unamed";
	bool is_valid_player = true;
	bool selected = false;
	float possible_spinspeed = 0;
	class DormanyData
	{
	public:
		float last_health = 0;
		float last_armor = 0;
		Vector last_min;
		Vector last_max;
		Vector last_origin;
		CBaseCombatWeapon* last_weapon;
		int last_ammo = 0;
		int last_team = 0;
		bool last_visible = false;
		bool initialized = true;
	};
	DormanyData dormancy_data;

	class ResolverData
	{
	public:
		Vector last_hit_angle = Vector( 0, 0, 0 );
		bool has_hit_angle = false;
		int missed_shots = 0;
		int shots = 0;
		float time_at_update;
		float stored_lby;
		float temp;
		float old_lowerbody_yaws;
		float resolved_yaw;
		float old_yaw_deltas;
		float newer_stored_lby;
		float stored_lby_two;
		bool lowerbody_is_updated;
	};
	ResolverData resolver_data;

	Vector last_origin = Vector( 0, 0, 0 );

	bool Priority = false;
	bool Friendly = false;
	bool PreferBodyAim = false;
	bool ForcePitch = false;
	int ForcePitch_Pitch = 0;
	bool ForceYaw = false;
	int ForceYaw_Yaw = 0;

	int aimbot_bone = 0;
};

class CPlist
{
private:
	bool Setup = false;
	CPlayer Players[ 100 ];
	int previous_player = 0;
public:
	int selected_player = 0;
public:
	void update( )
	{
		if ( m_pEngine->IsConnected( ) && m_pEngine->IsInGame( ) )
		{
			IClientEntity* me = m_pEntityList->GetClientEntity( m_pEngine->GetLocalPlayer( ) );

			for ( int i = 0; i < 99; i++ )
			{
				IClientEntity* Ent = m_pEntityList->GetClientEntity( i );
				if ( !Ent ) { Players[ i ].Reset_Player( ); continue; }

				ClientClass* cClass = ( ClientClass* )Ent->GetClientClass( );
				if ( cClass->m_ClassID != 35 )
				{
					Players[ i ].Reset_Player( ); continue;
				}

				if ( Ent == me ) { Players[ i ].Reset_Player( ); continue; }
				Players[ i ].Add_Player( Ent );
			}

			Setup = true;
		}
		else if ( !m_pEngine->IsConnected( ) || !m_pEngine->IsInGame( ) )
		{
			for ( int i = 0; i < std::size( Players ); i++ )
			{
				Players[ i ].Reset_Player( );
			}
		}
	}

	void reset( )
	{
		for ( int i = 0; i < 99; i++ )
		{
			Players[ i ].Reset_Player( );
		}
	}

	CPlayer* get_player( int i )
	{
		return &Players[ i ];
	}

	void paint( )
	{
		static bool initialized_menu = false;
		if ( !initialized_menu )
		{
			Menu::Window.PlayersTab.PlayerPriority.SetState( false );
			Menu::Window.PlayersTab.PlayerFriendly.SetState( false );
			Menu::Window.PlayersTab.PlayerPreferBody.SetState( false );
			Menu::Window.PlayersTab.PlayerForcePitch.SetState( false );
			Menu::Window.PlayersTab.PlayerForceYaw.SetState( false );
			initialized_menu = true;
		}

		if ( m_pEngine->IsInGame( ) && m_pEngine->IsConnected( ) )
		{
			Menu::Window.PlayersTab.PlayerListControl.ClearItems( );
			std::vector <CPlayer*> draw_list;
			for ( int i = 0; i < 99; i++ )
			{
				if ( Players[ i ].is_valid_player )
				{
					draw_list.push_back( &Players[ i ] );
				}
			}

			int players = 0;
			for ( CPlayer* Player : draw_list )
			{
				std::string playername = "";
				for ( int i = 0; i < 24; i++ )
				{
					playername += std::string( Player->name )[ i ];
				}

				Menu::Window.PlayersTab.PlayerListControl.AddItem( playername, Player->index );

				if ( Player->index == Menu::Window.PlayersTab.PlayerListControl.GetValue( ) ) Player->selected = true;
				else Player->selected = false;

				players++;

				if ( Player->selected ) selected_player = Player->entity->GetIndex( );
			}

			CPlayer* Player = get_player( Menu::Window.PlayersTab.PlayerListControl.GetValue( ) );
			if ( previous_player != Menu::Window.PlayersTab.PlayerListControl.GetValue( ) )
			{
				Menu::Window.PlayersTab.PlayerPriority.SetState( Player->Priority );
				Menu::Window.PlayersTab.PlayerFriendly.SetState( Player->Friendly );
				Menu::Window.PlayersTab.PlayerPreferBody.SetState( Player->PreferBodyAim );
				Menu::Window.PlayersTab.PlayerForcePitch.SetState( Player->ForcePitch );
				Menu::Window.PlayersTab.PlayerForcePitch_Pitch.SetIndex( Player->ForcePitch_Pitch );
				Menu::Window.PlayersTab.PlayerForceYaw.SetState( Player->ForceYaw );
				Menu::Window.PlayersTab.PlayerForceYaw_Yaw.SetIndex( Player->ForceYaw_Yaw );
				previous_player = Menu::Window.PlayersTab.PlayerListControl.GetValue( );
			}
			Player->Priority = Menu::Window.PlayersTab.PlayerPriority.GetState( );
			Player->Friendly = Menu::Window.PlayersTab.PlayerFriendly.GetState( );
			Player->PreferBodyAim = Menu::Window.PlayersTab.PlayerPreferBody.GetState( );
			Player->ForcePitch = Menu::Window.PlayersTab.PlayerForcePitch.GetState( );
			Player->ForcePitch_Pitch = Menu::Window.PlayersTab.PlayerForcePitch_Pitch.GetIndex( );
			Player->ForceYaw = Menu::Window.PlayersTab.PlayerForceYaw.GetState( );
			Player->ForceYaw_Yaw = Menu::Window.PlayersTab.PlayerForceYaw_Yaw.GetIndex( );
		}
	}
};
extern CPlist plist;