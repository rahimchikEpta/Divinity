#include "custom.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <map>
#include "easing.h"
#include <Windows.h>
#include <WinUser.h>
#include <ranges>
#include "../lua/lua.h"
void custom_gui::create_style ( ImGuiStyle style ) {

	ImGui::PushStyleVar ( ImGuiStyleVar_::ImGuiStyleVar_ChildRounding, 2.f );
	ImGui::GetStyle ().WindowBorderSize = 1.f;
	ImGui::GetStyle ().WindowPadding = ImVec2 ( 0, 0 );
	ImGui::PushStyleColor ( ImGuiCol_FrameBg, ImVec4 ( 0.093f, 0.093f, 0.093f, 1.000f ) );
	ImGui::PushStyleVar ( ImGuiStyleVar_::ImGuiStyleVar_FrameRounding, 3.f );
	ImGui::GetStyle ().FramePadding = ImVec2 ( 5, 3 ); // 5, 3
	//ImGui::PushStyleVar ( ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2 ( 5, 3 ) );
	ImGui::PushStyleVar ( ImGuiStyleVar_::ImGuiStyleVar_PopupRounding, 4.f );
	ImGui::GetStyle ().FrameBorderSize = 1.f;

	ImGui::GetStyle ().WindowRounding = 3.f;
	ImGui::GetStyle ().ChildRounding = 3.f;
	ImGui::GetStyle ().ScrollbarSize = 5.f;
	ImGui::GetStyle ().GrabRounding = 3.f;
	ImGui::GetStyle ().Colors[ ImGuiCol_Separator ] = ImColor ( 6, 6, 8 );
	ImGui::GetStyle ().Colors[ ImGuiCol_ScrollbarBg ] = ImVec4 ( 16 / 255.f, 16 / 255.f, 18 / 255.f, 1.f ); //23, 23, 25
	ImGui::PushStyleColor ( ImGuiCol_WindowBg, ImVec4 ( 10 / 255.f, 10 / 255.f, 10 / 255.f, 1.f ) );
	ImGui::PushStyleColor ( ImGuiCol_ChildBg, ImVec4 ( 15 / 255.f, 15 / 255.f, 15 / 255.f, 1.f ) );
	ImGui::PushStyleColor ( ImGuiCol_FrameBg, ImVec4 ( 12 / 255.f, 12 / 255.f, 12 / 255.f, 0.f ) );
	ImGui::PushStyleColor ( ImGuiCol_Border, ImVec4 ( 26 / 255.f, 26 / 255.f, 26 / 255.f, 1.f ) );
}

void custom_gui::end () {
	ImGui::End ();


}

void custom_gui::end_style () {
	ImGui::PopStyleVar ();
	ImGui::PopStyleVar ();
	//ImGui::PopStyleVar ();
	ImGui::PopStyleVar ();
	ImGui::PopStyleColor ();
	ImGui::PopStyleColor ();
	ImGui::PopStyleColor ();
	ImGui::PopStyleColor ();
	ImGui::PopStyleColor ();

	if ( !c::allow_menu_clr_change ) {
		c::menu_accent[ 0 ] = 143 / 255.f;
		c::menu_accent[ 1 ] = 190 / 255.f;
		c::menu_accent[ 2 ] = 255 / 255.f;
	}

}

