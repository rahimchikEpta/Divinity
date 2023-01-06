#include "../features.hpp"
#include "../../menu/menu.hpp"


void aim_assist::run(c_usercmd* cmd) {
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;


	auto weapon = csgo::local_player->active_weapon();
	if (!weapon || !weapon->clip1_count() || weapon->is_knife() || weapon->is_nade() || weapon->is_c4() || weapon->next_primary_attack() > csgo::server_time())
		return;

	if (menu::is_open)
		return;

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	int best_fov = 5;
	vec3_t target = vec3_t(0, 0, 0);
	vec3_t local_eye_position = csgo::local_player->get_eye_pos();

	std::vector<int> bones = {8, 7, 6, 5, 4, 3, 12, 40, 65, 66, 72, 73};

	convar* game_type = interfaces::console->get_convar("game_type");

	for (int i = 1; i <= 64; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive() || entity->has_gun_game_immunity())
			continue;

		if (game_type->get_int() == 6) {
			if ((csgo::local_player->survival_team() != -1) && (entity->survival_team() == csgo::local_player->survival_team()))
				continue;

			if (weapon->is_non_aim_weapon())
				continue;
		}
		else if (entity->team() == csgo::local_player->team())
			continue;

		for (const auto bone : bones) {
			auto bone_position = entity->get_bone_position(bone);


			auto angle = math::calculate_angle2(local_eye_position, bone_position, cmd->viewangles).clamped();

			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < 10) {

				target = bone_position;
				//best_fov = fov;
				is_targeting = true;
			}
			else
				is_targeting = false;
		}
	}
	// hella sanity checks bro.
	//is_targeting = false;
}