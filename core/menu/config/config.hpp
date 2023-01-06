#include <string_view>
#include <vector>
#include "nlohmann/json.hpp"

struct legitbot_settings {
	bool enabled = false;
	int aim_type = 1;
	float fov = 1.f;
	float rcs = 0.f;
	float smooth = 1.f;
	bool autostop = false;
	bool hitboxes[4] = { true, false, false, false };

	bool auto_shoot;
	bool aim_lock;
};

struct triggerbot_settings {
	float hitchance = 1.f;
	int delay = 1;
};

enum AIMBOT_WEAPON_CONFIG {
	WEAPONS_GENERAL,
	WEAPONS_RIFLE,
	WEAPONS_SNIPER,
	WEAPONS_PISTOL,
	WEAPONS_SMG,
	WEAPONS_HEAVY
};

namespace settings {
	void create_directory();
	void update_configs();
	void create_file(const std::string_view name);
	void delete_file(const std::size_t index);
	void save(const std::size_t index);
	void load(const std::size_t index);

	inline static const std::string path_lua = "divinity/luas/";

	namespace aimbot {
		inline bool silent;
		inline int aim_key;
		inline bool scoping_required;
		inline int delay_after_kill;
		inline bool visible_only;
		inline bool auto_pistol;
		inline bool target_in_air;
		inline bool target_team;
		inline bool target_visible;
		inline int type;
		inline bool target_smoke;
		inline float auto_wall_mim_damage = 10.f;
		inline bool ignore_flash;

		inline bool c_enabled[4];
		inline int c_aimtype[4];
		inline float c_fov[4];
		inline float c_rcs[4];
		inline float c_smooth[4];
		inline bool c_autostop[4];
		inline bool general_hitboxes[4] = { false, false, false, false };
		inline bool rifle_hitboxes[4] = { false, false, false, false };
		inline bool sniper_hitboxes[4] = { false, false, false, false };
		inline bool pistol_hitboxes[4] = { false, false, false, false };
		inline bool smg_hitboxes[4] = { false, false, false, false };
		inline bool heavy_hitboxes[4] = { false, false, false, false };

		

		inline bool triggerbot_enable;
		inline int trigger_key;
		inline int trigger_type;
		inline float trigger_ht;
		inline int trigger_delay;
		inline bool backtrack_enable;
		inline bool backtrack_ping_spike;
		inline int backtrack_ping_spike_value;
	}