bool sub_text_button ( const char* label, const char* label_icon, int index, int index_i, const ImVec2& size_arg ) {

	ImGuiWindow* window = ImGui::GetCurrentWindow ();
	if ( window->SkipItems )
		return false;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID ( label );
	const ImVec2 label_size = ImGui::CalcTextSize ( label, NULL, true );

	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize ( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

	const ImRect bb ( pos, pos + size );
	ImGui::ItemSize ( size, style.FramePadding.y );
	if ( !ImGui::ItemAdd ( bb, id ) )
		return false;



	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior ( bb, id, &hovered, &held, 0 );

	// Render
	float deltatime = ( 1.0f / 0.2f ) * ImGui::GetIO ().DeltaTime * 0.55f;
	float deltatime_a = ( 1.0f / 0.2f ) * ImGui::GetIO ().DeltaTime * 0.55f;

	static std::map<ImGuiID, float> toggled_anim;
	auto toggled = toggled_anim.find ( id );
	if ( toggled == toggled_anim.end () ) {
		static float value = 0.f;
		toggled_anim.insert ( { id, value } );
		toggled = toggled_anim.find ( id );
	}
	if ( index == index_i )
		toggled->second = ImMin ( toggled->second + 1.f * deltatime, 1.f );
	else
		toggled->second = ImMax ( 0.f, toggled->second - 1.f * deltatime );

	static std::map<ImGuiID, float> hovered_anim;
	auto hov_t = hovered_anim.find ( id );
	if ( hov_t == hovered_anim.end () ) {
		static float value = 0.f;
		hovered_anim.insert ( { id, value } );
		hov_t = hovered_anim.find ( id );
	}
	if ( hovered )
		hov_t->second = ImMin ( hov_t->second + 1.f * deltatime, 1.f );
	else
		hov_t->second = ImMax ( 0.f, hov_t->second - 1.f * deltatime );




	static std::map<ImGuiID, float> not_toggled_anim;
	auto not_toggled = not_toggled_anim.find ( id );
	if ( not_toggled == not_toggled_anim.end () ) {
		static float value = 0.f;
		not_toggled_anim.insert ( { id, value } );
		not_toggled = not_toggled_anim.find ( id );
	}

	if ( index != index_i )
		not_toggled->second = ImMin ( not_toggled->second + 1.f * deltatime, 1.f );
	else
		not_toggled->second = ImMax ( 0.f, not_toggled->second - 1.f * deltatime );
	float eased = Easing::InOutCubic ( toggled->second );
	float eased_circ = Easing::OutQuint ( toggled->second );
	float label_eased = Easing::OutQuint ( not_toggled->second );
	/// Render
	auto draw = window->DrawList;

	int alpha = ImGui::GetStyle ().Alpha * 255;
	int tog = ImGui::GetStyle ().Alpha * ( toggled->second * 255 );
	int hov = ImGui::GetStyle ().Alpha * ( hov_t->second * 255 );

	draw->AddRectFilled ( ImVec2 ( pos.x + size.x - 3, pos.y ), ImVec2 ( pos.x + size.x - 1, pos.y + size.y ), ImColor ( 148, 0, 211, tog ), 3.f, ImDrawFlags_RoundCornersLeft );


	draw->AddText ( ImVec2 ( pos.x + size.x / 2 - ( ImGui::CalcTextSize ( label ).x / 2 ), pos.y + ( size.y / 2 ) - ( ImGui::CalcTextSize ( label ).y / 2 ) ), ImColor ( 100, 100, 100, alpha ), label );
	draw->AddText ( ImVec2 ( pos.x + size.x / 2 - ( ImGui::CalcTextSize ( label ).x / 2 ), pos.y + ( size.y / 2 ) - ( ImGui::CalcTextSize ( label ).y / 2 ) ), ImColor ( 148, 0, 211, tog ), label );



	//    IMGUI_TEST_ENGINE_ITEM_INFO ( id, label, g.LastItemData.StatusFlags );
	return pressed;
}
bool sub_tab_button ( const char* label, const char* label_icon, int index, int index_i, const ImVec2& size_arg, int max_index ) {

	ImGuiWindow* window = ImGui::GetCurrentWindow ();
	if ( window->SkipItems )
		return false;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID ( label );
	const ImVec2 label_size = ImGui::CalcTextSize ( label, NULL, true );

	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize ( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

	const ImRect bb ( pos, pos + size );
	ImGui::ItemSize ( size, style.FramePadding.y );
	if ( !ImGui::ItemAdd ( bb, id ) )
		return false;



	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior ( bb, id, &hovered, &held, 0 );

	// Render
	float deltatime = ( 1.0f / 0.2f ) * ImGui::GetIO ().DeltaTime * 0.45f;
	float deltatime_a = ( 1.0f / 0.2f ) * ImGui::GetIO ().DeltaTime * 0.55f;

	static std::map<ImGuiID, float> toggled_anim;
	auto toggled = toggled_anim.find ( id );
	if ( toggled == toggled_anim.end () ) {
		static float value = 0.f;
		toggled_anim.insert ( { id, value } );
		toggled = toggled_anim.find ( id );
	}
	if ( index == index_i )
		toggled->second = ImMin ( toggled->second + 1.f * deltatime, 1.f );
	else
		toggled->second = ImMax ( 0.f, toggled->second - 1.f * deltatime );

	static std::map<ImGuiID, float> hovered_anim;
	auto hov_t = hovered_anim.find ( id );
	if ( hov_t == hovered_anim.end () ) {
		static float value = 0.f;
		hovered_anim.insert ( { id, value } );
		hov_t = hovered_anim.find ( id );
	}
	if ( hovered )
		hov_t->second = ImMin ( hov_t->second + 1.f * deltatime, 1.f );
	else
		hov_t->second = ImMax ( 0.f, hov_t->second - 1.f * deltatime );


	static std::map<ImGuiID, custom_gui::backend::toggle_states> press_anim;
	auto this_toggled = press_anim.find ( id );
	if ( this_toggled == press_anim.end () ) {
		custom_gui::backend::toggle_states newstate;
		newstate.alpha = hovered ? 1.f : 0.f;
		newstate.checked_state = hovered ? true : false;
		press_anim.insert ( { id, newstate } );
		this_toggled = press_anim.find ( id );
	}

	if ( hovered ) {
		if ( !this_toggled->second.checked_state )
			this_toggled->second.alpha += ( 1.0 / 0.3f ) * ImGui::GetIO ().DeltaTime * 0.25f;
		else
			this_toggled->second.alpha -= ( 1.0 / 0.3f ) * ImGui::GetIO ().DeltaTime * 0.2f;
		this_toggled->second.alpha = ImClamp<float> ( this_toggled->second.alpha, 0.f, 1.f );
	}
	else
		this_toggled->second.alpha -= ( 1.0 / 0.3f ) * ImGui::GetIO ().DeltaTime * 0.2f;

	if ( this_toggled->second.alpha == 1.f )
		this_toggled->second.checked_state = true;
	else if ( this_toggled->second.alpha == 0.f )
		this_toggled->second.checked_state = false;
	static std::map<ImGuiID, float> not_toggled_anim;
	auto not_toggled = not_toggled_anim.find ( id );
	if ( not_toggled == not_toggled_anim.end () ) {
		static float value = 0.f;
		not_toggled_anim.insert ( { id, value } );
		not_toggled = not_toggled_anim.find ( id );
	}

	if ( index != index_i )
		not_toggled->second = ImMin ( not_toggled->second + 1.f * deltatime, 1.f );
	else
		not_toggled->second = ImMax ( 0.f, not_toggled->second - 1.f * deltatime );
	int a = 255 * ImGui::GetStyle ().Alpha;
	float eased = index == index_i ? Easing::OutExpo ( toggled->second ) : Easing::InExpo ( toggled->second );
	float eased_circ = Easing::OutQuint ( toggled->second );
	float label_eased = Easing::OutQuint ( not_toggled->second );
	/// Render
	float e_e = this_toggled->second.checked_state ? Easing::OutCirc ( this_toggled->second.alpha ) : Easing::InCirc ( this_toggled->second.alpha );
	auto draw = window->DrawList;
	int toggled_alpha = ( toggled->second * 255 ) * ImGui::GetStyle ().Alpha;
	float t_alpha = ( toggled->second ) * ImGui::GetStyle ().Alpha;
	ImColor cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ], t_alpha );
	ImColor h_cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ], ( e_e - 0.5f ) * ImGui::GetStyle ().Alpha );
	draw->AddRectFilled ( ImVec2 ( pos.x + size.x - ( 60 * eased ), pos.y ), ImVec2 ( pos.x + size.x, pos.y + size.y ), ImColor ( 10, 10, 10, toggled_alpha ), label_eased * 28, ImDrawFlags_RoundCornersLeft );

	draw->AddText ( ImVec2 ( pos.x + size.x / 2 - ( ImGui::CalcTextSize ( label ).x / 2 ), pos.y + ( size.y / 2 ) - ( ImGui::CalcTextSize ( label ).y / 2 ) ), ImColor ( 100, 100, 100, a ), label );
	draw->AddText ( ImVec2 ( pos.x + size.x / 2 - ( ImGui::CalcTextSize ( label ).x / 2 ), pos.y + ( size.y / 2 ) - ( ImGui::CalcTextSize ( label ).y / 2 ) ), h_cc, label );
	draw->AddText ( ImVec2 ( pos.x + size.x / 2 - ( ImGui::CalcTextSize ( label ).x / 2 ), pos.y + ( size.y / 2 ) - ( ImGui::CalcTextSize ( label ).y / 2 ) ), cc, label );



	//    IMGUI_TEST_ENGINE_ITEM_INFO ( id, label, g.LastItemData.StatusFlags );
	return pressed;
}
bool icon_tab_button ( const char* label, const char* label_icon, int index, int index_i, const ImVec2& size_arg, int max_index ) {

	ImGuiWindow* window = ImGui::GetCurrentWindow ();
	if ( window->SkipItems )
		return false;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID ( label );
	const ImVec2 label_size = ImGui::CalcTextSize ( label, NULL, true );

	ImVec2 pos = window->DC.CursorPos;

	ImVec2 size = ImGui::CalcItemSize ( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

	const ImRect bb ( pos, pos + size );
	ImGui::ItemSize ( size, style.FramePadding.y );
	if ( !ImGui::ItemAdd ( bb, id ) )
		return false;



	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior ( bb, id, &hovered, &held, 0 );

	// Render
	float deltatime = ( 1.0f / 0.2f ) * ImGui::GetIO ().DeltaTime * 0.55f;
	float deltatime_a = ( 1.0f / 0.2f ) * ImGui::GetIO ().DeltaTime * 0.55f;

	static std::map<ImGuiID, float> toggled_anim;
	auto toggled = toggled_anim.find ( id );
	if ( toggled == toggled_anim.end () ) {
		static float value = 0.f;
		toggled_anim.insert ( { id, value } );
		toggled = toggled_anim.find ( id );
	}
	if ( index == index_i )
		toggled->second = ImMin ( toggled->second + 1.f * deltatime, 1.f );
	else
		toggled->second = ImMax ( 0.f, toggled->second - 1.f * deltatime );

	static std::map<ImGuiID, float> hovered_anim;
	auto hov_t = hovered_anim.find ( id );
	if ( hov_t == hovered_anim.end () ) {
		static float value = 0.f;
		hovered_anim.insert ( { id, value } );
		hov_t = hovered_anim.find ( id );
	}
	if ( hovered )
		hov_t->second = ImMin ( hov_t->second + 1.f * deltatime, 1.f );
	else
		hov_t->second = ImMax ( 0.f, hov_t->second - 1.f * deltatime );




	static std::map<ImGuiID, float> not_toggled_anim;
	auto not_toggled = not_toggled_anim.find ( id );
	if ( not_toggled == not_toggled_anim.end () ) {
		static float value = 0.f;
		not_toggled_anim.insert ( { id, value } );
		not_toggled = not_toggled_anim.find ( id );
	}

	if ( index != index_i )
		not_toggled->second = ImMin ( not_toggled->second + 1.f * deltatime, 1.f );
	else
		not_toggled->second = ImMax ( 0.f, not_toggled->second - 1.f * deltatime );
	float eased = Easing::InOutCubic ( toggled->second );
	float eased_circ = Easing::OutQuint ( toggled->second );
	float label_eased = Easing::OutQuint ( not_toggled->second );
	/// Render
	auto draw = window->DrawList;

	ImColor cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ], toggled->second );
	ImGui::PushFont ( custom_gui::backend::weapon_icons );
	draw->AddText ( ImVec2 ( pos.x + size.x / 2 - ( ImGui::CalcTextSize ( label ).x / 2 ), pos.y + ( size.y / 2 ) - ( ImGui::CalcTextSize ( label ).y / 2 ) ), ImColor ( 100, 100, 100 ), label );
	draw->AddText ( ImVec2 ( pos.x + size.x / 2 - ( ImGui::CalcTextSize ( label ).x / 2 ), pos.y + ( size.y / 2 ) - ( ImGui::CalcTextSize ( label ).y / 2 ) ), cc, label );
	ImGui::PopFont ();



	//    IMGUI_TEST_ENGINE_ITEM_INFO ( id, label, g.LastItemData.StatusFlags );
	return pressed;
}
void custom_gui::make_tabs ( std::vector<std::string> tabs, int& index ) {
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 0, 0 ) );


	ImGui::SetCursorPosX ( 0 );
	ImGui::SetCursorPosY ( 0 );
	for ( int i = 0; i < tabs.size (); i++ ) {
		if ( sub_tab_button ( tabs.at ( i ).c_str (), tabs.at ( i ).c_str (), i, index, ImVec2 ( 60, 60 ), 2 ) ) {
			index = i;
		}
	}
	ImGui::PopStyleVar ();
}

