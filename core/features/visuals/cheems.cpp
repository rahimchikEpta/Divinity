#include "../features.hpp"
#include "../../menu/config/config.hpp"


i_material* visuals::chams::create_material(shader_type_t shade, bool ignorez, bool wireframe) {
	static const std::string material_name = "game_material.vmt";
	std::string shade_type;

	switch (shade) {
	case shader_type_t::VertexLitGeneric: shade_type = "VertexLitGeneric"; break;
	case shader_type_t::UnlitGeneric: shade_type = "UnlitGeneric"; break;
	case shader_type_t::Modulate: shade_type = "Modulate"; break;
	default: break;
	}

	std::string material_data;

	material_data += "\"" + shade_type + "\"\n{\n";

	material_data += "\t\"$model\" \"1\"\n";
	material_data += "\t\"$flat\" \"1\"\n";
	material_data += "\t\"$selfillum\" \"1\"\n";
	material_data += "\t\"$selfillumtint\" \"[8 8 8]\"\n";
	material_data += "\t\"$halflambert\" \"1\"\n";
	material_data += "\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\n";
	material_data += "\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n";

	material_data += "\t\"$mod2x\" \"1\"\n";
	material_data += "\t\"$nocull\" \"1\"\n"; // for Modulate shader	

	material_data += "\"Proxies\"\n{\n";
	// add proxies here.
	material_data += "}\n"; // end of proxies.

	material_data += "}\n"; // end of shader type.

	auto kv = static_cast<c_key_values*>(malloc(36));

	kv->init(shade_type.c_str());
	kv->load_from_buffer(material_name.c_str(), material_data.c_str());

	return interfaces::material_system->create_material(material_name.c_str(), kv);
}
color fade_between_with_tick(color first, color second, int t, int max_tick) {



	int r_up = second.r - first.r;
	int r_down = first.r - second.r;

	int g_up = second.g - first.g;
	int g_down = first.g - second.g;

	int b_up = second.b - first.b;
	int b_down = first.b - second.b;

	bool red_has_to_go_up = first.r > second.r;
	bool green_has_to_go_up = first.g > second.g;
	bool blue_has_to_go_up = first.b > second.b;

	return color(red_has_to_go_up ? first.r + ((max_tick / t) * r_up) : first.r - ((max_tick / t) * r_down), green_has_to_go_up ? first.g + ((max_tick / t) * g_up) : first.g - ((max_tick / t) * g_down), blue_has_to_go_up ? first.b + ((max_tick / t) * b_up) : first.b - ((max_tick / t) * b_down));
}

bool only_run_once_my_nigga = false;

