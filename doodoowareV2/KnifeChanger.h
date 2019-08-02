#pragma once
#include "SourceSDK\sdk.h"
#include <map>
//#include "SourceSDK\Misc\CBaseCombatWeapon.h"

class CBaseAttributableItem;

class KnifeChanger
{
public:
	void setKnife(CBaseEntity* local, CBaseAttributableItem* weap);
	
	//std::map<int, int> weaptoskin;


	std::vector<std::pair<int, int>> weaptoskin;

};
extern KnifeChanger gKnifeChanger;