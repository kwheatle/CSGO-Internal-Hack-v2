#include "Visuals.h"

Draw draw;
RECT Visuals::GetBBox(CBaseEntity* pEntity)
{
	RECT rect{};
	Vector vOrigin, min, max;
	Vector flb, brt, blb, frt, frb, brb, blt, flt; // think of these as Front-Left-Bottom/Front-Left-Top... Etc.
	float left, top, right, bottom;


	// Get the locations
	vOrigin = pEntity->GetAbsOrigin();
	min = pEntity->GetCollideable()->OBBMins();
	max = pEntity->GetCollideable()->OBBMaxs();

	/*Vector test = pEntity->GetHitboxPos(HITBOX_HEAD);

	std::ostringstream Test;
	Test << "say Pos: " << test.x << ", " << test.y << ", " << test.z;
	gInterface.Engine()->ClientCmd(Test.str().c_str());*/

	/*std::ostringstream Max;
	Max << "echo Max: " << max.x << ", " << max.y << ", " << max.z;
	gInterface.Engine()->ClientCmd(Max.str().c_str());*/

	// Points of a 3d bounding box
	Vector points[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	/*
	.6--------5					        blb = 0
	.' |       .' |					        brb = 1
	7---+--4'   |		0 = min		  frb = 2
	|   |    |    |		4 = max		 flb = 3
	y|  ,0---+---1					        frt = 4
	|.'      | .'  z					       brt = 5
	3------2						          blt = 6
	x						                flt = 7

	blt------brt		blt = back-left-top
	.' |        .'|			brt = back-right-top
	flt---+--frt  |			brb = back-right-bottom
	|   |     |   |			blb = back-left-bottom
	y |  blb---+--brb			flt = front-left-top
	|.'        | .'  z		frt = front-right-top
	flb------frb			frb = front-right-bottom
	x					flb = front-left-bottom

	*/

	const matrix3x4_t& trans = pEntity->GetCoordFrame();

	Vector pointsTransformed[8];
	for (int i = 0; i < 8; i++) {
		gMath.VectorTransform(points[i], trans, pointsTransformed[i]);
	}

	if (!gMath.WorldToScreen(pointsTransformed[3], flb) || !gMath.WorldToScreen(pointsTransformed[5], brt)
		|| !gMath.WorldToScreen(pointsTransformed[0], blb) || !gMath.WorldToScreen(pointsTransformed[4], frt)
		|| !gMath.WorldToScreen(pointsTransformed[2], frb) || !gMath.WorldToScreen(pointsTransformed[1], brb)
		|| !gMath.WorldToScreen(pointsTransformed[6], blt) || !gMath.WorldToScreen(pointsTransformed[7], flt))
		return rect;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++) {
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	//Render::Get().RenderBoxByType(left, top, right, bottom, Color(255,0, 255,255), 1);
	return RECT{ (long)left, (long)top, (long)right, (long)bottom };
}

void Visuals::DrawVisuals() {

	if (!active)
		return;

	auto lp = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());

	if (!lp)
		return;

	for (int i = 1; i < gInterface.engine->GetMaxClients(); ++i) {
		auto ent = gInterface.entityList->GetClientEntity(i);

		if (!ent->IsValid(lp, false))
			continue;

		RECT bbox = GetBBox(ent);

		if (drawBox) {
			if (rainbow) {
				static int t = 0;
				static float hue = 0;

				t++;
				if (t > 860) t = 0;

				hue = float((1.f / 860.f) * (float)t);
				ImColor dotHSV = ImColor::HSV(hue, 1.f, 1.f);
				draw.drawPlayerBox(bbox.left, bbox.top, bbox.right, bbox.bottom, 2, Color(dotHSV.Value.x * 255, dotHSV.Value.y * 255, dotHSV.Value.z * 255, 255));
			}
			else {
				draw.drawPlayerBox(bbox.left, bbox.top, bbox.right, bbox.bottom, 2, Color(255, 0, 0, 255));
			}
		}

		if (drawNames) {
			player_info_t info = ent->GetPlayerInfo();
			draw.DrawString(bbox.left, bbox.top + 2, 'l', info.name, gMenu.fonts.esp, Color(255, 255, 255, 255));
		}

		if (drawHealth) {
			int hp = ent->GetHealth();
			draw.DrawHealth(bbox.left, bbox.top, bbox.right, bbox.bottom, 4, hp);
		}
		
	}
}

