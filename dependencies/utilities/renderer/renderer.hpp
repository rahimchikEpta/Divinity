#include "../../interfaces/interfaces.hpp"
#include <d3d9.h>
#pragma comment (lib, "d3dx9")
#include <d3dx9.h>

namespace render {
	extern IDirect3DDevice9* device;
	extern IDirect3DVertexBuffer9* vertex_buffer;
	extern IDirect3DIndexBuffer9* index_buffer;
	void reset();
	bool do_init(IDirect3DDevice9* device);
	void set_states();
	void line(const vec2_t& start, const vec2_t& end, const color& col);
	void gradient_rectangle(const vec2_t& pos, const vec2_t& size, const color& col, const color& oth, const bool vertical);
	void filled_rectangle(const vec2_t& pos, const vec2_t& size, const color& col);
	void filled_circle(const vec2_t& pos, int radius, int segments, const color& col);
	void rectangle(const vec2_t& pos, const vec2_t& size, const color& col);
	void dispatch_vertex_list(vertex_t* vertex_list, int size, D3DPRIMITIVETYPE primitive);
	vec2_t text_position(const std::string& txt, ID3DXFont* font);
	void text(const vec2_t& pos, const color& col, const std::string& txt, ID3DXFont* font, bool centered);
	void text_shadow(const vec2_t& pos, const color& col, const std::string& txt, ID3DXFont* font, const color& shadowcol, bool centered);
	void text_outline(const vec2_t& pos, const color& col, const std::string& txt, ID3DXFont* font, const color& outcol, bool centered);
	D3DVIEWPORT9 get_viewport();
	void set_viewport(D3DVIEWPORT9 vp);
	D3DVIEWPORT9 start_clipping(const vec2_t& pos, const vec2_t& size);
	void end_clipping(D3DVIEWPORT9 vp);
	bool screen_transform(vec3_t& screen, vec3_t& origin);
	bool world_to_screen(vec3_t& origin, vec3_t& screen);

	void color_convert_hsv_to_rgb(float h, float s, float v, float& out_r, float& out_g, float& out_b);

	void draw_circle_3d(const vec3_t& origin, float radius, color col);

	void draw_3d_cube(float scalar, vec3_t angles, vec3_t middle_origin, color col);

	void triangle_grad_filled(vec2_t pos1, vec2_t pos2, vec2_t pos3, color col1, color col2, color col3);

	namespace fonts {
		extern ID3DXFont* name;
		extern ID3DXFont* flags;
		extern ID3DXFont* key_strokes;
	}
}