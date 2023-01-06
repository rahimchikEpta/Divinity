#include "listener.h"
#include "../../features/features.hpp"

event_manager event_listener;

void event_manager::fire_game_event ( i_game_event* event ) {
	misc::hitmarker_event ( event );
	legit_bot::event ( event );
	visuals::beam_events ( event );
	misc::main::chicken_killsay ( event );
	misc::main::round_stats ( event );
	misc::main::vote_revealer::on_event ( event );
	misc::main::hitlogs ( event );
}

bool event_manager::initialize () {
	debug_id = EVENT_DEBUG_ID_INIT;
	interfaces::event_manager->add_listener ( this, ( "player_hurt" ), false );;
	interfaces::event_manager->add_listener ( this, ( "player_death" ), false );
	interfaces::event_manager->add_listener ( this, ( "round_start" ), false );
	interfaces::event_manager->add_listener ( this, ( "item_purchase" ), false );
	interfaces::event_manager->add_listener ( this, ( "other_death" ), false );
	interfaces::event_manager->add_listener ( this, ( "round_end" ), false );
	interfaces::event_manager->add_listener ( this, ( "vote_cast" ), false );

	if ( !interfaces::event_manager->find_listener ( this, ( "player_hurt" ) ) || !interfaces::event_manager->find_listener ( this, ( "player_death" ) ) ||
		!interfaces::event_manager->find_listener ( this, ( "round_start" ) ) || !interfaces::event_manager->find_listener ( this, ( "item_purchase" ) ) ||
			!interfaces::event_manager->find_listener ( this, ( "other_death" ) ) || !interfaces::event_manager->find_listener ( this, ( "round_end" ) ) ||
		!interfaces::event_manager->find_listener ( this, ( "vote_cast" ) ) ) {
		throw std::runtime_error ( ( "failed to initialize events. (add_listener?)" ) );
		return false;
	}
#ifdef _DEBUG
	console::log ( ( "[setup] event listener intialized! \n" ) );
#endif
	return true;
	}

bool event_manager::release () {
	interfaces::event_manager->remove_listener ( this );

	return true;
}