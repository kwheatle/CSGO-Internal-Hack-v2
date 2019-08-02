#include "EngPred.h"

float flOldCurtime;
float flOldFrametime;
int iFlags;
int iMovetype;
int oldtickcount;


/*const auto getRandomSeed = [&]() {

using MD5_PseudoRandomFn = unsigned long(__cdecl*)(std::uintptr_t);
static auto offset = gInterface.FindPattern("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 70 6A 58");
static auto MD5_PseudoRandom = reinterpret_cast<MD5_PseudoRandomFn>(offset);
return MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;
};*/

void EngPred::prestart() {
	if (!gInterface.engine->IsInGame())
		return;

	if (gInterface.clientState->m_delta_tick > 0) {
		gInterface.prediction->Update(gInterface.clientState->m_delta_tick, gInterface.clientState->m_delta_tick > 0,
			gInterface.clientState->m_last_command_ack, gInterface.clientState->m_last_outgoing_command + gInterface.clientState->m_choked_commands);
	}
}

int EngPred::post_think(CBaseEntity* player) const {

	using PostThinkVPhysicsFn = bool(__thiscall*)(CBaseEntity *);
	static auto PostThinkVPhysics = reinterpret_cast<PostThinkVPhysicsFn>(gInterface.FindPattern("client_panorama.dll", "55 8B EC 83 E4 F8 81 EC ?? ?? ?? ?? 53 8B D9"));

	using SimulatePlayerSimulatedEntitiesFn = void(__thiscall*)(CBaseEntity *);
	static auto SimulatePlayerSimulatedEntities = reinterpret_cast<SimulatePlayerSimulatedEntitiesFn>(gInterface.FindPattern("client_panorama.dll", "56 8B F1 57 8B BE ?? ?? ?? ?? 83 EF 01"));

	//util::misc::vfunc< void(__thiscall *)(void *) >(g_csgo.m_modelcache, 33)(g_csgo.m_modelcache);
	using unknFn = void(__thiscall *)(void *);
	using unkn1Fn = void(__thiscall *)(void *, int);
	using unkn2Fn = int(__thiscall *)(void *);
	gInterface.CallVirtualFunction<unknFn>(gInterface.mdlCache, 33, gInterface.mdlCache);
	if (player->GetHealth() > 0) {

		gInterface.CallVirtualFunction<unknFn>(player, 334, player);

		if (player->GetFlags() & FL_ONGROUND)
			*reinterpret_cast< uintptr_t * >(uintptr_t(player) + 0x3014) = 0;

		if (*reinterpret_cast< int * >(uintptr_t(player) + 0x28BC) == -1)
			gInterface.CallVirtualFunction<unkn1Fn>(player, 214,player, 0);

		gInterface.CallVirtualFunction<unknFn>(player, 215, player);

		PostThinkVPhysics(player);
	}
	SimulatePlayerSimulatedEntities(player);

	gInterface.CallVirtualFunction<unkn2Fn>(gInterface.mdlCache, 34, gInterface.mdlCache);
	return 1;
}

