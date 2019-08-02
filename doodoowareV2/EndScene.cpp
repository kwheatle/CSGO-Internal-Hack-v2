#pragma once
#include "hooks.h"
#include "../doodoowareV2/imgui/imgui_impl_win32.h"
#include "render.h"
Menu gMenu;
hooks hook;
hooks::EndSceneFn hooks::oEndScene;
void __stdcall hooks::hkEndScene(IDirect3DDevice9* device) {

	if (hook.UNLOAD) {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		//return oEndScene(device);
	}


	static bool once = false;
	int w, h;
	gInterface.engine->GetScreenSize(w, h);
	if (!once) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.DeltaTime = 1.0f / 60.0f;
		D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
		device->GetCreationParameters(&d3dcp);
		hook.Window = d3dcp.hFocusWindow;
		hook.OldWindow = (WNDPROC)SetWindowLongPtr(hook.Window, GWL_WNDPROC, (LONG)(LONG_PTR)hook.Hooked_WndProc);
		io.Fonts->AddFontDefault();

		//ImGui::StyleColorsClassic();

		ImGuiStyle* style = &ImGui::GetStyle();
		io.IniFilename = NULL;

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;
		style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		//style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
		//style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
		//style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

		/*io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 12);
		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 10);
		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 14);
		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 18);*/

		
		ImGui_ImplWin32_Init(d3dcp.hFocusWindow);
		ImGui_ImplDX9_Init(device);
		Render::Get().Initialize();
		gMenu.GUI_Init();
		gMenu.d3dinit = true;
		once = true;
	}

	ImGuiIO& io = ImGui::GetIO();

	DWORD colorwrite, srgbwrite;
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
	device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	//removes the source engine color correction
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

	device->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	device->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);


	ImGui_ImplDX9_NewFrame(); //DOES NOTHING
	ImGui_ImplWin32_NewFrame();
	io.DisplaySize = ImVec2{ (float)w, (float)h };
	ImGui::NewFrame();

	io.WantCaptureMouse = gMenu.active;
	//io.WantCaptureKeyboard = gMenu.active;
	io.MouseDrawCursor = gMenu.active;

	if (gMenu.active) {
		gMenu.Render();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);

	return oEndScene(device);
}
hooks::ResetFn hooks::oReset;
HRESULT __stdcall hooks::hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	if (!gMenu.d3dinit) {
		return oReset(device, pPresentationParameters);
	}

	ImGui_ImplDX9_InvalidateDeviceObjects();

	return oReset(device, pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return oReset(device, pPresentationParameters);
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK hooks::Hooked_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (hook.UNLOAD)
		return CallWindowProcA(hook.OldWindow, hwnd, msg, wParam, lParam);

	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		break;
	case WM_KEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		break;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	{

		static bool isDown = false;
		static bool isClicked = false;
		if (io.KeysDown[VK_INSERT]) {
			isClicked = false;
			isDown = true;
		}
		else if (!io.KeysDown[VK_INSERT] && isDown) {
			isClicked = true;
			isDown = false;
		}
		else {
			isClicked = false;
			isDown = false;
		}

		if (isClicked)
		{
			gMenu.active = !gMenu.active;
		}

	}

	if (gMenu.active && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return 1l;

	return CallWindowProcA(hook.OldWindow, hwnd, msg, wParam, lParam);
}

hooks::LockCursorFn hooks::OriginalLockCursor;
void __fastcall hooks::hkLockCursor(ISurface* thisptr, void* edx) {
	if (gMenu.active) {
		gInterface.surface->UnlockCursor();
		return;
	}

	OriginalLockCursor(thisptr, edx);
}

