#pragma once
#include "../../dependencies/math/math.hpp"
#include <array>
#include "collideable.hpp"
#include "client_class.hpp"
#include "../structs/animstate.hpp"
#include "../../dependencies/utilities/netvars/netvars.hpp"

enum data_update_type_t {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum cs_weapon_type {
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum client_frame_stage_t {
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

enum move_type {
	movetype_none = 0,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
	movetype_max_bits = 4,
	max_movetype
};
#define invalid_move_types { movetype_noclip, movetype_ladder, movetype_fly, movetype_flygravity }
enum entity_flags {
	fl_onground = (1 << 0),
	fl_ducking = (1 << 1),
	fl_waterjump = (1 << 2),
	fl_ontrain = (1 << 3),
	fl_inrain = (1 << 4),
	fl_frozen = (1 << 5),
	fl_atcontrols = (1 << 6),
	fl_client = (1 << 7),
	fl_fakeclient = (1 << 8),
	fl_inwater = (1 << 9),
	fl_fly = (1 << 10),
	fl_swim = (1 << 11),
	fl_conveyor = (1 << 12),
	fl_npc = (1 << 13),
	fl_godmode = (1 << 14),
	fl_notarget = (1 << 15),
	fl_aimtarget = (1 << 16),
	fl_partialground = (1 << 17),
	fl_staticprop = (1 << 18),
	fl_graphed = (1 << 19),
	fl_grenade = (1 << 20),
	fl_stepmovement = (1 << 21),
	fl_donttouch = (1 << 22),
	fl_basevelocity = (1 << 23),
	fl_worldbrush = (1 << 24),
	fl_object = (1 << 25),
	fl_killme = (1 << 26),
	fl_onfire = (1 << 27),
	fl_dissolving = (1 << 28),
	fl_transragdoll = (1 << 29),
	fl_unblockable_by_player = (1 << 30)
};
enum item_definitiones {
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_CORD,
	WEAPON_KNIFE_CANIS,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_OUTDOOR,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_SKELETON = 525,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

class entity_t {

public:
	void* animating() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x4);
	}
	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	collideable_t* collideable() {
		using original_fn = collideable_t * (__thiscall*)(void*);
		return (*(original_fn**)this)[3](this);
	}
	c_client_class* client_class() {
		using original_fn = c_client_class * (__thiscall*)(void*);
		return (*(original_fn**)networkable())[2](networkable());
	}

	int index() {
		if (!this)
			return 0;
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[10](networkable());
	}
	bool is_player() {
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[157](this);
	}
	bool is_weapon() {
		if (!this)
			return false;
		using original_fn = bool(__thiscall*)(entity_t*);
		return (*(original_fn**)this)[166](this);
	}
	bool setup_bones(matrix_t* out, int max_bones, int mask, float time) {
		if (!this)
			return false;

		using original_fn = bool(__thiscall*)(void*, matrix_t*, int, int, float);
		return (*(original_fn**)animating())[13](animating(), out, max_bones, mask, time);
	}
	model_t* model() {
		using original_fn = model_t * (__thiscall*)(void*);
		return (*(original_fn**)animating())[8](animating());
	}
	void update() {
		using original_fn = void(__thiscall*)(entity_t*);
		(*(original_fn**)this)[218](this);
	}
	int draw_model(int flags, uint8_t alpha) {
		using original_fn = int(__thiscall*)(void*, int, uint8_t);
		return (*(original_fn**)animating())[9](animating(), flags, alpha);
	}
	void set_angles(vec3_t angles) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_angles_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		set_angles_fn(this, angles);
	}
	void set_position(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}

	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}

	matrix_t& coordinate_frame() {
		static auto _coordinate_frame = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_CollisionGroup")) - 0x30;
		return *(matrix_t*)((uintptr_t)this + _coordinate_frame);
	}
	void set_destroyed_on_recreate_entities() {
		return utilities::call_virtual< void(__thiscall*)(void*) >(networkable(), 13)(networkable());
	}
	void notify_should_transmit(int state) {
		return utilities::call_virtual< void(__thiscall*)(void*, int) >(networkable(), 3)(networkable(), state);
	}

