#pragma once

#ifdef CreateFont
#undef CreateFont
#endif

namespace Indexes
{
	namespace Engine
	{
		const int GetScreenSize = 5;
		const int GetPlayerInfo = 8;
		const int GetPlayerForUserID = 9;
		const int GetLocalPlayer = 12;
		const int SetViewAngles = 19;
		const int GetMaxClients = 20;
		const int IsInGame = 26;
		const int IsConnected = 27;
		const int WorldToScreenMatrix = 37;
		const int ClientCmd = 108;
	}
	namespace ClientEntityList
	{
		const int GetClientEntity = 3;
		const int GetClientEntityFromHandle = 4;
	}
	namespace ClientMode
	{
		const int CreateMove = 24;
	}
	namespace CGamemovement
	{
		const int ProcessMovement = 1;
	}
	namespace EngineTrace
	{
		const int TraceRay = 5;
	}
	namespace BaseEntity
	{
		const int GetAbsOrigin = 10;
		const int GetAbsAngles = 11;
		//const int isPlayer = 153;
	}
	namespace Panel
	{
		const int GetName = 36;
		const int PaintTraverse = 41;
	}
	namespace Surface
	{
		const int DrawSetColor = 15;
		const int DrawFilledRect = 16;
		const int DrawOutlinedRect = 18;
		const int DrawLine = 19;
		const int CreateFont = 71;
		const int SetFontGlyphSet = 72;
		const int DrawSetTextFont = 23;
		const int DrawSetTextColor = 25;
		const int DrawSetTextPos = 26;
		const int DrawPrintText = 28;
		const int UnlockCursor = 66;
		const int GetTextSize = 79;
		const int DrawOutlinedCircle = 103;
		const int SurfaceGetCursorPos = 100;
		const int SurfaceSetCursorPos = 101;
	}
	namespace InputSystem
	{
		const int IsButtonDown = 15;
	}

	namespace InputInternal
	{
		const int InternalCursorMoved = 69;
		const int InternalMousePressed = 70;
		const int InternalMouseDoublePressed = 71;
	}

	namespace GAMEEVENTSMANAGER
	{
		const int FireEvent = 8;
		const int FireEventClientSide = 9;
	}
}