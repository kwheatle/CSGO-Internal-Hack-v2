#include "AutoWall.h"
#include "Aimbot.h"

#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture

#define	SURF_HINT		0x0100	// make a primary bsp splitter

#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

float AutoWall::GetHitgroupDamageMultiplier(int iHitGroup)
{
	static float hitgroupModifiers[] = { 1.f, 4.f, 1.f, 1.25f, 1.f, 1.f, .75f, .75f };
	return hitgroupModifiers[iHitGroup];
}

void AutoWall::ScaleDamage(int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMultiplier(hitgroup);
	//CSetupPlayer enemysetup = gPlayers[enemy->GetIndex()];
	if (enemy->GetArmor() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
				current_damage *= (weapon_armor_ratio * .5f);
		}
		else
		{
			current_damage *= (weapon_armor_ratio * .5f);
		}
	}
}

bool AutoWall::DidHitNonWorldEntity(CBaseEntity* m_pEnt)
{
	return m_pEnt != NULL && m_pEnt == gInterface.entityList->GetClientEntity(0);
}
bool IsBreakableEntity(CBaseEntity *ent)
{
	typedef bool(__thiscall *isBreakbaleEntityFn)(CBaseEntity*);
	static isBreakbaleEntityFn IsBreakableEntityFn = (isBreakbaleEntityFn)gInterface.FindPattern("client_panorama.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");

	if (IsBreakableEntityFn)
	{
		// 0x280 = m_takedamage

		auto backupval = *reinterpret_cast<int*>((uint32_t)ent + 0x280);
		auto className = ent->GetClientClass()->m_pNetworkName;

		if (ent != gInterface.entityList->GetClientEntity(0))
		{
			// CFuncBrush:
			// (className[1] != 'F' || className[4] != 'c' || className[5] != 'B' || className[9] != 'h')
			if (ent->GetClientClass()->m_ClassID == EClassIds::CBreakableSurface || ent->GetClientClass()->m_ClassID == EClassIds::CBaseDoor) // CBaseDoor because fuck doors
			{
				*reinterpret_cast<int*>((uint32_t)ent + 0x280) = 2;
			}
		}

		bool retn = IsBreakableEntityFn(ent);

		*reinterpret_cast<int*>((uint32_t)ent + 0x280) = backupval;

		return retn;
	}
	else
		return false;
}


bool AutoWall::TraceToExit(Vector& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace)
{

	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)gInterface.FindPattern("client_panorama.dll", "55 8B EC 83 EC 30 F3 0F 10 75");
	if (!TraceToExit)
		return false;
	//(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	/*_asm
	{
		push trace
		push z2
		push y2
		push x2
		push z
		push y
		push x
		mov edx, tr
		mov ecx, end
		call TraceToExit
		add esp, 0x1C
	}*/

	return TraceToExit(end, tr, x, y, z, x2, y2, z2, trace);
}

bool AutoWall::TraceToExit(Vector &end, CGameTrace *enter_trace, Vector start, Vector dir, CGameTrace *exit_trace)
{
	auto distance = 0.0f;
	int first_contents = 0;

	while (distance < 90.0f)
	{
		distance += 4.0f;
		end = start + (dir * distance);

		if (!first_contents)
			first_contents = gInterface.engineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX);

		auto point_contents = gInterface.engineTrace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX);

		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX) || first_contents == point_contents))
			continue;

		auto new_end = end - (dir * 4.0f);

		UTIL_TraceLine(end, new_end, MASK_SHOT | CONTENTS_GRATE, nullptr, exit_trace);

		if (exit_trace->startsolid && (exit_trace->surface.flags & SURF_HITBOX) < 0)
		{
			UTIL_TraceLine(end, start, MASK_SHOT_HULL, reinterpret_cast<CBaseEntity*>(exit_trace->m_pEnt), exit_trace);

			if (exit_trace->DidHit() && !exit_trace->startsolid)
			{
				end = exit_trace->endpos;
				return true;
			}
			continue;
		}

		if (!exit_trace->DidHit() || exit_trace->startsolid)
		{
			if (enter_trace->m_pEnt)
			{
				if (DidHitNonWorldEntity(enter_trace->m_pEnt) && IsBreakableEntity(reinterpret_cast<CBaseEntity*>(enter_trace->m_pEnt)))
				{
					*exit_trace = *enter_trace;
					exit_trace->endpos = start + dir;
					return true;
				}
			}
			continue;
		}

		if ((exit_trace->surface.flags >> 7) & SURF_LIGHT)
		{
			if (IsBreakableEntity(reinterpret_cast<CBaseEntity*>(exit_trace->m_pEnt)) && IsBreakableEntity(reinterpret_cast<CBaseEntity*>(enter_trace->m_pEnt)))
			{
				end = exit_trace->endpos;
				return true;
			}

			if (!((enter_trace->surface.flags >> 7) & SURF_LIGHT))
				continue;
		}

		if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
		{
			float fraction = exit_trace->fraction * 4.0f;
			end = end - (dir * fraction);

			return true;
		}
	}
	return false;
}

