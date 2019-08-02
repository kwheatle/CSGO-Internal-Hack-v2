#pragma once
#include <Windows.h>
#include "hooks.h"



hooks h;
HMODULE dll;
DWORD WINAPI Main(LPVOID base)
{
	while (!GetModuleHandle("serverbrowser.dll"))	// Wait for game to load completely
		Sleep(100);

	h.Hook();

	while (!GetAsyncKeyState(VK_END)) { Sleep(100); }
	gInterface.engine->ExecuteClientCmd("clear");
	gInterface.engine->ExecuteClientCmd("echo UNLOADED");
	//h.UNLOAD = true;

	
	//SetWindowLongPtr(h.Window, GWL_WNDPROC, (LONG_PTR)h.OldWindow);
	//FreeLibraryAndExitThread(dll, 1);
	
	return 0;
}



BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {
		dll = hinstDLL;
		DisableThreadLibraryCalls(dll);
		CreateThread(nullptr, 0, Main, dll, 0, nullptr);		
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		h.unhook();
		Sleep(1000);
		ImGui_ImplDX9_Shutdown();
		return TRUE;
	}

	return TRUE;
}