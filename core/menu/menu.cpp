#include "lua/lua.h"
#include "menu.hpp"
#include "config/config.hpp"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx9.h"
#include "../features/features.hpp"
#include "ImGui/custom.h"
#include "../features/utils/notifications.h"
#include "../xor.h"
static bool save_config = false;
static bool load_config = false;
int last_saved_parsed_skin;
const char* edgebug_mode[]{ "Dynamic", "Force crouch", "Force stand" };
const char* chams_vis[]{ "Material", "Flat", "Animated", "Crystal", "Metallic",  "Disco" };
const char* chams_invis[]{ "Material", "Flat", "Animated", "Crystal", "Metallic", "Disco" };
const char* chams_overlay[]{ "Glow", "Snow", "Animated lines" };
const char* knife_skins[]{ "Default", "Bayonet", "M9 bayonet","Karambit","Bowie","Butterfly","Falchion","Flip","Gut","Huntsman","Shaddow daggers","Navaja","Stiletto","Talon","Ursus","Canis","Outdoor","Cord","Skeleton","Classic" };
const char* glove_types[]{ "Default", "Blood","Sport","Slick","Leather","Moto","Speci","Hydra" };
const char* scan_types[]{ "Dynamic", "Stand", "Crouch" };
const char* print_types[]{ "Chat", "On screen" };
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool menu::on_init(IDirect3DDevice9* device) {
	hwnd = FindWindow("Valve001", NULL);
	original_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndproc)));

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(device);




	return true;
}

bool menu::second_init_because_lazy() {
	auto& io{ ImGui::GetIO() };
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigWindowsMoveFromTitleBarOnly = false;

	ImGui::StyleColorsDark();

	auto& style{ ImGui::GetStyle() };
	style.ScrollbarSize = 13.0f;
	style.WindowTitleAlign = ImVec2{ 0.5f, 0.5f };
	style.WindowRounding = 3.f;
	style.ChildRounding = 2.f;
	style.ChildBorderSize = 0.f;
	style.WindowBorderSize = 0.f;
	style.FrameBorderSize = 1.f;
	style.FrameRounding = 2.f;
	style.GrabMinSize = 1.f;
	style.ScrollbarRounding = 0.f;
	style.ScrollbarSize = 1.f;
	style.ItemSpacing.x = 4.f;
	style.ItemSpacing.y = 10.f;

	return true;
}
static auto vector_getter = [](void* vec, int idx, const char** out_text) {
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

bool combo_array(const char* label, int* current_index, std::vector<std::string>& values) {
	if (values.empty()) { return false; }
	return ImGui::Combo(label, current_index, vector_getter,
		static_cast<void*>(&values), values.size());
}
void c_c() {

	static int config_index = -1;
	static char buffer[10];
	c::text_input(("Config name"), buffer, sizeof(buffer));

	if (settings::configs.size() > 0) {
		ImGui::SetCursorPosX(10);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(20 / 255.f, 20 / 255.f, 20 / 255.f, ImGui::GetStyle().Alpha));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 0.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 4));
		ImGui::GetStyle().PopupRounding = 0.f;
		if (ImGui::BeginCombo((" "), config_index != -1 ? settings::configs.at(config_index).c_str() : "Configs")) {
			const std::vector<std::string>& configs_vector = settings::configs;
			for (std::size_t i = 0u; i < configs_vector.size(); ++i) {
				std::string const& config_name = configs_vector[i].data();
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
				if (ImGui::Selectable(config_name.c_str(), i == config_index)) {
					config_index = i;
				}
				ImGui::PopStyleVar();
			}
			ImGui::EndCombo();
		}
		ImGui::PopStyleVar();
		ImGui::GetStyle().PopupRounding = 3.f;
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
	if (c::button(("Create config"))) {
		settings::create_file(buffer);
		interfaces::chat_element->chat_print_f(std::format(xorstr_("\x08 \x0C divinity \x08~ created config \x0C{}"), buffer).data());
		notifications::notify(std::format(xorstr_("created config {}."), buffer).data());
	}
	if (config_index != -1 && config_index < static_cast<int>(settings::configs.size())) {
		if (c::button(("Load selected config"))) {
			interfaces::chat_element->chat_print_f(std::format(xorstr_("\x08 \x0C divinity \x08~ loaded config \x0C{}"), settings::configs.at(config_index)).data());
			notifications::notify(std::format(xorstr_("loaded config {}."), settings::configs.at(config_index)).data());
			settings::load(config_index);
			utilities::do_forceupdate = true;
		}
		if (c::button(("Save selected config"))) {
			interfaces::chat_element->chat_print_f(std::format(xorstr_("\x08 \x0C divinity \x08~ saved config \x0C{}"), settings::configs.at(config_index)).data());
			notifications::notify(std::format(xorstr_("saved config {}."), settings::configs.at(config_index)).data());
			settings::save(config_index);
			utilities::do_forceupdate = true;
		}
	}
	if (c::button(("Refresh config list"))) {
		settings::update_configs();
	}
	if (c::button(xorstr_("Open config area"))) {
		ShellExecute(0, 0, "divinity", 0, 0, SW_SHOW);
	}
}
static int weapon_index = 7;
static int weapon_vector_index = 0;
struct weapon_name_t {
	constexpr weapon_name_t(int32_t definition_index, const char* name) : definition_index(definition_index), name(name) { }

	int32_t definition_index = 0;
	const char* name = nullptr;
};

