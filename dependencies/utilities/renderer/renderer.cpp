#include "renderer.hpp"
#include "fonts.h"
#include "../../utilities/csgo.hpp"

ID3DXFont* render::fonts::name;
ID3DXFont* render::fonts::flags;
ID3DXFont* render::fonts::key_strokes;
IDirect3DDevice9* render::device;
IDirect3DVertexBuffer9* render::vertex_buffer;
IDirect3DIndexBuffer9* render::index_buffer;

void render::reset() {
	if (render::fonts::name)
		render::fonts::name->Release();
	if (render::fonts::flags)
		render::fonts::flags->Release();
}

bool render::do_init(IDirect3DDevice9* device) {
	if (!device)
		return false;

	render::device = device;


	D3DXCreateFontA(render::device, 14, 0, 100, 1, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana", &render::fonts::name);
	D3DXCreateFontA(render::device, 10, 0, 500, 1, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Tohoma", &render::fonts::flags); //Smallest Pixel-7
	D3DXCreateFontA(render::device, 12, 0, 500, 1, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Tohoma", &render::fonts::key_strokes);
	if (!render::fonts::name)
		return false;
	if (!render::fonts::flags)
		return false;
	if (!render::fonts::key_strokes)
		return false;

	return true;
}

void render::set_states() {
	D3DVIEWPORT9 scrn;
	render::device->GetViewport(&scrn);

	render::device->SetVertexShader(nullptr);
	render::device->SetPixelShader(nullptr);
	render::device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	render::device->SetRenderState(D3DRS_LIGHTING, false);
	render::device->SetRenderState(D3DRS_FOGENABLE, false);
	render::device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	render::device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	render::device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	render::device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	render::device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	render::device->SetRenderState(D3DRS_STENCILENABLE, false);

	render::device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
	render::device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, false);

	render::device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	render::device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	render::device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
	render::device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	render::device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	render::device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	render::device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	render::device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	render::device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void render::line(const vec2_t& start, const vec2_t& end, const color& col) {
	vertex_t vtx[2] = { { start.x, start.y, 0.0f, 1.0f, color::to_d3d(col) }, { end.x, end.y, 0.0f, 1.0f, color::to_d3d(col) } };

	render::device->SetTexture(0, nullptr);
	render::device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &vtx, sizeof(vertex_t));
}

void render::gradient_rectangle(const vec2_t& pos, const vec2_t& size, const color& col, const color& oth, const bool vertical) {
	const auto one = color::to_d3d(col), two = color::to_d3d(oth);
	std::uint32_t c1, c2, c3, c4;

	if (!vertical) {
		c1 = one;
		c2 = two;
		c3 = one;
		c4 = two;
	}
	else {
		c1 = one;
		c2 = one;
		c3 = two;
		c4 = two;
	}

	vertex_t vert[4] = {
		{ pos.x, pos.y, 0.0f, 1.0f, c1 },
		{ pos.x + size.x, pos.y, 0.0f, 1.0f, c2 },
		{ pos.x, pos.y + size.y, 0.0f, 1.0f, c3 },
		{ pos.x + size.x, pos.y + size.y, 0.0f, 1.0f, c4 }
	};

	render::device->SetTexture(0, nullptr);
	render::device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, false);
	render::device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &vert, sizeof(vertex_t));
	render::device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);
}

void render::filled_rectangle(const vec2_t& pos, const vec2_t& size, const color& col) {
	vertex_t vtx[4] = { { pos.x, pos.y + size.y , 0.0f, 1.0f, color::to_d3d(col) }, { pos.x, pos.y, 0.0f, 1.0f, color::to_d3d(col) }, { pos.x + size.x, pos.y + size.y, 0.0f, 1.0f, color::to_d3d(col) }, { pos.x + size.x, pos.y, 0.0f, 1.0f, color::to_d3d(col) } };

	render::device->SetTexture(0, nullptr);
	render::device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, false);
	render::device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(vertex_t));
	render::device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);
}

void render::filled_circle(const vec2_t& pos, int radius, int segments, const color& col) {
	vertex_t* vtx = new vertex_t[segments + 1];

	for (int i = 0; i <= segments; i++)
		vtx[i] = { pos.x + radius * cosf(D3DX_PI * (i / (segments / 2.0f))), pos.y + radius * sinf(D3DX_PI * (i / (segments / 2.0f))), 0.0f, 1.0f, color::to_d3d(col) };

	render::device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	render::device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, segments, vtx, sizeof(vertex_t));

	delete[] vtx;
}

void render::rectangle(const vec2_t& pos, const vec2_t& size, const color& col) {
	filled_rectangle(vec2_t(pos.x, pos.y), vec2_t(size.x, 1), col);
	filled_rectangle(vec2_t(pos.x, pos.y), vec2_t(1, size.y), col);
	filled_rectangle(vec2_t(pos.x + size.x - 1, pos.y), vec2_t(1, size.y), col);
	filled_rectangle(vec2_t(pos.x, pos.y + size.y - 1), vec2_t(size.x, 1), col);
}

