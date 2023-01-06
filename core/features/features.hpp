#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/variables.hpp"
#include <vector>
#define TICK_INTERVAL            ( interfaces::globals->interval_per_tick )
#define TICKS_TO_TIME( t )    ( TICK_INTERVAL * (float)( t ) )
#define ROUND_TO_TICKS( t )    ( TICK_INTERVAL * time_to_ticks( t ) )
#define TICK_NEVER_THINK         ( -1 )

struct restore_values {
	vec3_t viewangles;
	vec3_t view_delta;
	float stamina;
	float forwardmove;
	float sidemove;
	int buttons;
	player_t* local_player;
};
namespace misc {
	namespace movement {
		inline float eb_effect_time;
		inline float gravity_addictive = 800.f;
		void bunny_hop(c_usercmd* cmd);
		void edgejump ( c_usercmd* user_cmd, int pre_flags, int post_flags ) noexcept;
		void long_jump ( c_usercmd* user_cmd );
		void mini_jump ( c_usercmd* user_cmd, int pre_flags );
		void detect_edgebug(c_usercmd* user_cmd, vec3_t old_velocity);
		void quick_stop(c_usercmd* m_pcmd);
		void pre_pixelsurf(c_usercmd* cmd);
		void pixel_surf(c_usercmd* cmd);
		void auto_duck(c_usercmd* cmd);
		void jumpheight_exploit(c_usercmd* cmd);
		inline bool in_edgebug = false;
		inline bool in_crouch_edgebug = false;
		void visualize_eb();
		void is_jumpbugging();
		void jumpbug(c_usercmd* cmd);
		void edgebug_run(c_usercmd* cmd);
		void auto_strafe(c_usercmd* user_cmd) noexcept;
		struct velocity_data_t {
			int speed;
			bool on_ground;
			bool edge_bugged;
			bool jump_bugged;
			bool pixelsurfed;
			bool in_pixelsurf;
			bool was_in_prediction;
			int jump_duration;
		};
		struct stamina_data_t {
			float stamina;
			bool on_ground;
		};
		inline std::vector<velocity_data_t> velo_data;
		inline std::vector<stamina_data_t> stamina_data;

		void velo_graph_gather_data ();
		void velo_graph_draw ();
		void stamina_graph_gather_data ();
		void stamina_graph_draw ();
		void no_duck_cooldown ( c_usercmd* cmd );
		void beam_trail ();
		void auto_ladder_bug(c_usercmd* cmd);
		void auto_pixel_surf(c_usercmd* cmd);
		void display_velo();
		inline vec3_t origin;
		inline vec3_t viewangles;
		inline bool will_ps;
		inline bool hit_ps;
		inline int ps_lock;
		inline vec3_t ps_angles;
		inline vec3_t vel_backup;
		inline bool should_duck_next;
		inline bool strafe_assisted;
		inline bool did_jumpbug;
		inline bool should_eb;
		inline bool should_lb;
		inline int amount_of_edgebugs;
		inline int amount_of_pixelsurfs;
		inline int amount_of_jumpshots;
		inline bool successful_jb;
		inline bool successful_ej;
		inline bool successful_lb;
		void strafe_optimiser ( c_usercmd* cmd );
	};
	inline int chat_output_colour;
	inline std::string default_output_colour = "\0x01";
	inline static float last_edgebug;
	inline float effect_alpha;
	inline float hiteffect_alpha;
	namespace main {
		void watermark();
		void clantag();
		void viewmodel_offset();
		void aspect_ratio ();
		void ragdoll_gravity ();
		void anti_afk (c_usercmd* cmd);
		void reveal_rank ( c_usercmd* cmd );
		void chicken_killsay ( i_game_event* event );
		void round_stats ( i_game_event* event );
		void hitlogs ( i_game_event* event );
		void crash_ploobware_users ();
		void door_spammer(c_usercmd* cmd);
		void auto_jumpscout(c_usercmd* cmd);
		void bomb_info();
		void modulate_hud();
		void toggle_hud_modulation(bool enabled);
		namespace vote_revealer {
			void on_event ( i_game_event* event );
		}
		inline int last_z;
	};
	inline std::string get_chat_output_colour();
	inline bool can_do_glow;
	void hitmarker_event ( i_game_event* event );
}

