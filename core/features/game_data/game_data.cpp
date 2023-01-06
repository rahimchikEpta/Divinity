#include "game_data.h"
#include "../../../dependencies/utilities/csgo.hpp"


// This is where all of your createmove/command data is handled.
// Some insights, this is mostly used for edgebug and other shit like that.


// Pre command is before the first engine prediction call.
void data::called::grab_pre_command_information ( c_usercmd* cmd ) {
	data::pre_command::run_command = cmd;
	data::pre_command::velocity = csgo::local_player->velocity ();
	data::pre_command::buttons = cmd->buttons;
	data::pre_command::mousedelta_x = cmd->mousedx;
	data::pre_command::mousedelta_y = cmd->mousedy;
	data::pre_command::move_type = csgo::local_player->move_type ();
	data::pre_command::flags = csgo::local_player->flags ();
	data::pre_command::forward_move = cmd->forwardmove;
	data::pre_command::side_move = cmd->sidemove;
	data::pre_command::gravity = interfaces::console->get_convar ( "sv_gravity" )->get_float ();
}

// Post command is after the first engine prediction call.
void data::called::grab_post_command_information ( c_usercmd* cmd ) {
	data::post_command::run_command = cmd;
	data::post_command::velocity = csgo::local_player->velocity ();
	data::post_command::buttons = cmd->buttons;
	data::post_command::mousedelta_x = cmd->mousedx;
	data::post_command::mousedelta_y = cmd->mousedy;
	data::post_command::move_type = csgo::local_player->move_type ();
	data::post_command::flags = csgo::local_player->flags ();
	data::post_command::forward_move = cmd->forwardmove;
	data::post_command::side_move = cmd->sidemove;
	data::post_command::gravity = interfaces::console->get_convar ( "sv_gravity" )->get_float ();
}

bool data::movement::c::can_edgebug ( vec3_t pre_velocity ) {
	// Get the velocity ConVar.
	static convar* sv_gravity = interfaces::console->get_convar ( "sv_gravity" );
	// Create a float for your fall velocity that the edgebug should be hit at.
	float z_vel = ( sv_gravity->get_float () * 0.5f * interfaces::globals->interval_per_tick );

	// Return if your velocity and the "edgebug" velocity hits the same.
	return pre_velocity.z < -z_vel &&
		round ( csgo::local_player->velocity ().z ) == -round ( z_vel ) &&
		csgo::local_player->move_type () != movetype_ladder;
}