void render::dispatch_vertex_list(vertex_t* vertex_list, int size, D3DPRIMITIVETYPE primitive) {
	render::device->DrawPrimitiveUP(primitive, size, vertex_list, sizeof(vertex_t));
}

vec2_t render::text_position(const std::string& txt, ID3DXFont* font) {
	RECT rect = RECT();
	font->DrawTextA(nullptr, txt.data(), txt.length(), &rect, DT_CALCRECT, D3DCOLOR_RGBA(0, 0, 0, 0));

	vec2_t ret(rect.right - rect.left, rect.bottom - rect.top);
	return ret;
}

void render::text(const vec2_t& pos, const color& col, const std::string& txt, ID3DXFont* font, bool centered) {
	RECT rect = RECT();

	SetRect(&rect, pos.x, pos.y, pos.x, pos.y);
	font->DrawTextA(nullptr, txt.data(), -1, &rect, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(col));
}

void render::text_shadow(const vec2_t& pos, const color& col, const std::string& txt, ID3DXFont* font, const color& shadowcol, bool centered) {
	RECT rect = RECT();
	RECT rect2 = RECT();

	SetRect(&rect2, pos.x + 1, pos.y + 1, pos.x + 1, pos.y + 1);
	font->DrawTextA(nullptr, txt.data(), -1, &rect2, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(shadowcol));

	SetRect(&rect, pos.x, pos.y, pos.x, pos.y);
	font->DrawTextA(nullptr, txt.data(), -1, &rect, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(col));
}

void render::text_outline(const vec2_t& pos, const color& col, const std::string& txt, ID3DXFont* font, const color& outcol, bool centered) {
	RECT rect = RECT();
	RECT rect1 = RECT();
	RECT rect2 = RECT();
	RECT rect3 = RECT();
	RECT rect4 = RECT();

	SetRect(&rect1, pos.x + 1, pos.y, pos.x + 1, pos.y);
	font->DrawTextA(nullptr, txt.data(), -1, &rect1, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(outcol));

	SetRect(&rect2, pos.x - 1, pos.y, pos.x - 1, pos.y);
	font->DrawTextA(nullptr, txt.data(), -1, &rect2, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(outcol));

	SetRect(&rect3, pos.x, pos.y + 1, pos.x, pos.y + 1);
	font->DrawTextA(nullptr, txt.data(), -1, &rect3, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(outcol));

	SetRect(&rect4, pos.x, pos.y - 1, pos.x, pos.y - 1);
	font->DrawTextA(nullptr, txt.data(), -1, &rect4, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(outcol));

	SetRect(&rect, pos.x, pos.y, pos.x, pos.y);
	font->DrawTextA(nullptr, txt.data(), -1, &rect, centered ? DT_CENTER | DT_NOCLIP : DT_LEFT | DT_NOCLIP, color::to_d3d(col));
}

D3DVIEWPORT9 render::get_viewport() {
	D3DVIEWPORT9 vp;
	render::device->GetViewport(&vp);
	return vp;
}

void render::set_viewport(D3DVIEWPORT9 vp) {
	render::device->SetViewport(&vp);
}

/* easy to understand wrappers */
D3DVIEWPORT9 render::start_clipping(const vec2_t& pos, const vec2_t& size) {
	const auto old_vp = get_viewport(), new_vp = D3DVIEWPORT9{ unsigned long(pos.x) , unsigned long(pos.y) , unsigned long(size.x) , unsigned long(size.y) };
	set_viewport(new_vp);
	return old_vp;
}

void render::end_clipping(D3DVIEWPORT9 vp) {
	set_viewport(vp);
}

/* source specific */
bool render::screen_transform(vec3_t& screen, vec3_t& origin) {
	static auto view_matrix = *reinterpret_cast<std::uintptr_t*>(utilities::pattern_scan("client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9") + 3) + 176;

	if (!view_matrix)
		return false;

	const auto& world_matrix = *reinterpret_cast<view_matrix_t*>(view_matrix);

	screen.x = world_matrix.m[0][0] * origin.x + world_matrix.m[0][1] * origin.y + world_matrix.m[0][2] * origin.z + world_matrix.m[0][3];
	screen.y = world_matrix.m[1][0] * origin.x + world_matrix.m[1][1] * origin.y + world_matrix.m[1][2] * origin.z + world_matrix.m[1][3];
	screen.z = 0;

	const auto w = world_matrix.m[3][0] * origin.x + world_matrix.m[3][1] * origin.y + world_matrix.m[3][2] * origin.z + world_matrix.m[3][3];

	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;

		return false;
	}

	screen.x *= 1.f / w;
	screen.y *= 1.f / w;

	return true;
}

bool render::world_to_screen(vec3_t& origin, vec3_t& screen) {
	if (!render::screen_transform(screen, origin))
		return false;

	int width, height;
	interfaces::engine->get_screen_size(width, height);

	screen.x = (width * 0.5f) + (screen.x * width) * 0.5f;
	screen.y = (height * 0.5f) - (screen.y * height) * 0.5f;

	return true;
}

void render::color_convert_hsv_to_rgb(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
	if (s == 0.0f) {
		// gray
		out_r = out_g = out_b = v;
		return;
	}

	h = fmod(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i) {
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	}
}

