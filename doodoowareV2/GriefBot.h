#pragma once
#include "hooks.h"

class GriefBot
{
public:
	void run(CUserCmd* cmd, CBaseEntity* local);
	void CopyBot(CUserCmd* cmd, CBaseEntity* local);
	void BlockBot(CUserCmd* cmd, CBaseEntity* local);

private:

};

extern GriefBot gGrief;