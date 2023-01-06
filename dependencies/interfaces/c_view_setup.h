#pragma once
#include <cstdint>
#include <rpcndr.h>
#include "../../source-sdk/math/vector3d.hpp"


enum motion_blur_mode_t
{
	motion_blur_disable = 1,
	motion_blur_game = 2,
	motion_blur_sfm = 3
};
class c_view_setup
{
public:
	byte _0x0000[16];  // 0x0000
	int width;           // 0x0010
	int width_old;       // 0x0014
	int height;          // 0x0018
	int height_old;      // 0x001C
	int x;               // 0x0020
	int x_old;           // 0x0024
	int y;               // 0x0028
	int y_old;           // 0x002C
	byte _0x0030[128]; // 0x0030
	float fov;           // 0x00B0
	float fov_viewmodel; // 0x00B4
	vec3_t origin;   // 0x00B8
	vec3_t angles;
	float znear;
	float zfar;
	float znear_viewmodel;
	float zfar_viewmodel;
	float aspect_ratio;
	float near_blur_depth;
	float near_focus_depth;
	float far_focus_depth;
	float far_blur_depth;
	float near_blur_radius;
	float far_blur_radius;
	int dof_quality;
	motion_blur_mode_t motion_blur_mode;
	float shutter_time;
	vec3_t shutter_open_position;
	vec3_t shutter_open_angles;
	vec3_t shutter_close_position;
	vec3_t shutter_close_angles;
	float off_center_top;
	float off_center_bottom;
	float off_center_left;
	float off_center_right;
	int edge_blur;
};