std::vector<weapon_name_t> weapon_names = {
	{ WEAPON_AK47, "ak-47" },
	{ WEAPON_AUG, "aug" },
	{ WEAPON_AWP, "awp" },
	{ WEAPON_CZ75A, "cz75 auto" },
	{ WEAPON_DEAGLE, "desert eagle" },
	{ WEAPON_ELITE, "dual berettas" },
	{ WEAPON_FAMAS, "famas" },
	{ WEAPON_FIVESEVEN, "five-seveN" },
	{ WEAPON_G3SG1, "g3sg1" },
	{ WEAPON_GALILAR, "galil ar" },
	{ WEAPON_GLOCK, "glock-18" },
	{ WEAPON_M249, "m249" },
	{ WEAPON_M4A1_SILENCER, "m4a1-s" },
	{ WEAPON_M4A1, "m4a4" },
	{ WEAPON_MAC10, "mac-10" },
	{ WEAPON_MAG7, "mag-7" },
	{ WEAPON_MP7, "mp7" },
	{ WEAPON_MP5SD, "mp5" },
	{ WEAPON_MP9, "mp9" },
	{ WEAPON_NEGEV, "negev" },
	{ WEAPON_NOVA, "nova" },
	{ WEAPON_HKP2000, "p2000" },
	{ WEAPON_P250, "p250" },
	{ WEAPON_P90, "p90" },
	{ WEAPON_BIZON, "pp-bizon" },
	{ WEAPON_REVOLVER, "r8 revolver" },
	{ WEAPON_SAWEDOFF, "sawed-off" },
	{ WEAPON_SCAR20, "scar-20" },
	{ WEAPON_SSG08, "ssg 08" },
	{ WEAPON_SG556, "sg 553" },
	{ WEAPON_TEC9, "tec-9" },
	{ WEAPON_UMP45, "ump-45" },
	{ WEAPON_USP_SILENCER, "usp-s" },
	{ WEAPON_XM1014, "xm1014" },
};
int weapon_group;
void draw_aimbot_tab() {
	static const char* aimbot_type[] = { "Key held", "On fire" };
	static const char* hitboxes[4] = { "Head", "Chest", "Hands", "Legs" };
	static const char* trigger_type[] = { "Key", "Always" };
	const char* weapon_groups[] = { "General", "Rifle", "Sniper", "Pistol", "Smg", "Heavy" };



	c::begin(xorstr_("Main"), false, true);
	{
		c::checkbox(xorstr_("Enable aimbot"), &settings::aimbot::c_enabled[weapon_group]); c::keybind(xorstr_("aimbotkey"), settings::aimbot::aim_key);
		c::checkbox(xorstr_("Visible only"), &settings::aimbot::visible_only);
		c::checkbox(xorstr_("Silent aim"), &settings::aimbot::silent);
		c::dropdown(xorstr_("Target type"), settings::aimbot::type, aimbot_type, ARRAYSIZE(aimbot_type));
		c::checkbox(xorstr_("Auto pistol"), &settings::aimbot::auto_pistol);
		c::slider_int(xorstr_("Kill delay"), settings::aimbot::delay_after_kill, 0, 350);
	} c::end();

	c::begin(xorstr_("Settings"), true, true);
	{
		c::dropdown(xorstr_("Weapon group"), weapon_group, weapon_groups, ARRAYSIZE(weapon_groups));
		switch (weapon_group) {
		case WEAPONS_GENERAL:
			c::multi_select(xorstr_("Hitboxes"), settings::aimbot::general_hitboxes, hitboxes, ARRAYSIZE(hitboxes));
			c::slider_float(xorstr_("Fov"), settings::aimbot::c_fov[WEAPONS_GENERAL], 1.f, settings::misc::settings_unsafe_features ? 180.f : 25.f);
			c::slider_float(xorstr_("Smoothness"), settings::aimbot::c_smooth[WEAPONS_GENERAL], 1.f, 90.f);
			c::slider_float(("Recoil compensation"), settings::aimbot::c_rcs[WEAPONS_GENERAL], 0.f, 1.f);
	
			break;
		case WEAPONS_RIFLE:
			c::multi_select(xorstr_("Hitboxes"), settings::aimbot::rifle_hitboxes, hitboxes, ARRAYSIZE(hitboxes));
			c::slider_float(xorstr_("Fov"), settings::aimbot::c_fov[WEAPONS_RIFLE], 1.f, settings::misc::settings_unsafe_features ? 180.f : 25.f);
			c::slider_float(xorstr_("Smoothness"), settings::aimbot::c_smooth[WEAPONS_RIFLE], 1.f, 90.f);
			c::slider_float(xorstr_("Recoil compensation"), settings::aimbot::c_rcs[WEAPONS_RIFLE], 0.f, 1.f);

			break;
		case WEAPONS_SNIPER:
			c::multi_select(xorstr_("Hitboxes"), settings::aimbot::sniper_hitboxes, hitboxes, ARRAYSIZE(hitboxes));
			c::slider_float(xorstr_("Fov"), settings::aimbot::c_fov[WEAPONS_SNIPER], 1.f, settings::misc::settings_unsafe_features ? 180.f : 25.f);
			c::slider_float(xorstr_("Smoothness"), settings::aimbot::c_smooth[WEAPONS_SNIPER], 1.f, 90.f);
			c::slider_float(xorstr_("Recoil compensation"), settings::aimbot::c_rcs[WEAPONS_SNIPER], 0.f, 1.f);

			break;
		case WEAPONS_PISTOL:
			c::multi_select(xorstr_("Hitboxes"), settings::aimbot::pistol_hitboxes, hitboxes, ARRAYSIZE(hitboxes));
			c::slider_float(xorstr_("Fov"), settings::aimbot::c_fov[WEAPONS_PISTOL], 1.f, settings::misc::settings_unsafe_features ? 180.f : 25.f);
			c::slider_float(xorstr_("Smoothness"), settings::aimbot::c_smooth[WEAPONS_PISTOL], 1.f, 90.f);
			c::slider_float(xorstr_("Recoil compensation"), settings::aimbot::c_rcs[WEAPONS_PISTOL], 0.f, 1.f);

			break;
		case WEAPONS_SMG:
			c::multi_select(xorstr_("Hitboxes"), settings::aimbot::smg_hitboxes, hitboxes, ARRAYSIZE(hitboxes));
			c::slider_float(xorstr_("Fov"), settings::aimbot::c_fov[WEAPONS_SMG], 1.f, settings::misc::settings_unsafe_features ? 180.f : 25.f);
			c::slider_float(xorstr_("Smoothness"), settings::aimbot::c_smooth[WEAPONS_SMG], 1.f, 90.f);
			c::slider_float(xorstr_("Recoil compensation"), settings::aimbot::c_rcs[WEAPONS_SMG], 0.f, 1.f);

			break;
		case WEAPONS_HEAVY:
			c::multi_select(xorstr_("Hitboxes"), settings::aimbot::heavy_hitboxes, hitboxes, ARRAYSIZE(hitboxes));
			c::slider_float(xorstr_("Fov"), settings::aimbot::c_fov[WEAPONS_HEAVY], 1.f, settings::misc::settings_unsafe_features ? 180.f : 25.f);
			c::slider_float(xorstr_("Smoothness"), settings::aimbot::c_smooth[WEAPONS_HEAVY], 1.f, 90.f);
			c::slider_float(xorstr_("Recoil compensation"), settings::aimbot::c_rcs[WEAPONS_HEAVY], 0.f, 1.f);
		
			break;
		}

	} c::end();

	c::begin(xorstr_("Other"), false);
	{
		c::checkbox(xorstr_("Backtrack"), &settings::aimbot::backtrack_enable);
		/*	c::checkbox ( xorstr_("Fake ping"), &settings::aimbot::backtrack_ping_spike );
			if ( settings::aimbot::backtrack_ping_spike )
				c::slider_int ( xorstr_("Fake ping amount"), settings::aimbot::backtrack_ping_spike_value, 0, 200, "%d ms" );*/
		c::checkbox(xorstr_("Target teammates"), &settings::aimbot::target_team);
		c::checkbox(xorstr_("Target through smoke"), &settings::aimbot::target_smoke);
		c::checkbox(xorstr_("Target while jumping"), &settings::aimbot::target_in_air);
		c::checkbox(xorstr_("Scoping required"), &settings::aimbot::scoping_required);
	} c::end();

	/*c::begin (xorstr_( "Triggerbot"), true );
	{
		c::checkbox ( xorstr_("Enable##trigger_enable"), &settings::aimbot::triggerbot_enable ); if ( settings::aimbot::trigger_type == 0 ) c::keybind (xorstr_ ("triggerkey"), settings::aimbot::trigger_key );
		c::dropdown (xorstr_( "Trigger type"), settings::aimbot::trigger_type, trigger_type, ARRAYSIZE ( trigger_type ) );
		c::slider_float ( xorstr_("Hitchance"), settings::aimbot::trigger_ht, 1.f, 99.f, "%.2f " );
		c::slider_int ( xorstr_("Shoot delay"), settings::aimbot::trigger_delay, 0, 50, "%d ms" );
	} c::end ();*/
}
const char* _box_types[] = { "Outline", "Inline", "Standard", "Cornered" };
const char* bt_cham_types[] = { "Last tick", "All tick static", "All tick gradient" };
const char* box_line_types[] = { "Outline", "Inline" };
const char* glow_types[] = { "Basic", "Pulse", "Rim" };
void draw_players_tab() {
	c::begin(xorstr_("Player"), false, true);
	{
		c::checkbox(xorstr_("Enable##esp_enable"), &settings::visuals::enable_esp);
		if (settings::visuals::enable_esp) {
			c::checkbox(xorstr_("Engine radar"), &settings::visuals::engine_radar);
			c::checkbox(xorstr_("Name"), &settings::visuals::visuals_name);
			c::checkbox(xorstr_("Flash kill"), &settings::visuals::flash_kill);
			c::checkbox(xorstr_("Box"), &settings::visuals::visuals_box);
			if (settings::visuals::visuals_box) {
				c::slider_float(xorstr_("Box filled amount"), settings::visuals::box_vertical, 0.f, 100.f);
			}
			c::checkbox(xorstr_("Out of FOV arrows"), &settings::visuals::oof_arrows); c::color_picker(xorstr_("oofarrowcol"), settings::visuals::oof_arrow_col1, true, settings::visuals::oof_arrow_col2, true, settings::visuals::oof_arrow_col3);
			if (settings::visuals::oof_arrows) {
				c::slider_float(xorstr_("Arrow range"), settings::visuals::oof_arrow_range, 0.f, 100.f);
				c::slider_float(xorstr_("Arrow size"), settings::visuals::oof_arrow_size, 0.f, 100.f);
			}
			c::checkbox(xorstr_("Health"), &settings::visuals::visuals_health);
			if (settings::visuals::visuals_health) {
				c::checkbox(xorstr_("Gradient healthbar"), &settings::visuals::gradient_healthbar); c::color_picker(xorstr_("gradienthealthcols"), settings::visuals::gradient_healthbar_col1, true, settings::visuals::gradient_healthbar_col2);
			}
			c::checkbox(xorstr_("Flags"), &settings::visuals::visuals_flags);
			c::checkbox(xorstr_("Uppercase flag names"), &settings::visuals::visuals_flags_uppercase);
			c::checkbox(xorstr_("Skeleton"), &settings::visuals::visuals_skeleton);
			c::checkbox(xorstr_("Armor"), &settings::visuals::visuals_armor);
			c::checkbox(xorstr_("Weapon"), &settings::visuals::visuals_weapon);
		}
	} c::end();

	c::begin(xorstr_("Chams"), true, true);
	{
		c::checkbox(xorstr_("Visible"), &settings::visuals::enable_chams_vis); c::color_picker(xorstr_("chamsviscol"), settings::visuals::chams_vis);
		if (settings::visuals::enable_chams_vis) {
			c::dropdown(xorstr_("Visible material"), settings::visuals::player_material, chams_vis, ARRAYSIZE(chams_vis));

			c::checkbox(xorstr_("Invisible"), &settings::visuals::enable_chams_invis); c::color_picker(xorstr_("chamsinviscol"), settings::visuals::chams_invis);
			if (settings::visuals::enable_chams_invis)
				c::dropdown(xorstr_("Invisible material"), settings::visuals::player_material_invis, chams_invis, ARRAYSIZE(chams_invis));


			c::checkbox(xorstr_("Overlay"), &settings::visuals::chams_overlay); c::color_picker(xorstr_("chamsoverlaycol"), settings::visuals::chams_overlay_clr);
			if (settings::visuals::chams_overlay)
				c::dropdown(xorstr_("Overlay material"), settings::visuals::player_overlay_material, chams_overlay, ARRAYSIZE(chams_overlay));

		}


		c::checkbox(xorstr_("Backtrack"), &settings::visuals::backtrack_chams); c::color_picker(xorstr_("chamsbtcol"), settings::visuals::backtrack_chams_color, settings::visuals::backtrack_render_type == 2, settings::visuals::backtrack_chams_color2);
		if (settings::visuals::backtrack_chams) {
			c::dropdown(xorstr_("Chams type"), settings::visuals::backtrack_render_type, bt_cham_types, ARRAYSIZE(bt_cham_types));
			c::dropdown(xorstr_("Backtrack material"), settings::visuals::backtrack_chams_type, chams_invis, ARRAYSIZE(chams_invis));
		}

		c::checkbox(xorstr_("Arms"), &settings::visuals::arm_chams); c::color_picker(xorstr_("armchamscol"), settings::visuals::arm_chams_color);
		if (settings::visuals::arm_chams) {
			c::dropdown(xorstr_("Arms material"), settings::visuals::arm_chams_material, chams_invis, ARRAYSIZE(chams_invis));
			c::checkbox(xorstr_("Arms overlay"), &settings::visuals::arm_overlay_chams); c::color_picker(xorstr_("armoverlaycol"), settings::visuals::arm_overlay_color);
			if (settings::visuals::arm_overlay_chams)
				c::dropdown(xorstr_("Arms overlay material"), settings::visuals::arm_overlay_material, chams_overlay, ARRAYSIZE(chams_overlay));
		}
		c::checkbox(xorstr_("Weapon"), &settings::visuals::weapon_chams); c::color_picker(xorstr_("Weaponchamscol"), settings::visuals::weapon_chams_color);
		if (settings::visuals::weapon_chams) {
			c::dropdown(xorstr_("Weapon material"), settings::visuals::weapon_chams_material, chams_invis, ARRAYSIZE(chams_invis));
			c::checkbox(xorstr_("Weapon overlay"), &settings::visuals::weapon_overlay_chams); c::color_picker(xorstr_("Weaponoverlaychamscol"), settings::visuals::weapon_chams_overlay_color);
			if (settings::visuals::weapon_overlay_chams)
				c::dropdown(xorstr_("Weapon overlay material"), settings::visuals::weapon_overlay_material, chams_overlay, ARRAYSIZE(chams_overlay));
		}


	} c::end();


	c::begin(xorstr_("Glow"), false);
	{
		c::checkbox(xorstr_("Enable##glow_esp1"), &settings::visuals::glow_enabled);
		if (settings::edgebug_effects::effect_on_eb == 2 && settings::edgebug_effects::eb_effect) {
			ImGui::SameLine();
			ImGui::Text("?");
			if (ImGui::IsItemHovered()) {
				ImGui::BeginTooltip();
				ImGui::Text("Bloom is currently enabled. If you want to use glow, disable 'Effect on eb' in Visuals->Indicators.");
				ImGui::EndTooltip();
			}
		}
		if (settings::visuals::glow_enabled) {
			c::checkbox(xorstr_("Players##glow_esp2"), &settings::visuals::glow_players); c::color_picker(xorstr_("playerglowcol"), settings::visuals::glow_player_col, true, settings::visuals::glow_player_invis_col);
			if (settings::visuals::glow_players)
				c::dropdown(xorstr_("Player glow type"), settings::visuals::glow_type_player, glow_types, ARRAYSIZE(glow_types));
			c::checkbox(xorstr_("Chickens##glow_esp3"), &settings::visuals::glow_chickens); c::color_picker(xorstr_("chickenglowcol"), settings::visuals::glow_chicken_col);
			if (settings::visuals::glow_chickens)
				c::dropdown(xorstr_("Chickens glow type"), settings::visuals::glow_type_chicken, glow_types, ARRAYSIZE(glow_types));
			c::checkbox(xorstr_("Bomb##glow_esp4"), &settings::visuals::glow_bombs); c::color_picker(xorstr_("bombglowcol"), settings::visuals::glow_bomb_col);
			if (settings::visuals::glow_bombs)
				c::dropdown(xorstr_("Bomb glow type"), settings::visuals::glow_type_bomb, glow_types, ARRAYSIZE(glow_types));
			c::checkbox(xorstr_("Weapons##glow_esp3"), &settings::visuals::glow_weapons); c::color_picker(xorstr_("weaponsglowcol"), settings::visuals::glow_weapons_col);
			if (settings::visuals::glow_weapons)
				c::dropdown(xorstr_("Weapons glow type"), settings::visuals::glow_type_weapons, glow_types, ARRAYSIZE(glow_types));
		}

	} c::end();

	c::begin(xorstr_("Dropped"), true);
	{
		c::checkbox(xorstr_("Dropped weapons"), &settings::visuals::weapon_esp); c::color_picker(xorstr_("droppedweaponscol"), settings::visuals::weapon_esp_col);
		if (settings::visuals::weapon_esp) {
			c::slider_float(xorstr_("Max distance"), settings::visuals::weapon_esp_distance, 150.f, 2000.f);
			c::checkbox(xorstr_("Weapons name"), &settings::visuals::weapon_name);
			c::checkbox(xorstr_("Weapons box"), &settings::visuals::weapon_box);
			if (settings::visuals::weapon_box)
				c::slider_float(xorstr_("Filled amount"), settings::visuals::weapon_box_vertical, 0.f, 100.f);
			c::checkbox(xorstr_("Weapons ammo"), &settings::visuals::weapon_ammo);

			c::checkbox(xorstr_("Dropped bomb"), &settings::visuals::dropped_c4); c::color_picker(xorstr_("droppedc4col"), settings::visuals::dropped_c4_col);
		}

	} c::end();
}
void draw_visuals_tab() {
	const char* trail_types[]{
		"Static",
		"Fluctuate",
		"LGBT"
	};
	const char* eb_effects[]{
		"Healthboost",
		"Dangerzone",
		"Bloom"
	};
	const char* velo_alpha_types[]{ "Static", "Fade via speed" };
	const char* velo_color_types[]{ "Static", "Gain", "Speed" };
	const char* graph_styles[]{ "Static", "Fade by time", "Fade by amount" };
	const char* skyboxes[]{ "cs_baggage_skybox", "cs_tibet", "clearsky", "clearsky_hdr", "embassy", "italy", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night02", "sky_csgo_night02b", "sky_csgo_night_flat", "sky_day02_05_hdr", "sky_day02_05", "sky_l4d_rural02_ldr", "vertigo_hdr", "vertigoblue_hdr", "vertigo", "vietnam", "sky_dust", "jungle", "nukeblank", "office", "sky_lunacy" };
	c::begin(xorstr_("World"), false, true);
	{
		c::checkbox(xorstr_("World modulation"), &settings::visuals::world_modulation); c::color_picker(xorstr_("worldmodulationcol"), settings::visuals::world_modulation_clr);
		c::checkbox(xorstr_("Prop modulation"), &settings::visuals::prop_modulation); c::color_picker(xorstr_("propmodulationcol"), settings::visuals::prop_modulation_clr);
	//	c::dropdown(xorstr_("Skybox changer"), settings::visuals::skybox_changer_index, skyboxes, ARRAYSIZE(skyboxes));
		c::checkbox(xorstr_("Fog modulation"), &settings::visuals::fog); c::color_picker(xorstr_("fogcol"), settings::visuals::fog_clr);
		if (settings::visuals::fog) {
			c::slider_int(xorstr_("Fog start"), settings::visuals::fog_start, 0, 1500);
			c::slider_int(xorstr_("Fog end"), settings::visuals::fog_end, 0, 2500);
			c::slider_int(xorstr_("Fog density"), settings::visuals::fog_density, 0, 100);
		}
		c::checkbox(xorstr_("Modulate molotov"), &settings::visuals::modulate_molotov); c::color_picker(xorstr_("molocol"), settings::visuals::modulate_molotov_clr);
		c::checkbox(xorstr_("Wireframe molotov"), &settings::visuals::wireframe_fire);
		c::checkbox(xorstr_("Modulate smoke"), &settings::visuals::modulate_smoke); c::color_picker(xorstr_("smokecol"), settings::visuals::modulate_smoke_clr);
		c::checkbox(xorstr_("Wireframe smoke"), &settings::visuals::wireframe_smoke);
		c::checkbox(xorstr_("Modulate blood"), &settings::visuals::modulate_blood); c::color_picker(xorstr_("bloodcol"), settings::visuals::modulate_blood_clr);
		c::checkbox(xorstr_("Modulate weapon effects"), &settings::visuals::modulate_weapon_effects); c::color_picker(xorstr_("weaponeffectscol"), settings::visuals::modulate_weapon_effects_clr);

	} c::end();

	c::begin(xorstr_("Local"), false);
	{
		c::checkbox(xorstr_("Hitsound"), &settings::visuals::hitsound);
		c::checkbox(xorstr_("Killsound"), &settings::visuals::killsound);
		c::checkbox(xorstr_("Hit logs"), &settings::misc::hitlogs);
		if (settings::misc::hitlogs)
			c::dropdown(xorstr_("hitlog print type"), settings::misc::hitlogs_print_type, print_types, ARRAYSIZE(print_types));
		c::checkbox(xorstr_("Grenade prediction"), &settings::visuals::grenade_prediction); c::color_picker(xorstr_("gnadepredcol"), settings::visuals::grenade_prediction_clr, true, settings::visuals::grenade_prediction_clr2);
		c::checkbox(xorstr_("Spectators list"), &settings::visuals::spectator_list);
		c::checkbox(xorstr_("Force crosshair"), &settings::visuals::force_crosshair);
		c::checkbox(xorstr_("Custom viewmodel"), &settings::visuals::viewmodel);
		if (settings::visuals::viewmodel) {
			c::slider_float(xorstr_("Viewmodel x"), settings::visuals::viewmodel_offset_x, -10, 10);
			c::slider_float(xorstr_("Viewmodel y"), settings::visuals::viewmodel_offset_y, -10, 10);
			c::slider_float(xorstr_("Viewmodel z"), settings::visuals::viewmodel_offset_z, -10, 10);
		}
		c::checkbox(xorstr_("Disable flash"), &settings::visuals::visuals_disableflash);
		c::checkbox(xorstr_("Disable panaorama blur"), &settings::visuals::visuals_disable_panorama_blur);
		c::checkbox(xorstr_("Motion blur"), &settings::motion_blur::enabled);
		if (settings::motion_blur::enabled) {
			c::checkbox(xorstr_("Forward blur"), &settings::motion_blur::foward_blur);
			c::slider_float(xorstr_("Rotation intensity"), settings::motion_blur::rotation_intensity, 0.f, 5.f);
			c::slider_float(xorstr_("Roll intensity"), settings::motion_blur::blur_strength, 0.f, 5.f);
			c::slider_float(xorstr_("Falling min"), settings::motion_blur::falling_min, 1.f, 20.f);
			c::slider_float(xorstr_("Falling max"), settings::motion_blur::falling_max, 1.f, 40.f);
		}
		c::checkbox(xorstr_("Aspect ratio"), &settings::misc::aspect_ratio_changer);
		if (settings::misc::aspect_ratio_changer)
			c::slider_float(xorstr_("Ratio amount"), settings::misc::aspect_ratio_changer_value, 0.f, 90.f);

		c::checkbox(xorstr_("Movement trail"), &settings::misc::trails::enable); if (settings::misc::trails::color_type != 2) { c::color_picker(xorstr_("trailcol"), settings::misc::trails::color_type == 1 ? settings::misc::trails::fluc_col1 : settings::misc::trails::col, settings::misc::trails::color_type == 1, settings::misc::trails::fluc_col2); }
		if (settings::misc::trails::enable) {
			c::dropdown(xorstr_("Render color"), settings::misc::trails::color_type, trail_types, ARRAYSIZE(trail_types));
			if (settings::misc::trails::color_type == 2)
				c::slider_float(xorstr_("Rainbow speed"), settings::misc::trails::rainbow_speed, 0.f, 5.f);
			c::slider_float(xorstr_("Trail life"), settings::misc::trails::life, 0.f, 6.f);
			c::slider_float(xorstr_("Trail life speed"), settings::misc::trails::life_speed, 0.f, 2.f);
			c::slider_float(xorstr_("Trail width"), settings::misc::trails::width, 0.f, 5.f);
		}

	} c::end();

	c::begin(xorstr_("Indicators"), true, true, true);
	{
		c::checkbox(xorstr_("Velocity"), &settings::indicators::velo); c::color_picker(xorstr_("velocol"), settings::indicators::velo_col);
		if (settings::indicators::velo) {
			c::checkbox(xorstr_("Velocity takeoff"), &settings::indicators::velo_takeoff);
			c::dropdown(xorstr_("Velocity alpha style"), settings::indicators::velo_alpha_type, velo_alpha_types, ARRAYSIZE(velo_alpha_types));
			//if (settings::indicators::velo_alpha_type == 0) { // static

			c::dropdown(xorstr_("Velocity color style"), settings::indicators::velo_color_type, velo_color_types, ARRAYSIZE(velo_color_types));
			//}
		}
		c::checkbox(xorstr_("Stamina"), &settings::indicators::stamina); c::color_picker(xorstr_("staminacol"), settings::indicators::stamina_col);
		if (settings::indicators::stamina) {
			c::dropdown(xorstr_("Stamina alpha style"), settings::indicators::stamina_alpha_type, velo_alpha_types, ARRAYSIZE(velo_alpha_types));
			c::checkbox(xorstr_("Stamina takeoff"), &settings::indicators::stamina_takeoff);
			//			c::checkbox ( xorstr_("Round off stamina"), &settings::indicators::round_stamina );
		}
		c::checkbox(xorstr_("Velocity graph"), &settings::indicators::graph_velocity); c::color_picker(xorstr_("velographcol"), settings::indicators::graph_velo_col);
		c::checkbox(xorstr_("Stamina graph"), &settings::indicators::graph_stamina); c::color_picker(xorstr_("staminagraphcol"), settings::indicators::graph_stamina_col);
		if (settings::indicators::graph_velocity || settings::indicators::graph_stamina) {
			c::checkbox(xorstr_("Bold graph font"), &settings::indicators::graph_bold_font);
			c::dropdown(xorstr_("Velocity graph render style"), settings::indicators::velo_graph_style, graph_styles, ARRAYSIZE(graph_styles));
			c::dropdown(xorstr_("Stamina graph render style"), settings::indicators::stamina_graph_style, graph_styles, ARRAYSIZE(graph_styles));
			c::slider_int(xorstr_("Graph position"), settings::indicators::graph_position, 0, 600);
			c::slider_int(xorstr_("Graph size"), settings::indicators::velo_graph_size, 10, 160);
			c::slider_float(xorstr_("Graph scale"), settings::indicators::velo_graph_scale, 1, 15.f);
			c::slider_int(xorstr_("Graph thickness"), settings::indicators::graph_thickness, 1, 4);
			c::checkbox(xorstr_("Jump velocity on graph"), &settings::indicators::graph_jump_velo);
			c::checkbox(xorstr_("Land velocity on graph"), &settings::indicators::graph_land_velo);
			c::checkbox(xorstr_("Detected edgebug on graph"), &settings::indicators::graph_eb);
			c::checkbox(xorstr_("Detected jumpbug on graph"), &settings::indicators::graph_jb);
		//	c::checkbox(xorstr_("Detected pixelsurf on graph"), &settings::indicators::graph_ps);
		}

		c::checkbox(xorstr_("Edgebug indicator"), &settings::indicators::eb); c::color_picker(xorstr_("ebcols"), settings::indicators::eb_rage_col, true, settings::indicators::eb_default_col);
		//c::checkbox(xorstr_("Pixelsurf indicator"), &settings::indicators::ps); c::color_picker(xorstr_("pscols"), settings::indicators::ps_rage_col, true, settings::indicators::ps_default_col);
		c::checkbox(xorstr_("Ladderbug indicator"), &settings::indicators::lb); c::color_picker(xorstr_("lbcols"), settings::indicators::lb_rage_col, true, settings::indicators::lb_default_col);
		c::checkbox(xorstr_("Jumpbug indicator"), &settings::indicators::jb);  c::color_picker(xorstr_("jbcols"), settings::indicators::jb_rage_col, true, settings::indicators::jb_col);
		c::checkbox(xorstr_("Edgejump indicator"), &settings::indicators::ej); c::color_picker(xorstr_("ejcols"), settings::indicators::ej_rage_col, true, settings::indicators::ej_col);
		c::slider_int(xorstr_("Indicator position"), settings::indicators::position, 0, 600);
		c::checkbox(xorstr_("Key strokes"), &settings::indicators::key_strokes); c::color_picker(xorstr_("keystrokecol"), settings::indicators::key_strokes_col);
		if (settings::indicators::key_strokes) {
			c::slider_int(xorstr_("Key stroke position position"), settings::indicators::key_strokes_position, 0, ImGui::GetIO().DisplaySize.y);
			c::checkbox(xorstr_("Key stroke fadeout"), &settings::indicators::key_strokes_fadeout);
			c::checkbox(xorstr_("Key stroke stances"), &settings::indicators::key_strokes_stances);
		}
		c::checkbox(xorstr_("Jumpshot information"), &settings::movement::jumpshot_info);
		if (settings::movement::jumpshot_info) {
			c::checkbox(xorstr_("Jumpshot hit effect"), &settings::visuals::hit_effect);
			c::checkbox(xorstr_("Print slurs on jumpshot"), &settings::movement::jumpshot_slurs);
		}
		c::checkbox(xorstr_("Print edgebug information"), &settings::edgebug_effects::print_eb_info);
		if (settings::edgebug_effects::print_eb_info)
			c::dropdown(xorstr_("Edgebug print type"), settings::edgebug_effects::print_eb_info_print_type, print_types, ARRAYSIZE(print_types));
		c::checkbox(xorstr_("Effect on eb"), &settings::edgebug_effects::eb_effect);
		if (settings::edgebug_effects::eb_effect) {
			c::dropdown(xorstr_("Effect"), settings::edgebug_effects::effect_on_eb, eb_effects, ARRAYSIZE(eb_effects)); 
			if (settings::edgebug_effects::effect_on_eb == 2 && settings::edgebug_effects::eb_effect) {
				ImGui::SameLine();
				ImGui::Text("?");
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("Having bloom enabled disables your main glow in Players->Glow.");
					ImGui::EndTooltip();
				}
			}
			
			c::slider_float(xorstr_("Effect time"), settings::edgebug_effects::effect_time, 0.f, 10.f);
		}


		c::checkbox(xorstr_("Sound on eb"), &settings::edgebug_effects::sound_on_eb);
		c::checkbox(xorstr_("Visualize edgebug"), &settings::visulisation::visualize_eb);
		if (settings::visulisation::visualize_eb) {
			c::checkbox(xorstr_("Lightning while in eb"), &settings::visulisation::lightning_while_eb); c::color_picker(xorstr_("lightningebcol"), settings::visulisation::lightning_while_eb_col);
			c::checkbox(xorstr_("Sparks while in eb"), &settings::visulisation::sparks_while_eb);
			c::slider_float(xorstr_("Visualise time out"), settings::visulisation::vis_eb_fadeout_time, 0.f, 10.f, settings::visulisation::vis_eb_fadeout_time > 1 ? "%.2f seconds" : "%.2f second");
			c::checkbox(xorstr_("Text on eb"), &settings::visulisation::text_on_eb); c::color_picker(xorstr_("textonebcol"), settings::visulisation::text_on_eb_col);
			c::checkbox(xorstr_("Circle on eb"), &settings::visulisation::circle_on_eb); c::color_picker(xorstr_("circleonebcol"), settings::visulisation::circle_on_eb_col);
			c::checkbox(xorstr_("Show predicted circle"), &settings::visulisation::predicted_circle_on_eb); c::color_picker(xorstr_("predcircleonebcol"), settings::visulisation::predicted_circle_on_eb_col);
			c::checkbox(xorstr_("Show predicted circle 2d"), &settings::visulisation::predicted_eb_circle2d); c::color_picker(xorstr_("predcircleonebcol2d"), settings::visulisation::predicted_eb_circle2d_col);
			c::checkbox(xorstr_("Cube on eb"), &settings::visulisation::cube_on_eb); c::color_picker(xorstr_("cubeonebcol"), settings::visulisation::cube_on_eb_col);
			c::checkbox(xorstr_("Dlight on eb"), &settings::visulisation::dlight_on_eb); c::color_picker(xorstr_("dlightonebcol"), settings::visulisation::dlight_on_eb_col);
			if (settings::visulisation::dlight_on_eb) {
				c::slider_float(xorstr_("Dlight brightness"), settings::visulisation::dlight_on_eb_brightness, 0.f, 10.f);
				c::slider_int(xorstr_("Dlight radius"), settings::visulisation::dlight_on_eb_radius, 0, 200);
			}
		}
	} c::end();

}
LPDIRECT3DTEXTURE9* custom_image;
LPDIRECT3DTEXTURE9 image;
void CopyConvert(const uint8_t* rgba, uint8_t* out, const size_t size) {
	auto in = reinterpret_cast<const uint32_t*>(rgba);
	auto buf = reinterpret_cast<uint32_t*>(out);
	for (auto i = 0u; i < (size / 4); ++i) {
		const auto pixel = *in++;
		*buf++ = (pixel & 0xFF00FF00) | ((pixel & 0xFF0000) >> 16) | ((pixel & 0xFF) << 16);
	}
}