struct bbox_t {
	long x, y, w, h;
};
#include "utils/texture.h"
struct PlayerAvatar {
	mutable Texture texture;
	std::unique_ptr<std::uint8_t [ ]> rgba;
};
namespace visuals {
	inline static std::unordered_map<int, PlayerAvatar> playerAvatars;
	inline IDirect3DTexture9** spec_image;
	bbox_t get_bbox(player_t* player);
	void run();
	inline float dormant_alpha[64];
	void handle_dormant_animation(player_t* entity);

	inline bbox_t box_ctx;

	void render_players(player_t* entity);

	void render_names(player_t* entity);

	void render_health(player_t* entity);

	inline bool should_render_armor;
	void render_armor(player_t* entity);

	void render_flags(player_t* entity);

	void render_weapon_name(player_t* entity);

	void render_skeleton(player_t* entity);

	ImTextureID getAvatarTexture ( int i ) noexcept;


	void spectator_list();

	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};
	namespace nade_pred {
		

		class c_nadepoint {
		public:
			c_nadepoint() {
				m_valid = false;
			}

			c_nadepoint(vec3_t start, vec3_t end, bool plane, bool valid, vec3_t normal, bool detonate) {
				m_start = start;
				m_end = end;
				m_plane = plane;
				m_valid = valid;
				m_normal = normal;
				m_detonate = detonate;
			}

			vec3_t m_start, m_end, m_normal;
			bool m_valid, m_plane, m_detonate;
		};

		inline std::array< c_nadepoint, 500 >	_points{ };
		inline bool		 _predicted = false;
		void predict(c_usercmd* ucmd);
		bool detonated(weapon_t* weapon, float time, trace_t& trace);
		void trace(c_usercmd* ucmd);
		void draw();

	};
	namespace chams {
		enum class shader_type_t {
			VertexLitGeneric = 0,
			UnlitGeneric,
			Modulate
		};
		i_material* create_material ( shader_type_t shade, bool ignorez, bool wireframe );
		void run ( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world );
		void run_local ( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world );
		void run_weapons ( i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world );
		void run_gloves(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
	};
	void disable_panorama_blur ();
	void do_precipitation ( int stage );
	struct impact_data {
		vec3_t impact_position;
		float time;
	};
	inline vec3_t last_shot_pos;
	inline std::vector <impact_data> impacts;
	void beam_events ( i_game_event* event );
	void draw_beams ();
	void fov_circle (int fov);
	void skybox_changer ();
	void fog_modulation ();
	void engine_based_force_crosshair();
	void glow ();
	void run_weapon_esp ( weapon_t* weapon );
	void players_behind(player_t* player);
	void key_strokes();
	void run_weapon_esp();
}

struct tick_info {
	tick_info () {
		head_pos = vec3_t{};
		simulation_time = -1.f;
		bone_matrix_built = false;
	}
	tick_info ( player_t* player ) {
		head_pos = player->get_hitbox_position ( hitbox_head );
		simulation_time = player->simulation_time ();
		bone_matrix_built = false;
		if ( player->setup_bones ( bone_matrix, 128, BONE_USED_BY_ANYTHING, interfaces::globals->cur_time ) )
			bone_matrix_built = true;
	}
	vec3_t head_pos;
	float simulation_time;
	bool bone_matrix_built;
	matrix_t bone_matrix[ 128 ];
};

namespace legit_bot {
	void event ( i_game_event* event );
	void aimbot ( c_usercmd* cmd );
	bool hit_chance ( vec3_t ang, player_t* player, float hitchance );
	void trigger ( c_usercmd* cmd );
	void autostop(c_usercmd* cmd);

	inline int kill_delay = 0;
	inline int trigger_delay = 0;
}

namespace aim_assist {
	void run(c_usercmd* cmd);
	inline bool is_targeting;
}

