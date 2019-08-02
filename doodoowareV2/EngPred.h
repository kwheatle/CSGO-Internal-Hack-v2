#pragma once
#include "SourceSDK\sdk.h"

class EngPred
{
public:
	int post_think(CBaseEntity * player) const;
	void startpred(CUserCmd* cmd);
	static void prestart();
	void endpred(CUserCmd* cmd);
	CMoveData data;
	int* m_pPredictionRandomSeed = nullptr;
private:

};
extern EngPred gEngPred;
