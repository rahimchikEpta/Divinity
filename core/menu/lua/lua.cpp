///
/// If this source ever gets leaked GL trying to fix the external errors lmfao
/// 

#ifdef _DEBUG
#include <fstream>
#include <ShlObj.h>
#include  <iostream>
#include "../../features/utils/notifications.h"
#include "lua.h"
#else
#include <fstream>
#include <ShlObj.h>
#include  <iostream>
#include "../../features/utils/notifications.h"
#include "lua.h"
#endif // DEBUG
#include "../../xor.h"

namespace ns_client {
	void set_event_callback ( sol::this_state s, std::string eventname, sol::function func ) {
		sol::state_view lua_state ( s );
		sol::table rs = lua_state[ ("debug") ][ ("getinfo") ] ( 2, ( "S" ) );
		std::string source = rs[ ("source") ];
		std::string filename = std::filesystem::path ( source.substr ( 1 ) ).filename ().string ();


		lua::hook_manager::register_hook ( eventname, lua::get_script_id ( filename ), func );

		interfaces::console->console_color_printf ( color::from_float ( c::menu_accent ), ( "[lua] " ) );
		interfaces::console->console_printf ( "%s: indexed %s \n", filename.c_str (), eventname.c_str () );
	}

	void log ( std::string l ) {
		notifications::notify ( l );
	}

	bool edgebugged () {
		return misc::movement::in_edgebug;
	}

	bool in_edgebug () {
		return misc::movement::should_eb;
	}

	int edgebugs () {
		return misc::movement::amount_of_edgebugs;
	}

	int jumpshots () {
		return misc::movement::amount_of_jumpshots;
	}
};

namespace ns_local {
	int healthboost_effect () {
		return csgo::local_player->health_boost_time ();
	}
	bool is_alive () {
		return csgo::local_player->is_alive ();
	}
	int index () {
		return csgo::local_player->index ();
	}
	vec3_t origin () {
		return csgo::local_player->origin ();
	}
}
namespace ns_cvar { //dziala
	convar* find_var ( std::string name ) {
		return interfaces::console->get_convar ( name.c_str () );
	}
	void console_color_printf ( color color, std::string v ) { //dziala
		interfaces::console->console_color_printf ( color, v.c_str () );
	}
	void console_printf ( std::string v ) { //dziala
		interfaces::console->console_printf ( v.c_str () );
	}
};

namespace ns_convar {
	int get_int ( convar* c_convar ) { //dziala
		return c_convar->get_int ();
	}

	float get_float ( convar* c_convar ) {//dziala
		return c_convar->get_float ();
	}

	void set_int ( convar* c_convar, int value ) { //dziala
		c_convar->set_value ( value );
	}

	void set_float ( convar* c_convar, float value ) { //dziala
		c_convar->set_value ( value );
	}

	void set_char ( convar* c_convar, std::string value ) { //dziala
		c_convar->set_value ( value.c_str () );
	}
};

namespace ns_engine {
	void execute_cmd ( std::string cmd ) {
		interfaces::engine->execute_cmd ( cmd.c_str () );
	}

	player_info_t get_player_info ( int ent ) {
		player_info_t p;
		interfaces::engine->get_player_info ( ent, &p );
		return p;
	}

	int get_player_for_user_id ( int userid ) {
		return interfaces::engine->get_player_for_user_id ( userid );
	}

	int get_local_player_index () {
		return interfaces::engine->get_local_player ();
	}

	vec3_t get_view_angles () {
		vec3_t va;
		interfaces::engine->get_view_angles ( va );
		return va;
	}

	void set_view_angles ( vec3_t va ) {
		interfaces::engine->set_view_angles ( va );
	}

	int get_max_clients () { //dziala
		return 64;
	}

	bool is_in_game () { //dziala
		return interfaces::engine->is_in_game ();
	}

	bool is_connected () { //dziala
		return interfaces::engine->is_connected ();
	}
};

