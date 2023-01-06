#pragma once
#include "../math/vector2d.hpp"
struct vertex_t {
	float m_x, m_y, m_z, m_rhw;
	std::uint32_t m_color;
};