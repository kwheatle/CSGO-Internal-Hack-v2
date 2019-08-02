#include "CBaseEntity.h"
#include "CBaseCombatWeapon.h"

matrix3x4_t* CBaseEntity::GetBoneMatrix()
{
	return (matrix3x4_t*)((DWORD)this + Offsets::m_dwBoneMatrix);
}

ICollideable* CBaseEntity::GetCollideable()
{
	return (ICollideable*)((uintptr_t)this + Offsets::m_Collision);
}

studiohdr_t* CBaseEntity::GetPModel()
{
	auto st = (studiohdr_t*)((DWORD)this + Offsets::m_pStudioHdr);
	return (studiohdr_t*)((DWORD)st);
}


model_t* CBaseEntity::GetModel()
{
	return *(model_t**)((DWORD)this + 0x6C);
}

bool CBaseEntity::GetDormant()
{
	return *(bool*)((DWORD)this + Offsets::m_isDormant);
}

int CBaseEntity::GetTeamNumber()
{
	return *(int*)((DWORD)this + Offsets::m_iTeamNum);
}

int CBaseEntity::GetHealth()
{
	return *(int*)((DWORD)this + Offsets::m_iHealth);
}

int CBaseEntity::GetFlags()
{
	return *(int*)((DWORD)this + Offsets::m_fFlags);
}

Vector CBaseEntity::GetEyePosition()
{
	return (*(Vector*)((DWORD)this + Offsets::m_vecViewOffset)) + this->GetAbsOrigin();	
}

Vector CBaseEntity::GetShootPos() {
	Vector ret;
	gInterface.CallVirtualFunction<void>(this, 281, std::ref(ret));
	return ret;
}

Vector CBaseEntity::WorldSpaceCenter()
{
	Vector vecOrigin = GetAbsOrigin();

	Vector min = this->GetCollideable()->OBBMins() + vecOrigin;
	Vector max = this->GetCollideable()->OBBMaxs() + vecOrigin;

	Vector size = max - min;
	size = size / 2.f;
	size = size + min;

	return size;
}

Vector& CBaseEntity::GetAbsOrigin()
{
	return gInterface.CallVirtualFunction<Vector&>(this, Indexes::BaseEntity::GetAbsOrigin);
}

Vector CBaseEntity::GetVelocity()
{
	return *(Vector*)((DWORD)this + Offsets::m_vecVelocity);
}

Vector CBaseEntity::GetBonePosition(int iBone)
{
	DWORD boneAddress = *(DWORD*)((DWORD)this + Offsets::m_dwBoneMatrix);
	
	Vector vec;
	
	vec.x = *(float*)((DWORD)boneAddress + 0x30 * iBone + 0x0C);
	vec.y = *(float*)((DWORD)boneAddress + 0x30 * iBone + 0x1C);
	vec.z = *(float*)((DWORD)boneAddress + 0x30 * iBone + 0x2C);
	
	return vec;

}


QAngle CBaseEntity::GetAimPunchAngle()
{
	return *reinterpret_cast<QAngle*>(uintptr_t(this) + Offsets::m_aimPunchAngle);
}

int CBaseEntity::GetShotsFired()
{
	return *(int*)((DWORD)this + Offsets::m_iShotsFired);
}

int CBaseEntity::GetTickBase()
{
	return *(int*)((DWORD)this + Offsets::m_nTickBase);
}

float CBaseEntity::GetNextAttack()
{
	return *(float*)((DWORD)this + Offsets::m_flNextAttack);
}

float CBaseEntity::GetNextPrimaryAttack()
{
	return *(float*)((DWORD)this + Offsets::m_flNextPrimaryAttack);
}

trace_t CBaseEntity::TraceRay(Vector _from, Vector _to, CBaseEntity* _skip, DWORD _mask)
{
	Ray_t ray;
	trace_t trace;

	ray.Init(_from, _to);

	CTrace_Filter Filter;
	Filter.pSkip = _skip;

	gInterface.engineTrace->TraceRay(ray, _mask, &Filter, &trace);

	return trace;
}

