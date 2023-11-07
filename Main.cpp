#include "Main.h"
#include "Direct3D9Hook.h"
#include "Gta_sa.hpp"

AsiPlugin::AsiPlugin() {
	wndprocHook.set_cb( [this]( auto&&... args ) { return AsiPlugin::WndProcHooked( args... ); } );
	wndprocHook.install();
	mainLoop = std::thread( &AsiPlugin::MainLooped, this );
	mainLoop.detach();
	directxHook = std::make_unique<Direct3D9Hook>();

	std::string SaSetLocation = GTA::GetDirectoryToGtaSaSet();
	auto foot = GTA::GetKeysNumberFromGtaSaSet( SaSetLocation, onFootKeysAddresses );
	auto car = GTA::GetKeysNumberFromGtaSaSet( SaSetLocation, onCarKeysAddresses );
	for ( int value : foot ) {
		if ( value != 32 ) {
			onFootKeys.push_back( value );
		}
	}
	for ( int value : car ) {
		if ( value != 32 ) {
			onCarKeys.push_back( value );
		}
	}
}

AsiPlugin::~AsiPlugin() {
	if ( mainLoop.joinable() )
		mainLoop.join();
	wndprocHook.remove();
}

void AsiPlugin::MainLooped() {
	while ( true ) {
		if ( !isMenuOpened && isEnabled ) {
			if ( GTA::IsPedDriving() ) {
				GTA::SetGameKey( GTA::KeysNum::AccelerateCar, GTA::KeysValue::OnCar );
			} else {
				GTA::SetGameKey( GTA::KeysNum::Forward, GTA::KeysValue::OnFoot );
			}
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 0 ) );
	}
}


LRESULT AsiPlugin::WndProcHooked( const decltype( wndprocHook )& hook, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	if ( !isMenuOpened && uMsg == WM_KEYDOWN ) {
		if ( wParam == VK_OEM_PLUS ) {
			isEnabled ^= true;
		}
		if ( isEnabled ) {
			if ( GTA::IsPedDriving() ) {
				for ( auto i : onCarKeys ) {
					if ( wParam == i ) {
						isEnabled = false;
						break;
					}
				}
			} else {
				for ( auto i : onFootKeys ) {
					if ( wParam == i ) {
						isEnabled = false;
						break;
					}
				}
			}
		}
	}
	return hook.get_trampoline()( hWnd, uMsg, wParam, lParam );
}
