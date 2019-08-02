#pragma once
#include "..\sdk.h"
#include "..\Misc\IClientRenderable.h"
#include "..\Utils\Netvars.h"


class CBaseHandle;
class IClientEntity;
class studiohdr_t;
enum DrawModelFlags_t;

class IHandleEntity {
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle &GetRefEHandle() const = 0;
};

class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class IClientThinkable
{
public:
	virtual IClientUnknown*     GetIClientUnknown() = 0;
	virtual void                ClientThink() = 0;
	virtual ClientThinkHandle_t GetThinkHandle() = 0;
	virtual void                SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void                Release() = 0;
};
class IClientAlphaProperty;

class IClientUnknown : public IHandleEntity {
public:
	virtual ICollideable *get_collideable() = 0;
	virtual IClientNetworkable *get_client_networkable() = 0;
	virtual IClientRenderable *get_client_renderable() = 0;
	virtual IClientEntity *GetIClientEntity() = 0;
	virtual CBaseEntity *GetBaseEntity() = 0;
	virtual IClientThinkable *GetClientThinkable() = 0;
	// virtual IClientModelRenderable  *GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty *GetClientAlphaProperty() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
public:
	virtual void Release(void) = 0;
};


class CBaseEntity : public IClientEntity
{
public:

	int GetArmor()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + Offsets::m_ArmorValue);
	}
	bool HasHelmet()
	{
		return *reinterpret_cast<bool*>(uintptr_t(this) + Offsets::m_bHasHelmet);
	}

	DWORD m_dwBoneMatrix = NULL;
	matrix3x4_t m_BoneMatrix[128];
	PNETVAR(GetViewModel, DWORD, "DT_BasePlayer->m_hViewModel[0]");
	NETVAR(isImmune, bool, "DT_CSPlayer->m_bGunGameImmunity");
	NETVAR(m_flLowerBodyYawTarget, float, "DT_CSPlayer->m_flLowerBodyYawTarget");
	PNETVAR(GetModelIndex, int, "DT_BaseViewModel->m_nModelIndex");
	PNETVAR(GetViewModelIndex, int, "DT_BaseCombatWeapon->m_iViewModelIndex");
	NETVAR(GetWorldModel, DWORD, "DT_BaseCombatWeapon->m_hWeaponWorldModel");
	//PNETVAR(myWeapons, int, "DT_BasePlayer->m_hMyWeapons");

	NETVAR(GetOriginalOwnerXuidLow, int, "DT_BaseAttributableItem->m_OriginalOwnerXuidLow");
	NETVAR(GetOriginalOwnerXuidHigh, int, "DT_BaseAttributableItem->m_OriginalOwnerXuidHigh");

	//PNETVAR(AimPunch, QAngle, "DT_BasePlayer->m_Local->m_aimPunchAngle");
	//PNETVAR(ViewPunch, QAngle, "DT_BasePlayer->m_Local->m_viewPunchAngle");

	//PNETVAR(ItemDefIndex, int, "DT_BaseAttributableItem->m_iItemDefinitionIndex");

	NETVAR(tickbase, int, "DT_BasePlayer->m_nTickBase");
	//int GetIndex();
	matrix3x4_t* GetBoneMatrix();
	ICollideable * GetCollideable();
	bool GetDormant();
	int GetTeamNumber();
	int GetHealth();
	int GetFlags();
	Vector GetEyePosition();
	Vector GetShootPos();
	Vector WorldSpaceCenter();
	Vector& GetAbsOrigin();
	Vector GetVelocity();
	Vector GetBonePosition(int iBone);
	Vector get_eye_origin();
	QAngle GetAimPunchAngle();
	int GetShotsFired();
	DWORD GetWeaponHandle();
	int isAnyVisible(CBaseEntity* localPlayer);
	int getBestBox(CBaseEntity* localPlayer);
	

	int GetTickBase();
	float GetNextAttack();
	float GetNextPrimaryAttack();
	trace_t TraceRay(Vector _from, Vector _to, CBaseEntity * _skip, DWORD _mask);
	bool IsVisible(CBaseEntity* _localPlayer, int _boneId);
	bool IsValidTarget(CBaseEntity* _localPlayer, bool _targetTeammates, bool _targetPlayersInAir, bool _targetOnlyVisiblePlayers, int _visibleBoneId);
	bool IsValid(CBaseEntity * _localPlayer, bool _targetTeammates);
	bool IsValidBackTrack(CBaseEntity * _localPlayer, bool _targetTeammates);
	int GetWeapon();
	int* GetWeapons();
	//int* getMyWeapons();
	int GetWeapClip();
	bool HasGun();
	bool HasAutoGun();
	bool HasBoltAction();
	bool HasKnife();
	bool CanShoot();
	player_info_t GetPlayerInfo();
	float GetSimulationTime();

	bool isScoped();

	matrix3x4_t & GetCoordFrame();

	int GetMoveType();
	int DrawModel2(DrawModelFlags_t flags, uint8_t alpha);
	Vector GetHitboxPos(int hitbox_id);

	IClientRenderable* GetClientRenderable();

	void think();

	void pre_think();

	void post_think();

	bool physics_run_think(int unk01);

	void SetAngle2(QAngle &angle);

	QAngle & VisualAngles();

	//bool SetupBones(matrix3x4_t * pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);

	studiohdr_t* GetPModel();

	model_t * GetModel();

	studiohdr_t * GetHDR();

	


	bool HasC4();

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_ISOMETRIC,
		MOVETYPE_WALK,
		MOVETYPE_STEP,
		MOVETYPE_FLY,
		MOVETYPE_FLYGRAVITY,
		MOVETYPE_VPHYSICS,
		MOVETYPE_PUSH,
		MOVETYPE_NOCLIP,
		MOVETYPE_LADDER,
		MOVETYPE_OBSERVER,
		MOVETYPE_CUSTOM,
		MOVETYPE_LAST = MOVETYPE_CUSTOM,
		MOVETYPE_MAX_BITS = 4
	};

	

};