void render::draw_circle_3d(const vec3_t& origin, float radius, color col) {
	vec3_t prev_scr_pos;
	vec3_t scr_pos;
	float step = M_PI * 2.0 / 64;
	for (float rotation = -step; rotation <= (M_PI * 2.0); rotation += step) {
		vec3_t pos(radius * cos(rotation) + origin.x, radius * sin(rotation) + origin.y, origin.z);

		ray_t ray;
		trace_t trace;
		trace_filter filter;

		filter.skip = csgo::local_player;
		ray.init(origin, pos);

		interfaces::trace_ray->trace_ray(ray, MASK_SOLID, &filter, &trace);
		if (math::world_to_screen(trace.end, scr_pos)) {

			if (prev_scr_pos.x == 0)
				prev_scr_pos.x = scr_pos.x;
			if (prev_scr_pos.y == 0)
				prev_scr_pos.y = scr_pos.y;
			line(vec2_t(prev_scr_pos.x, prev_scr_pos.y), vec2_t(scr_pos.x, scr_pos.y), col);

			prev_scr_pos = scr_pos;
		}
	}
}
void a_angle_vectors(const vec3_t& angles, vec3_t& forward, vec3_t& right, vec3_t& up) {
	float sr, sp, sy, cr, cp, cy;

	math::sin_cos(DEG2RAD(angles[1]), sy, cy);
	math::sin_cos(DEG2RAD(angles[0]), sp, cp);
	math::sin_cos(DEG2RAD(angles[2]), sr, cr);

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}
void render::draw_3d_cube(float scalar, vec3_t angles, vec3_t middle_origin, color col) {
	vec3_t forward, right, up;
	a_angle_vectors(angles, forward, right, up);

	vec3_t points[8];
	points[0] = middle_origin - (right * scalar) + (up * scalar) - (forward * scalar); // BLT
	points[1] = middle_origin + (right * scalar) + (up * scalar) - (forward * scalar); // BRT
	points[2] = middle_origin - (right * scalar) - (up * scalar) - (forward * scalar); // BLB
	points[3] = middle_origin + (right * scalar) - (up * scalar) - (forward * scalar); // BRB

	points[4] = middle_origin - (right * scalar) + (up * scalar) + (forward * scalar); // FLT
	points[5] = middle_origin + (right * scalar) + (up * scalar) + (forward * scalar); // FRT
	points[6] = middle_origin - (right * scalar) - (up * scalar) + (forward * scalar); // FLB
	points[7] = middle_origin + (right * scalar) - (up * scalar) + (forward * scalar); // FRB

	vec3_t points_screen[8];
	for (int i = 0; i < 8; i++)
		if (!math::world_to_screen(points[i], points_screen[i]))
			return;


	// Back frame
	render::line(vec2_t(points_screen[0].x, points_screen[0].y), vec2_t(points_screen[1].x, points_screen[1].y), col);
	render::line(vec2_t(points_screen[0].x, points_screen[0].y), vec2_t(points_screen[2].x, points_screen[2].y), col);
	render::line(vec2_t(points_screen[3].x, points_screen[3].y), vec2_t(points_screen[1].x, points_screen[1].y), col);
	render::line(vec2_t(points_screen[3].x, points_screen[3].y), vec2_t(points_screen[2].x, points_screen[2].y), col);
	// Frame connector
	render::line(vec2_t(points_screen[0].x, points_screen[0].y), vec2_t(points_screen[4].x, points_screen[4].y), col);
	render::line(vec2_t(points_screen[1].x, points_screen[1].y), vec2_t(points_screen[5].x, points_screen[5].y), col);
	render::line(vec2_t(points_screen[2].x, points_screen[2].y), vec2_t(points_screen[6].x, points_screen[6].y), col);
	render::line(vec2_t(points_screen[3].x, points_screen[3].y), vec2_t(points_screen[7].x, points_screen[7].y), col);
	// Front frame
	render::line(vec2_t(points_screen[4].x, points_screen[4].y), vec2_t(points_screen[5].x, points_screen[5].y), col);
	render::line(vec2_t(points_screen[4].x, points_screen[4].y), vec2_t(points_screen[6].x, points_screen[6].y), col);
	render::line(vec2_t(points_screen[7].x, points_screen[7].y), vec2_t(points_screen[5].x, points_screen[5].y), col);
	render::line(vec2_t(points_screen[7].x, points_screen[7].y), vec2_t(points_screen[6].x, points_screen[6].y), col);
}

void render::triangle_grad_filled(vec2_t pos1, vec2_t pos2, vec2_t pos3, color col1, color col2, color col3) {


	vertex_t vert[4] =
	{
		{ pos1.x, pos1.y, 0.0f, 1.0f, color::to_d3d(col1)},
	{ pos2.x, pos2.y, 0.0f, 1.0f, color::to_d3d(col2) },
	{ pos3.x, pos3.y, 0.0f, 1.0f, color::to_d3d(col3) }
	};

	render::device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	render::device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	render::device->SetTexture(0, nullptr);
	render::device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, &vert, sizeof(vertex_t));

	render::device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	render::device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}