#pragma once
#include "../../../source-sdk/math/vector3d.hpp"
#include "../../../source-sdk/classes/c_usercmd.hpp"

namespace data {

	// * Globals * \\
	
	namespace pre_command {
		inline c_usercmd* run_command;
		inline int buttons;
		inline int flags;
		inline int move_type;
		inline vec3_t velocity;
		inline int mousedelta_x;
		inline int mousedelta_y;
		inline int forward_move;
		inline int side_move;
		inline float gravity;
		inline vec3_t viewangles;
	}

	namespace post_command {
		inline c_usercmd* run_command;
		inline int buttons;
		inline int flags;
		inline int move_type;
		inline vec3_t velocity;
		inline int mousedelta_x;
		inline int mousedelta_y;
		inline int forward_move;
		inline int side_move;
		inline float gravity;
		inline vec3_t viewangles;
	}
	// * Movement * \\

	namespace movement {
		namespace c {
			bool can_edgebug ( vec3_t pre_velocity );
		}
		inline bool in_edgebug;
		inline bool crouch_detected;
		namespace info {
			inline vec3_t origin_of_client_on_successful_eb;
			inline vec3_t origin_of_client_on_eb_detection;
		}
	}
	// * Game data functions * \\

	namespace called {
		void grab_pre_command_information ( c_usercmd* cmd );
		void grab_post_command_information ( c_usercmd* cmd );
	}

}