#include "Aimbot.h"
#include "AutoWall.h"
#include <algorithm>

struct aimData {
	float fov;
	int ent_index;
	CBaseEntity* ent;
	bool isVisible;
	LagRecord record;
	float damage;
};

std::vector<aimData> targets;

bool operator<(aimData const & a, aimData const & b)
{
	return a.fov < b.fov;
}
Vector LerpAngle(Vector& From, Vector& To, float Step)
{
	Vector Delta = (To - From);
	Delta.ClampAngles();
	Vector New = From + (Delta * Step);
	New.ClampAngles();
	return New;
}

Vector CerpAngle(Vector& From, Vector& To, float StepX, float StepY)
{
	float CubicStepX = (1 - cos(StepX*M_PI)) / 2;
	float CubicStepY = (1 - cos(StepY*M_PI)) / 2;
	Vector Delta = (To - From);
	Delta.ClampAngles();
	Vector New(0,0,0);
	New.x = (From.x + CubicStepX * Delta.x);
	New.y = (From.y + CubicStepY * Delta.y);
	New.ClampAngles();

	return New;
}

float LerpTime() {

	static ConVar* updaterate = gInterface.cvar->FindVar("cl_updaterate");
	static ConVar* minupdate = gInterface.cvar->FindVar("sv_minupdaterate");
	static ConVar* maxupdate = gInterface.cvar->FindVar("sv_maxupdaterate");
	static ConVar* lerp = gInterface.cvar->FindVar("cl_interp");
	static ConVar* cmin = gInterface.cvar->FindVar("sv_client_min_interp_ratio");
	static ConVar* cmax = gInterface.cvar->FindVar("sv_client_max_interp_ratio");
	static ConVar* ratio = gInterface.cvar->FindVar("cl_interp_ratio");

	float lerpurmom = lerp->GetFloat(), maxupdateurmom = maxupdate->GetFloat(),
		ratiourmom = ratio->GetFloat(), cminurmom = cmin->GetFloat(), cmaxurmom = cmax->GetFloat();
	int updaterateurmom = updaterate->GetInt(),
		sv_maxupdaterate = maxupdate->GetInt(), sv_minupdaterate = minupdate->GetInt();

	if (sv_maxupdaterate && sv_minupdaterate)
		updaterateurmom = maxupdateurmom;

	if (ratiourmom == 0)
		ratiourmom = 1.0f;

	if (cmin && cmax && cmin->GetFloat() != 1)
		ratiourmom = clamp(ratiourmom, cminurmom, cmaxurmom);

	return max(lerpurmom, ratiourmom / updaterateurmom);
}
bool justSwitched;
bool tarDelay = false;
float DelayTimer = 0;

void Aimbot::run(CBaseEntity * local, CUserCmd * cmd)
{
	if (!active)
		return;

	if (tarDelay) {
		DelayTimer += gInterface.globals->frametime;
		if (DelayTimer >= newTargetDelay) {
			tarDelay = false;
			DelayTimer = 0;
		}

		if (!(cmd->buttons & IN_ATTACK) && local->HasAutoGun()) {
			tarDelay = false;
			DelayTimer = 0;
		}

		return;
	}

	if (bestEnt == -1) {
		bestEnt = getBestTarget(local, cmd);
		if (bestEnt != prevBest) {
			prevBest = bestEnt;
			justSwitched = true;
			m_CurAimTime = 0;
			tarDelay = true;
			return;
		}
	}

	auto tar = gInterface.entityList->GetClientEntity(bestEnt);
	if (bestEnt != -1 && ((cmd->buttons & IN_ATTACK) || autoshoot || autowall) && local->CanShoot() && local->HasGun()) {
		m_CurAimTime += gInterface.globals->frametime;

		if (m_CurAimTime > m_AimTime)
			m_CurAimTime = m_AimTime;

		aimAtPlayer(local, tar, cmd);
	}//If we let go of our shoot button
	else if (bestEnt != -1 && !(cmd->buttons & IN_ATTACK) && local->CanShoot()) {
		m_CurAimTime = 0;
	}

	prevBest = bestEnt;
	justSwitched = false;
	bestEnt = -1;
}
Vector abs(Vector v) {
	v.x = fabs(v.x);
	v.y = fabs(v.y);
	v.z = fabs(v.z);
	return v;
}

