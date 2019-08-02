#include "KnifeChanger.h"
#include "Visuals.h"
#include "SourceSDK\Misc\CBaseCombatWeapon.h"
#include <unordered_map>
//#include "SourceSDK\Definitions\Definitions.h"


char* temp[] = {
	"models/weapons/v_knife_karam.mdl",
	"models/weapons/v_knife_survival_bowie.mdl",
	"models/weapons/v_knife_push.mdl",
	"models/weapons/v_knife_falchion_advanced.mdl",
	"models/weapons/v_knife_butterfly.mdl",
	"models/weapons/v_knife_bayonet.mdl",
	"models/weapons/v_knife_flip.mdl",
	"models/weapons/v_knife_gut.mdl",
	"models/weapons/v_knife_tactical.mdl",
	"models/weapons/v_knife_ursus.mdl",
	"models/weapons/v_knife_gypsy_jackknife.mdl",
	"models/weapons/v_knife_stiletto.mdl",
	"models/weapons/v_knife_widowmaker.mdl",
	"models/weapons/v_knife_m9_bay.mdl",
	"models/weapons/v_knife_default_ct.mdl",
	"models/weapons/v_knife_default_t.mdl"
};

int temp2[] = {
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_SURVIVAL_BOWIE,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_FALCHION,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_BAYONET,
	WEAPON_KNIFE_FLIP,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_URSUS,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE,
	WEAPON_KNIFE_T,
};


bool once = false;
bool onceper = false;
void KnifeChanger::setKnife(CBaseEntity* local, CBaseAttributableItem* weap)
{
	static std::unordered_map<int, int> knifetodef;

	if (!once) {
		for (int i = 0; i < ARRAYSIZE(temp); ++i) {
			knifetodef.emplace(std::make_pair(gInterface.modelInfo->GetModelIndex(temp[i]), temp2[i]));
		}


		for (int i = 0; i < Weapons::guns.size(); ++i) {
			weaptoskin.emplace_back(std::make_pair(Weapons::guns.at(i), 6));
		}

		for (int i = 0; i < Weapons::knives.size(); ++i) {
			weaptoskin.emplace_back(std::make_pair(Weapons::knives.at(i), 6));
		}
		gInterface.clientState->force_full_update();
		once = true;
	}

		auto pViewModel = (CBaseViewModel*)gInterface.entityList->GetClientEntityFromHandle(*local->GetViewModel());
		if (!pViewModel) return;

		//weap = (CBaseAttributableItem*)gInterface.entityList->GetClientEntityFromHandle(local->GetWeaponHandle());
		auto worldmodel = gInterface.entityList->GetClientEntityFromHandle(weap->GetWorldModel());

		int nViewModelIndex = *pViewModel->GetModelIndex();

		const auto nOriginalKnifeCT = gInterface.modelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		const auto nOriginalKnifeT = gInterface.modelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");

		
		if (nViewModelIndex == nOriginalKnifeCT || nViewModelIndex == nOriginalKnifeT) {
			int index = gInterface.modelInfo->GetModelIndex(gVis.knife);
			*pViewModel->GetModelIndex() = index;
			*weap->GetViewModelIndex() = index;
			if(worldmodel) *worldmodel->GetModelIndex() = index + 1;
			
			*weap->GetModelIndex() = index;
			*weap->GetItemDefinitionIndex() = knifetodef.find(index)->second;
		}

		*weap->GetEntityQuality() = 3;

		int idx = *weap->GetItemDefinitionIndex() & 0xFFF;

		auto it = std::find_if(weaptoskin.begin(), weaptoskin.end(),
				[&idx](const std::pair<int, int>& element) {
				return element.first == idx;
			});

		if (idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_T) {
			int index = gInterface.modelInfo->GetModelIndex(gVis.knife);
			*weap->GetModelIndex() = index;
			*weap->GetItemDefinitionIndex() = knifetodef.find(index)->second;
		}

		if (gMenu.bSkins) {
			*weap->GetFallbackPaintKit() = it->second;
			*weap->OwnerXuidLow() = 0;// local->GetOriginalOwnerXuidLow();
			*weap->OwnerXuidHigh() = 0;// local->GetOriginalOwnerXuidHigh();
			*weap->FallbackWear() = 0.0000000001f;
			*weap->ItemIDHigh() = -1;
		}

}