	void on_pre_data_changed(int update_type) {
		return utilities::call_virtual< void(__thiscall*)(void*, int) >(networkable(), 4)(networkable(), update_type);
	}

	void on_data_changed(int update_type) {
		return utilities::call_virtual< void(__thiscall*)(void*, int) >(networkable(), 5)(networkable(), update_type);
	}

	void pre_data_update(int update_type) {
		return utilities::call_virtual< void(__thiscall*)(void*, int) >(networkable(), 6)(networkable(), update_type);
	}

	void post_data_update(int update_type) {
		return utilities::call_virtual< void(__thiscall*)(void*, int) >(networkable(), 7)(networkable(), update_type);
	}
	void net_pre_data_update(int update_type) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}

	void net_release() {
		using original_fn = void(__thiscall*)(void*);
		return (*(original_fn**)networkable())[1](networkable());
	}

	int net_set_destroyed_on_recreate_entities() {
		using original_fn = int(__thiscall*)(void*);
		return (*(original_fn**)networkable())[13](networkable());
	}

	bool dormant() {
		using original_fn = bool(__thiscall*)(void*);
		return (*static_cast<original_fn**>(networkable()))[9](networkable());
	}

	NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
		NETVAR("DT_BaseEntity", "m_hOwnerEntity", owner_handle, unsigned long)
		NETVAR("DT_CSPlayer", "m_flSimulationTime", simulation_time, float)
		NETVAR("DT_BasePlayer", "m_vecOrigin", origin, vec3_t)
		NETVAR("DT_BasePlayer", "m_vecViewOffset[0]", view_offset, vec3_t)
		NETVAR("DT_CSPlayer", "m_iTeamNum", team, int)
		NETVAR("DT_BaseEntity", "m_bSpotted", spotted, bool)
		NETVAR("DT_CSPlayer", "m_nSurvivalTeam", survival_team, int)
		NETVAR("DT_CSPlayer", "m_flHealthShotBoostExpirationTime", health_boost_time, float)
};

class econ_view_item_t {
public:
	NETVAR("DT_ScriptCreatedItem", "m_bInitialized", is_initialized, bool)
		NETVAR("DT_ScriptCreatedItem", "m_iEntityLevel", entity_level, int)
		NETVAR("DT_ScriptCreatedItem", "m_iAccountID", account_id, int)
		NETVAR("DT_ScriptCreatedItem", "m_iItemIDLow", item_id_low, int)
};

class base_view_model_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseViewModel", "m_nViewModelIndex", view_model_index, int);
	NETVAR("DT_BaseViewModel", "m_hWeapon", m_hweapon, int);
	NETVAR("DT_BaseViewModel", "m_hOwner", m_howner, int);
};

class attributable_item_t : public entity_t {
public:
	NETVAR("DT_BaseViewModel", "m_nModelIndex", model_index, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow", oo_xuid_low, int);
	NETVAR("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh", oo_xuid_high, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackStatTrak", fb_stattrak, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackPaintKit", fb_paintkit, int);
	NETVAR("DT_BaseAttributableItem", "m_nFallbackSeed", fb_seed, int);
	NETVAR("DT_BaseAttributableItem", "m_iEntityQuality", entity_quality, int);
	NETVAR("DT_BaseAttributableItem", "m_flFallbackWear", fb_wear, std::float_t);
	NETVAR("DT_BaseCombatWeapon", "m_hWeaponWorldModel", world_model_handle, std::uint32_t);
	NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition, std::int16_t);
	NETVAR("DT_BaseAttributableItem", "m_iItemIDHigh", item_id_high, int); //jakby crash to wez offset z hazedumpera
	NETVAR("DT_BaseAttributableItem", "m_iAccountID", acc_id, int);
	//NETVAR( "DT_BaseAttributableItem", "m_Item",  m_Item2, econ_view_item_t);

	void set_model_index(const int index) {
		return utilities::call_virtual<void(__thiscall*)(entity_t*, int)>(this, 75)(this, index);
	}

	base_view_model_t* get_view_model() {
		return (base_view_model_t*)(DWORD)this;
	}