AutoWall gAutowall;
int bbox;
void Aimbot::aimAtPlayer(CBaseEntity * local, CBaseEntity * tar, CUserCmd * cmd)
{

	
	bbox = dynamicAim ? tar->getBestBox(local) : aimbox;

	if (!tar->IsVisible(local, bbox) && !autowall)return;

	QAngle viewAng = cmd->viewangles;//gInterface.view;
	auto vel = tar->GetVelocity();
	Vector enpos = tar->GetHitboxPos(bbox), eyepos = local->GetShootPos();
	if (velPredict && vel) {
		enpos = enpos + (vel* gInterface.globals->interval_per_tick);
	}

	QAngle aim = gMath.CalculateAngle(eyepos, enpos);
	
	/*if (tapShoot) {
		float dmg = gAutowall.GetDamagez(enpos);
		std::ostringstream t;
		t << "echo dmg: " << dmg;
		gInterface.engine->ExecuteClientCmd(t.str().c_str());
	}*/

	if((local->GetShotsFired() > 1 && rcs)|| justSwitched || autoshoot)
		RCS(aim, local, cmd);
	aim.ClampAngles();


	if (m_AimTime == 0) m_AimTime = 0.0000001f;
	float AimPercent = m_CurAimTime / m_AimTime;

	QAngle aimang;
	if(local->HasAutoGun())
		aimang = CerpAngle(viewAng, aim, AimPercent, AimPercent);
	else if (local->HasBoltAction())
		aimang = GetSmoothAngle(viewAng, aim, &smooth, 0);
	else
		aimang = LerpAngle(viewAng, aim, AimPercent);


	if ((autoshoot) && !(cmd->buttons & IN_ATTACK2) && local->GetWeapClip() > 0) {
		if (!local->HasAutoGun() || tapShoot) {
			switch (cmd->command_number % 2) {
			case 0: cmd->buttons |= IN_ATTACK; break;
			case 1: cmd->buttons &= ~IN_ATTACK; break;
			}
		}
		else {
			cmd->buttons |= IN_ATTACK;
		}

	}

	aimang.ClampAngles();
	cmd->viewangles = aimang;
	if(!silent)
		gInterface.engine->SetViewAngles(aimang);
}


QAngle RCSLastPunch;

void Aimbot::RCS(QAngle& angle, CBaseEntity* player, CUserCmd* cmd)
{

	if (!(cmd->buttons & IN_ATTACK) && !player->CanShoot())
		return;

	QAngle CurrentPunch = player->GetAimPunchAngle();

	if (silent || bestEnt != -1)
	{
		angle.x -= CurrentPunch.x * 2.f;
		angle.y -= CurrentPunch.y * 2.f;
	}
	else if (player->GetShotsFired() > 1 || tapShoot)
	{
		QAngle NewPunch = { CurrentPunch.x - RCSLastPunch.x, CurrentPunch.y - RCSLastPunch.y, 0 };

		angle.x -= NewPunch.x * 2.f;
		angle.y -= NewPunch.y * 2.f;
	}

	RCSLastPunch = CurrentPunch;
}


QAngle Aimbot::GetSmoothAngle(const QAngle _from, const QAngle _to, float* _smooth, const float _decr)
{
	QAngle smoothAngle = _to;

	QAngle delta = _to - _from;
	delta.ClampAngles();

	if (!delta.IsZero())
	{
		if (*_smooth - _decr > 1.f)
			*_smooth -= _decr;
		else
			*_smooth = 1.f;

		QAngle step = delta / *_smooth;
		smoothAngle = _from + step;
	}

	return smoothAngle;
}

trace_t TTraceRay(Vector _from, Vector _to, CBaseEntity* _skip, DWORD _mask)
{
	Ray_t ray;
	trace_t trace;

	ray.Init(_from, _to);

	CTrace_Filter Filter;
	Filter.pSkip = _skip;

	gInterface.engineTrace->TraceRay(ray, _mask, &Filter, &trace);

	return trace;
}

bool IsVisible(CBaseEntity* _localPlayer, Vector _boneId)
{

	trace_t trace = TTraceRay(_localPlayer->GetShootPos(), _boneId, _localPlayer, MASK_VISIBLE);

	if (trace.fraction == 1.0)
		return false;

	return true;
}

