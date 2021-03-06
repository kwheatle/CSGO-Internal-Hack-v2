#pragma once
#include <vector>
#include <map>

namespace Weapons
{
	enum ItemDefinitionIndex : int
	{
		WEAPON_NONE = 0,

		WEAPON_DEAGLE,
		WEAPON_ELITE,
		WEAPON_FIVESEVEN,
		WEAPON_GLOCK,
		WEAPON_AK47 = 7,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_GALILAR = 13,
		WEAPON_M249,
		WEAPON_M4A1 = 16,
		WEAPON_MAC10,
		WEAPON_P90 = 19,
		WEAPON_MP5_SD = 23,
		WEAPON_UMP45,
		WEAPON_XM1014,
		WEAPON_BIZON,
		WEAPON_MAG7,
		WEAPON_NEGEV,
		WEAPON_SAWEDOFF,
		WEAPON_TEC9,
		WEAPON_TASER,
		WEAPON_HKP2000,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NOVA,
		WEAPON_P250,
		WEAPON_SHIELD,
		WEAPON_SCAR20,
		WEAPON_SG556,
		WEAPON_SSG08,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_FLASHBANG,
		WEAPON_HEGRENADE,
		WEAPON_SMOKEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE,
		WEAPON_C4,
		WEAPON_HEALTHSHOT = 57,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1_SILENCER = 60,
		WEAPON_USP_SILENCER,
		WEAPON_CZ75A = 63,
		WEAPON_REVOLVER,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,
		WEAPON_BREACHCHARGE,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,
		WEAPON_HAMMER,
		WEAPON_SPANNER = 78,
		WEAPON_KNIFE_GHOST = 80,
		WEAPON_FIREBOMB,
		WEAPON_DIVERSION,
		WEAPON_FRAG_GRENADE,
		WEAPON_SNOWBALL,
		WEAPON_BUMPMINE,
		WEAPON_BAYONET = 500,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_WIDOWMAKER,
		GLOVE_STUDDED_BLOODHOUND = 5027,
		GLOVE_T_SIDE = 5028,
		GLOVE_CT_SIDE = 5029,
		GLOVE_SPORTY = 5030,
		GLOVE_SLICK = 5031,
		GLOVE_LEATHER_WRAP = 5032,
		GLOVE_MOTORCYCLE = 5033,
		GLOVE_SPECIALIST = 5034,
		GLOVE_HYDRA = 5035
	};

	const std::vector<int>automatic
	{
		WEAPON_AK47,
		WEAPON_M4A1,
		WEAPON_M4A1_SILENCER,
		WEAPON_SG556,
		WEAPON_AUG,
		WEAPON_GALILAR,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_SCAR20,
		WEAPON_M249,
		WEAPON_NEGEV,
		WEAPON_MAC10,
		WEAPON_P90,
		WEAPON_UMP45,
		WEAPON_BIZON,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_CZ75A
	};

	const std::vector<int>bolt_action
	{
		WEAPON_AWP,
		WEAPON_SSG08
	};

	const std::vector<int>guns
	{
		WEAPON_AK47,
		WEAPON_M4A1,
		WEAPON_M4A1_SILENCER,
		WEAPON_SG556,
		WEAPON_AUG,
		WEAPON_GALILAR,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_SCAR20,
		WEAPON_M249,
		WEAPON_NEGEV,
		WEAPON_MAC10,
		WEAPON_P90,
		WEAPON_UMP45,
		WEAPON_BIZON,
		WEAPON_MP7,
		WEAPON_MP5_SD,
		WEAPON_MP9,
		WEAPON_CZ75A,
		WEAPON_USP_SILENCER,
		WEAPON_HKP2000,
		WEAPON_GLOCK,
		WEAPON_P250,
		WEAPON_FIVESEVEN,
		WEAPON_TEC9,
		WEAPON_ELITE,
		WEAPON_DEAGLE,
		WEAPON_REVOLVER,
		WEAPON_NOVA,
		WEAPON_XM1014,
		WEAPON_MAG7,
		WEAPON_SAWEDOFF,
		WEAPON_SSG08,
		WEAPON_AWP
	};


	const std::vector<int>knives
	{
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_KNIFE_T,
		WEAPON_BAYONET,
		WEAPON_KNIFE_FLIP,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION,
		WEAPON_KNIFE_SURVIVAL_BOWIE,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_URSUS,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_STILETTO,
		WEAPON_KNIFE_WIDOWMAKER,
	};

	const std::map<int, char*> idxToName =
	{
		{ WEAPON_AK47, "AK-47" },
		{ WEAPON_AUG, "AUG" },
		{ WEAPON_AWP, "AWP" },
		{ WEAPON_CZ75A, "CZ75 Auto" },
		{ WEAPON_DEAGLE, "Desert Eagle" },
		{ WEAPON_ELITE, "Dual Berettas" },
		{ WEAPON_FAMAS, "FAMAS" },
		{ WEAPON_FIVESEVEN, "Five-SeveN" },
		{ WEAPON_G3SG1, "G3SG1" },
		{ WEAPON_GALILAR, "Galil AR" },
		{ WEAPON_GLOCK, "Glock-18" },
		{ WEAPON_M249, "M249" },
		{ WEAPON_M4A1_SILENCER, "M4A1-S" },
		{ WEAPON_M4A1, "M4A4" },
		{ WEAPON_MAC10, "MAC-10" },
		{ WEAPON_MAG7, "MAG-7" },
		{ WEAPON_MP5_SD, "MP5-SD" },
		{ WEAPON_MP7, "MP7" },
		{ WEAPON_MP9, "MP9" },
		{ WEAPON_NEGEV, "Negev" },
		{ WEAPON_NOVA, "Nova" },
		{ WEAPON_HKP2000, "P2000" },
		{ WEAPON_P250, "P250" },
		{ WEAPON_P90, "P90" },
		{ WEAPON_BIZON, "PP-Bizon" },
		{ WEAPON_REVOLVER, "R8 Revolver" },
		{ WEAPON_SAWEDOFF, "Sawed-Off" },
		{ WEAPON_SCAR20, "SCAR-20" },
		{ WEAPON_SSG08, "SSG 08" },
		{ WEAPON_SG556, "SG 553" },
		{ WEAPON_TEC9, "Tec-9" },
		{ WEAPON_UMP45, "UMP-45" },
		{ WEAPON_USP_SILENCER, "USP-S" },
		{ WEAPON_XM1014, "XM1014" },
		{ WEAPON_KNIFE, "Knife CT"},
		{ WEAPON_KNIFE_T, "Knife T" },
		{ WEAPON_BAYONET, "Bayonet"},
		{ WEAPON_KNIFE_FLIP, "Flip Knife" },
		{ WEAPON_KNIFE_GUT, "Gut Knife" },
		{ WEAPON_KNIFE_KARAMBIT, "Karambit" },
		{ WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" },
		{ WEAPON_KNIFE_TACTICAL, "Huntsman Knife" },
		{ WEAPON_KNIFE_FALCHION, "Flachion Knife" },
		{ WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife"},
		{ WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" },
		{ WEAPON_KNIFE_PUSH, "Push Knife" },
		{ WEAPON_KNIFE_URSUS, "Ursus Knife" },
		{ WEAPON_KNIFE_GYPSY_JACKKNIFE, "Gypsy knife" },
		{ WEAPON_KNIFE_STILETTO, "Stiletto" },
		{ WEAPON_KNIFE_WIDOWMAKER, "Windowmaker" },
	};
}
