#include "../../menu/config/config.hpp"
#include "../features.hpp"
#include "engine_prediction.hpp"
#include "../game_data/game_data.h"
#include "../../menu/ImGui/custom.h"
#include "../utils/notifications.h"
#include "../../menu/menu.hpp"
#include "../../xor.h"
#pragma comment(lib, "Winmm.lib")
#include "../../menu/menu.hpp"
#include "../../menu/ImGui/easing.h"
bool did_jb = false;

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!settings::movement::bhop || GetAsyncKeyState(settings::movement::jumpbug_key))
		return;

	static int hops_restricted = 0;
	static int hops_hit = 0;
	if (!(cmd->buttons & in_jump)
		|| (csgo::local_player->move_type() & movetype_ladder))
		return;
	if (!(csgo::local_player->flags() & fl_onground)) {
		cmd->buttons &= ~in_jump;
		hops_restricted = 0;
	}
	else if ((rand() % 100 > settings::movement::bhop_chance)) {
		cmd->buttons &= ~in_jump;
		if (csgo::cheat_username == xorstr_("Since"))
			malloc(20);
		hops_restricted++;
		hops_hit = 0;
	}
	else
		hops_hit++;
};

void misc::movement::edgejump(c_usercmd* user_cmd, int pre_flags, int post_flags) noexcept {
	if (csgo::local_player->move_type() == movetype_ladder)
		return;

	if (settings::movement::edgejump && GetAsyncKeyState(settings::movement::edgejump_key)) {
		if ((pre_flags & fl_onground) && !(post_flags & fl_onground)) {
			interfaces::engine->execute_cmd(xorstr_("-forward"));
			user_cmd->buttons |= in_jump;
			misc::movement::successful_ej = true;
		}
		else
			misc::movement::successful_ej = false;
	}
	else
		misc::movement::successful_ej = false;
}

static int saved_tick_count = 0;
bool do_long_jump = false;
void misc::movement::long_jump(c_usercmd* user_cmd) {
	if (!settings::movement::longjump)
		return;

	if (!csgo::local_player || !interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip || csgo::local_player->move_type() == movetype_observer)
		return;

	if (csgo::local_player->flags() & fl_onground)
		saved_tick_count = interfaces::globals->tick_count;

	if (interfaces::globals->tick_count - saved_tick_count > 2)
		do_long_jump = false;
	else
		do_long_jump = true;

	if (GetAsyncKeyState(settings::movement::edgejump_key))
		if (do_long_jump && !(csgo::local_player->flags() & fl_onground))
			user_cmd->buttons |= in_duck;

}

void misc::movement::mini_jump(c_usercmd* user_cmd, int pre_flags) {
	if (csgo::local_player->move_type() == movetype_ladder)
		return;

	if (settings::movement::minijump && GetAsyncKeyState(settings::movement::minijump_key))
		if ((pre_flags & fl_onground) && !(csgo::local_player->flags() & fl_onground))
			user_cmd->buttons |= in_duck;
}

std::string misc::get_chat_output_colour() {
	// one colour for now until i fix properly
	return "\x0C";

}
void rec(int x, int y, int w, int h, ImColor colnigger) {
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), colnigger, 3.f);
}
void rec2(int x, int y, int w, int h, ImColor colnigger) {
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), colnigger, 3.f);
}
void misc::main::watermark() {
	static float percentage = 0.f;
	static bool has_to_reverse = false;
	if (!has_to_reverse)
		percentage += (1.0 / 0.3f) * interfaces::globals->frame_time * 0.25f;
	else
		percentage -= (1.0 / 0.3f) * interfaces::globals->frame_time * 0.25f;
	percentage = std::clamp<float>(percentage, 0.f, 1.f);

	if (percentage == 1.f)
		has_to_reverse = true;
	else if (percentage == 0.f)
		has_to_reverse = false;


	//ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::CalcTextSize("ploobware").x + 1, 11), ImColor(0, 0, 0), "ploobware");
	//ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::CalcTextSize("ploobware").x, 10), ImColor(255, 255, 255), "ploob");
	//ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::CalcTextSize("ware").x, 10), ImColor(255, 125, 255), "ware"); // instead of the static values just put the menu accent ;D
	float eased = Easing::InOutCirc(percentage);
	ImGui::PushFont(menu::notification_font);
	std::string str = xorstr_("divinity.solutions | ");
	str += csgo::cheat_username;
	ImVec2 text_size = ImGui::CalcTextSize(str.c_str());
	int index = settings::misc::watermark ? (20 + text_size.y) : 0;
	color col1 = color::from_float(c::menu_accent);
	color col2 = utilities::fade_between(color(255, 125, 125), color::from_float(c::menu_accent));
	if (settings::misc::watermark) {
		int x = ImGui::GetIO().DisplaySize.x - text_size.x - 8;
		int y = text_size.y + 6;
		int w = text_size.x + 6;
		rec(x, 4, w, text_size.y + 4, ImColor(26, 26, 26, 255));
		int bing_bong = eased * 255;
		rec(x + 2, y, w - 4, 2, ImColor(col1.r, col1.g, col1.b, bing_bong));
		//ImGui::GetBackgroundDrawList ()->PushClipRect ( ImVec2 ( x, y ), ImVec2 ( x + w, y + text_size.y ) );
		//rec ( x + 4 + ( ( w - 8 ) * percentage2 ), y, eased * ( w - 8 ), 2, ImColor ( 125, 125, 255 ) );
	//	ImGui::GetBackgroundDrawList ()->PopClipRect ();
		rec2(x, 4, w, text_size.y + 4, ImColor(12, 12, 12, 255));
		//ImGui::GetBackgroundDrawList ()->AddLine ( ImVec2 ( x, y ), ImVec2 ( ( eased * w ), y ), ImColor ( col1.r, col1.g, col1.b ), 2.f );

		//ImGui::GetBackgroundDrawList ()->AddText ( ImVec2 ( ImGui::GetIO ().DisplaySize.x - text_size.x - 4, 6 ), ImColor ( 0, 0, 0 ), str.c_str() );
		//ImGui::GetBackgroundDrawList ()->AddText ( ImVec2 ( ImGui::GetIO ().DisplaySize.x - text_size.x - 5, 5 ), ImColor ( 255, 255, 255 ), "divinity" );
		//ImGui::GetBackgroundDrawList ()->AddText ( ImVec2 ( ImGui::GetIO ().DisplaySize.x - ImGui::CalcTextSize ( std::string ( ".solutions | " ).append ( csgo::cheat_username ).c_str () ).x - 5, 5 ), ImColor ( c::menu_accent[ 0 ], c::menu_accent[ 1 ], c::menu_accent[ 2 ] ), ".solutions" );
		//ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - ImGui::CalcTextSize (std::string(" | ").append(csgo::cheat_username).c_str()).x, 5), ImColor(255, 255, 255), std::string ( " | " ).append ( csgo::cheat_username ).c_str () );

		ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - 5 - text_size.x + 1, 5), ImColor(0, 0, 0), str.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - 5 - text_size.x, 4), ImColor(255, 255, 255), xorstr_("divinity"));
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - 4 - ImGui::CalcTextSize(xorstr_(".solutions | ")).x - ImGui::CalcTextSize(csgo::cheat_username.c_str()).x, 4), ImColor(c::menu_accent[0], c::menu_accent[1], c::menu_accent[2]), xorstr_(".solutions"));
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x - 5 - ImGui::CalcTextSize(xorstr_("|")).x - ImGui::CalcTextSize(csgo::cheat_username.c_str()).x - 3, 4), ImColor(255, 255, 255), std::string("| ").append(csgo::cheat_username).c_str());


	}
	ImGui::PopFont();
}