struct sequence_object_t {
	sequence_object_t ( int iInReliableState, int iOutReliableState, int iSequenceNr, float flCurrentTime )
		: iInReliableState ( iInReliableState ), iOutReliableState ( iOutReliableState ), iSequenceNr ( iSequenceNr ), flCurrentTime ( flCurrentTime ) {
	}

	int iInReliableState;
	int iOutReliableState;
	int iSequenceNr;
	float flCurrentTime;
};

namespace lag_compensation {
	void update ( int stage );
	void run ( c_usercmd* cmd );
	void update_incoming_sequences ( i_net_channel_t* net_channel );
	void clear_incoming_sequences ();
	void add_latency_to_net_channel ( i_net_channel_t* net_channel, float latency );

	inline std::deque<sequence_object_t> sequences_vector = { };
	inline int last_incoming_sequence = 0;

	struct stored_records {
		vec3_t hitbox;
		vec3_t origin;
		float simulation_time;
		matrix_t matrix[ 256 ];
	};

	struct convars {
		convar* get_updaterate;
		convar* get_maxupdaterate;
		convar* get_interp;
		convar* get_interpratio;
		convar* get_mininterpratio;
		convar* get_maxinterpratio;
		convar* get_maxunlag;
	};

	inline convars cvars;
	inline std::deque<stored_records> records[ 65 ];

	constexpr auto lerp_time () {
		auto ratio = std::clamp ( cvars.get_interpratio->get_float (), cvars.get_mininterpratio->get_float (), cvars.get_maxinterpratio->get_float () );

		return max ( cvars.get_interp->get_float (), ( ratio / ( ( cvars.get_maxupdaterate ) ? cvars.get_maxupdaterate->get_float () : cvars.get_updaterate->get_float () ) ) );
	}

	inline auto valid ( float simtime ) {
		auto network = interfaces::engine->get_net_channel_info ();
		if ( !network )
			return false;

		auto delta = std::clamp ( network->get_latency ( 0 ) + network->get_latency ( 1 ) + lerp_time (), 0.f, cvars.get_maxunlag->get_float () ) - ( csgo::server_time () - simtime );
		return std::fabsf ( delta ) <= 0.2f;
	}

	constexpr auto time_to_ticks ( float time ) {
		return static_cast<int>( 0.5f + time / interfaces::globals->interval_per_tick );
	}

	static void init () {
		records->clear ();

		cvars.get_updaterate = interfaces::console->get_convar ( "cl_updaterate" );
		cvars.get_maxupdaterate = interfaces::console->get_convar ( "sv_maxupdaterate" );
		cvars.get_interp = interfaces::console->get_convar ( "cl_interp" );
		cvars.get_interpratio = interfaces::console->get_convar ( "cl_interp_ratio" );
		cvars.get_mininterpratio = interfaces::console->get_convar ( "sv_client_min_interp_ratio" );
		cvars.get_maxinterpratio = interfaces::console->get_convar ( "sv_client_max_interp_ratio" );
		cvars.get_maxunlag = interfaces::console->get_convar ( "sv_maxunlag" );
	}
}
// new edgebug (never got around to doing...)
namespace edgebug {
	inline bool in_edgebug;
	inline bool should_duck;
	inline bool force_cancel;
	inline int old_flags;
	inline vec3_t old_velocity;

	void on_edgebug(c_usercmd* cmd);
	void on_mouse_override(float* x);
	bool can_edgebug(c_usercmd* cmd, player_t* local);
	void pre_cmd ( );
	void post_cmd ( c_usercmd* cmd );
}

namespace changers {
	namespace knife_proxy {
		void initialize ();
	};

	namespace parser {
		struct paint_kit {
			int id;
			std::string name;
			bool operator < ( const paint_kit& other ) {
				return name < other.name;
			}
		};
		void initialize ();

		inline std::vector<paint_kit> parser_skins;
		inline std::vector<paint_kit> parser_gloves;
	};

	namespace skins {
		void run ();
	};
};