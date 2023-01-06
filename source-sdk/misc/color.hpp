#pragma once
#include <cstdint>
#include <d3d9.h>

struct color {
	int a, r, g, b;
	color( ) = default;
	color( const int r, const int g, const int b, const int a = 255 ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	explicit color( const uint32_t color ) {
		this->a = ( color >> 24 ) & 0xff;
		this->r = ( color >> 16 ) & 0xff;
		this->g = ( color >> 8 ) & 0xff;
		this->b = ( color & 0xff );
	}
	bool operator== (const color& rhs)
	{
		return (*((int*)this) == *((int*)&rhs));
	}
	bool operator!= (const color& rhs)
	{
		return !(operator==(rhs));
	}
	static color from_uint( const uint32_t uint ) {
		return color( uint );
	}
	__forceinline color set_alpha(int al)
	{
		return color(this->r, this->g, this->b, std::clamp(al, 0, 255));
	}
	static std::uint32_t to_d3d(const color& col) {
		return D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	}
	static D3DCOLOR from_color( const color col ) {
		return D3DCOLOR_ARGB( col.a, col.r, col.g, col.b );
	}
	__forceinline static color blend(const color& a, const color& b, const float multiplier)
	{
		return color
		(
			a.r + static_cast<int>(std::clamp(multiplier, 0.f, 255.f) * (b.r - a.r)),
			a.g + static_cast<int>(std::clamp(multiplier, 0.f, 255.f) * (b.g - a.g)),
			a.b + static_cast<int>(std::clamp(multiplier, 0.f, 255.f) * (b.b - a.b)),
			a.a + static_cast<int>(std::clamp(multiplier, 0.f, 255.f) * (b.a - a.a))
		);
	}
	static color black( const int a = 255 ) { return { 0, 0, 0, a }; }
	static color white( const int a = 255 ) { return { 255, 255, 255, a }; }
	static color red( const int   a = 255 ) { return { 255, 0, 0, a }; }
	static color green( const int a = 255 ) { return { 0, 255, 0, a }; }
	static color blue( const int  a = 255 ) { return { 0, 0, 255, a }; }
	static color from_float(float* clr) { return { int(clr[0] * 255.f), int(clr[1] * 255.f), int(clr[2] * 255.f), int(clr[3] * 255.f) }; }
	static color from_float2(float* clr, float alpha) { return { int(clr[0] * 255.f), int(clr[1] * 255.f), int(clr[2] * 255.f), int(alpha * 255) }; }
};
