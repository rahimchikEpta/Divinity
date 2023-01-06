#include "config.hpp"
#include "nlohmann/json_fwd.hpp"
#include <filesystem>
#include <fstream>
#include "../../features/features.hpp"
#include "../ImGui/custom.h"
static std::string path = "divinity/";
static const std::filesystem::path path_fs = "divinity";

std::map<int, const char*> weapon_names = {
{ WEAPON_AK47, "AK-47" }, { WEAPON_AUG, "AUG" },
{ WEAPON_AWP, "AWP" }, { WEAPON_CZ75A, "CZ75 Auto" },
{ WEAPON_DEAGLE, "Desert Eagle" }, { WEAPON_ELITE, "Dual Berettas" },
{ WEAPON_FAMAS, "FAMAS" }, { WEAPON_FIVESEVEN, "Five-SeveN" },
{ WEAPON_G3SG1, "G3SG1" }, { WEAPON_GALILAR, "Galil AR" },
{ WEAPON_GLOCK, "Glock-18" }, { WEAPON_M249, "M249" },
{ WEAPON_M4A1_SILENCER, "M4A1-S" }, { WEAPON_M4A1, "M4A4" },
{ WEAPON_MAC10, "MAC-10" }, { WEAPON_MAG7, "MAG-7" },
{ WEAPON_MP7, "MP7" }, { WEAPON_MP5SD, "MP5" },
{ WEAPON_MP9, "MP9" }, { WEAPON_NEGEV, "Negev" },
{ WEAPON_NOVA, "Nova" }, { WEAPON_HKP2000, "P2000" },
{ WEAPON_P250, "P250" }, { WEAPON_P90, "P90" },
{ WEAPON_BIZON, "PP-Bizon" }, { WEAPON_REVOLVER, "R8 Revolver" },
{ WEAPON_SAWEDOFF, "Sawed-Off" }, { WEAPON_SCAR20, "SCAR-20" },
{ WEAPON_SSG08, "SSG 08" }, { WEAPON_SG556, "SG 553" },
{ WEAPON_TEC9, "Tec-9" }, { WEAPON_UMP45, "UMP-45" },
{ WEAPON_USP_SILENCER, "USP-S" }, { WEAPON_XM1014, "XM1014" },
};
template <typename T>
static void read_value(const nlohmann::json& src, T& dest) {
	if (src.is_null()) return;

	dest = src.get<T>();
}

template <typename T>
static T read_value(const nlohmann::json& src) {
	if (src.is_null()) return T{};

	return src.get<T>();
}

template <typename T>
static void write_value(nlohmann::json& dest, const T& src) {
	dest = src;
}

namespace settings {
	void create_directory() {
		if (!std::filesystem::exists(path_fs)) {
			std::filesystem::create_directory(path_fs);
		}
		else {
			update_configs();
		}
		if (!std::filesystem::exists(path_lua)) {
			std::filesystem::create_directory(path_lua);
		}
	}

	void update_configs() {
		configs.clear();

		for (const auto& entry : std::filesystem::directory_iterator{ path_fs }) {
			if (!entry.is_regular_file() || entry.path().extension() != (".json")) continue;

			const auto& file_name = entry.path().stem().string();
			configs.emplace_back(file_name);
		}
	}

	void create_file(const std::string_view name) {
		if (name.empty() || name.find_first_not_of(' ') == std::string_view::npos) return;
		if (std::find(configs.cbegin(), configs.cend(), name) != configs.cend()) return;

		configs.emplace_back(name);
		save(configs.size() - 1);
	}

	void delete_file(const std::size_t index) {
		if (configs.size() <= index) return;

		path.append(configs.at(index));
		path.append((".json"));
		std::filesystem::remove(path);
		path.erase(path.size() - configs.at(index).size() - 5);

		configs.erase(configs.cbegin() + index);
		configs.shrink_to_fit();
	}