namespace ns_entity {
	bool dormant ( player_t* entity ) {
		return entity->dormant ();
	}
	bool is_alive ( player_t* entity ) {
		return entity->is_alive ();
	}
	vec3_t get_bone_position ( player_t* entity, int bone ) {
		return entity->get_bone_position ( bone );
	}
	vec3_t get_eye_position ( player_t* entity ) {
		return entity->get_eye_pos ();
	}
	bool can_see_player_pos ( player_t* entity, vec3_t pos ) {
		return entity->can_see_player_pos ( entity, pos );
	}
	bool is_weapon ( player_t* entity ) {
		return entity->is_weapon ();
	}
	bool is_player ( player_t* entity ) {
		return entity->is_player ();
	}
	entity_t* get_active_weapon ( player_t* entity ) {
		return entity->active_weapon ();
	}
	vec3_t get_abs_origin ( player_t* entity ) {
		return entity->abs_origin ();
	}

	vec3_t get_origin ( player_t* entity ) {
		return entity->origin ();
	}

	anim_state* get_animstate ( player_t* entity ) {
		return entity->get_anim_state ();
	}
	vec3_t get_aim_punch ( player_t* entity ) {
		return entity->aim_punch_angle ();
	}

	int get_health ( player_t* entity ) {
		return entity->health ();
	}

	int index ( player_t* entity ) {
		return entity->index ();
	}

	bool spotted ( player_t* entity, bool data ) {
		return entity->spotted () = data;
	}
};

namespace ns_math {
	bool world_to_screen ( const vec3_t& origin, vec3_t& screen ) {
		return math::world_to_screen ( origin, screen );
	}
}

namespace ns_entity_list {
	player_t* get_entity ( int index ) {
		return reinterpret_cast<player_t*>( interfaces::entity_list->get_client_entity ( index ) );
	}
	void* get_entity_from_handle ( int handle ) {
		return interfaces::entity_list->get_client_entity_handle ( handle );
	}
	int get_highest_entity_index () {
		return interfaces::entity_list->get_highest_index ();
	}
};

namespace ns_surface {
	void draw_line ( float x0, float y0, float x1, float y1 ) {
		interfaces::surface->draw_line ( x0, y0, x1, y1 );
	}
	void set_draw_color ( int r, int g, int b, int a ) {
		interfaces::surface->set_drawing_color ( r, g, b, a );
	}
	void draw_filled_rect ( float x0, float y0, float x1, float y1 ) {
		interfaces::surface->draw_filled_rectangle ( x0, y0, x1, y1 );
	}
	void draw_filled_rect_fade ( int x, int y, int x2, int y2, int alpha, int alpha2, bool horizontal ) {
		interfaces::surface->draw_filled_rect_fade ( x, y, x2, y2, alpha, alpha2, horizontal );
	}
	void draw_outlined_rect ( float x0, float y0, float x1, float y1 ) {
		interfaces::surface->draw_outlined_rect ( x0, y0, x1, y1 );
	}
	void draw_outlined_circle ( float x, float y, int r, int seg ) {
		interfaces::surface->draw_outlined_circle ( x, y, r, seg );
	}
	int create_font ( std::string fontname, int w, int h, int blur, int flags ) {
		auto f = interfaces::surface->font_create ();
		interfaces::surface->set_font_glyph ( f, fontname.c_str (), w, h, blur, 0, flags );
		return f;
	}
	void set_text_font ( int font ) {
		interfaces::surface->draw_text_font ( font );
	}
	void set_text_color ( int r, int g, int b, int a ) {
		interfaces::surface->set_text_color ( r, g, b, a );
	}
	void set_text_pos ( int x, int y ) {
		interfaces::surface->draw_text_pos ( x, y );
	}
	void draw_text ( std::wstring str ) {
		interfaces::surface->draw_render_text ( str.c_str (), str.length () );
	}

	vec2_t screen_size () {
		int w, h;
		interfaces::surface->get_screen_size ( w, h );
		return vec2_t ( w, h );
	}
};

namespace ns_ui {
	bool new_checkbox ( std::string label, bool* key ) {
		return key;
	}
};