void custom_gui::side_tabs ( std::vector<std::string> tabs, int& index ) {
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 0, 0 ) );


	ImGui::SetCursorPosX ( 0 );
	ImGui::PushFont ( custom_gui::backend::bold_font );
	for ( int t = 0; t < 3; t++ ) {
		if ( sub_text_button ( tabs.at ( t ).c_str (), tabs.at ( t ).c_str (), t, index, ImVec2 ( 120, 25 ) ) ) {
			index = t;
		}

		//ImGui::SetCursorPosX ( 10 );
	}
	ImGui::PopFont ();
	ImGui::PopStyleVar ();
}

void c::begin_aimbot_tab ( std::string label, std::vector<std::string> tabs, int& index ) {
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 0, 0 ) );
	int padding = ( 45 * tabs.size () );
	int child_width = ImGui::GetWindowSize ().x / 2 - 15;
	int each_button_size = child_width / tabs.size ();
	ImGui::SetCursorPosX ( 10 );
	ImGui::SetCursorPosY ( 55 );
	ImGui::PushFont ( custom_gui::backend::icons );
	for ( int i = 0; i < tabs.size (); i++ ) {
		if ( icon_tab_button ( tabs.at ( i ).c_str (), tabs.at ( i ).c_str (), i, index, ImVec2 ( each_button_size, 35 ), 2 ) ) {
			index = i;
		}
		ImGui::SameLine ();
	}
	ImGui::PopFont ();
	ImGui::PopStyleVar ();

	ImGui::SetCursorPosY ( 90 );

	ImGui::SetCursorPosX ( 10 );

	ImGui::GetStyle ().ItemSpacing = ImVec2 ( 10, 6 );
	// sorry for this awful code </3
	ImVec2 size = ImVec2 ( ImGui::GetWindowSize ().x / 2 - 15, ( ImGui::GetWindowSize ().y - 166 ) / 2 ); // 75
	ImGui::BeginChild ( label.c_str (), size, true, ImGuiWindowFlags_NoScrollbar );
	auto dl = ImGui::GetWindowDrawList (); auto pos = ImGui::GetWindowPos (); auto win_size = ImGui::GetWindowSize ();
	ImGui::PushFont ( custom_gui::backend::bold_font );
	dl->AddText ( ImVec2 ( pos.x + 7, pos.y + 2 ), ImColor ( 200, 200, 200 ), label.c_str () );
	ImGui::PopFont ();
	ImColor cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ] );
	dl->AddLine ( ImVec2 ( pos.x, pos.y + 22 ), ImVec2 ( pos.x + win_size.x, pos.y + 22 ), cc, 2.f );

	//ImGui::PushStyleVar ( ImGuiStyleVar_WindowPadding, ImVec2 ( 7, 6 ) );

	// Set the controls pos
	ImGui::SetCursorPosY ( 24 );
	ImGui::BeginChildFrame ( ImGui::GetID ( label.c_str () ), ImVec2 ( ImGui::GetWindowSize ().x, size.y - 24 ) );
	ImGui::SetCursorPosY ( 10 );
}

