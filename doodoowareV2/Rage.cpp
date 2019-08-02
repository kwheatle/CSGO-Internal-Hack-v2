#include "Rage.h"


float RandomFloat(float min, float max)
{
	static auto ranFloat = reinterpret_cast<float(*)(float, float)>(GetProcAddress(GetModuleHandleW(L"vstdlib.dll"), "RandomFloat"));
	if (ranFloat)
	{
		return ranFloat(min, max);
	}
	else
	{
		return 0.f;
	}
}

void Rage::run(CUserCmd * cmd, CBaseEntity * local, byte* bSendPacket)
{
	FakeLag(cmd, local, bSendPacket);

	if (!active )
		return;

	//AA(cmd, local, bSendPacket);
	DoAntiAim(cmd, local, bSendPacket);
}

void Rage::FakeLag(CUserCmd * cmd, CBaseEntity * local, byte* bSendPacket) {
	static int ticks = 0;
	if (gMenu.FakeLag)
	{
		if (local && local->GetHealth() > 0 && !(cmd->buttons & IN_ATTACK))
		{
			if (ticks >= 15)
			{
				*bSendPacket = true;
				ticks = 0;
			}
			else
			{
				*bSendPacket = ticks < 15 - gMenu.FakeLag;
			}
			ticks++;
		}
	}
}

void Rage::AA(CUserCmd * cmd, CBaseEntity * local, byte* bSendPacket) {
	static bool real = false;
	static bool yFlip = true;
	static bool ticksTilFlip = 0;

	float serverTime = local->GetTickBase() * gInterface.globals->interval_per_tick;
	float time = TIME_TO_TICKS(serverTime);

	bool canshoot = (local->GetNextPrimaryAttack() < serverTime) && (local->GetNextAttack() < serverTime);

	if (cmd->buttons & IN_USE || (cmd->buttons & IN_ATTACK && canshoot) || local->GetMoveType() == CBaseEntity::MOVETYPE_LADDER || local->GetMoveType() == CBaseEntity::MOVETYPE_NOCLIP || local->GetHealth() < 1) {
		*bSendPacket = true;
		return;
	}

	if (!gMenu.FakeLag)
		*bSendPacket = cmd->command_number % 2;

	if (local->HasGun() || local->HasKnife()) {

		cmd->viewangles.x = 89;

		float RealAng = cmd->viewangles.y;
		static bool jitter = false;
		if (*bSendPacket)
		{

			while (time >= serverTime)
				time = 0.f;

			float idk = rand() % 100;

			jitter = !jitter;
			if (time >= serverTime / 2)
			{
				if (idk < 70)
				{
					if (!jitter)
						cmd->viewangles.y = RealAng + 55;

				}
				else
				{
					if (!jitter)
						cmd->viewangles.y = RealAng - 55;

				}
			}
			else
			{
				if (idk < 70)
				{
					if (jitter)
						cmd->viewangles.y = RealAng - yaw;
				}
				else
				{
					if (jitter)
						cmd->viewangles.y = RealAng + yaw;

				}
			}
		}
		cmd->viewangles.ClampAngles();
	}
}

void Rage::AutoWall(CUserCmd * cmd, CBaseEntity * local, byte* bSendPacket) {

}

int getBestTarget(CBaseEntity* local)
{
	float bestFov = FLT_MAX;
	int bestEnt = -1;
	QAngle ang;
	gInterface.engine->GetViewAngles(ang);
	for (int i = 1; i < gInterface.engine->GetMaxClients(); ++i) {
		CBaseEntity* ent = gInterface.entityList->GetClientEntity(i);

		if (!ent->IsValid(local, false))
			continue;

		QAngle aim = gMath.CalculateAngle(local->GetEyePosition(), ent->GetHitboxPos(HITBOX_HEAD));
		float FOV = gMath.GetFOV(local->GetEyePosition(), ent->GetHitboxPos(HITBOX_HEAD), gMath.AngleToVector(ang));
		if (FOV < bestFov) {
			bestFov = FOV;
			bestEnt = i;
		}

	}

	return bestEnt;
}

