#pragma once
#include "SourceSDK\sdk.h"
#include "SourceSDK\Misc\CBaseCombatWeapon.h"
#include "SourceSDK\Misc\SurfaceData.h"

class AutoWall
{
public:
	float GetDamagez(const Vector& vecPoint);
	void UTIL_TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, CBaseEntity *ignore, trace_t *ptr)
	{
		Ray_t ray;

		CTrace_Filter filter;
		filter.pSkip = ignore;

		ray.Init(vecAbsStart, vecAbsEnd);

		gInterface.engineTrace->TraceRay(ray, mask, &filter, ptr);
	}
	void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITrace_Filter* filter, trace_t* tr)
	{
		//typedef bool(__fastcall* ClipFn)(const Vector& , const Vector& , unsigned int , ITrace_Filter* , trace_t* );
		static DWORD ClipTrace = gInterface.FindPattern("client_panorama.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC D8 ? ? ? 0F 57 C9");
		if (!ClipTrace)
			return;

		//ClipTrace(vecAbsStart, vecAbsEnd, mask, filter, tr);

		_asm
		{
			MOV        EAX, filter
			LEA        ECX, tr
			PUSH    ECX
			PUSH    EAX
			PUSH    mask
			LEA        EDX, vecAbsEnd
			LEA        ECX, vecAbsStart
			CALL    ClipTrace
			ADD        ESP, 0xC
		}
	}
	struct FireBulletData
	{
		Vector          src;
		trace_t         enter_trace;
		Vector          direction;
		CTrace_Filter   filter;
		float           trace_length;
		float           trace_length_remaining;
		float           current_damage;
		int             penetrate_count;
	};

	//FireBulletData data;

	//float GetDamage(const Vector& vecPoint, FireBulletData& fData);
	bool SimulateFireBullet(FireBulletData& data);
	bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data);
	bool TraceToExit(Vector& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace);
	bool TraceToExit(Vector & end, CGameTrace * enter_trace, Vector start, Vector dir, CGameTrace * exit_trace);
	bool DidHitNonWorldEntity(CBaseEntity* m_pEnt);
	void ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage);
	float GetHitgroupDamageMultiplier(int iHitGroup);
	//bool SimulateFireBullet2(CBaseEntity *local, CBaseAttributableItem *weapon, FireBulletData &data);

};

extern AutoWall gAutowall;