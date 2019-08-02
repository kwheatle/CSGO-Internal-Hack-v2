#pragma once
#include "hooks.h"
class Rage
{
public:
	void run(CUserCmd* cmd, CBaseEntity* local, byte* bSendPacket);
	void FakeLag(CUserCmd * cmd, CBaseEntity * local, byte * bSendPacket);
	void AA(CUserCmd * cmd, CBaseEntity * local, byte * bSendPacket);
	void AutoWall(CUserCmd * cmd, CBaseEntity * local, byte * bSendPacket);
	bool Freestanding(CBaseEntity * player, float & ang);
	float yaw = 0.f;
	bool Flip = true;
	bool active = false;
	int flipticks = 64;

	void DoAntiAim(CUserCmd* cmd, CBaseEntity * local, byte* bSendPacket);
	void Pitch(CUserCmd* cmd, CBaseEntity * local);
	void Yaw(CUserCmd* cmd, CBaseEntity * local, bool fake);
	void YawAdd(CUserCmd* cmd, CBaseEntity * local, bool fake);

};

enum class YawAntiAims : int
{
	NONE,
	BACKWARDS,
	SPINBOT,
	LOWER_BODY,
	RANDOM,
	FREESTANDING,
	CUSTOM
};

enum class YawAddAntiAims : int
{
	NONE,
	JITTER,
	SWITCH,
	SPIN,
	RANDOM
};

enum class PitchAntiAims : int
{
	NONE,
	EMOTION,
	DOWN,
	UP,
	ZERO,
	JITTER,
	DOWN_JITTER,
	UP_JITTER,
	ZERO_JITTER,
	SPIN,
	SPIN_UP,
	SPIN_DOWN,
	RANDOM,
	SWITCH,
	DOWN_SWITCH,
	UP_SWITCH,
	FAKE_UP,
	FAKE_DOWN,
	CUSTOM
};

class AntiAim : public Rage
{
public:
	void OnCreateMove(CUserCmd* cmd, bool& bSendPacket);

	


	//freestanding
	float fov_player(Vector ViewOffSet, QAngle View, C_BasePlayer* entity);
	int GetNearestPlayerToCrosshair();
	
	bool FreestandingLbyBreak(float& ang);

	bool Freestanding(C_BasePlayer * player, float & ang, CBaseEntity* lp);

};

extern Rage gRage;