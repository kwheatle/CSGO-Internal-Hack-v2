#pragma once
#include "hooks.h"
//$(ProjectName)


void hooks::setupHook()
{
	;;;;;;;; NULL; void;


	//glow_manager = (glow_manager_t *)(*(uintptr_t *)(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3));

	gInterface.client = gInterface.FindInterface<IBaseClientDLL>("client_panorama.dll", "VClient");
	gInterface.clientMode = **(ClientModeShared***)((*(DWORD**)gInterface.client)[10] + 0x5);
	gInterface.globals = **(CGlobalVarsBase***)((*(DWORD**)gInterface.client)[0] + 0x1B);
	gInterface.engine = gInterface.FindInterface<IVEngineClient>("engine.dll", "VEngineClient");
	gInterface.entityList = gInterface.FindInterface<IClientEntityList>("client_panorama.dll", "VClientEntityList");
	gInterface.surface = gInterface.FindInterface<ISurface>("vguimatsurface.dll", "VGUI_Surface");
	gInterface.debug = gInterface.FindInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay");
	gInterface.panel = gInterface.FindInterface<IPanel>("vgui2.dll", "VGUI_Panel");
	gInterface.modelInfo = gInterface.FindInterface<IVModelInfo>("engine.dll", "VModelInfoClient");
	gInterface.engineTrace = gInterface.FindInterface<IEngineTrace>("engine.dll", "EngineTraceClient");
	gInterface.cvar = gInterface.FindInterface<ICVar>("vstdlib.dll", "VEngineCvar");
	gInterface.matsystem = gInterface.FindInterface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
	gInterface.mdlRender = gInterface.FindInterface<IVModelRender>("engine.dll", "VEngineModel");

	gInterface.prediction = gInterface.FindInterface<CPrediction>("client_panorama.dll", "VClientPrediction");
	gInterface.movement = gInterface.FindInterface<IGameMovement>("client_panorama.dll", "GameMovement");
	gInterface.moveHelper = **(IMoveHelper***)((gInterface.FindPattern("client_panorama.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2));
	gInterface.mdlCache = gInterface.FindInterface<IMdlCache>("datacache.dll", "MDLCache");
	gInterface.clientState = **(ClientState***)(gInterface.FindPattern("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);

	gInterface.physicsProps = gInterface.FindInterface<IPhysicsProps>("vphysics.dll", "VPhysicsSurfaceProps");


	gInterface.input = (CInput*)*(DWORD*)(gInterface.FindPattern("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1);
}
void hooks::Hook()
{
	setupHook();
	
	clientModeHook.setup(gInterface.clientMode);//, "client_panorama.dll");
	clientModeHook.hook_index(Indexes::ClientMode::CreateMove, this->HookedCreateMove);
	oCreateMove = clientModeHook.get_original<CreateMoveFn>(Indexes::ClientMode::CreateMove);

	clientModeHook.hook_index(18, hkOverrideView);
	oOverride = clientModeHook.get_original<OverrideViewfn>(18);

	clientHook.setup(gInterface.client);// , "client_panorama.dll");
	clientHook.hook_index(37, this->HookedFSN);
	OriginalFSN = clientHook.get_original<FrameStageNotifyFn>(37);

	surfacehook.setup(gInterface.surface);
	surfacehook.hook_index(67, hkLockCursor);
	OriginalLockCursor = surfacehook.get_original<LockCursorFn>(67);

	panelhook.setup(gInterface.panel);
	panelhook.hook_index(Indexes::Panel::PaintTraverse, this->HookedPaintTraverse);
	OriginalPaintTraverse = panelhook.get_original<PaintTraverseFn>(Indexes::Panel::PaintTraverse);

	DWORD g_D3DDevice9 = **(DWORD**)(gInterface.FindPattern("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 0x1);

	d3dhook.setup((DWORD**)g_D3DDevice9);// , "shaderapidx9.dll");
	d3dhook.hook_index(42, this->hkEndScene);//End Scene
	oEndScene = d3dhook.get_original<EndSceneFn>(42);

	d3dhook.hook_index(16, this->hkReset);//Reset
	oReset = d3dhook.get_original<ResetFn>(16);

	mdlRenderhook.setup(gInterface.mdlRender);// , "engine.dll");
	mdlRenderhook.hook_index(21, this->hkDrawModelExecute);
	OriginalDME = mdlRenderhook.get_original<DrawModelExecuteFn>(21);

	DWORD compassVtable = *(DWORD*)(gInterface.FindPattern("client_panorama.dll", "C7 47 ? ? ? ? ? 8D 97 ? ? ? ?") + 0x3);
	//compasshook.setup((DWORD*)compassVtable);
	//compasshook.hook_index(13, hkShouldDrawCompass);

	gInterface.engine->ExecuteClientCmd("clear");



	//gInterface.engine->ExecuteClientCmd("echo INJECTED");
}

bool __fastcall hooks::hkShouldDrawCompass(void* ecx, void* edx) {
	gInterface.engine->ExecuteClientCmd("echo RUNNING");
	return gMenu.noCrouchCooldown;

}

void hooks::unhook()
{
	clientModeHook.~vmt_hook();
	clientHook.~vmt_hook();
	d3dhook.~vmt_hook();
	surfacehook.~vmt_hook();
	panelhook.~vmt_hook();
	mdlRenderhook.~vmt_hook();
}
