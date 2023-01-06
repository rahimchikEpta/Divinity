class CTeslaInfo {
public:
	vec3_t			m_vPos;
	vec3_t			m_vAngles;
	int				m_nEntIndex;
	const char* m_pszSpriteName;
	float			m_flBeamWidth;
	int				m_nBeams;
	vec3_t			m_vColor;
	float			m_flTimeVisible;
	float			m_flRadius;
};
//-----------------------------------------------------------------------------
// Particle attachment methods
//-----------------------------------------------------------------------------
enum ParticleAttachment_t {
	PATTACH_ABSORIGIN = 0,			// Create at absorigin, but don't follow
	PATTACH_ABSORIGIN_FOLLOW,		// Create at absorigin, and update to follow the entity
	PATTACH_CUSTOMORIGIN,			// Create at a custom origin, but don't follow
	PATTACH_CUSTOMORIGIN_FOLLOW,	// Create at a custom origin, follow relative position to specified entity
	PATTACH_POINT,					// Create on attachment point, but don't follow
	PATTACH_POINT_FOLLOW,			// Create on attachment point, and update to follow the entity
	PATTACH_EYES_FOLLOW,			// Create on eyes of the attached entity, and update to follow the entity
	PATTACH_OVERHEAD_FOLLOW,		// Create at the top of the entity's bbox
	PATTACH_WORLDORIGIN,			// Used for control points that don't attach to an entity
	PATTACH_ROOTBONE_FOLLOW,		// Create at the root bone of the entity, and update to follow

	MAX_PATTACH_TYPES,
};
struct te_tf_particle_effects_colors_t {
	vec3_t m_vecColor1;
	vec3_t m_vecColor2;
};

struct te_tf_particle_effects_control_point_t {
	ParticleAttachment_t m_eParticleAttachment;
	vec3_t m_vecOffset;
};