namespace ns_menuelement {
	void checkbox ( std::string label, bool* v ) {
		c::checkbox ( label, v );
	}
}

namespace ns_globals {
	float absolute_frametime () {
		return interfaces::globals->absolute_frametime;
	}

	float cur_time () {
		return interfaces::globals->cur_time;
	}

	int frame_count () {
		return interfaces::globals->frame_count;
	}

	float frame_time () {
		return interfaces::globals->frame_time;
	}

	float interval_per_tick () {
		return interfaces::globals->interval_per_tick;
	}

	float realtime () {
		return interfaces::globals->realtime;
	}

	int tick_count () {
		return interfaces::globals->tick_count;
	}
}

namespace ns_utilities {
	/*void apply_clan_tag ( std::string clan ) {
		utilities::apply_clan_tag ( clan.c_str () );
	}

	void apply_name ( std::string clan ) {
		utilities::apply_name ( clan.c_str () );
	}*/

	color create_color ( int r, int g, int b, int a ) {
		return color ( r, g, b, a );
	}
	color menu_color () {
		return color::from_float ( c::menu_accent );
	}
}

namespace ns_render {
	void rect ( int x, int y, int w, int h, color color ) {
		render::rectangle (vec2_t( x, y), vec2_t(w, h), color);
	}

	void rect_filled ( int x, int y, int w, int h, color color ) {
		render::filled_rectangle ( vec2_t(x, y), vec2_t(w, h), color );
	}

	void line ( int x1, int y1, int x2, int y2, color color ) {
		render::line ( vec2_t(x1, y1), vec2_t(x2, y2), color );
	}

	void text ( int x, int y, ID3DXFont* font, std::string string, bool text_centered, color color ) {
		render::text(vec2_t(x, y), color, string, font, text_centered);
	}

	vec2_t text_size (ID3DXFont* font, std::string text ) {
		return render::text_position (text, font);
	}

	ID3DXFont* create_font(std::string fontname, int size, int weight, bool anti_aliased) {
		ID3DXFont* f;
		D3DXCreateFontA(render::device, size, 0, weight, 1, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, anti_aliased ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY, DEFAULT_PITCH, fontname.c_str(), &f);
		return f;
	}
}


//};

void lua::init_state () {
	lua::unload ();
	g_lua_state = luaL_newstate ();
	luaL_openlibs ( g_lua_state );
}