void custom_gui::begin () {

	ImGui::SetNextWindowSize ( ImVec2 ( 680, 580 ), ImGuiCond_Once );
	ImColor cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ] );
	ImGui::SetNextWindowSizeConstraints(ImVec2(680, 580), ImGui::GetIO().DisplaySize);
	ImGui::Begin ( "__window__", nullptr, ImGuiWindowFlags_NoTitleBar );
	auto dl = ImGui::GetWindowDrawList (); auto pos = ImGui::GetWindowPos (); auto size = ImGui::GetWindowSize (); int alpha = ImGui::GetStyle ().Alpha * 255;
	int a = 255 * ImGui::GetStyle ().Alpha;
	dl->AddRectFilled ( pos, ImVec2 ( pos.x + 60, pos.y + size.y ), ImColor ( 20, 20, 20, a ), ImGui::GetStyle ().WindowRounding, ImDrawFlags_RoundCornersRight );
	ImGui::PushFont ( backend::icons );

	custom_gui::make_tabs ( { "a", "p", "v", "s", "m", "c" }, custom_gui::backend::cur_tab );

	ImGui::PopFont ();
	// storing custom var

	if ( !c::allow_menu_clr_change ) {
		c::menu_accent[ 0 ] = 143 / 255.f;
		c::menu_accent[ 1 ] = 190 / 255.f;
		c::menu_accent[ 2 ] = 255 / 255.f;
	}
}

