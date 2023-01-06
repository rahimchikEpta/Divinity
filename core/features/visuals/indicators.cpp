#include "../features.hpp"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../../xor.h"
int tick_prev = 0;
int last_speed = 0, last_jump_speed = 0, last_stamina = 0;
float last_jump_stamina = 0.f;
char buf[512];
void misc::movement::display_velo() {

	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	if (!csgo::local_player->is_alive())
		return;

	int screen_width, screen_height;
	interfaces::engine->get_screen_size(screen_width, screen_height);



	static float last_vel_update = 0, jump_speed_duration = 0;
	int velocity = round(csgo::local_player->velocity().length_2d());
	float stamina = csgo::local_player->stamina();

	static bool last_onground = false;

	bool current_onground = csgo::local_player->flags() & fl_onground;


	if (last_onground && !current_onground) {
		last_jump_speed = velocity;
		last_jump_stamina = stamina;

	}

	float s = std::round(stamina);
	float ss = std::round(last_jump_stamina);
	std::string velo_str = std::to_string(velocity);
	std::string stamina_str = std::to_string(s);


	if (!current_onground) {
		jump_speed_duration = interfaces::globals->cur_time + 1.5f;
	}
	if (settings::indicators::velo_takeoff)
		if (jump_speed_duration > interfaces::globals->cur_time) {
			velo_str += " (" + std::to_string(last_jump_speed) + ")";
		}

	if (settings::indicators::stamina_takeoff)
		if (jump_speed_duration > interfaces::globals->cur_time) {
			stamina_str += " (" + std::to_string(last_jump_stamina) + ")";
			sprintf_s(buf, settings::indicators::round_stamina ? "%d (%d)" : "%.1f (%.1f)", settings::indicators::round_stamina ? std::round((int)stamina) : stamina, settings::indicators::round_stamina ? std::round((int)last_jump_stamina) : last_jump_stamina);
		}
		else
			sprintf_s(buf, settings::indicators::round_stamina ? "%d" : "%.1f", settings::indicators::round_stamina ? std::round((int)stamina) : stamina);
	else
		sprintf_s(buf, settings::indicators::round_stamina ? "%d" : "%.1f", settings::indicators::round_stamina ? std::round((int)stamina) : stamina);




	std::vector<std::pair<std::string, color>> ind;

	float velocity_alpha = 1.f * (float(velocity) / 350.f);
	float stamina_alpha = settings::indicators::stamina_alpha_type == 0 ? 1.f : 1.f * (float(stamina) / 25.f);
	if (settings::indicators::velo) {
		if (settings::indicators::velo_color_type == 2) {
			float h, s, v;
			float r, g, b;
			float ad_h = std::lerp(0.f, 0.8f, (1.f * (float(velocity) / 350.f)));

			ImGui::ColorConvertHSVtoRGB(((float)velocity / 3.f) / 360.f, 1.f, 1.f, r, g, b);
			ImGui::PushFont(menu::indicator_font);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(velo_str.c_str()).x / 2) + 1, (screen_height / 2) + settings::indicators::position + 1), ImColor(0.f, 0.f, 0.f, 1.f), velo_str.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(velo_str.c_str()).x / 2), (screen_height / 2) + settings::indicators::position), ImColor(r, g, b), velo_str.c_str());
			ImGui::PopFont();
		}
		if (settings::indicators::velo_color_type == 0) {
			ImGui::PushFont(menu::indicator_font);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(velo_str.c_str()).x / 2) + 1, (screen_height / 2) + settings::indicators::position + 1), ImColor(0.f, 0.f, 0.f, settings::indicators::velo_alpha_type == 1 ? velocity_alpha : settings::indicators::velo_col[3]), velo_str.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(velo_str.c_str()).x / 2), (screen_height / 2) + settings::indicators::position), ImColor(settings::indicators::velo_col[0], settings::indicators::velo_col[1], settings::indicators::velo_col[2], settings::indicators::velo_alpha_type == 1 ? velocity_alpha : settings::indicators::velo_col[3]), velo_str.c_str());
			ImGui::PopFont();
		}
		if (settings::indicators::velo_color_type == 1) {
			color c = velocity == last_speed ? color(255, 199, 89, 255) : velocity < last_speed ? color(255, 119, 119, 255) : color(30, 255, 109, 255);
			ImGui::PushFont(menu::indicator_font);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(velo_str.c_str()).x / 2) + 1, (screen_height / 2) + settings::indicators::position + 1), ImColor(0.f, 0.f, 0.f, settings::indicators::velo_alpha_type == 1 ? velocity_alpha : settings::indicators::velo_col[3]), velo_str.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(velo_str.c_str()).x / 2), (screen_height / 2) + settings::indicators::position), ImColor(c.r / 255.f, c.g / 255.f, c.b / 255.f, settings::indicators::velo_alpha_type == 1 ? velocity_alpha : settings::indicators::velo_col[3]), velo_str.c_str());
			ImGui::PopFont();
		}

	}


	if (settings::indicators::stamina) {
		ImGui::PushFont(menu::indicator_font);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(buf).x / 2) + 1, (screen_height / 2) + settings::indicators::position + 28 + 1), ImColor(0.f, 0.f, 0.f, stamina_alpha), buf);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(buf).x / 2), (screen_height / 2) + settings::indicators::position + 28), ImColor(settings::indicators::stamina_col[0], settings::indicators::stamina_col[1], settings::indicators::stamina_col[2], stamina_alpha), buf);
		ImGui::PopFont();
	}

	bool can_draw_jb_ind = (settings::indicators::jb && settings::movement::jumpbug && GetAsyncKeyState(settings::movement::jumpbug_key));
	bool can_draw_eb_ind = (settings::indicators::eb && settings::movement::edgebug && GetAsyncKeyState(settings::movement::edgebug_key));
	bool can_draw_ej_ind = (settings::indicators::ej && settings::movement::edgejump && GetAsyncKeyState(settings::movement::edgejump_key));
	bool can_draw_lb_ind = (settings::indicators::lb && settings::movement::ladder_bug && GetAsyncKeyState(settings::movement::ladder_bug_key));


	if (can_draw_jb_ind || variables::jb_ind_alpha > 0.01f) 
		ind.push_back(std::pair<std::string, color>{xorstr_("jb"), color::blend(color::from_float(settings::indicators::jb_col).set_alpha(can_draw_jb_ind ? 255 : variables::jb_ind_alpha * 255),
			color::from_float(settings::indicators::jb_rage_col).set_alpha(can_draw_jb_ind ? 255 : variables::jb_ind_alpha * 255), variables::jb_ind_alpha)});

	if (can_draw_eb_ind || variables::eb_ind_alpha > 0.01f)
		ind.push_back(std::pair<std::string, color>{xorstr_("eb"), color::blend(color::from_float(settings::indicators::eb_default_col).set_alpha(can_draw_eb_ind ? 255 : variables::eb_ind_alpha * 255),
			color::from_float(settings::indicators::eb_rage_col).set_alpha(can_draw_eb_ind ? 255 : variables::eb_ind_alpha * 255), variables::eb_ind_alpha)});

	if (can_draw_ej_ind || variables::ej_ind_alpha > 0.01f)
		ind.push_back(std::pair<std::string, color>{settings::movement::longjump ? xorstr_("ej+lj") : xorstr_("ej"), color::blend(color::from_float(settings::indicators::ej_col).set_alpha(can_draw_ej_ind ? 255 : variables::ej_ind_alpha * 255),
			color::from_float(settings::indicators::ej_rage_col).set_alpha(can_draw_ej_ind ? 255 : variables::ej_ind_alpha * 255), variables::ej_ind_alpha)});

	if (can_draw_lb_ind || variables::lb_ind_alpha > 0.01f)
		ind.push_back(std::pair<std::string, color>{"lb", color::blend(color::from_float(settings::indicators::lb_default_col).set_alpha(can_draw_lb_ind ? 255 : variables::lb_ind_alpha * 255),
			color::from_float(settings::indicators::lb_rage_col).set_alpha(can_draw_lb_ind ? 255 : variables::lb_ind_alpha * 255), variables::lb_ind_alpha)});


	int pos = 0;
	for (const auto& i : ind) {
		ImGui::PushFont(menu::indicator_font);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(i.first.c_str()).x / 2) + 1, (screen_height / 2) + settings::indicators::position + (28 * 2) + pos + 1), ImColor(0, 0, 0, i.second.a), i.first.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(screen_width / 2 - (ImGui::CalcTextSize(i.first.c_str()).x / 2), (screen_height / 2) + settings::indicators::position + (28 * 2) + pos), ImColor(i.second.r, i.second.g, i.second.b, i.second.a), i.first.c_str());
		ImGui::PopFont();
		pos += 28;
	}

	if (tick_prev + 5 < interfaces::globals->tick_count) {
		last_speed = velocity;
		last_stamina = stamina;
		tick_prev = interfaces::globals->tick_count;
	}

	if (interfaces::globals->tick_count <= 320) {
		tick_prev = 0;
	}

	last_onground = current_onground;
}