void  lua::init_command () {
	sol::state_view lua_state ( g_lua_state );
	lua_state[  ( "exit" ) ] = [ ]() { g_unload_flag = true; };

	lua_state.new_enum ( "BUTTONS",
		 ( "IN_ATTACK" ), cmd_buttons::in_attack,
		 ( "IN_JUMP" ), cmd_buttons::in_jump,
		 ( "IN_DUCK" ), cmd_buttons::in_duck,
		 ( "IN_FORWARD" ), cmd_buttons::in_forward,
		 ( "IN_BACK" ), cmd_buttons::in_back,
		 ( "IN_USE" ), cmd_buttons::in_use,
		 ( "IN_MOVELEFT" ), cmd_buttons::in_moveleft,
		 ( "IN_MOVERIGHT" ), cmd_buttons::in_moveright,
		 ( "IN_ATTACK2" ), cmd_buttons::in_attack2,
		 ( "IN_SCORE" ), cmd_buttons::in_score,
		 ( "IN_BULLRUSH" ), cmd_buttons::in_bullrush );

	lua_state.new_usertype<vec2_t> (  ( "vec2" ),
		 ( "x" ), &vec2_t::x,
		 ( "y" ), &vec2_t::y
		);

	lua_state.new_usertype<vec3_t> (  ( "vec3" ),
		 ( "x" ), &vec3_t::x,
		 ( "y" ), &vec3_t::y,
		 ( "z" ), &vec3_t::z
		);

	lua_state.new_usertype<player_info_t> ( "player_info",
		 ( "name" ), sol::readonly ( &player_info_t::name ),
		 ( "fakeplayer" ), sol::readonly ( &player_info_t::fakeplayer ),
		 ( "userid" ), sol::readonly ( &player_info_t::userid )

		);

	lua_state.new_usertype<color> (  ( "color" ),
		sol::constructors<color (), color ( int, int, int ), color ( int, int, int, int )> (),
		 ( "r" ), &color::r,
		 ( "g" ), &color::g,
		 ( "b" ), &color::b,
		 ( "a" ), &color::a
		);

	lua_state.new_usertype<c_usercmd> ( "c_usercmd",
		 ( "command_number" ), sol::readonly ( &c_usercmd::command_number ),
		 ( "tick_count" ), sol::readonly ( &c_usercmd::tick_count ),
		 ( "viewangles" ), &c_usercmd::viewangles,
		 ( "aimdirection" ), &c_usercmd::aimdirection,
		 ( "forwardmove" ), &c_usercmd::forwardmove,
		 ( "sidemove" ), &c_usercmd::sidemove,
		 ( "upmove" ), &c_usercmd::upmove,
		 ( "buttons" ), &c_usercmd::buttons,
		 ( "impulse" ), sol::readonly ( &c_usercmd::impulse ),
		 ( "weaponselect" ), &c_usercmd::weaponselect,
		 ( "weaponsubtype" ), sol::readonly ( &c_usercmd::weaponsubtype ),
		 ( "random_seed" ), sol::readonly ( &c_usercmd::randomseed ),
		 ( "mousedx" ), &c_usercmd::mousedx,
		 ( "mousedy" ), &c_usercmd::mousedy
		);

	auto client = lua_state.create_table ();
	client[  ( "callback" ) ] = ns_client::set_event_callback;
	client[  ( "log" ) ] = ns_client::log;
	client[  ( "edgebugged" ) ] = ns_client::edgebugged;
	client[  ( "edgebugs" ) ] = ns_client::edgebugs;
	client[  ( "in_edgebug" ) ] = ns_client::in_edgebug;
	client[  ( "jumpshots" ) ] = ns_client::jumpshots;
	lua_state[  ( "divinity" ) ] = client;

	auto local = lua_state.create_table ();
	local[  ( "healthboost_effect") ] = ns_local::healthboost_effect;
	local[  ( "is_alive") ] = ns_local::is_alive;
	local[  ( "index") ] = ns_local::index;
	local[  ( "origin") ] = ns_local::origin;
	lua_state[ ("local_player") ] = local;

	auto engine = lua_state.create_table ();
	engine[ ("execute_cmd") ] = ns_engine::execute_cmd;
	engine[ ("get_max_clients") ] = ns_engine::get_max_clients;
	engine[ ("get_player_for_user_id") ] = ns_engine::get_player_for_user_id;
	engine[ ("get_player_info") ] = ns_engine::get_player_info;
	engine[ ("get_view_angles") ] = ns_engine::get_view_angles;
	engine[ ("is_connected") ] = ns_engine::is_connected;
	engine[ ("is_in_game") ] = ns_engine::is_in_game;
	engine[ ("set_view_angles") ] = ns_engine::set_view_angles;
	lua_state[ ("engine") ] = engine;


	auto entity = lua_state.create_table ();
	entity[ ("get_bone_position") ] = ns_entity::get_bone_position;
	entity[ ("get_eye_position") ] = ns_entity::get_eye_position;
	entity[ ("can_see_player_pos") ] = ns_entity::can_see_player_pos;
	entity[ ("dormant") ] = ns_entity::dormant;
	entity[ ("is_weapon") ] = ns_entity::is_weapon;
	entity[ ("is_alive") ] = ns_entity::is_alive;
	entity[ ("is_player") ] = ns_entity::is_player;
	entity[ ("get_active_weapon") ] = ns_entity::get_active_weapon;
	entity[ ("get_abs_origin") ] = ns_entity::get_abs_origin;
	entity[ ("get_animstate") ] = ns_entity::get_animstate;
	entity[ ("get_aim_punch") ] = ns_entity::get_aim_punch;
	entity[ ("get_health") ] = ns_entity::get_health;
	entity[ ("spotted") ] = ns_entity::spotted;
	entity[ ("get_origin") ] = ns_entity::get_origin;
	entity[ ("index") ] = ns_entity::index;
	lua_state[ ("entity") ] = entity;


	auto entity_list = lua_state.create_table ();
	entity_list[ ("get_entity") ] = ns_entity_list::get_entity;
	entity_list[ ("get_entity_from_handle") ] = ns_entity_list::get_entity_from_handle;
	entity_list[ ("get_highest_entity_index") ] = ns_entity_list::get_highest_entity_index;
	lua_state[ ("entity_list") ] = entity_list;

	auto cvar = lua_state.create_table ();
	cvar[ ("find_var") ] = ns_cvar::find_var;
	cvar[ ("console_printf") ] = ns_cvar::console_printf;
	cvar[ ("console_color_printf") ] = ns_cvar::console_color_printf;
	lua_state[ ("con") ] = cvar;

	auto mathematics = lua_state.create_table ();
	mathematics[ ("w2s") ] = ns_math::world_to_screen;
	lua_state[ ("math") ] = mathematics;

	auto convar = lua_state.create_table ();
	convar[ ("get_int") ] = ns_convar::get_int;
	convar[ ("set_int") ] = ns_convar::set_int;
	convar[ ("get_float") ] = ns_convar::get_float;
	convar[ ("set_float") ] = ns_convar::set_float;
	convar[ ("set_char") ] = ns_convar::set_char;
	lua_state[ ("cvar") ] = convar;


	//auto surface = lua_state.create_table ();
	//surface[ ("draw_line") ] = ns_surface::draw_line;
	//surface[ ("set_draw_color") ] = ns_surface::set_draw_color;
	//surface[ ("draw_filled_rect") ] = ns_surface::draw_filled_rect;
	//surface[ ("draw_filled_rect_fade") ] = ns_surface::draw_filled_rect_fade;
	//surface[ ("draw_outlined_rect") ] = ns_surface::draw_outlined_rect;
	//surface[ ("draw_outlined_circle") ] = ns_surface::draw_outlined_circle;
	//surface[ ("create_font") ] = ns_surface::create_font;
	//surface[ ("set_text_font") ] = ns_surface::set_text_font;
	//surface[ ("set_text_color") ] = ns_surface::set_text_color;
	//surface[ ("set_text_pos") ] = ns_surface::set_text_pos;
	//surface[ ("draw_text") ] = ns_surface::draw_text;
	//surface[ ("screen_size") ] = ns_surface::screen_size;
	//lua_state[ ("surface") ] = surface;

	auto ui = lua_state.create_table ();
	ui[ ("new_checkbox") ] = ns_ui::new_checkbox;
	lua_state[ ("gui") ] = ui;


	auto utilities = lua_state.create_table ();
	utilities[ ("color") ] = ns_utilities::create_color;
	utilities[ ("menu_col") ] = ns_utilities::menu_color;
	lua_state[ ("utils") ] = utilities;

	auto globals = lua_state.create_table ();
	globals[ ("abs_frametime") ] = ns_globals::absolute_frametime;
	globals[ ("curtime") ] = ns_globals::cur_time;
	globals[ ("framecount") ] = ns_globals::frame_count;
	globals[ ("frametime") ] = ns_globals::frame_time;
	globals[ ("interval_per_tick") ] = ns_globals::interval_per_tick;
	globals[ ("realtime") ] = ns_globals::realtime;
	globals[ ("tickcount") ] = ns_globals::tick_count;
	lua_state[ ("globals") ] = globals;

	auto render = lua_state.create_table ();
	render[ ("line") ] = ns_render::line;
	render[ ("filled_rect") ] = ns_render::rect_filled;
	render[ ("rect") ] = ns_render::rect;
	render[ ("text") ] = ns_render::text;
	render[ ("create_font") ] = ns_render::create_font;
	render[ ("text_size") ] = ns_render::text_size;
	render[ ("screen_size") ] = ns_surface::screen_size;
	lua_state[ ("render") ] = render;

	refresh_scripts ();

}

