#include "discord_register.h"
#include "discord_rpc.h"
#include "Memory.h"
#include <Windows.h>
#include <ctime>
#include <string>
#include <iostream>
#include <chrono>
#pragma comment(lib, "discord-rpc.lib")
#include "../../../dependencies/interfaces/interfaces.hpp"
#include "../../xor.h"
#include "../../../dependencies/utilities/csgo.hpp"

int i = 0;

bool test = true;


time_t now = time(0);

//char* date_time = ctime(&now);



//float runtime =

DiscordRichPresence discordPresence;
void UpdatePresence()
{
	i++;
	std::string namemap = interfaces::engine->get_level_name();
	std::string mapname = xorstr_("Playing on: ") + namemap;

	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	if (interfaces::engine->is_in_game() || interfaces::engine->is_connected()) {
		discordPresence.state = mapname.c_str();
	}
	else {
		discordPresence.state = ("Main menu");
	}
#ifdef _DEBUG
	discordPresence.details = "Debug build :3";
#else
	discordPresence.details = std::format(xorstr_("Active user: {} {}"), csgo::cheat_username, csgo::cheat_username == xorstr_("Ichigo") ? xorstr_("[dev]") : csgo::cheat_username == xorstr_("Maki") ? xorstr_("[owner]") : csgo::cheat_username == xorstr_("Vyzer") ? xorstr_("[alpha]") : xorstr_("[stable]")).data();
#endif

	discordPresence.largeImageKey = csgo::cheat_username == xorstr_("Ichigo") ? xorstr_("ichigo_custom") : csgo::cheat_username == xorstr_("Vyzer") ? xorstr_("preds_asylum") : csgo::cheat_username == xorstr_("Maki") ? xorstr_("maki_custom") : xorstr_("glaive_falling"); //large image file name no extension
	discordPresence.largeImageText = xorstr_("divinity.solutions");// csgo::cheat_username.data();
	discordPresence.smallImageKey = ""; //same as large
	discordPresence.smallImageText = "";//std::format(xorstr_("squid game user: {}"), csgo::cheat_username).data(); //displays on hover
	Discord_UpdatePresence(&discordPresence); //do the do

}

void Initialize()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(xorstr_("946267138329555055"), &handlers, TRUE, nullptr);
}

void Shutdown()
{
	Discord_Shutdown(); //goodbye
}

void DiscordMain() //call in dllmain or hook w/e
{
	Initialize();
	UpdatePresence();
}