bool CBaseEntity::IsVisible(CBaseEntity* _localPlayer, int _boneId)
{
	if (!this || !_localPlayer)
		return false;

	trace_t trace = TraceRay(_localPlayer->GetEyePosition(), this->GetHitboxPos(_boneId), _localPlayer, MASK_VISIBLE);
	
	if (trace.m_pEnt != this || trace.fraction == 1.0)
		return false;
	
	return true;
}

/*bool CBaseEntity::IsValidTarget(CBaseEntity* _localPlayer, bool _targetTeammates, bool _targetPlayersInAir, bool _targetOnlyVisiblePlayers, int _visibleBoneId)
{
	if (!this || !_localPlayer)
		return false;

	if (this == _localPlayer)
		return false;

	int teamNum = this->GetTeamNumber();

	if (teamNum == 1) 
		return false;
	
	if (!_targetTeammates && teamNum == _localPlayer->GetTeamNumber())
		return false;

	if (this->GetHealth() <= 0)
		return false;

	if (this->GetDormant())
		return false;
	
	if (!_targetPlayersInAir && !(this->GetFlags() & FL_ONGROUND))
		return false;

	if (_targetOnlyVisiblePlayers && !this->IsVisible(_localPlayer, _visibleBoneId))
		return false;
	
	return true;
}*/

bool CBaseEntity::IsValid(CBaseEntity* _localPlayer, bool _targetTeammates)
{
	if (!this || !_localPlayer)
		return false;

	if (this == _localPlayer)
		return false;

	int teamNum = this->GetTeamNumber();

	if (teamNum == 1)
		return false;

	if (!_targetTeammates && teamNum == _localPlayer->GetTeamNumber())
		return false;

	if (!this->GetHealth())
		return false;

	if (this->GetDormant())
		return false;

	if (this->isImmune())
		return false;

	return true;
}

bool CBaseEntity::IsValidBackTrack(CBaseEntity* _localPlayer, bool _targetTeammates)
{
	if (!this || !_localPlayer)
		return false;

	int teamNum = this->GetTeamNumber();

	if (teamNum == 1)
		return false;

	if (!_targetTeammates && teamNum == _localPlayer->GetTeamNumber())
		return false;

	if (!this->GetHealth())
		return false;

	if (this->GetDormant())
		return false;

	if (this->isImmune())
		return false;

	return true;
}

DWORD CBaseEntity::GetWeaponHandle()
{
	return *(uintptr_t*)((uintptr_t)this + Offsets::m_hActiveWeapon);
}

int CBaseEntity::isAnyVisible(CBaseEntity* localPlayer)
{
	for (int i = 0; i < HITBOX_MAX; i++) {
		if (this->IsVisible(localPlayer, i))
			return true;
	}
	return false;
}

int CBaseEntity::getBestBox(CBaseEntity * localPlayer)
{
	for (int i = 0; i < HITBOX_MAX; i++) {
		if (this->IsVisible(localPlayer, i))
			return i;
	}
	return 0;
}


int CBaseEntity::GetWeapon()
{
	auto weaponEntity = (CBaseAttributableItem*)gInterface.entityList->GetClientEntityFromHandle(GetWeaponHandle());
	//return weaponEntity->GetWeaponID();
	return (*weaponEntity->GetItemDefinitionIndex() & 0xFFF);
}

int* CBaseEntity::GetWeapons() {
	//static uintptr_t m_hMyWeapons = NetVars::GetOffset("CBasePlayer", "m_hMyWeapons");
	return reinterpret_cast<int*>(uintptr_t(this) + Offsets::m_hMyWeapons);
}

/*int * CBaseEntity::getMyWeapons()
{
	return (int*)((uintptr_t)this + this->getMyWeapons())
}*/


int CBaseEntity::GetWeapClip()
{
	CBaseCombatWeapon* weaponEntity = (CBaseCombatWeapon*)(DWORD)gInterface.entityList->GetClientEntityFromHandle(this->GetWeaponHandle());
	return weaponEntity->GetBulletsInClip();
}

bool CBaseEntity::HasGun()
{
	for (const auto& weapon : Weapons::guns) {
		if (GetWeapon() == weapon)
			return true;
	}
	return false;
}