class CEnvTonemapController : CBaseEntity {
public:
	PNETVAR(use_custom_exposure_min, unsigned char, "DT_EnvTonemapController->m_bUseCustomAutoExposureMin");
	PNETVAR(use_custom_exposure_max, unsigned char, "DT_EnvTonemapController->m_bUseCustomAutoExposureMax");
	PNETVAR(custom_exposure_min, float,  "DT_EnvTonemapController->m_flCustomAutoExposureMin");
	PNETVAR(custom_exposure_max, float,  "DT_EnvTonemapController->m_flCustomAutoExposureMax");
};

class CMoveData {
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;
	bool			m_bNoAirControl : 1;

	unsigned long	m_nPlayerHandle;
	int				m_nImpulseCommand;
	QAngle			m_vecViewAngles;
	QAngle			m_vecAbsViewAngles;
	int				m_nButtons;
	int				m_nOldButtons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	Vector			m_vecVelocity;
	Vector			m_vecOldVelocity;
	float			somefloat;
	QAngle			m_vecAngles;
	QAngle			m_vecOldAngles;

	float			m_outStepHeight;
	Vector			m_outWishVel;
	Vector			m_outJumpVel;

	Vector			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	bool			m_bConstraintPastRadius;

	void			SetAbsOrigin(const Vector &vec);
	const Vector	&GetOrigin() const;

private:
	Vector			m_vecAbsOrigin;
};

class CBaseViewModel : public CBaseEntity {
public:
	DWORD GetOwner() {
		//static uintptr_t m_hOwner = NetVars::Get().GetOffset("CBaseViewModel", "m_hOwner");
		return *reinterpret_cast<DWORD*>(uintptr_t(this) + Offsets::m_hOwner);
	}

	int GetWeapon() {
		//static uintptr_t m_hWeapon = NetVars::Get().GetOffset("CBaseViewModel", "m_hWeapon");
		return *reinterpret_cast<int*>(uintptr_t(this) + Offsets::m_hActiveWeapon);
	}

	short* ItemDefenitionIndex()
	{
		return reinterpret_cast<short*>(uintptr_t(this) + Offsets::m_iItemDefinitionIndex);
	}
	//NETVAR(GetSequenceProp, "CBaseViewModel", "m_nSequence");

};

class C_BasePlayer : public CBaseEntity
{
public:
	


	/*bool GetImmune()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bGunGameImmunity);
	}

	bool IsRescuing()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_CSPlayer.m_bIsRescuing);
	}


	float* GetFlashMaxAlpha()
	{
		return (float*)((uintptr_t)this + offsets.DT_CSPlayer.m_flFlashMaxAlpha);
	}

	float* GetLowerBodyYawTarget()
	{
		return (float*)((uintptr_t)this + offsets.DT_CSPlayer.m_flLowerBodyYawTarget);
	}

	
	Vector GetEyePosition()
	{
		return this->GetVecOrigin() + this->GetVecViewOffset();
	}

	inline Vector GetBonePosition(int boneIndex)
	{
		matrix3x4_t BoneMatrix[MAXSTUDIOBONES];

		if (!this->SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
			return this->GetAbsOrigin();

		matrix3x4_t hitbox = BoneMatrix[boneIndex];

		return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}
	*/
};