void visuals::chams::run(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {

	static auto draw_model_execute_original = reinterpret_cast<hooks::dme::fn>(hooks::get_virtual(interfaces::model_render, 21));

	if (!interfaces::material_system || !interfaces::model_info || !interfaces::model_render)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	static i_material* mat = nullptr;
	static i_material* zmat = nullptr;
	static i_material* snow_mat = nullptr;
	static i_material* glow_mat = nullptr;
	static i_material* disco_mat = nullptr;
	static i_material* flat = nullptr;
	static i_material* zflat = nullptr;
	static i_material* animated_lines_mat = nullptr;
	static i_material* animated_mat = nullptr;
	static i_material* metallic = nullptr;
	static i_material* wireframe = nullptr;
	static i_material* palm_tree = nullptr;
	static i_material* velvet = nullptr;

	static i_material* crystal_mat = nullptr;

	if (!only_run_once_my_nigga) {


		std::ofstream("csgo/materials/snowy.vmt") << R"#("VertexLitGeneric" {
 
			"$basetexture"                "dev/snowfield"
                "$additive"                    "1"
                "$envmap"                    "editor/cube_vertigo"
                "$envmaptint"                "[0 0.5 0.55]"
                "$envmapfresnel"            "1"
                "$envmapfresnelminmaxexp"   "[0.00005 0.6 6]"
                "$alpha"                    "1"
 
                Proxies
                {
                    TextureScroll
                    {
                        "texturescrollvar"            "$baseTextureTransform"
                        "texturescrollrate"            "0.10"
                        "texturescrollangle"        "270"
                    }
                }
		})#";

		std::ofstream("csgo/materials/glow_mat.vmt") << R"#("VertexLitGeneric" {
 
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[1 1 1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "0.8"
		})#";

		std::ofstream("csgo/materials/animated.vmt") << R"#("VertexLitGeneric" {
 
			"$envmap editor" "cube_vertigo"
			"$envmapcontrast" "1"
			"$basetexture" "dev/zone_warning"
				
				Proxies 
				{
					texturescroll 
					{
						"texturescrollvar" 
						"$basetexturetransform"
						"texturescrollrate" "0.6"
						"texturescrollangle" "90"
					}
				}
		})#";

		std::ofstream("csgo/materials/animated_lines.vmt") << R"#("VertexLitGeneric" {
 
			"$envmap editor" "cube_vertigo"
			"$envmapcontrast" "1"
			"$basetexture" "models/inventory_items/dreamhack_trophies/dreamhack_star_blur"
				
				Proxies 
				{
					texturescroll 
					{
						"texturescrollvar" 
						"$basetexturetransform"
						"texturescrollrate" "0.6"
						"texturescrollangle" "90"
					}
				}
		})#";

		std::ofstream("csgo/materials/disco.vmt") << R"#("VertexLitGeneric"
        {
            "$basetexture" "vgui/white"
            "$envmap" "models/effects/crystal_cube_vertigo_hdr"
            "$envmaptint" "[1 1 1]"
            "$envmapfresnel" "1"
            "$envmapfresnelminmaxexp" "[0 1 2]"
            "$phong" "1"
            "$reflectivity" "[2 2 2]"
            "$bumpmap" "models/weapons/customization/materials/origamil_camo"
            "$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
            "Proxies"
            {
                "TextureScroll"
                {
                    "textureScrollVar" "$bumptransform"
                    "textureScrollRate" "0.2"
                    "textureScrollAngle" "0.0"
                }
            }
        })#";

		std::ofstream("csgo/materials/metallic.vmt") << R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";
		mat = create_material(shader_type_t::VertexLitGeneric, false, false);
		zmat = create_material(shader_type_t::VertexLitGeneric, true, false);

		wireframe = create_material(shader_type_t::VertexLitGeneric, false, true);

		flat = create_material(shader_type_t::UnlitGeneric, false, false);
		zflat = create_material(shader_type_t::UnlitGeneric, true, false);
		snow_mat = interfaces::material_system->find_material("snowy", nullptr);
		glow_mat = interfaces::material_system->find_material("glow_mat", nullptr);
		disco_mat = interfaces::material_system->find_material("disco", nullptr);
		animated_lines_mat = interfaces::material_system->find_material("animated_lines", nullptr);//interfaces::material_system->find_material ( "models/inventory_items/dreamhack_trophies/dreamhack_star_blur", nullptr );
		animated_mat = interfaces::material_system->find_material("animated", nullptr);
		metallic = interfaces::material_system->find_material("metallic", nullptr);
		palm_tree = interfaces::material_system->find_material("models/props/de_dust/hr_dust/foliage/palm_bark_01", nullptr);
		velvet = interfaces::material_system->find_material("models/inventory_items/trophy_majors/velvet", nullptr);

		crystal_mat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", nullptr);



		only_run_once_my_nigga = true;
	}

	static i_material* player_material = nullptr;
	static i_material* backtrack_material = nullptr;
	static i_material* ragdoll_player_material = nullptr;
	static i_material* player_material_invis = nullptr;
	static i_material* player_overlay_material = nullptr;
	switch (settings::visuals::player_material) {


	case 0:
		player_material = mat;
		break;
	case 1:
		player_material = flat;
		break;
	case 2:
		player_material = animated_mat;
		break;
	case 3:
		player_material = crystal_mat;
		break;
	case 4:
		player_material = metallic;
		break;
	case 5:
		player_material = disco_mat;
		break;

	}

	switch (settings::visuals::ragdoll_chams_material) {


	case 0:
		ragdoll_player_material = mat;
		break;
	case 1:
		ragdoll_player_material = flat;
		break;
	case 2:
		ragdoll_player_material = animated_mat;
		break;
	case 3:
		ragdoll_player_material = crystal_mat;
		break;
	case 4:
		ragdoll_player_material = metallic;
		break;

	}

	switch (settings::visuals::player_material_invis) {


	case 0:
		player_material_invis = zmat;
		break;
	case 1:
		player_material_invis = zflat;
		break;
	case 2:
		player_material_invis = animated_mat;
		break;
	case 3:
		player_material_invis = crystal_mat;
		break;
	case 4:
		player_material_invis = metallic;
		break;
	case 5:
		player_material_invis = disco_mat;
		break;

	}

	switch (settings::visuals::player_overlay_material) {
	case 0:
		player_overlay_material = glow_mat;
		break;
	case 1:
		player_overlay_material = snow_mat;
		break;
	case 2:
		player_overlay_material = animated_lines_mat;
		break;
	}

	switch (settings::visuals::backtrack_chams_type) {


	case 0:
		backtrack_material = zmat;
		break;
	case 1:
		backtrack_material = zflat;
		break;
	case 2:
		backtrack_material = animated_mat;
		break;
	case 3:
		backtrack_material = crystal_mat;
		break;
	case 4:
		backtrack_material = metallic;
		break;
	case 5:
		backtrack_material = disco_mat;
		break;

	}

	auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(info.entity_index));

	const std::string_view model_name = info.model->name;

	if (!player_material || !player_material_invis || !backtrack_material)
		return;

	if (!entity || entity->dormant() || entity->team() == csgo::local_player->team())
		return;


	float clr_chams_health[]{ (255 - entity->health() * 2.55) / 255.f, (entity->health() * 2.55) / 255.f, 0.f, 1.f };

	if (/*model_name.find("player") != std::string_view::npos && model_name.find("shadow") == std::string_view::npos &&*/ entity->is_alive()) {
		if (settings::aimbot::backtrack_enable && settings::visuals::backtrack_chams && entity->is_moving() && csgo::local_player->is_alive()) {
			auto record = &lag_compensation::records[info.entity_index];


			switch (settings::visuals::backtrack_render_type) {
			case 0: /// Static last tick
				if (record && !record->empty() && record->size() && lag_compensation::valid(record->back().simulation_time)) {

					interfaces::render_view->modulate_color(settings::visuals::backtrack_chams_color);
					interfaces::render_view->set_blend(settings::visuals::backtrack_chams_color[3]);
					backtrack_material->set_material_var_flag(material_var_ignorez, true);
					interfaces::model_render->override_material(backtrack_material);
					draw_model_execute_original(interfaces::model_render, ctx, state, info, record->back().matrix);
				}
				break;
			case 1: /// Static all tick
				for (int t = 1; t < record->size(); t++) {
					if (record && !record->empty() && record->size() && lag_compensation::valid(record->at(t).simulation_time)) {

						interfaces::render_view->modulate_color(settings::visuals::backtrack_chams_color);
						interfaces::render_view->set_blend(settings::visuals::backtrack_chams_color[3]);
						backtrack_material->set_material_var_flag(material_var_ignorez, true);
						interfaces::model_render->override_material(backtrack_material);
						draw_model_execute_original(interfaces::model_render, ctx, state, info, record->at(t).matrix);
					}
				}
				break;
			case 2: /// Gradient all tick
				for (int t = 1; t < record->size(); t++) {
					if (record && !record->empty() && record->size() && lag_compensation::valid(record->at(t).simulation_time)) {
						color first = color::from_float(settings::visuals::backtrack_chams_color);
						color second = color::from_float(settings::visuals::backtrack_chams_color2);

						float tick = (float)(t - 1.f) / (float)(record->size() - 1.f);

						float adjusted_r = math::lerp(first.r, second.r, tick);
						float adjusted_g = math::lerp(first.g, second.g, tick);
						float adjusted_b = math::lerp(first.b, second.b, tick);
						float adjusted_a = math::lerp(first.a, second.a, tick);


						float flCol[4] = { adjusted_r / 255.f, adjusted_g / 255.f, adjusted_b / 255.f, adjusted_a / 255.f };
						interfaces::render_view->modulate_color(flCol);
						interfaces::render_view->set_blend(adjusted_a / 255.f);
						backtrack_material->set_material_var_flag(material_var_ignorez, true);
						interfaces::model_render->override_material(backtrack_material);
						draw_model_execute_original(interfaces::model_render, ctx, state, info, record->at(t).matrix);
					}
				}
				break;
			}


		}

		if (settings::visuals::enable_chams_invis) {
			interfaces::render_view->modulate_color(settings::visuals::chams_invis);
			interfaces::render_view->set_blend(settings::visuals::chams_invis[3]);
			player_material_invis->set_material_var_flag(material_var_ignorez, true);
			interfaces::model_render->override_material(player_material_invis);
			draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
			if (settings::visuals::chams_overlay) {
				bool found = false;
				auto var = player_overlay_material->find_var("$envmaptint", &found);
				if (found)
					(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)var + 44))((uintptr_t)var, settings::visuals::chams_overlay_clr[0], settings::visuals::chams_overlay_clr[1], settings::visuals::chams_overlay_clr[2]);
				interfaces::render_view->modulate_color(settings::visuals::chams_overlay_clr);
				interfaces::render_view->set_blend(settings::visuals::chams_overlay_clr[3]);
				player_overlay_material->set_material_var_flag(material_var_ignorez, true);
				player_overlay_material->set_material_var_flag(material_var_wireframe, settings::visuals::player_overlay_material == 2);
				interfaces::model_render->override_material(player_overlay_material);
				//if ( settings::visuals::chams_invis )
				draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
			}
		}


		if (settings::visuals::enable_chams_vis) {
			interfaces::render_view->modulate_color(settings::visuals::chams_vis);
			interfaces::render_view->set_blend(settings::visuals::chams_vis[3]);
			player_material->set_material_var_flag(material_var_ignorez, false);
			interfaces::model_render->override_material(player_material);
			draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
			if (settings::visuals::chams_overlay) {
				bool found = false;
				auto var = player_overlay_material->find_var("$envmaptint", &found);
				if (found)
					(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)var + 44))((uintptr_t)var, settings::visuals::chams_overlay_clr[0], settings::visuals::chams_overlay_clr[1], settings::visuals::chams_overlay_clr[2]);
				interfaces::render_view->modulate_color(settings::visuals::chams_overlay_clr);
				interfaces::render_view->set_blend(settings::visuals::chams_overlay_clr[3]);
				player_overlay_material->set_material_var_flag(material_var_ignorez, false);
				player_overlay_material->set_material_var_flag(material_var_wireframe, settings::visuals::player_overlay_material == 2);
				interfaces::model_render->override_material(player_overlay_material);
				draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
			}
		}


		if (settings::visuals::ragdoll_chams && !entity->is_alive()) {
			interfaces::render_view->modulate_color(settings::visuals::ragdoll_chams_clr);
			interfaces::render_view->set_blend(settings::visuals::ragdoll_chams_clr[3]);
			ragdoll_player_material->set_material_var_flag(material_var_ignorez, false);
			interfaces::model_render->override_material(ragdoll_player_material);
			draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
		}
	}




}

