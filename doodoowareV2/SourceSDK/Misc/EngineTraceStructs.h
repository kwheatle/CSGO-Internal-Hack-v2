#pragma once
#include <Windows.h>
#include "../Math/Vector.h"
#include "../Math/VMatrix.h"

class CBaseEntity;

class __declspec(align(16))VectorAligned : public Vector
{
public:
	VectorAligned& operator=(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}
	float w;
};

struct Ray_t
{
	Ray_t() { }

	VectorAligned m_Start;	// starting point, centered within the extents
	VectorAligned m_Delta;	// direction + length of the ray
	VectorAligned m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned m_Extents;	// Describes an axis aligned box extruded along a ray

	const matrix3x4_t *m_pWorldAxisTransform;

	bool m_IsRay;	// are the extents zero?
	bool m_IsSwept;	// is delta != 0?
	
	void Init(const Vector& start, const Vector& end)
	{
		m_Delta = end - start;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.f;
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.f;
		m_Start = start;
	}

};

struct csurface_t
{
	const char *name;
	short surfaceProps;
	unsigned short flags;
};

struct cplane_t
{
	Vector normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
};

#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)

class CBaseTrace
{
public:
	/*bool IsDispSurface() const { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable() const { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable() const { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1() const { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2() const { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }*/
public:
	Vector startpos;				// start position
	Vector endpos;					// final position
	cplane_t plane;					// surface normal at impact
	float fraction;				// time completed, 1.0 = didn't hit anything
	int contents;				// contents on other side of surface hit
	unsigned short dispFlags;				// displacement flags for marking surfaces with data
	bool allsolid;				// if true, plane is not valid
	bool startsolid;				// if true, the initial point was in a solid area

	//CBaseTrace() {}
};

class CGameTrace : public CBaseTrace
{
public:
	float fractionleftsolid;		// time we left a solid, only valid if we started in solid
	csurface_t surface;				// surface hit (impact surface)
	int hitgroup;				// 0 == generic, non-zero is specific body part
	short physicsbone;			// physics bone hit by trace in studio
	CBaseEntity* m_pEnt;
	int hitbox;					// box hit by trace in studio
	bool DidHit() const { return fraction < 1 || allsolid || startsolid; };
	inline bool IsVisible() const
	{
		return fraction > 0.97f;
	}
	CGameTrace() {}

};
typedef CGameTrace trace_t;


enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITrace_Filter
{
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTrace_Filter : public ITrace_Filter
{
public:
	bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle != pSkip);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	CBaseEntity* pSkip;
};

class CTraceWorldOnly : public ITrace_Filter
{
public:
	bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{
		return false;
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	CBaseEntity* pSkip;
};

class CTraceFilterSkipTwoEntities : public ITrace_Filter
{
public:
	CTraceFilterSkipTwoEntities(CBaseEntity* ent1, CBaseEntity* ent2)
	{
		pEnt1 = ent1;
		pEnt2 = ent2;
	}
	bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pEnt1 || pEntityHandle == pEnt2);
	}
	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

private:
	CBaseEntity* pEnt1;
	CBaseEntity* pEnt2;
};

