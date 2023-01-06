#include "interfaces.hpp"
#include "../utilities/csgo.hpp"
#define _interface(val, type, module_name, interface_name) val = interfaces::get_interface2<type*>((module_name), (interface_name));
#define _interface_offset(val, type, ptr, index, add, name) val = **reinterpret_cast<type***>((*reinterpret_cast<uintptr_t**>(ptr))[index] + add); if (val) utilities::console::log(("[ debug ] custom interface %s found at 0x%p \n"), name, val);
template <typename T>
static constexpr auto relativeToAbsolute ( uintptr_t address ) noexcept {
	return (T) ( address + 4 + *reinterpret_cast<std::int32_t*>( address ) );
}
struct ModuleInfo {
	void* base;
	std::size_t size;
};
static ModuleInfo getModuleInformation ( const char* name ) noexcept {
	if ( HMODULE handle = GetModuleHandleA ( name ) ) {
		if ( MODULEINFO moduleInfo; GetModuleInformation ( GetCurrentProcess (), handle, &moduleInfo, sizeof ( moduleInfo ) ) )
			return ModuleInfo{ moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage };
	}
	return {};
}
[[nodiscard]] static auto generateBadCharTable ( std::string_view pattern ) noexcept {
	assert ( !pattern.empty () );

	std::array<std::size_t, ( std::numeric_limits<std::uint8_t>::max )( ) + 1> table;

	auto lastWildcard = pattern.rfind ( '?' );
	if ( lastWildcard == std::string_view::npos )
		lastWildcard = 0;

	const auto defaultShift = (std::max) ( std::size_t ( 1 ), pattern.length () - 1 - lastWildcard );
	table.fill ( defaultShift );

	for ( auto i = lastWildcard; i < pattern.length () - 1; ++i )
		table[ static_cast<std::uint8_t>( pattern[ i ] ) ] = pattern.length () - 1 - i;

	return table;
}
template <bool ReportNotFound = true>
static std::uintptr_t findPattern ( ModuleInfo moduleInfo, std::string_view pattern ) noexcept {
	static auto id = 0;
	++id;

	if ( moduleInfo.base && moduleInfo.size ) {
		const auto lastIdx = pattern.length () - 1;
		const auto badCharTable = generateBadCharTable ( pattern );

		auto start = static_cast<const char*>( moduleInfo.base );
		const auto end = start + moduleInfo.size - pattern.length ();

		while ( start <= end ) {
			int i = lastIdx;
			while ( i >= 0 && ( pattern[ i ] == '?' || start[ i ] == pattern[ i ] ) )
				--i;

			if ( i < 0 )
				return reinterpret_cast<std::uintptr_t>( start );

			start += badCharTable[ static_cast<std::uint8_t>( start[ lastIdx ] ) ];
		}
	}

	assert ( false );
#ifdef _WIN32
	if constexpr ( ReportNotFound )
		MessageBoxA ( nullptr, ( "Failed to find pattern #" + std::to_string ( id ) + '!' ).c_str (), "nigger LOL", MB_OK | MB_ICONWARNING );
#endif
	return 0;
}
template <bool ReportNotFound = true>
static std::uintptr_t findPattern ( const char* moduleName, std::string_view pattern ) noexcept {
	return findPattern<ReportNotFound> ( getModuleInformation ( moduleName ), pattern );
}

