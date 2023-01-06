#include "../features.hpp"
#include <config.hpp>
#include "../../xor.h"

bbox_t weapon_box;
void render_name(bbox_t box, weapon_t* entity) {
	player_t* owner = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity_handle(entity->owner_handle()));
	if (owner)
		return;

	weapon_info_t* data = entity->get_weapon_data();

	if (!data)
		return;

	vec3_t world;
	vec3_t origin = entity->origin(); 

	float distance = csgo::local_player->origin().distance_to(entity->origin());

	float alpha = distance <= 250.f ? 1.f : std::lerp(1.f, 0.f, (distance / (settings::visuals::weapon_esp_distance + 250.f)) * 1);
	color col = color::from_float((settings::visuals::dropped_c4 && entity->is_c4()) ? settings::visuals::dropped_c4_col : settings::visuals::weapon_esp_col).set_alpha(alpha * 255);
	if (alpha < 0.02f)
		return;

	if (!math::world_to_screen(origin, world))
		return;

	if (entity->is_knife())
		return;

	if (entity->is_c4() && !settings::visuals::dropped_c4)
		return;

	std::string name{ entity->is_nade() ? xorstr_("GRANATA") : entity->is_taser() ? xorstr_("ZAPPER") : entity->weapon_name()};


	const float calcHorizontal = 0.5f * (settings::visuals::weapon_box_vertical * 0.01f);
	const float calcVertical = 0.5f * (settings::visuals::weapon_box_vertical * 0.01f);
	const float offsetHorizontal = 1;
	const float offsetVertical = 1;
	bool should_render_bounding_box = (settings::visuals::weapon_box && settings::visuals::weapon_box_vertical > 2.f);

	// LOL>.....
	if (should_render_bounding_box) {
		render::line(vec2_t(box.x - 1, box.y - 1), vec2_t(offsetHorizontal + box.x + box.w * calcHorizontal, box.y - 1), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x - 1, box.y + 1), vec2_t(offsetHorizontal + box.x + box.w * calcHorizontal, box.y + 1), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w, box.y - 1), vec2_t(box.x + box.w * (1.f - calcHorizontal) - 1, box.y - 1), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w, box.y + 1), vec2_t(box.x + box.w * (1.f - calcHorizontal) - 1, box.y + 1), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x - 1, box.y + box.h), vec2_t(offsetHorizontal + box.x + box.w * calcHorizontal, box.y + box.h), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x - 1, box.y + box.h - 2), vec2_t(offsetHorizontal + box.x + box.w * calcHorizontal, box.y + box.h - 2), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w, box.y + box.h), vec2_t(box.x + box.w * (1.f - calcHorizontal) - 1, box.y + box.h), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w, box.y + box.h - 2), vec2_t(box.x + box.w * (1.f - calcHorizontal) - 1, box.y + box.h - 2), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x - 1, box.y), vec2_t(box.x - 1, offsetVertical + box.y + box.h * calcVertical), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + 1, box.y), vec2_t(box.x + 1, offsetVertical + box.y + box.h * calcVertical), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x - 1, box.y + box.h - 1), vec2_t(box.x - 1, box.y + box.h * (1.f - calcVertical)), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + 1, box.y + box.h - 1), vec2_t(box.x + 1, box.y + box.h * (1.f - calcVertical)), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w, box.y), vec2_t(box.x + box.w, offsetVertical + box.y + box.h * calcVertical), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w - 2, box.y), vec2_t(box.x + box.w - 2, offsetVertical + box.y + box.h * calcVertical), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w, box.y + box.h - 1), vec2_t(box.x + box.w, box.y + box.h * (1.f - calcVertical)), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x + box.w - 2, box.y + box.h - 2), vec2_t(box.x + box.w - 2, box.y + box.h * (1.f - calcVertical)), color(0, 0, 0, alpha * 255));
		render::line(vec2_t(box.x, box.y), vec2_t(offsetHorizontal + box.x + box.w * calcHorizontal, box.y), col);
		render::line(vec2_t(box.x + box.w - 1, box.y), vec2_t(box.x + box.w * (1.f - calcHorizontal) - 1, box.y), col);
		render::line(vec2_t(box.x, box.y + box.h - 1), vec2_t(offsetHorizontal + box.x + box.w * calcHorizontal, box.y + box.h - 1), col);
		render::line(vec2_t(box.x + box.w - 1, box.y + box.h - 1), vec2_t(box.x + box.w * (1.f - calcHorizontal) - 1, box.y + box.h - 1), col);
		render::line(vec2_t(box.x, box.y), vec2_t(box.x, offsetVertical + box.y + box.h * calcVertical), col);
		render::line(vec2_t(box.x, box.y + box.h - 1), vec2_t(box.x, box.y + box.h * (1.f - calcVertical)), col);
		render::line(vec2_t(box.x + box.w - 1, box.y), vec2_t(box.x + box.w - 1, offsetVertical + box.y + box.h * calcVertical), col);
		render::line(vec2_t(box.x + box.w - 1, box.y + box.h - 1), vec2_t(box.x + box.w - 1, box.y + box.h * (1.f - calcVertical)), col);
	}
	
	if (should_render_bounding_box && settings::visuals::weapon_name)
		render::text_outline(vec2_t(box.x + (box.w / 2), box.y - 14), col, name, render::fonts::name, color(0, 0, 0, alpha * 255), true);
	else if (settings::visuals::weapon_ammo && settings::visuals::weapon_name)
		render::text_outline(vec2_t(box.x + (box.w/2), box.y + (box.h / 2) - 7), col, name, render::fonts::name, color(0, 0, 0, alpha * 255), true);
	else if (settings::visuals::weapon_name)
		render::text_outline(vec2_t(box.x + (box.w / 2), box.y + (box.h / 2)), col, name, render::fonts::name, color(0, 0, 0, alpha * 255), true);
	

	if (entity->is_nade() || entity->is_taser() || data->weapon_type == WEAPONTYPE_C4 ||  data->weapon_type == WEAPONTYPE_KNIFE)
		return;
	std::string ammo = std::format(xorstr_("({}/{})"), entity->clip1_count(), data->weapon_max_clip);
	if (settings::visuals::weapon_ammo && should_render_bounding_box)
		render::text_outline(vec2_t(box.x + (box.w / 2), box.y + box.h + 2), entity->clip1_count() < 1 ? color(255, 0, 0, alpha * 255) : col, entity->clip1_count() < 1 ? xorstr_("EMPTY") : ammo, render::fonts::name, color(0, 0, 0, alpha * 255), true);
	else if (settings::visuals::weapon_name && settings::visuals::weapon_ammo)
		render::text_outline(vec2_t(box.x + (box.w / 2), box.y + (box.h / 2) + 2), entity->clip1_count() < 1 ? color(255, 0, 0, alpha * 255) : col, entity->clip1_count() < 1 ? xorstr_("EMPTY") : ammo, render::fonts::name, color(0, 0, 0, alpha * 255), true);
	else if (settings::visuals::weapon_ammo)
		render::text_outline(vec2_t(box.x + (box.w / 2), box.y + (box.h / 2)), entity->clip1_count() < 1 ? color(255, 0, 0, alpha * 255) : col, entity->clip1_count() < 1 ? xorstr_("EMPTY") : ammo, render::fonts::name, color(0, 0, 0, alpha * 255), true);
}
void visuals::run_weapon_esp() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;


	if (!settings::visuals::weapon_esp)
		return;

	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity->dormant())
			continue;


		if (entity->is_weapon()) {
			weapon_box = visuals::get_bbox(entity);


			render_name(weapon_box, (weapon_t*)entity);
		}
		
		
	}
}