void Visuals::DrawBacktrackChams(IMaterial* mat, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo) {
	if (!mat) return;
	CBaseEntity* mdlEnt = gInterface.entityList->GetClientEntity(pInfo.entity_index);
	CBaseEntity* local = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
	//if (mdlEnt->GetTeamNumber() == local->GetTeamNumber()) return;
	if (showlast) {
		auto& tick = gBacktrack.btArray.at(pInfo.entity_index).data.at(gBacktrack.maxTicks - 1);
		if (!tick.aimpos || !tick.origin || !tick.matrix || !tick.simtime) return;
		auto diff = tick.origin - pInfo.origin;
		auto dist = abs(diff.Length());

		if (dist < 0.2f) return;

		float sat = float((1.f / (float(gBacktrack.maxTicks * 2)) * (float)gBacktrack.maxTicks));
		ImVec4 dotHSV = ImColor::HSV(gMenu.hue, sat, 0.5f);
		ModelRenderInfo_t newInfo = pInfo;
		newInfo.origin = tick.origin;
		mat->ColorModulate(dotHSV.x, dotHSV.y, dotHSV.z);
		gInterface.mdlRender->ForcedMaterialOverride(mat);
		hook.OriginalDME(gInterface.mdlRender, ctx, state, newInfo, tick.matrix);
		gInterface.mdlRender->ForcedMaterialOverride(nullptr);

	}	else {
		for (int i = 0; i < gBacktrack.maxTicks; ++i) {
			auto& tick = gBacktrack.btArray.at(pInfo.entity_index).data.at(i);
			if (!tick.aimpos || !tick.origin || !tick.matrix || !tick.simtime) continue;
			auto diff = tick.origin - pInfo.origin;
			auto dist = abs(diff.Length());

			if (dist < 0.2f) continue;

			float sat = float((1.f / (float(gBacktrack.maxTicks * 2)) * (float)i));
			ImColor dotHSV = ImColor::HSV(gMenu.hue, sat, 0.5f);
			ModelRenderInfo_t newInfo = pInfo;
			newInfo.origin = tick.origin;
			mat->ColorModulate(dotHSV.Value.x, dotHSV.Value.y, dotHSV.Value.z);
			gInterface.mdlRender->ForcedMaterialOverride(mat);
			hook.OriginalDME(gInterface.mdlRender, ctx, state, newInfo, tick.matrix);
			gInterface.mdlRender->ForcedMaterialOverride(nullptr);
		}
	}
}

void Visuals::DrawChams(IMaterial* mat, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld) {
	if (!mat) return;
	CBaseEntity* mdlEnt = gInterface.entityList->GetClientEntity(pInfo.entity_index);
	CBaseEntity* local = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
	if (!mdlEnt->IsValid(local, false) && pInfo.entity_index != gInterface.engine->GetLocalPlayer())return;

	if (pInfo.entity_index == gInterface.engine->GetLocalPlayer() && gMenu.tpEnable) {
		mat->AlphaModulate(0.3f);
	}
	else {
		mat->AlphaModulate(1.f);
	}

	mat->ColorModulate(chamsColor.x, chamsColor.y, chamsColor.z);
	gInterface.mdlRender->ForcedMaterialOverride(mat);
	hook.OriginalDME(gInterface.mdlRender, ctx, state, pInfo, pCustomBoneToWorld);

}