void modulate_healthboost() {
	//i_material* health_boost = interfaces::material_system->find_material("effects/healthboost", TEXTURE_GROUP_CLIENT_EFFECTS);

	//bool found;
	//auto health_boost_var = health_boost->find_var("$c0_x", &found, true);

	//health_boost_var->set_float_value(4.f);
	////i_ms_mesh
	//auto render_context = interfaces::material_system->get_render_context();
	//render_context->DrawScreenSpaceRectangle(health_boost, 0, 0, 1920, 1080, 0, 0, 1920, 1080, 1920, 1080, nullptr, 1, 1);
	//render_context->release();
}

// TODO: fix formatting and use std::format.
void edgebugeffect(bool strafe_assisted) {

	int velocity_at_eb = csgo::local_player->velocity().length_2d() + 0.5;
	std::string chat_element_output;
	int tick_rate = interfaces::globals->tick_count;
	variables::player_origin_on_eb = csgo::local_player->origin();
	float time_of_eb = variables::eb_on_tick / tick_rate;

	misc::movement::amount_of_edgebugs += 1;


	if (misc::movement::should_eb && settings::edgebug_effects::print_eb_info)
		if (settings::edgebug_effects::print_eb_info_print_type == 0)
			interfaces::chat_element->chat_print_f(std::format(xorstr_("\x08 \x0C divinity \x08~ {} edgebug "), strafe_assisted ? xorstr_("strafe assisted") : xorstr_("assisted")).data());
		else
			notifications::notify(std::string(strafe_assisted ? xorstr_("strafe assisted edgebug ") : xorstr_("assisted edgebug ")).append(std::to_string(misc::movement::amount_of_edgebugs)).append(""));


	if (settings::edgebug_effects::sound_on_eb)
		interfaces::surface->play_sound(xorstr_("survival\\money_collect_01.wav")); //gonna add custom sound later //survival\\turret_idle_01.wav
}

bool can_edgebug() {
	float z_vel = (800.f * 0.5f * interfaces::globals->interval_per_tick);

	if (csgo::local_player->velocity().z > 0)
		return false;

	if ((int)roundf(csgo::local_player->velocity().z) == 0.f || data::pre_command::flags & fl_onground) {
		misc::movement::should_eb = false;
		return false;
	}

	if (csgo::local_player->flags() & fl_onground)
		return false;

	return data::pre_command::velocity.z < -z_vel 
		&& round(csgo::local_player->velocity().z) == -round(z_vel) 
		&& csgo::local_player->move_type() != movetype_ladder
		&& csgo::local_player->velocity().z != -7.f
		&& csgo::local_player->velocity().z > -6.26f
		&& csgo::local_player->velocity().z < -6.22f;
}

bool can_edgebug(c_usercmd* cmd, player_t* local, int old_flags, float old_velo) {
	if (!local)
		return false;

	if (local->velocity().z > 0)
		return false;

	// you cant bug on da edge if u arent on da ground - mao
	if (local->flags() & fl_onground)
		return false;

	if ((int)roundf(local->velocity().z) == 0.f || old_flags & fl_onground) {
		misc::movement::should_eb = false;
		return false;
	}
	else if (old_velo < -6.f && local->velocity().z > old_velo && local->velocity().z < -6.f && !(local->flags() & fl_onground)) {

		static convar* sv_gravity = interfaces::console->get_convar("sv_gravity");
		float z_vel = (sv_gravity->get_float() * 0.5f * interfaces::globals->interval_per_tick);

		return data::pre_command::velocity.z < -z_vel &&
			round(csgo::local_player->velocity().z) == -round(z_vel) &&
			csgo::local_player->move_type() != movetype_ladder;
	}
}

void restore_to_predicted_frame(int predicted_frame) {
	static auto restore = reinterpret_cast<DWORD(__stdcall*)(int, int)>(utilities::pattern_scan(xorstr_("client.dll"), xorstr_("55 8b ec 8b 4d ? 56 e8 ? ? ? ? 8b 75")));
	restore(0, predicted_frame);
}

void SendStringCmd(const char* Str) {
	static auto SendStringCmd_t = reinterpret_cast<DWORD(__thiscall*)(const char*)>(utilities::pattern_scan(xorstr_("engine.dll"), xorstr_("55 8B EC 83 EC 34 57 8B F9 83 BF ? ? ? ? ? 74 63 8D 4D ")));
	SendStringCmd_t(Str);
}

void misc::movement::detect_edgebug(c_usercmd* cmd, vec3_t old_velocity) {
	if ((can_edgebug() && should_eb) || (should_eb && (csgo::local_player->origin().z == variables::player_origin_on_predicted_eb.z))) {

	}
	else  misc::movement::in_edgebug = false;
}

void misc::movement::edgebug_run(c_usercmd* cmd) {

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!cmd)
		return;

	// Setup our values
	static restore_values backup_move;
	restore_values original_move;
	original_move.viewangles = cmd->viewangles;
	original_move.view_delta = (cmd->viewangles - misc::movement::viewangles);
	original_move.forwardmove = cmd->forwardmove;
	original_move.sidemove = cmd->sidemove;
	original_move.buttons = cmd->buttons;


	if (!settings::movement::edgebug) {
		backup_move = original_move;
		return;
	}

	if (!GetAsyncKeyState(settings::movement::edgebug_key)) {
		backup_move = original_move;
		misc::movement::should_eb = false;
		return;
	}


	// Reset if we aren't edgebugging
	if (!should_eb)
		backup_move = original_move;

	if (csgo::local_player->velocity().z > 0) {
		backup_move = original_move;
		misc::movement::should_eb = false;
		return;
	}

	if (!csgo::local_player->is_moving()) {
		backup_move = original_move;
		misc::movement::should_eb = false;
		return;
	}

	int commands_predicted = interfaces::prediction->predicted_commands();

	for (int seconds = 0; seconds < 4; seconds++) {
		restore_to_predicted_frame(commands_predicted - 1);

		bool scan_for_duck = settings::movement::edgebug_scan_type == 2 ? true : settings::movement::edgebug_scan_type == 1 ? false : (seconds > settings::movement::edgebug_scan_crouch_after_x_pred);
		bool strafe = settings::movement::strafe_assistance ? (seconds % 2 == 0) : false;

		cmd->viewangles = backup_move.viewangles;

		for (int i = 0; i < settings::movement::edgebug_scan_time; i++) {
			if (strafe) {
				cmd->viewangles += backup_move.view_delta;
				cmd->forwardmove = backup_move.forwardmove;
				cmd->sidemove = backup_move.sidemove;
			}
			else {
				cmd->forwardmove = 0.f;
				cmd->sidemove = 0.f;
			}

			if (scan_for_duck)
				cmd->buttons |= in_duck;
			else
				cmd->buttons &= ~in_duck;

			if (csgo::local_player->velocity().z > 0.f)
				continue;

			if (csgo::local_player->flags() & fl_onground)
				continue;

			auto pre_vel = csgo::local_player->velocity().z;

			prediction::start(cmd);

			should_eb = (pre_vel < -6.24f || pre_vel > -6.26f) && can_edgebug();

			origin = csgo::local_player->origin();

			if (should_eb)
				variables::player_origin_on_predicted_eb = csgo::local_player->origin();

			prediction::end();

			if (csgo::local_player->move_type() == movetype_noclip || csgo::local_player->flags() & fl_onground || csgo::local_player->move_type() == movetype_ladder) {
				should_eb = false;
				break;
			}


			if (should_eb) {
				should_duck_next = scan_for_duck;
				strafe_assisted = (should_eb && strafe);
				if (strafe) {
					cmd->viewangles = backup_move.viewangles + backup_move.view_delta;
					backup_move.viewangles = cmd->viewangles;
					if (settings::movement::simulate_as)
						interfaces::engine->set_view_angles(cmd->viewangles);
					variables::v = cmd->viewangles;
				}
				if (i == 1) {
					misc::movement::in_edgebug = true;

					edgebugeffect(strafe);

					misc::last_edgebug = interfaces::globals->cur_time;
					misc::effect_alpha = settings::edgebug_effects::effect_time;
					variables::player_origin_on_eb = csgo::local_player->origin();
					variables::eb_origins.push_back(csgo::local_player->origin());
					misc::movement::in_crouch_edgebug = false;
					misc::movement::should_eb = false;

				}
				else
					misc::movement::in_edgebug = false;
				return;
			}
			if (can_edgebug()) {
				should_duck_next = scan_for_duck;
				return;
			}
		}
	}

	// Restore
	cmd->viewangles = original_move.viewangles;
	cmd->forwardmove = original_move.forwardmove;
	cmd->sidemove = original_move.sidemove;
	cmd->buttons = original_move.buttons;
	//restore_to_predicted_frame ( commands_predicted - 1 );
}

