#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
#include "imgui/imgui_internal.h"
#include "variables.hpp"
//#include "lua/lua.h"
namespace menu {
	inline int current_tab{0};
	inline IDirect3DTexture9** friends_images;
	inline bool is_open;
	inline bool has_toggled_menu_once;
	inline bool init = false;
	void initialize() noexcept;
	void render() noexcept;
	bool on_init(IDirect3DDevice9* device);
	inline ImFont* indicator_font;
	inline ImFont* indicator_font_small;
	inline ImFont* indicator_font_small_bold;
	inline ImFont* notification_font;
	bool second_init_because_lazy();
	void fov_circle() noexcept;
	inline HWND hwnd;
	inline WNDPROC original_wndproc;
	void draw_skins_tab ();
	inline bool unload_module = false;
	LRESULT WINAPI wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	namespace fonts {
		inline ImFont* indicators;
		inline ImFont* menu_font;
	}
};
