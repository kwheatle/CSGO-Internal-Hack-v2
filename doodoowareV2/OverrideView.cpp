#include "hooks.h"

trace_t rayTrace(CBaseEntity* _skip, DWORD _mask)
{
	Ray_t ray;
	trace_t trace;

	Vector inverse_angles;
	gInterface.engine->GetViewAngles(inverse_angles);

	inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

	Vector direction = gMath.AngleToVector(inverse_angles);

	ray.Init(_skip->GetEyePosition(), (_skip->GetEyePosition()) + (direction * (gMenu.cameraOffset + 5.f)));

	CTraceWorldOnly Filter;
	Filter.pSkip = _skip;

	gInterface.engineTrace->TraceRay(ray, _mask, &Filter, &trace);

	return trace;
}

hooks::OverrideViewfn hooks::oOverride;

void __fastcall hooks::hkOverrideView(void* ecx, void* edx, CViewSetup* setup) {

	if (gInterface.engine->IsInGame() && gInterface.engine->IsConnected())
	{
		static Vector vecAngles;
		gInterface.engine->GetViewAngles(vecAngles);
		CBaseEntity* lp = gInterface.entityList->GetClientEntity(gInterface.engine->GetLocalPlayer());
		if (lp->GetHealth() > 0 && gMenu.tpEnable)
		{
			if (!gInterface.input->m_fCameraInThirdPerson)
			{
				gInterface.input->m_fCameraInThirdPerson = true;

				trace_t traceY;
				trace_t traceX;
				Vector wantedAngle = Vector(vecAngles.x, vecAngles.y, gMenu.cameraOffset);

				traceY = rayTrace(lp, 0xFFFFFFFF);
				
				if (traceY.fraction < 1.0) {
					wantedAngle.z *= traceY.fraction;
				}

				gInterface.input->m_vecCameraOffset = wantedAngle;
			}
		}
		else
		{
			gInterface.input->m_fCameraInThirdPerson = false;
			gInterface.input->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
		}

		if (lp && lp->GetHealth() > 0) {
			if (!lp->isScoped() || gMenu.noScope)
				setup->fov = gMenu.fov;
			
			
		}
	}


	return oOverride(ecx, setup);
}