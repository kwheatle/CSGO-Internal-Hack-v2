#pragma once
#include "hooks.h"

hooks::DrawModelExecuteFn hooks::OriginalDME;
void __stdcall hooks::hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld) {
	static IMaterial* chamsLit = gVis.CreateMaterialBasic(false, true, false);
	static IMaterial* chamsFlat = gVis.CreateMaterialBasic(false, false, false);
	static IMaterial* chamsLitIZ = gVis.CreateMaterialBasic(true, true, false);
	static IMaterial* chamsFlatIZ = gVis.CreateMaterialBasic(true, false, false);

	static IMaterial* mats[] = {
		chamsLit,
		chamsFlat,
		chamsLitIZ,
		chamsFlatIZ
	};
	if (pInfo.pModel) {

		std::string name = gInterface.modelInfo->GetModelName(pInfo.pModel);

		if (name.find("models/player") != std::string::npos && pInfo.entity_index <= 128 && pInfo.entity_index > 0) {
			if (gVis.drawBacktrack)
				gVis.DrawBacktrackChams(mats[gVis.btChamsIndex], ctx, state, pInfo);
			if (gVis.DrawLag)
				gVis.DrawFakeLag(chamsLit, ctx, state, pInfo);
			if (gVis.drawChams)
				gVis.DrawChams(mats[gVis.chamsIndex], ctx, state, pInfo, pCustomBoneToWorld);
			
		}

		if ((name.find("v_") != std::string::npos || name.find("uid") != std::string::npos || name.find("stattrack") != std::string::npos) && name.find("arms") == std::string::npos && gVis.onWeap)
		{
			IMaterial *material = gInterface.matsystem->FindMaterial(gVis.chamsMatWeap, TEXTURE_GROUP_OTHER);
			material->AlphaModulate(gVis.chamsWeapCol.w);
			material->ColorModulate(gVis.chamsWeapCol.x, gVis.chamsWeapCol.y, gVis.chamsWeapCol.z);
			gInterface.mdlRender->ForcedMaterialOverride(material);
		}
		else if (name.find("arms") != std::string::npos && gVis.onHands)
		{
			IMaterial *material = gInterface.matsystem->FindMaterial(gVis.chamsMatArm, TEXTURE_GROUP_OTHER);
			material->AlphaModulate(gVis.chamsArmCol.w);
			material->ColorModulate(gVis.chamsArmCol.x, gVis.chamsArmCol.y, gVis.chamsArmCol.z);
			gInterface.mdlRender->ForcedMaterialOverride(material);
		}

	}
	OriginalDME(gInterface.mdlRender, ctx, state, pInfo, pCustomBoneToWorld);
	gInterface.mdlRender->ForcedMaterialOverride(nullptr);
}