	econ_view_item_t& item() {
		return *(econ_view_item_t*)this;
	}
};

class weapon_t : public entity_t {
public:
	NETVAR("DT_BaseCombatWeapon", "m_flNextPrimaryAttack", next_primary_attack, float)
		NETVAR("DT_BaseCombatWeapon", "m_flNextSecondaryAttack", next_secondary_attack, float)
		NETVAR("DT_BaseCombatWeapon", "m_iClip1", clip1_count, int)
		NETVAR("DT_BaseCombatWeapon", "m_iClip2", clip2_count, int)
		NETVAR("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount", primary_reserve_ammo_acount, int)
		NETVAR("DT_WeaponCSBase", "m_flRecoilIndex", recoil_index, float)
		NETVAR("DT_WeaponCSBaseGun", "m_zoomLevel", zoom_level, float)
		NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short)
		NETVAR("DT_BaseCombatWeapon", "m_iEntityQuality", entity_quality, int)

		float inaccuracy() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[482](this);
	}

	float get_spread() {
		using original_fn = float(__thiscall*)(void*);
		return (*(original_fn**)this)[452](this);
	}

	void update_accuracy_penalty() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[483](this);
	}

	bool is_sniper() {
		return (this->item_definition_index() == WEAPON_AWP || this->item_definition_index() == WEAPON_SSG08 || this->item_definition_index() == WEAPON_G3SG1 || this->item_definition_index() == WEAPON_SCAR20);
	}
	bool is_scout() {
		return this->item_definition_index() == WEAPON_SSG08;
	}
	bool is_smg() {
		return (this->get_weapon_data()->weapon_type == cs_weapon_type::WEAPONTYPE_SUBMACHINEGUN);
	}
	bool is_heavy() {
		return (this->get_weapon_data()->weapon_type == cs_weapon_type::WEAPONTYPE_MACHINEGUN);
	}
	bool is_pistol() {
		return (this->get_weapon_data()->weapon_type == cs_weapon_type::WEAPONTYPE_PISTOL);
	}
	
	bool is_rifle() {
		return (this->get_weapon_data()->weapon_type == cs_weapon_type::WEAPONTYPE_RIFLE);
	}
	bool is_non_aim_weapon() {
		auto index = item_definition_index();

		return index == WEAPON_FISTS || index == WEAPON_HEALTHSHOT || index == WEAPON_SHIELD || index == WEAPON_TASER
			|| index == WEAPON_TABLET || index == WEAPON_HAMMER || index == WEAPON_SPANNER || index == WEAPON_BUMPMINE || index == WEAPON_AXE;
	}

	bool is_knife() {
		return (this->get_weapon_data()->weapon_type == cs_weapon_type::WEAPONTYPE_KNIFE);
	}
	bool is_full_auto() noexcept
	{
		const auto weaponData = this->get_weapon_data();
		if (weaponData)
			return weaponData->weapon_full_auto;
		return false;
	}

	
	bool is_nade() {
		return (this->get_weapon_data()->weapon_type == cs_weapon_type::WEAPONTYPE_GRENADE);
	}

	bool is_c4() {
		return (this->get_weapon_data()->weapon_type == cs_weapon_type::WEAPONTYPE_C4);
	}

	bool is_taser() {
		return (this->item_definition_index() == WEAPON_TASER);
	}

	weapon_info_t* get_weapon_data() {
		return interfaces::weapon_system->get_weapon_data(this->item_definition_index());
	}
	std::int32_t get_type() {
		if (!this) return WEAPONTYPE_UNKNOWN;
		std::int32_t id = this->item_definition_index();
		switch (id) {
		case WEAPON_DEAGLE:
		case WEAPON_P250:
		case WEAPON_USP_SILENCER:
		case WEAPON_HKP2000:
		case WEAPON_GLOCK:
		case WEAPON_FIVESEVEN:
		case WEAPON_TEC9:
		case WEAPON_ELITE:
		case WEAPON_REVOLVER:
		case WEAPON_CZ75A:
			return WEAPONTYPE_PISTOL;
			break;
		case WEAPON_MP9:
		case WEAPON_MP7:
		case WEAPON_MP5SD:
		case WEAPON_UMP45:
		case WEAPON_BIZON:
		case WEAPON_P90:
		case WEAPON_MAC10:
			return WEAPONTYPE_SUBMACHINEGUN;
			break;
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_CSS:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_M9_BAYONET:
		case WEAPON_KNIFE_TACTICAL:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_PUSH:
		case WEAPON_KNIFE_CORD:
		case WEAPON_KNIFE_CANIS:
		case WEAPON_KNIFE_URSUS:
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		case WEAPON_KNIFE_OUTDOOR:
		case WEAPON_KNIFE_STILETTO:
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
			return WEAPONTYPE_KNIFE;
			break;
		case WEAPON_SAWEDOFF:
		case WEAPON_XM1014:
		case WEAPON_MAG7:
		case WEAPON_NOVA:
		case WEAPON_M249:
		case WEAPON_NEGEV:
			return WEAPONTYPE_MACHINEGUN;
		case WEAPON_TASER:
		case WEAPON_C4:
			return WEAPONTYPE_C4;
			break;
		case WEAPON_HEGRENADE:
		case WEAPON_FLASHBANG:
		case WEAPON_DECOY:
		case WEAPON_SMOKEGRENADE:
		case WEAPON_INCGRENADE:
		case WEAPON_MOLOTOV:
			return WEAPONTYPE_GRENADE;
			break;
		case WEAPON_AK47:
		case WEAPON_M4A1:
		case WEAPON_M4A1_SILENCER:
		case WEAPON_GALILAR:
		case WEAPON_FAMAS:
		case WEAPON_AUG:
		case WEAPON_SG556:
			return WEAPONTYPE_RIFLE;
			break;
		case WEAPON_SCAR20:
		case WEAPON_G3SG1:
		case WEAPON_SSG08:
		case WEAPON_AWP:
			return WEAPONTYPE_SNIPER_RIFLE;
			break;
		default:
			return WEAPONTYPE_KNIFE;
		}
	}
	std::string weapon_name() {
		if (!this)
			return ("");
		int id = this->item_definition_index();
		switch (id) {
		case WEAPON_DEAGLE:
			return ("DEAGLE");
		case WEAPON_AUG:
			return ("AUG");
		case WEAPON_G3SG1:
			return ("G3SG1");
		case WEAPON_MAC10:
			return ("MAC10");
		case WEAPON_P90:
			return ("P90");
		case WEAPON_SSG08:
			return ("SSG08");
		case WEAPON_SCAR20:
			return ("SCAR20");
		case WEAPON_UMP45:
			return ("UMP45");
		case WEAPON_ELITE:
			return ("ELITE");
		case WEAPON_FAMAS:
			return ("FAMAS");
		case WEAPON_FIVESEVEN:
			return ("FIVESEVEN");
		case WEAPON_GALILAR:
			return ("GALILAR");
		case WEAPON_M4A1_SILENCER:
			return ("M4A1-S");
		case WEAPON_M4A1:
			return ("M4A4");
		case WEAPON_P250:
			return ("P250");
		case WEAPON_M249:
			return ("M249");
		case WEAPON_XM1014:
			return ("XM1014");
		case WEAPON_GLOCK:
			return ("GLOCK");
		case WEAPON_USP_SILENCER:
			return ("USP-S");
		case WEAPON_HKP2000:
			return ("P2000");
		case WEAPON_AK47:
			return ("AK47");
		case WEAPON_AWP:
			return ("AWP");
		case WEAPON_BIZON:
			return ("BIZON");
		case WEAPON_MAG7:
			return ("MAG7");
		case WEAPON_NEGEV:
			return ("NEGEV");
		case WEAPON_SAWEDOFF:
			return ("SAWEDOFF");
		case WEAPON_TEC9:
			return ("TEC9");
		case WEAPON_TASER:
			return ("ZEUS");
		case WEAPON_NOVA:
			return ("NOVA");
		case WEAPON_CZ75A:
			return ("CZ75");
		case WEAPON_SG556:
			return ("SG553");
		case WEAPON_REVOLVER:
			return ("REVOLVER");
		case WEAPON_MP7:
			return ("MP7");
		case WEAPON_MP9:
			return ("MP9");
		case WEAPON_MP5SD:  //same icon as ump
			return ("MP5");
		case WEAPON_C4:
			return ("C4");
		case WEAPON_FRAG_GRENADE:
			return ("GRENADE");
		case WEAPON_SMOKEGRENADE:
			return ("SMOKE");
		case WEAPON_MOLOTOV:
			return ("MOLOTOV");
		case WEAPON_INCGRENADE:
			return ("INCENDIARY");
		case WEAPON_FLASHBANG:
			return ("FLASHBANG");
		case WEAPON_DECOY:
			return ("DECOY");
		default:
			return ("KNIFE");
		}
		return ("");
	}
};
class c_key_values {
public:
	~c_key_values() {
		free(this);
	}