bool interfaces::initialize () {
	client = get_interface<i_base_client_dll, interface_type::index> ( "client.dll", "VClient018" );
	entity_list = get_interface<i_client_entity_list, interface_type::index> ( "client.dll", "VClientEntityList003" );
	engine = get_interface<iv_engine_client, interface_type::index> ( "engine.dll", "VEngineClient014" );
	panel = get_interface<i_panel, interface_type::index> ( "vgui2.dll", "VGUI_Panel009" );
	surface = get_interface<i_surface, interface_type::index> ( "vguimatsurface.dll", "VGUI_Surface031" );
	material_system = get_interface<i_material_system, interface_type::index> ( "materialsystem.dll", "VMaterialSystem080" );
	model_info = get_interface<iv_model_info, interface_type::index> ( "engine.dll", "VModelInfoClient004" );
	model_render = get_interface<iv_model_render, interface_type::index> ( "engine.dll", "VEngineModel016" );
	render_view = get_interface<i_render_view, interface_type::index> ( "engine.dll", "VEngineRenderView014" );
	engine_sound = get_interface<iv_model_info, interface_type::index> ( "engine.dll", "IEngineSoundClient003" );
	console = get_interface<i_console, interface_type::index> ( "vstdlib.dll", "VEngineCvar007" );
	localize = get_interface<i_localize, interface_type::index> ( "localize.dll", "Localize_001" );
	event_manager = get_interface<i_game_event_manager2, interface_type::index> ( "engine.dll", "GAMEEVENTSMANAGER002" );
	debug_overlay = get_interface<iv_debug_overlay, interface_type::index> ( "engine.dll", "VDebugOverlay004" );
	inputsystem = get_interface<i_inputsytem, interface_type::index> ( "inputsystem.dll", "InputSystemVersion001" );
	trace_ray = get_interface<trace, interface_type::index> ( "engine.dll", "EngineTraceClient004" );
	game_movement = get_interface<player_game_movement, interface_type::index> ( "client.dll", "GameMovement001" );
	prediction = get_interface<IPrediction, interface_type::index> ( "client.dll", "VClientPrediction001" );
	effects_system = get_interface<c_effects, interface_type::index> ( "engine.dll", "VEngineEffects001" );
	engine_effects = get_interface<IVEngineEffects, interface_type::index> ( "engine.dll", "VEngineEffects001" );
	model_cache = get_interface<IMDLCache, interface_type::index> ( "datacache.dll", "MDLCache004" );
	_interface ( surface_props_physics, physics_surface_props, "vphysics.dll", "VPhysicsSurfaceProps001" );
	/*custom interfaces*/
	clientmode = **reinterpret_cast<i_client_mode***>( ( *reinterpret_cast<uintptr_t**>( client ) )[ 10 ] + 5 );
	globals = **reinterpret_cast<c_global_vars_base***>( ( *reinterpret_cast<uintptr_t**>( client ) )[ 11 ] + 10 );

	clientstate = **reinterpret_cast<i_client_state***>( utilities::pattern_scan ( "engine.dll", sig_client_state ) + 1 );
	directx = **reinterpret_cast<IDirect3DDevice9***>( utilities::pattern_scan ( "shaderapidx9.dll", sig_directx ) + 1 );
	input = *reinterpret_cast<i_input**>( utilities::pattern_scan ( "client.dll", sig_input ) + 1 );
	glow_manager = reinterpret_cast<glow_manager_t*>( *reinterpret_cast<uintptr_t*>( utilities::pattern_scan ( "client.dll", sig_glow_manager ) + 3 ) );
	move_helper = **reinterpret_cast<IMoveHelper***>( utilities::pattern_scan ( "client.dll", sig_player_move_helper ) + 2 );
	weapon_system = *reinterpret_cast<i_weapon_system**>( utilities::pattern_scan ( "client.dll", sig_weapon_data ) + 2 );
	chat_element = *reinterpret_cast<c_hud_chat**>( reinterpret_cast<uintptr_t>( clientmode ) + *reinterpret_cast<uint8_t*>( utilities::pattern_scan ( ( "client.dll" ), ( "E8 ? ? ? ? 8B 4F ? 85 C9 74 06 51" ) ) + 7 ) );
	render_beams = *reinterpret_cast<i_view_render_beams**>( ( utilities::pattern_scan ( "client.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9" ) + 1 ) );
	view_render = **reinterpret_cast<ViewRender***>(findPattern("client.dll", "\x8B\x0D????\xFF\x75\x0C\x8B\x45\x08") + 2);
	drawScreenEffectMaterial = relativeToAbsolute<uintptr_t>(findPattern("client.dll", "\xE8????\x83\xC4\x0C\x8D\x4D\xF8") + 1);
	effects = get_interface<i_effects, interface_type::index> ( "client.dll", "IEffects001" );
	frame_render_start = utilities::pattern_scan(("client.dll"), ("FF 50 40 8B 1D")) + 0x3;
	spatial_partition = get_interface<void*, interface_type::index> ( "engine.dll", "SpatialPartition001" );
	tesla = reinterpret_cast<CTeslaInfo*>( *reinterpret_cast<uintptr_t*>( utilities::pattern_scan ( "client.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 8B 47 18" ) ) );
	file_system = get_interface<void, interface_type::index> ( "filesystem_stdio.dll", "VFileSystem017" );
	console::log ( "[setup] interfaces initialized!\n" );

	return true;
}
