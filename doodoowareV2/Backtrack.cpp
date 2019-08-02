#include "Backtrack.h"

void Backtracking::run(CBaseEntity * local, CUserCmd* cmd)
{
	if (!active) {
		for (auto& records : btArray) { //Clear arrays
			for (auto& Data : records.data) {
				Data = tickRecord{ Vector(0,0,0), NULL, Vector(0,0,0) };
			}
		}
		return;
	}

	

	if (bestEnt == -1)
		bestEnt = getBestTarget(local, cmd);

	if (bestEnt != -1 && (cmd->buttons & IN_ATTACK)) {
		backtrackPlayer(local, gInterface.entityList->GetClientEntity(bestEnt), cmd);
	}
	
	bestEnt = -1;
}

void Backtracking::DisableInterpolation(CBaseEntity* pEntity)
{
	VarMapping_t* map = GetVarMap(pEntity);
	if (!map) return;
	for (int i = 0; i < map->m_nInterpolatedEntries; i++)
	{
		VarMapEntry_t *e = &map->m_Entries[i];
		e->m_bNeedsToInterpolate = false;
	}
}

void Backtracking::setLagRecord(CBaseEntity* ent, CUserCmd* cmd)
{
	
	
	int tick = cmd->command_number % 12;
	int lasttick = -1;
	
	int index = ent->GetIndex();
	tickRecord temp = { ent->GetHitboxPos(HITBOX_HEAD), ent->GetSimulationTime(), ent->GetAbsOrigin() };
	
	if (ent->SetupBones(temp.matrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, gInterface.globals->curtime)) {
		//Putting new tick at the front og the array

		if (newBt) {
			std::rotate(btArray[index].data.rbegin(), btArray[index].data.rbegin() + 1, btArray[index].data.rend());
			btArray[index].data[0] = temp;
		}
		else {
			btArray[index].data[tick] = temp;
		}
		
	}	

}

void Backtracking::backtrackPlayer(CBaseEntity* local, CBaseEntity* tar, CUserCmd* cmd)
{
	float bestSim = 0.0f, bestFov = FLT_MAX;

	for (int t = 0; t < maxTicks; ++t) {
		if (!btArray[tar->GetIndex()].data[t].simtime)
			continue;

		QAngle aim = gMath.CalculateAngle(local->GetShootPos(), btArray[tar->GetIndex()].data[t].aimpos);
		float fov = gMath.GetFOV(local->GetShootPos(), btArray[tar->GetIndex()].data[t].aimpos, gMath.AngleToVector(cmd->viewangles + (local->GetAimPunchAngle() * 2.f)));//(cmd->viewangles + (local->GetAimPunchAngle() * 2.f), aim);

		if (fov < bestFov) {
			bestFov = fov;
			bestSim = btArray[tar->GetIndex()].data[t].simtime;
		}
	}

	if (!bestSim)
		return;

	cmd->tick_count = TIME_TO_TICKS(bestSim);
	//gInterface.globals->tickcount = TIME_TO_TICKS(bestSim);
}

int Backtracking::getBestTarget(CBaseEntity* local, CUserCmd* cmd)
{
	float bestFov = FLT_MAX;

	for (int i = 1; i < gInterface.engine->GetMaxClients(); ++i) {
		CBaseEntity* ent = gInterface.entityList->GetClientEntity(i);

		if (!ent->IsValid(local, false))
			continue;

		QAngle aim = gMath.CalculateAngle(local->GetShootPos(), ent->GetHitboxPos(HITBOX_HEAD));
		float FOV = gMath.GetFOV(local->GetShootPos(), ent->GetHitboxPos(HITBOX_HEAD), gMath.AngleToVector(cmd->viewangles + (local->GetAimPunchAngle() * 2.f)));
		if (FOV < bestFov) {
			bestFov = FOV;
			bestEnt = i;
		}

		//setLagRecord(ent, cmd);
	}

	return bestEnt;
}