	void init(const char* key_name) {
		using InitKeyValues_t = void(__thiscall*)(void*, const char*);
		static InitKeyValues_t InitKeyValuesEx = nullptr;

		if (!InitKeyValuesEx)
			InitKeyValuesEx = *reinterpret_cast<InitKeyValues_t>(utilities::pattern_scan("client.dll", "55 8B EC 51 33 C0 C7 45"));

		InitKeyValuesEx(this, key_name);
	}

	void load_from_buffer(const char* resource, const char* buf, void* file = nullptr, const char* id = nullptr, void* eval_proc = nullptr, void* unk = nullptr) {
		using LoadFromBuffer_t = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
		static LoadFromBuffer_t LoadFromBufferEx = nullptr;

		if (!LoadFromBufferEx)
			LoadFromBufferEx = *reinterpret_cast<LoadFromBuffer_t>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"));

		LoadFromBufferEx(this, resource, buf, file, id, eval_proc, unk);
	}

	c_key_values* find_key(const char* name, bool create) {
		using findkey_t = c_key_values * (__thiscall*)(c_key_values*, const char*, bool);
		static auto FindKey = *reinterpret_cast<findkey_t>(utilities::pattern_scan("client.dll", "55 8B EC 83 EC 1C 53 8B D9 85 DB"));
		return FindKey(this, name, create);
	}

