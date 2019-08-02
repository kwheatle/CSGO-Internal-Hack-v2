#include "hooks.h"
#include "KnifeChanger.h"
#include "SourceSDK\Misc\CBaseCombatWeapon.h"

hooks::FrameStageNotifyFn hooks::OriginalFSN;
KnifeChanger gKnifeChanger;
Vector aimpunch, viewpunch;
bool done = false;
void __fastcall hooks::HookedFSN(void* ecx, void* edx, ClientFrameStage_t stage) {
	static CBaseEntity* localPlayer = nullptr;
	if (gInterface.engine->IsConnected() && gInterface.engine->IsInGame()) {
		
		localPlayer = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());

		if (stage == FRAME_RENDER_START) {
			for (int i = 1; i < 2048; ++i) {
				if (gInterface.engine->GetLocalPlayer() == i) continue;

				auto ent = gInterface.entityList->GetClientEntity(i);
				if (!ent) continue;

				/*if (ent->GetClientClass()->m_ClassID == EClassIds::CEnvTonemapController) {
					auto CETC = (CEnvTonemapController*)ent;
					if (gMenu.nightmode) {
						*CETC->use_custom_exposure_max() = 1;
						*CETC->use_custom_exposure_min() = 1;

						*CETC->custom_exposure_max() = 0.075f;
						*CETC->custom_exposure_min() = 0.075f;
						continue;
					}
					else {
						*CETC->use_custom_exposure_max() = 0;
						*CETC->use_custom_exposure_min() = 0;
					}
				}*/

				if (!ent->IsValid(localPlayer, false))continue;

				*(int*)((uintptr_t)ent + 0xA30) = gInterface.globals->framecount; //Skipping occlusion check
				*(int*)((uintptr_t)ent + 0xA28) = 0; //clearing occlusion flags

				if (gVis.onHands) {
					done = false;
				}

					if (!done)
					{
						static auto sv_skyname = gInterface.cvar->FindVar("sv_skyname");
						static auto r_3dsky = gInterface.cvar->FindVar("r_3dsky"); //r_DrawSpecificStaticProp
						static auto r_DrawSpecificStaticProp = gInterface.cvar->FindVar("r_DrawSpecificStaticProp");
						r_3dsky->SetValue(0);
						r_DrawSpecificStaticProp->SetValue(0);
						sv_skyname->SetValue("sky_csgo_night02");

						for (MaterialHandle_t i = gInterface.matsystem->FirstMaterial(); i != gInterface.matsystem->InvalidMaterial(); i = gInterface.matsystem->NextMaterial(i))
						{
							IMaterial *pMaterial = gInterface.matsystem->GetMaterial(i);

							if (!pMaterial)
								continue;

							if (!pMaterial)
								continue;

							const char* group = pMaterial->GetTextureGroupName();
							const char* name = pMaterial->GetName();
							float world_textures = gMenu.nightmode ? 0.10f : 1.f;
							float staticprop = gMenu.nightmode ? 0.30f : 1.f;
							float palace_pillars = gMenu.nightmode ? 0.30f : 1.f;
							if (strstr(group, "World textures"))
							{
								pMaterial->ColorModulate(world_textures, world_textures, world_textures);
							}
							if (strstr(group, "StaticProp"))
							{
								pMaterial->ColorModulate(staticprop, staticprop, staticprop);
							}
							if (strstr(name, "models/props/de_dust/palace_bigdome"))
							{
								pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, gMenu.nightmode);
							}
							if (strstr(name, "models/props/de_dust/palace_pillars"))
							{
								pMaterial->ColorModulate(palace_pillars, palace_pillars, palace_pillars);
							}

							if (strstr(group, "Particle textures"))
							{
								pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, gMenu.nightmode);
							}
							done = true;
						}

					}

				//gBacktrack.setLagRecord(ent);

			}

			if (gVis.DrawLag && gMenu.tpEnable) {
				gInterface.prediction->SetLocalViewAngles(gInterface.fakeang);
			}

			if (gVis.noRecoil) {
				aimpunch = *reinterpret_cast<QAngle*>(uintptr_t(localPlayer) + Offsets::m_aimPunchAngle);
				viewpunch = *reinterpret_cast<QAngle*>(uintptr_t(localPlayer) + Offsets::m_viewPunchAngle);

				*reinterpret_cast<QAngle*>(uintptr_t(localPlayer) + Offsets::m_aimPunchAngle) = QAngle(0, 0, 0);
				*reinterpret_cast<QAngle*>(uintptr_t(localPlayer) + Offsets::m_viewPunchAngle) = QAngle(0,0,0);
			}

		}
		else if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			if (localPlayer->GetHealth() > 0) {
				int* hWeapons = localPlayer->GetWeapons();
				for (int i = 0; hWeapons[i] != 0xFFFFFFFF; ++i) {
					if (!hWeapons[i]) continue;

					auto weap = (CBaseAttributableItem*)gInterface.entityList->GetClientEntityFromHandle(hWeapons[i]);
					if (!weap) continue;

					gKnifeChanger.setKnife(localPlayer, weap);
				}
			}		
		}
		else if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END) {
			static std::vector<const char*> vistasmoke_mats =
			{
				"particle/vistasmokev1/vistasmokev1_fire",
				"particle/vistasmokev1/vistasmokev1_smokegrenade",
				"particle/vistasmokev1/vistasmokev1_emods",
				"particle/vistasmokev1/vistasmokev1_emods_impactdust",
			};

			
			for (auto mat_s : vistasmoke_mats)
			{
				IMaterial* mat = gInterface.matsystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, gMenu.wiresmoke);
			}
			
		}
		else if (stage == FRAME_RENDER_END) {
			for (int i = 1; i < 65; ++i) {
				if (gInterface.engine->GetLocalPlayer() == i) continue;

				auto ent = gInterface.entityList->GetClientEntity(i);
				if (!ent) continue;

				if (!ent->IsValid(localPlayer, false))continue;

				if (gAim.m_AimTime <= 0.15)
					gBacktrack.DisableInterpolation(ent);
			}
		}


	}
	OriginalFSN(ecx, stage);

	if (gVis.noRecoil && localPlayer) {
		*reinterpret_cast<QAngle*>(uintptr_t(localPlayer) + Offsets::m_viewPunchAngle) = viewpunch;
		*reinterpret_cast<QAngle*>(uintptr_t(localPlayer) + Offsets::m_aimPunchAngle) = aimpunch;
	}

}