void c::begin ( const char* label, bool on_right, bool on_top, bool full_size ) {
	// lmao dont ask
	if ( on_top )
		ImGui::SetCursorPosY ( 10 );
	if ( on_right )
		ImGui::SetCursorPosX ( ( ImGui::GetWindowSize ().x + 20 ) / 2 + 25 );
	else
		ImGui::SetCursorPosX ( 70 );
	if ( on_right && !on_top )
		ImGui::SameLine ();
	ImGui::GetStyle ().ItemSpacing = ImVec2 ( 10, 6 );
	// sorry for this awful code </3
	int a = ImGui::GetStyle ().Alpha * 255;
	float fla = ImGui::GetStyle ().Alpha;
	ImVec2 size = ImVec2 ( ( ImGui::GetWindowSize ().x - 60 ) / 2 - 15, !full_size ? ( ImGui::GetWindowSize ().y - 30 ) / 2 : ImGui::GetWindowSize ().y - 20 ); // 75
	ImGui::BeginChild ( label, size, true, ImGuiWindowFlags_NoScrollbar );
	auto dl = ImGui::GetWindowDrawList (); auto pos = ImGui::GetWindowPos (); auto win_size = ImGui::GetWindowSize ();
	ImGui::PushFont ( custom_gui::backend::bold_font );
	dl->AddText ( ImVec2 ( pos.x + 7, pos.y + 2 ), ImColor ( 200, 200, 200, a ), label );
	ImGui::PopFont ();
	ImColor cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ], fla );
	dl->AddLine ( ImVec2 ( pos.x, pos.y + 22 ), ImVec2 ( pos.x + win_size.x, pos.y + 22 ), cc, 2.f );

	//ImGui::PushStyleVar ( ImGuiStyleVar_WindowPadding, ImVec2 ( 7, 6 ) );

	// Set the controls pos
	ImGui::SetCursorPosY ( 24 );
	ImGui::GetForegroundDrawList ()->PushClipRect ( ImVec2 ( ImGui::GetWindowPos ().x, ImGui::GetWindowPos ().y + 24 ), ImVec2 ( ImGui::GetWindowPos ().x + ImGui::GetWindowSize ().x, ImGui::GetWindowPos ().y + ( size.y - 24 ) ) );
	ImGui::BeginChildFrame ( ImGui::GetID ( label ), ImVec2 ( ImGui::GetWindowSize ().x, size.y - 24 ) );
	ImGui::Spacing();
}

void c::end () {
	ImGui::Spacing ();
	ImGui::EndChildFrame ();
	ImGui::GetForegroundDrawList ()->PopClipRect ();
	ImGui::GetStyle ().ItemSpacing = ImVec2 ( 10, 10 );
	//ImGui::PopStyleVar ();
	ImGui::EndChild ();

}