bool arm_run_nigger = false;
void visuals::chams::run_local(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {

	static auto draw_model_execute_original = reinterpret_cast<hooks::dme::fn>(hooks::get_virtual(interfaces::model_render, 21));

	if (!interfaces::material_system || !interfaces::model_info || !interfaces::model_render)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	static i_material* mat = nullptr;
	static i_material* snow_mat = nullptr;
	static i_material* glow_mat = nullptr;
	static i_material* flat = nullptr;
	static i_material* animated_lines_mat = nullptr;
	static i_material* animated_mat = nullptr;
	static i_material* metallic = nullptr;
	static i_material* disco_mat = nullptr;

	static i_material* crystal_mat = nullptr;

	if (!arm_run_nigger) {


		std::ofstream("csgo/materials/snowy.vmt") << R"#("VertexLitGeneric" {
 
			"$basetexture"                "dev/snowfield"
                "$additive"                    "1"
                "$envmap"                    "editor/cube_vertigo"
                "$envmaptint"                "[0 0.5 0.55]"
                "$envmapfresnel"            "1"
                "$envmapfresnelminmaxexp"   "[0.00005 0.6 6]"
                "$alpha"                    "1"
 
                Proxies
                {
                    TextureScroll
                    {
                        "texturescrollvar"            "$baseTextureTransform"
                        "texturescrollrate"            "0.10"
                        "texturescrollangle"        "270"
                    }
                }
		})#";

		std::ofstream("csgo/materials/glow_material.vmt") << R"#("VertexLitGeneric" {
 
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[1 1 1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "0.8"
		})#";

		std::ofstream("csgo/materials/animated.vmt") << R"#("VertexLitGeneric" {
 
			"$envmap editor" "cube_vertigo"
			"$envmapcontrast" "1"
			"$basetexture" "dev/zone_warning"
				
				Proxies 
				{
					texturescroll 
					{
						"texturescrollvar" 
						"$basetexturetransform"
						"texturescrollrate" "0.6"
						"texturescrollangle" "90"
					}
				}
		})#";

		std::ofstream("csgo/materials/animated_lines.vmt") << R"#("VertexLitGeneric" {
 
			"$envmap editor" "cube_vertigo"
			"$envmapcontrast" "1"
			"$basetexture" "models/inventory_items/dreamhack_trophies/dreamhack_star_blur"
				
				Proxies 
				{
					texturescroll 
					{
						"texturescrollvar" 
						"$basetexturetransform"
						"texturescrollrate" "0.6"
						"texturescrollangle" "90"
					}
				}
		})#";
		std::ofstream("csgo/materials/disco.vmt") << R"#("VertexLitGeneric"
        {
            "$basetexture" "vgui/white"
            "$envmap" "models/effects/crystal_cube_vertigo_hdr"
            "$envmaptint" "[1 1 1]"
            "$envmapfresnel" "1"
            "$envmapfresnelminmaxexp" "[0 1 2]"
            "$phong" "1"
            "$reflectivity" "[2 2 2]"
            "$bumpmap" "models/weapons/customization/materials/origamil_camo"
            "$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
            "Proxies"
            {
                "TextureScroll"
                {
                    "textureScrollVar" "$bumptransform"
                    "textureScrollRate" "0.2"
                    "textureScrollAngle" "0.0"
                }
            }
        })#";
		std::ofstream("csgo/materials/metallic.vmt") << R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";
		mat = create_material(shader_type_t::VertexLitGeneric, false, false);

		flat = create_material(shader_type_t::UnlitGeneric, false, false);
		snow_mat = interfaces::material_system->find_material("snowy", nullptr);
		glow_mat = interfaces::material_system->find_material("glow_material", nullptr);
		disco_mat = interfaces::material_system->find_material("disco", nullptr);
		animated_lines_mat = interfaces::material_system->find_material("animated_lines", nullptr);//interfaces::material_system->find_material ( "models/inventory_items/dreamhack_trophies/dreamhack_star_blur", nullptr );
		animated_mat = interfaces::material_system->find_material("animated", nullptr);
		metallic = interfaces::material_system->find_material("metallic", nullptr);
		crystal_mat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", nullptr);



		arm_run_nigger = true;
	}

	static i_material* arm_material = nullptr;
	static i_material* arm_overlay_material = nullptr;


	switch (settings::visuals::arm_chams_material) {


	case 0:
		arm_material = mat;
		break;
	case 1:
		arm_material = flat;
		break;
	case 2:
		arm_material = animated_mat;
		break;
	case 3:
		arm_material = crystal_mat;
		break;
	case 4:
		arm_material = metallic;
		break;
	case 5:
		arm_material = disco_mat;
		break;

	}

	switch (settings::visuals::arm_overlay_chams) {
	case 0:
		arm_overlay_material = glow_mat;
		break;
	case 1:
		arm_overlay_material = snow_mat; //snow_mat
		break;
	case 2:
		arm_overlay_material = animated_lines_mat; //animated_lines_mat
		break;
	default:
		arm_overlay_material = glow_mat;
	}


	if ((std::strstr(info.model->name, "arms")))
		if (settings::visuals::arm_chams) {
			interfaces::render_view->modulate_color(settings::visuals::arm_chams_color);
			interfaces::render_view->set_blend(settings::visuals::arm_chams_color[3]);
			interfaces::model_render->override_material(arm_material);
			draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
			if (settings::visuals::arm_overlay_chams) {
				bool found = false;
				auto var = arm_overlay_material->find_var("$envmaptint", &found);
				if (found)
					(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)var + 44))((uintptr_t)var, settings::visuals::arm_overlay_color[0], settings::visuals::arm_overlay_color[1], settings::visuals::arm_overlay_color[2]);
				interfaces::render_view->modulate_color(settings::visuals::arm_overlay_color);
				interfaces::render_view->set_blend(settings::visuals::arm_overlay_color[3]);
				arm_overlay_material->set_material_var_flag(material_var_ignorez, false);
				arm_overlay_material->set_material_var_flag(material_var_wireframe, settings::visuals::arm_overlay_material == 2);
				interfaces::model_render->override_material(arm_overlay_material);
				draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);

			}
		}
}

