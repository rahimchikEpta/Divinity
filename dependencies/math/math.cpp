#include "../utilities/csgo.hpp"
#include <DirectXMath.h>
//aimtux
void math::correct_movement(vec3_t old_angles, c_usercmd* cmd, float old_forwardmove, float old_sidemove) {
	float delta_view;
	float f1;
	float f2;

	if (old_angles.y < 0.f)
		f1 = 360.0f + old_angles.y;
	else
		f1 = old_angles.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		delta_view = abs(f2 - f1);
	else
		delta_view = 360.0f - abs(f1 - f2);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}


vec3_t math::calculate_angle ( const vec3_t& src, const vec3_t& dst ) {
	/*vec3_t angles;
	vec3_t delta;
	delta.x = (a.x - b.x);
	delta.y = (a.y - b.y);
	delta.z = (a.z - b.z);
	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);
	angles.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angles.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);
	angles.z = 0.0f;
	if (delta.x >= 0.0) { angles.y += 180.0f; }
	return angles;*/
	vec3_t angle;
	vector_angles ( dst - src, angle );

	return angle;
}
vec3_t math::calculate_angle2 ( const vec3_t& source, const vec3_t& destination, const vec3_t& viewAngles ) {
	vec3_t delta = source - destination;
	vec3_t angles;

	angles.x = rad_to_deg ( atanf ( delta.z / std::hypotf ( delta.x, delta.y ) ) ) - viewAngles.x;
	angles.y = rad_to_deg ( atanf ( delta.y / delta.x ) ) - viewAngles.y;
	angles.z = 0.0f;

	if ( delta.x >= 0.0 )
		angles.y += 180.0f;

	return angles;
}

void math::sin_cos(float r, float& s, float& c) {
	s = sin(r);
	c = cos(r);
}


vec3_t math::angle_vector(vec3_t angle) {
	auto sy = sin(angle.y / 180.f * static_cast<float>(M_PI));
	auto cy = cos(angle.y / 180.f * static_cast<float>(M_PI));

	auto sp = sin(angle.x / 180.f * static_cast<float>(M_PI));
	auto cp = cos(angle.x / 180.f * static_cast<float>(M_PI));

	return vec3_t(cp * cy, cp * sy, -sp);
}

void math::transform_vector(vec3_t & a, matrix_t & b, vec3_t & out) {
	out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
	out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
	out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}

