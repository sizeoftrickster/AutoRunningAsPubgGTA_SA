#include "Direct3D9Hook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "IconFont.hpp"
#include "Gta_sa.hpp"

Direct3D9Hook::Direct3D9Hook() {
	presentHook.before += [this]( auto&&... args ) { return Direct3D9Hook::PresentHooked( args... ); };
	resetHook.before += [this]( auto&&... args) { return Direct3D9Hook::ResetHooked( args... ); };
	presentHook.install();
	resetHook.install();
}

Direct3D9Hook::~Direct3D9Hook() {
	ImGui_Release();
	resetHook.remove();
	presentHook.remove();
}

std::optional<HRESULT> Direct3D9Hook::PresentHooked( const decltype(presentHook)& hook, IDirect3DDevice9*, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* ) {
	static bool isInitialized = false;
	if ( !isInitialized ) {
		ImGui_Install();
		isInitialized = true;
	}
	if ( !isMenuOpened && isEnabled ) {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		auto drawList = ImGui::GetBackgroundDrawList();
		auto [posX, posY] = GTA::GetScreenResolution();
	
		char buf[128];
		sprintf( buf, "%s", ICON_SMILE );
		drawList->AddText( ImVec2( 10, posY - 30 ), IM_COL32_WHITE, buf );

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
	}
	return std::nullopt;
}

std::optional<HRESULT> Direct3D9Hook::ResetHooked( const decltype( resetHook )& hook, IDirect3DDevice9*, D3DPRESENT_PARAMETERS* ) {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	return std::nullopt;
}

void Direct3D9Hook::ImGui_Install() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	static const ImWchar icons_ranges[] = { ICON_MIN,  ICON_MAX, 0 };
	ImFontConfig fontConfig;
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF( icon_font_compressed_data_base85, 19.0f, &fontConfig, icons_ranges);

	ImGui_ImplWin32_Init( Direct3D9Wrapper::getHWND() );
	ImGui_ImplDX9_Init( Direct3D9Wrapper::getDevice() );
}

void Direct3D9Hook::ImGui_Release() {
	ImGui::DestroyContext();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
}