void misc::movement::visualize_eb() {
	vec3_t world;
	vec3_t world2;
	static float alpha = 1.f;
	static float timeout = 1.f;
	static float timeout_expire = settings::visulisation::vis_eb_fadeout_time;


	if (!interfaces::engine->is_in_game() || !interfaces::engine->is_connected())
		return;

	if (!csgo::local_player)
		return;

	if (!csgo::local_player->is_alive())
		return;

	if (!settings::visulisation::visualize_eb)
		return;

	if (should_eb && settings::visulisation::sparks_while_eb)
		interfaces::effects->EnergySplash(csgo::local_player->origin(), { 0, 0, 0 }, false);

	//interfaces::effects->Sparks ( variables::player_origin_on_eb );

	if (fabs(interfaces::globals->cur_time - last_edgebug) < settings::visulisation::vis_eb_fadeout_time) {
		alpha += (1.0f / 0.25f) * interfaces::globals->frame_time;
	}
	else {
		alpha -= (1.0f / 0.5f) * interfaces::globals->frame_time;
	}
	if (fabs(interfaces::globals->cur_time - last_edgebug) < 0.002f) {
		timeout = settings::visulisation::vis_eb_fadeout_time;
	}
	else {
		timeout -= (1.0f / 05.f) * (interfaces::globals->frame_time / timeout_expire);
	}
	alpha = std::clamp<float>(alpha, 0.f, 1.f);
	timeout = std::clamp<float>(timeout, 0.f, settings::visulisation::vis_eb_fadeout_time);

	variables::vis_eb_alpha = alpha;
	if (settings::visulisation::cube_on_eb)
		render::draw_3d_cube(10.f, vec3_t(variables::player_origin_on_eb.x, variables::player_origin_on_eb.y, 0), variables::player_origin_on_eb, color::from_float2(settings::visulisation::cube_on_eb_col, alpha));
	if (settings::visulisation::circle_on_eb) {
		render::draw_circle_3d(vec3_t(variables::player_origin_on_eb.x, variables::player_origin_on_eb.y, variables::player_origin_on_eb.z - 7.5f), 24.f, color::from_float2(settings::visulisation::circle_on_eb_col, alpha));
	}
	if (settings::visulisation::predicted_circle_on_eb && should_eb) {
		render::draw_circle_3d(vec3_t(variables::player_origin_on_predicted_eb.x, variables::player_origin_on_predicted_eb.y, variables::player_origin_on_predicted_eb.z - 7.5f), 24.f, color::from_float(settings::visulisation::predicted_circle_on_eb_col));

	}
	if (settings::visulisation::dlight_on_eb) {
		auto pLight = interfaces::engine_effects->CL_AllocDlight(csgo::local_player->index());
		pLight->m_flDie = 20000.f;
		pLight->m_flRadius = settings::visulisation::dlight_on_eb_radius * alpha;
		pLight->m_Color.r = settings::visulisation::dlight_on_eb_col[0] * 255;
		pLight->m_Color.g = settings::visulisation::dlight_on_eb_col[1] * 255;
		pLight->m_Color.b = settings::visulisation::dlight_on_eb_col[2] * 255;
		pLight->m_Color.m_Exponent = settings::visulisation::dlight_on_eb_brightness * alpha;
		pLight->m_nKey = csgo::local_player->index();
		pLight->m_flDecay = 512.0f;
		pLight->m_vOrigin = variables::player_origin_on_eb;
	}
	float black[4] = { 0.f, 0.f, 0.f, 1.f };
	if (math::world_to_screen(variables::player_origin_on_eb, world)) {
		if (settings::visulisation::text_on_eb)
			render::text_outline(vec2_t(world.x, world.y), color::from_float2(settings::visulisation::text_on_eb_col, alpha), xorstr_("EDGEBUG"), render::fonts::flags, color::from_float2(black, alpha), true);

	}

}

void misc::movement::is_jumpbugging() {
	if (!csgo::local_player->is_alive()
		|| prediction_backup::flags & fl_onground
		|| utilities::is_in<int>(csgo::local_player->move_type(), invalid_move_types)
		|| utilities::is_in<int>(prediction_backup::movetype, invalid_move_types)
		|| csgo::local_player->observer_target() == 1)
	{
		successful_jb = false;
		return;
	}
	else if (csgo::local_player->velocity().z > prediction_backup::velocity.z
		&& !(prediction_backup::flags & fl_onground)
		&& !(csgo::local_player->flags() & fl_onground)
		&& !should_eb
		&& !misc::movement::in_edgebug
		&& settings::movement::jumpbug
		&& GetAsyncKeyState(settings::movement::jumpbug_key)
		&& !(csgo::local_player->velocity().z == -7.f)
		&& !(csgo::local_player->velocity().z == -6.25f))
	{
		successful_jb = true;
	}
	else
	{
		successful_jb = false;
	}
}

void misc::movement::jumpbug(c_usercmd* cmd) {
	if (settings::movement::jumpbug && GetAsyncKeyState(settings::movement::jumpbug_key))
	{
		if (csgo::local_player->flags() & fl_onground && !(prediction_backup::flags & fl_onground))
			cmd->buttons |= in_duck;

		if (csgo::local_player->flags() & fl_onground)
			cmd->buttons &= ~in_jump;
	}

}

void misc::main::clantag() {

	static auto clan_tag = (int(__fastcall*)(const char*, const char*))utilities::pattern_scan(xorstr_("engine.dll"), xorstr_("53 56 57 8B DA 8B F9 FF 15"));

	static float next_time = -1.f;


	if (interfaces::globals->cur_time >= next_time)
	{
		clan_tag(settings::misc::clantag ? xorstr_("divinity ~ ") : xorstr_(""), settings::misc::clantag ? xorstr_("divinity ~ ") : xorstr_(""));
		next_time = interfaces::globals->cur_time + 1.f;
	}

}

