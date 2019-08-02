#include "Menu.h"
#include "skinids.h"
#include "KnifeChanger.h"
#include <map>

void Menu::GUI_Init()
{
	this->fonts.watermark = draw.SetupFont(60, "Comic Sans MS", FONTFLAG_ANTIALIAS | FONTFLAG_ADDITIVE | FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
	this->fonts.text = draw.SetupFont(17, "Calibri", FONTFLAG_ANTIALIAS | FONTFLAG_ADDITIVE);
	this->fonts.esp = draw.SetupFont(11, "Tahoma", 600, FONTFLAG_DROPSHADOW);

	int i = 0;
	for (auto thing : skins) {
		skinList[i] = thing.second;
		i++;
	}

	i = 0;
	for (auto thing : Weapons::idxToName) {
		weapList[i] = thing.second;
		i++;
	}

}

void ButtonColor(int r, int g, int b)
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImColor(r, g, b);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(r, g, b);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(r, g, b);
}

void styled()
{
	ImGuiStyle* style = &ImGui::GetStyle();
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
}

void color()
{
	ImGuiStyle* style = &ImGui::GetStyle();
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
}
int tab = 0;

int w, h, ow, oh;
void Menu::Render()
{

	gInterface.engine->GetScreenSize(w, h);

	if (ow != w || oh != h) {
		GUI_Init();
		ow = w;
		oh = h;
	}

	static ImVec2 pos;
	ImGui::Begin("Doodooware Recode", &gMenu.active, ImVec2(840, 600), 1.f, ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoScrollbar);
	{
		pos = ImGui::GetWindowPos();
		ImVec2 size = ImGui::GetWindowSize();
		int width = (size.x - 30) / 4;
		ImGui::SameLine(1);
		if (tab == 0) ButtonColor(50, 50, 50); else ButtonColor(26, 23, 31);
		if (ImGui::Button("Legit", ImVec2(width, 100))) tab = 0;
		ImGui::SameLine(0, 10);
		if (tab == 1) ButtonColor(50, 50, 50); else ButtonColor(26, 23, 31);
		if (ImGui::Button("Visuals", ImVec2(width, 100))) tab = 1;
		ImGui::SameLine(0,10);
		if (tab == 2) ButtonColor(50, 50, 50); else ButtonColor(26, 23, 31);
		if (ImGui::Button("Misc", ImVec2(width, 100))) tab = 2;
		ImGui::SameLine(0, 10);
		if (tab == 3) ButtonColor(50, 50, 50); else ButtonColor(26, 23, 31);
		if (ImGui::Button("AA", ImVec2(width, 100))) tab = 3;
		
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		switch (tab) {
		case 0:
		{
			DrawLegit();
		} break;
		case 1:
		{
			DrawVisual();
		}break;
		case 2:
		{
			DrawMisc();
		}break;
		case 3:
		{
			DrawAA();
		}break;
		case 4:
		{
			//DrawCfg();
		}
		}//End Switch

	}
	ImGui::End();

}


int hitbox = 0;

const char* HITBOXES[] =
{
	"head",
	"neck",
	"pelvis",
	"spine_0",
	"spine_1",
	"spine_2",
	"spine_3",
	"leg_upper_L",
	"leg_upper_R",
	"leg_lower_L",
	"leg_lower_R",
	"ankle_L",
	"ankle_R",
	"hand_L",
	"hand_R",
	"arm_upper_L",
	"arm_lower_L",
	"arm_upper_R",
	"arm_lower_R"
};

