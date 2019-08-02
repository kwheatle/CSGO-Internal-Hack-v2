#include "Bhop.h"

void Bhop::bhop(CBaseEntity* localPlayer, CUserCmd* cmd) {

	if (!enable)
		return;

	if (localPlayer->GetMoveType() != CBaseEntity::MOVETYPE_LADDER) {
		if (!(localPlayer->GetFlags() & FL_ONGROUND) && cmd->buttons & IN_JUMP) {
			cmd->buttons &= ~IN_JUMP;
		}
	}
}