void math::vector_angles(const vec3_t & forward, vec3_t & angles) {
	float	tmp, yaw, pitch;

	if ( forward[ 1 ] == 0 && forward[ 0 ] == 0 ) {
		yaw = 0;
		if ( forward[ 2 ] > 0 )
			pitch = 270;
		else
			pitch = 90;
	}
	else {
		yaw = ( atan2 ( forward[ 1 ], forward[ 0 ] ) * 180 / DirectX::XM_PI );
		if ( yaw < 0 )
			yaw += 360;

		tmp = sqrt ( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );
		pitch = ( atan2 ( -forward[ 2 ], tmp ) * 180 / DirectX::XM_PI );
		if ( pitch < 0 )
			pitch += 360;
	}

	angles[ 0 ] = pitch;
	angles[ 1 ] = yaw;
	angles[ 2 ] = 0;
}
vec3_t math::cross_product ( const vec3_t& a, const vec3_t& b ) {
	return vec3_t ( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}
void math::vector_angles2 ( const vec3_t& forward, vec3_t& up, vec3_t& angles ) {
	vec3_t left = cross_product ( up, forward );
	left.normalized ();

	float forwardDist = forward.length_2d ();

	if ( forwardDist > 0.001f ) {
		angles.x = atan2f ( -forward.z, forwardDist ) * 180 / M_PI_F;
		angles.y = atan2f ( forward.y, forward.x ) * 180 / M_PI_F;

		float upZ = ( left.y * forward.x ) - ( left.x * forward.y );
		angles.z = atan2f ( left.z, upZ ) * 180 / M_PI_F;
	}
	else {
		angles.x = atan2f ( -forward.z, forwardDist ) * 180 / M_PI_F;
		angles.y = atan2f ( -left.x, left.y ) * 180 / M_PI_F;
		angles.z = 0;
	}
}
float math::get_fov ( const vec3_t& view_angle, const vec3_t& aim_angle ) {
	vec3_t ang, aim;

	angle_vectors ( view_angle, aim );
	angle_vectors ( aim_angle, ang );

	return RAD2DEG ( acos ( aim.dot ( ang ) / aim.length_sqr () ) );
}
float math::distance_point_to_line ( vec3_t Point, vec3_t LineOrigin, vec3_t Dir ) {
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.dot ( Dir ) / ( Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z );
	if ( TempOffset < 0.000001f )
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + ( Dir * TempOffset );

	return ( Point - PerpendicularPoint ).length ();
}
void math::angle_vectors ( const vec3_t& angles, vec3_t& forward ) {
	float	sp, sy, cp, cy;

	DirectX::XMScalarSinCos ( &sp, &cp, DEG2RAD ( angles.x ) );
	DirectX::XMScalarSinCos ( &sy, &cy, DEG2RAD ( angles.y ) );

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
void math::angle_vectors ( const vec3_t& angles, vec3_t& forward, vec3_t& right, vec3_t& up ) {
	float sr, sp, sy, cr, cp, cy;

	sin_cos ( DEG2RAD ( angles[ 1 ] ), sy, cy );
	sin_cos ( DEG2RAD ( angles[ 0 ] ), sp, cp );
	sin_cos ( DEG2RAD ( angles[ 2 ] ), sr, cr );

	forward.x = ( cp * cy );
	forward.y = ( cp * sy );
	forward.z = ( -sp );
	right.x = ( -1 * sr * sp * cy + -1 * cr * -sy );
	right.y = ( -1 * sr * sp * sy + -1 * cr * cy );
	right.z = ( -1 * sr * cp );
	up.x = ( cr * sp * cy + -sr * -sy );
	up.y = ( cr * sp * sy + -sr * cy );
	up.z = ( cr * cp );
}
void math::angle_vectors2 ( const vec3_t& angles, vec3_t* forward ) {
	float	sp, sy, cp, cy;

	sy = sin ( deg_to_rad ( angles[ 1 ] ) );
	cy = cos ( deg_to_rad ( angles[ 1 ] ) );

	sp = sin ( deg_to_rad ( angles[ 0 ] ) );
	cp = cos ( deg_to_rad ( angles[ 0 ] ) );

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

vec3_t math::vector_add(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

vec3_t math::vector_subtract(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x - b.x,
		a.y - b.y,
		a.z - b.z);
}

vec3_t math::vector_multiply(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x * b.x,
		a.y * b.y,
		a.z * b.z);
}

vec3_t math::vector_divide(vec3_t & a, vec3_t & b) {
	return vec3_t(a.x / b.x,
		a.y / b.y,
		a.z / b.z);
}

bool math::screen_transform(const vec3_t & point, vec3_t & screen) {
	auto matrix = interfaces::engine->world_to_screen_matrix();

	float w = matrix[3][0] * point.x + matrix[3][1] * point.y + matrix[3][2] * point.z + matrix[3][3];
	screen.x = matrix[0][0] * point.x + matrix[0][1] * point.y + matrix[0][2] * point.z + matrix[0][3];
	screen.y = matrix[1][0] * point.x + matrix[1][1] * point.y + matrix[1][2] * point.z + matrix[1][3];
	screen.z = 0.0f;

	int inverse_width = static_cast<int>((w < 0.001f) ? -1.0f / w :
		1.0f / w);

	screen.x *= inverse_width;
	screen.y *= inverse_width;
	return (w < 0.001f);
}

bool math::world_to_screen(const vec3_t & origin, vec3_t & screen) {
	static std::uintptr_t view_matrix;
	if ( !view_matrix )
		view_matrix = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( utilities::pattern_scan( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) ) + 3 ) + 176;

	const auto& matrix = *reinterpret_cast< view_matrix_t* >( view_matrix );

	const auto w = matrix.m[ 3 ][ 0 ] * origin.x + matrix.m[ 3 ][ 1 ] * origin.y + matrix.m[ 3 ][ 2 ] * origin.z + matrix.m[ 3 ][ 3 ];
	if ( w < 0.001f )
		return false;

	int x, y;
	interfaces::engine->get_screen_size( x, y );
	
	screen.x = static_cast<float>(x) / 2.0f;
	screen.y = static_cast<float>(y) / 2.0f;

	screen.x *= 1.0f + ( matrix.m[ 0 ][ 0 ] * origin.x + matrix.m[ 0 ][ 1 ] * origin.y + matrix.m[ 0 ][ 2 ] * origin.z + matrix.m[ 0 ][ 3 ] ) / w;
	screen.y *= 1.0f - ( matrix.m[ 1 ][ 0 ] * origin.x + matrix.m[ 1 ][ 1 ] * origin.y + matrix.m[ 1 ][ 2 ] * origin.z + matrix.m[ 1 ][ 3 ] ) / w;

	return true;
}
void math::normalize_angle ( vec3_t& angles ) {
	for ( auto i = 0; i < 3; i++ ) {
		while ( angles[ i ] < -180.0f ) angles[ i ] += 360.0f;
		while ( angles[ i ] > 180.0f ) angles[ i ] -= 360.0f;
	}
}
void math::clamp_angle ( vec3_t& angles ) {
	angles.x = std::clamp ( angles.x, -89.0f, 89.0f );
	angles.y = std::clamp ( angles.y, -180.0f, 180.0f );
	angles.z = 0.0f;
}
bool math::sanitize_angle ( vec3_t& angles ) {
	vec3_t temp = angles;
	normalize_angle ( temp );
	clamp_angle ( temp );

	if ( !isfinite ( temp.x ) ||
		!isfinite ( temp.y ) ||
		!isfinite ( temp.z ) )
		return false;

	angles = temp;

	return true;
}
void math::angle_vectors_alternative ( vec3_t& angles, vec3_t* forward, vec3_t* right, vec3_t* up ) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos ( deg_to_rad ( angles.x ), sp, cp );
	sin_cos ( deg_to_rad ( angles.y ), sy, cy );
	sin_cos ( deg_to_rad ( angles.z ), sr, cr );

	if ( forward ) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if ( right ) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if ( up ) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}
float math::random_float ( float min, float max ) {
	static auto rnd = reinterpret_cast<float ( * ) ( float, float )> ( GetProcAddress ( GetModuleHandleW ( L"vstdlib.dll" ), "RandomFloat" ) );

	if ( rnd )
		return rnd ( min, max );
	else
		return 0.f;
}
const void math::normalize_in_place ( vec3_t& angles ) {
	for ( auto i = 0; i < 3; i++ ) {
		while ( angles[ i ] < -180.0f )
			angles[ i ] += 360.0f;

		while ( angles[ i ] > 180.0f )
			angles[ i ] -= 360.0f;
	}
}

void math::rotate_triangle(std::array<vec2_t, 3>& points, float rotation)
{
	vec2_t points_center = vec2_t(points.at(0) + points.at(1) + points.at(2)) / vec2_t(3, 3);
	for (auto& point : points)
	{
		point -= points_center;

		const auto temp_x = point.x;
		const auto temp_y = point.y;

		const auto theta = deg_to_rad(rotation);
		const auto c = cosf(theta);
		const auto s = sinf(theta);

		point.x = temp_x * c - temp_y * s;
		point.y = temp_x * s + temp_y * c;

		point += points_center;
	}
}