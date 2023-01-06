#include "../features.hpp"
#include "../../menu/config/config.hpp"

enum nade_throw_act {
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

void visuals::nade_pred::predict(c_usercmd* ucmd) {
	//	readability
	constexpr float restitution = 0.45f;
	constexpr float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	constexpr float velocity = 403.0f * 0.9f;

	float step, gravity, new_velocity, unk01;
	int index{}, grenade_act{ 1 };
	vec3_t pos, thrown_direction, start, eye_origin;
	vec3_t angles, thrown;

	//	first time setup
	static auto sv_gravity = interfaces::console->get_convar("sv_gravity");

	//	calculate step and actual gravity value
	gravity = sv_gravity->get_float() / 8.0f;
	step = interfaces::globals->interval_per_tick;

	//	get local view and eye origin
	eye_origin = csgo::local_player->get_eye_pos();
	angles = ucmd->viewangles;

	//	copy current angles and normalise pitch
	thrown = angles;

	if (thrown.x < 0) {
		thrown.x = -10 + thrown.x * ((90 - 10) / 90.0f);
	}
	else {
		thrown.x = -10 + thrown.x * ((90 + 10) / 90.0f);
	}

	//	find out how we're throwing the grenade
	auto primary_attack = ucmd->buttons & in_attack;
	auto secondary_attack = ucmd->buttons & in_attack2;

	if (primary_attack && secondary_attack) {
		grenade_act = ACT_LOB;
	}
	else if (secondary_attack) {
		grenade_act = ACT_DROP;
	}

	//	apply 'magic' and modulate by velocity
	unk01 = power[grenade_act];

	unk01 = unk01 * 0.7f;
	unk01 = unk01 + 0.3f;

	new_velocity = velocity * unk01;

	//	here's where the fun begins
	math::angle_vectors(thrown, thrown_direction);

	start = eye_origin + thrown_direction * 16.0f;
	thrown_direction = (thrown_direction * new_velocity) + csgo::local_player->velocity();

	//	let's go ahead and predict
	for (auto time = 0.0f; index < 500; time += step) {
		pos = start + thrown_direction * step;

		//	setup trace
		trace_t trace;
		trace_filter_skip_one_entity filter(csgo::local_player);

		interfaces::trace_ray->trace_ray(ray_t{ start, pos }, MASK_SOLID, &filter, &trace);

		//	modify path if we have hit something
		if (trace.flFraction != 1.0f) {
			thrown_direction = trace.plane.normal * -2.0f * thrown_direction.dot(trace.plane.normal) + thrown_direction;

			thrown_direction *= restitution;

			pos = start + thrown_direction * trace.flFraction * step;

			time += (step * (1.0f - trace.flFraction));
		}

		//	check for detonation
		auto detonate = detonated(csgo::local_player->active_weapon(), time, trace);

		//	emplace nade point
		_points.at(index++) = c_nadepoint(start, pos, trace.flFraction != 1.0f, true, trace.plane.normal, detonate);
		start = pos;

		//	apply gravity modifier
		thrown_direction.z -= gravity * trace.flFraction * step;

		if (detonate) {
			break;
		}
	}

	//	invalidate all empty points and finish prediction
	for (auto n = index; n < 500; ++n) {
		_points.at(n).m_valid = false;
	}

	_predicted = true;
}

bool visuals::nade_pred::detonated(weapon_t* weapon, float time, trace_t& trace) {
	if (!weapon) {
		return true;
	}

	//	get weapon item index
	const auto index = weapon->item_definition_index();

	switch (index) {
		//	flash and HE grenades only live up to 2.5s after thrown
	case 43:
	case 44:
		if (time > 2.5f) {
			return true;
		}
		break;

		//	fire grenades detonate on ground hit, or 3.5s after thrown
	case WEAPON_MOLOTOV:
	case 48:
		if (trace.flFraction != 1.0f && trace.plane.normal.z > 0.7f || time > 3.5f) {
			return true;
		}
		break;

		//	decoy and smoke grenades were buggy in prediction, so i used this ghetto work-around
	case WEAPON_DECOY:
	case 45:
		if (time > 2.5f) {
			return true;
		}
		break;
	}

	return false;
}

void visuals::nade_pred::trace(c_usercmd* ucmd) {
	if (!settings::visuals::grenade_prediction)
		return;

	if (!(ucmd->buttons & in_attack) && !(ucmd->buttons & in_attack2)) {
		_predicted = false;
		return;
	}

	const static std::vector< int > nades{
		WEAPON_FLASHBANG,
		WEAPON_SMOKEGRENADE,
		WEAPON_HEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE
	};

	//	grab local weapon
	auto weapon = csgo::local_player->active_weapon();

	if (!weapon) {
		return;
	}

	if (std::find(nades.begin(), nades.end(), weapon->item_definition_index()) != nades.end()) {
		return predict(ucmd);
	}

	_predicted = false;
}

void visuals::nade_pred::draw() {
	if (!settings::visuals::grenade_prediction)
		return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (!render::device)
		return;

	auto draw_3d_dotted_circle = [](vec3_t position, float points, float radius) {
		float step = 3.141592654f * 2.0f / points;
		for (float a = 0; a < 3.141592654f * 2.0f; a += step) {
			vec3_t start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);

			vec3_t start2d;
			if (math::world_to_screen(start, start2d))
				render::line(vec2_t(start2d.x, start2d.y), vec2_t(start2d.x + 1, start2d.y + 1), color::from_float(settings::visuals::grenade_prediction_clr));
		}
	};

	vec3_t start, end;

	//	draw nade path
	if (_predicted) {
		for (auto& p : _points) {
			if (!p.m_valid) {
				break;
			}

			if (math::world_to_screen(p.m_start, start) && math::world_to_screen(p.m_end, end)) {

				//	draw line
				render::line(vec2_t(start.x, start.y), vec2_t(end.x, end.y), color::from_float(settings::visuals::grenade_prediction_clr));

				if (p.m_plane && !p.m_detonate)
					render::draw_3d_cube(3.5f, vec3_t(p.m_start.x, p.m_start.y, 0), p.m_start, color::from_float(settings::visuals::grenade_prediction_clr2));

				if (p.m_detonate)
					render::draw_circle_3d(p.m_end, 100, color::from_float(settings::visuals::grenade_prediction_clr2));
			}
		}
	}
}


