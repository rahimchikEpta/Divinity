#pragma once
#include "../../../dependencies/utilities/csgo.hpp"
#define TICK_INTERVAL            ( interfaces::globals->interval_per_tick )
#define time_to_ticks( t )        ( (int)( 0.5f + (float)( t ) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )        ( TICK_INTERVAL * (float)( t ) )
#define ROUND_TO_TICKS( t )        ( TICK_INTERVAL * time_to_ticks( t ) )
#define TICK_NEVER_THINK        ( -1 )

namespace prediction_backup
{
	inline player_move_data data{};
	inline vec3_t		origin{};
	inline vec3_t		velocity{};
	inline vec3_t      angles{};
	inline c_usercmd* cmd{};
	inline float		forward_move{};
	inline float		side_move{};
	inline float		curtime{};
	inline float		frametime{};
	inline bool		in_pred = false;
	inline bool		in_first_time = true;
	inline int		tickcount{};
	inline int       player_tickbase{};
	inline int		movetype{};
	inline int		flags{};
	inline int* prediction_random_seed{};
	inline player_t* prediction_player{ nullptr };
}

namespace prediction {
	inline bool is_prediction_runnable()
	{
		return
			(
				interfaces::engine->is_connected() //not ingame, dont predict
				&& interfaces::engine->is_in_game() //not ingame, dont predict
				&& csgo::local_player //null check
				);
	}
	inline void backup_originals(c_usercmd* cmd)
	{
		//player						   
		prediction_backup::flags = csgo::local_player->flags();
		prediction_backup::velocity = csgo::local_player->velocity();
		prediction_backup::origin = csgo::local_player->origin();
		prediction_backup::player_tickbase = csgo::local_player->get_tick_base();
		prediction_backup::movetype = csgo::local_player->move_type();
		//cmd
		prediction_backup::cmd = cmd;
		prediction_backup::angles = cmd->viewangles;
		prediction_backup::side_move = cmd->sidemove;
		prediction_backup::forward_move = cmd->forwardmove;
	};
	inline void process_buttons(c_usercmd* cmd)
	{
		int buttons = csgo::cmd->buttons;
		//GetButtons
		int* m_button_last = reinterpret_cast<int*>(uintptr_t(csgo::local_player) + 0x3208);
		int buttons_changed = buttons ^ *m_button_last;
		//GetButtonLast
		*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 0x31FC) = *m_button_last;
		*m_button_last = buttons;
		//GetButtonPressed
		*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 0x3200) = buttons & buttons_changed;
		//GetButtonReleased
		*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 0x31F4) = buttons_changed & ~buttons;
	};
	void post_think(player_t* player);
	void start(c_usercmd* cmd);
	void end();

	inline void* data;
	inline float old_cur_time;
	inline float old_frame_time;
	inline vec3_t old_velocity;
	inline float old_stamina;
	inline vec3_t old_origin;
	inline vec3_t old_viewangles;
	inline int* prediction_random_seed;
	extern bool in_prediction;
};
