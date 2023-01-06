#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"
#include <cmath>
#include <numbers>
#include "../../source-sdk/misc/color.hpp"
#include <chrono>

class address
{
public:
	__forceinline  address() : m_address{} {};
	__forceinline ~address() {};

	__forceinline address(std::size_t address) : m_address{ address }
	{
	}
	__forceinline address(void* address) : m_address{ reinterpret_cast<std::size_t>(address) }
	{
	}

	inline operator std::size_t()
	{
		return m_address;
	}

	inline operator void* ()
	{
		return reinterpret_cast<void*>(m_address);
	}

	inline bool operator==(address address)
	{
		return as< std::size_t >() == address.as< std::size_t >();
	}

	inline bool operator!=(address address)
	{
		return as< std::size_t >() != address.as< std::size_t >();
	}

	template< typename t = address > inline t to()
	{
		return *(t*)m_address;
	}

	template< typename t = address > inline t as()
	{
		return m_address ? (t)m_address : t();
	}

	template< typename t = address > inline t at(std::size_t offset)
	{
		return m_address ? *(t*)(m_address + offset) : t();
	}

	template< typename t = address > inline t add(std::size_t offset)
	{
		return m_address ? (t)(m_address + offset) : t();
	}

	template< typename t = address > inline t sub(std::size_t offset)
	{
		return m_address ? (t)(m_address - offset) : t();
	}

	template< typename t = address >
	static __forceinline t RVA(address base, size_t offset) {
		return base.as< t >(offset);
	}

	template< typename t = address > inline t get(std::size_t dereferences = 1)
	{
		if (!m_address)
			return t();

		std::size_t object = m_address;

		while (dereferences--)
			if (object)
				object = *reinterpret_cast<std::size_t*>(object);

		return (t)object;
	}

	template< typename t = std::size_t > inline void set(const t& value)
	{
		if (!m_address)
			return;

		*(t*)m_address = value;
	}

	template< typename t = address > inline t relative(std::size_t offset = 0x1)
	{
		if (!m_address)
			return t();

		std::size_t new_address = m_address + offset;

		std::int32_t relative_offset = *reinterpret_cast<std::int32_t*>(new_address);

		if (!relative_offset)
			return t();

		return (t)(new_address + sizeof(std::size_t) + relative_offset);
	}

	template< typename t = address >
	__forceinline t rel32(size_t offset) {
		uintptr_t   out;
		uint32_t    r;

		if (!m_address)
			return t{};

		out = m_address + offset;
		r = *(uint32_t*)out;
		if (!r)
			return t{};
		out = (out + 4) + r;

		return (t)out;
	}

protected:

	std::size_t m_address;
};

namespace utilities {
	template< typename t >
	__forceinline static t call_virtual( void *name, int offset ) {
		int *table = *( int ** ) name;
		int address = table[ offset ];
		return ( t ) ( address );
	}

	template<typename T, typename ...Args>
	constexpr auto call_virtual_method ( void* class_base, int index, Args... args ) {
		return ( ( *reinterpret_cast<T ( __thiscall*** )( void*, Args... )>( class_base ) )[ index ] )( class_base, args... );
	}

	struct hud_weapons_t {
		std::int32_t* get_weapon_count () {
			return reinterpret_cast<std::int32_t*>( std::uint32_t ( this ) + 0x80 );
		}
	};
	uint8_t* scan_pattern(void* PModule, const char* Signature);
	template<typename t>
	t pattern_to_address(void* module, const char* sig)
	{
		return address(scan_pattern(module, sig)).as<t>();
	}
	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;
	std::tuple<float, float, float> draw_rainbow(float speed) noexcept;
	bool goes_through_smoke ( vec3_t start, vec3_t end );
	void force_update ();
	color fade_between ( color first, color second );
	template<class T>
	static T* find_hud_element ( const char* name );

	__forceinline int epoch_time () {
		return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now ().time_since_epoch () ).count ();
	}
	inline bool force_updated;
	inline bool do_forceupdate = false;

	template <typename T>
	//is V inside lst, example of usage:
	//if (utils::is_in(num, {1, 2, 3})) { function }
	__forceinline bool is_in(const T& v, std::initializer_list<T> lst)
	{
		return std::find(std::begin(lst), std::end(lst), v) != std::end(lst);
	};


	template <typename I>
	__forceinline I v(void* iface, uint32_t index) { return (I)((*(uint32_t**)iface)[index]); }
}
