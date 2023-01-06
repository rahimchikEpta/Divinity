#include "../features.hpp"
#include <config.hpp>

void misc::movement::strafe_optimiser ( c_usercmd* cmd ) {
	if ( !settings::movement::strafe_optimiser )
		return;

	if ( ( settings::movement::strafe_optimiser_on_key && !GetAsyncKeyState ( settings::movement::strafe_optimiser_key ) ) )
		return;
	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	static auto m_yaw = interfaces::console->get_convar("m_yaw");
	static auto sensitivity = interfaces::console->get_convar("sensitivity");
	static float previous_view_angles_yaw = cmd->viewangles.y;

	if (cmd->forwardmove == 0 && !(csgo::local_player->flags() & fl_onground)) if (auto velocity = csgo::local_player->velocity(); velocity.length_2d() > settings::movement::required_speed) {
		interfaces::prediction->Update(interfaces::clientstate->delta_tick,
			interfaces::clientstate->delta_tick > 0, interfaces::clientstate->last_command_ack,
			interfaces::clientstate->last_outgoing_command + interfaces::clientstate->choked_commands);

		float mouse_yaw_factor = m_yaw->get_float();
		float mouse_sensitivity = sensitivity->get_float();
		float mouse_yaw_step = mouse_sensitivity * mouse_yaw_factor;

		if (cmd->sidemove < 0) {
			if (previous_view_angles_yaw - cmd->viewangles[1] < 0) {
				float strafe_angle = remainderf(cmd->viewangles[1] - atan2f(velocity[1], velocity[0]) * 180 / M_PI, 360) * settings::movement::desired_gain / 100;
				if (strafe_angle < -mouse_yaw_step) {
					if (strafe_angle < -180) {
						strafe_angle = -180;
					}
					cmd->viewangles[1] = remainderf(cmd->viewangles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
					cmd->mousedx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->viewangles[1], 360) / sqrtf(mouse_yaw_step)));
					interfaces::engine->set_view_angles(cmd->viewangles);
				}
			}
		}
		else {
			if (cmd->sidemove > 0) {
				if (previous_view_angles_yaw - cmd->viewangles[1] > 0) {
					float strafe_angle = remainderf(cmd->viewangles[1] - atan2f(velocity[1], velocity[0]) * 180 / M_PI, 360) * settings::movement::desired_gain / 100;
					if (strafe_angle > mouse_yaw_step) {
						if (strafe_angle > 180) {
							strafe_angle = 180;
						}
						cmd->viewangles[1] = remainderf(cmd->viewangles[1] - mouse_yaw_step * roundf(strafe_angle / mouse_yaw_step), 360);
						cmd->mousedx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_angles_yaw - cmd->viewangles[1], 360) / sqrtf(mouse_yaw_step)));
						interfaces::engine->set_view_angles(cmd->viewangles);

					}
				}
			}
		}
	}
	previous_view_angles_yaw = cmd->viewangles[1];
}