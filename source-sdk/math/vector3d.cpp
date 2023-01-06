#include "vector3d.hpp"
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>

vec3_t::vec3_t(void) {
	x = y = z = 0.0f;
}

vec3_t::vec3_t(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

vec3_t::~vec3_t(void) {};

void vec3_t::init(float _x, float _y, float _z) {
	x = _x; y = _y; z = _z;
}
bool vec3_t::zero() {
	return ((x == 0) && (y == 0) && (z == 0));
}

void vec3_t::clamp(void) {
	x = std::clamp(x, -89.0f, 89.0f);
	y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
	z = std::clamp(z, -50.0f, 50.0f);
}

vec3_t vec3_t::clamped() {
	vec3_t clamped = *this;
	clamped.clamp();
	return clamped;
}

float vec3_t::distance_to(const vec3_t& other) {
	vec3_t delta;
	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.length();
}

void vec3_t::normalize() {
	x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
	y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
	z = 0.0f;
}

vec3_t vec3_t::normalized(void) {
	vec3_t vec(*this);
	vec.normalize();

	return vec;
}

float vec3_t::length(void) {
	float root = 0.0f, sqsr = this->length_sqr();

	__asm        sqrtss xmm0, sqsr
	__asm        movss root, xmm0

	return root;
}

float vec3_t::length_sqr(void) {
	auto sqr = [](float n) {
		return static_cast<float>(n * n);
	};

	return (sqr(x) + sqr(y) + sqr(z));
}
vec3_t vec3_t::to_angle()
{
	return vec3_t{ RAD2DEG(std::atan2(-z, std::hypot(x,y))), RAD2DEG(std::atan2(y,x)), 0.f };
}

float vec3_t::normalize_movement()
{
	vec3_t res = *this;
	float l = res.length();
	if (l != 0.0f)
	{
		res /= l;
	}
	else
	{
		res.x = res.y = res.z = 0.0f;
	}
	return l;
}
float vec3_t::length_2d_sqr(void) const {
	return (x * x + y * y);
}

float vec3_t::dot(const vec3_t other) {
	return (x * other.x + y * other.y + z * other.z);
}
float vec3_t::dot ( const vec3_t& other ) const {
	return ( x * other.x + y * other.y + z * other.z );
}
float vec3_t::dot(const float* other) {
	const vec3_t& a = *this;

	return(a.x * other[0] + a.y * other[1] + a.z * other[2]);
}
void vec3_t::mul ( float scalar ) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void vec3_t::mul_add ( const vec3_t& a, const vec3_t& b, float scalar ) {
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}

void vec3_t::to_zero ( ) {
	x = 0;
	y = 0;
	z = 0;
}