void lua::unload () {
	if ( g_lua_state != NULL ) {
		lua_close ( g_lua_state );
		g_lua_state = NULL;
	}
}

void lua::load_script ( int id ) {
	if ( id == -1 )
		return;

	if ( loaded.at ( id ) )
		return;

	auto path = get_script_path ( id );
	if ( path == ( "" ) )
		return;

	sol::state_view state ( g_lua_state );
	state.script_file ( path, [ ]( lua_State* me, sol::protected_function_result result ) {
		if ( !result.valid () ) {
			sol::error err = result;
			interfaces::console->console_color_printf ( color::from_float ( c::menu_accent ), ( "[lua] " ) );
			interfaces::console->console_printf ( std::string ( err.what () ).append ( " \n" ).c_str () );
			notifications::notify ( err.what () );
		}

		return result;
	} );
	loaded.at ( id ) = true;
}

void lua::unload_script ( int id ) {
	if ( id == -1 )
		return;

	if ( !loaded.at ( id ) )
		return;

	hook_manager::un_register_hook ( id );
	loaded.at ( id ) = false;
}

void lua::reload_all_scripts () {
	for ( auto s : scripts ) {
		if ( loaded.at ( get_script_id ( s ) ) ) {
			unload_script ( get_script_id ( s ) );
			load_script ( get_script_id ( s ) );
		}
	}
}

