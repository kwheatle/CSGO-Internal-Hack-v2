#include "GriefBot.h"

void GriefBot::run(CUserCmd * cmd, CBaseEntity * local)
{
	//CopyBot(cmd, local);
	BlockBot(cmd, local);
}

void GriefBot::CopyBot(CUserCmd * cmd, CBaseEntity * local)
{
	//static CBaseEntity* target = nullptr;
	for (int i = 1; i < gInterface.engine->GetMaxClients(); ++i) {
		auto target = gInterface.entityList->GetClientEntity(i);
		if (!target->IsValid(local, true))continue;

		if (cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK || cmd->buttons & IN_LEFT || cmd->buttons & IN_RIGHT)
			return;

		auto dist = target->GetEyePosition() - local->GetAbsOrigin();
		//dist.z = 0.f;
		if (abs(dist.Length()) < 32.f) {
			auto v = target->GetVelocity();
			auto speed = v.Length();
			auto dir = gMath.VectorToAngle(v);


			dir.y = cmd->viewangles.y - dir.y;

			auto fwd = gMath.AngleToVector(dir);
			cmd->forwardmove = fwd.x * speed;
			cmd->sidemove = fwd.y * speed;
		}

	}

}

void GriefBot::BlockBot(CUserCmd * cmd, CBaseEntity * local)
{


	if (local->HasKnife() && cmd->buttons & IN_ATTACK2) {
		static int w, h;
		int bestTar = -1;
		Vector bestpos;
		float bestDist = FLT_MAX;
		//CBaseEntity::TraceRay( )
		for (int i = 1; i < gInterface.engine->GetMaxClients(); ++i) {
			auto target = gInterface.entityList->GetClientEntity(i);
			if (!target->IsValid(local, true))continue;
			Vector w2sPos;
			if (!gMath.WorldToScreen(target->GetHitboxPos(HITBOX_HEAD), w2sPos)) continue;

			auto dist = (target->GetEyePosition() - local->GetEyePosition()).Length();

			if (dist < bestDist) {
				bestTar = i;
				bestpos = w2sPos;
				bestDist = dist;
				gInterface.engine->GetScreenSize(w, h);
			}

		}

		auto target = gInterface.entityList->GetClientEntity(bestTar);
		if (!target) return;

		if (bestpos.x > (w / 2) + 15) {
			cmd->sidemove = 450;
		}
		else if (bestpos.x < w / 2) {
			cmd->sidemove = -450;
		}
	}
}