void Visuals::DrawFakeLag(IMaterial* mat, IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo) {
	if (!mat) return;
	if (pInfo.entity_index != gInterface.engine->GetLocalPlayer())return;
	CBaseEntity* local = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
	mat->AlphaModulate(1.f);
	mat->ColorModulate(1.f, 1.f, 1.f);
	gInterface.mdlRender->ForcedMaterialOverride(mat);
	hook.OriginalDME(gInterface.mdlRender, ctx, state, pInfo, gInterface.localMatrix);
	mat->AlphaModulate(1.f);

}

void Visuals::DrawRecoilXhair(CBaseEntity* _localPlayer)
{
	if (!DrawXhair || !gInterface.engine->IsConnected() || !gInterface.engine->IsInGame()) return;

	int w, h;
	QAngle punchAngle = _localPlayer->GetAimPunchAngle();
	gInterface.engine->GetScreenSize(w, h);


	QAngle aimPunch;
	QAngle viewPunch;

	aimPunch = _localPlayer->GetAimPunchAngle() / 2.f;

	aimPunch.y *= w / gMenu.fov;
	aimPunch.x *= h / gMenu.fov;
	viewPunch.y *= w / gMenu.fov;
	viewPunch.x *= h / gMenu.fov;
	int cx = w / 2 - (int)aimPunch.y - (int)viewPunch.y;
	int cy = h / 2 + (int)aimPunch.x + (int)aimPunch.x;

	draw.DrawFilledRect(cx - 5, cy, 10, 1, Color(255, 255, 255, 127));
	draw.DrawFilledRect(cx, cy - 5, 1, 10, Color(255, 255, 255, 127));
}


void Visuals::InitKeyValues(KeyValues* kv_, std::string name_)
{
	static auto address = gInterface.FindPattern("client_panorama.dll", "55 8B EC 51 33 C0 C7 45");
	using Fn = void(__thiscall*)(void* thisptr, const char* name);
	reinterpret_cast< Fn >(address)(kv_, name_.c_str());
}

void Visuals::LoadFromBuffer(KeyValues* vk_, std::string name_, std::string buffer_)
{
	static auto address = gInterface.FindPattern("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");
	using Fn = void(__thiscall*)(
		void* thisptr, const char* resourceName,
		const char* pBuffer, void* pFileSystem,
		const char* pPathID, void* pfnEvaluateSymbolProc);

	reinterpret_cast< Fn >(address)(
		vk_, name_.c_str(), buffer_.c_str(), nullptr, nullptr, nullptr);
}

/*IMaterial* Visuals::CreateMaterialMetallic(bool ignorez, bool lit, bool wireframe)
{
	static auto created = 0;

	std::string type = lit ? "VertexLitGeneric" : "UnlitGeneric";
	auto matdata =
		"\"" + type + "\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"env_cubemap\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$normalmapalphaenvmapmask\" \"1\"\
		\n\t\"$envmapcontrast\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\
		\n}\n";

	auto matname = "custom_" + std::to_string(created);
	++created;

	auto keyValues = static_cast< KeyValues* >(malloc(sizeof(KeyValues)));

	InitKeyValues(keyValues, type.c_str());
	LoadFromBuffer(keyValues, matname, matdata);

	auto material =
		gInterface.matsystem->CreateMaterial(matname.c_str(), keyValues);

	return material;
}*/

IMaterial* Visuals::CreateMaterialBasic(bool ignorez, bool lit, bool wireframe)
{
	static auto created = 0;

	std::string type = lit ? "VertexLitGeneric" : "UnlitGeneric";
	auto matdata =
		"\"" + type + "\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"" + std::to_string(wireframe) + "\"\
		\n}\n";

	auto matname = "custom_" + std::to_string(created);
	++created;

	auto keyValues = static_cast< KeyValues* >(malloc(sizeof(KeyValues)));

	InitKeyValues(keyValues, type.c_str());
	LoadFromBuffer(keyValues, matname, matdata);

	auto material =
		gInterface.matsystem->CreateMaterial(matname.c_str(), keyValues);

	return material;
}