#include <config.hpp>
#include "../features.hpp"
#include "../../xor.h"
#include "../../menu/menu.hpp"
legitbot_settings ctx;

vec3_t calculate_relative_angle(const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles) noexcept
{
	return ((destination - source).to_angle() - viewAngles).normalized();
}

void legit_bot::event(i_game_event* event) {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	auto event_name = fnv::hash(event->get_name());
	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker || !event_name)
		return;

	switch (event_name) {
	case fnv::hash("player_death"):
		if (attacker == csgo::local_player)
			legit_bot::kill_delay = interfaces::globals->tick_count + settings::aimbot::delay_after_kill;

		break;
	case fnv::hash("round_start"):
		legit_bot::kill_delay = 0;

		break;
	}
};

bool is_visible(player_t* ent, vec3_t position) {
	trace_t trace;
	trace_filter filter;
	filter.skip = csgo::local_player;
	interfaces::trace_ray->trace_ray({ csgo::local_player->get_eye_pos(), position.not_null() ? position : ent->get_bone_position(8) }, 0x46004009, &filter, &trace);
	return trace.entity == ent || trace.flFraction > 0.97f;
}
void legit_bot::aimbot(c_usercmd* cmd) {
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (!(settings::aimbot::type == 0 ? GetAsyncKeyState(settings::aimbot::aim_key) : settings::aimbot::type == 1 ? GetAsyncKeyState(settings::aimbot::aim_key) & 1 : 0))
		return;


	auto weapon = csgo::local_player->active_weapon();
	if (!weapon || !weapon->clip1_count() || weapon->is_knife() || weapon->is_nade() || weapon->is_c4() || weapon->next_primary_attack() > csgo::server_time())
		return;

	if (menu::is_open)
		return;

	if (weapon->is_rifle()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_RIFLE];
		ctx.aim_type = settings::aimbot::c_aimtype[WEAPONS_RIFLE];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_RIFLE];
		ctx.rcs = settings::aimbot::c_rcs[WEAPONS_RIFLE];
		ctx.smooth = settings::aimbot::c_smooth[WEAPONS_RIFLE];
		ctx.hitboxes[0] = settings::aimbot::rifle_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::rifle_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::rifle_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::rifle_hitboxes[3];
	}
	else if (weapon->is_sniper()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_SNIPER];
		ctx.aim_type = settings::aimbot::c_aimtype[WEAPONS_SNIPER];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_SNIPER];
		ctx.rcs = settings::aimbot::c_rcs[WEAPONS_SNIPER];
		ctx.smooth = settings::aimbot::c_smooth[WEAPONS_SNIPER];
		ctx.hitboxes[0] = settings::aimbot::sniper_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::sniper_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::sniper_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::sniper_hitboxes[3];
	}
	else if (weapon->is_pistol()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_PISTOL];
		ctx.aim_type = settings::aimbot::c_aimtype[WEAPONS_PISTOL];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_PISTOL];
		ctx.rcs = settings::aimbot::c_rcs[WEAPONS_PISTOL];
		ctx.smooth = settings::aimbot::c_smooth[WEAPONS_PISTOL];
		ctx.hitboxes[0] = settings::aimbot::pistol_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::pistol_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::pistol_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::pistol_hitboxes[3];
	}
	else if (weapon->is_smg()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_SMG];
		ctx.aim_type = settings::aimbot::c_aimtype[WEAPONS_SMG];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_SMG];
		ctx.rcs = settings::aimbot::c_rcs[WEAPONS_SMG];
		ctx.smooth = settings::aimbot::c_smooth[WEAPONS_SMG];
		ctx.hitboxes[0] = settings::aimbot::smg_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::smg_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::smg_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::smg_hitboxes[3];
	}
	else if (weapon->is_heavy()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_HEAVY];
		ctx.aim_type = settings::aimbot::c_aimtype[WEAPONS_HEAVY];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_HEAVY];
		ctx.rcs = settings::aimbot::c_rcs[WEAPONS_HEAVY];
		ctx.smooth = settings::aimbot::c_smooth[WEAPONS_HEAVY];
		ctx.hitboxes[0] = settings::aimbot::heavy_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::heavy_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::heavy_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::heavy_hitboxes[3];
	}
	else {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_GENERAL];
		ctx.aim_type = settings::aimbot::c_aimtype[WEAPONS_GENERAL];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_GENERAL];
		ctx.rcs = settings::aimbot::c_rcs[WEAPONS_GENERAL];
		ctx.smooth = settings::aimbot::c_smooth[WEAPONS_GENERAL];
		ctx.hitboxes[0] = settings::aimbot::general_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::general_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::general_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::general_hitboxes[3];
	}

	if (weapon->is_sniper() && settings::aimbot::scoping_required && !csgo::local_player->is_scoped())
		return;

	auto settings = ctx;

	if (!settings.enabled)
		return;

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	if (weapon->is_pistol() && weapon->next_primary_attack() > csgo::server_time() && settings::aimbot::auto_pistol)
		cmd->buttons &= ~in_attack;
	
	if (settings::aimbot::delay_after_kill > 1 && legit_bot::kill_delay >= interfaces::globals->tick_count)
		return;

	

	auto best_fov = settings.fov;
	auto best_target = vec3_t(0, 0, 0);
	auto local_eye_position = csgo::local_player->get_eye_pos();

	std::vector<int> bones;

	if (settings.hitboxes[0]) { //head
		bones.emplace_back(8); // THIS IS NECK IT IS VEY BAD bones.emplace_back(7);
	}

	if (settings.hitboxes[1]) { //chest
		bones.emplace_back(6); bones.emplace_back(5); bones.emplace_back(4); bones.emplace_back(3);
	}

	if (settings.hitboxes[2]) { //hands
		bones.emplace_back(12); bones.emplace_back(40);
	}

	if (settings.hitboxes[3]) { //legs
		bones.emplace_back(65); bones.emplace_back(66); bones.emplace_back(72); bones.emplace_back(73);
	}

	static auto game_type = interfaces::console->get_convar("game_type");
	static auto weapon_recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->get_float();

	for (int i = 1; i <= 64; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive() || entity->has_gun_game_immunity()
			|| csgo::local_player->is_in_air() && !settings::aimbot::target_in_air)
			continue;

		if (game_type->get_int() == 6) {
			if ((csgo::local_player->survival_team() != -1) && (entity->survival_team() == csgo::local_player->survival_team()))
				continue;

			if (weapon->is_non_aim_weapon())
				continue;
		}
		else if (entity->team() == csgo::local_player->team() && !settings::aimbot::target_team)
			continue;

		for (const auto bone : bones) {
			auto bone_position = entity->get_bone_position(bone);

			if ((!csgo::local_player->can_see_player_pos(entity, bone_position) && settings::aimbot::visible_only)
				|| utilities::goes_through_smoke(local_eye_position, bone_position) && !settings::aimbot::target_smoke)
				continue;

			auto angle = math::calculate_angle2(local_eye_position, bone_position, cmd->viewangles).clamped();
			angle -= (csgo::local_player->aim_punch_angle() * (weapon_recoil_scale * settings.rcs));

			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < best_fov) {

				best_target = bone_position;
				best_fov = fov;
			}
		}
	}

	if (best_target.zero())
		return;

	static vec3_t last_angles{ cmd->viewangles };
	static int last_cmd{};

	if (last_cmd == cmd->command_number - 1 && !last_angles.zero() && settings::aimbot::silent)
		cmd->viewangles = last_angles;

	auto angle = math::calculate_angle2(local_eye_position, best_target, cmd->viewangles).clamped() /= settings.smooth;
	angle -= (csgo::local_player->aim_punch_angle() * (weapon_recoil_scale * settings.rcs)) /= settings.smooth;
	cmd->viewangles += angle.clamped();

	if (!settings::aimbot::silent)
		interfaces::engine->set_view_angles(cmd->viewangles);
	
	if (ctx.smooth > 1.f)
		last_angles = cmd->viewangles;
	else
		last_angles = vec3_t{};
	last_cmd = cmd->command_number;

}