int Aimbot::getBestTarget(CBaseEntity * local, CUserCmd * cmd)
{
	float maxFov = aimFOV;
	int bestVisTar = -1;
	int bestDamageTar = -1;

	targets.clear();

	for (int i = 1; i < gInterface.engine->GetMaxClients(); ++i) {
		CBaseEntity* ent = gInterface.entityList->GetClientEntity(i);

		if (!ent->IsValid(local, false))
			continue;

		Vector enpos = ent->GetHitboxPos(aimbox), eyepos = local->GetShootPos();

		QAngle aim = gMath.CalculateAngle(eyepos, enpos);
		float FOV = gMath.GetFOV(cmd->viewangles + (local->GetAimPunchAngle() * 2.f), aim);

		if (FOV > maxFov) continue;

		aimData temp = { FOV, i , ent, ent->isAnyVisible(local), gBacktrack.btArray[i].data , gAutowall.GetDamagez(enpos)};
		targets.emplace_back(temp);
	}

	if (targets.empty()) {
		return -1; //NO TARGET
	}

	std::sort(targets.begin(), targets.end());

	//Aiming only at players
	
	{

		//Return Visible players before anyone else
		for (int i = 0; i < targets.size(); ++i) {
			if (targets[i].isVisible) {
				bestVisTar = targets[i].ent_index;
				break;
			}
		}

		float tempBest = 0.f;
		for (int i = 0; i < targets.size(); ++i) {
			if (targets[i].damage > minDamage && targets[i].damage > tempBest) {
				bestDamageTar = targets[i].ent_index;
				tempBest = targets[i].damage;
			}
		}


		if (autowall) {
			return bestDamageTar;
		}

		if (!targetThroughWalls) {
			//Only return tagrets we can see
			return bestVisTar;
		}

		if (targetThroughWalls && prioritizeVisible) {
			//If we want to target Through Walls but prioritize visible targets
			if (bestVisTar != -1) //If we can see someone, make them our target
				return bestVisTar;
			else //If we cant then return the player closest to our crosshair
				return targets[0].ent_index;
		}
		else if (targetThroughWalls && !prioritizeVisible) {
			//return the closest player
			return targets[0].ent_index;
		}
	}

	//TO-DO
	/*
	Change return type to a struct and return all possible targets [visTar, closestTar, bestDamage]
	select target based of selection type in aim function
	
	*/
	


	/*				YIKES (Backtrack meme)					*/		
	/*{
		bestFov = FLT_MAX;
		for (auto data : targets) {
			for (int t = 0; t < gBacktrack.maxTicks; ++t) {
				if (!data.record.data[t].simtime)
					continue;
				//data.record.data[t].aimpos
				if (!IsVisible(local, data.record.data[t].aimpos) && !targetThroughWalls) continue;

				QAngle aim = gMath.CalculateAngle(local->GetEyePosition(), data.record.data[t].aimpos);
				float fov = gMath.GetFOV(local->GetEyePosition(), data.record.data[t].aimpos, gMath.AngleToVector(cmd->viewangles + (local->GetAimPunchAngle() * 2.f)));//(cmd->viewangles + (local->GetAimPunchAngle() * 2.f), aim);

				if (fov < bestFov) {
					bestEnt = data.ent_index;
					bestFov = fov;
					bestSim = data.record.data[t].simtime;
					bestTick = t;
				}
			}
		}
	}*/


	//should never get here but w/e
	return -1;
}

bool Aimbot::good_backtrack_tick(int tick)
{
	const auto delta = gInterface.globals->tickcount - tick;
	const auto delta_time = delta * gInterface.globals->interval_per_tick;
	const auto max = static_cast< float >(static_cast < float >(gBacktrack.maxTicks * (1/64)) / static_cast < float >(1000));
	return (fabs(delta_time) <= max);
}

/*void CerpAngle(Vector& From, Vector& To, float StepX, float StepY)
{
	float CubicStepX = (1 - cos(StepX*PI)) / 2;
	float CubicStepY = (1 - cos(StepY*PI)) / 2;
	Vector Delta = (To - From);
	NormalizeAngle(Delta);
	To.x = (From.x + CubicStepX * Delta.x);
	To.y = (From.y + CubicStepY * Delta.y);
	NormalizeAngle(To);
}*/