bool Rage::Freestanding(CBaseEntity* player, float& ang)
{

	auto local = player;

	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	static float hold = 0.f;
	Vector besthead;

	auto leyepos = local->GetEyePosition();
	auto headpos = local->GetHitboxPos(0); //GetHitboxPosition(local_player, 0);
	auto origin = local->GetAbsOrigin();

	auto checkWallThickness = [&](CBaseEntity * pPlayer, Vector newhead) -> float
	{

		Vector endpos1, endpos2;

		Vector eyepos = pPlayer->GetEyePosition();
		Ray_t ray;
		ray.Init(newhead, eyepos);
		CTraceFilterSkipTwoEntities filter(pPlayer, local);

		trace_t trace1, trace2;
		gInterface.engineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY | MASK_OPAQUE_AND_NPCS, &filter, &trace1);

		if (trace1.DidHit())
		{
			endpos1 = trace1.endpos;
		}
		else
		{
			return 0.f;
		}

		ray.Init(eyepos, newhead);
		gInterface.engineTrace->TraceRay(ray, MASK_SHOT_BRUSHONLY | MASK_OPAQUE_AND_NPCS, &filter, &trace2);

		if (trace2.DidHit())
		{
			endpos2 = trace2.endpos;
		}

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;

	};

	int index = getBestTarget(local);
	static CBaseEntity* entity;

	if (!local->GetHealth() > 0)
	{
		hold = 0.f;
	}

	if (index != -1)
	{
		entity = (CBaseEntity*)gInterface.entityList->GetClientEntity(index); // maybe?
	}

	if (!entity || entity == nullptr)
	{
		return false;
	}

	float radius = Vector(headpos - origin).Length2D();

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		float step = (M_PI * 2) / 90;

		for (float besthead = 0; besthead < (M_PI * 2); besthead += step)
		{
			Vector newhead(radius * cos(besthead) + leyepos.x, radius * sin(besthead) + leyepos.y, leyepos.z);
			float totalthickness = 0.f;
			no_active = false;
			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;

				bestrotation = besthead;
			}
		}
	}

	if (no_active)
	{
		return false;
	}
	else
	{
		ang = RAD2DEG(bestrotation);
		return true;
	}

	return false;
}

void Rage::DoAntiAim(CUserCmd* cmd, CBaseEntity * local, byte* bSendPacket)
{
	float serverTime = local->GetTickBase() * gInterface.globals->interval_per_tick;
	float time = TIME_TO_TICKS(serverTime);

	bool canshoot = (local->GetNextPrimaryAttack() < serverTime) && (local->GetNextAttack() < serverTime);

	if (local->GetHealth() < 1) {
		return;
	}

	if (cmd->buttons & IN_USE || (cmd->buttons & IN_ATTACK && local->CanShoot()) || local->GetMoveType() == CBaseEntity::MOVETYPE_LADDER || local->GetMoveType() == CBaseEntity::MOVETYPE_NOCLIP) {
		//*bSendPacket = false;
		return;
	}

	if (!gMenu.FakeLag)
		*bSendPacket = cmd->command_number % 2;

	if (local->HasGun() || local->HasKnife())
	{
		Yaw(cmd, local, false);
		YawAdd(cmd, local, false);
		Pitch(cmd, local);
	}

	cmd->viewangles.ClampAngles();
}

