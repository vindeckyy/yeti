#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include "Hooks.h"

class CheatUserStruct
{
public:
	std::string name;
	std::vector<int> addr;
	int mac_int;

	CheatUserStruct(std::string _name, std::vector<int> _addr, int _mac_int)
	{
		this->name = _name;
		this->addr = _addr;
		this->mac_int = _mac_int;
	}
};

class SecurityVaults
{
public:
	std::vector<CheatUserStruct> users;
	void setup( )
	{
		int Monarch_addr[ 6 ] = { 248, 188, 18, 254, 108, 71 };
		std::vector<int> Monarch_vector;
		for ( int i = 0; i < 6; i++ )
			Monarch_vector.push_back( Monarch_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "Monarch" ), Monarch_vector, 887 ) );
		int paradox_addr[ 6 ] = { 208, 80, 153, 82, 9, 160 };
		std::vector<int> paradox_vector;
		for ( int i = 0; i < 6; i++ )
			paradox_vector.push_back( paradox_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "paradox" ), paradox_vector, 692 ) );
		int villain_addr[ 6 ] = { 28, 27, 13, 127, 192, 58 };
		std::vector<int> villain_vector;
		for ( int i = 0; i < 6; i++ )
			villain_vector.push_back( villain_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "villain" ), villain_vector, 445 ) );
		int R3DFluXx_addr[ 6 ] = { 76, 204, 106, 73, 39, 8 };
		std::vector<int> R3DFluXx_vector;
		for ( int i = 0; i < 6; i++ )
			R3DFluXx_vector.push_back( R3DFluXx_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "R3DFluXx" ), R3DFluXx_vector, 506 ) );
		int millions_addr[ 6 ] = { 112, 28, 231, 4, 244, 11 };
		std::vector<int> millions_vector;
		for ( int i = 0; i < 6; i++ )
			millions_vector.push_back( millions_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "millions" ), millions_vector, 630 ) );
		int jeremymetzler_addr[ 6 ] = { 0, 33, 155, 8, 32, 207 };
		std::vector<int> jeremymetzler_vector;
		for ( int i = 0; i < 6; i++ )
			jeremymetzler_vector.push_back( jeremymetzler_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "jeremymetzler" ), jeremymetzler_vector, 435 ) );
		int fluorine_addr[ 6 ] = { 188, 95, 244, 223, 235, 3 };
		int stewartlittle_addr[ 6 ] = { 76, 204, 106, 36, 180, 40 };
		std::vector<int> stewartlittle_vector;
		for ( int i = 0; i < 6; i++ )
			stewartlittle_vector.push_back( stewartlittle_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "stewartlittle" ), stewartlittle_vector, 642 ) );
		std::vector<int> fluorine_vector;
		for ( int i = 0; i < 6; i++ )
			fluorine_vector.push_back( fluorine_addr[ i ] );
		users.push_back( CheatUserStruct( XorStr( "fluorine" ), fluorine_vector, 988 ) );
	}
};
extern SecurityVaults vaults;