#include "../../source-sdk/steam-sdk/steamclientpublic.h"
#include "../../source-sdk/steam-sdk/isteamutils.h"
#include "../../source-sdk/steam-sdk/isteamfriends.h"
LPDIRECT3DTEXTURE9 steam_image_friends(CSteamID SteamId) {
	LPDIRECT3DTEXTURE9 asdgsdgadsg;



	int iImage = SteamFriends->GetLargeFriendAvatar(SteamId);
	if (iImage == -1)
		return nullptr;
	uint32 uAvatarWidth, uAvatarHeight;
	if (!SteamUtils->GetImageSize(iImage, &uAvatarWidth, &uAvatarHeight))
		return nullptr;
	const int uImageSizeInBytes = uAvatarWidth * uAvatarHeight * 4;
	uint8* pAvatarRGBA = new uint8[uImageSizeInBytes];
	if (!SteamUtils->GetImageRGBA(iImage, pAvatarRGBA, uImageSizeInBytes)) {
		delete[] pAvatarRGBA;
		return nullptr;
	}
	auto res = render::device->CreateTexture(uAvatarWidth,
		uAvatarHeight,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&asdgsdgadsg,
		nullptr);
	std::vector<uint8_t> texData;
	texData.resize(uAvatarWidth * uAvatarHeight * 4u);
	CopyConvert(pAvatarRGBA,
		texData.data(),
		uAvatarWidth * uAvatarHeight * 4u);
	D3DLOCKED_RECT rect;
	res = asdgsdgadsg->LockRect(0, &rect, nullptr, D3DLOCK_DISCARD);
	auto src = texData.data();
	auto dst = reinterpret_cast<uint8_t*>(rect.pBits);
	for (auto y = 0u; y < uAvatarHeight; ++y) {
		std::copy(src, src + (uAvatarWidth * 4), dst);

		src += uAvatarWidth * 4;
		dst += rect.Pitch;
	}
	res = asdgsdgadsg->UnlockRect(0);
	delete[] pAvatarRGBA;
	return asdgsdgadsg;
}
int t;
int active_t;
char input_skins_knife[256];
std::vector<const char*> knife_names;
#include "../../source-sdk/steam-sdk/steam_api.h"
#include "../post-processing/postprocessing.h"
void menu::draw_skins_tab() {
	c::begin(xorstr_("Knife"), false, true, true);
	{
		c::checkbox(xorstr_("Enable"), &settings::skins::knife_changer);
		c::dropdown(xorstr_("Knife type"), settings::skins::knife_model, knife_skins, ARRAYSIZE(knife_skins));

		ImGui::SetCursorPosX(10);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(20 / 255.f, 20 / 255.f, 20 / 255.f, ImGui::GetStyle().Alpha));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 0.f));
		ImGui::GetStyle().PopupRounding = 0.f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 4));
		ImGui::SetCursorPosX(10);
		static ImGuiTextFilter filter;
		ImGui::Text(xorstr_("knife skin"));
		ImGui::SetCursorPosX(10);
		filter.Draw(xorstr_("##search bar for knife skin"), 200);
		ImGui::SetCursorPosX(10);
		if (ImGui::BeginListBox(xorstr_("##theactualskinslol"), ImVec2(200, ImGui::GetCurrentWindow()->Size.y - 250))) {
			for (int i = 0; i < changers::parser::parser_skins.size(); i++) {
				std::string out_text = changers::parser::parser_skins[i].name.c_str();

				if (filter.PassFilter(out_text.c_str())) {
					std::string label = xorstr_("(") + std::to_string(i) + xorstr_(") ") + out_text;
					if (ImGui::Selectable(label.c_str(), settings::skins::knife_paintkit_index == changers::parser::parser_skins[i].id)) {
						settings::skins::knife_paintkit_index = changers::parser::parser_skins[i].id;
						last_saved_parsed_skin = i;
					}
				}
			} //ichigo is a nigger paster

			// Thanks mao.
			ImGui::EndListBox();
		}
		ImGui::PopStyleVar();
		ImGui::GetStyle().PopupRounding = 3.f;
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		c::slider_float(xorstr_("Skin wear"), settings::skins::knife_wear, 0.f, 1.f, "%.2f");
		ImGui::SetCursorPosX(10);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 3));
		ImGui::Text(xorstr_("Seed"));
		ImGui::SetCursorPosX(11);
		ImGui::PopStyleVar();
		ImGui::InputInt(xorstr_("##seed"), &settings::skins::kinfe_paintkit_seed);
		ImGui::GetStyle().FramePadding = ImVec2(5, 3);
		c::checkbox(xorstr_("Modulate knife skin"), &settings::skins::modulate_knife); c::color_picker(xorstr_("knifecol"), settings::skins::knife_color1, true, settings::skins::knife_color2, true, settings::skins::knife_color3, true, settings::skins::knife_color4);
		if (c::button(xorstr_("Force update"))) {

			utilities::do_forceupdate = true;
		}

	} c::end();

	c::begin(xorstr_("Glove"), true, true, true);
	{
		c::checkbox(xorstr_("Enable##glovechanger"), &settings::skins::glove_changer);
		c::dropdown(xorstr_("Glove type"), settings::skins::glove_model, glove_types, ARRAYSIZE(glove_types));
		ImGui::SetCursorPosX(10);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(20 / 255.f, 20 / 255.f, 20 / 255.f, ImGui::GetStyle().Alpha));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 0.f));
		ImGui::GetStyle().PopupRounding = 0.f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 4));
		ImGui::Text(xorstr_("Glove skin"));
		ImGui::SetCursorPosX(10);
		static ImGuiTextFilter filter;
		filter.Draw(xorstr_("##search bar for glove skin"), 200);
		ImGui::SetCursorPosX(10);
		if (ImGui::BeginListBox(xorstr_("##theactualgloveslol"), ImVec2(200, 210))) {
			for (int i = 0; i < changers::parser::parser_gloves.size(); i++) {
				std::string out_text = changers::parser::parser_gloves[i].name.c_str();

				if (filter.PassFilter(out_text.c_str())) {
					std::string label = xorstr_("(") + std::to_string(i) + xorstr_(") ") + out_text;
					if (ImGui::Selectable(label.c_str(), settings::skins::glove_paintkit_index == changers::parser::parser_gloves[i].id)) {
						settings::skins::glove_paintkit_index = changers::parser::parser_gloves[i].id;
					}
				}
			} //ichigo is a nigger paster

			// Thanks mao.
			ImGui::EndListBox();
		}
		/*ImGui::Combo(("##glove_skin"), &settings::skins::glove_vec_index, [](void* data, int idx, const char** out_text) {
			*out_text = changers::parser::parser_gloves[idx].name.c_str();
			return true;
			}, nullptr, changers::parser::parser_gloves.size(), 30);*/
		ImGui::PopStyleVar();
		ImGui::GetStyle().PopupRounding = 3.f;
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		//settings::skins::glove_paintkit_index = changers::parser::parser_gloves[settings::skins::glove_vec_index].id;

		c::slider_float(xorstr_("Glove wear"), settings::skins::glove_wear, 0.f, 1.f, "%.2f");
	} c::end();
}
void draw_misc_tab() {
	const char* data_center_list_names[] = { "Australia", "Austria", "Brazil", "Chile", "Dubai", "France", "Germany", "Hong Kong", "India (Chennai)", "India (Mumbai)", "Japan", "Luxembourg",
	"Netherlands", "Peru", "Philipines", "Poland", "Singapore", "South Africa", "Spain", "Sweden", "UK", "USA (Atlanta)", "USA (Seattle)", "USA (Chicago)", "USA (Los Angeles)", "USA (Moses Lake)",
	"USA (Oklahoma)", "USA (Seattle)", "USA (Washington DC)" };
	std::string data_center_list[] = { "syd", "vie", "gru", "scl", "dxb", "par", "fra", "hkg",
"maa", "bom", "tyo", "lux", "ams", "limc", "man", "waw", "sgp", "jnb",
"mad", "sto", "lhr", "atl", "eat", "ord", "lax", "mwh", "okc", "sea", "iad" };

	c::begin(xorstr_("Movement"), false, true);
	{
		c::checkbox(xorstr_("Bhop"), &settings::movement::bhop);
		c::slider_int(xorstr_("Bhop hitchance"), settings::movement::bhop_chance, 0, 100, "%d%");
		if (settings::misc::settings_unsafe_features)
			c::checkbox(xorstr_("Disable duck cooldown"), &settings::movement::no_duck_cooldown);
		c::checkbox(xorstr_("Edgejump"), &settings::movement::edgejump); c::keybind(xorstr_("ejkey"), settings::movement::edgejump_key);
		if (settings::movement::edgejump)
			c::checkbox(xorstr_("Longjump"), &settings::movement::longjump);
		c::checkbox(xorstr_("Quickstop"), &settings::movement::quickstop);
		c::checkbox(xorstr_("Jumpbug"), &settings::movement::jumpbug); c::keybind(xorstr_("jbkey"), settings::movement::jumpbug_key);
		c::checkbox(xorstr_("Ladderbug"), &settings::movement::ladder_bug); c::keybind(xorstr_("lbkey"), settings::movement::ladder_bug_key);
		c::checkbox(xorstr_("Minijump"), &settings::movement::minijump); c::keybind(xorstr_("mjkey"), settings::movement::minijump_key);
		c::checkbox(xorstr_("Strafe optimiser"), &settings::movement::strafe_optimiser);
		if (settings::movement::strafe_optimiser) {
			c::checkbox(xorstr_("Optimise on key"), &settings::movement::strafe_optimiser_on_key); c::keybind(xorstr_("strafeopkey"), settings::movement::strafe_optimiser_key);
			c::slider_float(xorstr_("Required speed"), settings::movement::required_speed, 0.f, 15.f);
			c::slider_float(xorstr_("Desired gain"), settings::movement::desired_gain, 0.f, 15.f);
		}
		c::checkbox(xorstr_("Pixelsurf"), &settings::movement::pixelsurf); c::keybind(xorstr_("pskey"), settings::movement::pixelsurf_key);
		c::checkbox(xorstr_("Edgebug"), &settings::movement::edgebug); c::keybind(xorstr_("ebkey"), settings::movement::edgebug_key);
		if (settings::movement::edgebug) {
			c::checkbox(xorstr_("Lock cursor"), &settings::movement::lock_on_eb);
			c::checkbox(xorstr_("Lock pitch"), &settings::movement::lock_pitch);
			c::checkbox(xorstr_("Edgebug strafe assistance"), &settings::movement::strafe_assistance);
			c::checkbox(xorstr_("Simulate auto strafe"), &settings::movement::simulate_as);
			c::slider_float(xorstr_("Lock strength"), settings::movement::lock_pull_amount, 0, 100);
			c::slider_int(xorstr_("Scan time"), settings::movement::edgebug_scan_time, 1, 64);
		}
	} c::end();

	c::begin(xorstr_("Configs"), true, true);
	{
		c_c();
	} c::end();


	c::begin(xorstr_("General"), false); {
		c::checkbox(xorstr_("Force matchmaking region"), &settings::misc::custom_region);
		if (settings::misc::custom_region)
			c::dropdown(xorstr_("Region"), settings::misc::custom_region_selection, data_center_list_names, IM_ARRAYSIZE(data_center_list_names));


		static std::string* force_relay_cluster_value = *(std::string**)(utilities::pattern_scan(xorstr_("steamnetworkingsockets.dll"), xorstr_("B8 ? ? ? ? B9 ? ? ? ? 0F 43")) + 1);
		if (settings::misc::custom_region && !interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
			*force_relay_cluster_value = data_center_list[settings::misc::custom_region_selection];
		else if (!settings::misc::custom_region && !interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
			*force_relay_cluster_value = "";


		c::checkbox(xorstr_("Auto accept"), &settings::misc::auto_accept);
		c::checkbox(xorstr_("Vote revealer"), &settings::misc::vote_revealer);
		//	if (settings::misc::vote_revealer)
		//		c::dropdown(xorstr_("Revealer print type"), settings::misc::vote_revealer_print_type, print_types, ARRAYSIZE(print_types));
		c::checkbox(xorstr_("Reverse ragdoll gravity"), &settings::misc::ragdoll_gravity);
		c::checkbox(xorstr_("Clantag"), &settings::misc::clantag);
		c::checkbox(xorstr_("Door spammer"), &settings::misc::door_spammer); c::keybind("doorspamkey", settings::misc::door_spammer_key);
		//c::checkbox(xorstr_("Auto jumpscout"), &settings::misc::jumpscout); c::keybind("jumpscoutkey", settings::misc::jumpscout_key);
		if (settings::misc::door_spammer) {
			c::slider_int(xorstr_("Door spam delay"), settings::misc::door_spammer_delay, 2, 10);
		}
		c::checkbox(xorstr_("Anti afk"), &settings::misc::anti_afk);
		c::checkbox(xorstr_("Rank revealer"), &settings::misc::reveal_ranks);
	} c::end();

	c::begin(xorstr_("Settings"), true);
	{
		c::text(xorstr_("Menu accent")); c::color_picker(xorstr_("menucol"), c::menu_accent);
		c::checkbox(xorstr_("Watermark"), &settings::misc::watermark);
		c::checkbox(xorstr_("Discord rich presence"), &settings::discord_rpc);
		c::checkbox(xorstr_("Stream proof ESP"), &settings::misc::stream_proof);
		c::checkbox(xorstr_("Menu background"), &settings::menu_bg);
		c::checkbox(xorstr_("Menu background particles"), &settings::menu_bg_particles);
		c::checkbox(xorstr_("Custom netgraph"), &settings::misc::custom_netgraph);
		//c::checkbox(xorstr_("Custom hud color"), &settings::misc::modulate_hud); c::color_picker(xorstr_("hudcol"), settings::misc::modulate_hud_col);
		if (settings::misc::custom_netgraph)
			c::checkbox(xorstr_("Modulate netgraph"), &settings::misc::modulate_netgraph);
		//c::checkbox(xorstr_("Display active Spotify song"), &settings::misc::spotify_on_watermark);
		c::checkbox(xorstr_("Show unsafe features"), &settings::misc::settings_unsafe_features);
		//if (c::button(xorstr_("Uninject"))) {
		//	/*	std::vector<std::string> error_messages = { "Unhandled exception: Error reading memory at 0x02", "Unhandled exception: The engine couldn't identify the mutual indecencies caused by memory at 0xc00C84", "Unhandled exception: Memory buffer overflow" };
		//		MessageBoxA(menu::hwnd, error_messages.at ( rand () % error_messages.size () ).c_str (), "Error!", MB_HELP | MB_ICONERROR );
		//		IM_ASSERT(error_messages.at(rand() % error_messages.size()).c_str());
		//		exit(-1);*/
		//	//Discord_ClearPresence();
		//	hooks::disable = true;
		//}
	} c::end();

}
void draw_lua_tab() {
	c::begin(xorstr_("Scripts"), false, true, true);
	{
		static int current_lua_item = -1;
		ImGui::SetCursorPosX(10);
		ImGui::Text(xorstr_("Scripts"));
		ImGui::SetCursorPosX(10);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(20 / 255.f, 20 / 255.f, 20 / 255.f, ImGui::GetStyle().Alpha));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(45 / 255.f, 45 / 255.f, 45 / 255.f, 0.f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 4));
		ImGui::GetStyle().PopupRounding = 0.f;
		ImGui::Combo(xorstr_("##niggerscripts"), &current_lua_item,
			[](void* vec, int idx, const char** out_text) {
				std::vector<std::string>* vector = reinterpret_cast<std::vector<std::string>*>(vec);
				if (idx < 0 || idx >= vector->size())return false;
				*out_text = vector->at(idx).c_str();
				return true;
			}, reinterpret_cast<void*>(&lua::scripts), lua::scripts.size());
		ImGui::GetStyle().PopupRounding = 3.f;
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		if (c::button(xorstr_("refresh"))) {
			lua::unload_all_scripts();
			lua::refresh_scripts();
		}
		if (c::button(xorstr_("Open lua area"))) {
			ShellExecute(0, 0, xorstr_("divinity/luas/"), 0, 0, SW_SHOW);
		}
		if (c::button(xorstr_("reload all")))
			lua::reload_all_scripts();

		if (c::button(xorstr_("unload All")))
			lua::unload_all_scripts();

		if (current_lua_item != -1) {
			if (lua::loaded[current_lua_item]) {
				if (c::button(xorstr_("unload selected")))
					lua::unload_script(current_lua_item);
			}
			else {
				if (c::button(xorstr_("load selected")))
					lua::load_script(current_lua_item);
			}

		}
	} c::end();
}