bool weapon_run_nigger = false;
void visuals::chams::run_weapons(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {

	static auto draw_model_execute_original = reinterpret_cast<hooks::dme::fn>(hooks::get_virtual(interfaces::model_render, 21));

	if (!interfaces::material_system || !interfaces::model_info || !interfaces::model_render)
		return;

	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player)
		return;

	static i_material* mat = nullptr;
	static i_material* zmat = nullptr;
	static i_material* snow_mat = nullptr;
	static i_material* glow_mat = nullptr;
	static i_material* flat = nullptr;
	static i_material* zflat = nullptr;
	static i_material* animated_lines_mat = nullptr;
	static i_material* animated_mat = nullptr;
	static i_material* metallic = nullptr;
	static i_material* disco_mat = nullptr;
	static i_material* wireframe = nullptr;
	static i_material* palm_tree = nullptr;
	static i_material* velvet = nullptr;

	static i_material* crystal_mat = nullptr;

	if (!weapon_run_nigger) {


		std::ofstream("csgo/materials/snowy.vmt") << R"#("VertexLitGeneric" {
 
			"$basetexture"                "dev/snowfield"
                "$additive"                    "1"
                "$envmap"                    "editor/cube_vertigo"
                "$envmaptint"                "[0 0.5 0.55]"
                "$envmapfresnel"            "1"
                "$envmapfresnelminmaxexp"   "[0.00005 0.6 6]"
                "$alpha"                    "1"
 
                Proxies
                {
                    TextureScroll
                    {
                        "texturescrollvar"            "$baseTextureTransform"
                        "texturescrollrate"            "0.10"
                        "texturescrollangle"        "270"
                    }
                }
		})#";

		std::ofstream("csgo/materials/glow_mat.vmt") << R"#("VertexLitGeneric" {
 
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[1 1 1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "0.8"
		})#";

		std::ofstream("csgo/materials/animated.vmt") << R"#("VertexLitGeneric" {
 
			"$envmap editor" "cube_vertigo"
			"$envmapcontrast" "1"
			"$basetexture" "dev/zone_warning"
				
				Proxies 
				{
					texturescroll 
					{
						"texturescrollvar" 
						"$basetexturetransform"
						"texturescrollrate" "0.6"
						"texturescrollangle" "90"
					}
				}
		})#";
		std::ofstream("csgo/materials/disco.vmt") << R"#("VertexLitGeneric"
        {
            "$basetexture" "vgui/white"
            "$envmap" "models/effects/crystal_cube_vertigo_hdr"
            "$envmaptint" "[1 1 1]"
            "$envmapfresnel" "1"
            "$envmapfresnelminmaxexp" "[0 1 2]"
            "$phong" "1"
            "$reflectivity" "[2 2 2]"
            "$bumpmap" "models/weapons/customization/materials/origamil_camo"
            "$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
            "Proxies"
            {
                "TextureScroll"
                {
                    "textureScrollVar" "$bumptransform"
                    "textureScrollRate" "0.2"
                    "textureScrollAngle" "0.0"
                }
            }
        })#";
		std::ofstream("csgo/materials/animated_lines.vmt") << R"#("VertexLitGeneric" {
 
			"$envmap editor" "cube_vertigo"
			"$envmapcontrast" "1"
			"$basetexture" "models/inventory_items/dreamhack_trophies/dreamhack_star_blur"
				
				Proxies 
				{
					texturescroll 
					{
						"texturescrollvar" 
						"$basetexturetransform"
						"texturescrollrate" "0.6"
						"texturescrollangle" "90"
					}
				}
		})#";

		std::ofstream("csgo/materials/metallic.vmt") << R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";
		mat = create_material(shader_type_t::VertexLitGeneric, false, false);
		zmat = create_material(shader_type_t::VertexLitGeneric, true, false);

		wireframe = create_material(shader_type_t::VertexLitGeneric, false, true);

		flat = create_material(shader_type_t::UnlitGeneric, false, false);
		zflat = create_material(shader_type_t::UnlitGeneric, true, false);
		snow_mat = interfaces::material_system->find_material("snowy", nullptr);
		glow_mat = interfaces::material_system->find_material("glow_mat", nullptr);
		disco_mat = interfaces::material_system->find_material("disco", nullptr);
		animated_lines_mat = interfaces::material_system->find_material("animated_lines", nullptr);//interfaces::material_system->find_material ( "models/inventory_items/dreamhack_trophies/dreamhack_star_blur", nullptr );
		animated_mat = interfaces::material_system->find_material("animated", nullptr);
		metallic = interfaces::material_system->find_material("metallic", nullptr);
		palm_tree = interfaces::material_system->find_material("models/props/de_dust/hr_dust/foliage/palm_bark_01", nullptr);
		velvet = interfaces::material_system->find_material("models/inventory_items/trophy_majors/velvet", nullptr);

		crystal_mat = interfaces::material_system->find_material("models/inventory_items/trophy_majors/crystal_clear", nullptr);



		weapon_run_nigger = true;
	}

	static i_material* weapon_material = nullptr;
	static i_material* weapon_overlay_material = nullptr;

	switch (settings::visuals::weapon_chams_material) {


	case 0:
		weapon_material = mat;
		break;
	case 1:
		weapon_material = flat;
		break;
	case 2:
		weapon_material = animated_mat;
		break;
	case 3:
		weapon_material = crystal_mat;
		break;
	case 4:
		weapon_material = metallic;
		break;
	case 5:
		weapon_material = disco_mat;
		break;

	}
	switch (settings::visuals::weapon_overlay_material) {
	case 0:
		weapon_overlay_material = glow_mat;
		break;
	case 1:
		weapon_overlay_material = snow_mat;
		break;
	case 2:
		weapon_overlay_material = animated_lines_mat;
		break;
	}
	/*if (!(std::strstr(info.model->name, "models/weapons/v_")))
		return;

	if ((std::strstr(info.model->name, "arms")))
		return;

	if ((std::strstr(info.model->name, "sleeve")))
		return;*/


	float white[4] = { 1.f, 1.f, 1.f, 1.f };


	/*if (strstr(info.model->name, "models/weapons/") &&
		!strstr(info.model->name, "v_models") &&
		!strstr(info.model->name, "arms") &&
		!strstr(info.model->name, "sleeve") &&
		!strstr(info.model->name, "glove")) {*/

	if (settings::visuals::weapon_chams) {

		interfaces::render_view->modulate_color(settings::visuals::weapon_chams_color);
		interfaces::render_view->set_blend(settings::visuals::weapon_chams_color[3]);
		weapon_overlay_material->set_material_var_flag(material_var_ignorez, false);
		interfaces::model_render->override_material(weapon_material);
		draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
		if (settings::visuals::weapon_overlay_chams) {
			bool found = false;
			auto var = weapon_overlay_material->find_var("$envmaptint", &found);
			if (found)
				(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)var + 44))((uintptr_t)var, settings::visuals::weapon_chams_overlay_color[0], settings::visuals::weapon_chams_overlay_color[1], settings::visuals::weapon_chams_overlay_color[2]);
			interfaces::render_view->modulate_color(settings::visuals::weapon_chams_overlay_color);
			interfaces::render_view->set_blend(settings::visuals::weapon_chams_overlay_color[3]);
			weapon_overlay_material->set_material_var_flag(material_var_ignorez, false);
			weapon_overlay_material->set_material_var_flag(material_var_wireframe, settings::visuals::weapon_overlay_material == 2);
			interfaces::model_render->override_material(weapon_overlay_material);
			draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);

		}
	}

	//}


}
bool run_glove_nigger;
void visuals::chams::run_gloves(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	

}