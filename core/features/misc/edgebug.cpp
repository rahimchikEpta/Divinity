#include "../features.hpp"

//void edgebug::on_mouse_override ( float* x ) {
//	if ( in_edgebug )
//		*x = 0.f;
//}
//
//void edgebug::on_edgebug ( c_usercmd* cmd ) {
//	if ( in_edgebug ) {
//		if ( should_duck )
//			cmd->buttons |= ~in_duck;
//		cmd->sidemove = 0.f;
//		cmd->forwardmove = 0.f;
//		cmd->buttons &= ~0x400;
//		cmd->buttons &= ~0x8;
//		cmd->buttons &= ~0x10;
//	}
//}
//
//bool edgebug::can_edgebug ( c_usercmd* cmd, player_t* local ) {
//	if ( !local )
//		return false;
//
//	if ( local->velocity ().z > 0 )
//		return false;
//
//	// you cant bug on da edge if u arent on da ground - mao
//	if ( local->flags () & fl_onground )
//		return false;
//
//	if ( (int) roundf ( local->velocity ().z ) == 00.f || old_flags & fl_onground ) {
//		in_edgebug = false;
//		force_cancel = true;
//		return false;
//	}
//	else if ( old_velocity.z < -6.f && local->velocity ().z > old_velocity.z && local->velocity ().z < -6.f && !( local->flags () & fl_onground ) ) {
//
//		static convar* sv_gravity = interfaces::console->get_convar ( "sv_gravity" );
//		float z_vel = ( sv_gravity->get_float () * 0.5f * interfaces::globals->interval_per_tick );
//
//		return old_velocity.z < -z_vel && round ( local->velocity ().z ) == -round ( z_vel ) && local->move_type () != movetype_ladder;
//	}
//}
//
//
//void edgebug::pre_cmd () {
//	old_velocity = csgo::local_player->velocity ();
//	old_flags = csgo::local_player->flags ();
//}
//
//void edgebug::post_cmd ( c_usercmd* cmd ) {
//
//
//	for ( int ticks_ran = 0; ticks_ran < 2; ++ticks_ran ) {
//		if ( ticks_ran == 1 ) {
//			should_duck = true;
//		}
//	}
//}