char* knives[] = {
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

char* chams[] = {
	"chamsLit",
	"chamsFlat",
	"chamsLitIgnoreZ",
	"chamsFlatIgnoreZ"
};

void Menu::DrawLegit()
{
	ImGui::Checkbox("Aimbot", &gAim.active);
	ImGui::SameLine();
	ImGui::Checkbox("Silent Aim", &gAim.silent);
	ImGui::SameLine();
	ImGui::Checkbox("RCS", &gAim.rcs);
	ImGui::SameLine();
	ImGui::Checkbox("Auto shoot", &gAim.autoshoot);
	ImGui::SameLine();
	ImGui::Checkbox("Tap shoot", &gAim.tapShoot);
	ImGui::Checkbox("Prioritize Visible", &gAim.prioritizeVisible);
	ImGui::Checkbox("Aim Through Walls", &gAim.targetThroughWalls);
	ImGui::Checkbox("AutoWall", &gAim.autowall);

	if (gAim.autowall) {
		ImGui::SliderFloat("Min Damage", &gAim.minDamage, 0, 100);
	}

	ImGui::NewLine();
	ImGui::SliderFloat("FOV", &gAim.aimFOV, 0, 270);
	ImGui::SliderFloat("Aim Time (For auto)", &gAim.m_AimTime, 0, 10);
	ImGui::SliderFloat("Aim smooth (For bolt action)", &gAim.smooth, 0, 10);
	ImGui::SliderFloat("Target Switch Delay", &gAim.newTargetDelay, 0, 10);
	
	ImGui::NewLine();
	ImGui::Checkbox("Velocity Compensation", &gAim.velPredict);
	ImGui::Checkbox("Engine Prediction", &engPred);
	ImGui::NewLine();
	if (ImGui::Combo("Aim Bone", &hitbox, HITBOXES, ARRAYSIZE(HITBOXES))) {
		gAim.aimbox = hitbox;
	}
	ImGui::Checkbox("Dynamic Hitbox", &gAim.dynamicAim);
	ImGui::NewLine();
	ImGui::Checkbox("Enable Backtrack", &gBacktrack.active); 
	
	ImGui::SliderInt("Backtrack Ticks", &gBacktrack.maxTicks, 1, gBacktrack.btArray[0].data.size());
}

char* chamsMat[] = {
	"models/inventory_items/cologne_prediction/cologne_prediction_glass",
	"models/gibs/glass/glass",
	"models/inventory_items/trophy_majors/gloss",
	"models/inventory_items/dogtags/dogtags_outline",
	"models/inventory_items/music_kit/darude_01/mp3_detail",
	"models/inventory_items/dreamhack_trophies/dreamhack_star_blur",
	"models/inventory_items/hydra_crystal/hydra_crystal_detail",
	"models/inventory_items/trophy_majors/silver_winners",
	"models/props_shacks/fishing_net01",
	"models/inventory_items/trophy_majors/gold_dust",
	"models/inventory_items/sticker_inspect/sticker_backing"
};
char* selectChams[] =
{
	"Player",
	"Hands",
	"Weapons",
};

int matIndex = 0;
int matIndexWeap = 0;
int matIndexArm = 0;
int chamTar = 0;

void Menu::DrawVisual()
{
	ImGui::Checkbox("Enable Visuals", &gVis.active);
	ImGui::Checkbox("Draw Box", &gVis.drawBox);
	ImGui::Checkbox("Rainbow Col", &gVis.rainbow);
	ImGui::Checkbox("Player Names", &gVis.drawNames);
	ImGui::Checkbox("Draw Health", &gVis.drawHealth);
	ImGui::Checkbox("Draw xHair", &gVis.DrawXhair);
	ImGui::NewLine();
	ImGui::Checkbox("No Recoil", &gVis.noRecoil);
	ImGui::Checkbox("Wire Smoke", &wiresmoke); 
	ImGui::Checkbox("No Scope", &noScope);
	ImGui::NewLine();
	ImGui::Checkbox("FakeLag Chams", &gVis.DrawLag);
	ImGui::NewLine();
	ImGui::Checkbox("Chams", &gVis.drawChams);
	ImGui::SameLine();
	ImGui::Checkbox("Hand Chams", &gVis.onHands);
	ImGui::SameLine();
	ImGui::Checkbox("Weapon Chams", &gVis.onWeap);
	ImGui::NewLine();
	ImGui::Combo("Chams Target", &chamTar, selectChams, IM_ARRAYSIZE(selectChams));
	switch (chamTar) {
	case 0:
		//ImGui::ColorEdit4("Player Hidden Color", (float*)&gVisuals.chamsColHidden);
		ImGui::ColorEdit4("Chams Color", (float*)&gVis.chamsColor);
		ImGui::Combo("Chams Material", &gVis.chamsIndex, chams, IM_ARRAYSIZE(chams));
		break;
	case 1:
		ImGui::ColorEdit4("Arm Color", (float*)&gVis.chamsArmCol);
		ImGui::NewLine();
		if (ImGui::Combo("Arm Material", &matIndexArm, chamsMat, ARRAYSIZE(chamsMat))) {
			gVis.chamsMatArm = chamsMat[matIndexArm];
		}
		break;
	case 2:
		ImGui::ColorEdit4("Weapon Color", (float*)&gVis.chamsWeapCol);
		ImGui::NewLine();
		if (ImGui::Combo("Weapon Material", &matIndexWeap, chamsMat, ARRAYSIZE(chamsMat))) {
			gVis.chamsMatWeap = chamsMat[matIndexWeap];
		}
		break;
	}

	ImGui::Checkbox("Backtrack Chams", &gVis.drawBacktrack);
	if (gVis.drawBacktrack) {
		ImGui::SliderFloat("Chams End Hue", &hue, 0, 1.f); ImGui::SameLine();ImGui::ColorButton("THIS WILL BE THE ENDING COLOR OF YOUR CHAMS", ImColor::HSV(hue, 0.5f, 0.5f));
		ImGui::Combo("Backtrack Material", &gVis.btChamsIndex, chams, IM_ARRAYSIZE(chams));
		ImGui::Checkbox("Show Last", &gVis.showlast);
	}

	ImGui::NewLine();
	ImGui::Checkbox("Aim Circle", &gVis.drawAimCircle);
}
int knife = 6;
int test;

int get_first(const char* second) {
	for (auto keyval : skins) // Look at each key-value pair
	{
		if (strcmp(keyval.second, second) == 0) // If the value is 0...
		{
			return keyval.first; // ...return the first element in the pair
		}
	}
}

int get_first1(const char* second) {
	for (auto keyval : Weapons::idxToName) // Look at each key-value pair
	{
		if (strcmp(keyval.second, second) == 0) // If the value is 0...
		{
			return keyval.first; // ...return the first element in the pair
		}
	}
}

int id = 2;
int temp = 0;
void Menu::DrawMisc()
{

	 
	ImGui::Checkbox("Bhop", &gBhop.enable);
	ImGui::Checkbox("Watermark", &gVis.watermark); 
	ImGui::Checkbox("Night Mode", &nightmode);

	ImGui::Checkbox("Enable ThirdPerson", &tpEnable);
	if (tpEnable)
		ImGui::SliderFloat("Third Person Camera Dist", &cameraOffset, 10, 300);

	ImGui::Checkbox("Desireable Duck Feature", &noCrouchCooldown);
	ImGui::NewLine();
	if(ImGui::Combo("Knife  Model", &knife, knives, IM_ARRAYSIZE(knives))) {
		gVis.knife = knives[knife];
		gInterface.clientState->force_full_update();
	}
	ImGui::NewLine();
	//ImGui::Checkbox("Scope Remover", &noScope);
	ImGui::SliderInt("Fake Lag", &FakeLag, 0, 16);
	ImGui::NewLine();
	ImGui::SliderInt("Fov", &fov, 9, 179);
	ImGui::Checkbox("New Backtrack Mode", &gBacktrack.newBt);
	if (ImGui::Combo("Weapon name", &temp, weapList, IM_ARRAYSIZE(weapList))) {
		id = get_first1(weapList[temp]);
	}
	//ImGui::InputInt("aijsdh", &id);
	ImGui::Checkbox("Enable Skins", &bSkins);
	if(ImGui::Combo("skins", &test, skinList, IM_ARRAYSIZE(skinList), 10)) {
		auto it = std::find_if(gKnifeChanger.weaptoskin.begin(), gKnifeChanger.weaptoskin.end(),
			[](const std::pair<int, int>& element) {
			return element.first == id;
		});

		for (int i = 0; i < gKnifeChanger.weaptoskin.size(); ++i) {
			if (gKnifeChanger.weaptoskin[i] == *it) {
				gKnifeChanger.weaptoskin[i] = std::make_pair(it->first, get_first(skinList[test]));
				break;
			}
		}
		gInterface.clientState->force_full_update();
	}

}


	char* yawAA[] = {
		"NONE",
		"BACKWARDS",
		"SPINBOT",
		"LOWER_BODY",
		"RANDOM",
		"FREESTANDING",
		"CUSTOM"
	};

	char* yawAddAA[] = {
		"NONE",
		"JITTER",
		"SWITCH",
		"SPIN",
		"RANDOM"
	};

	char* pitchAA[] = {
		"NONE",
		"EMOTION",
		"DOWN",
		"UP",
		"ZERO",
		"JITTER",
		"DOWN_JITTER",
		"UP_JITTER",
		"ZERO_JITTER",
		"SPIN",
		"SPIN_UP",
		"SPIN_DOWN",
		"RANDOM",
		"SWITCH",
		"DOWN_SWITCH",
		"UP_SWITCH",
		"FAKE_UP",
		"FAKE_DOWN",
		"CUSTOM"
	};


void Menu::DrawAA()
{
	ImGui::Checkbox("Rage", &gRage.active);
	
	ImGui::Combo("Stand Pitch Mode", &rage.aaStandYMode, pitchAA, IM_ARRAYSIZE(pitchAA));
	ImGui::SliderFloat("Stand Custom Pitch", &rage.aaStandY, 0, 90);
	ImGui::Combo("Move Pitch Mode", &rage.aaMoveYMode, pitchAA, IM_ARRAYSIZE(pitchAA));
	ImGui::SliderFloat("Move Custom Pitch", &rage.aaMoveY, 0, 90);
	ImGui::Combo("Jump Pitch Mode", &rage.aaJumpYMode, pitchAA, IM_ARRAYSIZE(pitchAA));
	ImGui::SliderFloat("Jump Custom Pitch", &rage.aaJumpY, 0, 90);
	ImGui::NewLine();

	ImGui::Combo("Stand Yaw Mode", &rage.aaStandXModeReal, yawAA, IM_ARRAYSIZE(yawAA));
	ImGui::SliderFloat("Stand Custom Yaw", &rage.aaStandXReal, 0, 90);
	ImGui::Combo("Move Yaw Mode", &rage.aaMoveXModeReal, yawAA, IM_ARRAYSIZE(yawAA));
	ImGui::SliderFloat("Move Custom Yaw", &rage.aaMoveXReal, 0, 90);
	ImGui::Combo("Jump Yaw Mode", &rage.aaJumpXModeReal, yawAA, IM_ARRAYSIZE(yawAA));
	ImGui::SliderFloat("Jump Custom Yaw", &rage.aaJumpXReal, 0, 90);
	ImGui::NewLine();

	ImGui::Combo("Stand YawAdd Mode", &rage.aaStandXAddrModeReal, yawAddAA, IM_ARRAYSIZE(yawAddAA));
	ImGui::SliderFloat("Stand YawAdd Range", &rage.aaStandXAddrReal, 0, 90);
	ImGui::Combo("Move YawAdd Mode", &rage.aaMoveXAddrModeReal, yawAddAA, IM_ARRAYSIZE(yawAddAA));
	ImGui::SliderFloat("Move YawAdd Range", &rage.aaMoveXAddrReal, 0, 90);
	ImGui::Combo("Jump YawAdd Mode", &rage.aaJumpXAddrModeReal, yawAddAA, IM_ARRAYSIZE(yawAddAA));
	ImGui::SliderFloat("Jump YawAdd Range", &rage.aaJumpXAddrReal, 0, 90);
	ImGui::NewLine();


}



/*

enum class YawAntiAims : int
{
NONE,
BACKWARDS,
SPINBOT,
LOWER_BODY,
RANDOM,
FREESTANDING,
CUSTOM
};

enum class YawAddAntiAims : int
{
NONE,
JITTER,
SWITCH,
SPIN,
RANDOM
};

enum class PitchAntiAims : int
{
NONE,
EMOTION,
DOWN,
UP,
ZERO,
JITTER,
DOWN_JITTER,
UP_JITTER,
ZERO_JITTER,
SPIN,
SPIN_UP,
SPIN_DOWN,
RANDOM,
SWITCH,
DOWN_SWITCH,
UP_SWITCH,
FAKE_UP,
FAKE_DOWN,
CUSTOM
};

struct Rage
	{
		int aaStandYMode;
		int aaMoveYMode;
		int aaJumpYMode;
		int aaStandY;
		int aaMoveY;
		int aaJumpY;

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


	}rage;*/

/*ImGui::SliderInt("Fov", &fov, 9, 179);
	ImGui::Checkbox("New Backtrack Mode", &gBacktrack.newBt);
	//gKnifeChanger.
	ImGui::InputInt("Weapon id: ", &id);
	if(ImGui::Combo("skins", &test, skinList, IM_ARRAYSIZE(skinList))) {
		if (gInterface.local) {
			auto it = std::find_if(gKnifeChanger.weaptoskin.begin(), gKnifeChanger.weaptoskin.end(),
				[](const std::pair<int, int>& element) {
				return element.first == id;
			});

			for (int i = 0; i < gKnifeChanger.weaptoskin.size(); ++i) {
				if (gKnifeChanger.weaptoskin[i] == *it) {
					gKnifeChanger.weaptoskin[i] = std::make_pair(it->first, get_first(skinList[test]));
					break;
				}
			}

			//it->second = get_first(skinList[test]);
		}
		gInterface.clientState->force_full_update();
		}*/