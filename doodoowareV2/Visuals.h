#pragma once
#include "SourceSDK\sdk.h"
#include "render.h"
#include "Backtrack.h"
#include "Draw.h"

class Visuals
{
	
public:
	RECT GetBBox(CBaseEntity * ent);
	bool active = false;
	bool watermark = false;
	bool rainbow = true;
	bool drawBox = false;
	bool drawBacktrack = false;
	bool drawNames = false;
	bool drawHealth = false;
	bool drawAimCircle = false;
	bool DrawLag = false;

	int btChamsIndex = 0;
	int chamsIndex = 0;

	bool noRecoil = false;

	bool DrawXhair;
	bool drawChams = false;
	bool onHands = false;
	bool onWeap = false;

	bool showlast = false;
	void DrawVisuals();
	ImVec4 backtrackColor = { 1.f, 0.5f, 1.f, 1.f };
	ImVec4 chamsColor = { 1.f, 0.5f, 1.f, 1.f };
	char* chamsMatArm = "models/inventory_items/trophy_majors/crystal_clear";
	char* chamsMatWeap = "vgui/white_additive";

	ImVec4 chamsArmCol = { 1, 1, 1, 1.f };
	ImVec4 chamsWeapCol = { 1, 1, 1, 1.f };


	void DrawBacktrackChams(IMaterial * mat, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo);
	void DrawChams(IMaterial * mat, IMatRenderContext * ctx, const DrawModelState_t & state, const ModelRenderInfo_t & pInfo, matrix3x4_t * pCustomBoneToWorld);
	void DrawFakeLag(IMaterial * mat, IMatRenderContext * ctx, const DrawModelState_t & state, const ModelRenderInfo_t & pInfo);
	void DrawRecoilXhair(CBaseEntity * _localPlayer);
	void InitKeyValues(KeyValues * kv_, std::string name_);
	void LoadFromBuffer(KeyValues * vk_, std::string name_, std::string buffer_);
	IMaterial* CreateMaterialMetallic(bool ignorez, bool lit, bool wireframe);
	IMaterial* CreateMaterialBasic(bool ignorez, bool lit, bool wireframe);
	char* knife = "models/weapons/v_knife_flip.mdl";

private:
	
};
extern Visuals gVis;
