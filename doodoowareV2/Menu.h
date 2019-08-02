#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/directx9/imgui_impl_dx9.h"
#include <d3d9.h>
#include "Bhop.h"
#include "Rage.h"
#include "Backtrack.h"
#include "Aimbot.h"
#include "Visuals.h"
#include "Draw.h"
//#include "skinids.h"
//#include <d3dx9.h>

typedef unsigned long HFont;

class Menu
{
public:
	void GUI_Init();
	void Render();
	void DrawLegit();
	void DrawVisual();
	void DrawMisc();
	void DrawAA();
	void DrawCfg();
	bool IsActive();

	bool nightmode = false;
	bool engPred = true;
	bool noCrouchCooldown = true;
	bool wiresmoke = false;
	bool bSkins = false;

	float cameraOffset = 150;
	float hue = 0.5f;

	int FakeLag = 0;
	bool noScope = false;
	bool tpEnable = false;
	int fov = 90;

	//static const int h = skins.size();

	char* skinList[763];
	char* weapList[49];

	int resolutionW;
	int resolutionH;
	int flipTicks = 10;

	HWND wnd;
	bool d3dinit = false;
	bool active = false;
	bool bhopEnable;
	ImGuiStyle _style;
	ImGuiStyle style;

public:
	struct Fonts
	{
		HFont watermark;
		HFont text;
		HFont esp;
	} fonts;

	struct Rage
	{
		int aaStandYMode;
		int aaMoveYMode;
		int aaJumpYMode;
		float aaStandY;
		float aaMoveY;
		float aaJumpY;

		int aaStandXModeReal;
		int aaMoveXModeReal;
		int aaJumpXModeReal;
		float aaStandXReal;
		float aaMoveXReal;
		float aaJumpXReal;



		int aaStandXAddrModeReal;
		int aaMoveXAddrModeReal;
		int aaJumpXAddrModeReal;
		float aaStandXAddrReal;
		float aaMoveXAddrReal;
		float aaJumpXAddrReal;


	}rage;

private:
	
};

extern Menu gMenu;