void lua::unload_all_scripts () {
	for ( auto s : scripts )
		if ( loaded.at ( get_script_id ( s ) ) )
			unload_script ( get_script_id ( s ) );
}

void lua::refresh_scripts () {
	auto oldLoaded = loaded;
	auto oldScripts = scripts;

	loaded.clear ();
	pathes.clear ();
	scripts.clear ();

	for ( auto& entry : std::filesystem::directory_iterator (  xorstr_( "divinity/luas/" ) ) ) {
		if ( entry.path ().extension () == xorstr_ ( ".lua" ) ) {
			auto path = entry.path ();
			auto filename = path.filename ().string ();

			bool didPut = false;
			int oldScriptsSize = 0;
			oldScriptsSize = oldScripts.size ();
			if ( oldScriptsSize < 0 )
				continue;

			for ( int i = 0; i < oldScriptsSize; i++ ) {
				if ( filename == oldScripts.at ( i ) ) {
					loaded.push_back ( oldLoaded.at ( i ) );
					didPut = true;
				}
			}

			if ( !didPut )
				loaded.push_back ( false );

			pathes.push_back ( path );
			scripts.push_back ( filename );
		}
	}
}

int lua::get_script_id ( std::string name ) {
	int script_size = 0;
	script_size = scripts.size ();
	if ( script_size <= 0 )
		return -1;

	for ( int i = 0; i < script_size; i++ ) {
		if ( scripts.at ( i ) == name )
			return i;
	}

	return -1;
}

int lua::get_script_id_by_path ( std::string path ) {
	int path_size = 0;
	path_size = pathes.size ();
	if ( path_size <= 0 )
		return -1;

	for ( int i = 0; i < path_size; i++ ) {
		if ( pathes.at ( i ).string () == path )
			return i;
	}

	return -1;
}

std::string lua::get_script_path ( std::string name ) {
	return get_script_path ( get_script_id ( name ) );
}

std::string lua::get_script_path ( int id ) {
	if ( id == -1 )
		return  "";

	return pathes.at ( id ).string ();
}

void lua::hook_manager::register_hook ( std::string event_name, int script_id, sol::protected_function func ) {
	c_lua_hook hk = { script_id, func };

	hooks[ event_name ].push_back ( hk );
}

void lua::hook_manager::un_register_hook ( int script_id ) {
	for ( auto& ev : hooks ) {
		int pos = 0;

		for ( auto& hk : ev.second ) {
			if ( hk.scriptId == script_id )
				ev.second.erase ( ev.second.begin () + pos );

			pos++;
		}
	}
}

std::vector<c_lua_hook> lua::hook_manager::get_hooks ( std::string event_name ) {
	return hooks[ event_name ];
}