	namespace visuals {
		//visuals ayaayyay
		inline bool enable_esp;
		inline bool engine_radar;
		inline bool oof_arrows;
		inline float oof_arrow_range;
		inline float oof_arrow_size;
		inline float oof_arrow_col1[4] = { 25 / 255.f, 25 / 255.f, 25 / 255.f, 1.f };
		inline float oof_arrow_col2[4] = { 125 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline float oof_arrow_col3[4] = { 25 / 255.f, 25 / 255.f, 25 / 255.f, 1.f };
		inline bool dormant_esp;
		inline bool visuals_box;
		inline float box_vertical;
		inline bool box_lines[2] = {false, false};
		inline float box_horizontal;
		inline int box_type;
		inline bool visuals_name;
		inline bool flash_kill;
		inline char visuals_bot_extention_name[128];
		inline bool visuals_health;
		inline bool gradient_healthbar;
		inline float gradient_healthbar_col1[4] = { 135 / 255.f, 155 / 255.f, 255 / 255.f, 1.f };
		inline float gradient_healthbar_col2[4] = { 255 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline bool visuals_armor;
		inline bool visuals_flags;
		inline bool visuals_flags_uppercase;
		inline bool visuals_weapon;
		inline bool visuals_skeleton;
		inline bool spectator_list;
		inline int spectator_list_pos_x;
		inline int spectator_list_pos_y;

		inline bool hit_screeneffect;
		inline int hit_screeneffect_type;
		inline float hit_screeneffect_time;

		inline bool bullet_tracers;
		inline float bullet_tracer_col[4] = { 30 / 255.f, 255 / 255.f, 109 / 255.f, 1.f };

		inline bool fov_circle;
		inline float fov_circle_clr[4] = { 1.f, 1.f, 1.f, 1.f };
		//chamie
		inline bool enable_chams;
		inline bool enable_chams_vis;
		inline bool enable_chams_invis;
		inline bool chams_overlay;
		inline int player_material;
		inline int player_material_invis;
		inline int player_overlay_material;

		inline bool backtrack_chams;
		inline int backtrack_chams_type;
		inline int backtrack_render_type;
		inline float backtrack_chams_color2[4] = { 203 / 255.f, 135 / 255.f, 255 / 255.f, 1.f };
		inline float backtrack_chams_color[4] = { 135 / 255.f, 155 / 255.f, 255 / 255.f, 1.f };

		inline bool arm_chams;
		inline int arm_chams_material;
		inline float arm_chams_color[ 4 ] = { 135 / 255.f, 155 / 255.f, 255 / 255.f, 1.f };
		inline bool arm_overlay_chams;
		inline int arm_overlay_material;
		inline float arm_overlay_color[ 4 ] = { 135 / 255.f, 155 / 255.f, 255 / 255.f, 1.f };
		inline bool weapon_chams;
		inline bool weapon_overlay_chams;
		inline int weapon_chams_material;
		inline int weapon_overlay_material;
		inline float weapon_chams_color[ 4 ] = { 203 / 255.f, 135 / 255.f, 255 / 255.f, 1.f };
		inline float weapon_chams_overlay_color[ 4 ] = { 203 / 255.f, 135 / 255.f, 255 / 255.f, 1.f };

		inline float chams_vis[4] = { 135 / 255.f, 155 / 255.f, 255 / 255.f, 1.f };
		inline float chams_invis[4] = { 255 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline float chams_overlay_clr[4] = { 255 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline bool ragdoll_chams;
		inline float ragdoll_chams_clr[4] = { 135 / 255.f, 155 / 255.f, 1.f, 1.f };
		inline int ragdoll_chams_material;

		inline bool glove_chams;
		inline int glove_chams_material;
		inline int glove_overlay_material;
		inline float glove_chams_clr[4] = { 135 / 255.f, 155 / 255.f, 1.f, 1.f };
		inline bool glove_overlay_chams;
		inline float glove_overlay_chams_clr[4] = { 255 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };

		inline bool visuals_disableflash;
		inline bool visuals_disable_panorama_blur;


		inline bool glow_enabled;
		inline bool glow_players;
		inline int glow_type_player;
		inline int glow_type_bomb;
		inline int glow_type_chicken;
		inline int glow_type_weapons;
		inline int glow_type_fish;
		inline float glow_player_col[4] = { 1.f, 0.5f, 1.f, 0.5f };
		inline float glow_player_invis_col[4] = { 1.f, 0.5f, 1.f, 0.5f };
		inline bool glow_chickens;
		inline float glow_chicken_col[4] = { 1.f, 0.5f, 1.f, 0.5f };
		inline bool glow_weapons;
		inline float glow_weapons_col[4] = { 1.f, 0.5f, 1.f, 0.5f };
		inline bool glow_bombs;
		inline float glow_bomb_col[4] = { 1.f, 0.5f, 1.f, 0.5f };
		inline bool glow_fish;
		inline float glow_fish_col[4] = { 1.f, 0.5f, 1.f, 0.5f };
		inline bool world_modulation;
		inline float world_modulation_clr[4] = { 20 / 255.f, 20 / 255.f, 20 / 255.f, 1.f };
		inline bool prop_modulation;
		inline float prop_modulation_clr[4] = { 255 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline bool hitsound = false;
		inline bool killsound = false;
		inline bool modulate_molotov = false;
		inline float modulate_molotov_clr[4] = { 125 / 255.f, 255 / 255.f, 125 / 255.f, 1.f };
		inline bool modulate_smoke = false;
		inline float modulate_smoke_clr[4] = { 255 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline bool modulate_weapon_effects = false;
		inline float modulate_weapon_effects_clr[4] = { 255 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline bool modulate_blood = false;
		inline float modulate_blood_clr[4] = { 125 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline bool rain = false;
		inline int skybox_changer_index = 12;

		inline bool fog;
		inline int fog_start;
		inline int fog_end;
		inline int fog_density;
		inline float fog_clr[4] = { 125 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };

		inline bool force_crosshair;
		inline bool hit_effect;

		inline bool wireframe_smoke;
		inline bool wireframe_fire;

		inline bool grenade_prediction;
		inline float grenade_prediction_clr[ 4 ] = { 125 / 255.f, 125 / 255.f, 255 / 255.f, 1.f };
		inline float grenade_prediction_clr2 [4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };

		inline bool viewmodel;
		inline float viewmodel_offset_x;
		inline float viewmodel_offset_y;
		inline float viewmodel_offset_z;


		inline bool weapon_esp;
		inline float weapon_esp_col[4] = { 59 / 255.f, 111 / 255.f, 1.f, 1.f };
		inline float weapon_esp_distance;
		inline bool weapon_name;
		inline bool weapon_ammo;
		inline bool weapon_box;
		inline float weapon_box_vertical;

		inline bool dropped_c4;
		inline float dropped_c4_col[4] = { 235 / 255.f, 100 / 255.f, 52 / 255.f, 1.f };
	}

	namespace visulisation {
		inline bool visualize_eb;
		inline bool text_on_eb;
		inline float text_on_eb_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline int effect_on_eb;
		inline float effect_on_eb_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline bool circle_on_eb;
		inline float circle_on_eb_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline bool predicted_circle_on_eb;
		inline float predicted_circle_on_eb_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline bool cube_on_eb;
		inline float cube_on_eb_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline bool dlight_on_eb;
		inline float dlight_on_eb_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float dlight_on_eb_brightness = 8;
		inline int dlight_on_eb_radius = 120;
		inline float vis_eb_fadeout_time = 4;
		inline bool predicted_eb_circle2d;
		inline float predicted_eb_circle2d_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline bool sparks_while_eb;
		inline bool lightning_while_eb;
		inline float lightning_while_eb_col[ 4 ] = { 1.f, 1.f, 1.f, 1.f };
	}

	namespace indicators {
		inline int position = 350;
		inline bool velo;
		inline int velo_alpha_type;
		inline bool velo_takeoff;
		inline int velo_color_type;
		inline int velo_graph_style;
		inline int stamina_graph_style;
		inline bool graph_velocity;
		inline int graph_position = 385;
		inline int velo_graph_size = 65;
		inline float velo_graph_scale = 7.5f;
		inline int graph_thickness = 2;
		inline bool graph_land_velo;
		inline bool graph_jump_velo;
		inline bool graph_bold_font;
		inline bool graph_eb;
		inline bool graph_ps;
		inline bool graph_jb;
		inline float graph_velo_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline bool stamina;
		inline bool graph_stamina;
		inline float graph_stamina_col[4] = { 181 / 255.f, 75 / 255.f, 75 / 255.f, 1.f };
		inline bool ej;
		inline bool ps;
		inline bool lb;
		inline bool jb;
		inline bool eb;
		inline float velo_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float jb_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float jb_rage_col[4] = { 1.f, 0.f, 0.f, 1.f };
		inline float ej_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float ej_rage_col[4] = { 1.f, 0.f, 0.f, 1.f };
		inline float lb_default_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float lb_rage_col[4] = { 1.f, 0.f, 0.f, 1.f };
		inline float ps_default_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float ps_rage_col[4] = { 1.f, 0.f, 0.f, 1.f };
		inline float stamina_col[4] = { 181 / 255.f, 75 / 255.f, 75 / 255.f, 1.f };
		inline float eb_default_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float eb_rage_col[4] = { 1.f, 0.f, 0.f, 1.f };

		inline bool round_stamina;
		inline int stamina_alpha_type;
		inline bool stamina_takeoff;
		inline int stamina_color_type;


		inline bool key_strokes;
		inline bool key_strokes_fadeout;
		inline float key_strokes_col[4] = { 1.f, 1.f, 1.f, 1.f };
		inline bool key_strokes_stances;
		inline int key_strokes_position = 200;
	}

	namespace movement {
		inline bool bhop;
		inline int bhop_chance = 100;
		inline bool no_duck_cooldown;
		inline bool jumpbug;
		inline int jumpbug_key;
		inline bool edgejump;
		inline int edgejump_key;
		inline bool minijump;
		inline int minijump_key;
		inline bool edgebug;
		inline int edgebug_key;
		inline bool longjump;
		inline int longjump_key;
		inline bool quickstop;

		inline int edgebug_scan_time = 64;
		inline int edgebug_scan_strength = 4;
		inline int edgebug_scan_type;
		inline bool strafe_assistance;
		inline int edgebug_scan_crouch_after_x_pred = 1;
		inline bool lock_on_eb;
		inline bool simulate_as;
		inline float lock_pull_amount = 100.f;
		inline bool lock_pitch;
		inline bool jumpshot_info;
		inline int jumpshot_info_print_type;
		inline bool jumpshot_slurs;


		inline bool strafe_optimiser;
		inline bool strafe_optimiser_on_key;
		inline float required_speed;
		inline float desired_gain;
		inline int strafe_optimiser_key;

		inline bool ladder_bug;
		inline int ladder_bug_key;

		inline bool pixelsurf;
		inline int pixelsurf_key;
		inline bool auto_duck;
		inline int auto_duck_key;
	}

	namespace edgebug_effects {
		inline bool print_eb_info;
		inline int print_eb_info_print_type;
		inline bool healthboost_on_eb;
		inline bool sound_on_eb;
		inline bool eb_effect;
		inline int effect_on_eb;
		inline float effect_time;
	}

	namespace skins {
		inline bool agent_changer;
		inline bool knife_changer;
		inline int knife_model;
		inline int knife_paintkit_index;
		inline int kinfe_paintkit_seed;
		inline int knife_vec_index;
		inline float knife_wear;
		inline bool glove_changer;
		inline int glove_paintkit_index;
		inline int glove_vec_index;
		inline float glove_wear;
		inline int glove_model;
		inline bool modulate_knife;
		inline float knife_color1[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float knife_color2[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float knife_color3[4] = { 1.f, 1.f, 1.f, 1.f };
		inline float knife_color4[4] = { 1.f, 1.f, 1.f, 1.f };
	}

	namespace misc {
		inline bool stream_proof;
		inline bool anti_afk;
		inline bool reveal_ranks;
		inline bool settings_unsafe_features = true;
		inline bool custom_netgraph;
		inline bool modulate_netgraph;
		inline bool aspect_ratio_changer;
		inline float aspect_ratio_changer_value;
		namespace trails {
			inline bool enable;
			inline int color_type = 0;
			inline float rainbow_speed = 1.f;
			inline float life = 2.5f;
			inline float width = 1.f;
			inline float life_speed = 0.5f;
			inline float col[4] = { 1.f, 0.5f, 1.f, 1.f };
			inline float fluc_col1[4] = { 1.f, 0.5f, 1.f, 1.f };
			inline float fluc_col2[4] = { 1.f, 0.5f, 1.f, 1.f };
		}

		inline bool ragdoll_gravity;

		inline bool custom_region;
		inline int custom_region_selection;

		inline bool watermark;
		inline bool spotify_on_watermark;

		inline bool roundend_message;
		inline bool chickendeath_message;
		inline bool round_stats;
		inline int round_stats_print_type;
		inline bool vote_revealer;
		inline int vote_revealer_print_type;


		inline bool auto_accept;
		inline bool clantag;

		inline bool hitlogs;
		inline int hitlogs_print_type;

		inline bool door_spammer;
		inline int door_spammer_delay;
		inline int door_spammer_key;

		inline bool jumpscout;
		inline int jumpscout_key;

		inline bool motion_blur;
		inline float motion_blur_modifier;

		inline bool modulate_hud;
		inline float modulate_hud_col[4] = { 1.f, 1.f, 1.f, 1.f };
	}

	namespace motion_blur {
		inline bool enabled;
		inline bool foward_blur;
		inline float rotation_intensity = 1.f;
		inline float blur_strength = 1.f;
		inline float roll_intensity = 1.f;
		inline float falling_intensity = 1.f;
		inline float falling_min = 10.f;
		inline float falling_max = 20.f;
	}
	inline bool rainbow_menu_col;
	inline bool reduce_server_lag_upon_crash;
	inline int reduce_server_lag_upon_crash_key;
	inline bool menu_bg;
	inline bool menu_bg_particles;
	inline bool discord_rpc;
	inline std::vector<std::string> configs;
}