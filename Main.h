#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <kthook/kthook.hpp>
#include <thread>
#include <memory>
#include <vector>
#include "Switcher.hpp"

using WndProcPrototype = LRESULT( __stdcall* )( HWND, UINT, WPARAM, LPARAM );

class AsiPlugin : Switcher
{
	kthook::kthook_simple<WndProcPrototype> wndprocHook{ 0x747EB0 };
	std::thread mainLoop;

	std::unique_ptr<class Direct3D9Hook> directxHook;

	/*        1      2
		W - 0x2FC | 0x38
		S - 0x308 | 0x44
		A - 0x314 | 0x50
		D - 0x320 | 0x5C 
	*/
	std::vector<int> onFootKeysAddresses = { 0x2FC, 0x38, 0x308, 0x44, 0x314, 0x50, 0x320, 0x5C };
	std::vector<int> onFootKeys;

	/*			  1       2
		Accel - 0x128 | 0x3EC
		Break - 0x3F8 | 0x134
	*/
	std::vector<int> onCarKeysAddresses = { 0x128, 0x3EC, 0x3F8, 0x134 };
	std::vector<int> onCarKeys;
public:
	explicit AsiPlugin();
	virtual ~AsiPlugin();
private:
	void MainLooped();
	LRESULT WndProcHooked( const decltype( wndprocHook )& hook, HWND, UINT, WPARAM, LPARAM );
} AsiPlugin;

#endif // _MAIN_H_