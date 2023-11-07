#ifndef _DIRECT3D9_HOOK_H_
#define _DIRECT3D9_HOOK_H_

#include <kthook/kthook.hpp>
#include "Direct3D9Wrapper.hpp"
#include "Switcher.hpp"

using PresentPrototype = HRESULT( __stdcall* )( IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );
using ResetPrototype = HRESULT( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );

class Direct3D9Hook : Switcher
{
    static const int CD3DInfomation_Reset_Index{ 16 };
    static const int CD3DInfomation_Present_Index{ 17 };

	kthook::kthook_signal<PresentPrototype> presentHook{ Direct3D9Wrapper::getFuncAddr( CD3DInfomation_Present_Index ) };
	kthook::kthook_signal<ResetPrototype> resetHook{ Direct3D9Wrapper::getFuncAddr( CD3DInfomation_Reset_Index ) };
public:
	Direct3D9Hook();
	~Direct3D9Hook();
private:
	std::optional<HRESULT> PresentHooked( const decltype( presentHook )& hook, IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );
	std::optional<HRESULT> ResetHooked( const decltype( resetHook )& hook, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
	void ImGui_Install();
	void ImGui_Release();
}; // class Direct3D9Hook

#endif // !_DIRECT3D9_HOOK_H_