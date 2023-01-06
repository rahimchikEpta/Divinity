#include "motion_blur.h"
#include "../../xor.h"
#include <config.hpp>
inline float motion_blur_values[4] = { 0.f, 0.f, 0.f, 0.f };
inline float motion_blur_viewport_values[4] = { 0.f, 0.f, 1.f, 1.f };
i_texture* get_full_frame_frame_buffer_texture(const int texture_index)
{
	static auto fn = address(utilities::scan_pattern((uint32_t*)GetModuleHandle("client.dll"), xorstr_("55 8B EC 81 EC ? ? ? ? 56 8B F1 83 3C"))).as<i_texture* (__thiscall*)(int)>();
	return fn(texture_index);
}
#ifdef _WIN32
#undef xor
#define render_dis_bitch(material) \
{ \
    const auto drawFunction = interfaces::drawScreenEffectMaterial; \
    int w, h; \
    interfaces::engine->get_screen_size(w, h); \
    __asm { \
        __asm push h \
        __asm push w \
        __asm push 0 \
        __asm xor edx, edx \
        __asm mov ecx, material \
        __asm call drawFunction \
        __asm add esp, 12 \
    } \
}

#else
#define render_dis_bitch(material) \
{ \
    int w, h; \
    interfaces::engine->get_screen_size(w, h); \
    reinterpret_cast<void(*)(Material*, int, int, int, int)>(interfaces::drawScreenEffectMaterial)(material, 0, 0, w, h); \
}
#endif
void update_screen_effect_texture(const int texture_index, const int x, const int y, const int z, const int w, const bool dest_fullscreen, vec4* actual_rect)
{
	vec4 src_rect{};
	src_rect.x = x;
	src_rect.y = y;
	src_rect.z = z;
	src_rect.w = w;

	auto render_ctx = interfaces::material_system->get_render_context();
	auto texture = get_full_frame_frame_buffer_texture(texture_index);

	int src_width, src_height;
	render_ctx->get_render_target_dimensions(&src_width, &src_height);

	int dest_width = texture->get_width();
	int dest_height = texture->get_height();

	vec4 dest_rect = src_rect;

	if (!dest_fullscreen && (src_width > dest_width || src_height > dest_height))
	{
		int scale_x = dest_width / src_width;
		int scale_y = dest_height / src_height;
		dest_rect.x = src_rect.x * scale_x;
		dest_rect.y = src_rect.y * scale_y;
		dest_rect.z = src_rect.z * scale_x;
		dest_rect.w = src_rect.w * scale_y;

		dest_rect.x = std::clamp((int)dest_rect.x, 0, dest_width);
		dest_rect.y = std::clamp((int)dest_rect.y, 0, dest_height);
		dest_rect.z = std::clamp((int)dest_rect.z, 0, src_width - (int)dest_rect.x);
		dest_rect.w = std::clamp((int)dest_rect.w, 0, dest_height - (int)dest_rect.y);
	}

	render_ctx->copy_render_target_to_texture_ex(texture, 0, &src_rect, dest_fullscreen ? NULL : &dest_rect);
	render_ctx->set_frame_buffer_copy_texture(texture, texture_index);


	if (actual_rect)
	{
		actual_rect->x = dest_rect.x;
		actual_rect->y = dest_rect.y;
		actual_rect->z = dest_rect.z;
		actual_rect->w = dest_rect.w;
	}
}
constexpr void find_materials(i_material_var* mat)
{
	if (!mat) return;

	bool material_valid = false;

	i_material_unknown* blur_param = mat->FindVar(xorstr_("$MotionBlurInternal"), &material_valid, false);
	if (!material_valid) return;

	i_material_unknown* viewport_blur_param = mat->FindVar(xorstr_("$MotionBlurViewportInternal"), &material_valid, false);
	if (!material_valid) return;

	blur_param->set_vec_value(motion_blur_values, 4);
	viewport_blur_param->set_vec_value(motion_blur_viewport_values, 4);
};

bool motion_blur::gather_information(c_view_setup& setup) {
	motion_blur::view_setup = setup;
	return true;
}

