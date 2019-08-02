#pragma once
#include "AutoWall.h"
#include "hooks.h"
#include "Bhop.h"
#include "Aimbot.h"
#include "Rage.h"
#include "EngPred.h"
#include "GriefBot.h"
#include "Backtrack.h"
#include "SourceSDK\Misc\CBaseEntity.h"
#include <array>

Interface gInterface;
Bhop gBhop;
GriefBot gGrief;
Aimbot gAim;
EngPred gEngPred;
Rage gRage;
Backtracking gBacktrack;
hooks::CreateMoveFn hooks::oCreateMove;


bool ret;


void fix_movement(CUserCmd* cmd, CBaseEntity* local_player)
{
	if (!local_player)
		return;

	Vector real_viewangles;
	gInterface.engine->GetViewAngles(real_viewangles);

	Vector vecMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	Vector angMove;
	gMath.VectorAngles(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

	cmd->forwardmove = cos(yaw) * speed;
	cmd->sidemove = sin(yaw) * speed;

	cmd->viewangles.ClampAngles();

	if (cmd->viewangles.x < -89.f || cmd->viewangles.x > 89.f) cmd->forwardmove *= -1;
}

bool __stdcall hooks::HookedCreateMove(float _inputSampleTime, CUserCmd* _cmd) {
	ret = oCreateMove(_inputSampleTime, _cmd);

	gInterface.oCmd = _cmd;

	if (!gInterface.engine->IsConnected() || !gInterface.engine->IsInGame())
		return false;



	if (!_cmd->command_number)
		return ret;


	uintptr_t* FPointer; __asm { MOV FPointer, EBP }
	byte* SendPacket = (byte*)(*FPointer - 0x1C);
	if (!SendPacket) return false;
	
	CBaseEntity* lp = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
	gInterface.local = lp;
	

	for (int i = 1; i < gInterface.engine->GetMaxClients(); ++i) {
		CBaseEntity* ent = gInterface.entityList->GetClientEntity(i);

		if (!ent->IsValid(lp, false))
			continue;

		gBacktrack.setLagRecord(ent, gInterface.oCmd);
	}

	if (lp) {
		//gInterface.lastWeapon = lp->GetWeapon();
		//RANK REVEAL
		if ((_cmd->buttons & IN_SCORE) != 0) {
			gInterface.client->DispatchUserMessage(50, 0, 0, nullptr);
		}

		if (gMenu.noCrouchCooldown) {
			_cmd->buttons |= IN_BULLRUSH;
		}

		if (lp->GetHealth() > 0) {

			//BHOP
			gBhop.bhop(lp, _cmd);

			//gInterface.view = _cmd->viewangles;

			//PREDICTION STUFF
			if (gMenu.engPred) {
				//gEngPred.prestart();
				gEngPred.startpred(_cmd);
			}
			{	
				//AimBot
				gAim.run(lp, gInterface.oCmd);

				//Backtrack
				gBacktrack.run(lp, gInterface.oCmd);

				gRage.run(gInterface.oCmd, lp, SendPacket);
				
			}
			if (gMenu.engPred)
				gEngPred.endpred(_cmd);

			if(gAim.silent || gRage.active)
				fix_movement(gInterface.oCmd, lp);

			//gGrief.run(gInterface.oCmd, lp);

		}
	}
	gInterface.bSendPacket = *SendPacket;
	if (*SendPacket) {
		gInterface.fakeang = _cmd->viewangles;
		//lp->SetAngle2()
		lp->SetupBones(gInterface.localMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, gInterface.globals->curtime);
	}
	else {
		gInterface.realang = _cmd->viewangles;
	}

	return false;
}