#pragma once
#define deg_to_rad( x )  ( (float)(x) * (float)( M_PI / 180.f ) )
#define rad_to_deg( x )  ( (float)(x) * (float)( 180.f / M_PI ) )
namespace math {
	constexpr float rad_per_deg{ 0.01745329251f };
	constexpr float deg_per_rad{ 57.2957795131f };
	void correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove);
	vec3_t calculate_angle( const vec3_t& a, const vec3_t& b );
	vec3_t calculate_angle2 ( const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles );
	void sin_cos(float r, float& s, float& c);
	vec3_t angle_vector(vec3_t angle);
	void transform_vector(vec3_t&, matrix_t&, vec3_t&);
	void vector_angles(const vec3_t&, vec3_t&);
	vec3_t cross_product ( const vec3_t& a, const vec3_t& b );
	void vector_angles2 ( const vec3_t& forward, vec3_t& up, vec3_t& angles );
	float get_fov ( const vec3_t& view_angle, const vec3_t& aim_angle );
	float distance_point_to_line ( vec3_t Point, vec3_t LineOrigin, vec3_t Dir );
	void angle_vectors ( const vec3_t& angles, vec3_t& forward );
	void angle_vectors ( const vec3_t& angles, vec3_t& forward, vec3_t& right, vec3_t& up );
	void angle_vectors2 ( const vec3_t& angles, vec3_t* forward );
	void angle_vectors(vec3_t&, vec3_t*, vec3_t*, vec3_t*);
	vec3_t vector_add(vec3_t&, vec3_t&);
	vec3_t vector_subtract(vec3_t&, vec3_t&);
	vec3_t vector_multiply(vec3_t&, vec3_t&);
	vec3_t vector_divide(vec3_t&, vec3_t&);
	bool screen_transform(const vec3_t& point, vec3_t& screen);
	bool world_to_screen(const vec3_t& origin, vec3_t& screen);
	void normalize_angle ( vec3_t& angles );
	void clamp_angle ( vec3_t& angles );
	bool sanitize_angle ( vec3_t& angles );
	void angle_vectors_alternative ( vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up );
	float random_float ( float min, float max );
	const void normalize_in_place ( vec3_t& angles );

	void rotate_triangle(std::array<vec2_t, 3>& points, float rotation);

	inline float lerp ( float a, float b, float f ) {
		return a + f * ( b - a );
	}
};