float DistanceToRay(const Vector &pos, const Vector &rayStart, const Vector &rayEnd, float *along = nullptr, Vector *pointOnRay = nullptr)
{
	Vector to = pos - rayStart;
	Vector dir = rayEnd - rayStart;
	float length = dir.Length();
	//dir.Normalize();

	float rangeAlong = dir.Dot(to);
	if (along)
		*along = rangeAlong;

	float range;

	if (rangeAlong < 0.0f)
	{
		range = -(pos - rayStart).Length();

		if (pointOnRay)
			*pointOnRay = rayStart;
	}
	else if (rangeAlong > length)
	{
		range = -(pos - rayEnd).Length();

		if (pointOnRay)
			*pointOnRay = rayEnd;
	}
	else
	{
		Vector onRay = rayStart + (dir * rangeAlong);
		range = (pos - onRay).Length();

		if (pointOnRay)
			*pointOnRay = onRay;
	}

	return range;
}

void ClipTraceToPlayers(CBaseEntity *player, const Vector &vecAbsStart, const Vector &vecAbsEnd, unsigned int mask, ITrace_Filter *filter, CGameTrace *tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;
	const float maxRange = 60.0f;

	ray.Init(vecAbsStart, vecAbsEnd);

	if (!player || !(player->GetHealth() > 0) || player->IsDormant())
	{
		return;
	}

	if (filter && filter->ShouldHitEntity(player, mask) == false)
	{
		return;
	}

	float range = DistanceToRay(player->WorldSpaceCenter(), vecAbsStart, vecAbsEnd);
	if (range >= 0.0f || range <= maxRange)
	{
		gInterface.engineTrace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, player, &playerTrace);
		if (playerTrace.fraction < smallestFraction)
		{
			*tr = playerTrace;
			smallestFraction = playerTrace.fraction;
		}
	}
}


bool AutoWall::HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = gInterface.physicsProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;// *(float*)((DWORD)enter_surface_data + 80);

	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= pow((wpn_data->m_RangeModifier), (data.trace_length * 0.002));

	

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0 || wpn_data->m_Penetration <= 0.f)
		return false;

	auto ff = data;

	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit)) {
		if (!(gInterface.engineTrace->GetPointContents(dummy, MASK_SHOT_HULL) & MASK_SHOT_HULL))
			return false;
	}
		




	surfacedata_t *exit_surface_data = gInterface.physicsProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	//auto f = exit_surface_data;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;//*(float*)((DWORD)exit_surface_data + 76);
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float modifier = fmaxf(0.0f, 1.0f / combined_penetration_modifier);
	float thickness = (trace_exit.endpos - data.enter_trace.endpos).LengthSqr();
	float taken_damage = ((modifier * 3.0f) * fmaxf(0.0f, (3.0f / wpn_data->m_Penetration) * 1.25f) + (data.current_damage * final_damage_modifier)) + ((thickness * modifier) / 24.0f);

	//thickness *= thickness;
	// *= v34;
	//thickness /= 24.0f;

	float lost_damage = fmaxf(0.0f, taken_damage);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count -= 1;

	return true;
}

//Aim gAim;
bool AutoWall::SimulateFireBullet(FireBulletData &data)
{
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto local_player = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
	auto weapon = (CBaseCombatWeapon*)gInterface.entityList->GetClientEntityFromHandle(local_player->GetWeaponHandle());

	if (!weapon) return false;

	// weapon data
	CSWeaponInfo* weaponData = weapon->get_full_info();

	if (weaponData == NULL)
		return false;

	data.current_damage = (float)weaponData->m_WeaponDamage;

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = weaponData->m_WeaponRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		UTIL_TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, local_player, &data.enter_trace);
		ClipTraceToPlayers(gInterface.entityList->GetClientEntity(gAim.bestEnt), data.src, end + data.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0))
		{
			data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
			data.current_damage *= pow(weaponData->m_RangeModifier, data.trace_length * 0.002);
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, weaponData->m_ArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(weaponData, data))
			break;
	}

	return false;
}
void VectorAnglesxd(const Vector& forward, QAngle &angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], ((Vector)forward).Length()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90) angles[1] -= 180;
		else if (angles[1] < 90) angles[1] += 180;
		else if (angles[1] == 90) angles[1] = 0;
	}

	angles[2] = 0.0f;
}
QAngle CalcAngle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;
	VectorAnglesxd(delta, angles);
	delta.Normalize();
	return angles;
}
void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}
void AngleVectors(const QAngle &angles, Vector *forward)
{
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

float AutoWall::GetDamagez(const Vector& point)
{
	float damage = 0.f;
	Vector dst = point;
	static FireBulletData empty;
	FireBulletData data;


	auto local_player = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
	data.src = local_player->GetShootPos();
	data.filter.pSkip = local_player;

	//data.filter.pSkip = G::LocalPlayer;

	QAngle angles = gMath.CalculateAngle(data.src, dst);
	//AngleVectors(angles, &data.direction);
	data.direction = gMath.AngleToVector(angles);
	//Vector dataNormalized;

	//data.direction.Normalize();

	//auto weapon = reinterpret_cast<CBaseAttributableItem*>(gInterface.entityList->GetClientEntity(local_player->GetWeapon()));
	if (SimulateFireBullet(data))
	{
		damage = data.current_damage;
	}

	return damage;
}