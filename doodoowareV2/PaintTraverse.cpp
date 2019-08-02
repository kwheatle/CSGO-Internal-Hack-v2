#pragma once
#include "hooks.h"
#include "render.h"
#include "Visuals.h"
Visuals gVis;

hooks::PaintTraverseFn hooks::OriginalPaintTraverse;

void __stdcall hooks::HookedPaintTraverse(VPANEL _vguiPanel, bool _forceRepaint, bool _allowForce) {
	static auto panelId = VPANEL{ 0 };
	OriginalPaintTraverse(_vguiPanel, _forceRepaint, _allowForce);

	if (!panelId) {
		const auto panelName = gInterface.panel->GetName(_vguiPanel);
		if (!strcmp(panelName, "MatSystemTopPanel")) {
			panelId = _vguiPanel;
		}
		if (!strcmp(panelName, "HudZoom") && gMenu.noScope) {
			return;
		}
	}
	else if(panelId == _vguiPanel){
		//if(gInterface.local && gInterface.engine->IsConnected() && gInterface.engine->IsInGame())
			//draw.DrawString(0, 0, 'l', "Item id: " + gInterface.local->GetWeapon(), gMenu.fonts.watermark, Color(0, 0, 0, 255));///Helps with broken textures for some reason
		if (gVis.watermark) {
			static int t = 0;
			static float hue = 0;

			t++;
			if (t > 860) t = 0;

			hue = float((1.f / 860.f) * (float)t);
			ImColor dotHSV = ImColor::HSV(hue, 1.f, 1.f);

			draw.DrawString(0, 20, 'l', "DOODOOWARE", gMenu.fonts.watermark, Color(dotHSV.Value.x * 255, dotHSV.Value.y * 255, dotHSV.Value.z * 255, 255));
		}
		
		int width, height;
		gInterface.engine->GetScreenSize(width, height);
		float pxR = tan(DEG2RAD(gAim.aimFOV / 2)) / tan(DEG2RAD(gMenu.fov / 2)) * (width / 2);
		if(gVis.drawAimCircle)
			draw.DrawOutlinedCircle(width / 2, height / 2, pxR, 300, Color(255, 0, 255, 255));

		gVis.DrawVisuals();
		auto lp = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
		gVis.DrawRecoilXhair(lp);
	}
}