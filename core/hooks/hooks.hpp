#pragma once
#include "../../dependencies/interfaces/c_view_setup.h"
template<class T> struct CUtlReference {
	CUtlReference* m_pNext;
	CUtlReference* m_pPrev;
	T* m_pObject;
};
template<class T> struct CUtlIntrusiveList {
	T* m_pHead;
};
template<class T> struct CUtlIntrusiveDList : public CUtlIntrusiveList<T> { };
template<class T> struct CUtlReferenceList : public CUtlIntrusiveDList< CUtlReference<T> > { };

enum EAttributeDataType {
	ATTRDATATYPE_NONE = -1,
	ATTRDATATYPE_FLOAT = 0,
	ATTRDATATYPE_4V,
	ATTRDATATYPE_INT,
	ATTRDATATYPE_POINTER,

	ATTRDATATYPE_COUNT,
};

#define MAX_PARTICLE_ATTRIBUTES 24

#define DEFPARTICLE_ATTRIBUTE( name, bit, datatype )			\
	const int PARTICLE_ATTRIBUTE_##name##_MASK = (1 << bit);	\
	const int PARTICLE_ATTRIBUTE_##name = bit;					\
	const EAttributeDataType PARTICLE_ATTRIBUTE_##name##_DATATYPE = datatype;

DEFPARTICLE_ATTRIBUTE(TINT_RGB, 6, ATTRDATATYPE_4V);

DEFPARTICLE_ATTRIBUTE(ALPHA, 7, ATTRDATATYPE_FLOAT);
struct CUtlString_simple {
	char* buffer;
	int capacity;
	int grow_size;
	int length;
};

class CParticleSystemDefinition {
	BYTE pad_0[308];
public:
	CUtlString_simple m_Name;
};
struct CParticleAttributeAddressTable {
	float* m_pAttributes[MAX_PARTICLE_ATTRIBUTES];
	size_t m_nFloatStrides[MAX_PARTICLE_ATTRIBUTES];

	FORCEINLINE float* FloatAttributePtr(int nAttribute, int nParticleNumber) const {
		int block_ofs = nParticleNumber / 4;
		return m_pAttributes[nAttribute] +
			m_nFloatStrides[nAttribute] * block_ofs +
			(nParticleNumber & 3);
	}

};
class CParticleCollection {
	BYTE pad_0[48];//0
public:
	int m_nActiveParticles;//48
private:
	BYTE pad_1[12];//52
public:
	CUtlReference<CParticleSystemDefinition> m_pDef;//64
private:
	BYTE pad_2[60];//80
public:
	CParticleCollection* m_pParent;//136
private:
	BYTE pad_3[84];//140
public:
	CParticleAttributeAddressTable m_ParticleAttributes;//224
};
template <typename T>
static auto Rel2Abs(const T& address) noexcept {
	return address + 4 + *reinterpret_cast<int*>(address);
}



template <typename T = void*>
constexpr T vfunc(void* thisptr, std::size_t idx) {
	return (*static_cast<T**>(thisptr))[idx];
}


class WeaponVisualData_t {
public:
	char padding1[0x77C];
	char weapon_path[260];
	char padding2[0x10C];

	float color1_r;
	float color1_g;
	float color1_b;
	float color2_r;
	float color2_g;
	float color2_b;
	float color3_r;
	float color3_g;
	float color3_b;
	float color4_r;
	float color4_g;
	float color4_b;

	int nPhongAlbedoBoost;
	int nPhongExponent;
	int nPhongIntensity;

	float flPhongAlbedoFactor;

	float flWearProgress;

	float flPatternScale;
	float flPatternOffsetX;
	float flPatternOffsetY;
	float flPatternRot;

	float flWearScale;
	float flWearOffsetX;
	float flWearOffsetY;
	float flWearRot;

	float flGrungeScale;
	float flGrungeOffsetX;
	float flGrungeOffsetY;
	float flGrungeRot;
};