void Rage::Pitch(CUserCmd* cmd, CBaseEntity * local)
{
	bool Moving = local->GetVelocity().Length() > 0.1;
	bool InAir = !(local->GetFlags() & FL_ONGROUND);
	bool Standing = !Moving && !InAir;
	static PitchAntiAims mode = PitchAntiAims::NONE;

	float CustomPitch = 0.f;

	if (Standing)
	{
		mode = (PitchAntiAims)gMenu.rage.aaStandYMode;
		CustomPitch = gMenu.rage.aaStandY;
	}
	else if (Moving && !InAir)
	{
		mode = (PitchAntiAims)gMenu.rage.aaMoveYMode;
		CustomPitch = gMenu.rage.aaMoveY;
	}
	else
	{
		mode = (PitchAntiAims)gMenu.rage.aaJumpYMode;
		CustomPitch = gMenu.rage.aaJumpY;
	}

	switch (mode)
	{
	case PitchAntiAims::EMOTION:
		cmd->viewangles.x = 82.f;
		break;

	case PitchAntiAims::DOWN:
		cmd->viewangles.x = 90.f;
		break;

	case PitchAntiAims::UP:
		cmd->viewangles.x = -90.f;
		break;

	case PitchAntiAims::ZERO:
		cmd->viewangles.x = 0.f;
		break;

	case PitchAntiAims::JITTER:
	{
		static int mode = 0;

		switch (mode)
		{
		case 0:
			cmd->viewangles.x = 90.f;
			mode++;
			break;

		case 1:
			cmd->viewangles.x = 0.f;
			mode++;
			break;

		case 2:
			cmd->viewangles.x = -90.f;
			mode++;
			break;

		case 3:
			cmd->viewangles.x = 0.f;
			mode = 0;
			break;
		}

		break;
	}

	case PitchAntiAims::DOWN_JITTER:
	{
		static int mode = 0;

		switch (mode)
		{
		case 0:
			cmd->viewangles.x = 90.f;
			mode++;
			break;

		case 1:
			cmd->viewangles.x = 45.f;
			mode++;
			break;

		case 2:
			cmd->viewangles.x = 0.f;
			mode++;
			break;

		case 3:
			cmd->viewangles.x = 45.f;
			mode = 0;
			break;
		}

		break;
	}

	case PitchAntiAims::UP_JITTER:
	{
		static int mode = 0;

		switch (mode)
		{
		case 0:
			cmd->viewangles.x = -90.f;
			mode++;
			break;

		case 1:
			cmd->viewangles.x = -45.f;
			mode++;
			break;

		case 2:
			cmd->viewangles.x = 0.f;
			mode++;
			break;

		case 3:
			cmd->viewangles.x = -45.f;
			mode = 0;
			break;
		}

		break;
	}

	case PitchAntiAims::ZERO_JITTER:
	{
		static int mode = 0;

		switch (mode)
		{
		case 0:
			cmd->viewangles.x = 45.f;
			mode++;
			break;

		case 1:
			cmd->viewangles.x = 0.f;
			mode++;
			break;

		case 2:
			cmd->viewangles.x = -45.f;
			mode++;
			break;

		case 3:
			cmd->viewangles.x = 0.f;
			mode = 0;
			break;
		}

		break;
	}
	break;

	case PitchAntiAims::SPIN:
	{
		//float pitch = fmodf(gInterface.globals->tickcount * g_Config.GetFloat("rbot_aa_spinbot_speed"), 180.f);
		//Math::NormalizePitch(pitch);
		//cmd->viewangles.x = pitch;
		break;
	}

	case PitchAntiAims::SPIN_UP:
	{
		//float pitch = -fmodf(gInterface.globals->tickcount * g_Config.GetFloat("rbot_aa_spinbot_speed"), 90.f);
		//Math::NormalizePitch(pitch);
		//cmd->viewangles.x = pitch;
		break;
	}

	case PitchAntiAims::SPIN_DOWN:
	{
		//float pitch = fmodf(gInterface.globals->tickcount * g_Config.GetFloat("rbot_aa_spinbot_speed"), 90.f);
		//Math::NormalizePitch(pitch);
		//cmd->viewangles.x = pitch;
		break;
	}

	case PitchAntiAims::RANDOM:
		cmd->viewangles.x = RandomFloat(-90.f, 90.f);
		break;

	case PitchAntiAims::SWITCH:
	{
		static bool sbool = false;

		if (sbool)
		{
			cmd->viewangles.x = 90.f;
		}
		else
		{
			cmd->viewangles.x = -90.f;
		}

		sbool = !sbool;
		break;
	}

	case PitchAntiAims::DOWN_SWITCH:
	{
		static bool sbool = false;

		if (sbool)
		{
			cmd->viewangles.x = 90.f;
		}
		else
		{
			cmd->viewangles.x = 0.f;
		}

		sbool = !sbool;
		break;
	}

	case PitchAntiAims::UP_SWITCH:
	{
		static bool sbool = false;

		if (sbool)
		{
			cmd->viewangles.x = -90.f;
		}
		else
		{
			cmd->viewangles.x = 0.f;
		}

		sbool = !sbool;
		break;
	}
	break;

	case PitchAntiAims::FAKE_UP:
		cmd->viewangles.x = 90.3f;
		break;

	case PitchAntiAims::FAKE_DOWN:
		cmd->viewangles.x = -90.3f;
		break;

	case PitchAntiAims::CUSTOM:
		cmd->viewangles.x = CustomPitch;
		break;

	default:
		break;
	}	
}

