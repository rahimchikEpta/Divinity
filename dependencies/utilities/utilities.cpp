#include "utilities.hpp"
#include "../utilities/csgo.hpp"
#include <psapi.h>
#include "../../core/menu/ImGui/easing.h"

std::uint8_t* utilities::pattern_scan ( const char* module_name, const char* signature ) noexcept {
	const auto module_handle = GetModuleHandleA ( module_name );

	if ( !module_handle )
		return nullptr;

	static auto pattern_to_byte = [ ]( const char* pattern ) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>( pattern );
		auto end = const_cast<char*>( pattern ) + std::strlen ( pattern );

		for ( auto current = start; current < end; ++current ) {
			if ( *current == '?' ) {
				++current;

				if ( *current == '?' )
					++current;

				bytes.push_back ( -1 );
			}
			else {
				bytes.push_back ( std::strtoul ( current, &current, 16 ) );
			}
		}
		return bytes;
	};

	auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>( module_handle );
	auto nt_headers =
		reinterpret_cast<PIMAGE_NT_HEADERS>( reinterpret_cast<std::uint8_t*>( module_handle ) + dos_header->e_lfanew );

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte ( signature );
	auto scan_bytes = reinterpret_cast<std::uint8_t*>( module_handle );

	auto s = pattern_bytes.size ();
	auto d = pattern_bytes.data ();

	for ( auto i = 0ul; i < size_of_image - s; ++i ) {
		bool found = true;

		for ( auto j = 0ul; j < s; ++j ) {
			if ( scan_bytes[ i + j ] != d[ j ] && d[ j ] != -1 ) {
				found = false;
				break;
			}
		}
		if ( found )
			return &scan_bytes[ i ];
	}

	throw std::runtime_error ( std::string ( "Wrong signature: " ) + signature );
}

std::tuple<float, float, float> utilities::draw_rainbow ( float speed ) noexcept {
	constexpr float pi = std::numbers::pi_v<float>;
	return std::make_tuple ( std::sin ( speed * interfaces::globals->realtime ) * 0.5f + 0.5f,
						   std::sin ( speed * interfaces::globals->realtime + 2 * pi / 3 ) * 0.5f + 0.5f,
						   std::sin ( speed * interfaces::globals->realtime + 4 * pi / 3 ) * 0.5f + 0.5f );
}

bool utilities::goes_through_smoke ( vec3_t start, vec3_t end ) {
	typedef bool ( __cdecl* goes_through_smoke )( vec3_t, vec3_t );
	static uint32_t goes_through_smoke_offset = (uint32_t) utilities::pattern_scan ( "client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" );
	static goes_through_smoke goes_through_smoke_fn = (goes_through_smoke) goes_through_smoke_offset;
	return goes_through_smoke_fn ( start, end );
}

template<class T>
static T* utilities::find_hud_element ( const char* name ) {
	static auto fn = *reinterpret_cast<DWORD**>( utilities::pattern_scan ( "client.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08" ) + 1 );
	if (!fn)
		return (T*)0;

	static auto find_hud_element = reinterpret_cast<DWORD ( __thiscall* )( void*, const char* )>( utilities::pattern_scan ( "client.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) );
	if (!find_hud_element)
		return (T*)0;

	return (T*) find_hud_element ( fn, name );
}

void utilities::force_update () {
	if ( !interfaces::engine->is_in_game () )
		return;
	static auto clear_hud_weapon_icon_fn = reinterpret_cast<std::int32_t ( __thiscall* )( void*, std::int32_t )>(
		pattern_scan ( "client.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 34" ) );

	if (!clear_hud_weapon_icon_fn)
		return;

	auto element = find_hud_element<std::uintptr_t*> ( ( "CCSGO_HudWeaponSelection" ) );

	if (!element)
		return;

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>( std::uintptr_t ( element ) - 0xa0 );
	if ( !hud_weapons )
		return;

	if ( !*hud_weapons->get_weapon_count () )
		return;

	for ( std::int32_t i = 0; i < *hud_weapons->get_weapon_count (); i++ )
		i = clear_hud_weapon_icon_fn ( hud_weapons, i );

	force_updated = true;

	if (!interfaces::clientstate)
		return;

	interfaces::clientstate->full_update ();
}

color utilities::fade_between ( color first, color second ) {
	static float percentage = 0.f;
	static bool has_to_reverse = false;
	if ( !has_to_reverse )
		percentage += ( 1.0 / 0.3f ) * interfaces::globals->frame_time * 0.25f;
	else
		percentage -= ( 1.0 / 0.3f ) * interfaces::globals->frame_time * 0.25f;
	percentage = std::clamp<float> ( percentage, 0.f, 1.f );

	if ( percentage == 1.f )
		has_to_reverse = true;
	else if ( percentage == 0.f )
		has_to_reverse = false;

	float eased = Easing::InOutCirc ( percentage );
	int r_up = second.r - first.r;
	int r_down = first.r - second.r;

	int g_up = second.g - first.g;
	int g_down = first.g - second.g;

	int b_up = second.b - first.b;
	int b_down = first.b - second.b;

	bool red_has_to_go_up = first.r > second.r;
	bool green_has_to_go_up = first.g > second.g;
	bool blue_has_to_go_up = first.b > second.b;

	return color ( red_has_to_go_up ? first.r + ( eased * r_up ) : first.r - ( eased * r_down ), green_has_to_go_up ? first.g + ( eased * g_up ) : first.g - ( eased * g_down ), blue_has_to_go_up ? first.b + ( eased * b_up ) : first.b - ( eased * b_down ) );
}
#define xin_range(x,a,b)		(x >= a && x <= b) 
#define xget_bits( x )		(xin_range(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define xget_byte( x )		(xget_bits(x[0]) << 4 | xget_bits(x[1]))
uint8_t* utilities::scan_pattern(void* PModule, const char* Signature) {
	const char* pat = Signature;
	uint8_t* first_match = 0;

	static auto id = 0;
	++id;

	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)PModule;
	PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((int8_t*)PModule + dos->e_lfanew);

	for (uint8_t* current = (uint8_t*)PModule; current < (uint8_t*)PModule + nt->OptionalHeader.SizeOfCode; current++) {
		if (*(uint8_t*)pat == '\?' || *(uint8_t*)current == xget_byte(pat)) {
			if (!*pat)
				return first_match;
			if (!first_match)
				first_match = current;
			if (!pat[2])
				return first_match;

			pat += (*(uint16_t*)pat == (uint16_t)'\?\?' || *(uint8_t*)pat != (uint8_t)'\?') ? 3 : 2;
		}
		else {
			if (first_match != 0)
				current = first_match;
			pat = Signature;
			first_match = 0;
		}
	}

#ifdef _DEBUG
	assert(false);

	std::stringstream ss{};

	ss << "[pattern-scan] failed to find pattern: " << id << ", sig: " << Signature << ", module:" << PModule;

	throw std::runtime_error(ss.str());
#endif
	return 0;
}