#define min(x,y) ((x)<(y)?x:y)
#define wh(a) ImColor(c::menu_accent[0], c::menu_accent[1], c::menu_accent[2], ImGui::GetStyle().Alpha * a)
void FX(ImDrawList* d, ImVec2 a, ImVec2 b, ImVec2 sz, float t) {
	auto start = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(start);
	bool december = strstr(std::ctime(&end_time), "Dec");
	int drop_size = december ? 3 : 10;
	float time = december ? t / 2 : t;
	static float fl;
	if ((rand() % 500) == 0) fl = time;
	for (int i = 0; i < 2000; ++i) {
		unsigned h = ImGui::GetID(d + i + int(time / 4));
		auto f = fmodf(time + fmodf(h / 777.f, 99), 99);
		auto tx = h % (int)sz.x;
		auto ty = h % (int)sz.y;
		if (f < 1) {
			auto py = ty - 1000 * (1 - f);
			d->AddLine({ a.x + tx, a.y + py }, { a.x + tx, a.y + min(py + drop_size,ty) }, ImColor(1.f, 1.f, 1.f, ImGui::GetStyle().Alpha), december ? 2.f : 1.f);
		}
		else if (f < 1.2f && !december)
			d->AddCircle({ a.x + tx, a.y + ty }, (f - 1) * 10 + h % 5, wh(1 - (f - 1) * 5.f));
	}
}
float menu_alpha = 0.f;
float aim_tab_alpha = 1.f;
float players_tab_alpha = 1.f;
float visuals_tab_alpha = 1.f;
float skins_tab_alpha = 1.f;
float misc_tab_alpha = 1.f;
float lua_tab_alpha = 1.f;
namespace menu {

