#include "../features.hpp"
#include "../../menu/ImGui/custom.h"
#include "../../menu/config/config.hpp"
#include "../../menu/menu.hpp"
#include "../../../source-sdk/steam-sdk/steam_api.h"
#include "../../xor.h"


bbox_t visuals::get_bbox(player_t* player) {
	bbox_t bbox = { 0,0,0,0 };
	auto collideable = player->collideable();

	if (!collideable)
		return bbox;


	auto min = collideable->mins();
	auto max = collideable->maxs();

	matrix_t& trans = player->coordinate_frame();

	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	vec3_t points_transformed[8];
	for (int i = 0; i < 8; i++) {
		math::transform_vector(points[i], trans, points_transformed[i]);
	}

	vec3_t screen_points[8] = {};

	for (int i = 0; i < 8; i++) {
		if (!math::world_to_screen(points_transformed[i], screen_points[i]))
			return bbox;
	}

	auto left = screen_points[0].x;
	auto top = screen_points[0].y;
	auto right = screen_points[0].x;
	auto bottom = screen_points[0].y;

	for (int i = 1; i < 8; i++) {
		if (left > screen_points[i].x)
			left = screen_points[i].x;
		if (top < screen_points[i].y)
			top = screen_points[i].y;
		if (right < screen_points[i].x)
			right = screen_points[i].x;
		if (bottom > screen_points[i].y)
			bottom = screen_points[i].y;
	}
	bbox.x = left;
	bbox.y = bottom;
	bbox.w = right - left;
	bbox.h = top - bottom;
	return bbox;
}

void visuals::run() {
	if (!csgo::local_player)
		return;

	if (!interfaces::engine->is_in_game() && !interfaces::engine->is_connected())
		return;

	if (&entity_t::networkable == nullptr)
		return;

	//player drawing loop
	for (int i = 1; i <= 64; ++i) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!player || player == csgo::local_player || player->team() == csgo::local_player->team())
			continue;

		if (player) {
			if (i <= 64) {
				vec3_t pl_head = player->get_hitbox_position(hitbox_head);
				vec3_t pl_origin = player->origin();
				if (math::world_to_screen(player->get_hitbox_position(hitbox_head), pl_head) ||
					math::world_to_screen(player->origin(), pl_origin)) {
					visuals::box_ctx = get_bbox(player);

					if (settings::visuals::engine_radar)
						player->spotted() = true;

					handle_dormant_animation(player);


					if (player->is_alive()) {
						if (settings::visuals::enable_esp) {
							render_players(player);
							render_names(player);
							render_health(player);
							render_armor(player);
							render_flags(player);
							render_weapon_name(player);
							render_skeleton(player);
							players_behind(player);
						}
					}

				}
			}
		}
	}
}

void visuals::handle_dormant_animation(player_t* entity) {

	float deltatime = (1.f / 0.2f) * interfaces::globals->frame_time * 0.4f;

	dormant_alpha[entity->index()] = ImClamp(dormant_alpha[entity->index()] + (3 * deltatime * (entity->dormant() ? -1.f : 1.f)), 0.2f, 1.f);
}