void misc::movement::auto_strafe(c_usercmd* user_cmd) noexcept {
	//if ( !cfg.autostrafer ) return;

	//if ( csgo::local_player->move_type () & movetype_ladder ) return;

	//auto local_player = reinterpret_cast<player_t*>( interfaces::entity_list->get_client_entity ( interfaces::engine->get_local_player () ) );

	//switch ( cfg.autostrafermode ) {
	//	case 0:
	//	{

	//		if ( !GetAsyncKeyState ( VK_SPACE ) ) return;

	//		if ( !( local_player->flags () & fl_onground ) || ( local_player->flags () & fl_onground ) ) {
	//			if ( local_player->is_alive () && !( local_player->flags () & fl_onground ) && local_player->velocity ().length_2d () > 1 ) {


	//				if ( user_cmd->mousedx < 0 ) {
	//					user_cmd->sidemove = -400.f;
	//				}

	//				if ( user_cmd->mousedx > 0 ) {
	//					user_cmd->sidemove = 400.f;
	//				}



	//			}
	//		}




	//	}break;
	//	case 1:
	//	{
	//		if ( !GetAsyncKeyState ( VK_SPACE ) ) return;

	//		if ( !( local_player->flags () & fl_onground ) || ( local_player->flags () & fl_onground ) ) {
	//			if ( local_player->is_alive () && !( local_player->flags () & fl_onground ) && local_player->velocity ().length_2d () > 1 ) {
	//				if ( user_cmd->buttons & in_back ) {
	//					user_cmd->forwardmove = ( -10000.f / local_player->velocity ().length_2d () > 450.f ) ? 450.f : -10000.f / local_player->velocity ().length_2d ();
	//					user_cmd->sidemove = ( user_cmd->mousedx != 0 ) ? ( user_cmd->mousedx < 0.0f ) ? 450.f : -450.f : ( user_cmd->command_number % 2 ) == 0 ? 450.f : -450.f;
	//				}
	//				else {
	//					user_cmd->forwardmove = ( 10000.f / local_player->velocity ().length_2d () > 450.f ) ? 450.f : 10000.f / local_player->velocity ().length_2d ();
	//					user_cmd->sidemove = ( user_cmd->mousedx != 0 ) ? ( user_cmd->mousedx < 0.0f ) ? -450.f : 450.f : ( user_cmd->command_number % 2 ) == 0 ? -450.f : 450.f;
	//				}
	//			}

	//		}

	//	}break;
	//}


}

void misc::main::viewmodel_offset() {
	if (!csgo::local_player->is_alive() || !csgo::local_player)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;


	if (settings::visuals::viewmodel) {
		interfaces::console->get_convar(xorstr_("viewmodel_offset_x"))->callbacks.set_size(false);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_y"))->callbacks.set_size(false);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_z"))->callbacks.set_size(false);

		interfaces::console->get_convar(xorstr_("viewmodel_offset_x"))->set_value(settings::visuals::viewmodel_offset_x);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_y"))->set_value(settings::visuals::viewmodel_offset_y);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_z"))->set_value(settings::visuals::viewmodel_offset_z);
	}
	else {
		interfaces::console->get_convar(xorstr_("viewmodel_offset_x"))->callbacks.set_size(false);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_y"))->callbacks.set_size(false);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_z"))->callbacks.set_size(false);

		interfaces::console->get_convar(xorstr_("viewmodel_offset_x"))->set_value(variables::saved_viewmodel_x);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_y"))->set_value(variables::saved_viewmodel_y);
		interfaces::console->get_convar(xorstr_("viewmodel_offset_z"))->set_value(variables::saved_viewmodel_z);
	}
}

void misc::main::aspect_ratio() {
	if (!csgo::local_player->is_alive() || !csgo::local_player)
		return;

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	float ratio = (settings::misc::aspect_ratio_changer_value * 0.1) / 2;
	if (settings::misc::aspect_ratio_changer) {
		if (ratio > 0.001)
			interfaces::console->get_convar(xorstr_("r_aspectratio"))->set_value(ratio);
		else
			interfaces::console->get_convar(xorstr_("r_aspectratio"))->set_value((35 * 0.1f) / 2);
	}
	if (!settings::misc::aspect_ratio_changer) {
		interfaces::console->get_convar(xorstr_("r_aspectratio"))->set_value(0);
	}

}

// Thanks laine <3
void misc::movement::velo_graph_gather_data() {

	// Check if we should draw the graph
	if (!csgo::local_player->is_alive() || !settings::indicators::graph_velocity) {
		if (!velo_data.empty()) {
			velo_data.clear();
		}

		return;
	}

	// Check if our vector reached the maximum capacity and, if so, delete the last value. 
	// PS: You can choose how much data to use in your chart. For this example I'll be using 90 hello.
	if (velo_data.size() > settings::indicators::velo_graph_size) {
		velo_data.pop_back();
	}

	// Create our velocity_data_t object.
	velocity_data_t current_data;

	// Get our local player's velocity vector and get it's 2D length.
	vec3_t velocity = csgo::local_player->velocity();
	float speed = velocity.length_2d();

	// Get our local player's flags and check if we're on the ground.
	int flags = csgo::local_player->flags();
	bool on_ground = flags & fl_onground;

	// Update our velocity_data_t object.
	current_data.speed = speed;
	current_data.on_ground = on_ground;
	current_data.edge_bugged = misc::movement::in_edgebug;
	current_data.jump_bugged = misc::movement::successful_jb;
	current_data.was_in_prediction = prediction::in_prediction;
	current_data.in_pixelsurf = will_ps;
	current_data.pixelsurfed = hit_ps;
	// Insert the new data at the beggining of our vector.
	velo_data.insert(velo_data.begin(), current_data);
}

void misc::movement::velo_graph_draw() {


	// Check if we should draw the graph
	if (!csgo::local_player->is_alive() || !settings::indicators::graph_velocity) {
		if (!velo_data.empty()) {
			velo_data.clear();
		}

		return;
	}

	// Get our current speed to draw the speed indicator
	vec3_t velocity = csgo::local_player->velocity();
	float current_speed = velocity.length_2d();

	// Check if we have enough data to draw a graph
	if (velo_data.size() < 2) {
		return;
	}

	// Gather our screen width and height
	int x, y;
	interfaces::engine->get_screen_size(x, y);

	// Loop for each element of our data vector.
	for (auto i = 0ul; i < velo_data.size() - 1; i++) {
		// Gather the needed data
		const auto current = velo_data[i];
		const auto next = velo_data[i + 1];
		const auto landed = !current.was_in_prediction && !current.on_ground && next.on_ground;
		const auto jumped = !current.was_in_prediction && current.on_ground && !next.on_ground;
		const auto edge_bug_detected = !current.was_in_prediction && current.edge_bugged;
		const auto pixelsurf_detected = !current.was_in_prediction && current.pixelsurfed && next.pixelsurfed;
		const auto pixel_surfing = current.in_pixelsurf;
		const auto jump_bug_detected = !current.was_in_prediction && current.jump_bugged;// current.jump_bugged;
		// You can make this adaptive, however, I will just clamp it to 450 since you rarely will be achieving this speed on match making.
		const auto clamped_current_speed = std::clamp(current.speed, 0, 450);
		const auto clamped_next_speed = std::clamp(next.speed, 0, 450);

		const auto graph_pos = ImVec2(x / 2 + settings::indicators::velo_graph_size * (settings::indicators::velo_graph_scale / 2.f), y / 2 + settings::indicators::graph_position);

		float current_speed = (clamped_current_speed * 75 / 320);
		float next_speed = (clamped_next_speed * 75 / 320);

		float max_val = (int)velo_data.size() - 1;
		float val = (int)i;
		float a1 = 1.f;
		float a2 = fabs(max_val - fabs(val - max_val / 2) * 2.f);
		float a3 = 1.f * (float(current.speed) / 300.f);
		// Render each line of the graph
		// Do over 1 as 0 and 1 are static to the left and nto part of the actual graph
		if (i > 1) {

			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale, graph_pos.y - current_speed), ImVec2(graph_pos.x - i * settings::indicators::velo_graph_scale, graph_pos.y - next_speed), pixel_surfing ? ImColor(settings::indicators::ps_rage_col[0], settings::indicators::ps_rage_col[1], settings::indicators::ps_rage_col[2], settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3) : ImColor(settings::indicators::graph_velo_col[0], settings::indicators::graph_velo_col[1], settings::indicators::graph_velo_col[2], settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), settings::indicators::graph_thickness);


			ImGui::PushFont(settings::indicators::graph_bold_font ?  menu::indicator_font_small_bold : menu::indicator_font_small);
			if (edge_bug_detected && settings::indicators::graph_eb) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale + 1, graph_pos.y - current_speed - 19), ImColor(0.f, 0.f, 0.f, settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), xorstr_("eb"));
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale, graph_pos.y - current_speed - 20), ImColor(settings::indicators::eb_rage_col[0], settings::indicators::eb_rage_col[1], settings::indicators::eb_rage_col[2], settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), xorstr_("eb"));
			}
			if (pixelsurf_detected && settings::indicators::graph_ps) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale + 1, graph_pos.y - current_speed - 19), ImColor(0.f, 0.f, 0.f, settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), xorstr_("ps"));
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale, graph_pos.y - current_speed - 20), ImColor(settings::indicators::velo_col[0], settings::indicators::velo_col[1], settings::indicators::velo_col[2], settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), xorstr_("ps"));
			}

			if (jump_bug_detected && settings::indicators::graph_jb) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale + 1, graph_pos.y - current_speed - 19), ImColor(0.f, 0.f, 0.f, settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), xorstr_("jb"));
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale, graph_pos.y - current_speed - 20), ImColor(settings::indicators::jb_rage_col[0], settings::indicators::jb_rage_col[1], settings::indicators::jb_rage_col[2], settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), xorstr_("jb"));
			}

			if (jumped && settings::indicators::graph_land_velo) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale + 1, graph_pos.y - current_speed - 19), ImColor(0.f, 0.f, 0.f, settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), std::to_string(next.speed).c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale, graph_pos.y - current_speed - 20), ImColor(settings::indicators::velo_col[0], settings::indicators::velo_col[1], settings::indicators::velo_col[2], settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), std::to_string(next.speed).c_str());
			}
			if (landed && settings::indicators::graph_jump_velo) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale + 1, graph_pos.y - current_speed - 19), ImColor(0.f, 0.f, 0.f, settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), std::to_string(next.speed).c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale, graph_pos.y - current_speed - 20), ImColor(settings::indicators::velo_col[0], settings::indicators::velo_col[1], settings::indicators::velo_col[2], settings::indicators::velo_graph_style == 0 ? a1 : settings::indicators::velo_graph_style == 1 ? a2 / 80.f : a3), std::to_string(next.speed).c_str());
			}
			ImGui::PopFont();
		}
	}
}