bool CBaseEntity::HasAutoGun()
{
	for (const auto& weapon : Weapons::automatic) {
		if (GetWeapon()== weapon)
			return true;
	}
	return false;
}

bool CBaseEntity::HasBoltAction()
{
	for (const auto& weapon : Weapons::bolt_action) {
		if (GetWeapon() == weapon)
			return true;
	}
	return false;
}

bool CBaseEntity::HasKnife()
{
	for (const auto& weapon : Weapons::knives) {
		if (GetWeapon() == weapon)
			return true;
	}
	return false;
}

bool CBaseEntity::CanShoot()
{
	float predictedCurrentTime = this->GetTickBase() * gInterface.globals->interval_per_tick;
	
	if (predictedCurrentTime >= this->GetNextAttack() && this->GetWeapClip() > 0 && !(this->GetFlags() & FL_ATCONTROLS))
		return true;

	return false;
}

player_info_t CBaseEntity::GetPlayerInfo()
{
	player_info_t info;
	gInterface.engine->GetPlayerInfo(this->GetIndex(), &info);
	return info;
}

float CBaseEntity::GetSimulationTime()
{
	return *(float*)((DWORD)this + Offsets::m_flSimulationTime);
}

bool CBaseEntity::isScoped()
{
	return *(bool*)((DWORD)this + Offsets::m_bIsScoped);
}

matrix3x4_t& CBaseEntity::GetCoordFrame() {
	return *(matrix3x4_t*)((uintptr_t)this + Offsets::m_rgflCoordinateFrame);
}

int CBaseEntity::GetMoveType() {
	return *(int*)((DWORD)this + Offsets::m_MoveType);
}

int CBaseEntity::DrawModel2(DrawModelFlags_t flags, uint8_t alpha)
{
	typedef int(__thiscall* origfn)(void*, DrawModelFlags_t, uint8_t);
	return gInterface.CallVirtualFunction<int>(this->GetClientRenderable(), 9, flags, alpha);
}

Vector CBaseEntity::GetHitboxPos(int hitbox_id)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];

	if (SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, gInterface.globals->curtime)) {
		studiohdr_t* studio_model = gInterface.modelInfo->GetStudioModel(GetModel());
		if (studio_model) {
			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
			if (hitbox) {
				auto
					min = Vector{},
					max = Vector{};

				gMath.VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], min);
				gMath.VectorTransform(hitbox->bbmax, boneMatrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}
	return Vector{};
}

IClientRenderable* CBaseEntity::GetClientRenderable()
{
	return reinterpret_cast<IClientRenderable*>(this + 0x4);
}
void CBaseEntity::think() {
	gInterface.CallVirtualFunction< void>(this, 138);
}

void CBaseEntity::pre_think() {
	gInterface.CallVirtualFunction< void>(this, 314);
}

void CBaseEntity::post_think() {
	gInterface.CallVirtualFunction< void>(this, 315);
}

bool CBaseEntity::physics_run_think(int unk01) {
	// if ( c_base_entity::PhysicsRunThink((_DWORD *)player, 0) )
	static auto impl_PhysicsRunThink = reinterpret_cast< bool(__thiscall *)(void *, int) >(
		gInterface.FindPattern("client_panorama.dll", "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1 E8 16")
		);

	return impl_PhysicsRunThink(this, unk01);
}

void CBaseEntity::SetAngle2(QAngle &angle) {
	typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
	static SetAngleFn SetAngle = (SetAngleFn)(gInterface.FindPattern("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
	SetAngle(this, angle);
}

QAngle& CBaseEntity::VisualAngles()
{
	constexpr auto hash = FnvHash("DT_CSPlayer->deadflag");
	static uintptr_t offset = NetVarManager::Get().GetOffset(hash);
	return *(QAngle*)((uintptr_t)this + offset + 0x4);
}

/*QAngle eye_pos() {
	vec3_t vec{};

	util::misc::vfunc< void(__thiscall *)(void *, QAngle &) >
		(this, 279)(this, vec);

	return vec;
}*/

