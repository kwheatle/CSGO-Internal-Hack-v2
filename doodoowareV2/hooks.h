#pragma once
#include "SourceSDK\sdk.h"
#include "vmt.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9")
#include "Interfaces.h"
#include <d3d9.h>
#include "Menu.h"
#include "imgui\imgui_impl_win32.h"
#include "SourceSDK\Math\Vector.h"


class hooks
{
public:
	bool UNLOAD = false;
	void Hook();
	void unhook();
	
	vmt_hook clientModeHook;
	vmt_hook clientHook;
	vmt_hook d3dhook;
	vmt_hook panelhook;
	vmt_hook mdlRenderhook;
	vmt_hook surfacehook;
	vmt_hook compasshook;
private:
	void setupHook();
public:

	typedef bool(__stdcall* CreateMoveFn)(float, CUserCmd*);
	static CreateMoveFn oCreateMove;
	static bool __stdcall HookedCreateMove(float _inputSampleTime, CUserCmd* _cmd);

	using EndSceneFn = void(__stdcall*)(IDirect3DDevice9*);
	static EndSceneFn oEndScene;
	static void __stdcall hkEndScene(IDirect3DDevice9* device);

	using LockCursorFn = void(__fastcall*)(ISurface*, void*);
	static LockCursorFn OriginalLockCursor;
	static void __fastcall hkLockCursor(ISurface* thisptr, void* edx);

	using PaintTraverseFn = void(__stdcall* )(VPANEL, bool, bool);
	static PaintTraverseFn OriginalPaintTraverse;
	static void __stdcall HookedPaintTraverse(VPANEL _vguiPanel, bool _forceRepaint, bool _allowForce);

	using ResetFn = HRESULT(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	static ResetFn oReset;
	static HRESULT __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);

	using FrameStageNotifyFn = void(__thiscall *) (void*, ClientFrameStage_t);
	static FrameStageNotifyFn OriginalFSN;
	static void __fastcall HookedFSN(void* ecx, void* edx, ClientFrameStage_t stage);

	using DrawModelExecuteFn = void(__thiscall* )(IVModelRender*, IMatRenderContext *, const DrawModelState_t &, const ModelRenderInfo_t &, matrix3x4_t *);
	static DrawModelExecuteFn OriginalDME;
	static void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);

	using CompassFn = bool(__thiscall*)(void*);
	static bool __fastcall hkShouldDrawCompass(void* ecx, void* edx);

	using OverrideViewfn = void(__thiscall*)(void*, CViewSetup*);
	static OverrideViewfn oOverride;
	static void __fastcall hkOverrideView(void* ecx, void* edx, CViewSetup* setup);

	WNDPROC OldWindow = NULL;
	HWND Window = NULL;
	static LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
extern hooks hook;