	void set_int(const char* key_name, std::int32_t val) {
		auto key_int = find_key(key_name, true);
		if (key_int) {
			*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint32_t>(key_int) + 0xC) = val;
			*reinterpret_cast<char*>(reinterpret_cast<std::uint32_t>(key_int) + 0x10) = 2;
		}
	}

	void set_string(const char* name, const char* a2) {
		auto key = find_key(name, true);
		using setstring_t = void(__thiscall*)(void*, const char*);
		if (key) {
			static auto SetString = *reinterpret_cast<setstring_t>(utilities::pattern_scan("client.dll", "55 8B EC A1 ? ? ? ? 53 56 57 8B F9 8B 08 8B 01"));
			SetString(key, a2);
		}
	}
};
class player_t : public entity_t {
public:
	// helper methods.
	template< typename t >
	__forceinline t& get(size_t offset) {
		return *(t*)((uintptr_t)this + offset);
	}

	template< typename t >
	__forceinline void set(size_t offset, const t& val) {
		*(t*)((uintptr_t)this + offset) = val;
	}

	template< typename t >
	__forceinline t as() {
		return (t)this;
	}

private:

	
	template <typename T>
	T& read(uintptr_t offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

	template <typename T>
	void write(uintptr_t offset, T data) {
		*reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset) = data;
	}

public:
	NETVAR("DT_BasePlayer", "m_hViewModel[0]", view_model, int)
		NETVAR("DT_CSPlayer", "m_bHasDefuser", has_defuser, bool)
		NETVAR("DT_CSPlayer", "m_bGunGameImmunity", has_gun_game_immunity, bool)
		NETVAR("DT_CSPlayer", "m_iShotsFired", shots_fired, int)
		NETVAR("DT_CSPlayer", "m_angEyeAngles", eye_angles, vec3_t)
		NETVAR("DT_CSPlayer", "m_ArmorValue", armor, int)
		NETVAR("DT_CSPlayer", "m_bHasHelmet", has_helmet, bool)
		NETVAR("DT_CSPlayer", "m_bIsScoped", is_scoped, bool)
		NETVAR("DT_CSPlayer", "m_bIsDefusing", is_defusing, bool)
		NETVAR("DT_CSPlayer", "m_iAccount", money, int)
		NETVAR("DT_CSPlayer", "m_bInBuyZone", in_buy_zone, bool)
		NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget", lower_body_yaw, float)
		NETVAR("DT_CSPlayer", "m_flNextAttack", next_attack, float)
		NETVAR("DT_CSPlayer", "m_flFlashDuration", flash_duration, float)
		NETVAR("DT_CSPlayer", "m_flFlashMaxAlpha", flash_alpha, float)
		NETVAR("DT_CSPlayer", "m_bHasNightVision", has_night_vision, bool)
		NETVAR("DT_CSPlayer", "m_bNightVisionOn", night_vision_on, bool)
		NETVAR("DT_CSPlayer", "m_iHealth", health, int)
		NETVAR("DT_CSPlayer", "m_lifeState", life_state, int)
		NETVAR("DT_CSPlayer", "m_fFlags", flags, int)
		NETVAR("DT_BasePlayer", "m_viewPunchAngle", punch_angle, vec3_t)
		NETVAR("DT_BasePlayer", "m_aimPunchAngle", aim_punch_angle, vec3_t)
		NETVAR("DT_BasePlayer", "m_vecVelocity[0]", velocity, vec3_t)
		NETVAR("DT_CSPlayer", "m_flStamina", stamina, float)
		NETVAR("DT_BasePlayer", "m_flMaxspeed", max_speed, float)
		NETVAR("DT_BaseEntity", "m_flShadowCastDistance", fov_time, float)
		NETVAR("DT_BasePlayer", "m_hObserverTarget", observer_target, unsigned long)
		NETVAR("DT_BasePlayer", "m_nHitboxSet", hitbox_set, int)
		NETVAR("DT_CSPlayer", "m_flDuckAmount", duck_amount, float)
		NETVAR("DT_CSPlayer", "m_bHasHeavyArmor", has_heavy_armor, bool)
		NETVAR("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", smoke_grenade_tick_begin, int)
		NETVAR("DT_CSPlayer", "m_nTickBase", get_tick_base, int)
		NETVAR("DT_BaseAttributableItem", "m_iItemDefinitionIndex", item_definition_index, short)
		NETVAR("DT_CSPlayer", "m_bWaitForNoAttack", wait_for_no_attack, bool)
		bool is_enemy() {
		auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));
		if (!local_player)return false;

		if (!this->is_alive())return false;

		if (!team())return false;
		if (local_player->team() != this->team())return true;

		static convar* game_type = nullptr;
		if (!game_type)game_type = interfaces::console->get_convar("game_type");
		game_type->flags &= ~fcvar_cheat;

		if (game_type->get_int() == 6) {
			if (!survival_team())return false;
			if (local_player->survival_team() == -1)return true;
			if (local_player->survival_team() != this->survival_team())return true;
		}

		return false;
	}
	weapon_t* active_weapon() {
		auto active_weapon = read<uintptr_t>(netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hActiveWeapon"))) & 0xFFF;
		return reinterpret_cast<weapon_t*>(interfaces::entity_list->get_client_entity(active_weapon));
	}
	std::string weapon_name() {
		if (!this)
			return ("");
		int id = this->item_definition_index();
		switch (id) {
		case WEAPON_DEAGLE:
			return ("DEAGLE");
		case WEAPON_AUG:
			return ("AUG");
		case WEAPON_G3SG1:
			return ("G3SG1");
		case WEAPON_MAC10:
			return ("MAC10");
		case WEAPON_P90:
			return ("P90");
		case WEAPON_SSG08:
			return ("SSG08");
		case WEAPON_SCAR20:
			return ("SCAR20");
		case WEAPON_UMP45:
			return ("UMP45");
		case WEAPON_ELITE:
			return ("ELITE");
		case WEAPON_FAMAS:
			return ("FAMAS");
		case WEAPON_FIVESEVEN:
			return ("FIVESEVEN");
		case WEAPON_GALILAR:
			return ("GALILAR");
		case WEAPON_M4A1_SILENCER:
			return ("M4A1-S");
		case WEAPON_M4A1:
			return ("M4A4");
		case WEAPON_P250:
			return ("P250");
		case WEAPON_M249:
			return ("M249");
		case WEAPON_XM1014:
			return ("XM1014");
		case WEAPON_GLOCK:
			return ("GLOCK");
		case WEAPON_USP_SILENCER:
			return ("USP-S");
		case WEAPON_HKP2000:
			return ("P2000");
		case WEAPON_AK47:
			return ("AK47");
		case WEAPON_AWP:
			return ("AWP");
		case WEAPON_BIZON:
			return ("BIZON");
		case WEAPON_MAG7:
			return ("MAG7");
		case WEAPON_NEGEV:
			return ("NEGEV");
		case WEAPON_SAWEDOFF:
			return ("SAWEDOFF");
		case WEAPON_TEC9:
			return ("TEC9");
		case WEAPON_TASER:
			return ("ZEUS");
		case WEAPON_NOVA:
			return ("NOVA");
		case WEAPON_CZ75A:
			return ("CZ75");
		case WEAPON_SG556:
			return ("SG553");
		case WEAPON_REVOLVER:
			return ("REVOLVER");
		case WEAPON_MP7:
			return ("MP7");
		case WEAPON_MP9:
			return ("MP9");
		case WEAPON_MP5SD:  //same icon as ump
			return ("MP5");
		case WEAPON_C4:
			return ("C4");
		case WEAPON_FRAG_GRENADE:
			return ("GRENADE");
		case WEAPON_SMOKEGRENADE:
			return ("SMOKE");
		case WEAPON_MOLOTOV:
			return ("MOLOTOV");
		case WEAPON_INCGRENADE:
			return ("INCENDIARY");
		case WEAPON_FLASHBANG:
			return ("FLASHBANG");
		case WEAPON_DECOY:
			return ("DECOY");
		default:
			return ("KNIFE");
		}
		return ("");
	}
	UINT* get_wearables() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWearables"))));
	}

	UINT* get_weapons() {
		return (UINT*)((uintptr_t)this + (netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_hMyWeapons"))));
	}
	bool PhysicsRunThink(int index) {
		using PhysicsRunThinkFn = bool(__thiscall*)(void*, int);
		static auto oPhysicsRunThink = (PhysicsRunThinkFn)utilities::pattern_scan("client.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87");
		return oPhysicsRunThink(this, index);
	}
	void PreThink() {
		return v<void(__thiscall*)(void*)>(this, 318)(this);
	}
	void Think() {
		return v<void(__thiscall*)(void*)>(this, 139)(this);
	}
	void PostThink() {
		return v<void(__thiscall*)(void*)>(this, 319)(this);
	}
	int button_disabled() {
		return *(int*)((std::uintptr_t)this + 0x3330);
	}
	int button_forced() {
		return *(int*)((std::uintptr_t)this + 0x3334);
	}
	vec3_t get_eye_pos() {
		if (!this)
			return vec3_t(0, 0, 0);
		return origin() + view_offset();
	}

	anim_state* get_anim_state() {
		return *reinterpret_cast<anim_state**>(this + 0x3914);
	}
	player_info_t get_player_info() {
		player_info_t info;
		interfaces::engine->get_player_info(this->index(), &info);
		return info;
	}
	std::string get_name(bool console_safe = false) {
		player_info_t pinfo = this->get_player_info();

		char* pl_name = pinfo.name;
		char buf[128];
		std::int32_t c = 0;

		for (std::int32_t i = 0; pl_name[i]; ++i) {
			if (c >= sizeof(buf) - 1)
				break;

			switch (pl_name[i]) {
			case '"': if (console_safe) break;
			case '\\':
			case ';': if (console_safe) break;
			case '\n':
				break;
			default:
				buf[c++] = pl_name[i];
			}
		}

		buf[c] = '\0';
		return std::string(buf);
	}
	bool can_see_player_pos(player_t* player, const vec3_t& pos) {
		if (!this)
			return false;
		trace_t tr;
		ray_t ray;
		trace_filter filter;
		filter.skip = this;

		auto start = get_eye_pos();
		auto dir = (pos - start).normalized();

		ray.init(start, pos);
		interfaces::trace_ray->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

		return tr.entity == player || tr.flFraction > 0.97f;
	}

	vec3_t get_bone_position(int bone) {
		matrix_t bone_matrices[128];
		if (setup_bones(bone_matrices, 128, 256, 0.0f))
			return vec3_t{ bone_matrices[bone][0][3], bone_matrices[bone][1][3], bone_matrices[bone][2][3] };
		else
			return vec3_t{ };
	}
	vec3_t get_hitbox_position(int hitbox_id) {
		matrix_t bone_matrix[MAXSTUDIOBONES];

		if (setup_bones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0.0f)) {
			auto studio_model = interfaces::model_info->get_studio_model(model());

			if (studio_model) {
				auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

				if (hitbox) {
					vec3_t min{}, max{};

					math::transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
					math::transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

					return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
				}
			}
		}
		return vec3_t{};
	}
	vec3_t get_hitbox_position(int hitbox_id, matrix_t* matrix) {
		auto studio_model = interfaces::model_info->get_studio_model(model());

		if (studio_model) {
			auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

			if (hitbox) {
				vec3_t min{}, max{};

				math::transform_vector(hitbox->mins, matrix[hitbox->bone], min);
				math::transform_vector(hitbox->maxs, matrix[hitbox->bone], max);

				return vec3_t((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
			}

		}
		return vec3_t{};
	}
	c_usercmd** current_cmd() {
		return reinterpret_cast<c_usercmd**>(std::uintptr_t(this) + 0x3348);
	}
	c_usercmd& last_cmd() {
		return *reinterpret_cast<c_usercmd*>(std::uintptr_t(this) + 0x3288);
	}
	bool is_alive() {
		if (!this) return false;
		return this->life_state() == 0;
	}
	std::array<vec3_t, 5>& player_patch_econ_indices() //m_vecPlayerPatchEconIndices
	{
		static int _m_vecPlayerPatchEconIndices = netvar_manager::get_net_var(fnv::hash("DT_CSPlayer"), fnv::hash("m_vecPlayerPatchEconIndices"));
		return *(std::array<vec3_t, 5>*)((uintptr_t)this + _m_vecPlayerPatchEconIndices);
	}

	bool is_moving() {
		if (!this) return false;
		return this->velocity().length() > 0.1f;
	}

	bool is_in_air() {
		if (!this) return false;
		return !(this->flags() & fl_onground);
	}

	bool is_flashed() {
		if (!this) return false;
		return this->flash_duration() > 0.0f;
	}

	void update_client_side_animations() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[223](this);
	}
	void update_colision_bounds() {
		using original_fn = void(__thiscall*)(void*);
		(*(original_fn**)this)[340](this);
	}
	vec3_t& abs_origin() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[10](this);;
	}
	vec3_t& abs_angles() {
		using original_fn = vec3_t & (__thiscall*)(void*);
		return (*(original_fn**)this)[11](this);;
	}
	float& fall_velocity()
	{
		return *(float*)(this + 0x3024);
	}
	int32_t& get_sequence() {
		return *(int32_t*)(this + 0x28C0);
	}
	void set_sequence(int index) {
		v<void(__thiscall*)(decltype(this), int)>(this, 220)(this, index);
	}
	void studio_frame_advance() {
		v<void(__thiscall*)(decltype(this))>(this, 220)(this);
	}
	void set_abs_origin(vec3_t position) {
		using original_fn = void(__thiscall*)(void*, const vec3_t&);
		static original_fn set_position_fn = (original_fn)((DWORD)utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8"));
		set_position_fn(this, position);
	}

	int move_type() {
		static int type = netvar_manager::get_net_var(fnv::hash("DT_BaseEntity"), fnv::hash("m_nRenderMode")) + 1;
		return read<int>(type);
	}
};