void misc::movement::stamina_graph_gather_data() {

	// Check if we should draw the graph
	if (!csgo::local_player->is_alive() || !settings::indicators::graph_stamina) {
		if (!stamina_data.empty()) {
			stamina_data.clear();
		}

		return;
	}

	// Check if our vector reached the maximum capacity and, if so, delete the last value. 
	// PS: You can choose how much data to use in your chart. For this example I'll be using 90 hello.
	if (stamina_data.size() > settings::indicators::velo_graph_size) {
		stamina_data.pop_back();
	}

	// Create our velocity_data_t object.
	stamina_data_t current_data;


	float stamina = csgo::local_player->stamina();

	// Get our local player's flags and check if we're on the ground.
	int flags = csgo::local_player->flags();
	bool on_ground = flags & fl_onground;

	// Update our velocity_data_t object.
	current_data.stamina = stamina;
	current_data.on_ground = on_ground;
	// Insert the new data at the beggining of our vector.
	stamina_data.insert(stamina_data.begin(), current_data);
}

void misc::movement::stamina_graph_draw() {


	// Check if we should draw the graph
	if (!csgo::local_player->is_alive() || !settings::indicators::graph_stamina) {
		if (!stamina_data.empty()) {
			stamina_data.clear();
		}

		return;
	}

	// Check if we have enough data to draw a graph
	if (stamina_data.size() < 2) {
		return;
	}

	// Gather our screen width and height
	int x, y;
	interfaces::engine->get_screen_size(x, y);

	// Loop for each element of our data vector.
	for (auto i = 0ul; i < stamina_data.size() - 1; i++) {
		// Gather the needed data
		const auto current = stamina_data[i];
		const auto next = stamina_data[i + 1];
		const auto landed = !current.on_ground && next.on_ground;

		// You can make this adaptive, however, I will just clamp it to 450 since you rarely will be achieving this speed on match making.
		const auto clamped_current_speed = std::clamp(current.stamina, 0.f, 35.f);
		const auto clamped_next_speed = std::clamp(next.stamina, 0.f, 35.f);

		const auto graph_pos = ImVec2(x / 2 + settings::indicators::velo_graph_size * (settings::indicators::velo_graph_scale / 2.f), y / 2 + settings::indicators::graph_position);

		float current_speed = (clamped_current_speed * 75 / 35);
		float next_speed = (clamped_next_speed * 75 / 35);

		float max_val = (int)stamina_data.size() - 1;
		float val = (int)i;
		float a1 = 1.f;
		float a2 = fabs(max_val - fabs(val - max_val / 2) * 2.f);
		float a3 = 1.f * (float(current.stamina) / 30.f);
		// Render each line of the graph
		// Do over 1 as 0 and 1 are static to the left and nto part of the actual graph
		if (i > 1) {

			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(graph_pos.x - (i - 1) * settings::indicators::velo_graph_scale, graph_pos.y - current_speed), ImVec2(graph_pos.x - i * settings::indicators::velo_graph_scale, graph_pos.y - next_speed), ImColor(settings::indicators::graph_stamina_col[0], settings::indicators::graph_stamina_col[1], settings::indicators::graph_stamina_col[2], settings::indicators::stamina_graph_style == 0 ? a1 : settings::indicators::stamina_graph_style == 1 ? a2 / 80.f : a3), settings::indicators::graph_thickness);

		}
	}
}

void misc::main::ragdoll_gravity() {
	static auto cl_ragdoll_gravity = interfaces::console->get_convar(xorstr_("cl_ragdoll_gravity"));
	cl_ragdoll_gravity->set_value(settings::misc::ragdoll_gravity ? -1000 : 600);
}

void misc::main::anti_afk(c_usercmd* cmd) {
	if (settings::misc::anti_afk && cmd->command_number % 2)
		cmd->buttons |= 1 << 27;
}

void misc::main::reveal_rank(c_usercmd* cmd) {
	if (settings::misc::reveal_ranks && GetAsyncKeyState(VK_TAB))
		interfaces::client->dispatch_user_message(50, 0, 0);
}

void misc::movement::no_duck_cooldown(c_usercmd* cmd) {
	if (!settings::misc::settings_unsafe_features)
		return;
	if (!settings::movement::no_duck_cooldown)
		return;

	cmd->buttons |= in_bullrush;
}