	void save(const std::size_t index) {
		nlohmann::json json;

		/// Aimbot
		for (int i = 0; i < 4; i++) {
			write_value(json["aimbot"][std::string("enabled").append(std::to_string(i))], settings::aimbot::c_enabled[i]);
			write_value(json["aimbot"][std::string("fov").append(std::to_string(i))], settings::aimbot::c_fov[i]);
			write_value(json["aimbot"][std::string("smooth").append(std::to_string(i))], settings::aimbot::c_smooth[i]);
			write_value(json["aimbot"][std::string("rcs").append(std::to_string(i))], settings::aimbot::c_rcs[i]);
		}
		write_value(json[("aimbot")][("general_hitbox0")], aimbot::general_hitboxes[0]);
		write_value(json[("aimbot")][("general_hitbox1")], aimbot::general_hitboxes[1]);
		write_value(json[("aimbot")][("general_hitbox2")], aimbot::general_hitboxes[2]);
		write_value(json[("aimbot")][("general_hitbox3")], aimbot::general_hitboxes[3]);
		write_value(json[("aimbot")][("rifle_hitbox0")], aimbot::rifle_hitboxes[0]);
		write_value(json[("aimbot")][("rifle_hitbox1")], aimbot::rifle_hitboxes[1]);
		write_value(json[("aimbot")][("rifle_hitbox2")], aimbot::rifle_hitboxes[2]);
		write_value(json[("aimbot")][("rifle_hitbox3")], aimbot::rifle_hitboxes[3]);
		write_value(json[("aimbot")][("sniper_hitbox0")], aimbot::sniper_hitboxes[0]);
		write_value(json[("aimbot")][("sniper_hitbox1")], aimbot::sniper_hitboxes[1]);
		write_value(json[("aimbot")][("sniper_hitbox2")], aimbot::sniper_hitboxes[2]);
		write_value(json[("aimbot")][("sniper_hitbox3")], aimbot::sniper_hitboxes[3]);
		write_value(json[("aimbot")][("pistol_hitbox0")], aimbot::pistol_hitboxes[0]);
		write_value(json[("aimbot")][("pistol_hitbox1")], aimbot::pistol_hitboxes[1]);
		write_value(json[("aimbot")][("pistol_hitbox2")], aimbot::pistol_hitboxes[2]);
		write_value(json[("aimbot")][("pistol_hitbox3")], aimbot::pistol_hitboxes[3]);
		write_value(json[("aimbot")][("smg_hitbox0")], aimbot::smg_hitboxes[0]);
		write_value(json[("aimbot")][("smg_hitbox1")], aimbot::smg_hitboxes[1]);
		write_value(json[("aimbot")][("smg_hitbox2")], aimbot::smg_hitboxes[2]);
		write_value(json[("aimbot")][("smg_hitbox3")], aimbot::smg_hitboxes[3]);
		write_value(json[("aimbot")][("heavy_hitbox0")], aimbot::heavy_hitboxes[0]);
		write_value(json[("aimbot")][("heavy_hitbox1")], aimbot::heavy_hitboxes[1]);
		write_value(json[("aimbot")][("heavy_hitbox2")], aimbot::heavy_hitboxes[2]);
		write_value(json[("aimbot")][("heavy_hitbox3")], aimbot::heavy_hitboxes[3]);
		write_value(json[("aimbot")][("scoping_required")], aimbot::scoping_required);
		write_value(json[("aimbot")][("visible_only")], aimbot::visible_only);
		write_value(json[("aimbot")][("aim_key")], aimbot::aim_key);
		write_value(json[("aimbot")][("silent")], aimbot::silent);
		write_value(json[("aimbot")][("target_in_air")], aimbot::target_in_air);
		write_value(json[("aimbot")][("target_smoke")], aimbot::target_smoke);
		write_value(json[("aimbot")][("target_team")], aimbot::target_team);
		write_value(json[("aimbot")][("target_visible")], aimbot::target_visible);
		write_value(json[("aimbot")][("type")], aimbot::type);
		write_value(json[("aimbot")][("auto_pistol")], aimbot::auto_pistol);
		write_value(json[("aimbot")][("delay_after_kill")], aimbot::delay_after_kill);

		write_value(json[("aimbot")][("backtrack_enable")], aimbot::backtrack_enable);
		write_value(json[("aimbot")][("backtrack_ping_spike")], aimbot::backtrack_ping_spike);
		write_value(json[("aimbot")][("backtrack_ping_spike_value")], aimbot::backtrack_ping_spike_value);

		/// Visuals
		write_value(json[("visuals")][("enabled")], visuals::enable_esp);
		write_value(json[("visuals")][("dormant_esp")], visuals::dormant_esp);
		write_value(json[("visuals")][("visuals_box")], visuals::visuals_box);
		write_value(json[("visuals")][("box_lines[0]")], visuals::box_lines[0]);
		write_value(json[("visuals")][("box_lines[1]")], visuals::box_lines[1]);
		write_value(json[("visuals")][("box_filled_amount")], visuals::box_vertical);
		write_value(json[("visuals")][("box_type")], visuals::box_type);
		write_value(json[("visuals")][("visuals_name")], visuals::visuals_name);
		for (int i = 0; i < 128; i++) {
			write_value(json[("visuals")][("visuals_bot_extention_name")], visuals::visuals_bot_extention_name[i]);
		}
		write_value(json[("visuals")][("visuals_health")], visuals::visuals_health);
		write_value(json[("visuals")][("gradient_healthbar")], visuals::gradient_healthbar);
		write_value(json[("visuals")][("gradient_healthbar_col1[0]")], visuals::gradient_healthbar_col1[0]);
		write_value(json[("visuals")][("gradient_healthbar_col1[1]")], visuals::gradient_healthbar_col1[1]);
		write_value(json[("visuals")][("gradient_healthbar_col1[2]")], visuals::gradient_healthbar_col1[2]);
		write_value(json[("visuals")][("gradient_healthbar_col1[3]")], visuals::gradient_healthbar_col1[3]);
		write_value(json[("visuals")][("gradient_healthbar_col2[0]")], visuals::gradient_healthbar_col2[0]);
		write_value(json[("visuals")][("gradient_healthbar_col2[1]")], visuals::gradient_healthbar_col2[1]);
		write_value(json[("visuals")][("gradient_healthbar_col2[2]")], visuals::gradient_healthbar_col2[2]);
		write_value(json[("visuals")][("gradient_healthbar_col2[3]")], visuals::gradient_healthbar_col2[3]);
		write_value(json[("visuals")][("visuals_armor")], visuals::visuals_armor);
		write_value(json[("visuals")][("engine_radar")], visuals::engine_radar);
		write_value(json[("visuals")][("flash_kill")], visuals::flash_kill);
		write_value(json[("visuals")][("visuals_flags")], visuals::visuals_flags);
		write_value(json[("visuals")][("visuals_flags_uppercase")], visuals::visuals_flags_uppercase);
		write_value(json[("visuals")][("visuals_weapon")], visuals::visuals_weapon);
		write_value(json[("visuals")][("visuals_skeleton")], visuals::visuals_skeleton);
		write_value(json[("visuals")][("spectator_list")], visuals::spectator_list);
		write_value(json[("visuals")][("spectator_list_pos_x")], visuals::spectator_list_pos_x);
		write_value(json[("visuals")][("spectator_list_pos_y")], visuals::spectator_list_pos_y);
		write_value(json[("visuals")][("enable_chams")], visuals::enable_chams);
		write_value(json[("visuals")][("enable_chams_vis")], visuals::enable_chams_vis);
		write_value(json[("visuals")][("enable_chams_invis")], visuals::enable_chams_invis);
		write_value(json[("visuals")][("chams_overlay")], visuals::chams_overlay);
		write_value(json[("visuals")][("arm_chams")], visuals::arm_chams);
		write_value(json[("visuals")][("arm_chams_material")], visuals::arm_chams_material);
		write_value(json[("visuals")][("arm_chams_color[0]")], visuals::arm_chams_color[0]);
		write_value(json[("visuals")][("arm_chams_color[1]")], visuals::arm_chams_color[1]);
		write_value(json[("visuals")][("arm_chams_color[2]")], visuals::arm_chams_color[2]);
		write_value(json[("visuals")][("arm_chams_color[3]")], visuals::arm_chams_color[3]);
		write_value(json[("visuals")][("arm_overlay_chams")], visuals::arm_overlay_chams);
		write_value(json[("visuals")][("arm_overlay_material")], visuals::arm_overlay_material);
		write_value(json[("visuals")][("arm_overlay_color[0]")], visuals::arm_overlay_color[0]);
		write_value(json[("visuals")][("arm_overlay_color[1]")], visuals::arm_overlay_color[1]);
		write_value(json[("visuals")][("arm_overlay_color[2]")], visuals::arm_overlay_color[2]);
		write_value(json[("visuals")][("arm_overlay_color[3]")], visuals::arm_overlay_color[3]);
		write_value(json[("visuals")][("weapon_chams")], visuals::weapon_chams);
		write_value(json[("visuals")][("weapon_chams_material")], visuals::weapon_chams_material);
		write_value(json[("visuals")][("weapon_chams_color[0]")], visuals::weapon_chams_color[0]);
		write_value(json[("visuals")][("weapon_chams_color[1]")], visuals::weapon_chams_color[1]);
		write_value(json[("visuals")][("weapon_chams_color[2]")], visuals::weapon_chams_color[2]);
		write_value(json[("visuals")][("weapon_chams_color[3]")], visuals::weapon_chams_color[3]);
		write_value(json[("visuals")][("weapon_overlay_chams")], visuals::weapon_overlay_chams);
		write_value(json[("visuals")][("weapon_overlay_material")], visuals::weapon_overlay_material);
		write_value(json[("visuals")][("weapon_chams_overlay_color[0]")], visuals::weapon_chams_overlay_color[0]);
		write_value(json[("visuals")][("weapon_chams_overlay_color[1]")], visuals::weapon_chams_overlay_color[1]);
		write_value(json[("visuals")][("weapon_chams_overlay_color[2]")], visuals::weapon_chams_overlay_color[2]);
		write_value(json[("visuals")][("weapon_chams_overlay_color[3]")], visuals::weapon_chams_overlay_color[3]);
		write_value(json[("visuals")][("player_material")], visuals::player_material);
		write_value(json[("visuals")][("player_material_invis")], visuals::player_material_invis);
		write_value(json[("visuals")][("player_overlay_material")], visuals::player_overlay_material);
		write_value(json[("visuals")][("backtrack_chams_type")], visuals::backtrack_chams_type);
		write_value(json[("visuals")][("chams_vis[0]")], visuals::chams_vis[0]);
		write_value(json[("visuals")][("chams_vis[1]")], visuals::chams_vis[1]);
		write_value(json[("visuals")][("chams_vis[2]")], visuals::chams_vis[2]);
		write_value(json[("visuals")][("chams_vis[3]")], visuals::chams_vis[3]);
		write_value(json[("visuals")][("chams_invis[0]")], visuals::chams_invis[0]);
		write_value(json[("visuals")][("chams_invis[1]")], visuals::chams_invis[1]);
		write_value(json[("visuals")][("chams_invis[2]")], visuals::chams_invis[2]);
		write_value(json[("visuals")][("chams_invis[3]")], visuals::chams_invis[3]);
		write_value(json[("visuals")][("chams_overlay_clr[0]")], visuals::chams_overlay_clr[0]);
		write_value(json[("visuals")][("chams_overlay_clr[1]")], visuals::chams_overlay_clr[1]);
		write_value(json[("visuals")][("chams_overlay_clr[2]")], visuals::chams_overlay_clr[2]);
		write_value(json[("visuals")][("chams_overlay_clr[3]")], visuals::chams_overlay_clr[3]);
		write_value(json[("visuals")][("backtrack_chams")], visuals::backtrack_chams);
		write_value(json[("visuals")][("backtrack_render_type")], visuals::backtrack_render_type);
		write_value(json[("visuals")][("backtrack_chams_color[0]")], visuals::backtrack_chams_color[0]);
		write_value(json[("visuals")][("backtrack_chams_color[1]")], visuals::backtrack_chams_color[1]);
		write_value(json[("visuals")][("backtrack_chams_color[2]")], visuals::backtrack_chams_color[2]);
		write_value(json[("visuals")][("backtrack_chams_color[3]")], visuals::backtrack_chams_color[3]);
		write_value(json[("visuals")][("backtrack_chams_color2[0]")], visuals::backtrack_chams_color2[0]);
		write_value(json[("visuals")][("backtrack_chams_color2[1]")], visuals::backtrack_chams_color2[1]);
		write_value(json[("visuals")][("backtrack_chams_color2[2]")], visuals::backtrack_chams_color2[2]);
		write_value(json[("visuals")][("backtrack_chams_color2[3]")], visuals::backtrack_chams_color2[3]);
		write_value(json[("visuals")][("glow_enabled")], visuals::glow_enabled);
		write_value(json[("visuals")][("glow_players")], visuals::glow_players);
		write_value(json[("visuals")][("glow_bombs")], visuals::glow_bombs);
		write_value(json[("visuals")][("glow_chickens")], visuals::glow_chickens);
		write_value(json[("visuals")][("glow_fish")], visuals::glow_fish);
		write_value(json[("visuals")][("glow_type_player")], visuals::glow_type_player);
		write_value(json[("visuals")][("glow_type_bomb")], visuals::glow_type_bomb);
		write_value(json[("visuals")][("glow_type_chicken")], visuals::glow_type_chicken);
		write_value(json[("visuals")][("glow_type_weapons")], visuals::glow_type_weapons);
		write_value(json[("visuals")][("glow_weapons_col[0]")], visuals::glow_weapons_col[0]);
		write_value(json[("visuals")][("glow_weapons_col[1]")], visuals::glow_weapons_col[1]);
		write_value(json[("visuals")][("glow_weapons_col[2]")], visuals::glow_weapons_col[2]);
		write_value(json[("visuals")][("glow_weapons_col[3]")], visuals::glow_weapons_col[3]);
		write_value(json[("visuals")][("glow_type_fish")], visuals::glow_type_fish);
		write_value(json[("visuals")][("glow_player_col[0]")], visuals::glow_player_col[0]);
		write_value(json[("visuals")][("glow_player_col[1]")], visuals::glow_player_col[1]);
		write_value(json[("visuals")][("glow_player_col[2]")], visuals::glow_player_col[2]);
		write_value(json[("visuals")][("glow_player_col[3]")], visuals::glow_player_col[3]);
		write_value(json[("visuals")][("glow_player_invis_col[0]")], visuals::glow_player_invis_col[0]);
		write_value(json[("visuals")][("glow_player_invis_col[1]")], visuals::glow_player_invis_col[1]);
		write_value(json[("visuals")][("glow_player_invis_col[2]")], visuals::glow_player_invis_col[2]);
		write_value(json[("visuals")][("glow_player_invis_col[3]")], visuals::glow_player_invis_col[3]);
		write_value(json[("visuals")][("glow_bomb_col[0]")], visuals::glow_bomb_col[0]);
		write_value(json[("visuals")][("glow_bomb_col[1]")], visuals::glow_bomb_col[1]);
		write_value(json[("visuals")][("glow_bomb_col[2]")], visuals::glow_bomb_col[2]);
		write_value(json[("visuals")][("glow_bomb_col[3]")], visuals::glow_bomb_col[3]);
		write_value(json[("visuals")][("glow_chicken_col[0]")], visuals::glow_chicken_col[0]);
		write_value(json[("visuals")][("glow_chicken_col[1]")], visuals::glow_chicken_col[1]);
		write_value(json[("visuals")][("glow_chicken_col[2]")], visuals::glow_chicken_col[2]);
		write_value(json[("visuals")][("glow_chicken_col[3]")], visuals::glow_chicken_col[3]);
		write_value(json[("visuals")][("glow_fish_col[0]")], visuals::glow_fish_col[0]);
		write_value(json[("visuals")][("glow_fish_col[1]")], visuals::glow_fish_col[1]);
		write_value(json[("visuals")][("glow_fish_col[2]")], visuals::glow_fish_col[2]);
		write_value(json[("visuals")][("glow_fish_col[3]")], visuals::glow_fish_col[3]);
		write_value(json[("visuals")][("world_modulation")], visuals::world_modulation);
		write_value(json[("visuals")][("world_modulation_clr[0]")], visuals::world_modulation_clr[0]);
		write_value(json[("visuals")][("world_modulation_clr[1]")], visuals::world_modulation_clr[1]);
		write_value(json[("visuals")][("world_modulation_clr[2]")], visuals::world_modulation_clr[2]);
		write_value(json[("visuals")][("world_modulation_clr[3]")], visuals::world_modulation_clr[3]);
		write_value(json[("visuals")][("prop_modulation")], visuals::prop_modulation);
		write_value(json[("visuals")][("prop_modulation_clr[0]")], visuals::prop_modulation_clr[0]);
		write_value(json[("visuals")][("prop_modulation_clr[1]")], visuals::prop_modulation_clr[1]);
		write_value(json[("visuals")][("prop_modulation_clr[2]")], visuals::prop_modulation_clr[2]);
		write_value(json[("visuals")][("prop_modulation_clr[3]")], visuals::prop_modulation_clr[3]);
		write_value(json[("visuals")][("hitsound")], visuals::hitsound);
		write_value(json[("visuals")][("killsound")], visuals::killsound);
		write_value(json[("visuals")][("visuals_disableflash")], visuals::visuals_disableflash);
		write_value(json[("visuals")][("visuals_disable_panorama_blur")], visuals::visuals_disable_panorama_blur);
		write_value(json[("visuals")][("fog")], visuals::fog);
		write_value(json[("visuals")][("fog_start")], visuals::fog_start);
		write_value(json[("visuals")][("fog_end")], visuals::fog_end);
		write_value(json[("visuals")][("fog_density")], visuals::fog_density);
		write_value(json[("visuals")][("fog_clr[0]")], visuals::fog_clr[0]);
		write_value(json[("visuals")][("fog_clr[1]")], visuals::fog_clr[1]);
		write_value(json[("visuals")][("fog_clr[2]")], visuals::fog_clr[2]);
		write_value(json[("visuals")][("fog_clr[3]")], visuals::fog_clr[3]);
		write_value(json[("visuals")][("skybox_changer_index")], visuals::skybox_changer_index);
		write_value(json[("visuals")][("force_crosshair")], visuals::force_crosshair);
		write_value(json[("visuals")][("hit_effect")], visuals::hit_effect);
		write_value(json[("visuals")][("modulate_molotov")], visuals::modulate_molotov);
		write_value(json[("visuals")][("wireframe_fire")], visuals::wireframe_fire);
		write_value(json[("visuals")][("modulate_molotov_clr[0]")], visuals::modulate_molotov_clr[0]);
		write_value(json[("visuals")][("modulate_molotov_clr[1]")], visuals::modulate_molotov_clr[1]);
		write_value(json[("visuals")][("modulate_molotov_clr[2]")], visuals::modulate_molotov_clr[2]);
		write_value(json[("visuals")][("modulate_molotov_clr[3]")], visuals::modulate_molotov_clr[3]);

		write_value(json[("visuals")][("viewmodel")], visuals::viewmodel);
		write_value(json[("visuals")][("viewmodel_offset_x")], visuals::viewmodel_offset_x);
		write_value(json[("visuals")][("viewmodel_offset_y")], visuals::viewmodel_offset_y);
		write_value(json[("visuals")][("viewmodel_offset_z")], visuals::viewmodel_offset_z);

		write_value(json[("visuals")][("modulate_smoke")], visuals::modulate_smoke);
		write_value(json[("visuals")][("wireframe_smoke")], visuals::wireframe_smoke);
		write_value(json[("visuals")][("modulate_smoke_clr[0]")], visuals::modulate_smoke_clr[0]);
		write_value(json[("visuals")][("modulate_smoke_clr[1]")], visuals::modulate_smoke_clr[1]);
		write_value(json[("visuals")][("modulate_smoke_clr[2]")], visuals::modulate_smoke_clr[2]);
		write_value(json[("visuals")][("modulate_smoke_clr[3]")], visuals::modulate_smoke_clr[3]);

		write_value(json[("visuals")][("modulate_blood")], visuals::modulate_blood);
		write_value(json[("visuals")][("modulate_blood_clr[0]")], visuals::modulate_blood_clr[0]);
		write_value(json[("visuals")][("modulate_blood_clr[1]")], visuals::modulate_blood_clr[1]);
		write_value(json[("visuals")][("modulate_blood_clr[2]")], visuals::modulate_blood_clr[2]);
		write_value(json[("visuals")][("modulate_blood_clr[3]")], visuals::modulate_blood_clr[3]);

		write_value(json[("visuals")][("modulate_weapon_effects")], visuals::modulate_weapon_effects);
		write_value(json[("visuals")][("modulate_weapon_effects_clr[0]")], visuals::modulate_weapon_effects_clr[0]);
		write_value(json[("visuals")][("modulate_weapon_effects_clr[1]")], visuals::modulate_weapon_effects_clr[1]);
		write_value(json[("visuals")][("modulate_weapon_effects_clr[2]")], visuals::modulate_weapon_effects_clr[2]);
		write_value(json[("visuals")][("modulate_weapon_effects_clr[3]")], visuals::modulate_weapon_effects_clr[3]);

		write_value(json[("visuals")][("grenade_prediction")], visuals::grenade_prediction);
		write_value(json[("visuals")][("grenade_prediction_clr[0]")], visuals::grenade_prediction_clr[0]);
		write_value(json[("visuals")][("grenade_prediction_clr[1]")], visuals::grenade_prediction_clr[1]);
		write_value(json[("visuals")][("grenade_prediction_clr[2]")], visuals::grenade_prediction_clr[2]);
		write_value(json[("visuals")][("grenade_prediction_clr[3]")], visuals::grenade_prediction_clr[3]);

		write_value(json[("visuals")][("oof_arrows")], visuals::oof_arrows);
		write_value(json[("visuals")][("oof_arrows")], visuals::oof_arrows);
		write_value(json[("visuals")][("oof_arrow_range")], visuals::oof_arrow_range);
		write_value(json[("visuals")][("oof_arrow_size")], visuals::oof_arrow_size);
		write_value(json[("visuals")][("oof_arrow_col1[0]")], visuals::oof_arrow_col1[0]);
		write_value(json[("visuals")][("oof_arrow_col1[1]")], visuals::oof_arrow_col1[1]);
		write_value(json[("visuals")][("oof_arrow_col1[2]")], visuals::oof_arrow_col1[2]);
		write_value(json[("visuals")][("oof_arrow_col1[3]")], visuals::oof_arrow_col1[3]);
		write_value(json[("visuals")][("oof_arrow_col2[0]")], visuals::oof_arrow_col2[0]);
		write_value(json[("visuals")][("oof_arrow_col2[1]")], visuals::oof_arrow_col2[1]);
		write_value(json[("visuals")][("oof_arrow_col2[2]")], visuals::oof_arrow_col2[2]);
		write_value(json[("visuals")][("oof_arrow_col2[3]")], visuals::oof_arrow_col2[3]);
		write_value(json[("visuals")][("oof_arrow_col3[0]")], visuals::oof_arrow_col3[0]);
		write_value(json[("visuals")][("oof_arrow_col3[1]")], visuals::oof_arrow_col3[1]);
		write_value(json[("visuals")][("oof_arrow_col3[2]")], visuals::oof_arrow_col3[2]);
		write_value(json[("visuals")][("oof_arrow_col3[3]")], visuals::oof_arrow_col3[3]);


		write_value(json[("motion_blur")][("enabled")], motion_blur::enabled);
		write_value(json[("motion_blur")][("foward_blur")], motion_blur::foward_blur);
		write_value(json[("motion_blur")][("rotation_intensity")], motion_blur::rotation_intensity);
		write_value(json[("motion_blur")][("blur_strength")], motion_blur::blur_strength);
		write_value(json[("motion_blur")][("falling_min")], motion_blur::falling_min);
		write_value(json[("motion_blur")][("falling_max")], motion_blur::falling_max);


		/// Visualisation
		write_value(json[("visualisation")][("visualize_eb")], visulisation::visualize_eb);
		write_value(json[("visualisation")][("text_on_eb")], visulisation::text_on_eb);
		write_value(json[("visualisation")][("text_on_eb_col[0]")], visulisation::text_on_eb_col[0]);
		write_value(json[("visualisation")][("text_on_eb_col[1]")], visulisation::text_on_eb_col[1]);
		write_value(json[("visualisation")][("text_on_eb_col[2]")], visulisation::text_on_eb_col[2]);
		write_value(json[("visualisation")][("text_on_eb_col[3]")], visulisation::text_on_eb_col[3]);
		write_value(json[("visualisation")][("effect_on_eb")], visulisation::effect_on_eb);
		write_value(json[("visualisation")][("effect_on_eb_col[0]")], visulisation::effect_on_eb_col[0]);
		write_value(json[("visualisation")][("effect_on_eb_col[1]")], visulisation::effect_on_eb_col[1]);
		write_value(json[("visualisation")][("effect_on_eb_col[2]")], visulisation::effect_on_eb_col[2]);
		write_value(json[("visualisation")][("effect_on_eb_col[3]")], visulisation::effect_on_eb_col[3]);
		write_value(json[("visualisation")][("circle_on_eb")], visulisation::circle_on_eb);
		write_value(json[("visualisation")][("circle_on_eb_col[0]")], visulisation::circle_on_eb_col[0]);
		write_value(json[("visualisation")][("circle_on_eb_col[1]")], visulisation::circle_on_eb_col[1]);
		write_value(json[("visualisation")][("circle_on_eb_col[2]")], visulisation::circle_on_eb_col[2]);
		write_value(json[("visualisation")][("circle_on_eb_col[3]")], visulisation::circle_on_eb_col[3]);
		write_value(json[("visualisation")][("cube_on_eb")], visulisation::cube_on_eb);
		write_value(json[("visualisation")][("cube_on_eb_col[0]")], visulisation::cube_on_eb_col[0]);
		write_value(json[("visualisation")][("cube_on_eb_col[1]")], visulisation::cube_on_eb_col[1]);
		write_value(json[("visualisation")][("cube_on_eb_col[2]")], visulisation::cube_on_eb_col[2]);
		write_value(json[("visualisation")][("cube_on_eb_col[3]")], visulisation::cube_on_eb_col[3]);
		write_value(json[("visualisation")][("dlight_on_eb")], visulisation::dlight_on_eb);
		write_value(json[("visualisation")][("dlight_on_eb_col[0]")], visulisation::dlight_on_eb_col[0]);
		write_value(json[("visualisation")][("dlight_on_eb_col[1]")], visulisation::dlight_on_eb_col[1]);
		write_value(json[("visualisation")][("dlight_on_eb_col[2]")], visulisation::dlight_on_eb_col[2]);
		write_value(json[("visualisation")][("dlight_on_eb_col[3]")], visulisation::dlight_on_eb_col[3]);
		write_value(json[("visualisation")][("dlight_on_eb_brightness")], visulisation::dlight_on_eb_brightness);
		write_value(json[("visualisation")][("dlight_on_eb_radius")], visulisation::dlight_on_eb_radius);
		write_value(json[("visualisation")][("vis_eb_fadeout_time")], visulisation::vis_eb_fadeout_time);
		write_value(json[("visualisation")][("predicted_circle_on_eb")], visulisation::predicted_circle_on_eb);
		write_value(json[("visualisation")][("predicted_eb_circle_col[0]")], visulisation::predicted_circle_on_eb_col[0]);
		write_value(json[("visualisation")][("predicted_eb_circle_col[1]")], visulisation::predicted_circle_on_eb_col[1]);
		write_value(json[("visualisation")][("predicted_eb_circle_col[2]")], visulisation::predicted_circle_on_eb_col[2]);
		write_value(json[("visualisation")][("predicted_eb_circle_col[3]")], visulisation::predicted_circle_on_eb_col[3]);
		write_value(json[("visualisation")][("predicted_eb_circle2d")], visulisation::predicted_eb_circle2d);
		write_value(json[("visualisation")][("predicted_eb_circle2d_col[0]")], visulisation::predicted_eb_circle2d_col[0]);
		write_value(json[("visualisation")][("predicted_eb_circle2d_col[1]")], visulisation::predicted_eb_circle2d_col[1]);
		write_value(json[("visualisation")][("predicted_eb_circle2d_col[2]")], visulisation::predicted_eb_circle2d_col[2]);
		write_value(json[("visualisation")][("predicted_eb_circle2d_col[3]")], visulisation::predicted_eb_circle2d_col[3]);
		write_value(json[("visuals")][("sparks_while_eb")], visulisation::sparks_while_eb);
		write_value(json[("visuals")][("lightning_while_eb")], visulisation::lightning_while_eb);

		/// Indicators
		write_value(json[("indicators")][("velo")], indicators::velo);
		write_value(json[("indicators")][("graph_velocity")], indicators::graph_velocity);
		write_value(json[("indicators")][("velo_graph_size")], indicators::velo_graph_size);
		write_value(json[("indicators")][("velo_graph_scale")], indicators::velo_graph_scale);
		write_value(json[("indicators")][("velo_graph_size")], indicators::velo_graph_size);
		write_value(json[("indicators")][("velo_alpha_type")], indicators::velo_alpha_type);
		write_value(json[("indicators")][("velo_color_type")], indicators::velo_color_type);
		write_value(json[("indicators")][("velo_takeoff")], indicators::velo_takeoff);
		write_value(json[("indicators")][("graph_position")], indicators::graph_position);
		write_value(json[("indicators")][("graph_thickness")], indicators::graph_thickness);
		write_value(json[("indicators")][("graph_jump_velo")], indicators::graph_jump_velo);
		write_value(json[("indicators")][("graph_land_velo")], indicators::graph_land_velo);
		write_value(json[("indicators")][("graph_eb")], indicators::graph_eb);
		write_value(json[("indicators")][("graph_bold_font")], indicators::graph_bold_font);
		write_value(json[("indicators")][("graph_jb")], indicators::graph_jb);
		write_value(json[("indicators")][("position")], indicators::position);
		write_value(json[("indicators")][("velo_graph_style")], indicators::velo_graph_style);
		write_value(json[("indicators")][("stamina_graph_style")], indicators::stamina_graph_style);
		write_value(json[("indicators")][("graph_velo_col[0]")], indicators::graph_velo_col[0]);
		write_value(json[("indicators")][("graph_velo_col[1]")], indicators::graph_velo_col[1]);
		write_value(json[("indicators")][("graph_velo_col[2]")], indicators::graph_velo_col[2]);
		write_value(json[("indicators")][("graph_velo_col[3]")], indicators::graph_velo_col[3]);
		write_value(json[("indicators")][("stamina")], indicators::stamina);
		write_value(json[("indicators")][("graph_stamina")], indicators::graph_stamina);
		write_value(json[("indicators")][("graph_stamina_col[0]")], indicators::graph_stamina_col[0]);
		write_value(json[("indicators")][("graph_stamina_col[1]")], indicators::graph_stamina_col[1]);
		write_value(json[("indicators")][("graph_stamina_col[2]")], indicators::graph_stamina_col[2]);
		write_value(json[("indicators")][("graph_stamina_col[3]")], indicators::graph_stamina_col[3]);
		write_value(json[("indicators")][("ej")], indicators::ej);
		write_value(json[("indicators")][("jb")], indicators::jb);
		write_value(json[("indicators")][("eb")], indicators::eb);
		write_value(json[("indicators")][("lb")], indicators::lb);
		write_value(json[("indicators")][("ps_default_col[0]")], indicators::ps_default_col[0]);
		write_value(json[("indicators")][("ps_default_col[1]")], indicators::ps_default_col[1]);
		write_value(json[("indicators")][("ps_default_col[2]")], indicators::ps_default_col[2]);
		write_value(json[("indicators")][("ps_default_col[3]")], indicators::ps_default_col[3]);
		write_value(json[("indicators")][("ps_rage_col[0]")], indicators::ps_rage_col[0]);
		write_value(json[("indicators")][("ps_rage_col[1]")], indicators::ps_rage_col[1]);
		write_value(json[("indicators")][("ps_rage_col[2]")], indicators::ps_rage_col[2]);
		write_value(json[("indicators")][("ps_rage_col[3]")], indicators::ps_rage_col[3]);
		write_value(json[("indicators")][("lb_default_col[0]")], indicators::lb_default_col[0]);
		write_value(json[("indicators")][("lb_default_col[1]")], indicators::lb_default_col[1]);
		write_value(json[("indicators")][("lb_default_col[2]")], indicators::lb_default_col[2]);
		write_value(json[("indicators")][("lb_default_col[3]")], indicators::lb_default_col[3]);
		write_value(json[("indicators")][("lb_rage_col[0]")], indicators::lb_rage_col[0]);
		write_value(json[("indicators")][("lb_rage_col[1]")], indicators::lb_rage_col[1]);
		write_value(json[("indicators")][("lb_rage_col[2]")], indicators::lb_rage_col[2]);
		write_value(json[("indicators")][("lb_rage_col[3]")], indicators::lb_rage_col[3]);
		write_value(json[("indicators")][("velo_col[0]")], indicators::velo_col[0]);
		write_value(json[("indicators")][("velo_col[1]")], indicators::velo_col[1]);
		write_value(json[("indicators")][("velo_col[2]")], indicators::velo_col[2]);
		write_value(json[("indicators")][("velo_col[3]")], indicators::velo_col[3]);
		write_value(json[("indicators")][("ej_col[0]")], indicators::ej_col[0]);
		write_value(json[("indicators")][("ej_col[1]")], indicators::ej_col[1]);
		write_value(json[("indicators")][("ej_col[2]")], indicators::ej_col[2]);
		write_value(json[("indicators")][("ej_col[3]")], indicators::ej_col[3]);
		write_value(json[("indicators")][("ej_rage_col[0]")], indicators::ej_rage_col[0]);
		write_value(json[("indicators")][("ej_rage_col[1]")], indicators::ej_rage_col[1]);
		write_value(json[("indicators")][("ej_rage_col[2]")], indicators::ej_rage_col[2]);
		write_value(json[("indicators")][("ej_rage_col[3]")], indicators::ej_rage_col[3]);
		write_value(json[("indicators")][("jb_col[0]")], indicators::jb_col[0]);
		write_value(json[("indicators")][("jb_col[1]")], indicators::jb_col[1]);
		write_value(json[("indicators")][("jb_col[2]")], indicators::jb_col[2]);
		write_value(json[("indicators")][("jb_col[3]")], indicators::jb_col[3]);
		write_value(json[("indicators")][("jb_rage_col[0]")], indicators::jb_rage_col[0]);
		write_value(json[("indicators")][("jb_rage_col[1]")], indicators::jb_rage_col[1]);
		write_value(json[("indicators")][("jb_rage_col[2]")], indicators::jb_rage_col[2]);
		write_value(json[("indicators")][("jb_rage_col[3]")], indicators::jb_rage_col[3]);
		write_value(json[("indicators")][("stamina_col[0]")], indicators::stamina_col[0]);
		write_value(json[("indicators")][("stamina_col[1]")], indicators::stamina_col[1]);
		write_value(json[("indicators")][("stamina_col[2]")], indicators::stamina_col[2]);
		write_value(json[("indicators")][("stamina_col[3]")], indicators::stamina_col[3]);
		write_value(json[("indicators")][("eb_default_col[0]")], indicators::eb_default_col[0]);
		write_value(json[("indicators")][("eb_default_col[1]")], indicators::eb_default_col[1]);
		write_value(json[("indicators")][("eb_default_col[2]")], indicators::eb_default_col[2]);
		write_value(json[("indicators")][("eb_default_col[3]")], indicators::eb_default_col[3]);
		write_value(json[("indicators")][("eb_rage_col[0]")], indicators::eb_rage_col[0]);
		write_value(json[("indicators")][("eb_rage_col[1]")], indicators::eb_rage_col[1]);
		write_value(json[("indicators")][("eb_rage_col[2]")], indicators::eb_rage_col[2]);
		write_value(json[("indicators")][("eb_rage_col[3]")], indicators::eb_rage_col[3]);
		write_value(json[("indicators")][("stamina_alpha_type")], indicators::stamina_alpha_type);
		write_value(json[("indicators")][("round_stamina")], indicators::round_stamina);
		write_value(json[("indicators")][("stamina_takeoff")], indicators::stamina_takeoff);
		write_value(json[("indicators")][("key_strokes")], indicators::key_strokes);
		write_value(json[("indicators")][("key_strokes_fadeout")], indicators::key_strokes_fadeout);
		write_value(json[("indicators")][("key_strokes_stances")], indicators::key_strokes_stances);
		write_value(json[("indicators")][("key_strokes_position")], indicators::key_strokes_position);
		/// Edgebug effects
		write_value(json[("eb_effects")][("print_eb_info")], edgebug_effects::print_eb_info);
		write_value(json[("eb_effects")][("print_eb_info_print_type")], edgebug_effects::print_eb_info_print_type);
		write_value(json[("eb_effects")][("healthboost_on_eb")], edgebug_effects::healthboost_on_eb);
		write_value(json[("eb_effects")][("sound_on_eb")], edgebug_effects::sound_on_eb);
		write_value(json[("eb_effects")][("eb_effect")], edgebug_effects::eb_effect);
		write_value(json[("eb_effects")][("effect_on_eb")], edgebug_effects::effect_on_eb);
		write_value(json[("eb_effects")][("effect_time")], edgebug_effects::effect_time);
		
		/// Movement
		write_value(json[("movement")][("bhop")], movement::bhop);
		write_value(json[("movement")][("bhop_hitchance")], movement::bhop_chance);
		write_value(json[("movement")][("edgebug")], movement::edgebug);
		write_value(json[("movement")][("jumpbug")], movement::jumpbug);
		write_value(json[("movement")][("edgebug_key")], movement::edgebug_key);
		write_value(json[("movement")][("jumpbug_key")], movement::jumpbug_key);
		write_value(json[("movement")][("ladder_bug")], movement::ladder_bug);
		write_value(json[("movement")][("ladder_bug_key")], movement::ladder_bug_key);
		write_value(json[("movement")][("edgebug_scan_crouch_after_x_pred")], movement::edgebug_scan_crouch_after_x_pred);
		write_value(json[("movement")][("edgebug_scan_strength")], movement::edgebug_scan_strength);
		write_value(json[("movement")][("edgebug_scan_time")], movement::edgebug_scan_time);
		write_value(json[("movement")][("edgebug_scan_type")], movement::edgebug_scan_type);
		write_value(json[("movement")][("lock_pull_amount")], movement::lock_pull_amount);
		write_value(json[("movement")][("lock_on_eb")], movement::lock_on_eb);
		write_value(json[("movement")][("lock_pitch")], movement::lock_pitch);
		write_value(json[("movement")][("edgejump")], movement::edgejump);
		write_value(json[("movement")][("edgejump_key")], movement::edgejump_key);
		write_value(json[("movement")][("pixelsurf")], movement::pixelsurf);
		write_value(json[("movement")][("pixelsurf_key")], movement::pixelsurf_key);
		write_value(json[("movement")][("jumpshot_info")], movement::jumpshot_info);
		write_value(json[("movement")][("jumpshot_info_print_type")], movement::jumpshot_info_print_type);
		write_value(json[("movement")][("jumpshot_slurs")], movement::jumpshot_slurs);
		write_value(json[("movement")][("no_duck_cooldown")], movement::no_duck_cooldown);
		write_value(json[("movement")][("longjump")], movement::longjump);
		write_value(json[("movement")][("strafe_assistance")], movement::strafe_assistance);
		write_value(json[("movement")][("simulate_as")], movement::simulate_as);
		write_value(json[("movement")][("strafe_optimiser")], movement::strafe_optimiser);
		write_value(json[("movement")][("strafe_optimiser_on_key")], movement::strafe_optimiser_on_key);
		write_value(json[("movement")][("strafe_optimiser_key")], movement::strafe_optimiser_key);
		write_value(json[("movement")][("desired_gain")], movement::desired_gain);
		write_value(json[("movement")][("required_speed")], movement::required_speed);
		/// Misc
		write_value(json[("misc")][("anti_afk")], misc::anti_afk);
		write_value(json[("misc")][("reveal_ranks")], misc::reveal_ranks);
		write_value(json[("misc")][("settings_unsafe_features")], misc::settings_unsafe_features);
		write_value(json[("misc")][("custom_netgraph")], misc::custom_netgraph);
		write_value(json[("misc")][("modulate_netgraph")], misc::modulate_netgraph);
		write_value(json[("misc")][("aspect_ratio_changer")], misc::aspect_ratio_changer);
		write_value(json[("misc")][("aspect_ratio_changer_value")], misc::aspect_ratio_changer_value);
		write_value(json[("trails")][("enable")], misc::trails::enable);
		write_value(json[("trails")][("color_type")], misc::trails::color_type);
		write_value(json[("trails")][("rainbow_speed")], misc::trails::rainbow_speed);
		write_value(json[("trails")][("life")], misc::trails::life);
		write_value(json[("trails")][("width")], misc::trails::width);
		write_value(json[("trails")][("life_speed")], misc::trails::life_speed);
		write_value(json[("trails")][("col[0]")], misc::trails::col[0]);
		write_value(json[("trails")][("col[1]")], misc::trails::col[1]);
		write_value(json[("trails")][("col[2]")], misc::trails::col[2]);
		write_value(json[("trails")][("col[3]")], misc::trails::col[3]);
		write_value(json[("trails")][("fluc_col1[0]")], misc::trails::fluc_col1[0]);
		write_value(json[("trails")][("fluc_col1[1]")], misc::trails::fluc_col1[1]);
		write_value(json[("trails")][("fluc_col1[2]")], misc::trails::fluc_col1[2]);
		write_value(json[("trails")][("fluc_col1[3]")], misc::trails::fluc_col1[3]);
		write_value(json[("trails")][("fluc_col2[0]")], misc::trails::fluc_col1[0]);
		write_value(json[("trails")][("fluc_col2[1]")], misc::trails::fluc_col1[1]);
		write_value(json[("trails")][("fluc_col2[2]")], misc::trails::fluc_col1[2]);
		write_value(json[("trails")][("fluc_col2[3]")], misc::trails::fluc_col1[3]);
		write_value(json[("misc")][("ragdoll_gravity")], misc::ragdoll_gravity);
		write_value(json[("misc")][("custom_region")], misc::custom_region);
		write_value(json[("misc")][("custom_region_selection")], misc::custom_region_selection);
		write_value(json[("misc")][("watermark")], misc::watermark);
		write_value(json[("misc")][("spotify_on_watermark")], misc::spotify_on_watermark);
		write_value(json[("misc")][("vote_revealer")], misc::vote_revealer);
		write_value(json[("misc")][("vote_revealer_print_type")], misc::vote_revealer_print_type);
		write_value(json[("misc")][("auto_accept")], misc::auto_accept);
		write_value(json[("misc")][("round_stats")], misc::round_stats);
		write_value(json[("misc")][("clantag")], misc::clantag);
		write_value(json[("misc")][("door_spammer")], misc::door_spammer);
		write_value(json[("misc")][("door_spammer_delay")], misc::door_spammer_delay);
		write_value(json[("misc")][("door_spammer_key")], misc::door_spammer_key);
		write_value(json[("misc")][("hitlogs")], misc::hitlogs);
		write_value(json[("misc")][("hitlogs_print_type")], misc::hitlogs_print_type);
		write_value(json[("misc")][("jumpscout")], misc::jumpscout);
		write_value(json[("misc")][("jumpscout_key")], misc::jumpscout_key);
		write_value(json[("misc")][("stream_proof")], misc::stream_proof);

		/// Skins
		write_value(json[("skins")][("knife_changer")], skins::knife_changer);
		write_value(json[("skins")][("knife_index")], skins::knife_model);
		write_value(json[("skins")][("knife_paintkit_index")], skins::knife_paintkit_index);
		write_value(json[("skins")][("knife_onkit_index")], skins::knife_vec_index);
		write_value(json[("skins")][("glove_changer")], skins::glove_changer);
		write_value(json[("skins")][("glove_index")], skins::glove_model);
		write_value(json[("skins")][("glove_paintkit_index")], skins::glove_paintkit_index);
		write_value(json[("skins")][("glove_onkit_index")], skins::glove_vec_index);
		write_value(json[("skins")][("modulate_knife")], skins::modulate_knife);
		write_value(json[("skins")][("knife_color1[0]")], skins::knife_color1[0]);
		write_value(json[("skins")][("knife_color1[1]")], skins::knife_color1[1]);
		write_value(json[("skins")][("knife_color1[2]")], skins::knife_color1[2]);
		write_value(json[("skins")][("knife_color2[0]")], skins::knife_color2[0]);
		write_value(json[("skins")][("knife_color2[1]")], skins::knife_color2[1]);
		write_value(json[("skins")][("knife_color2[2]")], skins::knife_color2[2]);
		write_value(json[("skins")][("knife_color3[0]")], skins::knife_color3[0]);
		write_value(json[("skins")][("knife_color3[1]")], skins::knife_color3[1]);
		write_value(json[("skins")][("knife_color3[2]")], skins::knife_color3[2]);
		write_value(json[("skins")][("knife_color4[0]")], skins::knife_color4[0]);
		write_value(json[("skins")][("knife_color4[1]")], skins::knife_color4[1]);
		write_value(json[("skins")][("knife_color4[2]")], skins::knife_color4[2]);

		write_value(json[("settings")][("menu_col[0]")], c::menu_accent[0]);
		write_value(json[("settings")][("menu_col[1]")], c::menu_accent[1]);
		write_value(json[("settings")][("menu_col[2]")], c::menu_accent[2]);
		write_value(json[("settings")][("menu_col[3]")], c::menu_accent[3]);
		write_value(json[("settings")][("menu_bg")], settings::menu_bg);
		write_value(json[("settings")][("menu_bg_particles")], settings::menu_bg_particles);
		write_value(json[("settings")][("reduce_server_lag_upon_crash")], settings::reduce_server_lag_upon_crash);
		write_value(json[("settings")][("reduce_server_lag_upon_crash_key")], settings::reduce_server_lag_upon_crash_key);

		path.append(configs.at(index));

		const bool has_json_in_filename = path.rfind((".json")) != std::string::npos;
		if (!has_json_in_filename) {
			path.append((".json"));
		}

		if (std::ofstream output_file{ path }; output_file.good()) {
			output_file << std::setw(4) << json;
		}

		path.erase(path.size() - configs.at(index).size() - (!has_json_in_filename ? 5 : 0));
	}

