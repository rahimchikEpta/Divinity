#pragma once
#include "../../source-sdk/math/vector3d.hpp"
#include "../../source-sdk/classes/c_usercmd.hpp"
template <typename I>
__forceinline I v2(void* iface, uint32_t index) { return (I)((*(uint32_t**)iface)[index]); }
class IMoveHelper
{
private:
	virtual	void unknown1() = 0;
public:
	virtual void SetHost(player_t* host) = 0;
private:
	virtual void unknown2() = 0;
	virtual void unknown3() = 0;
public:
	virtual void ProcessImpacts() = 0;
};

class player_move_data {
public:
	bool    first_run_of_instructions : 1;
	bool    game_code_moved_player : 1;
	int     player_handle;
	int     impulse_command;
	vec3_t	view_angles;
	vec3_t	abs_view_angles;
	int     buttons;
	int     old_buttons;
	float   fw_move;
	float   sd_move;
	float   up_move;
	float   max_speed;
	float   client_max_speed;
	vec3_t	velocity;
	vec3_t	angles;
	vec3_t	old_angles;
	float   step_height;
	vec3_t	wish_velocity;
	vec3_t	jump_velocity;
	vec3_t	constraint_center;
	float   constraint_radius;
	float   constraint_width;
	float   constraint_speed_factor;
	float   u0[ 5 ];
	vec3_t	abs_origin;
};

class virtual_game_movement {

public:
	virtual				~virtual_game_movement( void ) {}
	virtual void			process_movement( player_t *player, player_move_data *move ) = 0;
	virtual void			reset( void ) = 0;
	virtual void			start_track_prediction_errors( player_t *player ) = 0;
	virtual void			finish_track_prediction_errors( player_t *player ) = 0;
	virtual void			diff_print( char const *fmt, ... ) = 0;
	virtual vec3_t const	&get_player_mins( bool ducked ) const = 0;
	virtual vec3_t const	&get_player_maxs( bool ducked ) const = 0;
	virtual vec3_t const	&get_player_view_offset( bool ducked ) const = 0;
	virtual bool			is_moving_player_stuck( void ) const = 0;
	virtual player_t		*get_moving_player( void ) const = 0;
	virtual void			unblock_posher( player_t *player, player_t *pusher ) = 0;
	virtual void			setup_movement_bounds( player_move_data *move ) = 0;
};

class player_game_movement : public virtual_game_movement {
public:
	virtual ~player_game_movement(void) { }
};

class IPrediction
{
public:

	virtual ~IPrediction(void) = 0;
	virtual void Init(void) = 0;
	virtual void Shutdown(void) = 0;
	virtual void Update
	(
		int startframe, // World update ( un-modded ) most recently received
		bool validframe, // Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command // Last command (most recent) sent to server (un-modded)
	) = 0;
	virtual void	PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void	PostEntityPacketReceived(void) = 0;
	virtual void	PostNetworkDataReceived(int commands_acknowledged) = 0;
	virtual void	OnReceivedUncompressedPacket(void) = 0;
	// The engine needs to be able to access a few predicted values
	virtual void	GetViewOrigin(vec3_t& org) = 0;
	virtual void	SetViewOrigin(vec3_t& org) = 0;
	virtual void	GetViewAngles(vec3_t& ang) = 0;
	virtual void	SetViewAngles(vec3_t& ang) = 0;

	virtual void	GetLocalViewAngles(vec3_t& ang) = 0;
	virtual void	SetLocalViewAngles(vec3_t& ang) = 0;

	__forceinline void run_command(player_t* player, c_usercmd* cmd, IMoveHelper* helper)
	{
		return v2<void(__thiscall*)(void*, player_t*, c_usercmd*, IMoveHelper*)>(this, 19)(this, player, cmd, helper);
	}
	__forceinline void check_moving_ground(player_t* player, double frametime)
	{
		return v2<void(__thiscall*)(void*, player_t*, double)>(this, 18)(this, player, frametime);
	}
	__forceinline void setup_move(player_t* player, c_usercmd* cmd, IMoveHelper* helper, player_move_data* data)
	{
		return v2<void(__thiscall*)(void*, player_t*, c_usercmd*, IMoveHelper*, void*)>(this, 20)(this, player, cmd, helper, data);
	}
	__forceinline void finish_move(player_t* player, c_usercmd* cmd, void* data)
	{
		return v2<void(__thiscall*)(void*, player_t*, c_usercmd*, void*)>(this, 21)(this, player, cmd, data);
	}
	__forceinline bool in_prediction()
	{
		return v2<bool(__thiscall*)(void*)>(this, 14)(this);
	}
	__forceinline bool first_time_pedicted()
	{
		return v2<bool(__thiscall*)(void*)>(this, 15)(this);
	}

	int32_t& predicted_commands()
	{
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + 0x1C);
	}

	int32_t& previous_start_frame()
	{
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + 0xC);
	}

	int32_t& server_commands_acknowledged()
	{
		return *reinterpret_cast<int32_t*>(reinterpret_cast<uintptr_t>(this) + 0x20);
	}

	//restore player globals to frame. Duh 
	//https://github.com/click4dylan/mutiny_csgo_public/blob/master/CSGOFullv2/IPrediction.h#L158
	void restore_entity_to_predicted_frame(const int frame)
	{
		static auto pointer = reinterpret_cast<void(__stdcall*)(int, int)>
			(utilities::pattern_scan
			(
				"client.dll", "55 8b ec 8b 4d ? 56 e8 ? ? ? ? 8b 75"
			)
				);
		pointer(0, frame);
	}

	///purpose: return to original globals and stored data after running prediction, example we ran pred 60 times,
	///come back 61 times and restore all values to original so we could re predict.
	void Repredict()
	{
		this->restore_entity_to_predicted_frame(this->predicted_commands() - 1);
		this->previous_start_frame() = -1;
		this->predicted_commands() = 0;
	}


	char pad00[8]; 		 // 0x0000
	bool mInPrediction;		 // 0x0008
	bool mFirstTimePredicted;// 0x0009
	bool mEnginePaused;		 // 0x000A
};