bool legit_bot::hit_chance(vec3_t ang, player_t* player, float hitchance) {
	constexpr auto iterations = 150.f;

	auto weapon = csgo::local_player->active_weapon();

	if (!weapon)
		return false;

	vec3_t forward, right, up;
	const auto src = csgo::local_player->get_eye_pos();
	math::angle_vectors_alternative(ang, &forward, &right, &up);

	hitchance *= 100.f;

	auto c_hits = 0;
	const auto c_needed_hits = static_cast<int> (iterations * (hitchance / 100.f));

	weapon->update_accuracy_penalty();
	const auto weapon_spread = weapon->get_spread();
	const auto weapon_inaccuracy = weapon->inaccuracy();

	for (auto i = 0; i < static_cast<int> (iterations); i++) {
		auto a = math::random_float(0.f, 1.f);
		const auto b = math::random_float(0.f, 2.f * M_PI_F);
		auto c = math::random_float(0.f, 1.f);
		const auto d = math::random_float(0.f, 2.f * M_PI_F);

		if (weapon->item_definition_index() == WEAPON_REVOLVER) {
			a = 1.f - a * a;
			c = 1.f - c * c;
		}

		const auto inaccuracy = a * weapon_inaccuracy;
		const auto spread = c * weapon_spread;

		const vec3_t spread_view(cos(b) * inaccuracy + cos(d) * spread, sin(b) * inaccuracy + sin(d) * spread, 0);
		vec3_t direction;

		direction.x = forward.x + spread_view.x * right.x + spread_view.y * up.x;
		direction.y = forward.y + spread_view.x * right.y + spread_view.y * up.y;
		direction.z = forward.z + spread_view.x * right.z + spread_view.y * up.z;
		direction.normalized();

		vec3_t view_angles_spread;
		math::vector_angles2(direction, up, view_angles_spread);
		math::normalize_in_place(view_angles_spread);

		vec3_t view_forward;
		math::angle_vectors2(view_angles_spread, &view_forward);
		view_forward.normalized();

		view_forward = src + view_forward * weapon->get_weapon_data()->weapon_range;

		trace_t tr;
		ray_t ray;

		ray.init(src, view_forward);
		interfaces::trace_ray->clip_ray_to_entity(ray, MASK_SHOT | CONTENTS_GRATE, player, &tr);

		if (tr.entity == player)
			++c_hits;

		if (static_cast<int> (static_cast<float> (c_hits) / iterations * 100.f) > hitchance)
			return true;

		if (static_cast<int> (iterations) - i + c_hits < c_needed_hits)
			return false;
	}

	return false;
}
// TODO: fix lol
void legit_bot::trigger(c_usercmd* cmd) {
	//if ( !settings::aimbot::triggerbot_enable )
	//	return;

	//if ( !settings::aimbot::trigger_type > 0 ? !GetAsyncKeyState ( settings::aimbot::trigger_key ) : 0 )
	//	return;

	//if ( !csgo::local_player || !csgo::local_player->is_alive () )
	//	return;

	//auto weapon = csgo::local_player->active_weapon ();

	//if ( !weapon || weapon->is_knife () || weapon->is_c4 () || weapon->is_nade () || !weapon->clip1_count () /*|| weapon->next_primary_attack () > csgo::server_time ()*/ )
	//	return;


	//auto weapon_data = weapon->get_weapon_data ();

	//if ( !weapon_data )
	//	return;

	//vec3_t src{}, dst{}, forward{};
	//trace_t tr{};
	//ray_t ray{};
	//trace_filter filter{};

	//math::angle_vectors2 ( cmd->viewangles, &forward );
	//forward *= weapon->get_weapon_data ()->weapon_range;
	//filter.skip = csgo::local_player;
	//src = csgo::local_player->get_eye_pos ();
	//dst = src + forward;
	//ray.initialize ( src, dst );
	//interfaces::trace_ray->trace_ray ( ray, 0x46004003, &filter, &tr );

	//static auto weapon_recoil_scale = interfaces::console->get_convar ( "weapon_recoil_scale" )->get_float ();
	//cmd->viewangles -= ( csgo::local_player->aim_punch_angle () * weapon_recoil_scale );

	//if ( !tr.entity || !tr.entity->is_alive () || tr.entity->dormant () || tr.entity->has_gun_game_immunity () || csgo::local_player->team () == tr.entity->team () ||
	//	weapon->is_sniper () && !csgo::local_player->is_scoped () ||
	//	utilities::goes_through_smoke ( csgo::local_player->get_eye_pos (), tr.entity->get_hitbox_position ( hitbox_head ) ) )
	//	return;

	//auto chance = settings::aimbot::trigger_ht / 100.f;

	//if ( !hit_chance ( cmd->viewangles, tr.entity, chance ) )
	//	return;

	//if ( tr.hitGroup == hitgroup_head ||
	//	tr.hitGroup == hitgroup_chest ||
	//	tr.hitGroup == hitgroup_stomach ||
	//	tr.hitGroup == hitgroup_rightarm ||
	//	tr.hitGroup == hitgroup_leftarm ||
	//	tr.hitGroup == hitgroup_leftleg ||
	//	tr.hitGroup == hitgroup_rightleg ) {

	//	if ( trigger_delay >= settings::aimbot::trigger_delay ) {
	//		trigger_delay = 0;
	//		cmd->buttons |= in_attack;
	//	}
	//	trigger_delay++;
	//}
}

