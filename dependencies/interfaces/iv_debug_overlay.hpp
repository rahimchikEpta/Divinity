#pragma once
#include "../../source-sdk/math/vector3d.hpp"

class iv_debug_overlay {
public:
	bool world_to_screen(const vec3_t& in, vec3_t& out) {
		using original_fn = int(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		int return_value = (*(original_fn * *)this)[13](this, in, out);
		return static_cast<bool>(return_value != 1);
	}
	virtual void AddCapsuleOverlay(const vec3_t& mins, const vec3_t& max, float& radius, int r, int g, int b, int a, float duration, char unknown, char ignorez) = 0;
	bool screen_position(const vec3_t& in, vec3_t& out) {
		using original_fn = bool(__thiscall*)(iv_debug_overlay*, const vec3_t&, vec3_t&);
		return (*(original_fn * *)this)[11](this, std::ref(in), std::ref(out));
	}
};