void c::begin_config_tab ( std::string label ) {
	ImGui::SetCursorPosY ( 55 );

	ImGui::SetCursorPosX ( 10 );

	ImGui::GetStyle ().ItemSpacing = ImVec2 ( 10, 6 );
	// sorry for this awful code </3
	ImVec2 size = ImVec2 ( ImGui::GetWindowSize ().x / 2 - 15, ( ImGui::GetWindowSize ().y - 86 ) ); // 75
	ImGui::BeginChild ( label.c_str (), size, true, ImGuiWindowFlags_NoScrollbar );
	auto dl = ImGui::GetWindowDrawList (); auto pos = ImGui::GetWindowPos (); auto win_size = ImGui::GetWindowSize ();
	ImGui::PushFont ( custom_gui::backend::bold_font );
	dl->AddText ( ImVec2 ( pos.x + 7, pos.y + 2 ), ImColor ( 200, 200, 200 ), label.c_str () );
	ImGui::PopFont ();
	ImColor cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ] );
	dl->AddLine ( ImVec2 ( pos.x, pos.y + 22 ), ImVec2 ( pos.x + win_size.x, pos.y + 22 ), cc, 2.f );


	// Set the controls pos
	ImGui::SetCursorPosY ( 24 );
	ImGui::BeginChildFrame ( ImGui::GetID ( label.c_str () ), ImVec2 ( ImGui::GetWindowSize ().x, size.y - 24 ) );
	ImGui::SetCursorPosY ( 10 );


}

void c::begin_scripts_tab ( std::string label ) {
	ImGui::SetCursorPosY ( 55 );

	ImGui::SameLine ();

	ImGui::GetStyle ().ItemSpacing = ImVec2 ( 10, 6 );
	// sorry for this awful code </3
	ImVec2 size = ImVec2 ( ImGui::GetWindowSize ().x / 2 - 15, ( ImGui::GetWindowSize ().y - 86 ) ); // 75
	ImGui::BeginChild ( label.c_str (), size, true, ImGuiWindowFlags_NoScrollbar );
	auto dl = ImGui::GetWindowDrawList (); auto pos = ImGui::GetWindowPos (); auto win_size = ImGui::GetWindowSize ();
	ImGui::PushFont ( custom_gui::backend::bold_font );
	dl->AddText ( ImVec2 ( pos.x + 7, pos.y + 2 ), ImColor ( 200, 200, 200 ), label.c_str () );
	ImGui::PopFont ();
	ImColor cc = ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ] );
	dl->AddLine ( ImVec2 ( pos.x, pos.y + 22 ), ImVec2 ( pos.x + win_size.x, pos.y + 22 ), cc, 2.f );

	ImGui::SetCursorPosY ( 24 );
	ImGui::BeginChildFrame ( ImGui::GetID ( label.c_str () ), ImVec2 ( ImGui::GetWindowSize ().x, size.y - 24 ) );
	ImGui::SetCursorPosY ( 10 );


}

// Main controls
bool c::checkbox ( std::string label, bool* v ) {
	ImGui::SetCursorPosX ( 10 );
	ImGui::GetStyle ().FramePadding = ImVec2 ( 5, 2 );
	return ImGui::Checkbox ( label.c_str (), v );
}

bool c::button ( std::string label ) {
	ImGui::SetCursorPosX ( 10 );
	return ImGui::Button ( label.c_str (), ImVec2 ( ImGui::GetWindowSize ().x - 20, 24 ) );
}

void c::slider_int ( const char* label, int& v, int min, int max, const char* format ) {
	ImGui::SetCursorPosX ( 10 );
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 6, 3 ) );
	ImGui::Text ( label );
	ImGui::SetCursorPosX ( 11 );
	ImGui::PopStyleVar ();
	//ImGui::GetStyle().FramePadding = ImVec2(5, 3);
	/// Retarded way to do the width LOL
	//ImGui::SetNextItemWidth(ImGui::GetWindowSize().x - 35 - 35);
	ImGui::SliderInt ( label, &v, min, max, format );
	ImGui::GetStyle ().FramePadding = ImVec2 ( 5, 3 );
}

void c::slider_float ( const char* label, float& v, float min, float max, const char* format ) {
	ImGui::SetCursorPosX ( 10 );
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 6, 3 ) );
	ImGui::Text ( label );
	ImGui::SetCursorPosX ( 11 );
	ImGui::PopStyleVar ();
	ImGui::SliderFloat ( label, &v, min, max, format );
	ImGui::GetStyle ().FramePadding = ImVec2 ( 5, 3 );
}

void c::text ( std::string label ) {
	ImGui::SetCursorPosX ( 10 );
	ImGui::Text ( label.c_str () );
}