void legit_bot::autostop(c_usercmd* cmd)
{

	// no.
	return;
	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;


	auto weapon = csgo::local_player->active_weapon();
	if (!weapon || !weapon->clip1_count() || weapon->is_knife() || weapon->is_nade() || weapon->is_c4())
		return;



	if (weapon->is_rifle()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_RIFLE];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_RIFLE];
		ctx.autostop = settings::aimbot::c_autostop[WEAPONS_RIFLE];
		ctx.hitboxes[0] = settings::aimbot::rifle_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::rifle_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::rifle_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::rifle_hitboxes[3];

	}
	else if (weapon->is_sniper()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_SNIPER];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_SNIPER];
		ctx.autostop = settings::aimbot::c_autostop[WEAPONS_SNIPER];
		ctx.hitboxes[0] = settings::aimbot::sniper_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::sniper_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::sniper_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::sniper_hitboxes[3];
	}
	else if (weapon->is_pistol()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_PISTOL];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_PISTOL];
		ctx.autostop = settings::aimbot::c_autostop[WEAPONS_PISTOL];
		ctx.hitboxes[0] = settings::aimbot::pistol_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::pistol_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::pistol_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::pistol_hitboxes[3];
	}
	else if (weapon->is_smg()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_SMG];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_SMG];
		ctx.autostop = settings::aimbot::c_autostop[WEAPONS_SMG];
		ctx.hitboxes[0] = settings::aimbot::smg_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::smg_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::smg_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::smg_hitboxes[3];
	}
	else if (weapon->is_heavy()) {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_HEAVY];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_HEAVY];
		ctx.autostop = settings::aimbot::c_autostop[WEAPONS_HEAVY];
		ctx.hitboxes[0] = settings::aimbot::heavy_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::heavy_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::heavy_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::heavy_hitboxes[3];
	}
	else {
		ctx.enabled = settings::aimbot::c_enabled[WEAPONS_GENERAL];
		ctx.fov = settings::aimbot::c_fov[WEAPONS_GENERAL];
		ctx.autostop = settings::aimbot::c_autostop[WEAPONS_GENERAL];
		ctx.hitboxes[0] = settings::aimbot::general_hitboxes[0];
		ctx.hitboxes[1] = settings::aimbot::general_hitboxes[1];
		ctx.hitboxes[2] = settings::aimbot::general_hitboxes[2];
		ctx.hitboxes[3] = settings::aimbot::general_hitboxes[3];
	}

	if (weapon->is_sniper() && settings::aimbot::scoping_required && !csgo::local_player->is_scoped())
		return;

	auto settings = ctx;

	if (!settings.enabled)
		return;

	auto weapon_data = weapon->get_weapon_data();

	if (!weapon_data)
		return;

	auto best_fov = settings.fov;
	auto best_target = vec3_t(0, 0, 0);
	auto local_eye_position = csgo::local_player->get_eye_pos();

	std::vector<int> bones;

	if (settings.hitboxes[0]) { //head
		bones.emplace_back(8); bones.emplace_back(7);
	}

	if (settings.hitboxes[1]) { //chest
		bones.emplace_back(6); bones.emplace_back(5); bones.emplace_back(4); bones.emplace_back(3);
	}

	if (settings.hitboxes[2]) { //hands
		bones.emplace_back(12); bones.emplace_back(40);
	}

	if (settings.hitboxes[3]) { //legs
		bones.emplace_back(65); bones.emplace_back(66); bones.emplace_back(72); bones.emplace_back(73);
	}

	static auto game_type = interfaces::console->get_convar("game_type");
	static auto weapon_recoil_scale = interfaces::console->get_convar("weapon_recoil_scale")->get_float();

	for (int i = 1; i <= 64; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity || entity == csgo::local_player || entity->dormant() || !entity->is_alive() || entity->has_gun_game_immunity()
			|| csgo::local_player->is_in_air() && !settings::aimbot::target_in_air)
			continue;

		if (game_type->get_int() == 6) {
			if ((csgo::local_player->survival_team() != -1) && (entity->survival_team() == csgo::local_player->survival_team()))
				continue;

			if (weapon->is_non_aim_weapon())
				continue;
		}
		else if (entity->team() == csgo::local_player->team() && !settings::aimbot::target_team)
			continue;

		for (const auto bone : bones) {
			auto bone_position = entity->get_bone_position(bone);

			if ((!csgo::local_player->can_see_player_pos(entity, bone_position) && settings::aimbot::visible_only)
				|| utilities::goes_through_smoke(local_eye_position, bone_position) && !settings::aimbot::target_smoke)
				continue;

			auto angle = math::calculate_angle2(local_eye_position, bone_position, cmd->viewangles).clamped();
			angle -= (csgo::local_player->aim_punch_angle() * (weapon_recoil_scale * settings.rcs));

			auto fov = std::hypotf(angle.x, angle.y);
			if (fov < best_fov) {

				best_target = bone_position;
				best_fov = fov;
			}
		}
	}

	if (best_target.zero())
		return;

	if (!(csgo::local_player->flags() & fl_onground)) return;

	if (!ctx.autostop) return;

	if (weapon->next_primary_attack() > csgo::server_time()) return;

	cmd->forwardmove = 0.f;
	cmd->sidemove = 0.f;

	auto local = csgo::local_player;

	vec3_t velocity = local->velocity();

	if (velocity.length_2d() > 20.0f) {
		vec3_t direction;
		vec3_t real_view;

		math::vector_angles(velocity, direction);
		interfaces::engine->get_view_angles(real_view);

		direction.y = real_view.y - direction.y;

		vec3_t forward;
		math::angle_vectors(direction, forward);

		static auto cl_forwardspeed = interfaces::console->get_convar(xorstr_("cl_forwardspeed"));
		static auto cl_sidespeed = interfaces::console->get_convar(xorstr_("cl_sidespeed"));

		auto negative_forward_speed = -cl_forwardspeed->float_value;
		auto negative_side_speed = -cl_sidespeed->float_value;

		auto negative_forward_direction = forward * negative_forward_speed;
		auto negative_side_direction = forward * negative_side_speed;

		cmd->forwardmove = negative_forward_direction.x;
		cmd->sidemove = negative_side_direction.y;
	}

}