void EngPred::startpred(CUserCmd * cmd)
{
	static int flTickBase;
	static CUserCmd* pLastCmd;

	auto local_player = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());

	//if (!local_player)
		//return;

	if (pLastCmd) {
		if (pLastCmd->hasbeenpredicted)
			flTickBase = local_player->GetTickBase();
		else
			++flTickBase;
	}

	pLastCmd = cmd;

	const int old_tickbase = local_player->GetTickBase();

	//	CPrediction::StartCommand
	{

		//*reinterpret_cast< CUserCmd ** >(uint32_t(local_player) + 0x3334) = cmd; // m_pCurrentCommand
		//*reinterpret_cast< CUserCmd ** >(uint32_t(local_player) + 0x3288) = cmd; // unk01
	}

	flOldCurtime = gInterface.globals->curtime;
	flOldFrametime = gInterface.globals->frametime;
	iFlags = local_player->GetFlags();
	iMovetype = local_player->GetMoveType();
	oldtickcount = gInterface.globals->tickcount;
	

	const bool old_in_prediction = gInterface.prediction->InPrediction();
	const bool old_first_prediction = gInterface.prediction->IsFirstTimePredicted();

	gInterface.globals->curtime = flTickBase * gInterface.globals->interval_per_tick;
	gInterface.globals->frametime = gInterface.globals->interval_per_tick;

	gInterface.globals->tickcount = local_player->GetTickBase();

	*(bool*)((uint32_t)gInterface.prediction + 0x18) = false;
	*(bool*)((uint32_t)gInterface.prediction + 0x8) = true;

	gInterface.movement->StartTrackPredictionErrors(local_player);

	if (cmd->impulse)
		*reinterpret_cast< uint32_t * >(uint32_t(local_player) + 0x31FC) = cmd->impulse;

	//	CBasePlayer::UpdateButtonState
	{
		cmd->buttons |= *reinterpret_cast< uint32_t * >(uint32_t(local_player) + 0x3330);

		const int v16 = cmd->buttons;
		int *unk02 = reinterpret_cast< int * >(uint32_t(local_player) + 0x31F8);
		const int v17 = v16 ^ *unk02;

		*reinterpret_cast< int * >(uint32_t(local_player) + 0x31EC) = *unk02;
		*reinterpret_cast< int * >(uint32_t(local_player) + 0x31F8) = v16;
		*reinterpret_cast< int * >(uint32_t(local_player) + 0x31F0) = v16 & v17;
		*reinterpret_cast< int * >(uint32_t(local_player) + 0x31F4) = v17 & ~v16;
	}

	//	check if player is standing on moving ground
	gInterface.prediction->CheckMovingGround(local_player, gInterface.globals->framecount);

	//	copy from command to player


	//	CPrediction::RunPreThink
	{
		//	THINK_FIRE_ALL_FUNCTIONS
		//if (local_player->physics_run_think(0)) {
		//	local_player->pre_think();
		//}
	}

	local_player->pre_think();
	local_player->think();
	local_player->post_think();

	//	CPrediction::RunThink
	{
		//const auto next_think = reinterpret_cast< int * >(uint32_t(local_player) + 0xFC);

		//if (*next_think > 0 && *next_think <= local_player->tickbase()) {
			//	TICK_NEVER_THINK
		//	*next_think = -1;

		//	local_player->think();
		//}
	}

	
	memset(&data, 0, sizeof(CMoveData));

	gInterface.moveHelper->SetHost(local_player);
	gInterface.prediction->SetupMove(local_player, cmd, gInterface.moveHelper, &data);
	gInterface.movement->ProcessMovement(local_player, &data);
	gInterface.prediction->FinishMove(local_player, cmd, &data);
	gInterface.moveHelper->ProcessImpacts();
	//	CPrediction::RunPostThink
	{
		//post_think(local_player);
	}

	//	restore tickbase
	*(int*)((uint32_t)local_player + Offsets::m_nTickBase) = old_tickbase;

	//	restore prediction
	*(bool*)((uint32_t)gInterface.prediction + 0x18) = old_first_prediction;
	*(bool*)((uint32_t)gInterface.prediction + 0x8) = old_in_prediction;
}

void EngPred::endpred(CUserCmd* cmd)
{
	auto local_player = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());

	if (!local_player)
		return;

	*(int*)((uint32_t)local_player + Offsets::m_fFlags) = iFlags;
	*(int*)((uint32_t)this + Offsets::m_MoveType) = iMovetype;
	gInterface.globals->curtime = flOldCurtime;
	gInterface.globals->frametime = flOldFrametime;
	gInterface.globals->tickcount = oldtickcount;

	//	CPrediction::FinishCommand
	{
		*reinterpret_cast< uint32_t * >(reinterpret_cast< uint32_t >(local_player) + 0x3334) = 0;
		*reinterpret_cast< CUserCmd ** >(uint32_t(local_player) + 0x3334) = nullptr;

	}

	gInterface.movement->FinishTrackPredictionErrors(local_player);
	gInterface.moveHelper->SetHost(0);

	gInterface.movement->Reset();
}