void c::dropdown ( const char* name, int& variable, const char* labels [ ], int count ) {
	ImGui::SetCursorPosX ( 10 );
	ImGui::GetStyle ().FramePadding = ImVec2 ( 5, 2 );
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 6, 3 ) );
	ImGui::Text ( name );
	ImGui::SetCursorPosX ( 11 );
	ImGui::PopStyleVar ();

	ImGui::PushStyleColor ( ImGuiCol_PopupBg, ImVec4 ( 20 / 255.f, 20 / 255.f, 20 / 255.f, ImGui::GetStyle ().Alpha ) );
	ImGui::PushStyleColor ( ImGuiCol_Border, ImVec4 ( 45 / 255.f, 45 / 255.f, 45 / 255.f, 0.f ) );
	ImGui::GetStyle ().PopupRounding = 0.f;
	ImGui::Combo ( std::string ( "##" + std::string ( name ) ).c_str (), &variable, labels, count );
	ImGui::GetStyle ().PopupRounding = 3.f;
	ImGui::PopStyleColor ();
	ImGui::PopStyleColor ();
}

void c::listbox ( const char* name, int& variable, const char* labels [ ], int count ) {

	ImGui::SetCursorPosX ( 10 );
	ImGui::Text ( name );
	ImGui::SetCursorPosX ( 10 );
	ImGui::ListBox ( std::string ( "##" ).append ( name ).c_str (), &variable, labels, count );
}

void c::multi_select ( std::string name, std::vector<int>& variable, const char* labels [ ], int count ) {
	auto hashname = ( "##" ) + name;

	for ( auto i = 0, j = 0; i < count; i++ ) {
		if ( variable[ i ] ) {
			if ( j )
				preview += ( ", " ) + (std::string) labels[ i ];
			else
				preview = labels[ i ];

			j++;
		}
	}
	ImGui::SetCursorPosX ( 10 );
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 6, 3 ) );
	ImGui::Text ( name.c_str () );
	ImGui::SetCursorPosX ( 11 );
	ImGui::PushStyleColor ( ImGuiCol_PopupBg, ImVec4 ( 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f ) );
	ImGui::PushStyleColor ( ImGuiCol_Border, ImVec4 ( 45 / 255.f, 45 / 255.f, 45 / 255.f, 0.f ) );
	ImGui::GetStyle ().PopupRounding = 0.f;
	if ( ImGui::BeginCombo ( hashname.c_str (), preview.c_str (), ImGuiComboFlags_RenderAsMultiSelect ) ) // draw start
	{
		ImGui::BeginGroup ();
		{

			for ( auto i = 0; i < count; i++ )
				ImGui::Selectable ( labels[ i ], (bool*) &variable[ i ], ImGuiSelectableFlags_DontClosePopups );

		}
		ImGui::EndGroup ();

		ImGui::EndCombo ();
	}
	ImGui::PopStyleColor ();
	ImGui::PopStyleColor ();
	ImGui::PopStyleVar ();
	ImGui::GetStyle ().PopupRounding = 3.f;
	preview = ( "None" ); // reset preview to use later
}

void c::multi_select ( std::string name, bool* variable, const char* labels [ ], int count ) {
	auto hashname = ( "##" ) + name;

	for ( auto i = 0, j = 0; i < count; i++ ) {
		if ( variable[ i ] ) {
			if ( j )
				preview += ( ", " ) + (std::string) labels[ i ];
			else
				preview = labels[ i ];

			j++;
		}
	}
	ImGui::SetCursorPosX ( 10 );
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 6, 3 ) );
	ImGui::Text ( name.c_str () );
	ImGui::SetCursorPosX ( 11 );
	ImGui::PushStyleColor ( ImGuiCol_PopupBg, ImVec4 ( 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f ) );
	ImGui::PushStyleColor ( ImGuiCol_Border, ImVec4 ( 45 / 255.f, 45 / 255.f, 45 / 255.f, 0.f ) );
	ImGui::GetStyle ().PopupRounding = 0.f;
	if ( ImGui::BeginCombo ( hashname.c_str (), preview.c_str (), ImGuiComboFlags_RenderAsMultiSelect ) ) // draw start
	{
		ImGui::BeginGroup ();
		{

			for ( auto i = 0; i < count; i++ )
				ImGui::Selectable ( labels[ i ], (bool*) &variable[ i ], ImGuiSelectableFlags_DontClosePopups );

		}
		ImGui::EndGroup ();

		ImGui::EndCombo ();
	}
	ImGui::PopStyleColor ();
	ImGui::PopStyleColor ();
	ImGui::PopStyleVar ();
	ImGui::GetStyle ().PopupRounding = 3.f;
	preview = ( "None" ); // reset preview to use later
}

void c::text_input ( std::string label, char* input, size_t buffer_size ) {
	ImGui::SetCursorPosX ( 10 );
	ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 6, 3 ) );
	ImGui::Text ( label.c_str () );
	ImGui::SetCursorPosX ( 8 );
	ImGui::PopStyleVar ();
	ImGui::InputText ( std::string ( "##" ).append ( label ).c_str (), input, buffer_size );
}

