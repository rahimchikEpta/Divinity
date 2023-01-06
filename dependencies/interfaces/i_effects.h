#pragma once
// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/IEffects.h

class IPredictionSystem {
public:
    virtual ~IPredictionSystem () { }
};

class i_effects : public IPredictionSystem {
public:
    virtual void    Beam ( const vec3_t& vecStart, const vec3_t& vecEnd, int nModelIndex,
        int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
        float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength,
        unsigned char noise, unsigned char red, unsigned char green,
        unsigned char blue, unsigned char brightness, unsigned char speed ) = 0;

    virtual void    Smoke ( const vec3_t& vecOrigin, int nModelIndex, float flScale, float flFrameRate ) = 0;
    virtual void    Sparks ( const vec3_t& vecOrigin, int iMagnitude = 1, int nTrailLength = 1, const vec3_t* pvecDirection = nullptr ) = 0;
    virtual void    Dust ( const vec3_t& vecOrigin, const vec3_t& vecDirection, float flSize, float flSpeed ) = 0;
    virtual void    MuzzleFlash ( const vec3_t& vecOrigin, const vec3_t& angView, float flScale, int iType ) = 0;
    virtual void    MetalSparks ( const vec3_t& vecOrigin, const vec3_t& vecDirection ) = 0;
    virtual void    EnergySplash ( const vec3_t& vecOrigin, const vec3_t& vecDirection, bool bExplosive = false ) = 0;
    virtual void    Ricochet ( const vec3_t& vecOrigin, const vec3_t& vecDirection ) = 0;
    virtual float    Time () = 0;
    virtual bool    IsServer () = 0;
    virtual void    SuppressEffectsSounds ( bool bSuppress ) = 0;
};