namespace hooks {
	bool initialize();
	void release();
	inline bool disable = false;
	inline bool fully_init = false;
	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }


	inline bool present_hooked = false;
	inline int hooked = 0;
	namespace wnd_proc {
		using fn = WNDPROC;
		long __stdcall hook(HWND window, UINT msg, WPARAM wparm, LPARAM lparm);
	}

	namespace reset {
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
	}

	namespace present {
		using fn = long(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
		long __stdcall hook(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window_override, const RGNDATA* dirty_region);
	}

	namespace get_color_modulation {
		using fn = void(__thiscall*)(void*, float*, float*, float*);
		static void __fastcall hook(void* ecx, void* edx, float* r, float* g, float* b);
	}

	namespace is_using_static_prop_debug_modes {
		using fn = bool(__stdcall*)();
		static bool __stdcall hook();
	}

	namespace endscene {
		using fn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*);
		HRESULT __stdcall hook(IDirect3DDevice9* device);
	}

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	};

	namespace retaddr {
		using fn = bool(__fastcall*)(void*, void*, const char*);
		bool __fastcall hook(void* ecx, void* rcx, const char* m_modulename);
	}

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace send_datagram {
		using fn = int(__fastcall*)(i_net_channel_t*, int, bf_write*);
		int __fastcall hook(i_net_channel_t* net_channel, int edx, bf_write* datagram);
	}

	namespace hk_particle_collection_simulate {
		using fn = void(__fastcall*)(CParticleCollection*, void*);
		void __fastcall hook(CParticleCollection* thisPtr, void* edx);
	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		void __stdcall hook(int frame_stage);
	}

	namespace do_post_screen_effects {
		using fn = int(__thiscall*)(void*, c_view_setup&);
		static int __stdcall hook(c_view_setup& value);
	};

	namespace lock_cursor {
		using fn = void(__thiscall*)(void*);
		void __stdcall hook();
	}

	namespace send_net_message {
		using fn = bool(__fastcall*)(i_net_channel*, void*, INetMessage&, bool, bool);
		bool __fastcall hook(i_net_channel* this_ptr, void* edx, INetMessage& msg, bool force_reliable, bool voice);
	}

	namespace dme {
		using fn = void(__thiscall*)(iv_model_render*, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		static void __stdcall hook(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
	}

	namespace emit_sound {
		using fn = void(__fastcall*)(void*, void*, void*, int, int, const char*, unsigned int, const char*, float, float, int, int, int, const vec3_t*, const vec3_t*, vec3_t*, bool, float, int, void*);
		void __fastcall hook(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry,
			unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch,
			const vec3_t* pOrigin, const vec3_t* pDirection, vec3_t* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* fds);
	}

	namespace override_mouse_input {
		using fn = void(__fastcall*)(void*, void*, float*, float*);
		void __fastcall hook(void* ecx_, void* edx_, float* x, float* y);
	};

	namespace crc_server_check {
		using fn = void(__thiscall*)(void*, void*);
		void __fastcall hook(void*, void*);
	}

	namespace file_system {
		using fn = int(__fastcall*)(void*, void*);
		int __fastcall hook(void* ecx, void* edx);
	}

	namespace draw_text_fields {
		using fn = void(__fastcall*)(void*, void*, const wchar_t*, int, void*);
		void __fastcall hook(void* ecx, void* edx, const wchar_t* text, int text_length, void* draw_type);
	}

	typedef struct {
		unsigned short msgbytes[17]; // source engine: [INetChannelInfo::TOTAL+1]
		int				sampleY;
		int				sampleHeight;

	} netbandwidthgraph_t;

	typedef struct {
		float		cmd_lerp;
		int			size;
		bool		sent;
	} cmdinfo_t;

	namespace DrawTextFields {
		using fn = void(__thiscall*)(void*, void*, int, int, int, void*, void*);
		void __fastcall hook(void* thisptr, void* edx, int x, int y, int w, void* graph, void* cmdinfo);
	}

	namespace LooseFileAllowed {
		using fn = bool(__fastcall*)(void*, void*);
		bool __fastcall hook(void* edx, void* ecx);
	}

	namespace scene_end {
		using fn = void(__thiscall*)(void*);
		static void __stdcall hook();
	}

	namespace Render2dEffectsPreHud {
		using fn = void(__stdcall*)(void*);
		void __stdcall hook(void* viewSetup);
	}

	namespace set_visuals_data {
		using fn = void(__fastcall*)(void*, void*, const char*);
		void __fastcall set_visuals_data(void* ecx, void* edx, const char* shader_name);
	}

	namespace send_net_long {
		using fn = int(__fastcall*)(void*, DWORD*, int, int);
		int __fastcall hook(void* netchannel, DWORD* socket, int bytes_left, int max_routable_table);
	}

	namespace supress_lists {
		using fn = void(__fastcall*)(void*, int, int, bool);
		void __fastcall hook(void* _this, int, int a2, bool a3);
	}

	namespace set_draw_color {
		using fn = void(__stdcall*)(int, int, int, int);
		void __stdcall hook(int r, int g, int b, int a);
	}

	namespace find_or_add_filename {
		using fn = void(__fastcall*)(void*, char const*);
		void __fastcall hook(void* self, int edx, char const* file_name);
	}

	namespace render_smoke_overlay {
		using fn = void(__fastcall*)(void*, void*, bool);
		void __fastcall hook(void* thisptr, void* edx, bool pre_viewmodel);
	}

	namespace draw_viewmodels {
		using fn = void(__fastcall*)(void*, void*, c_view_setup&, bool, bool);
		static void __fastcall hook(void* thisptr, void* edx, c_view_setup& view, bool draw_view_model, bool draw_scope_lens_mask);
	}
}
