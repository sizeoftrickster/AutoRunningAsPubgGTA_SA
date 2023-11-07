#ifndef _GTA_SA_HPP_
#define _GTA_SA_HPP_

#include <Windows.h>
#include <fstream>
#include <vector>
#include "Utility.hpp"

#define PLAYER_PED ( *reinterpret_cast<DWORD*>( 0xB6F5F0 ) )

class GTA : Utility
{
public:
	enum KeysNum
	{
		Right = 0x0,
		Left = 0x1,
		Backwards = 0x2,
		Forward = 0x3,
		AccelerateCar = 0x20
	}; // enum KeysNum
	enum KeysValue
	{
		OnFoot = 128,
		OnCar = 255
	}; // enum KeysValue
	static bool IsPedAvailable() {
		if ( !PLAYER_PED )
			return false;
		return true;
	}
	static int GetPedStatus() {
		if ( !IsPedAvailable() )
			return -1;
		return *reinterpret_cast<int*>( PLAYER_PED + 0x530 );
	}
	static bool IsPedDriving() {
		return GetPedStatus() == 50 || GetPedStatus() == 0;
	}
	static void SetGameKey( KeysNum key, KeysValue value ) {
		*reinterpret_cast<int*>( 0xB73458 + key ) = value;
	}
	static BYTE GetGameKey( KeysNum key ) {
		return *reinterpret_cast<BYTE*>( 0xB73458 + key );
	}
	static std::pair<float, float> GetScreenResolution() {
		auto posX = *reinterpret_cast<int*>( 0xC9C040 );
		auto posY = *reinterpret_cast<int*>( 0xC9C044 );
		return std::make_pair( (float)posX, (float)posY );
	}
	static std::string GetDirectoryToGtaSaSet() {
		std::string username = GetSystemUsername();
		char buffer[sizeof( username ) + 60];
		sprintf( buffer, "C:\\Users\\%s\\Documents\\GTA San Andreas User Files\\gta_sa.set", username.data() );
		return buffer;
	}
	static std::vector<int> GetKeysNumberFromGtaSaSet( std::string directoryToSet, std::vector<int> addresses ) {
		std::vector<int> out;
		for ( size_t index = 0; index < addresses.size(); index ++ ) {
			std::ifstream file( directoryToSet, std::ios::binary );
			if ( file.is_open() ) {
				file.seekg( addresses[index] );
				std::streamsize byteCount = file.gcount();
				char buffer[ sizeof( byteCount )];

				file.seekg( addresses[index] );
				file.getline( buffer, sizeof( buffer ) );
            
				auto keyNumber = static_cast<int>( static_cast<int>( buffer[0] ) & 0x0000FF );
				out.push_back( keyNumber );
			}
		}
		return out;
	}
}; // class GTA

#endif // !_GTA_SA_HPP_