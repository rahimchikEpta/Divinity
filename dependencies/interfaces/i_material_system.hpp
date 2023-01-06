#pragma once
#include "../../source-sdk/structs/materials.hpp"
#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#define MAXSTUDIOSKINS		32

// These are given to FindMaterial to reference the texture groups that Show up on the
#define TEXTURE_GROUP_LIGHTMAP				"Lightmaps"
#define TEXTURE_GROUP_WORLD				"World textures"
#define TEXTURE_GROUP_MODEL				"Model textures"
#define TEXTURE_GROUP_VGUI				"VGUI textures"
#define TEXTURE_GROUP_PARTICLE				"Particle textures"
#define TEXTURE_GROUP_DECAL				"Decal textures"
#define TEXTURE_GROUP_SKYBOX				"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS			"ClientEffect textures"
#define TEXTURE_GROUP_OTHER				"Other textures"
#define TEXTURE_GROUP_PRECACHED				"Precached"
#define TEXTURE_GROUP_CUBE_MAP				"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET			"RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED			"Unaccounted textures"
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER		"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER		"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER		"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER			"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL			"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS			"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS			"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE		"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS			"Morph Targets"

class	i_ms_mat;
class	i_ms_mesh;
class	i_ms_vertex_buffer;
class	i_ms_index_buffer;
struct	i_ms_system_config_t;
class	i_ms_v_matrix;
class	matrix3x4_t;
class	i_ms_texture;
struct	i_ms_hwid_t;
class	i_ms_key_values;
class	i_ms_shader;
class	i_ms_vtx_texture;
class	i_ms_morph;
class	i_mat_render_ctx;
class	i_ms_call_queue;
struct	i_ms_morph_weight_t;
class	i_ms_file_list;
struct	i_ms_vtx_stream_spec_t;
struct	i_ms_shader_stencil_state_t;
struct	i_ms_mesh_instance_data_t;
class	i_ms_client_mat_sys;
class	i_ms_paint_mat;
class	i_ms_paint_map_data_mgr;
class	i_ms_paint_map_texture_mgr;
class	i_ms_gpu_mem_stats;
struct	i_ms_aspect_ratio_info_t;
struct	i_ms_cascaded_shadow_mapping_state_t;

class	i_ms_proxy_factory;
class	i_ms_texture;
class	i_ms_sys_hardware_cfg;
class	i_ms_shadow_mgr;

enum i_ms_compiled_vtf_flags {
	TEXTURE_FLAGS_POINT_SAMPLE = 1 << 0,
	TEXTURE_FLAGS_TRILINEAR = 1 << 1,
	TEXTURE_FLAGS_CLAMPS = 1 << 2,
	TEXTURE_FLAGS_CLAMPT = 1 << 3,
	TEXTURE_FLAGS_ANISOTROPIC = 1 << 4,
	TEXTURE_FLAGS_HINT_DXT5 = 1 << 5,
	TEXTURE_FLAGS_PWL_CORRECTED = 1 << 6,
	TEXTURE_FLAGS_NORMAL = 1 << 7,
	TEXTURE_FLAGS_NO_MIP = 1 << 8,
	TEXTURE_FLAGS_NO_LOD = 1 << 9,
	TEXTURE_FLAGS_ALL_MIPS = 1 << 10,
	TEXTURE_FLAGS_PROCEDURAL = 1 << 11,
	TEXTURE_FLAGS_ONE_BIT_ALPHA = 1 << 12,
	TEXTURE_FLAGS_EIGHT_BIT_ALPHA = 1 << 13,
	TEXTURE_FLAGS_ENVMAP = 1 << 14,
	TEXTURE_FLAGS_RENDER_TARGET = 1 << 15,
	TEXTURE_FLAGS_DEPTH_RENDER_TARGET = 1 << 16,
	TEXTURE_FLAGS_NO_DEBUG_OVERRIDE = 1 << 17,
	TEXTURE_FLAGS_SINGLE_COPY = 1 << 18,
	TEXTURE_FLAGS_PRE_SRGB = 1 << 19,
	TEXTURE_FLAGS_UNUSED_0x001 = 1 << 20,
	TEXTURE_FLAGS_UNUSED_0x002 = 1 << 21,
	TEXTURE_FLAGS_UNUSED_0x004 = 1 << 22,
	TEXTURE_FLAGS_NO_DEPTH_BUFFER = 1 << 23,
	TEXTURE_FLAGS_UNUSED_0x01 = 1 << 24,
	TEXTURE_FLAGS_CLAMPU = 1 << 25,
	TEXTURE_FLAGS_VERTEX_TEXTURE = 1 << 26,
	TEXTURE_FLAGS_SSBUMP = 1 << 27,
	TEXTURE_FLAGS_UNUSED_0x1 = 1 << 28,
	TEXTURE_FLAGS_BORDER = 1 << 29,
	TEXTURE_FLAGS_UNUSED_0x4 = 1 << 30,
	TEXTURE_FLAGS_UNUSED_0x8 = 1 << 31
};