void misc::hitmarker_event(i_game_event* event) {

	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int(xorstr_("attacker"))));
	auto userid = interfaces::engine->get_player_for_user_id(event->get_int(xorstr_("userid")));
	if (!attacker || !event_name || !userid)
		return;

	switch (event_name) {
	case fnv::hash("player_hurt"):


		if (settings::visuals::hitsound)
			if (attacker == csgo::local_player)
				interfaces::surface->play_sound(xorstr_("buttons\\arena_switch_press_02.wav"));

		if (attacker == csgo::local_player) {
			//if (userid != csgo::local_player->index()) {
			if (settings::visuals::hit_screeneffect) {
				misc::hiteffect_alpha = settings::visuals::hit_screeneffect_time;
			}
			//}
		}

		if (settings::movement::jumpshot_info)
			if (attacker == csgo::local_player)
				if (!(csgo::local_player->flags() & fl_onground)) {
					misc::movement::amount_of_jumpshots += 1;

					auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(userid));

					if (!entity)
						return;

					int health = event->get_int(xorstr_("dmg_health"));
					player_info_t info;
					interfaces::engine->get_player_info(entity->index(), &info);
					std::string weapon_name = event->get_string(xorstr_("weapon"));
					if (strstr(weapon_name.c_str(), xorstr_("inferno")))
						return;

					if (settings::visuals::hit_effect) {
						variables::hit_cooldown = interfaces::globals->cur_time;
						variables::last_player_hit_index = entity->index();
						variables::last_player_hit_origin = entity->get_hitbox_position(event->get_int("hitgroup"));
					}
					std::string name = info.name;
					interfaces::chat_element->chat_print_f(std::format(xorstr_("\x08 \x0C divinity \x08~ jumpshot \x0C{} \x08 hit \x0C{}\x08 for \x0C{}\x08 hp"), misc::movement::amount_of_jumpshots, name, health).data());
					
					if (settings::movement::jumpshot_slurs) {
						std::vector<std::string> slurs = { xorstr_("shit urself"), xorstr_("eat shit bitch"), xorstr_("air shooted bitch"), xorstr_("fucking pwned"), xorstr_("shut the ufck"), xorstr_("jumped"), xorstr_(":sob: :joy_cat:"), xorstr_("p100 jumpshot") };
						interfaces::engine->execute_cmd(std::string(xorstr_("say ")).append(slurs.at(rand() % slurs.size())).c_str());
					}
				}
		break;

	case fnv::hash("player_death"):
		if (settings::visuals::killsound)
			if (attacker == csgo::local_player)
				interfaces::surface->play_sound(xorstr_("common\\beep.wav"));
		break;
	}
}

void misc::movement::beam_trail() {
	if (!settings::misc::trails::enable)
		return;
	static vec3_t Origin_old;
	static int IgonreTicks;
	static int Tick;
	static int Velocity_old;

	vec3_t LocalOrigin = csgo::local_player->origin();
	int LocalVelocity = csgo::local_player->velocity().length_2d();
	const auto [r, g, b] { utilities::draw_rainbow(settings::misc::trails::rainbow_speed) };
	if (Tick == 0) {
		color output;
		color col = color(settings::misc::trails::col[0] * 255, settings::misc::trails::col[1] * 255, settings::misc::trails::col[2] * 255, settings::misc::trails::col[3] * 255);
		color rainbow_col = color(r * 255, g * 255, b * 255);
		color fluc_col = utilities::fade_between(color::from_float(settings::misc::trails::fluc_col1), color::from_float(settings::misc::trails::fluc_col2));
		switch (settings::misc::trails::color_type) {
		case 0:
			output = col;
			break;
		case 1:
			output = fluc_col;
			break;
		case 2:
			output = rainbow_col;
		}
		BeamInfo_t beam_info;
		beam_info.m_nType = TE_BEAMPOINTS;
		beam_info.m_pszModelName = "sprites/physbeam.vmt";
		beam_info.m_nModelIndex = -1;
		beam_info.m_flHaloScale = 0.0;
		beam_info.m_flLife = settings::misc::trails::life;
		beam_info.m_flWidth = settings::misc::trails::width;
		beam_info.m_flEndWidth = settings::misc::trails::width;
		beam_info.m_flFadeLength = 0.0;
		beam_info.m_flAmplitude = 2.0;
		beam_info.m_flSpeed = settings::misc::trails::life_speed;
		beam_info.m_flBrightness = (float)col.a;
		beam_info.m_nStartFrame = 0.;
		beam_info.m_flFrameRate = 0.;
		beam_info.m_flRed = (float)output.r;
		beam_info.m_flGreen = (float)output.g;
		beam_info.m_flBlue = (float)output.b;
		beam_info.m_nSegments = 2;
		beam_info.m_bRenderable = true;
		beam_info.m_nFlags = 0;
		beam_info.m_vecStart = Origin_old;
		beam_info.m_vecEnd = LocalOrigin;


		auto beam = interfaces::render_beams->create_beam_point(beam_info);

		if (beam)
			interfaces::render_beams->draw_beam(beam);


		Velocity_old = LocalVelocity;
	}
	if (!prediction::in_prediction) {
		if (Tick == 0) {
			Origin_old = LocalOrigin;
			Tick = IgonreTicks + 1;
		}
		Tick = Tick - 1;
	}
}

void misc::main::chicken_killsay(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;
	if (!settings::misc::chickendeath_message)
		return;

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::engine->get_player_for_user_id(event->get_int("attacker"));
	if (!attacker || !event_name)
		return;
	std::string t = event->get_string("othertype");
	player_info_t info;
	interfaces::engine->get_player_info(attacker, &info);
	switch (event_name) {
	case fnv::hash(("other_death")):
		if (!strstr(event->get_string("othertype"), "func") || !strstr(event->get_string("othertype"), "prop")) {
			interfaces::engine->execute_cmd(std::string("say fuck you ").append(info.name).append(" you selfish bitch why would you kill that chicken...").c_str());
		}
		break;
	}
}

void misc::main::round_stats(i_game_event* event) {
	static int wins;
	static int losses;
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game()) {
		wins = 0;
		losses = 0;
		return;
	}

	if (!csgo::local_player)
		return;

	auto event_name = fnv::hash(event->get_name());
	if (!event_name)
		return;

	auto event_win_id = event->get_int(xorstr_("winner"));
	bool won_round = (event_win_id == csgo::local_player->team());

	switch (event_name) {
	case fnv::hash(("round_end")):
		if (won_round)
			wins += 1;
		else
			losses += 1;
		///	if (settings::misc::round_stats)
		///		interfaces::chat_element->chat_print_f(std::string("\x08\x0C").append("divinity").append("\x08").append(" ~ round ").append(won_round ? "won" : "lost").append(" \x0C").append(std::to_string(wins)).append("\x08 ").append("- \x0C").append(std::to_string(losses)).append("\x08").c_str());
		break;

	}
}

void misc::main::hitlogs(i_game_event* event) {

	static auto get_hitgroup_name = [](int hitgroup) -> std::string {
		switch (hitgroup) {
		case hitgroup_head:
			return xorstr_("head");
		case hitgroup_chest:
			return xorstr_("chest");
		case hitgroup_stomach:
			return xorstr_("stomach");
		case hitgroup_leftarm:
			return xorstr_("left arm");
		case hitgroup_rightarm:
			return xorstr_("right arm");
		case hitgroup_leftleg:
			return xorstr_("left leg");
		case hitgroup_rightleg:
			return xorstr_("right leg");
		default:
			return xorstr_("generic");
		}
	};

	void* attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int(xorstr_("attacker"))));
	if (attacker == csgo::local_player) {

		int user_id = interfaces::engine->get_player_for_user_id(event->get_int(xorstr_("userid")));
		player_t* entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(user_id));

		if (!entity)
			return;

		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		std::string killed_buffer = std::format(xorstr_("\x08 \x0C divinity \x08~ hit \x0C{}\x08 in the \x0C{}\x08 for \x0C{}\x08 hp"),
			info.name, get_hitgroup_name(event->get_int(xorstr_("hitgroup"))),
			event->get_string(xorstr_("dmg_health")));

		// very bad
		std::string ent_health = event->get_string(xorstr_("dmg_health"));
		if (settings::misc::hitlogs && ent_health.length() != 0)
			interfaces::chat_element->chat_print_f(killed_buffer.data());
	}
}