void visuals::render_players(player_t* entity) {

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);
	const float calcHorizontal = 0.5f * (settings::visuals::box_vertical * 0.01f);
	const float calcVertical = 0.5f * (settings::visuals::box_vertical * 0.01f);
	const float offsetHorizontal = settings::visuals::box_horizontal >= 99.f ? 1 : 0;
	const float offsetVertical = settings::visuals::box_horizontal >= 99.f ? 1 : 0;

	if (settings::visuals::visuals_box) {


		//if ( outline ) {
			//top left
		// outline

		render::line(vec2_t(box_ctx.x - 1, box_ctx.y - 1), vec2_t(offsetHorizontal + box_ctx.x + box_ctx.w * calcHorizontal, box_ctx.y - 1), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		// inline
		render::line(vec2_t(box_ctx.x - 1, box_ctx.y + 1), vec2_t(offsetHorizontal + box_ctx.x + box_ctx.w * calcHorizontal, box_ctx.y + 1), color(0, 0, 0, dormant_alpha[entity->index()] * 255));

		//top right
		//outline
		render::line(vec2_t(box_ctx.x + box_ctx.w, box_ctx.y - 1), vec2_t(box_ctx.x + box_ctx.w * (1.f - calcHorizontal) - 1, box_ctx.y - 1), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		//inline
		render::line(vec2_t(box_ctx.x + box_ctx.w, box_ctx.y + 1), vec2_t(box_ctx.x + box_ctx.w * (1.f - calcHorizontal) - 1, box_ctx.y + 1), color(0, 0, 0, dormant_alpha[entity->index()] * 255));

		//bottom left
		//outline
		render::line(vec2_t(box_ctx.x - 1, box_ctx.y + box_ctx.h), vec2_t(offsetHorizontal + box_ctx.x + box_ctx.w * calcHorizontal, box_ctx.y + box_ctx.h), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		//inline
		render::line(vec2_t(box_ctx.x - 1, box_ctx.y + box_ctx.h - 2), vec2_t(offsetHorizontal + box_ctx.x + box_ctx.w * calcHorizontal, box_ctx.y + box_ctx.h - 2), color(0, 0, 0, dormant_alpha[entity->index()] * 255));

		//bottom right
		//outline
		render::line(vec2_t(box_ctx.x + box_ctx.w, box_ctx.y + box_ctx.h), vec2_t(box_ctx.x + box_ctx.w * (1.f - calcHorizontal) - 1, box_ctx.y + box_ctx.h), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		//inline
		render::line(vec2_t(box_ctx.x + box_ctx.w, box_ctx.y + box_ctx.h - 2), vec2_t(box_ctx.x + box_ctx.w * (1.f - calcHorizontal) - 1, box_ctx.y + box_ctx.h - 2), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		//}

		/*Vertical*/



		//left top
		// outline
		render::line(vec2_t(box_ctx.x - 1, box_ctx.y), vec2_t(box_ctx.x - 1, offsetVertical + box_ctx.y + box_ctx.h * calcVertical), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		// inline
		render::line(vec2_t(box_ctx.x + 1, box_ctx.y), vec2_t(box_ctx.x + 1, offsetVertical + box_ctx.y + box_ctx.h * calcVertical), color(0, 0, 0, dormant_alpha[entity->index()] * 255));

		//left bottom
		//outline
		render::line(vec2_t(box_ctx.x - 1, box_ctx.y + box_ctx.h - 1), vec2_t(box_ctx.x - 1, box_ctx.y + box_ctx.h * (1.f - calcVertical)), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		// inline
		render::line(vec2_t(box_ctx.x + 1, box_ctx.y + box_ctx.h - 1), vec2_t(box_ctx.x + 1, box_ctx.y + box_ctx.h * (1.f - calcVertical)), color(0, 0, 0, dormant_alpha[entity->index()] * 255));

		//right top
		render::line(vec2_t(box_ctx.x + box_ctx.w, box_ctx.y), vec2_t(box_ctx.x + box_ctx.w, offsetVertical + box_ctx.y + box_ctx.h * calcVertical), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		// inline
		render::line(vec2_t(box_ctx.x + box_ctx.w - 2, box_ctx.y), vec2_t(box_ctx.x + box_ctx.w - 2, offsetVertical + box_ctx.y + box_ctx.h * calcVertical), color(0, 0, 0, dormant_alpha[entity->index()] * 255));

		//right bottom
		// outline
		render::line(vec2_t(box_ctx.x + box_ctx.w, box_ctx.y + box_ctx.h - 1), vec2_t(box_ctx.x + box_ctx.w, box_ctx.y + box_ctx.h * (1.f - calcVertical)), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		// inline
		render::line(vec2_t(box_ctx.x + box_ctx.w - 2, box_ctx.y + box_ctx.h - 2), vec2_t(box_ctx.x + box_ctx.w - 2, box_ctx.y + box_ctx.h * (1.f - calcVertical)), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
		//	}	//top left
		render::line(vec2_t(box_ctx.x, box_ctx.y), vec2_t(offsetHorizontal + box_ctx.x + box_ctx.w * calcHorizontal, box_ctx.y), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//top right
		render::line(vec2_t(box_ctx.x + box_ctx.w - 1, box_ctx.y), vec2_t(box_ctx.x + box_ctx.w * (1.f - calcHorizontal) - 1, box_ctx.y), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//bottom left
		render::line(vec2_t(box_ctx.x, box_ctx.y + box_ctx.h - 1), vec2_t(offsetHorizontal + box_ctx.x + box_ctx.w * calcHorizontal, box_ctx.y + box_ctx.h - 1), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//bottom right
		render::line(vec2_t(box_ctx.x + box_ctx.w - 1, box_ctx.y + box_ctx.h - 1), vec2_t(box_ctx.x + box_ctx.w * (1.f - calcHorizontal) - 1, box_ctx.y + box_ctx.h - 1), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//left top
		render::line(vec2_t(box_ctx.x, box_ctx.y), vec2_t(box_ctx.x, offsetVertical + box_ctx.y + box_ctx.h * calcVertical), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//left bottom
		render::line(vec2_t(box_ctx.x, box_ctx.y + box_ctx.h - 1), vec2_t(box_ctx.x, box_ctx.y + box_ctx.h * (1.f - calcVertical)), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//right top
		render::line(vec2_t(box_ctx.x + box_ctx.w - 1, box_ctx.y), vec2_t(box_ctx.x + box_ctx.w - 1, offsetVertical + box_ctx.y + box_ctx.h * calcVertical), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//right bottom
		render::line(vec2_t(box_ctx.x + box_ctx.w - 1, box_ctx.y + box_ctx.h - 1), vec2_t(box_ctx.x + box_ctx.w - 1, box_ctx.y + box_ctx.h * (1.f - calcVertical)), color(255, 255, 255, dormant_alpha[entity->index()] * 255));

		//	if ( outline ) {
	}
}

void visuals::render_names(player_t* entity) {

	if (settings::visuals::visuals_name)
	{
		player_info_t info;
		interfaces::engine->get_player_info(entity->index(), &info);

		std::string name = info.name;
		//name += std::string ( " " ).append ( settings::visuals::visuals_bot_extention_name );
		if (name.size() > 32)
			name = xorstr_("...");

		render::text_shadow(vec2_t(box_ctx.x + box_ctx.w / 2, box_ctx.y - 16), color(255, 255, 255, dormant_alpha[entity->index()] * 255), name, render::fonts::name, color(0, 0, 0, dormant_alpha[entity->index()] * 255), true);
	}

	if (settings::visuals::flash_kill)
		if (entity->health() <= 1)
			render::text_outline(vec2_t(box_ctx.x + box_ctx.w / 2, box_ctx.y - 16 - render::text_position(settings::visuals::visuals_flags_uppercase ? "FLASH" : "flash", render::fonts::flags).y - 2), color(255, 212, 143, dormant_alpha[entity->index()] * 205), settings::visuals::visuals_flags_uppercase ? "FLASH" : "flash", render::fonts::flags, color(0, 0, 0, dormant_alpha[entity->index()] * 255), true);
}

void visuals::render_health(player_t* entity) {

	if (!settings::visuals::visuals_health)
		return;

	float green = (entity->health() / 100.f);
	float red = 1 - (entity->health() / 100.f);

	render::filled_rectangle(vec2_t(box_ctx.x - 6, box_ctx.y - 1), vec2_t(4, box_ctx.h + 2), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
	if (!settings::visuals::gradient_healthbar)
		render::filled_rectangle(vec2_t(box_ctx.x - 5, box_ctx.y + (box_ctx.h - box_ctx.h * (std::clamp<int>(entity->health(), 0, 100.f) / 100.f))), vec2_t(2, box_ctx.h * (std::clamp<int>(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1)), color(red * 255, green * 255, 0, dormant_alpha[entity->index()] * 255));
	else
		render::gradient_rectangle(vec2_t(box_ctx.x - 5, box_ctx.y + (box_ctx.h - box_ctx.h * (std::clamp<int>(entity->health(), 0, 100.f) / 100.f))), vec2_t(2, box_ctx.h * (std::clamp<int>(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1)), color::from_float(settings::visuals::gradient_healthbar_col1).set_alpha(dormant_alpha[entity->index()] * 255), color::from_float(settings::visuals::gradient_healthbar_col2).set_alpha(dormant_alpha[entity->index()] * 255), true);

	if (entity->health() < 100) {
		render::text_outline(vec2_t(box_ctx.x - 5, box_ctx.y + (box_ctx.h - box_ctx.h * (std::clamp<int>(entity->health(), 0, 100.f) / 100.f))), color(255, 255, 255, dormant_alpha[entity->index()] * 255), std::to_string(entity->health()), render::fonts::flags, color(0, 0, 0, dormant_alpha[entity->index()] * 255), true);
	}
}

void visuals::render_armor(player_t* entity) {


	if (entity->armor() < 1)
		should_render_armor = false;
	else
		should_render_armor = true;

	if (!settings::visuals::visuals_armor)
		return;

	if (!should_render_armor)
		return;

	render::filled_rectangle(vec2_t(box_ctx.x - 1, box_ctx.y + box_ctx.h + 2), vec2_t(box_ctx.w + 2, 4), color(0, 0, 0, dormant_alpha[entity->index()] * 255));
	render::filled_rectangle(vec2_t(box_ctx.x, box_ctx.y + box_ctx.h + 3), vec2_t(box_ctx.w * (std::clamp<int>(entity->armor(), 0, 100) / 100.f) - (entity->armor() >= 100 ? 0 : -1), 2), color(99, 102, 255, dormant_alpha[entity->index()] * 255));
}

void visuals::render_flags(player_t* entity) {

	if (!settings::visuals::visuals_flags)
		return;

	std::vector<std::pair<std::string, color>> flags;

	if (entity->has_helmet() && entity->armor() > 0)
		flags.push_back(std::pair<std::string, color>(("HK"), color(255, 255, 255, dormant_alpha[entity->index()] * 255)));
	else if (!entity->has_helmet() && entity->armor() > 0)
		flags.push_back(std::pair<std::string, color>(("K"), color(255, 255, 255, 255)));

	if (entity->money())
		flags.push_back(std::pair<std::string, color>(std::format(("$ {}"), entity->money()), color(120, 255, 50, dormant_alpha[entity->index()] * 255)));

	if (entity->is_scoped())
		flags.push_back(std::pair<std::string, color>(("SCOPED"), color(80, 160, 200, dormant_alpha[entity->index()] * 255)));

	if (entity->is_flashed())
		flags.push_back(std::pair<std::string, color>(("FLASHED"), color(255, 255, 255, dormant_alpha[entity->index()] * 255)));

	auto positioning = 0;

	for (auto text : flags) {
		render::text_outline(vec2_t(box_ctx.x + box_ctx.w + 2, box_ctx.y + positioning - 1), text.second, text.first, render::fonts::flags, color(0, 0, 0, dormant_alpha[entity->index()] * 255), false);
		positioning += 10;
	}
}

void visuals::render_weapon_name(player_t* entity) {

	if (!settings::visuals::visuals_weapon)
		return;

	auto weapon = entity->active_weapon();
	if (!weapon)
		return;

	render::text_outline(vec2_t(box_ctx.x + (box_ctx.w / 2), box_ctx.y + box_ctx.h + (settings::visuals::visuals_armor ? 7 : 3)), color(255, 255, 255, dormant_alpha[entity->index()] * 255), weapon->weapon_name(), render::fonts::flags, color(0, 0, 0, dormant_alpha[entity->index()] * 255), true);

}

void visuals::render_skeleton(player_t* entity) {

	if (!settings::visuals::visuals_skeleton)
		return;

	auto p_studio_hdr = interfaces::model_info->get_studio_model(entity->model());
	if (!p_studio_hdr)
		return;

	vec3_t v_parent, v_child, s_parent, s_child;

	for (int i = 0; i < p_studio_hdr->bones_count; i++) {
		studio_bone_t* bone = p_studio_hdr->bone(i);
		if (!bone)
			return;

		if (bone && (bone->flags & BONE_USED_BY_HITBOX) && (bone->parent != -1)) {
			v_child = entity->get_bone_position(i);
			v_parent = entity->get_bone_position(bone->parent);

			if (math::world_to_screen(v_parent, s_parent) && math::world_to_screen(v_child, s_child))
				render::line(vec2_t(s_parent[0], s_parent[1]), vec2_t(s_child[0], s_child[1]), color(255, 255, 255, dormant_alpha[entity->index()] * 255));
		}
	}
}

ImTextureID visuals::getAvatarTexture(int i) noexcept {
	const auto it = std::as_const(playerAvatars).find(i);
	/*if ( it == playerAvatars.cend () )
		return team == Team::TT ? avatarTT.getTexture () : avatarCT.getTexture ();*/

	const auto& avatar = it->second;
	if (!avatar.texture.get())
		avatar.texture.init(32, 32, avatar.rgba.get());
	return avatar.texture.get();
}
void visuals::spectator_list() {

	if (!settings::visuals::spectator_list)
		return;

	std::vector<std::string> spectators;
	float deltatime = (1.0f / 0.2f) * ImGui::GetIO().DeltaTime * 0.75;
	static float spec_alpha = 1.f;
	int a = spec_alpha * 255;

	for (int i = 1; i <= 32; i++) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (player) {
			auto handle = player->observer_target();
			if (handle != 0xFFFFFFFF) {
				player_t* target = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(handle));
				player_info_t info;

				if (!target)
					continue;

				interfaces::engine->get_player_info(target->index(), &info);
				if (target && target == csgo::local_player) {
					player_info_t spectator_info;
					interfaces::engine->get_player_info(player->index(), &spectator_info);

					if (!player->dormant())
						spectators.push_back(spectator_info.name);
				}
			}
		}
	}
	spec_alpha = ImClamp(spec_alpha + (3.f * deltatime * ((menu::is_open || spectators.size() > 0) ? 1.f : -1.f)), 0.0f, 1.f);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, spec_alpha);
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.12, 0.5);
	ImGui::GetStyle().ItemSpacing = ImVec2(5, 1);
	ImGui::SetNextWindowSizeConstraints(ImVec2(180, 22), ImVec2(220, 400));
	ImGui::Begin("Spectators", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
	{ auto pos = ImGui::GetWindowPos(); auto size = ImGui::GetWindowSize();

	settings::visuals::spectator_list_pos_x = pos.x;
	settings::visuals::spectator_list_pos_y = pos.y;

	pos.x = settings::visuals::spectator_list_pos_x;
	pos.y = settings::visuals::spectator_list_pos_y;

	ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 10, pos.y + 4), ImColor(200, 200, 200, a), "Spectators");
	ImGui::GetWindowDrawList()->AddLine(ImVec2(pos.x, pos.y + 21), ImVec2(pos.x + size.x, pos.y + 21), ImColor(c::menu_accent[0], c::menu_accent[1], c::menu_accent[2], spec_alpha), 2.f);

	ImGui::SetCursorPos(ImVec2(4, 24));

	for (int i = 0; i < spectators.size(); i++) {
		ImGui::SetCursorPosX(4);
		ImGui::Text(spectators.at(i).c_str());
	}

	ImGui::Spacing();
	} ImGui::End();
	ImGui::PopStyleVar();
}




void visuals::run_weapon_esp(weapon_t* weapon) {

}

void visuals::players_behind(player_t* player)
{
	if (!settings::visuals::enable_esp)
		return;
	if (!settings::visuals::oof_arrows)
		return;

	if (!csgo::local_player)
		return;

	auto isOnScreen = [](vec3_t origin, vec3_t& screen) -> bool
	{
		if (!math::world_to_screen(origin, screen))
			return false;

		static int iScreenWidth, iScreenHeight;
		interfaces::engine->get_screen_size(iScreenWidth, iScreenHeight);

		auto xOk = iScreenWidth > screen.x;
		auto yOk = iScreenHeight > screen.y;

		return xOk && yOk;
	};

	vec3_t screenPos;

	if (isOnScreen(player->abs_origin(), screenPos))
		return;

	vec3_t viewAngles;
	interfaces::engine->get_view_angles(viewAngles);

	static int width, height;
	interfaces::engine->get_screen_size(width, height);

	auto screenCenter = vec2_t(width * 0.5f, height * 0.5f);
	auto angleYawRad = DEG2RAD(viewAngles.y - math::calculate_angle(csgo::local_player->get_eye_pos(), player->abs_origin()).y);

	auto radius = settings::visuals::oof_arrow_range;
	auto size = settings::visuals::oof_arrow_size;

	auto newPointX = screenCenter.x + ((((width - (size * 3)) * 0.5f) * (radius / 100.0f)) * cos(angleYawRad)) + (int)(6.0f * (((float)size - 4.0f) / 16.0f));
	auto newPointY = screenCenter.y + ((((height - (size * 3)) * 0.5f) * (radius / 100.0f)) * sin(angleYawRad));

	std::array <vec2_t, 3> points
	{
		vec2_t(newPointX - size, newPointY - size),
		vec2_t(newPointX + size, newPointY),
		vec2_t(newPointX - size, newPointY + size)
	};

	math::rotate_triangle(points, viewAngles.y - math::calculate_angle(csgo::local_player->get_eye_pos(), player->abs_origin()).y);
	render::triangle_grad_filled(points.at(0), points.at(1), points.at(2), color::from_float(settings::visuals::oof_arrow_col1), color::from_float(settings::visuals::oof_arrow_col2), color::from_float(settings::visuals::oof_arrow_col3));
}