	void menu::initialize() noexcept {

		auto& io{ ImGui::GetIO() };
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigWindowsMoveFromTitleBarOnly = false;


	}

	void menu::render() noexcept {
		float deltatime = (1.0f / 0.2f) * ImGui::GetIO().DeltaTime * 0.4f;
		float deltatime2 = (1.0f / 0.2f) * ImGui::GetIO().DeltaTime * 0.2;
		menu_alpha = ImClamp(menu_alpha + (3.f * deltatime * (menu::is_open ? 1.f : -1.f)), 0.0f, 1.f);

		if (!menu_alpha)
			return;

		custom_gui::create_style(ImGui::GetStyle());

		aim_tab_alpha = ImClamp(aim_tab_alpha + (3.f * deltatime2 * ((custom_gui::backend::cur_tab == 0) ? 1.f : -1.f)), 0.0f, 1.f);
		players_tab_alpha = ImClamp(players_tab_alpha + (3.f * deltatime2 * ((custom_gui::backend::cur_tab == 1) ? 1.f : -1.f)), 0.0f, 1.f);
		visuals_tab_alpha = ImClamp(visuals_tab_alpha + (3.f * deltatime2 * ((custom_gui::backend::cur_tab == 2) ? 1.f : -1.f)), 0.0f, 1.f);
		skins_tab_alpha = ImClamp(skins_tab_alpha + (3.f * deltatime2 * ((custom_gui::backend::cur_tab == 3) ? 1.f : -1.f)), 0.0f, 1.f);
		misc_tab_alpha = ImClamp(misc_tab_alpha + (3.f * deltatime2 * ((custom_gui::backend::cur_tab == 4) ? 1.f : -1.f)), 0.0f, 1.f);
		lua_tab_alpha = ImClamp(lua_tab_alpha + (3.f * deltatime2 * ((custom_gui::backend::cur_tab == 5) ? 1.f : -1.f)), 0.0f, 1.f);

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha);
		custom_gui::begin();
		{
			switch (custom_gui::backend::cur_tab) {
			case 0: // aimbot
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha * aim_tab_alpha);
				draw_aimbot_tab();
				ImGui::PopStyleVar();
				break;
			case 1: // players
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha * players_tab_alpha);
				draw_players_tab();
				ImGui::PopStyleVar();
				break;
			case 2: // visuals
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha * visuals_tab_alpha);
				draw_visuals_tab();
				ImGui::PopStyleVar();
				break;
			case 3: // skins
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha * skins_tab_alpha);
				draw_skins_tab();
				ImGui::PopStyleVar();
				break;
			case 4: // misc
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha * misc_tab_alpha);
				draw_misc_tab();
				ImGui::PopStyleVar();
				break;
			case 5: // scripts (WOAH)
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menu_alpha * lua_tab_alpha);
				draw_lua_tab();
				ImGui::PopStyleVar();
				break;
			}

		} custom_gui::end();
		ImGui::PopStyleVar();
		int a = menu_alpha * 125;
		custom_gui::end_style();
		if (settings::menu_bg)
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImGui::GetIO().DisplaySize, ImColor(0, 0, 0, a));
		if (settings::menu_bg_particles)
			FX(ImGui::GetBackgroundDrawList(), ImVec2(0, 0), ImGui::GetIO().DisplaySize, ImGui::GetIO().DisplaySize, ImGui::GetTime());

	}

}







LRESULT WINAPI menu::wndproc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (menu::is_open && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;


	return CallWindowProc(original_wndproc, hWnd, msg, wParam, lParam);
}