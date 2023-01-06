#include "engine_prediction.hpp"
#include "../../xor.h"

bool prediction::in_prediction;
template<unsigned int index, typename tret, typename ... T>
__forceinline static auto vfunc2(void* thisptr, T ... arglist) -> tret
{
	using fn = tret(__thiscall*)(void*, decltype(arglist)...);
	return (*static_cast<fn**>(thisptr))[index](thisptr, arglist...);
}
void prediction::post_think(player_t* player) {
	static const auto PostThinkVPhysics = address(utilities::scan_pattern((uint32_t*)GetModuleHandle("client.dll"), xorstr_("E8 ? ? ? ? 8B CF E8 ? ? ? ? 8B 06"))).relative().as<bool(__thiscall*)(player_t*)>();
	static const auto SimulatePlayerSimulatedEntities = address(utilities::scan_pattern((uint32_t*)GetModuleHandle("client.dll"), xorstr_("E8 ? ? ? ? 8B 06 8B CE 5F 5E FF A0 88"))).relative().as<void(__thiscall*)(player_t*)>();

	vfunc2<33, void>(interfaces::model_cache);

	if (player->is_alive() || *reinterpret_cast<std::uint32_t*>(uintptr_t(player) + 0x9AE1)) {
		player->update_colision_bounds();

		if (player->flags() & fl_onground)
			player->fall_velocity() = 0;

		if (player->get_sequence() == -1)
			player->set_sequence(0);

		player->studio_frame_advance();

		PostThinkVPhysics(player);
	}
	SimulatePlayerSimulatedEntities(player);

	vfunc2<34, void>(interfaces::model_cache);
}
int custom_tickbase_gather(c_usercmd* cmd, player_t* player)
{
	static int tick = 0;

	if (cmd)
	{
		static c_usercmd* last_cmd = nullptr;
		// if command was not predicted - increment tickbase
		if (!last_cmd || last_cmd->predicted)
			tick = player->get_tick_base();
		else
			tick++;

		last_cmd = cmd;
	}

	return tick;
}
void prediction::start(c_usercmd* cmd) {
	if (!is_prediction_runnable()) return;

	*csgo::local_player->current_cmd() = cmd;
	csgo::local_player->last_cmd() = *cmd;

	if (!prediction_backup::prediction_random_seed)
		prediction_backup::prediction_random_seed = *reinterpret_cast<int**>(utilities::pattern_scan("client.dll", sig_prediction_random_seed) + 2);

	*prediction_backup::prediction_random_seed = cmd->randomseed & 0x7FFFFFFF;

	//globals
	prediction_backup::curtime = interfaces::globals->cur_time;
	prediction_backup::frametime = interfaces::globals->frame_time;
	prediction_backup::tickcount = interfaces::globals->tick_count;
	//prediction		 			   
	prediction_backup::in_first_time = interfaces::prediction->mFirstTimePredicted;
	prediction_backup::in_pred = interfaces::prediction->mInPrediction;

	interfaces::globals->cur_time = csgo::local_player->get_tick_base() * interfaces::globals->interval_per_tick;
	interfaces::globals->frame_time = interfaces::globals->interval_per_tick;
	prediction::in_prediction = 1;
	//cmd->buttons |= csgo::local_player->button_forced();
	//cmd->buttons &= ~(csgo::local_player->button_disabled());

	interfaces::game_movement->start_track_prediction_errors(csgo::local_player);
	if (cmd->impulse)
		*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 0x320C) = cmd->impulse;

//	process_buttons(cmd);

	interfaces::prediction->check_moving_ground(csgo::local_player, interfaces::globals->frame_time);

	interfaces::prediction->SetLocalViewAngles(cmd->viewangles);
	if (csgo::local_player->PhysicsRunThink(0))
		csgo::local_player->PreThink();

	const auto next_think_tick = static_cast<int>(*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 0xFC));
	//player->GetTickBase()
	if (next_think_tick > 0 && next_think_tick <= static_cast<int>(*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 0x3440)) && csgo::local_player->is_alive())
	{
		*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 0xFC) = -1;
		// player->Think();
		(*(void(__thiscall**)(player_t*))(*reinterpret_cast<std::uint32_t*>(uintptr_t(csgo::local_player) + 556)))(csgo::local_player);
	}
	interfaces::prediction->setup_move(csgo::local_player, cmd, interfaces::move_helper, &prediction_backup::data);

	interfaces::move_helper->SetHost(csgo::local_player);

	interfaces::game_movement->process_movement(csgo::local_player, &prediction_backup::data);

	interfaces::prediction->finish_move(csgo::local_player, cmd, &prediction_backup::data);
	interfaces::move_helper->ProcessImpacts();

	post_think(csgo::local_player);

	interfaces::prediction->mInPrediction = prediction_backup::in_pred;
	interfaces::prediction->mFirstTimePredicted = prediction_backup::in_first_time;
}

void prediction::end() {
	if (!is_prediction_runnable()) return;
	interfaces::game_movement->finish_track_prediction_errors(csgo::local_player);
	interfaces::prediction->mInPrediction = false;

	interfaces::move_helper->SetHost(nullptr);

	interfaces::globals->cur_time = prediction_backup::curtime;
	interfaces::globals->frame_time = prediction_backup::frametime;
	prediction::in_prediction = 0;
	*csgo::local_player->current_cmd() = NULL;

	*prediction_backup::prediction_random_seed = -1;

	interfaces::game_movement->reset();

	if (!interfaces::prediction->mEnginePaused && interfaces::globals->frame_time > 0.f)
	{
		++csgo::local_player->get_tick_base();
		//	++* reinterpret_cast<std::uint32_t*>(uintptr_t(player) + 0x3440);
	}
}