bool motion_blur::render()
{
	if (!settings::motion_blur::enabled)
		return false;

	auto vector_angle_matrix = [](const vec3_t& angles, matrix_t& matrix) {
		float sr{ }, sp{ }, sy{ }, cr{ }, cp{ }, cy{ };

		math::sin_cos(DEG2RAD(angles.y), sy, cy);
		math::sin_cos(DEG2RAD(angles.x), sp, cp);
		math::sin_cos(DEG2RAD(angles.z), sr, cr);

		matrix[0][0] = cp * cy;
		matrix[1][0] = cp * sy;
		matrix[2][0] = -sp;

		matrix[0][1] = sr * sp * cy + cr * -sy;
		matrix[1][1] = sr * sp * sy + cr * cy;
		matrix[2][1] = sr * cp;
		matrix[0][2] = (cr * sp * cy + -sr * -sy);
		matrix[1][2] = (cr * sp * sy + -sr * cy);
		matrix[2][2] = cr * cp;

		matrix[0][3] = 0.0f;
		matrix[1][3] = 0.0f;
		matrix[2][3] = 0.0f;
	};

	constexpr auto dot_product = [](const vec3_t& a, const vec3_t& b) { return (a.x * b.x + a.y * b.y + a.z * b.z); };

	float x = motion_blur::view_setup.x;
	float y = motion_blur::view_setup.y;
	float w = motion_blur::view_setup.width;
	float h = motion_blur::view_setup.height;

	float rotation_intensity = settings::motion_blur::rotation_intensity;
	float blur_strength = settings::motion_blur::blur_strength;
	float falling_min = settings::motion_blur::falling_min;
	float falling_max = settings::motion_blur::falling_max;

	static motion_blur_history_t history{ };

	matrix_t current_basis_vectors{ };
	vector_angle_matrix(motion_blur::view_setup.angles, current_basis_vectors);

	vec3_t current_side_vec(current_basis_vectors[0][1], current_basis_vectors[1][1], current_basis_vectors[2][1]);
	vec3_t current_forward_vec(current_basis_vectors[0][0], current_basis_vectors[1][0], current_basis_vectors[2][0]);
	vec3_t normalized_angles = motion_blur::view_setup.angles.normalized();
	vec3_t position_change = history.previous_position - motion_blur::view_setup.origin;

	if (true) {
		//====================//
		// Normal update path //
		//====================//
		// Compute horizontal and vertical fov

		float horizontal_fov = motion_blur::view_setup.fov;
		float vertical_fov = (motion_blur::view_setup.aspect_ratio <= 0.0f) ? (motion_blur::view_setup.fov) : (motion_blur::view_setup.fov / motion_blur::view_setup.aspect_ratio);

		//=====================//
		// Forward motion blur //
		//=====================//
		float view_dot_motion = dot_product(current_forward_vec, position_change);
		if (settings::motion_blur::foward_blur)
			motion_blur_values[2] = view_dot_motion * fabs(current_forward_vec[2]);

		//====================================//
		// Yaw (Compensate for circle strafe) //
		//====================================//

		float yaw_diff_original = history.previous_yaw - normalized_angles.y;
		if (((history.previous_yaw - normalized_angles.y > 180.0f) || (history.previous_yaw - normalized_angles.y < -180.0f)) &&
			((history.previous_yaw + normalized_angles.y > -180.0f) && (history.previous_yaw + normalized_angles.y < 180.0f)))
			yaw_diff_original = history.previous_yaw + normalized_angles.y;

		float side_dot_motion = dot_product(current_side_vec, position_change);
		float yaw_diff_adjusted = yaw_diff_original + (side_dot_motion / 3.0f);

		// Make sure the adjustment only lessens the effect, not magnify it or reverse it
		if (yaw_diff_original < 0.0f)
			yaw_diff_adjusted = std::clamp(yaw_diff_adjusted, yaw_diff_original, 0.0f);
		else
			yaw_diff_adjusted = std::clamp(yaw_diff_adjusted, 0.0f, yaw_diff_original);

		// Use pitch to dampen yaw
		float undampened_yaw = yaw_diff_adjusted / horizontal_fov;
		motion_blur_values[0] = undampened_yaw * (1.0f - (fabs(normalized_angles.x) / 90.0f));


		//=======================================//
		// Pitch (Compensate for forward motion) //
		//=======================================//
		float pitch_compensate_mask = 1.0f - ((1.0f - fabs(current_forward_vec[2])) * (1.0f - fabs(current_forward_vec[2])));
		float pitch_diff_original = history.previous_pitch - normalized_angles.x;
		float pitch_diff_adjusted = pitch_diff_original;

		if (normalized_angles.x > 0.0f)
			pitch_diff_adjusted = pitch_diff_original - ((view_dot_motion / 2.0f) * pitch_compensate_mask);
		else
			pitch_diff_adjusted = pitch_diff_original + ((view_dot_motion / 2.0f) * pitch_compensate_mask);

		// Make sure the adjustment only lessens the effect, not magnify it or reverse it
		if (pitch_diff_original < 0.0f)
			pitch_diff_adjusted = std::clamp(pitch_diff_adjusted, pitch_diff_original, 0.0f);
		else
			pitch_diff_adjusted = std::clamp(pitch_diff_adjusted, 0.0f, pitch_diff_original);

		motion_blur_values[1] = pitch_diff_adjusted / vertical_fov;

		//========================================================//
		// Roll (Enabled when we're looking down and yaw changes) //
		//========================================================//
		motion_blur_values[3] = undampened_yaw;
		motion_blur_values[3] *= (fabs(normalized_angles.x) / 90.0f) * (fabs(normalized_angles.x) / 90.0f) * (fabs(normalized_angles.x) / 90.0f);

		//==============================================================//
		// Time-adjust falling effect until we can do something smarter //
		//==============================================================//
		if ((interfaces::globals->realtime - history.last_time_update) > 0.0f)
			motion_blur_values[2] /= (interfaces::globals->realtime - history.last_time_update) * 30.0f;
		else
			motion_blur_values[2] = 0.0f;

		motion_blur_values[2] = std::clamp((fabs(motion_blur_values[2]) - falling_min) / (falling_max - falling_min), 0.0f, 1.0f) *
			(motion_blur_values[2] >= 0.0f ? 1.0f : -1.0f);
		motion_blur_values[2] /= 30.0f;

		//=================//
		// Apply intensity //
		//=================//
		motion_blur_values[0] *= rotation_intensity * blur_strength;
		motion_blur_values[1] *= rotation_intensity * blur_strength;
		motion_blur_values[2] *= rotation_intensity * blur_strength;
		motion_blur_values[3] *= rotation_intensity * blur_strength;

		//====================================//
		// Store current frame for next frame //
		//====================================//
		history.previous_position = motion_blur::view_setup.origin;
		history.previous_frame_basis_vectors = current_basis_vectors;
		history.previous_pitch = normalized_angles.x;
		history.previous_yaw = normalized_angles.y;
		history.last_time_update = interfaces::globals->realtime;
	}

	// viewport
	if (true) {
		i_texture* full_frame = interfaces::material_system->find_texture(xorstr_("_rt_FullFrameFB"), xorstr_("RenderTargets"));
		float src_width = static_cast<float>(full_frame->get_width());
		float src_height = static_cast<float>(full_frame->get_height());
		int offset{ };

		offset = (x > 0) ? 1 : 0;
		motion_blur_viewport_values[0] = float(x + offset) / (src_width - 1);

		offset = (x < (src_width - 1)) ? -1 : 0;
		motion_blur_viewport_values[3] = float(x + w + offset) / (src_width - 1);

		offset = (y > 0) ? 1 : 0;
		motion_blur_viewport_values[1] = float(y + offset) / (src_height - 1);

		offset = (y < (src_height - 1)) ? -1 : 0;
		motion_blur_viewport_values[2] = float(y + h + offset) / (src_height - 1);

		for (int i = 0; i < 4; i++) {
			if (motion_blur_viewport_values[i] <= 0.0f)
				motion_blur_viewport_values[i] = -1.0f;
			else if (motion_blur_viewport_values[i] >= 1.0f)
				motion_blur_viewport_values[i] = 2.0f;
		}
	}

	// perform motion blur
	bool performed_motion_blur = false;
	if (true) {
		i_mat_context* render_ctx = interfaces::material_system->get_render_context();
		i_texture* full_frame = interfaces::material_system->find_texture(xorstr_("_rt_FullFrameFB"), xorstr_("RenderTargets"));

		int src_width = full_frame->get_width();
		int src_height = full_frame->get_height();
		int vport_width{ }, vport_height{ }, dummy{ };
		render_ctx->get_viewport(&dummy, &dummy, &vport_width, &vport_height);
		update_screen_effect_texture(0, int(x), int(y), int(w), int(h), false, NULL);

		i_material_var* mat_motion_blur = interfaces::material_system->FindMaterial(xorstr_("dev/motion_blur"), xorstr_("Other textures"));

		if (mat_motion_blur && src_width > 0 && src_height > 0) {
			find_materials(mat_motion_blur);


			render_dis_bitch(mat_motion_blur)

				performed_motion_blur = true;
		}
	}

	return performed_motion_blur;
}
