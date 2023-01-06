#include "csgo.hpp"

namespace csgo {
	player_t* local_player = nullptr;
	std::string cheat_username = "";
    std::string build_type = "beta";
    i_client_networkable* networkable;
    c_usercmd* cmd = nullptr;
}

float csgo::server_time ( c_usercmd* cmd ) {
    static int tick;
    static c_usercmd* last_cmd;

    if ( cmd ) {
        if ( !last_cmd || last_cmd->predicted )
            tick = csgo::local_player->get_tick_base ();
        else
            tick++;
        last_cmd = cmd;
    }
    return tick * interfaces::globals->interval_per_tick;
}