enum i_ms_standard_lightmap_t {
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE = -1,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE_BUMP = -2,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_USER_DEFINED = -3
};

struct i_ms_sort_info_t {
	i_ms_mat* material;
	int		lightmap_page_id;
};

enum i_matsys_material_thread_mode_t {
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

enum i_ms_material_ctx_type_t {
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};

enum {
	MATERIAL_ADAPTER_NAME_LENGTH = 1 << 9
};

struct i_ms_material_texture_info_t {
	int exclude_information;
};

struct i_ms_app_perf_counters_info_t {
	float ms_main;
	float ms_mst;
	float ms_gpu;
	float ms_flip;
	float ms_total;
};

struct i_ms_app_instant_counters_info_t {
	uint32_t cpu_activity_mask;
	uint32_t deferred_words_allocated;
};

struct i_matsys_material_adapter_info_t {
	char		driver_name[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int	vendor_id;
	unsigned int	device_id;
	unsigned int	sub_sys_id;
	unsigned int	revision;
	int		dx_support_level;
	int		min_dx_support_level;
	int		max_dx_support_level;
	unsigned int	driver_version_high;
	unsigned int	driver_version_low;
};

struct i_ms_mat_video_mode_t {
	int		width;
	int		height;
	image_format 	format;
	int		refresh_rate;
};

enum i_ms_hdr_type_t {
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT,
};

enum i_ms_res_change_flags_t {
	MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED = 1,
	MATERIAL_RESTORE_RELEASE_MANAGED_RESOURCES
};

enum i_ms_render_target_size_mode_t {
	RT_SIZE_NO_CHANGE,
	RT_SIZE_DEFAULT,
	RT_SIZE_PICMIP,
	RT_SIZE_HDR,
	RT_SIZE_FULL_FRAME_BUFFER,
	RT_SIZE_OFFSCREEN,
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP
};

enum i_ms_mat_render_target_depth_t {
	MATERIAL_RT_DEPTH_SHARED,
	MATERIAL_RT_DEPTH_SEPARATE,
	MATERIAL_RT_DEPTH_NONE,
	MATERIAL_RT_DEPTH_ONLY
};

typedef void(*mat_buffer_release_func_t)(int flags);
typedef void(*mat_buffer_restore_func_t)(int flags);
typedef void(*mode_change_callback_function_t)(void);
typedef void(*end_frame_cleanup_function_t)(void);
typedef bool(*end_frame_prior_to_next_ctx_function_t)(void);
typedef void(*on_level_shutdown_function_t)(void* data);

typedef unsigned short mat_handle_t;
DECLARE_POINTER_HANDLE(mat_lock_t);

class i_mat_context;
class i_material_system {
public:
	i_material* find_material(char const* material_name, const char* group_name, bool complain = true, const char* complain_prefix = 0) {
		using fn = i_material * (__thiscall*)(i_material_system*, char const*, const char*, bool, const char*);
		return (*(fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
	}
	i_material_var* FindMaterial(char const* material_name, const char* group_name, bool complain = true, const char* complain_prefix = 0)
	{
		using fn = i_material_var * (__thiscall*)(i_material_system*, char const*, const char*, bool, const char*);
		return (*(fn**)this)[84](this, material_name, group_name, complain, complain_prefix);
	}
	material_handle_t first_material() {
		using fn = material_handle_t(__thiscall*)(i_material_system*);
		return (*(fn**)this)[86](this);
	}
	material_handle_t next_material(material_handle_t handle) {
		using fn = material_handle_t(__thiscall*)(i_material_system*, material_handle_t);
		return (*(fn**)this)[87](this, handle);
	}
	material_handle_t invalid_material_handle() {
		using fn = material_handle_t(__thiscall*)(i_material_system*);
		return (*(fn**)this)[88](this);
	}
	i_material* get_material(material_handle_t handle) {
		using fn = i_material * (__thiscall*)(i_material_system*, material_handle_t);
		return (*(fn**)this)[89](this, handle);
	}
	int	get_materials_count() {
		using fn = int(__thiscall*)(i_material_system*);
		return (*(fn**)this)[90](this);
	}

	i_texture* find_texture(char const* szTextureName, const char* szTextureGroupName, bool bComplain = true, int nAdditionalCreationFlags = 0)
	{
		using fn = i_texture * (__thiscall*)(i_material_system*, char const*, const char*, bool, int);
		return (*(fn**)this)[91](this, szTextureName, szTextureGroupName, bComplain, nAdditionalCreationFlags);
	}
	i_mat_context* get_render_context()
	{
		using fn = i_mat_context * (__thiscall*)(i_material_system*);
		return (*(fn**)this)[115](this);
	}
	i_material* create_material(const char* material_name, c_key_values* kv) {
		return utilities::call_virtual< i_material* (__thiscall*)(decltype(this), const char*, c_key_values*) >(this, 83)(
			this, material_name, kv);
	}
};

class i_material_unknown
{
public:
	typedef unsigned long four_cc;

public:
	virtual i_texture* get_texture_value(void) = 0;
	virtual bool is_texture_value_internal_env_cubemap(void) const = 0;
	virtual char const* get_name(void) const = 0;
	virtual void pad() const = 0;
	virtual void set_float_value(float val) = 0;
	virtual void set_int_value(int val) = 0;
	virtual void set_string_value(char const* val) = 0;
	virtual char const* get_string_value(void) const = 0;
	virtual void set_four_cc_value(four_cc type, void* pData) = 0;
	virtual void get_four_cc_value(four_cc* type, void** ppData) = 0;
	virtual void set_vec_value(float const* val, int numcomps) = 0;
	virtual void set_vec_value(float x, float y) = 0;
	virtual void set_vec_value(float x, float y, float z) = 0;
	virtual void set_vec_value(float x, float y, float z, float w) = 0;
	virtual void get_linear_vec_value(float* val, int numcomps) const = 0;
	virtual void set_texture_value(i_texture*) = 0;
	virtual i_material_var* get_material_value(void) = 0;
	virtual void set_material_value(i_material_var*) = 0;
	virtual bool is_defined() const = 0;
	virtual void set_undefined() = 0;
	virtual void set_matrix_value(matrix_t const& matrix) = 0;
	virtual const matrix_t& get_matrix_value() = 0;
	virtual bool matrix_is_identity() const = 0;
	virtual void copy_from(i_material_unknown* pMaterialVar) = 0;
	virtual void set_value_autodetect_type(char const* val) = 0;
	virtual i_material_var* get_owning_material() = 0;
	virtual void set_vec_component_value(float fVal, int nComponent) = 0;

protected:
	virtual int get_int_value_internal(void) const = 0;
	virtual float get_float_value_internal(void) const = 0;
	virtual float const* get_vec_value_internal() const = 0;
	virtual void get_vec_value_internal(float* val, int numcomps) const = 0;
	virtual int vector_size_internal() const = 0;
}; enum PREVIEW_IMAGE_RET_VAL_Tt
{
	material_preview_image_badt = 0,
	material_preview_image_okt,
	material_no_preview_imaget,
};
class i_material_var
{
public:
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;
	virtual PREVIEW_IMAGE_RET_VAL_Tt GetPreviewImageProperties(int* width, int* height, void* image_format, bool* is_translucent) const = 0;
	virtual PREVIEW_IMAGE_RET_VAL_Tt GetPreviewImage(unsigned char* data, int width, int height, void* image_format) const = 0;
	virtual int GetMappingWidth() = 0;
	virtual int GetMappingHeight() = 0;
	virtual int GetNumAnimationFrames() = 0;
	virtual bool InMaterialPage(void) = 0;
	virtual void GetMaterialOffset(float* offset) = 0;
	virtual void GetMaterialScale(float* scale) = 0;
	virtual i_material_system* GetMaterialPage(void) = 0;
	virtual i_material_unknown* FindVar(const char* var_name, bool* found, bool complain = true) = 0;
	virtual void IncrementReferenceCount(void) = 0;
	virtual void DecrementReferenceCount(void) = 0;
	virtual int GetEnumerationId(void) const = 0;

private:
	virtual void pad00() = 0; // get_low_res_color_sample(float s, float t, float* color) const = 0;
	virtual void pad01() = 0; // recompute_state_snapshots() = 0;
public:
	virtual bool IsTranslucent() = 0;

private:
	virtual bool pad02() = 0; // is_alpha_tested() = 0;
public:
	virtual bool IsVertexLit() = 0;
	virtual void GetVertexFormat() const = 0;
	virtual bool HasProxy(void) const = 0;

private:
	virtual bool pad03() = 0; // uses_env_cubemap(void) = 0;
	virtual bool pad04() = 0; // needs_tangent_space(void) = 0;
	virtual bool pad05() = 0; // needs_power_of_two_frame_buffer_texture(bool check_specific_to_this_frame = true) = 0;
	virtual bool pad06() = 0; // needs_full_frame_buffer_texture(bool check_specific_to_this_frame = true) = 0;
	virtual bool pad07() = 0; // needs_software_skinning(void) = 0;
public:
	virtual void AlphaModulate(float alpha) = 0;
	virtual void ColorModulate(float r, float g, float b) = 0;
	virtual void SetMaterialFlag(material_var_flags_t flag, bool on) = 0;
	virtual bool GetMaterialFlag(material_var_flags_t flag) const = 0;
	virtual void GetReflectivity(vec3_t& reflect) = 0;
	virtual bool GetPropertyFlag(void* type) = 0;
	virtual bool IsTwoSided() = 0;
	virtual void SetShader(const char* shader_name) = 0;
	virtual int GetNumPasses(void) = 0;
	virtual int get_texture_memory_bytes(void) = 0;
	virtual void Refresh() = 0;

private:
	virtual bool pad08() = 0; // bool needs_lightmap_blend_alpha(void) = 0;
	virtual bool pad09() = 0; // bool needs_software_lighting(void) = 0;
	virtual bool pad10() = 0; // int shader_param_count() const = 0;
	virtual bool pad11() = 0; // IMaterialVar** get_shader_params(void) = 0;
public:
	virtual bool IsErrorMaterial() const = 0;

private:
	virtual void pad12() = 0;

public:
	virtual float GetAlphaModulation() = 0;
	virtual void GetColorModulation(float* r, float* g, float* b) = 0;
	virtual bool IsTransluscentUnderModulation(float alpha_modulation = 1.0f) const = 0;
	virtual i_material_var* FindVarFast(char const* var_name, unsigned int* token) = 0;
	virtual void SetShaderParams(void* key_values) = 0;
	virtual const char* GetShaderName() const = 0;

private:
	virtual bool pad13() = 0; // void delete_if_unreferenced() = 0;
	virtual bool pad14() = 0; // bool is_sprite_card() = 0;
	virtual bool pad15() = 0; // void call_bind_proxy(void* proxy_data) = 0;
	virtual bool pad16() = 0; // void refresh_preserving_material_vars() = 0;
	virtual bool pad17() = 0; // bool was_reloaded_from_whitelist() = 0;
	virtual bool pad18() = 0; // bool set_temp_excluded(bool set, int excluded_dimension_limit) = 0;
public:
	virtual int GetReferenceCount() const = 0;

public:
	void set_float_value(float value) {
		return utilities::call_virtual<void(__thiscall*)(void*, float)>(this, 4)(this, value);
	}

	void set_int_value(float value) {
		return utilities::call_virtual<void(__thiscall*)(void*, float)>(this, 5)(this, value);
	}
};

class i_mat_context
{
public:
	virtual void				BeginRender() = 0;
	virtual void				EndRender() = 0;

	void get_render_target_dimensions(int* width, int* height)
	{
		utilities::v<void(__thiscall*)(void*, int*, int*)>(this, 8)(this, width, height);
	}
	void copy_render_target_to_texture_ex(i_texture* texture, int unk, vec4* dst, vec4* src)
	{
		utilities::v<void(__thiscall*)(void*, i_texture*, int, vec4*, vec4*)>(this, 122)(this, texture, unk, dst, src);
	}
	void set_frame_buffer_copy_texture(i_texture* texture, int texture_index)
	{
		utilities::v<void(__thiscall*)(void*, i_texture*, int)>(this, 20)(this, texture, texture_index);
	}

	void get_viewport(int* x, int* y, int* width, int* height)
	{
		utilities::v< void(__thiscall*)(void*, int*, int*, int*, int*) >(this, 41)(this, x, y, width, height);
	}

	void draw_screen_space_rectangle(i_material_var* pMaterial,
		int dest_x, int dest_y,
		int width, int height,
		float texture_x0, float texture_y0,
		float texture_x1, float texture_y1,
		int texture_width, int texture_height,
		void* client_renderable = nullptr, int x_dice = 1,
		int y_dice = 1) {
		utilities::v<void(__thiscall*)(void*,
			i_material_var*,
			int, int,
			int, int,
			float, float,
			float, float,
			int, int,
			void*, int,
			int)>(this, 114)(this, pMaterial, dest_x, dest_y, width, height, texture_x0, texture_y0, texture_x1, texture_y1, texture_width, texture_height, client_renderable, x_dice, y_dice);
	}
};
