#pragma once
#include "..\Misc\EngineTraceStructs.h"
class IHandleEntity;
class ICollideable;
class IEngineTrace
{
public:
	virtual int   GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = nullptr) = 0;
    virtual int   GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = MASK_ALL) = 0;
    virtual int   GetPointContents_Collideable(ICollideable *pCollide, const Vector &vecAbsPosition) = 0;
    virtual void  ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, trace_t *pTrace) = 0;
    virtual void  ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, trace_t *pTrace) = 0;
    virtual void  TraceRay(const Ray_t &ray, unsigned int fMask, ITrace_Filter *pTraceFilter, trace_t *pTrace) = 0;
};

/*inline int IEngineTrace::GetPointContents(const Vector &vecAbsPosition, int contentsMask, IHandleEntity** ppEntity)
{
	//typedef int(__thiscall* fn)(void*, const Vector&, int, IHandleEntity**);
	return gInterface.CallVirtualFunction<int>(this, 0, std::ref(vecAbsPosition), contentsMask, ppEntity);
}

inline void IEngineTrace::TraceRay(const Ray_t &_ray, unsigned int _mask, ITrace_Filter *_traceFilter, trace_t *_trace)
{
	return gInterface.CallVirtualFunction<void>(this, Indexes::EngineTrace::TraceRay, std::ref(_ray), _mask, _traceFilter, _trace);
}*/

class CPhysCollide;

struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	// VPhysicsSolids
	CPhysCollide   **solids;
	char           *pKeyValues;
	void           *pUserData;
};