	void load(const std::size_t index) {
		path.append(configs.at(index));
		path.append((".json"));

		if (std::ifstream input_file{ path }; input_file.good()) {
			nlohmann::json json;
			input_file >> json;

			/// Aimbot
			for (int i = 0; i < 4; i++) {
				read_value(json["aimbot"][std::string("enabled").append(std::to_string(i))], settings::aimbot::c_enabled[i]);
				read_value(json["aimbot"][std::string("fov").append(std::to_string(i))], settings::aimbot::c_fov[i]);
				read_value(json["aimbot"][std::string("smooth").append(std::to_string(i))], settings::aimbot::c_smooth[i]);
				read_value(json["aimbot"][std::string("rcs").append(std::to_string(i))], settings::aimbot::c_rcs[i]);
			}
			read_value(json[("aimbot")][("general_hitbox0")], aimbot::general_hitboxes[0]);
			read_value(json[("aimbot")][("general_hitbox1")], aimbot::general_hitboxes[1]);
			read_value(json[("aimbot")][("general_hitbox2")], aimbot::general_hitboxes[2]);
			read_value(json[("aimbot")][("general_hitbox3")], aimbot::general_hitboxes[3]);
			read_value(json[("aimbot")][("rifle_hitbox0")], aimbot::rifle_hitboxes[0]);
			read_value(json[("aimbot")][("rifle_hitbox1")], aimbot::rifle_hitboxes[1]);
			read_value(json[("aimbot")][("rifle_hitbox2")], aimbot::rifle_hitboxes[2]);
			read_value(json[("aimbot")][("rifle_hitbox3")], aimbot::rifle_hitboxes[3]);
			read_value(json[("aimbot")][("sniper_hitbox0")], aimbot::sniper_hitboxes[0]);
			read_value(json[("aimbot")][("sniper_hitbox1")], aimbot::sniper_hitboxes[1]);
			read_value(json[("aimbot")][("sniper_hitbox2")], aimbot::sniper_hitboxes[2]);
			read_value(json[("aimbot")][("sniper_hitbox3")], aimbot::sniper_hitboxes[3]);
			read_value(json[("aimbot")][("pistol_hitbox0")], aimbot::pistol_hitboxes[0]);
			read_value(json[("aimbot")][("pistol_hitbox1")], aimbot::pistol_hitboxes[1]);
			read_value(json[("aimbot")][("pistol_hitbox2")], aimbot::pistol_hitboxes[2]);
			read_value(json[("aimbot")][("pistol_hitbox3")], aimbot::pistol_hitboxes[3]);
			read_value(json[("aimbot")][("smg_hitbox0")], aimbot::smg_hitboxes[0]);
			read_value(json[("aimbot")][("smg_hitbox1")], aimbot::smg_hitboxes[1]);
			read_value(json[("aimbot")][("smg_hitbox2")], aimbot::smg_hitboxes[2]);
			read_value(json[("aimbot")][("smg_hitbox3")], aimbot::smg_hitboxes[3]);
			read_value(json[("aimbot")][("heavy_hitbox0")], aimbot::heavy_hitboxes[0]);
			read_value(json[("aimbot")][("heavy_hitbox1")], aimbot::heavy_hitboxes[1]);
			read_value(json[("aimbot")][("heavy_hitbox2")], aimbot::heavy_hitboxes[2]);
			read_value(json[("aimbot")][("heavy_hitbox3")], aimbot::heavy_hitboxes[3]);
			read_value(json[("aimbot")][("scoping_required")], aimbot::scoping_required);
			read_value(json[("aimbot")][("visible_only")], aimbot::visible_only);
			read_value(json[("aimbot")][("aim_key")], aimbot::aim_key);
			read_value(json[("aimbot")][("silent")], aimbot::silent);
			read_value(json[("aimbot")][("target_in_air")], aimbot::target_in_air);
			read_value(json[("aimbot")][("target_smoke")], aimbot::target_smoke);
			read_value(json[("aimbot")][("target_team")], aimbot::target_team);
			read_value(json[("aimbot")][("target_visible")], aimbot::target_visible);
			read_value(json[("aimbot")][("type")], aimbot::type);
			read_value(json[("aimbot")][("auto_pistol")], aimbot::auto_pistol);
			read_value(json[("aimbot")][("delay_after_kill")], aimbot::delay_after_kill);

			read_value(json[("aimbot")][("backtrack_enable")], aimbot::backtrack_enable);
			read_value(json[("aimbot")][("backtrack_ping_spike")], aimbot::backtrack_ping_spike);
			read_value(json[("aimbot")][("backtrack_ping_spike_value")], aimbot::backtrack_ping_spike_value);

			/// Visuals
			read_value(json[("visuals")][("enabled")], visuals::enable_esp);
			read_value(json[("visuals")][("dormant_esp")], visuals::dormant_esp);
			read_value(json[("visuals")][("visuals_box")], visuals::visuals_box);
			read_value(json[("visuals")][("box_lines[0]")], visuals::box_lines[0]);
			read_value(json[("visuals")][("box_lines[1]")], visuals::box_lines[1]);
			read_value(json[("visuals")][("box_filled_amount")], visuals::box_vertical);
			read_value(json[("visuals")][("box_type")], visuals::box_type);
			read_value(json[("visuals")][("box_type")], visuals::box_type);
			read_value(json[("visuals")][("visuals_name")], visuals::visuals_name);
			for (int i = 0; i < 128; i++) {
				read_value(json[("visuals")][("visuals_bot_extention_name")], visuals::visuals_bot_extention_name[i]);
			}
			read_value(json[("visuals")][("visuals_health")], visuals::visuals_health);
			read_value(json[("visuals")][("gradient_healthbar")], visuals::gradient_healthbar);
			read_value(json[("visuals")][("gradient_healthbar_col1[0]")], visuals::gradient_healthbar_col1[0]);
			read_value(json[("visuals")][("gradient_healthbar_col1[1]")], visuals::gradient_healthbar_col1[1]);
			read_value(json[("visuals")][("gradient_healthbar_col1[2]")], visuals::gradient_healthbar_col1[2]);
			read_value(json[("visuals")][("gradient_healthbar_col1[3]")], visuals::gradient_healthbar_col1[3]);
			read_value(json[("visuals")][("gradient_healthbar_col2[0]")], visuals::gradient_healthbar_col2[0]);
			read_value(json[("visuals")][("gradient_healthbar_col2[1]")], visuals::gradient_healthbar_col2[1]);
			read_value(json[("visuals")][("gradient_healthbar_col2[2]")], visuals::gradient_healthbar_col2[2]);
			read_value(json[("visuals")][("gradient_healthbar_col2[3]")], visuals::gradient_healthbar_col2[3]);
			read_value(json[("visuals")][("visuals_armor")], visuals::visuals_armor);
			read_value(json[("visuals")][("engine_radar")], visuals::engine_radar);
			read_value(json[("visuals")][("flash_kill")], visuals::flash_kill);
			read_value(json[("visuals")][("visuals_flags")], visuals::visuals_flags);
			read_value(json[("visuals")][("visuals_flags_uppercase")], visuals::visuals_flags_uppercase);
			read_value(json[("visuals")][("visuals_weapon")], visuals::visuals_weapon);
			read_value(json[("visuals")][("visuals_skeleton")], visuals::visuals_skeleton);
			read_value(json[("visuals")][("spectator_list")], visuals::spectator_list);
			read_value(json[("visuals")][("enable_chams")], visuals::enable_chams);
			read_value(json[("visuals")][("enable_chams_vis")], visuals::enable_chams_vis);
			read_value(json[("visuals")][("enable_chams_invis")], visuals::enable_chams_invis);
			read_value(json[("visuals")][("chams_overlay")], visuals::chams_overlay);
			read_value(json[("visuals")][("player_material")], visuals::player_material);
			read_value(json[("visuals")][("player_material_invis")], visuals::player_material_invis);
			read_value(json[("visuals")][("player_overlay_material")], visuals::player_overlay_material);
			read_value(json[("visuals")][("backtrack_chams_type")], visuals::backtrack_chams_type);
			read_value(json[("visuals")][("chams_vis[0]")], visuals::chams_vis[0]);
			read_value(json[("visuals")][("chams_vis[1]")], visuals::chams_vis[1]);
			read_value(json[("visuals")][("chams_vis[2]")], visuals::chams_vis[2]);
			read_value(json[("visuals")][("chams_vis[3]")], visuals::chams_vis[3]);
			read_value(json[("visuals")][("chams_invis[0]")], visuals::chams_invis[0]);
			read_value(json[("visuals")][("chams_invis[1]")], visuals::chams_invis[1]);
			read_value(json[("visuals")][("chams_invis[2]")], visuals::chams_invis[2]);
			read_value(json[("visuals")][("chams_invis[3]")], visuals::chams_invis[3]);
			read_value(json[("visuals")][("chams_overlay_clr[0]")], visuals::chams_overlay_clr[0]);
			read_value(json[("visuals")][("chams_overlay_clr[1]")], visuals::chams_overlay_clr[1]);
			read_value(json[("visuals")][("chams_overlay_clr[2]")], visuals::chams_overlay_clr[2]);
			read_value(json[("visuals")][("chams_overlay_clr[3]")], visuals::chams_overlay_clr[3]);
			read_value(json[("visuals")][("backtrack_chams")], visuals::backtrack_chams);
			read_value(json[("visuals")][("backtrack_chams_color[0]")], visuals::backtrack_chams_color[0]);
			read_value(json[("visuals")][("backtrack_chams_color[1]")], visuals::backtrack_chams_color[1]);
			read_value(json[("visuals")][("backtrack_chams_color[2]")], visuals::backtrack_chams_color[2]);
			read_value(json[("visuals")][("backtrack_chams_color[3]")], visuals::backtrack_chams_color[3]);
			read_value(json[("visuals")][("backtrack_render_type")], visuals::backtrack_render_type);
			read_value(json[("visuals")][("backtrack_chams_color2[0]")], visuals::backtrack_chams_color2[0]);
			read_value(json[("visuals")][("backtrack_chams_color2[1]")], visuals::backtrack_chams_color2[1]);
			read_value(json[("visuals")][("backtrack_chams_color2[2]")], visuals::backtrack_chams_color2[2]);
			read_value(json[("visuals")][("backtrack_chams_color2[3]")], visuals::backtrack_chams_color2[3]);
			read_value(json[("visuals")][("arm_chams")], visuals::arm_chams);
			read_value(json[("visuals")][("arm_chams_material")], visuals::arm_chams_material);
			read_value(json[("visuals")][("arm_chams_color[0]")], visuals::arm_chams_color[0]);
			read_value(json[("visuals")][("arm_chams_color[1]")], visuals::arm_chams_color[1]);
			read_value(json[("visuals")][("arm_chams_color[2]")], visuals::arm_chams_color[2]);
			read_value(json[("visuals")][("arm_chams_color[3]")], visuals::arm_chams_color[3]);
			read_value(json[("visuals")][("arm_overlay_chams")], visuals::arm_overlay_chams);
			read_value(json[("visuals")][("arm_overlay_material")], visuals::arm_overlay_material);
			read_value(json[("visuals")][("arm_overlay_color[0]")], visuals::arm_overlay_color[0]);
			read_value(json[("visuals")][("arm_overlay_color[1]")], visuals::arm_overlay_color[1]);
			read_value(json[("visuals")][("arm_overlay_color[2]")], visuals::arm_overlay_color[2]);
			read_value(json[("visuals")][("arm_overlay_color[3]")], visuals::arm_overlay_color[3]);
			read_value(json[("visuals")][("weapon_chams")], visuals::weapon_chams);
			read_value(json[("visuals")][("weapon_chams_material")], visuals::weapon_chams_material);
			read_value(json[("visuals")][("weapon_chams_color[0]")], visuals::weapon_chams_color[0]);
			read_value(json[("visuals")][("weapon_chams_color[1]")], visuals::weapon_chams_color[1]);
			read_value(json[("visuals")][("weapon_chams_color[2]")], visuals::weapon_chams_color[2]);
			read_value(json[("visuals")][("weapon_chams_color[3]")], visuals::weapon_chams_color[3]);
			read_value(json[("visuals")][("weapon_overlay_chams")], visuals::weapon_overlay_chams);
			read_value(json[("visuals")][("weapon_overlay_material")], visuals::weapon_overlay_material);
			read_value(json[("visuals")][("weapon_chams_overlay_color[0]")], visuals::weapon_chams_overlay_color[0]);
			read_value(json[("visuals")][("weapon_chams_overlay_color[1]")], visuals::weapon_chams_overlay_color[1]);
			read_value(json[("visuals")][("weapon_chams_overlay_color[2]")], visuals::weapon_chams_overlay_color[2]);
			read_value(json[("visuals")][("weapon_chams_overlay_color[3]")], visuals::weapon_chams_overlay_color[3]);
			read_value(json[("visuals")][("glow_enabled")], visuals::glow_enabled);
			read_value(json[("visuals")][("glow_players")], visuals::glow_players);
			read_value(json[("visuals")][("glow_type_player")], visuals::glow_type_player);
			read_value(json[("visuals")][("glow_type_bomb")], visuals::glow_type_bomb);
			read_value(json[("visuals")][("glow_type_chicken")], visuals::glow_type_chicken);
			read_value(json[("visuals")][("glow_type_fish")], visuals::glow_type_fish);
			read_value(json[("visuals")][("glow_bombs")], visuals::glow_bombs);
			read_value(json[("visuals")][("glow_chickens")], visuals::glow_chickens);
			read_value(json[("visuals")][("glow_type_weapons")], visuals::glow_type_weapons);
			read_value(json[("visuals")][("glow_weapons_col[0]")], visuals::glow_weapons_col[0]);
			read_value(json[("visuals")][("glow_weapons_col[1]")], visuals::glow_weapons_col[1]);
			read_value(json[("visuals")][("glow_weapons_col[2]")], visuals::glow_weapons_col[2]);
			read_value(json[("visuals")][("glow_weapons_col[3]")], visuals::glow_weapons_col[3]);
			read_value(json[("visuals")][("glow_fish")], visuals::glow_fish);
			read_value(json[("visuals")][("glow_player_col[0]")], visuals::glow_player_col[0]);
			read_value(json[("visuals")][("glow_player_col[1]")], visuals::glow_player_col[1]);
			read_value(json[("visuals")][("glow_player_col[2]")], visuals::glow_player_col[2]);
			read_value(json[("visuals")][("glow_player_col[3]")], visuals::glow_player_col[3]);
			read_value(json[("visuals")][("glow_player_invis_col[0]")], visuals::glow_player_invis_col[0]);
			read_value(json[("visuals")][("glow_player_invis_col[1]")], visuals::glow_player_invis_col[1]);
			read_value(json[("visuals")][("glow_player_invis_col[2]")], visuals::glow_player_invis_col[2]);
			read_value(json[("visuals")][("glow_player_invis_col[3]")], visuals::glow_player_invis_col[3]);
			read_value(json[("visuals")][("glow_bomb_col[0]")], visuals::glow_bomb_col[0]);
			read_value(json[("visuals")][("glow_bomb_col[1]")], visuals::glow_bomb_col[1]);
			read_value(json[("visuals")][("glow_bomb_col[2]")], visuals::glow_bomb_col[2]);
			read_value(json[("visuals")][("glow_bomb_col[3]")], visuals::glow_bomb_col[3]);
			read_value(json[("visuals")][("glow_chicken_col[0]")], visuals::glow_chicken_col[0]);
			read_value(json[("visuals")][("glow_chicken_col[1]")], visuals::glow_chicken_col[1]);
			read_value(json[("visuals")][("glow_chicken_col[2]")], visuals::glow_chicken_col[2]);
			read_value(json[("visuals")][("glow_chicken_col[3]")], visuals::glow_chicken_col[3]);
			read_value(json[("visuals")][("glow_fish_col[0]")], visuals::glow_fish_col[0]);
			read_value(json[("visuals")][("glow_fish_col[1]")], visuals::glow_fish_col[1]);
			read_value(json[("visuals")][("glow_fish_col[2]")], visuals::glow_fish_col[2]);
			read_value(json[("visuals")][("glow_fish_col[3]")], visuals::glow_fish_col[3]);
			read_value(json[("visuals")][("world_modulation")], visuals::world_modulation);
			read_value(json[("visuals")][("world_modulation_clr[0]")], visuals::world_modulation_clr[0]);
			read_value(json[("visuals")][("world_modulation_clr[1]")], visuals::world_modulation_clr[1]);
			read_value(json[("visuals")][("world_modulation_clr[2]")], visuals::world_modulation_clr[2]);
			read_value(json[("visuals")][("world_modulation_clr[3]")], visuals::world_modulation_clr[3]);
			read_value(json[("visuals")][("prop_modulation")], visuals::prop_modulation);
			read_value(json[("visuals")][("prop_modulation_clr[0]")], visuals::prop_modulation_clr[0]);
			read_value(json[("visuals")][("prop_modulation_clr[1]")], visuals::prop_modulation_clr[1]);
			read_value(json[("visuals")][("prop_modulation_clr[2]")], visuals::prop_modulation_clr[2]);
			read_value(json[("visuals")][("prop_modulation_clr[3]")], visuals::prop_modulation_clr[3]);
			read_value(json[("visuals")][("hitsound")], visuals::hitsound);
			read_value(json[("visuals")][("killsound")], visuals::killsound);
			read_value(json[("visuals")][("visuals_disableflash")], visuals::visuals_disableflash);
			read_value(json[("visuals")][("visuals_disable_panorama_blur")], visuals::visuals_disable_panorama_blur);
			read_value(json[("visuals")][("fov_circle")], visuals::fov_circle);
			read_value(json[("visuals")][("fov_circle_clr[0]")], visuals::fov_circle_clr[0]);
			read_value(json[("visuals")][("fov_circle_clr[1]")], visuals::fov_circle_clr[1]);
			read_value(json[("visuals")][("fov_circle_clr[2]")], visuals::fov_circle_clr[2]);
			read_value(json[("visuals")][("fov_circle_clr[3]")], visuals::fov_circle_clr[3]);
			read_value(json[("visuals")][("spectator_list")], visuals::spectator_list);
			read_value(json[("visuals")][("spectator_list_pos_x")], visuals::spectator_list_pos_x);
			read_value(json[("visuals")][("spectator_list_pos_y")], visuals::spectator_list_pos_y);


			read_value(json[("visuals")][("modulate_molotov")], visuals::modulate_molotov);
			read_value(json[("visuals")][("wireframe_fire")], visuals::wireframe_fire);
			read_value(json[("visuals")][("modulate_molotov_clr[0]")], visuals::modulate_molotov_clr[0]);
			read_value(json[("visuals")][("modulate_molotov_clr[1]")], visuals::modulate_molotov_clr[1]);
			read_value(json[("visuals")][("modulate_molotov_clr[2]")], visuals::modulate_molotov_clr[2]);
			read_value(json[("visuals")][("modulate_molotov_clr[3]")], visuals::modulate_molotov_clr[3]);

			read_value(json[("visuals")][("modulate_smoke")], visuals::modulate_smoke);
			read_value(json[("visuals")][("wireframe_smoke")], visuals::wireframe_smoke);
			read_value(json[("visuals")][("modulate_smoke_clr[0]")], visuals::modulate_smoke_clr[0]);
			read_value(json[("visuals")][("modulate_smoke_clr[1]")], visuals::modulate_smoke_clr[1]);
			read_value(json[("visuals")][("modulate_smoke_clr[2]")], visuals::modulate_smoke_clr[2]);
			read_value(json[("visuals")][("modulate_smoke_clr[3]")], visuals::modulate_smoke_clr[3]);

			read_value(json[("visuals")][("modulate_blood")], visuals::modulate_blood);
			read_value(json[("visuals")][("modulate_blood_clr[0]")], visuals::modulate_blood_clr[0]);
			read_value(json[("visuals")][("modulate_blood_clr[1]")], visuals::modulate_blood_clr[1]);
			read_value(json[("visuals")][("modulate_blood_clr[2]")], visuals::modulate_blood_clr[2]);
			read_value(json[("visuals")][("modulate_blood_clr[3]")], visuals::modulate_blood_clr[3]);

			read_value(json[("visuals")][("modulate_weapon_effects")], visuals::modulate_weapon_effects);
			read_value(json[("visuals")][("modulate_weapon_effects_clr[0]")], visuals::modulate_weapon_effects_clr[0]);
			read_value(json[("visuals")][("modulate_weapon_effects_clr[1]")], visuals::modulate_weapon_effects_clr[1]);
			read_value(json[("visuals")][("modulate_weapon_effects_clr[2]")], visuals::modulate_weapon_effects_clr[2]);
			read_value(json[("visuals")][("modulate_weapon_effects_clr[3]")], visuals::modulate_weapon_effects_clr[3]);

			read_value(json[("visuals")][("grenade_prediction")], visuals::grenade_prediction);
			read_value(json[("visuals")][("grenade_prediction_clr[0]")], visuals::grenade_prediction_clr[0]);
			read_value(json[("visuals")][("grenade_prediction_clr[1]")], visuals::grenade_prediction_clr[1]);
			read_value(json[("visuals")][("grenade_prediction_clr[2]")], visuals::grenade_prediction_clr[2]);
			read_value(json[("visuals")][("grenade_prediction_clr[3]")], visuals::grenade_prediction_clr[3]);

			read_value(json[("visuals")][("fog")], visuals::fog);
			read_value(json[("visuals")][("fog_start")], visuals::fog_start);
			read_value(json[("visuals")][("fog_end")], visuals::fog_end);
			read_value(json[("visuals")][("fog_density")], visuals::fog_density);
			read_value(json[("visuals")][("fog_clr[0]")], visuals::fog_clr[0]);
			read_value(json[("visuals")][("fog_clr[1]")], visuals::fog_clr[1]);
			read_value(json[("visuals")][("fog_clr[2]")], visuals::fog_clr[2]);
			read_value(json[("visuals")][("fog_clr[3]")], visuals::fog_clr[3]);

			read_value(json[("visuals")][("skybox_changer_index")], visuals::skybox_changer_index);
			read_value(json[("visuals")][("force_crosshair")], visuals::force_crosshair);
			read_value(json[("visuals")][("hit_effect")], visuals::hit_effect);

			read_value(json[("visuals")][("viewmodel")], visuals::viewmodel);
			read_value(json[("visuals")][("viewmodel_offset_x")], visuals::viewmodel_offset_x);
			read_value(json[("visuals")][("viewmodel_offset_y")], visuals::viewmodel_offset_y);
			read_value(json[("visuals")][("viewmodel_offset_z")], visuals::viewmodel_offset_z);
			read_value(json[("visuals")][("oof_arrows")], visuals::oof_arrows);
			read_value(json[("visuals")][("oof_arrow_range")], visuals::oof_arrow_range);
			read_value(json[("visuals")][("oof_arrow_size")], visuals::oof_arrow_size);
			read_value(json[("visuals")][("oof_arrow_col1[0]")], visuals::oof_arrow_col1[0]);
			read_value(json[("visuals")][("oof_arrow_col1[1]")], visuals::oof_arrow_col1[1]);
			read_value(json[("visuals")][("oof_arrow_col1[2]")], visuals::oof_arrow_col1[2]);
			read_value(json[("visuals")][("oof_arrow_col1[3]")], visuals::oof_arrow_col1[3]);
			read_value(json[("visuals")][("oof_arrow_col2[0]")], visuals::oof_arrow_col2[0]);
			read_value(json[("visuals")][("oof_arrow_col2[1]")], visuals::oof_arrow_col2[1]);
			read_value(json[("visuals")][("oof_arrow_col2[2]")], visuals::oof_arrow_col2[2]);
			read_value(json[("visuals")][("oof_arrow_col2[3]")], visuals::oof_arrow_col2[3]);
			read_value(json[("visuals")][("oof_arrow_col3[0]")], visuals::oof_arrow_col3[0]);
			read_value(json[("visuals")][("oof_arrow_col3[1]")], visuals::oof_arrow_col3[1]);
			read_value(json[("visuals")][("oof_arrow_col3[2]")], visuals::oof_arrow_col3[2]);
			read_value(json[("visuals")][("oof_arrow_col3[3]")], visuals::oof_arrow_col3[3]);

			read_value(json[("motion_blur")][("enabled")], motion_blur::enabled);
			read_value(json[("motion_blur")][("foward_blur")], motion_blur::foward_blur);
			read_value(json[("motion_blur")][("rotation_intensity")], motion_blur::rotation_intensity);
			read_value(json[("motion_blur")][("blur_strength")], motion_blur::blur_strength);
			read_value(json[("motion_blur")][("falling_min")], motion_blur::falling_min);
			read_value(json[("motion_blur")][("falling_max")], motion_blur::falling_max);

			/// Visualisation
			read_value(json[("visualisation")][("visualize_eb")], visulisation::visualize_eb);
			read_value(json[("visualisation")][("text_on_eb")], visulisation::text_on_eb);
			read_value(json[("visualisation")][("text_on_eb_col[0]")], visulisation::text_on_eb_col[0]);
			read_value(json[("visualisation")][("text_on_eb_col[1]")], visulisation::text_on_eb_col[1]);
			read_value(json[("visualisation")][("text_on_eb_col[2]")], visulisation::text_on_eb_col[2]);
			read_value(json[("visualisation")][("text_on_eb_col[3]")], visulisation::text_on_eb_col[3]);
			read_value(json[("visualisation")][("effect_on_eb")], visulisation::effect_on_eb);
			read_value(json[("visualisation")][("effect_on_eb_col[0]")], visulisation::effect_on_eb_col[0]);
			read_value(json[("visualisation")][("effect_on_eb_col[1]")], visulisation::effect_on_eb_col[1]);
			read_value(json[("visualisation")][("effect_on_eb_col[2]")], visulisation::effect_on_eb_col[2]);
			read_value(json[("visualisation")][("effect_on_eb_col[3]")], visulisation::effect_on_eb_col[3]);
			read_value(json[("visualisation")][("circle_on_eb")], visulisation::circle_on_eb);
			read_value(json[("visualisation")][("circle_on_eb_col[0]")], visulisation::circle_on_eb_col[0]);
			read_value(json[("visualisation")][("circle_on_eb_col[1]")], visulisation::circle_on_eb_col[1]);
			read_value(json[("visualisation")][("circle_on_eb_col[2]")], visulisation::circle_on_eb_col[2]);
			read_value(json[("visualisation")][("circle_on_eb_col[3]")], visulisation::circle_on_eb_col[3]);
			read_value(json[("visualisation")][("cube_on_eb")], visulisation::cube_on_eb);
			read_value(json[("visualisation")][("cube_on_eb_col[0]")], visulisation::cube_on_eb_col[0]);
			read_value(json[("visualisation")][("cube_on_eb_col[1]")], visulisation::cube_on_eb_col[1]);
			read_value(json[("visualisation")][("cube_on_eb_col[2]")], visulisation::cube_on_eb_col[2]);
			read_value(json[("visualisation")][("cube_on_eb_col[3]")], visulisation::cube_on_eb_col[3]);
			read_value(json[("visualisation")][("dlight_on_eb")], visulisation::dlight_on_eb);
			read_value(json[("visualisation")][("dlight_on_eb_col[0]")], visulisation::dlight_on_eb_col[0]);
			read_value(json[("visualisation")][("dlight_on_eb_col[1]")], visulisation::dlight_on_eb_col[1]);
			read_value(json[("visualisation")][("dlight_on_eb_col[2]")], visulisation::dlight_on_eb_col[2]);
			read_value(json[("visualisation")][("dlight_on_eb_col[3]")], visulisation::dlight_on_eb_col[3]);
			read_value(json[("visualisation")][("dlight_on_eb_brightness")], visulisation::dlight_on_eb_brightness);
			read_value(json[("visualisation")][("dlight_on_eb_radius")], visulisation::dlight_on_eb_radius);
			read_value(json[("visualisation")][("vis_eb_fadeout_time")], visulisation::vis_eb_fadeout_time);
			read_value(json[("visualisation")][("predicted_circle_on_eb")], visulisation::predicted_circle_on_eb);
			read_value(json[("visualisation")][("predicted_eb_circle_col[0]")], visulisation::predicted_circle_on_eb_col[0]);
			read_value(json[("visualisation")][("predicted_eb_circle_col[1]")], visulisation::predicted_circle_on_eb_col[1]);
			read_value(json[("visualisation")][("predicted_eb_circle_col[2]")], visulisation::predicted_circle_on_eb_col[2]);
			read_value(json[("visualisation")][("predicted_eb_circle_col[3]")], visulisation::predicted_circle_on_eb_col[3]);
			read_value(json[("visualisation")][("predicted_eb_circle2d")], visulisation::predicted_eb_circle2d);
			read_value(json[("visualisation")][("predicted_eb_circle2d_col[0]")], visulisation::predicted_eb_circle2d_col[0]);
			read_value(json[("visualisation")][("predicted_eb_circle2d_col[1]")], visulisation::predicted_eb_circle2d_col[1]);
			read_value(json[("visualisation")][("predicted_eb_circle2d_col[2]")], visulisation::predicted_eb_circle2d_col[2]);
			read_value(json[("visualisation")][("predicted_eb_circle2d_col[3]")], visulisation::predicted_eb_circle2d_col[3]);
			read_value(json[("visuals")][("sparks_while_eb")], visulisation::sparks_while_eb);
			read_value(json[("visuals")][("lightning_while_eb")], visulisation::lightning_while_eb);

			/// Indicators
			read_value(json[("indicators")][("velo")], indicators::velo);
			read_value(json[("indicators")][("graph_velocity")], indicators::graph_velocity);
			read_value(json[("indicators")][("velo_graph_size")], indicators::velo_graph_size);
			read_value(json[("indicators")][("velo_alpha_type")], indicators::velo_alpha_type);
			read_value(json[("indicators")][("velo_color_type")], indicators::velo_color_type);
			read_value(json[("indicators")][("velo_takeoff")], indicators::velo_takeoff);
			read_value(json[("indicators")][("graph_position")], indicators::graph_position);
			read_value(json[("indicators")][("graph_thickness")], indicators::graph_thickness);
			read_value(json[("indicators")][("graph_jump_velo")], indicators::graph_jump_velo);
			read_value(json[("indicators")][("graph_land_velo")], indicators::graph_land_velo);
			read_value(json[("indicators")][("graph_bold_font")], indicators::graph_bold_font);
			read_value(json[("indicators")][("graph_eb")], indicators::graph_eb);
			read_value(json[("indicators")][("graph_jb")], indicators::graph_jb);
			read_value(json[("indicators")][("position")], indicators::position);
			read_value(json[("indicators")][("velo_graph_style")], indicators::velo_graph_style);
			read_value(json[("indicators")][("stamina_graph_style")], indicators::stamina_graph_style);
			read_value(json[("indicators")][("velo_graph_scale")], indicators::velo_graph_scale);
			read_value(json[("indicators")][("graph_velo_col[0]")], indicators::graph_velo_col[0]);
			read_value(json[("indicators")][("graph_velo_col[1]")], indicators::graph_velo_col[1]);
			read_value(json[("indicators")][("graph_velo_col[2]")], indicators::graph_velo_col[2]);
			read_value(json[("indicators")][("graph_velo_col[3]")], indicators::graph_velo_col[3]);
			read_value(json[("indicators")][("stamina")], indicators::stamina);
			read_value(json[("indicators")][("graph_stamina")], indicators::graph_stamina);
			read_value(json[("indicators")][("graph_stamina_col[0]")], indicators::graph_stamina_col[0]);
			read_value(json[("indicators")][("graph_stamina_col[1]")], indicators::graph_stamina_col[1]);
			read_value(json[("indicators")][("graph_stamina_col[2]")], indicators::graph_stamina_col[2]);
			read_value(json[("indicators")][("graph_stamina_col[3]")], indicators::graph_stamina_col[3]);
			read_value(json[("indicators")][("ej")], indicators::ej);
			read_value(json[("indicators")][("jb")], indicators::jb);
			read_value(json[("indicators")][("eb")], indicators::eb);
			read_value(json[("indicators")][("lb")], indicators::lb);
			read_value(json[("indicators")][("ps_default_col[0]")], indicators::ps_default_col[0]);
			read_value(json[("indicators")][("ps_default_col[1]")], indicators::ps_default_col[1]);
			read_value(json[("indicators")][("ps_default_col[2]")], indicators::ps_default_col[2]);
			read_value(json[("indicators")][("ps_default_col[3]")], indicators::ps_default_col[3]);
			read_value(json[("indicators")][("ps_rage_col[0]")], indicators::ps_rage_col[0]);
			read_value(json[("indicators")][("ps_rage_col[1]")], indicators::ps_rage_col[1]);
			read_value(json[("indicators")][("ps_rage_col[2]")], indicators::ps_rage_col[2]);
			read_value(json[("indicators")][("ps_rage_col[3]")], indicators::ps_rage_col[3]);
			read_value(json[("indicators")][("lb_default_col[0]")], indicators::lb_default_col[0]);
			read_value(json[("indicators")][("lb_default_col[1]")], indicators::lb_default_col[1]);
			read_value(json[("indicators")][("lb_default_col[2]")], indicators::lb_default_col[2]);
			read_value(json[("indicators")][("lb_default_col[3]")], indicators::lb_default_col[3]);
			read_value(json[("indicators")][("lb_rage_col[0]")], indicators::lb_rage_col[0]);
			read_value(json[("indicators")][("lb_rage_col[1]")], indicators::lb_rage_col[1]);
			read_value(json[("indicators")][("lb_rage_col[2]")], indicators::lb_rage_col[2]);
			read_value(json[("indicators")][("lb_rage_col[3]")], indicators::lb_rage_col[3]);
			read_value(json[("indicators")][("velo_col[0]")], indicators::velo_col[0]);
			read_value(json[("indicators")][("velo_col[1]")], indicators::velo_col[1]);
			read_value(json[("indicators")][("velo_col[2]")], indicators::velo_col[2]);
			read_value(json[("indicators")][("velo_col[3]")], indicators::velo_col[3]);
			read_value(json[("indicators")][("ej_col[0]")], indicators::ej_col[0]);
			read_value(json[("indicators")][("ej_col[1]")], indicators::ej_col[1]);
			read_value(json[("indicators")][("ej_col[2]")], indicators::ej_col[2]);
			read_value(json[("indicators")][("ej_col[3]")], indicators::ej_col[3]);
			read_value(json[("indicators")][("ej_rage_col[0]")], indicators::ej_rage_col[0]);
			read_value(json[("indicators")][("ej_rage_col[1]")], indicators::ej_rage_col[1]);
			read_value(json[("indicators")][("ej_rage_col[2]")], indicators::ej_rage_col[2]);
			read_value(json[("indicators")][("ej_rage_col[3]")], indicators::ej_rage_col[3]);
			read_value(json[("indicators")][("jb_col[0]")], indicators::jb_col[0]);
			read_value(json[("indicators")][("jb_col[1]")], indicators::jb_col[1]);
			read_value(json[("indicators")][("jb_col[2]")], indicators::jb_col[2]);
			read_value(json[("indicators")][("jb_col[3]")], indicators::jb_col[3]);
			read_value(json[("indicators")][("jb_rage_col[0]")], indicators::jb_rage_col[0]);
			read_value(json[("indicators")][("jb_rage_col[1]")], indicators::jb_rage_col[1]);
			read_value(json[("indicators")][("jb_rage_col[2]")], indicators::jb_rage_col[2]);
			read_value(json[("indicators")][("jb_rage_col[3]")], indicators::jb_rage_col[3]);
			read_value(json[("indicators")][("stamina_col[0]")], indicators::stamina_col[0]);
			read_value(json[("indicators")][("stamina_col[1]")], indicators::stamina_col[1]);
			read_value(json[("indicators")][("stamina_col[2]")], indicators::stamina_col[2]);
			read_value(json[("indicators")][("stamina_col[3]")], indicators::stamina_col[3]);
			read_value(json[("indicators")][("eb_default_col[0]")], indicators::eb_default_col[0]);
			read_value(json[("indicators")][("eb_default_col[1]")], indicators::eb_default_col[1]);
			read_value(json[("indicators")][("eb_default_col[2]")], indicators::eb_default_col[2]);
			read_value(json[("indicators")][("eb_default_col[3]")], indicators::eb_default_col[3]);
			read_value(json[("indicators")][("eb_rage_col[0]")], indicators::eb_rage_col[0]);
			read_value(json[("indicators")][("eb_rage_col[1]")], indicators::eb_rage_col[1]);
			read_value(json[("indicators")][("eb_rage_col[2]")], indicators::eb_rage_col[2]);
			read_value(json[("indicators")][("eb_rage_col[3]")], indicators::eb_rage_col[3]);
			read_value(json[("indicators")][("stamina_alpha_type")], indicators::stamina_alpha_type);
			read_value(json[("indicators")][("round_stamina")], indicators::round_stamina);
			read_value(json[("indicators")][("stamina_takeoff")], indicators::stamina_takeoff);

			read_value(json[("indicators")][("key_strokes")], indicators::key_strokes);
			read_value(json[("indicators")][("key_strokes_fadeout")], indicators::key_strokes_fadeout);
			read_value(json[("indicators")][("key_strokes_stances")], indicators::key_strokes_stances);
			read_value(json[("indicators")][("key_strokes_position")], indicators::key_strokes_position);

			/// Edgebug effects
			read_value(json[("eb_effects")][("print_eb_info")], edgebug_effects::print_eb_info);
			read_value(json[("eb_effects")][("print_eb_info_print_type")], edgebug_effects::print_eb_info_print_type);
			read_value(json[("eb_effects")][("healthboost_on_eb")], edgebug_effects::healthboost_on_eb);
			read_value(json[("eb_effects")][("sound_on_eb")], edgebug_effects::sound_on_eb);
			read_value(json[("eb_effects")][("effect_on_eb")], edgebug_effects::effect_on_eb);
			read_value(json[("eb_effects")][("effect_time")], edgebug_effects::effect_time);
			read_value(json[("eb_effects")][("eb_effect")], edgebug_effects::eb_effect);

			/// Movement
			read_value(json[("movement")][("bhop")], movement::bhop);
			read_value(json[("movement")][("bhop_hitchance")], movement::bhop_chance);
			read_value(json[("movement")][("edgebug")], movement::edgebug);
			read_value(json[("movement")][("jumpbug")], movement::jumpbug);
			read_value(json[("movement")][("edgebug_key")], movement::edgebug_key);
			read_value(json[("movement")][("jumpbug_key")], movement::jumpbug_key);
			read_value(json[("movement")][("ladder_bug")], movement::ladder_bug);
			read_value(json[("movement")][("ladder_bug_key")], movement::ladder_bug_key);
			read_value(json[("movement")][("edgebug_scan_crouch_after_x_pred")], movement::edgebug_scan_crouch_after_x_pred);
			read_value(json[("movement")][("edgebug_scan_strength")], movement::edgebug_scan_strength);
			read_value(json[("movement")][("edgebug_scan_time")], movement::edgebug_scan_time);
			read_value(json[("movement")][("edgebug_scan_type")], movement::edgebug_scan_type);
			read_value(json[("movement")][("lock_pull_amount")], movement::lock_pull_amount);
			read_value(json[("movement")][("lock_on_eb")], movement::lock_on_eb);
			read_value(json[("movement")][("lock_pitch")], movement::lock_pitch);
			read_value(json[("movement")][("edgejump")], movement::edgejump);
			read_value(json[("movement")][("edgejump_key")], movement::edgejump_key);
			read_value(json[("movement")][("pixelsurf")], movement::pixelsurf);
			read_value(json[("movement")][("pixelsurf_key")], movement::pixelsurf_key);
			read_value(json[("movement")][("jumpshot_info")], movement::jumpshot_info);
			read_value(json[("movement")][("jumpshot_info_print_type")], movement::jumpshot_info_print_type);
			read_value(json[("movement")][("jumpshot_slurs")], movement::jumpshot_slurs);
			read_value(json[("movement")][("no_duck_cooldown")], movement::no_duck_cooldown);
			read_value(json[("movement")][("longjump")], movement::longjump);
			read_value(json[("movement")][("strafe_assistance")], movement::strafe_assistance);
			read_value(json[("movement")][("simulate_as")], movement::simulate_as); 
			read_value(json[("movement")][("strafe_optimiser")], movement::strafe_optimiser);
			read_value(json[("movement")][("strafe_optimiser_on_key")], movement::strafe_optimiser_on_key);
			read_value(json[("movement")][("strafe_optimiser_key")], movement::strafe_optimiser_key);
			read_value(json[("movement")][("desired_gain")], movement::desired_gain);
			read_value(json[("movement")][("required_speed")], movement::required_speed);

			/// Misc
			read_value(json[("misc")][("anti_afk")], misc::anti_afk);
			read_value(json[("misc")][("reveal_ranks")], misc::reveal_ranks);
			read_value(json[("misc")][("settings_unsafe_features")], misc::settings_unsafe_features);
			read_value(json[("misc")][("custom_netgraph")], misc::custom_netgraph);
			read_value(json[("misc")][("modulate_netgraph")], misc::modulate_netgraph);
			read_value(json[("misc")][("aspect_ratio_changer")], misc::aspect_ratio_changer);
			read_value(json[("misc")][("aspect_ratio_changer_value")], misc::aspect_ratio_changer_value);
			read_value(json[("trails")][("enable")], misc::trails::enable);
			read_value(json[("trails")][("color_type")], misc::trails::color_type);
			read_value(json[("trails")][("rainbow_speed")], misc::trails::rainbow_speed);
			read_value(json[("trails")][("life")], misc::trails::life);
			read_value(json[("trails")][("width")], misc::trails::width);
			read_value(json[("trails")][("life_speed")], misc::trails::life_speed);
			read_value(json[("trails")][("col[0]")], misc::trails::col[0]);
			read_value(json[("trails")][("col[1]")], misc::trails::col[1]);
			read_value(json[("trails")][("col[2]")], misc::trails::col[2]);
			read_value(json[("trails")][("col[3]")], misc::trails::col[3]);
			read_value(json[("trails")][("fluc_col1[0]")], misc::trails::fluc_col1[0]);
			read_value(json[("trails")][("fluc_col1[1]")], misc::trails::fluc_col1[1]);
			read_value(json[("trails")][("fluc_col1[2]")], misc::trails::fluc_col1[2]);
			read_value(json[("trails")][("fluc_col1[3]")], misc::trails::fluc_col1[3]);
			read_value(json[("trails")][("fluc_col2[0]")], misc::trails::fluc_col1[0]);
			read_value(json[("trails")][("fluc_col2[1]")], misc::trails::fluc_col1[1]);
			read_value(json[("trails")][("fluc_col2[2]")], misc::trails::fluc_col1[2]);
			read_value(json[("trails")][("fluc_col2[3]")], misc::trails::fluc_col1[3]);
			read_value(json[("misc")][("ragdoll_gravity")], misc::ragdoll_gravity);
			read_value(json[("misc")][("custom_region")], misc::custom_region);
			read_value(json[("misc")][("custom_region_selection")], misc::custom_region_selection);
			read_value(json[("misc")][("watermark")], misc::watermark);
			read_value(json[("misc")][("spotify_on_watermark")], misc::spotify_on_watermark);
			read_value(json[("misc")][("vote_revealer")], misc::vote_revealer);
			read_value(json[("misc")][("vote_revealer_print_type")], misc::vote_revealer_print_type);
			read_value(json[("misc")][("auto_accept")], misc::auto_accept);
			read_value(json[("misc")][("round_stats")], misc::round_stats);
			read_value(json[("misc")][("clantag")], misc::clantag);
			read_value(json[("misc")][("door_spammer")], misc::door_spammer);
			read_value(json[("misc")][("door_spammer_delay")], misc::door_spammer_delay);
			read_value(json[("misc")][("door_spammer_key")], misc::door_spammer_key);
			read_value(json[("misc")][("hitlogs")], misc::hitlogs);
			read_value(json[("misc")][("hitlogs_print_type")], misc::hitlogs_print_type);
			read_value(json[("misc")][("jumpscout")], misc::jumpscout);
			read_value(json[("misc")][("jumpscout_key")], misc::jumpscout_key);
			read_value(json[("misc")][("stream_proof")], misc::stream_proof);

			/// Skins
			read_value(json[("skins")][("knife_changer")], skins::knife_changer);
			read_value(json[("skins")][("knife_index")], skins::knife_model);
			read_value(json[("skins")][("knife_paintkit_index")], skins::knife_paintkit_index);
			read_value(json[("skins")][("knife_onkit_index")], skins::knife_vec_index);
			read_value(json[("skins")][("glove_changer")], skins::glove_changer);
			read_value(json[("skins")][("glove_index")], skins::glove_model);
			read_value(json[("skins")][("glove_paintkit_index")], skins::glove_paintkit_index);
			read_value(json[("skins")][("glove_onkit_index")], skins::glove_vec_index);
			read_value(json[("skins")][("modulate_knife")], skins::modulate_knife);
			read_value(json[("skins")][("knife_color1[0]")], skins::knife_color1[0]);
			read_value(json[("skins")][("knife_color1[1]")], skins::knife_color1[1]);
			read_value(json[("skins")][("knife_color1[2]")], skins::knife_color1[2]);
			read_value(json[("skins")][("knife_color2[0]")], skins::knife_color2[0]);
			read_value(json[("skins")][("knife_color2[1]")], skins::knife_color2[1]);
			read_value(json[("skins")][("knife_color2[2]")], skins::knife_color2[2]);
			read_value(json[("skins")][("knife_color3[0]")], skins::knife_color3[0]);
			read_value(json[("skins")][("knife_color3[1]")], skins::knife_color3[1]);
			read_value(json[("skins")][("knife_color3[2]")], skins::knife_color3[2]);
			read_value(json[("skins")][("knife_color4[0]")], skins::knife_color4[0]);
			read_value(json[("skins")][("knife_color4[1]")], skins::knife_color4[1]);
			read_value(json[("skins")][("knife_color4[2]")], skins::knife_color4[2]);

			read_value(json[("settings")][("menu_col[0]")], c::menu_accent[0]);
			read_value(json[("settings")][("menu_col[1]")], c::menu_accent[1]);
			read_value(json[("settings")][("menu_col[2]")], c::menu_accent[2]);
			read_value(json[("settings")][("menu_col[3]")], c::menu_accent[3]);
			read_value(json[("settings")][("rainbow_menu_col")], settings::rainbow_menu_col);
			read_value(json[("settings")][("menu_bg")], settings::menu_bg);
			read_value(json[("settings")][("menu_bg_particles")], settings::menu_bg_particles);
			read_value(json[("settings")][("reduce_server_lag_upon_crash")], settings::reduce_server_lag_upon_crash);
			read_value(json[("settings")][("reduce_server_lag_upon_crash_key")], settings::reduce_server_lag_upon_crash_key);
		}

		path.erase(path.size() - configs.at(index).size() - 5);
	}
}