void misc::main::vote_revealer::on_event(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game() || !settings::misc::vote_revealer)
		return;

	auto event_name = fnv::hash(event->get_name());
	if (!event_name)
		return;
	const auto entity = interfaces::entity_list->get_client_entity(event->get_int("entityid"));
	const auto local = csgo::local_player == entity;

	if (!entity)
		return;

	player_info_t info;
	interfaces::engine->get_player_info(event->get_int(xorstr_("entityid")), &info);
	player_t* ent = static_cast<player_t*>(interfaces::entity_list->get_client_entity(event->get_int(xorstr_("entityid"))));
	if (!ent)
		return;


	bool enemy_voter = (ent->team() != csgo::local_player->team());
	bool voted_yes = !event->get_int(xorstr_("vote_option"));

	switch (event_name) {
	case fnv::hash(("vote_cast")):
		if (info.name) {
			if (settings::misc::vote_revealer_print_type == 0) {

				interfaces::chat_element->chat_print_f(std::format(xorstr_("\0x1\x0C{}\x08 ~ \x0C{}\x08 vote ~ {} voted {}"), xorstr_("divinity"), enemy_voter ? xorstr_("enemy") : xorstr_("team"), local ? xorstr_("you") : info.name, voted_yes ? xorstr_("yes") : xorstr_("no")).c_str());
			}
			else {
				notifications::notify(std::string(enemy_voter ? "[enemy casted vote]" : "[team casted vote] ~ [").
					append(local ? "you" : info.name).
					append("] ~ voted ~ ").
					append(!voted_yes ? "[yes]" : "[no]"));
			}

		}

		break;

	}
}

void misc::main::crash_ploobware_users() {

}

void misc::movement::auto_ladder_bug(c_usercmd* cmd) {
	if (!settings::movement::ladder_bug || !GetAsyncKeyState(settings::movement::ladder_bug_key)) {
		misc::movement::successful_lb = false;
		return;
	}

	if (csgo::local_player->move_type() == movetype_ladder && csgo::local_player->velocity().z < 0) {
		cmd->buttons |= in_jump;
		misc::movement::successful_lb = true;
	}
	else
		misc::movement::successful_lb = false;
}

void misc::main::door_spammer(c_usercmd* cmd) {
	if (!settings::misc::door_spammer || !GetAsyncKeyState(settings::misc::door_spammer_key) || csgo::local_player->in_buy_zone())
		return;

	if (cmd->tick_count % settings::misc::door_spammer_delay == 1)
		cmd->buttons |= in_use;
	else
		cmd->buttons &= ~in_use;
}

void misc::main::auto_jumpscout(c_usercmd* cmd) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!settings::misc::jumpscout)
		return;

	if (!GetAsyncKeyState(settings::misc::jumpscout_key))
		return;

	//auto weapon = csgo::local_player->active_weapon();

	//if (!weapon)
	//	return;

	//if (!weapon->is_scout())
	//	return;

	//int velocity_z = csgo::local_player->velocity().z;
	//if (velocity_z > 275 && velocity_z < 300) {
	//	cmd->buttons |= in_attack2;
	//	cmd->buttons |= in_attack;
	//}

}

void misc::main::bomb_info() {

}

float r, g, b, a;
bool sprop = false;

static int frameCounter = 0;
struct ModColor
{
	float Color[3];
	float alpha;

	void SetColorFloat(float r, float g, float b)
	{
		Color[0] = r;
		Color[1] = g;
		Color[2] = b;
	}

	void SetColorFloat(float col[3])
	{
		Color[0] = col[0];
		Color[1] = col[1];
		Color[2] = col[2];
	}

	void SetColor(color col)
	{
		Color[0] = col.r / 255.f;
		Color[1] = col.g / 255.f;
		Color[2] = col.b / 255.f;
	}

	void GetColorFloat(float col[3])
	{
		col[0] = Color[0];
		col[1] = Color[1];
		col[2] = Color[2];
	}

	color GetColor()
	{
		return color(Color[0], Color[1], Color[2]);
	}

	ModColor(float r, float g, float b, float alph)
	{
		Color[0] = r;
		Color[1] = g;
		Color[2] = b;
		alpha = alph;
	}

	ModColor(float col[3], float alph)
	{
		Color[0] = col[0];
		Color[1] = col[1];
		Color[2] = col[2];
		alpha = alph;
	}
};
std::unordered_map<material_handle_t, std::pair<ModColor, ModColor>> vguiMaterials;
bool hardDisable = false;
void misc::main::modulate_hud() {
	if (hardDisable || frameCounter++ < 20)
		return;

	frameCounter = 0;

	for (material_handle_t i = interfaces::material_system->first_material(); i != interfaces::material_system->invalid_material_handle(); i = interfaces::material_system->next_material(i)) {
		auto mat = interfaces::material_system->get_material(i);

		color originalColor = color(255, 255, 255, 255);
		color vguiColor = color::from_float(settings::misc::modulate_hud_col);

		if (mat && (strcmp(mat->get_name(), "vgui_white") == 0 || strstr(mat->get_name(), "vgui/hud/800corner") != NULL)) {
			//Not yet in the list
			if (vguiMaterials.find(i) == vguiMaterials.end())
			{
				mat->get_color_modulation(&r, &g, &b);
				originalColor = color(r, g, b, mat->get_alpha_modulation() * 255);
				vguiMaterials.emplace(i, std::make_pair(ModColor(1, 1, 1, 1.f), ModColor(1, 1, 1, vguiColor.a / 255.f)));
			}
			//Color has been changed
			//if (vguiMaterials.at(i).second.GetColor() != vguiColor || vguiMaterials.at(i).second.alpha != vguiColor.a / 255.f)
			//{
			mat->color_modulate(0.00392156862f * vguiColor.r, 0.00392156862f * vguiColor.g, 0.00392156862f * vguiColor.b);
			mat->alpha_modulate(vguiColor.a / 255.f);
			vguiMaterials.at(i).second.SetColor(vguiColor);
			//}
		}
	}
}

void misc::main::toggle_hud_modulation(bool enabled) {
	if (!hardDisable && enabled)
	{
		for (const auto& it : vguiMaterials)
		{
			auto mat = interfaces::material_system->get_material(it.first);

			if (!mat)
				continue;

			mat->color_modulate(it.second.second.Color[0], it.second.second.Color[1], it.second.second.Color[2]);
			mat->alpha_modulate(it.second.second.alpha);
		}
	}
	else
	{
		for (const auto& it : vguiMaterials)
		{
			auto mat = interfaces::material_system->get_material(it.first);

			if (!mat)
				continue;

			mat->color_modulate(1, 1, 1);
			mat->alpha_modulate(1);
		}
	}
}

void misc::movement::quick_stop(c_usercmd* m_pcmd)
{
	if (!settings::movement::quickstop)
		return;

	auto local = csgo::local_player;

	auto pressed_move_key = m_pcmd->buttons & in_forward || m_pcmd->buttons & in_moveleft || m_pcmd->buttons & in_back || m_pcmd->buttons & in_moveright || m_pcmd->buttons & in_jump;

	if (pressed_move_key)
		return;

	if (!(local->flags() & fl_onground)) return;

	vec3_t velocity = local->velocity();

	if (velocity.length_2d() > 20.0f) {
		vec3_t direction;
		vec3_t real_view;

		math::vector_angles(velocity, direction);
		interfaces::engine->get_view_angles(real_view);

		direction.y = real_view.y - direction.y;

		vec3_t forward;
		math::angle_vectors(direction, forward);

		static auto cl_forwardspeed = interfaces::console->get_convar(xorstr_("cl_forwardspeed"));
		static auto cl_sidespeed = interfaces::console->get_convar(xorstr_("cl_sidespeed"));

		auto negative_forward_speed = -cl_forwardspeed->float_value;
		auto negative_side_speed = -cl_sidespeed->float_value;

		auto negative_forward_direction = forward * negative_forward_speed;
		auto negative_side_direction = forward * negative_side_speed;

		m_pcmd->forwardmove = negative_forward_direction.x;
		m_pcmd->sidemove = negative_side_direction.y;
	}
}

