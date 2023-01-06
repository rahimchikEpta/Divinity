#include "notifications.h"
#include "../../menu/menu.hpp"
#include "../../../dependencies/utilities/rendering/rendering.hpp"

#pragma warning(disable : 4996)

const wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

void notifications::draw()
{
	const auto stay_time = 5.f;

	for (auto i = 0; i < notify_list.size(); i++)
	{
		if (notify_list.empty())
			continue;

		auto notify = notify_list[i];

		if ((interfaces::globals->cur_time > notify.time + stay_time) || (notify_list.size() > 10))
		{
			notify_list.erase(notify_list.begin() + i);
			i--;
			continue;
		}

		auto eventlog = xorstr_("[inspre]") + notify.text;

		int width = 0, height = 0;
		interfaces::surface->get_text_size(rendering::fonts::esp, GetWC(eventlog.c_str()), width, height);

		rendering::filled_rect(4, 6 + (i * 24), width + 16, 21, color(33, 33, 33, 255));
		rendering::filled_rect(width + 18, 6 + (i * 24), 2, 21, notify._color);
		rendering::text(10, 10 + (i * 24), rendering::fonts::esp, xorstr_("[inspre]"), false, notify._color);
		rendering::text(55, 10 + (i * 24), rendering::fonts::esp, notify.text.c_str(), false, color(255, 255, 255));
	}
}

void notifications::notify(std::string text, color color)
{
	if (text.empty())
		return;

	notify_list.push_back(notify_t(text, color));

	interfaces::console->console_color_printf(color, xorstr_("[inspre] "));
	interfaces::console->console_printf(text.append(xorstr_(" \n")).c_str());
}