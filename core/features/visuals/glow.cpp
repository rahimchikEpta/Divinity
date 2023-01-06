#include "../features.hpp"
#include "../../menu/config/config.hpp"

void visuals::glow() {
	if (!settings::visuals::glow_enabled)
		return;

	i_material* mat_glow = interfaces::material_system->find_material("dev/glow_color", TEXTURE_GROUP_OTHER, true);
	interfaces::model_render->override_material(mat_glow);


	for (size_t i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glow = interfaces::glow_manager->objects[i];
		if (glow.IsUnused())
			continue;

		auto glow_entity = reinterpret_cast<player_t*>(glow.m_hEntity);
		auto client_class = glow_entity->client_class();
		if (!glow_entity || glow_entity->dormant())
			continue;

		bool is_visible = csgo::local_player->can_see_player_pos(glow_entity, glow_entity->get_bone_position(8)); // LOL!
		bool is_enemy = glow_entity->is_enemy();

		switch (client_class->class_id) {
		case class_ids::ccsplayer:
			if (is_enemy && settings::visuals::glow_players) {
				if (is_visible)
					glow.set(settings::visuals::glow_player_col[0], settings::visuals::glow_player_col[1], settings::visuals::glow_player_col[2], settings::visuals::glow_player_col[3], 1.f, settings::visuals::glow_type_player);
				else
					glow.set(settings::visuals::glow_player_invis_col[0], settings::visuals::glow_player_invis_col[1], settings::visuals::glow_player_invis_col[2], settings::visuals::glow_player_invis_col[3], 1.f, settings::visuals::glow_type_player);
			}

			break;
		case class_ids::cchicken:
			if (settings::visuals::glow_chickens)
				glow.set(settings::visuals::glow_chicken_col[0], settings::visuals::glow_chicken_col[1], settings::visuals::glow_chicken_col[2], settings::visuals::glow_chicken_col[3], 1.f, settings::visuals::glow_type_chicken);
			break;

		case class_ids::cc4:
		case class_ids::cplantedc4:
			if (settings::visuals::glow_bombs)
				glow.set(settings::visuals::glow_bomb_col[0], settings::visuals::glow_bomb_col[1], settings::visuals::glow_bomb_col[2], settings::visuals::glow_bomb_col[3], 1.f, settings::visuals::glow_type_bomb);
			break;
		}
		if (glow_entity->is_weapon() && client_class->class_id != class_ids::cplantedc4) {
			if (settings::visuals::glow_weapons)
				glow.set(settings::visuals::glow_weapons_col[0], settings::visuals::glow_weapons_col[1], settings::visuals::glow_weapons_col[2], settings::visuals::glow_weapons_col[3], 1.f, settings::visuals::glow_type_weapons);
		}
	}
}