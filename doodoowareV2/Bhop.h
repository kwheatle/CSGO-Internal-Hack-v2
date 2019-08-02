#pragma once
#include "hooks.h"

class Bhop
{
public:
	bool enable = true;

	void bhop(CBaseEntity* , CUserCmd* );

private:

};
extern Bhop gBhop;
