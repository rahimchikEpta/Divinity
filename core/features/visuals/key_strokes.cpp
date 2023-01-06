#include "../features.hpp"
#include "../../menu/ImGui/imgui_internal.h"
#include <config.hpp>

void visuals::key_strokes() {

	if (!settings::indicators::key_strokes)
		return;

	if (!interfaces::engine)
		return;

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	float deltatime = (1.0f / 0.2f) * ImGui::GetIO().DeltaTime * 0.6f;
	float white[4] = { 1.f, 1.f, 1.f, 1.f };
	float black[4] = { 0.f, 0.f, 0.f, 1.f };
	static float w_alpha = 1.f;
	static float a_alpha = 1.f;
	static float s_alpha = 1.f;
	static float d_alpha = 1.f;
	static float space_alpha = 1.f;
	static float crouch_alpha = 1.f;


	if (settings::indicators::key_strokes_fadeout) {
		if (GetAsyncKeyState(0x57))
			w_alpha = 1.f;
		else
			w_alpha = ImClamp(w_alpha + (3.f * deltatime * (GetAsyncKeyState(0x57) ? 1.f : -1.f)), 0.0f, 1.f);

		if (GetAsyncKeyState(0x41))
			a_alpha = 1.f;
		else
			a_alpha = ImClamp(a_alpha + (3.f * deltatime * (GetAsyncKeyState(0x41) ? 1.f : -1.f)), 0.0f, 1.f);

		if (GetAsyncKeyState(0x53))
			s_alpha = 1.f;
		else
			s_alpha = ImClamp(s_alpha + (3.f * deltatime * (GetAsyncKeyState(0x53) ? 1.f : -1.f)), 0.0f, 1.f);

		if (GetAsyncKeyState(0x44))
			d_alpha = 1.f;
		else
			d_alpha = ImClamp(d_alpha + (3.f * deltatime * (GetAsyncKeyState(0x44) ? 1.f : -1.f)), 0.0f, 1.f);

		if (settings::indicators::key_strokes_stances)
			if (GetAsyncKeyState(VK_SPACE))
				space_alpha = 1.f;
			else
				space_alpha = ImClamp(space_alpha + (3.f * deltatime * (GetAsyncKeyState(VK_SPACE) ? 1.f : -1.f)), 0.0f, 1.f);
		else
			space_alpha = 0.f;

		if (settings::indicators::key_strokes_stances)
			if (GetAsyncKeyState(VK_CONTROL))
				crouch_alpha = 1.f;
			else
				crouch_alpha = ImClamp(crouch_alpha + (3.f * deltatime * (GetAsyncKeyState(VK_CONTROL) ? 1.f : -1.f)), 0.0f, 1.f);
		else
			crouch_alpha = 0.f;
	}
	else {
		if (GetAsyncKeyState(0x57))
			w_alpha = 1.f;
		else
			w_alpha = 0.f;

		if (GetAsyncKeyState(0x41))
			a_alpha = 1.f;
		else
			a_alpha = 0.f;

		if (GetAsyncKeyState(0x53))
			s_alpha = 1.f;
		else
			s_alpha = 0.f;

		if (GetAsyncKeyState(0x44))
			d_alpha = 1.f;
		else
			d_alpha = 0.f;

		if (settings::indicators::key_strokes_stances)
			if (GetAsyncKeyState(VK_SPACE))
				space_alpha = 1.f;
			else
				space_alpha = 0.f;
		else
			space_alpha = 0.f;

		if (settings::indicators::key_strokes_stances)
			if (GetAsyncKeyState(VK_CONTROL))
				crouch_alpha = 1.f;
			else
				crouch_alpha = 0.f;
		else
			crouch_alpha = 0.f;
		
	}


	int w, h;
	interfaces::engine->get_screen_size(w, h);

	render::text_shadow(vec2_t(w / 2, settings::indicators::key_strokes_position), color::from_float2(settings::indicators::key_strokes_col, w_alpha), "w", render::fonts::key_strokes, color::from_float2(black, w_alpha), true);
	render::text_shadow(vec2_t(w / 2 - 10, settings::indicators::key_strokes_position + 10), color::from_float2(settings::indicators::key_strokes_col, a_alpha), "a", render::fonts::key_strokes, color::from_float2(black, a_alpha), true);
	render::text_shadow(vec2_t(w / 2 - 10, settings::indicators::key_strokes_position), color::from_float2(settings::indicators::key_strokes_col, crouch_alpha), "c", render::fonts::key_strokes, color::from_float2(black, crouch_alpha), true);
	render::text_shadow(vec2_t(w / 2, settings::indicators::key_strokes_position + 10), color::from_float2(settings::indicators::key_strokes_col, s_alpha), "s", render::fonts::key_strokes, color::from_float2(black, s_alpha), true);
	render::text_shadow(vec2_t(w / 2 + 9, settings::indicators::key_strokes_position + 10), color::from_float2(settings::indicators::key_strokes_col, d_alpha), "d", render::fonts::key_strokes, color::from_float2(black, d_alpha), true);
	render::line(vec2_t(w / 2 - 11, settings::indicators::key_strokes_position + 10 + 13), vec2_t(w / 2 + 13, settings::indicators::key_strokes_position + 10 + 13), color::from_float2(black, space_alpha));
	render::line(vec2_t(w / 2 - 12, settings::indicators::key_strokes_position + 10 + 12), vec2_t(w / 2 + 12, settings::indicators::key_strokes_position + 10 + 12), color::from_float2(settings::indicators::key_strokes_col, space_alpha));
}