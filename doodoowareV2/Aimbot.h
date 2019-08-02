#pragma once
#include "SourceSDK\sdk.h"
#include "Backtrack.h"
#include "LegitBot.h"
#include "PID.h"

class Aimbot
{

public:
	bool active = false;
	bool velPredict = false;
	bool silent = false;
	bool autoshoot = false;
	bool prioritizeVisible = false;
	bool targetThroughWalls = false;
	bool dynamicAim = false;
	bool tapShoot = true;
	bool rcs = false;
	bool autowall = false;

	int aimbox = HITBOX_HEAD;

	float aimFOV = 20.f, smooth = 1.f, fov_independence = 1.f, minDamage = 50.f;
	void run(CBaseEntity* local, CUserCmd* cmd);
	void aimAtPlayer(CBaseEntity * local, CBaseEntity* tar, CUserCmd* cmd);
	Vector fix_movement(CUserCmd * cmd, CUserCmd originalCMD);
	void RCS(QAngle & angle, CBaseEntity * player, CUserCmd * cmd);
	QAngle GetSmoothAngle(const QAngle _from, const QAngle _to, float * _smooth, const float _decr);
	//QAngle FinalAngle(C_BasePlayer * target, CUserCmd * cmd);
	int getBestTarget(CBaseEntity* local, CUserCmd* cmd);
	bool good_backtrack_tick(int tick);
	//QAngle AimPID(CBaseEntity * local, QAngle& aim, CUserCmd * cmd);
	int bestEnt = -1;
	int prevBest = -1;

	float m_CurAimTime = 0;//How long its been since we started aiming, in second
	float m_AimTime = 1;//Time to reach target in seconds
	float newTargetDelay = 1;

	float bestSim;
	float bestFov;
	float bestTick;

private:

};
extern Aimbot gAim;
