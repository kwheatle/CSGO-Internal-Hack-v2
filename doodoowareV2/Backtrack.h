#pragma once
#include "hooks.h"
#include "LegitBot.h"
#include "SourceSDK\Math\Math.h"
#include "SourceSDK\sdk.h"
#include "Visuals.h"
#include <array>

struct tickRecord {
	Vector aimpos;
	float simtime;
	Vector origin;
	matrix3x4_t matrix[128];
};

struct LagRecord {
	//int ent_index;
	std::array<tickRecord, 12> data;
};

class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

class Backtracking
{
public:
	bool active = false;
	bool newBt = true;


	int maxTicks = 12;

	void run(CBaseEntity* local, CUserCmd* cmd);
	void DisableInterpolation(CBaseEntity * pEntity);
	void setLagRecord(CBaseEntity* ent, CUserCmd* cmd);
	void backtrackPlayer(CBaseEntity * local, CBaseEntity* tar, CUserCmd* cmd);
	int getBestTarget(CBaseEntity* local, CUserCmd* cmd);
	int bestEnt = -1;
	std::array<LagRecord, 128> btArray;
	//LagRecord btArray[128][13];
private:
	VarMapping_t * GetVarMap(void* pBaseEntity)
	{
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24);
	}

};
extern Backtracking gBacktrack;