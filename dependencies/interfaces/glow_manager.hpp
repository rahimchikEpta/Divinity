#pragma once
#include "../../source-sdk/math/vector3d.hpp"
class GlowObjectDefinition_t {
public:
    void set ( float r, float g, float b, float a, float bloom, int style ) {
        m_vGlowColor = vec3_t ( r, g, b );
        m_flGlowAlpha = a;
        m_bRenderWhenOccluded = true;
        m_bRenderWhenUnoccluded = false;
        m_flBloomAmount = 1.f;
        m_nGlowStyle = style;
    }

    player_t* GetEnt () {
        return m_hEntity;
    }

    bool IsUnused () const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

public:
    int              m_nNextFreeSlot; // Linked list of free slots
    player_t* m_hEntity;
    vec3_t           m_vGlowColor;
    float            m_flGlowAlpha;

    char             unknown[ 4 ]; //pad
    float            flUnk; //confirmed to be treated as a float while reversing glow functions
    float            m_flBloomAmount;
    float            localplayeriszeropoint3;

    bool            m_bRenderWhenOccluded;
    bool            m_bRenderWhenUnoccluded;
    bool            m_bFullBloomRender;
    char            unknown1[ 1 ]; //pad

    int                m_nFullBloomStencilTestValue; // 0x28
    int                m_nGlowStyle;
    int                m_nSplitScreenSlot; //Should be -1


    // Special values for GlowObjectDefinition_t::m_nNextFreeSlot
    static const int END_OF_FREE_LIST = -1;
    static const int ENTRY_IN_USE = -2;
};

class glow_manager_t {
public:
    GlowObjectDefinition_t* objects;
	char pad[8];
	int size;
};