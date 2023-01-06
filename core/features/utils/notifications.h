#pragma once
#include "../features.hpp"
#include <string>
#include <deque>
#include "../../menu/ImGui/custom.h"
#include "../../xor.h"
namespace notifications {
	struct notify_t
	{
		std::string text;
		float time;
		color _color;
		notify_t(std::string _text, color __color)
		{
			text = _text;
			_color = __color;
			time = interfaces::globals->cur_time;
		}
	};

	inline std::vector<notify_t> notify_list;

	void draw();
	void notify(std::string text, color _color = color(255, 255, 255, 255));
}