#pragma once
#include "imgui.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <WinUser.h>




namespace custom_gui {
	void create_style ( ImGuiStyle style );
	void begin ();
	void end ();
	void end_style ();
	void make_tabs ( std::vector<std::string> tabs, int& index );
	void side_tabs ( std::vector<std::string> tabs, int& index );

	namespace backend {
		inline ImColor accent = ImColor ( 148, 0, 211 );
		inline int cur_tab;
		inline bool init = false;
		inline ImFont* icons;
		inline ImFont* smaller_icons;
		inline ImFont* weapon_icons;
		inline ImFont* bold_font;
		inline ImFont* title_font;
		inline ImFont* multiselect_font;
		inline ImFont* select_fonts;

		// This goes for checkboxes and what not
		struct toggle_states {
			float alpha;
			bool checked_state;
		};
		struct button_state {
			float alpha;
			float radius;
			bool active;
			ImVec2 pos;
			bool pos_set;
		};
	}
}


namespace c {
	void begin_aimbot_tab ( std::string label, std::vector<std::string> tabs, int& index );
	// Childs
	void begin ( const char* label, bool on_right, bool on_top = false, bool full_size = false);
	void end ();

	void begin_config_tab ( std::string label );
	void begin_scripts_tab ( std::string label );


	// Controls
	bool button ( std::string label );
	void slider_int ( const char* label, int& v, int min, int max, const char* format = "%d" );
	void slider_float ( const char* label, float& v, float min, float max, const char* format = "%.2f" );
	void text ( std::string label );
	void dropdown ( const char* name, int& variable, const char* labels [ ], int count );
	void listbox ( const char* name, int& variable, const char* labels [ ], int count );
	void multi_select ( std::string name, std::vector<int>& variable, const char* labels [ ], int count );
	void multi_select ( std::string name, bool* variable, const char* labels [ ], int count );
	void text_input ( std::string label, char* input, size_t buffer_size );
	void color_picker ( std::string label, float* col, bool second = false, float* sec_col = 0, bool third = false, float* third_col = 0, bool fourth = false, float* fourth_col = 0);
	void keybind ( std::string label, int& keybind );
	inline std::string preview = "none";
	bool checkbox ( std::string label, bool* v );



	// menu vars i guess
	inline bool allow_menu_clr_change = true;
	inline float menu_accent[ 4 ] = { 136 / 255.f, 124 / 255.f, 252 / 255.f, 255 / 255.f };
}


inline const char* const KeyNames [ ] = {
	"none",
	"m1",
	"m2",
	"something",
	"m3",
	"m4",
	"m5",
	"unk",
	"back",
	"tab",
	"unk",
	"unk",
	"clear",
	"enter",
	"unk",
	"unk",
	"shift",
	"control",
	"menu",
	"pause",
	"capital",
	"kana",
	"unk",
	"junja",
	"final",
	"kanja",
	"unk",
	"escape",
	"convert",
	"nonconvert",
	"accept",
	"modechange",
	"space",
	"prior",
	"next",
	"end",
	"home",
	"left",
	"up",
	"right",
	"down",
	"select",
	"print",
	"exec",
	"snap",
	"insert",
	"delete",
	"help",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z",
	"lwin",
	"rwin",
	"apps",
	"unk",
	"sleep",
	"num0",
	"num1",
	"num2",
	"num3",
	"num4",
	"num5",
	"num6",
	"num7",
	"num8",
	"num9",
	"multiply",
	"add",
	"separator",
	"subtract",
	"dec",
	"divide",
	"f1",
	"f2",
	"f3",
	"f4",
	"f5",
	"f6",
	"f7",
	"f8",
	"f9",
	"f10",
	"f11",
	"f12",
	"f13",
	"f14",
	"f15",
	"f16",
	"f17",
	"f18",
	"f19",
	"f20",
	"f21",
	"f22",
	"f23",
	"f24",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"numlock",
	"scroll",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"unk",
	"lshift",
	"rshit",
	"lcontrol",
	"rcontrol",
	"lwin",
	"rwin"
};