// fun fact, i almost sent the gui without a color picker.
void c::color_picker ( std::string label, float* col, bool second, float* sec_col, bool third, float* third_col, bool fourth, float* fourth_col ) {
	ImGui::PushStyleVar ( ImGuiStyleVar_WindowPadding, ImVec2 ( 0, 0 ) );
	ImGui::PushStyleVar ( ImGuiStyleVar_FramePadding, ImVec2 ( 6.f, 2.f ) );
	ImGui::PushStyleVar ( ImGuiStyleVar_FrameRounding, 3.f );
	ImGui::GetStyle ().PopupRounding = 0.f;

	ImGui::SameLine ();
	if (fourth) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - 101);
		ImGui::ColorEdit4(label.c_str(), col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
		ImGui::SameLine();
		ImGui::ColorEdit4(std::string("2").append(label).c_str(), sec_col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
		ImGui::SameLine();
		ImGui::ColorEdit4(std::string("3").append(label).c_str(), third_col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
		ImGui::SameLine();
		ImGui::ColorEdit4(std::string("4").append(label).c_str(), fourth_col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
		ImGui::PopStyleVar();
	}
	else if ( third ) {
		ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 5, 5 ) );
		ImGui::SetCursorPosX ( ImGui::GetWindowSize ().x - 71 );
		ImGui::ColorEdit4 ( label.c_str (), col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar );
		ImGui::SameLine ();
		ImGui::ColorEdit4 ( std::string ( "2" ).append ( label ).c_str (), sec_col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar );
		ImGui::SameLine ();
		ImGui::ColorEdit4 ( std::string ( "3" ).append ( label ).c_str (), third_col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar );
		ImGui::PopStyleVar ();
	}
	else if ( second ) {
		ImGui::PushStyleVar ( ImGuiStyleVar_ItemSpacing, ImVec2 ( 5, 5 ) );
		ImGui::SetCursorPosX ( ImGui::GetWindowSize ().x - 51 );
		ImGui::ColorEdit4 ( label.c_str (), col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar );
		ImGui::SameLine ();
		ImGui::ColorEdit4 ( std::string ( "2" ).append ( label ).c_str (), sec_col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar );
		ImGui::PopStyleVar ();
	}
	else {
		ImGui::SetCursorPosX ( ImGui::GetWindowSize ().x - 28 );
		ImGui::ColorEdit4 ( label.c_str (), col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar );
	}

	ImGui::GetStyle ().PopupRounding = 3.f;
	ImGui::PopStyleVar ();
	ImGui::PopStyleVar ();
	ImGui::PopStyleVar ();
}

// wow i forgot about this too
bool set_to_pressed_key ( int& keybind ) noexcept {
	if ( ImGui::IsKeyPressed ( ImGui::GetIO ().KeyMap[ ImGuiKey_Escape ] ) ) {
		keybind = 0;
		return true;
	}


	for ( auto i = 0; i < 5; i++ ) {
		if ( ImGui::GetIO ().MouseDown[ i ] ) {
			switch ( i ) {
				case 0:
					keybind = VK_LBUTTON;
					break;
				case 1:
					keybind = VK_RBUTTON;
					break;
				case 2:
					keybind = VK_MBUTTON;
					break;
				case 3:
					keybind = VK_XBUTTON1;
					break;
				case 4:
					keybind = VK_XBUTTON2;
					break;
			}
			return true;
		}
	}

	for ( auto i = 0; i <= 256; i++ ) {
		if ( ImGui::GetIO ().KeysDown[ i ] ) {
			keybind = i;
			return true;
		}
	}
	return false;
}
void c::keybind ( std::string label, int& keybind ) {
	const auto id = ImGui::GetID ( label.c_str () );
	ImGui::PushID ( label.c_str () );

	int key_width = ImGui::CalcTextSize ( KeyNames[ keybind ] ).x;
	ImGui::SameLine ();
	ImGui::SetCursorPosX ( ImGui::GetWindowSize ().x - 10 - ( key_width + 10 ) );
	if ( ImGui::GetActiveID () == id ) {
		ImGui::PushStyleColor ( ImGuiCol_Button, ImGui::GetColorU32 ( ImGuiCol_ButtonActive ) );
		ImGui::SetCursorPosX ( ImGui::GetWindowSize ().x - 10 - ( ImGui::CalcTextSize ( "..." ).x + 10 ) );
		ImGui::Button ( "...", ImVec2 ( 10 + ( ImGui::CalcTextSize ( "..." ).x ), 20 ), ImGuiButtonFlags_DontRenderCircle );
		ImGui::PopStyleColor ();

		ImGui::GetCurrentContext ()->ActiveIdAllowOverlap = true;
		if ( ( !ImGui::IsItemHovered () && ImGui::GetIO ().MouseClicked[ 0 ] ) || set_to_pressed_key ( keybind ) ) {
			ImGui::ClearActiveID ();
		}

	}
	else if ( ImGui::Button ( KeyNames[ keybind ], ImVec2 ( 10 + key_width, 20 ), ImGuiButtonFlags_DontRenderCircle ) ) {
		ImGui::SetActiveID ( id, ImGui::GetCurrentWindow () );
	}

	ImGui::PopID ();
}
