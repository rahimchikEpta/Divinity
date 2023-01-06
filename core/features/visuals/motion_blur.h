#pragma once
#include "../features.hpp"
#include "../../../dependencies/interfaces/c_view_setup.h"
namespace motion_blur {
	//struct impl {
	struct motion_blur_history_t {
		motion_blur_history_t()
		{
			last_time_update = 0.0f;
			previous_pitch = 0.0f;
			previous_yaw = 0.0f;
			previous_position = { 0.0f, 0.0f, 0.0f };
			previous_frame_basis_vectors = { };
			no_rotational_motion_blur_until = 0.0f;
		}

		float last_time_update;
		float previous_pitch;
		float previous_yaw;
		vec3_t previous_position;
		matrix_t previous_frame_basis_vectors;
		float no_rotational_motion_blur_until;
	};
	inline c_view_setup view_setup{};
	bool gather_information(c_view_setup& setup);
	bool render();
	//};
} // namespace rebuilt_motion_blur