void Rage::Yaw(CUserCmd* cmd, CBaseEntity * local, bool fake)
{
	bool Moving = local->GetVelocity().Length() > 0.1;
	bool InAir = !(local->GetFlags() & FL_ONGROUND);
	bool Standing = !Moving && !InAir;
	static YawAntiAims mode = YawAntiAims::NONE;

	float CustomYaw = 0.f;

	if (!fake)
	{
		if (Standing)
		{
			mode = (YawAntiAims)gMenu.rage.aaStandXModeReal;
			CustomYaw = gMenu.rage.aaStandXReal;
		}
		else if (Moving && !InAir)
		{
			mode = (YawAntiAims)gMenu.rage.aaMoveXModeReal;
			CustomYaw = gMenu.rage.aaMoveXReal;
		}
		else
		{
			mode = (YawAntiAims)gMenu.rage.aaJumpXModeReal;
			CustomYaw = gMenu.rage.aaJumpXReal;
		}
	}
	else
	{
		/*if (Standing)
		{
			mode = (YawAntiAims)g_Config.GetInt("rbot_aa_stand_fake_yaw");
			CustomYaw = g_Config.GetFloat("rbot_aa_stand_fake_yaw_custom");
		}
		else if (Moving && !InAir)
		{
			mode = (YawAntiAims)g_Config.GetInt("rbot_aa_move_fake_yaw");
			CustomYaw = g_Config.GetFloat("rbot_aa_move_fake_yaw_custom");
		}
		else
		{
			mode = (YawAntiAims)g_Config.GetInt("rbot_aa_air_fake_yaw");
			CustomYaw = g_Config.GetFloat("rbot_aa_air_fake_yaw_custom");
		}*/
	}

	switch (mode)
	{
	case YawAntiAims::BACKWARDS:
		cmd->viewangles.y -= 180.f;
		break;

	case YawAntiAims::SPINBOT:
		//cmd->viewangles.y = fmodf(gInterface.globals->tickcount * g_Config.GetFloat("rbot_aa_spinbot_speed"), 360.f);
		break;

	case YawAntiAims::LOWER_BODY:
		cmd->viewangles.y = local->m_flLowerBodyYawTarget();
		break;

	case YawAntiAims::RANDOM:
		cmd->viewangles.y = RandomFloat(-180.f, 180.f);
		break;

	case YawAntiAims::FREESTANDING:
	{
		float ang = 0.f;
		bool canuse = Freestanding(local, ang);

		if (!canuse)
		{
			cmd->viewangles.y = local->m_flLowerBodyYawTarget();
		}
		else
		{
			cmd->viewangles.y = ang;
		}

		break;
	}

	case YawAntiAims::CUSTOM:
		cmd->viewangles.y = CustomYaw;
		break;
	default:
		break;
	}
}

void Rage::YawAdd(CUserCmd* cmd, CBaseEntity * local, bool fake)
{
	bool Moving = local->GetVelocity().Length() > 0.1;
	bool InAir = !(local->GetFlags() & FL_ONGROUND);
	bool Standing = !Moving && !InAir;
	static YawAddAntiAims mode = YawAddAntiAims::NONE;

	//float CustomYawAdd = 0.f;
	float YawAddRange = 0.f;

	if (!fake)
	{
		if (Standing)
		{
			mode = (YawAddAntiAims)gMenu.rage.aaStandXAddrModeReal;
			YawAddRange = gMenu.rage.aaStandXAddrReal;
		}
		else if (Moving && !InAir)
		{
			mode = (YawAddAntiAims)gMenu.rage.aaMoveXAddrModeReal;
			YawAddRange = gMenu.rage.aaMoveXAddrReal;
		}
		else
		{
			mode = (YawAddAntiAims)gMenu.rage.aaJumpXModeReal;
			YawAddRange = gMenu.rage.aaJumpXAddrReal;
		}
	}
	else
	{
		/*if (Standing)
		{
			mode = (YawAddAntiAims)g_Config.GetInt("rbot_aa_stand_fake_add_yaw_add");
			YawAddRange = g_Config.GetFloat("rbot_aa_stand_fake_add_yaw_add_range");
		}
		else if (Moving && !InAir)
		{
			mode = (YawAddAntiAims)g_Config.GetInt("rbot_aa_move_fake_add_yaw_add");
			YawAddRange = g_Config.GetFloat("rbot_aa_move_fake_add_yaw_add_range");
		}
		else
		{
			mode = (YawAddAntiAims)g_Config.GetInt("rbot_aa_air_fake_add_yaw_add");
			YawAddRange = g_Config.GetFloat("rbot_aa_air_fake_add_yaw_add_range");
		}*/
	}

	switch (mode)
	{
		case YawAddAntiAims::JITTER:
		{
			static int mode1 = 0;

			switch (mode1)
			{
			case 0:
				cmd->viewangles.y += YawAddRange;
				mode1++;
				break;

			case 1:
				mode1++;
				break;

			case 2:
				cmd->viewangles.y -= YawAddRange;
				mode1 = 0;
				break;
			}

			break;
		}

		case YawAddAntiAims::SWITCH:
		{
			static bool sbool = false;

			if (sbool)
			{
				cmd->viewangles.y += YawAddRange;
			}
			else
			{
				cmd->viewangles.y -= YawAddRange;
			}

			sbool = !sbool;
			break;
		}

		case YawAddAntiAims::SPIN:
		{
			//cmd->viewangles.y += fmodf(gInterface.globals->tickcount * g_Config.GetFloat("rbot_aa_spinbot_speed"), YawAddRange);
			break;
		}

		case YawAddAntiAims::RANDOM:
			cmd->viewangles.y += RandomFloat(-YawAddRange, YawAddRange);
			break;

		default:
			break;
	}
}


