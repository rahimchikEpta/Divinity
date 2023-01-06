#include "../features.hpp"
#include "../../menu/config/config.hpp"


void visuals::disable_panorama_blur () {
	static auto panorama_disable_blur = interfaces::console->get_convar ( "@panorama_disable_blur" );
	panorama_disable_blur->set_value ( settings::visuals::visuals_disable_panorama_blur );
}

int random_int ( int min, int max ) {
	typedef int( *RandomInt_t )( int, int );
	static RandomInt_t m_RandomInt = (RandomInt_t) GetProcAddress ( GetModuleHandle (  ( "vstdlib.dll" ) ),  ( "RandomInt" ) );
	return m_RandomInt ( min, max );
}

void visuals::do_precipitation ( int stage ) {
	//static c_client_class* c_class = nullptr;

	//if (!c_class )
	//	c_class = interfaces::client->get_client_classes();

	//while ( c_class ) {
	//	if ( c_class->class_id == cprecipitation ) // 138 - CPrecipitation
	//		break;

	//	c_class = c_class->next_ptr;
	//}

	//if (!c_class)
	//	return;

	//auto entry = interfaces::entity_list->get_highest_index();
	//auto serial = random_int(0, 4096);

	//csgo::networkable = c_class->create_fn(entry, serial);

	//if (!csgo::networkable)
	//	return;

	//auto m_parcipitation = csgo::networkable->GetIClientUnknown()->GetBaseEntity();

}

void draw_beam ( bool local_tracer, const vec3_t& src, const vec3_t& end ) {

	BeamInfo_t beam_info;
	beam_info.m_vecStart = src;


	beam_info.m_vecStart.z -= 2.0f;
	beam_info.m_vecEnd = end;
	beam_info.m_nType = TE_BEAMPOINTS;
	beam_info.m_pszModelName = "sprites/physbeam.vmt";
	beam_info.m_nModelIndex = -1;
	beam_info.m_flHaloScale = 0.0f;
	beam_info.m_flLife = 4.0f;
	beam_info.m_flWidth = 2.0f;
	beam_info.m_flEndWidth = 2.0f;
	beam_info.m_flFadeLength = 0.0f;
	beam_info.m_flAmplitude = 2.0f;
	beam_info.m_flBrightness = (float) settings::visuals::bullet_tracer_col[ 3 ] * 255;
	beam_info.m_flSpeed = 0.2f;
	beam_info.m_nStartFrame = 0;
	beam_info.m_flFrameRate = 0.0f;
	beam_info.m_flRed = (float) settings::visuals::bullet_tracer_col[ 0 ] * 255;
	beam_info.m_flGreen = (float) settings::visuals::bullet_tracer_col[ 1 ] * 255;
	beam_info.m_flBlue = (float) settings::visuals::bullet_tracer_col[ 2 ] * 255;
	beam_info.m_nSegments = 2;
	beam_info.m_bRenderable = true;
	beam_info.m_nFlags = FBEAM_SHADEIN | FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;



	auto beam = interfaces::render_beams->create_beam_point ( beam_info );

	if ( beam )
		interfaces::render_beams->draw_beam ( beam );
}

void visuals::beam_events ( i_game_event* event ) {
	auto event_name = fnv::hash ( event->get_name () );
	switch ( event_name ) {
		case fnv::hash ( "bullet_impact" ):
			if ( settings::visuals::bullet_tracers ) {
				auto user_id = event->get_int ( "userid" );
				auto attacker = interfaces::entity_list->get_client_entity ( interfaces::engine->get_player_for_user_id ( event->get_int ( "attacker" ) ) );




				if ( attacker == csgo::local_player ) {
					auto new_record = true;
					vec3_t position ( event->get_float ( ( "x" ) ), event->get_float ( ( "y" ) ), event->get_float ( ( "z" ) ) );
					last_shot_pos = csgo::local_player->get_eye_pos ();
					for ( auto& current : impacts ) {

						new_record = false;

						current.impact_position = position;
						current.time = interfaces::globals->cur_time;

					}
					if ( new_record )
						impacts.push_back ( impact_data{ position, interfaces::globals->cur_time } );
				}
			}


			break;
	}

}
#define ti            ( interfaces::globals->interval_per_tick )
#define ttt( t )    ( (int)( 0.5f + (float)( t ) / ti ) )
void visuals::draw_beams () {
	if ( impacts.empty () )
		return;

	while ( !impacts.empty () ) {
		if ( impacts.begin ()->impact_position.zero () ) //-V807
		{
			impacts.erase ( impacts.begin () );
			continue;
		}

		if ( fabs ( interfaces::globals->cur_time - impacts.begin ()->time ) > 4.0f ) //-V807
		{
			impacts.erase ( impacts.begin () );
			continue;
		}

		if ( !csgo::local_player->is_alive () ) {
			impacts.erase ( impacts.begin () );
			continue;
		}

		if ( ttt ( interfaces::globals->cur_time ) > ttt ( impacts.begin ()->time ) ) {


			if ( !settings::visuals::bullet_tracers ) {
				impacts.erase ( impacts.begin () );
				continue;
			}


			//Vector origin;
		//	Vector dest;


			draw_beam ( true, last_shot_pos, impacts.begin ()->impact_position );
			impacts.erase ( impacts.begin () );
			continue;
		}

		break;
	}
}