void misc::movement::pre_pixelsurf(c_usercmd* cmd) {
	//if (will_ps) {
	//	cmd->buttons |= in_duck;
	//	cmd->viewangles = ps_angles;
	//	cmd->sidemove = ps_lock;
	//	cmd->forwardmove = 0.f;
	//}

}

bool pixel_surf_detection(vec3_t pre_velocity, int pre_flags) {
	if (pre_velocity.z == -6.25f && // the velocity of a pixel surf
		csgo::local_player->velocity().z == -6.25f && // check it again but for predicted velocity
		csgo::local_player->velocity().z > -7 && // make sure we aren't just falling
		pre_velocity.z > -7 && // same here again for a fall check
		!(pre_flags & fl_onground) && // make sure we're in the air lol
		!(csgo::local_player->flags() & fl_onground)) // and air again
		return true;
	else return false;

}

//#define FLOAT float
//#undef float
//#define INT int
//#undef int
//#define STRING const char*
//#undef const char*
//#define POINTER *
//#define DEF_ADD(x) (float)x
//int address = DEF_ADD(0x01232);

std::pair<int, vec3_t> get_align_side()
{
	vec3_t mins = csgo::local_player->collideable()->mins();
	vec3_t maxs = csgo::local_player->collideable()->maxs();
	const vec3_t org_origin = csgo::local_player->origin();

	trace_t info[4];

	for (int i = 0; i < 4; i++)
	{
		vec3_t startpos = org_origin;
		vec3_t endpos = org_origin;
		switch (i)
		{
		case 0:
			startpos.y += mins.y; // adding mins.y so were on the edge of bbox
			endpos.y = floor(startpos.y); // flooring so we trace to the nearest whole hammer unit
			break;
		case 1:
			startpos.x += maxs.x;
			endpos.x = floor(startpos.x) + 1.f;
			break;
		case 2:
			startpos.y += maxs.y;
			endpos.y = floor(startpos.y) + 1.f; //flooring y and adding 1 so we trace to opposite side
			break;
		case 3: //flooring x
			startpos.x += mins.x;
			endpos.x = floor(startpos.x); // negative x from origin
			break;
		}

		trace_t tr;
		trace_filter fil;
		fil.skip = csgo::local_player;
		ray_t ray;
		ray.init(org_origin, endpos);
		interfaces::trace_ray->trace_ray(ray, MASK_PLAYERSOLID, &fil, &tr);

		info[i] = tr;


	}

	float min_frac = 1.f;
	int bestind = -1;
	for (int i = 0; i < 4; i++)
	{
		auto& tr = info[i];
		if ((tr.flFraction < 1.f || tr.allsolid || tr.startSolid) && (tr.entity ? !tr.entity->is_player() : true))
		{
			min_frac = tr.flFraction;
			bestind = i;
		}
	}

	if (bestind != -1)
	{
		return { bestind, info[bestind].end };
	}


	return { bestind, {0,0,0} };
}

int savedbuttons;
int predictedtickamount = -1;
void misc::movement::pixel_surf(c_usercmd* cmd) {


	if (csgo::local_player->flags() & fl_onground ||
		should_eb ||
		!settings::movement::pixelsurf ||
		!GetAsyncKeyState(settings::movement::pixelsurf_key)) {
		will_ps = false;
		hit_ps = false;
		return;
	}

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (csgo::local_player->move_type() == movetype_ladder || csgo::local_player->move_type() == movetype_noclip || csgo::local_player->flags() & fl_onground)
		return;

	int tickspredicted = 0;
	bool foundpx = false;

	int predicted_ticks = interfaces::prediction->predicted_commands();


	interfaces::prediction->restore_entity_to_predicted_frame(predicted_ticks - 1);

	auto al_res = get_align_side();

	float org_fmove = cmd->forwardmove;
	float org_smove = cmd->sidemove;

	if (al_res.first != -1 && !(csgo::local_player->flags() & fl_onground) && cmd->sidemove == 0.f && cmd->forwardmove == 0.f)
	{
		vec3_t ang_diff = ((al_res.second - csgo::local_player->origin().to_angle() - cmd->viewangles).normalized()); // calcrelativeangle

		vec3_t  move, dir;
		float   delta, len;
		vec3_t   move_angle;


		move = { 450.f, 0.f, 0.f };

		len = move.normalize_movement();

		if (len)
		{
			math::vector_angles(move, move_angle);

			delta = ((cmd->viewangles.y + ang_diff.y) - cmd->viewangles.y);

			move_angle.y += delta;

			math::angle_vectors2(move_angle, &dir);

			dir *= len;

			if (cmd->viewangles.x < -90 || cmd->viewangles.x > 90)
				dir.x = -dir.x;

			cmd->forwardmove = dir.x;
			cmd->sidemove = -dir.y;
		}
	}

	for (int tickspredicted = 0; tickspredicted <= 12; tickspredicted++)
	{
		float pre_pred_vel = csgo::local_player->velocity().z;
		int restore_buttons = cmd->buttons;
		cmd->buttons |= in_duck;
		prediction::start(cmd);
		cmd->buttons = restore_buttons;
		float post_pred_vel = csgo::local_player->velocity().z;
		int post_pred_flags = csgo::local_player->flags();

		if (csgo::local_player->move_type() == movetype_ladder || (post_pred_flags & fl_onground))
			break;


		if (pre_pred_vel < -6.25 && post_pred_vel < -6.25 && post_pred_vel > pre_pred_vel && !(post_pred_flags & fl_onground))
		{
			foundpx = true;
			predictedtickamount = tickspredicted;

			break;
		}
		prediction::end();
	}

	if (foundpx)
	{
		misc::movement::hit_ps = true;
		cmd->buttons |= in_duck;
	}
	else
	{
		if (csgo::local_player->velocity().z == -6.25 || csgo::local_player->velocity().z == -3.125)
		{
			misc::movement::hit_ps = true;
			cmd->buttons |= in_duck;

		}
		else
		{
			cmd->forwardmove = org_fmove;
			cmd->sidemove = org_smove;
			misc::movement::hit_ps = false;
		}

	}

	if (predictedtickamount == -1)
	{
		//csgo->pixelsurf_lockmouse = false;
	}
	else
	{
		--predictedtickamount;
		//	csgo->pixelsurf_lockmouse = true;

	}
}

void misc::movement::auto_duck(c_usercmd* cmd) {

	if (!settings::movement::auto_duck ||
		!GetAsyncKeyState(settings::movement::auto_duck_key))
		return;

	if (!(data::pre_command::flags & fl_onground) || // in da air
		(csgo::local_player->flags() & fl_onground)) // on da ground
		cmd->buttons |= in_duck; // da crouch

	for (int i = 0; i < time_to_ticks(0.2f); i++) {

		int pre_flags = csgo::local_player->flags();

		prediction::start(cmd);
		prediction::end();

		int post_flags = csgo::local_player->flags();

		if (!(pre_flags & fl_onground) && (post_flags & fl_onground))
			cmd->buttons |= in_duck;
	}
}

// dont remove this.
void set_jumpheight(float height) {
	height = height - 1;
}

void misc::movement::jumpheight_exploit(c_usercmd* cmd)
{
	// THIS IS SUPER PRIVETATE!"!!! DO NOT LEKAK K!K!!
	// THANK TYOU N4ZZUUU  FOR COEDEE
	// FIRSTLYYYY we check if OUR LOCAL_PLAYER is ALIVE AND RE£ADY TO BOUNCEEEE
	if (csgo::local_player->is_alive()
		&& csgo::local_player->flags() & fl_onground)
	{
		set_jumpheight(INT_MAX);
	}
}