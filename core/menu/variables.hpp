#pragma once
#include "../../source-sdk/classes/studio.hpp"
namespace variables {
	inline bool test_bool = false;
	inline float test_float = 0.f;
	inline float test_float2 = 0.f;
	inline float test_float3 = 0.f;
	inline bool edgebugin = false;
	inline int eb_on_tick;
	inline bool go_un_inject_faggot = false;
	inline vec3_t player_origin_on_eb;
	inline int hit_cooldown;
	inline int last_player_hit_index;
	inline vec3_t last_player_hit_origin;
	inline std::vector<vec3_t> bone_pos_on_eb;
	inline std::vector<studio_bone_t*> studio_bone_on_eb;
	inline vec3_t player_origin_on_predicted_eb;
	inline float glow_bloom;
	inline int glow_style;
	inline vec3_t v;
	inline bool render_init;


	inline float jb_ind_alpha;
	inline float eb_ind_alpha;
	inline float ej_ind_alpha;
	inline float lb_ind_alpha;

	inline bool console_is_drawing;

	inline int saved_viewmodel_x;
	inline int saved_viewmodel_y;
	inline int saved_viewmodel_z;

	inline float vis_eb_alpha;
	inline std::vector<vec3_t> eb_origins;
	inline matrix_t bone_matrix_on_eb;

	inline int movement_visuals_alpha;
	namespace menu {
		inline bool opened = false;
		inline int x = 140, y = 140;
		inline int w = 400, h = 300;
		}
	}