void visuals::fov_circle ( int fov ) {
	if ( !settings::visuals::fov_circle )
		return;

	//ImGui::GetBackgroundDrawList ()->AddCircle ( ImVec2 ( ImGui::GetIO ().DisplaySize.x / 2, ImGui::GetIO ().DisplaySize.y / 2 ), fov * 2, ImColor ( settings::visuals::fov_circle_clr[ 0 ], settings::visuals::fov_circle_clr[ 1 ], settings::visuals::fov_circle_clr[ 2 ], settings::visuals::fov_circle_clr[ 3 ] ), 360, 1.f );
}

void visuals::skybox_changer () {
	static convar* skybox = interfaces::console->get_convar ( "sv_skyname" );
	static convar* sky3d = interfaces::console->get_convar ( "r_3dsky" );

	switch ( settings::visuals::skybox_changer_index ) {
		case 0: //Baggage
			skybox->set_value ( "cs_baggage_skybox" );
			break;
		case 1: //Tibet
			skybox->set_value ( "cs_tibet" );
			break;
		case 2: //Clear Sky
			skybox->set_value ( "clearsky" );
			break;
		case 3: //Clear Sky HD
			skybox->set_value ( "clearsky_hdr" );
			break;
		case 4: //Embassy
			skybox->set_value ( "embassy" );
			break;
		case 5: //Italy
			skybox->set_value ( "italy" );
			break;
		case 6: //Daylight 1
			skybox->set_value ( "sky_cs15_daylight01_hdr" );
			break;
		case 7: //Daylight 2
			skybox->set_value ( "sky_cs15_daylight02_hdr" );
			break;
		case 8: //Daylight 3
			skybox->set_value ( "sky_cs15_daylight03_hdr" );
			break;
		case 9: //Daylight 4
			skybox->set_value ( "sky_cs15_daylight04_hdr" );
			break;
		case 10: //Cloudy
			skybox->set_value ( "sky_csgo_cloudy01" );
			break;
		case 11: //Night 1
			skybox->set_value ( "sky_csgo_night02" );
			break;
		case 12: //Night 2
			skybox->set_value ( "sky_csgo_night02b" );
			break;
		case 13: //Night Flat
			skybox->set_value ( "sky_csgo_night_flat" );
			break;
		case 14: //Day HD
			skybox->set_value ( "sky_day02_05_hdr" );
			break;
		case 15: //Day
			skybox->set_value ( "sky_day02_05" );
			break;
		case 16: //Rural
			skybox->set_value ( "sky_l4d_rural02_ldr" );
			break;
		case 17: //Vertigo HD
			skybox->set_value ( "vertigo_hdr" );
			break;
		case 18: //Vertigo Blue HD
			skybox->set_value ( "vertigoblue_hdr" );
			break;
		case 19: //Vertigo
			skybox->set_value ( "vertigo" );
			break;
		case 20: //Vietnam
			skybox->set_value ( "vietnam" );
			break;
		case 21: //Dusty Sky
			skybox->set_value ( "sky_dust" );
			break;
		case 22: //Jungle
			skybox->set_value ( "jungle" );
			break;
		case 23: //Nuke
			skybox->set_value ( "nukeblank" );
			break;
		case 24: //Office
			skybox->set_value ( "office" );
			break;
		case 25:
			skybox->set_value ( "sky_lunacy" );
			break;
		default:
			break;
	}
}

void visuals::fog_modulation () {
	static auto fog_mod = interfaces::console->get_convar ( "fog_override" );
	fog_mod->set_value ( settings::visuals::fog );

	if ( !settings::visuals::fog )
		return;

	static auto fog_start = interfaces::console->get_convar ( "fog_start" );
	fog_start->set_value ( settings::visuals::fog_start );

	static auto fog_end = interfaces::console->get_convar ( "fog_end" );
	fog_end->set_value ( settings::visuals::fog_end );

	static auto fog_maxdensity = interfaces::console->get_convar ( "fog_maxdensity" );

	if ( fog_maxdensity->get_float () != (float) settings::visuals::fog_density * 0.01f )
		fog_maxdensity->set_value ( (float) settings::visuals::fog_density * 0.01f );

	color t = color::from_float ( settings::visuals::fog_clr );
	char buffer_color[ 12 ];
	sprintf_s ( buffer_color, 12, "%i %i %i", t.r, t.g, t.b );

	convar* fog_color = interfaces::console->get_convar ( "fog_color" );
	if ( strcmp ( fog_color->string, buffer_color ) )
		fog_color->set_value ( buffer_color );
}

void visuals::engine_based_force_crosshair() {
	if (!interfaces::engine->is_connected() || !interfaces::engine->is_in_game())
		return;

	if (!csgo::local_player->is_alive())
		return;

	static auto crosshair = interfaces::console->get_convar("weapon_debug_spread_show");
	if (settings::visuals::force_crosshair)
		crosshair->set_value(3);
	else
		crosshair->set_value(0);

	if (csgo::local_player->is_scoped())
		crosshair->set_value(0);
}