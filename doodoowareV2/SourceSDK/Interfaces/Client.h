#pragma once
#include "..\Interfaces\Recv.h"
#include "Globals.h"

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);


enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};
class bf_read
{
public:
	uintptr_t base_address;
	uintptr_t cur_offset;

	bf_read::bf_read(uintptr_t addr)
	{
		base_address = addr;
		cur_offset = 0;
	}

	void bf_read::SetOffset(uintptr_t offset)
	{
		cur_offset = offset;
	}

	void bf_read::Skip(uintptr_t length)
	{
		cur_offset += length;
	}

	int bf_read::ReadByte()
	{
		auto val = *reinterpret_cast<char*>(base_address + cur_offset);
		++cur_offset;
		return val;
	}

	bool bf_read::ReadBool()
	{
		auto val = *reinterpret_cast<bool*>(base_address + cur_offset);
		++cur_offset;
		return val;
	}

	std::string bf_read::ReadString()
	{
		char buffer[256];
		auto str_length = *reinterpret_cast<char*>(base_address + cur_offset);
		++cur_offset;
		memcpy(buffer, reinterpret_cast<void*>(base_address + cur_offset), str_length > 255 ? 255 : str_length);
		buffer[str_length > 255 ? 255 : str_length] = '\0';
		cur_offset += str_length + 1;
		return std::string(buffer);
	}
};

// Used by RenderView
enum RenderViewInfo_t
{
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
	RENDERVIEW_DRAWHUD = (1 << 1),
	RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

class ClientClass;
class IClientUnknown;
class IClientNetworkable
{
public:
    virtual IClientUnknown*  GetIClientUnknown() = 0;
    virtual void             Release() = 0;
    virtual ClientClass*     GetClientClass() = 0;
    virtual void             NotifyShouldTransmit(int state) = 0;
    virtual void             OnPreDataChanged(int updateType) = 0;
    virtual void             OnDataChanged(int updateType) = 0;
    virtual void             PreDataUpdate(int updateType) = 0;
    virtual void             PostDataUpdate(int updateType) = 0;
    virtual void             __unkn(void) = 0;
    virtual bool             IsDormant(void) = 0;
    virtual int              GetIndex(void) const = 0;
    virtual void             ReceiveMessage(int classID, bf_read& msg) = 0;
    virtual void*            GetDataTableBasePtr() = 0;
    virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class bf_write;
class vrect_t;
class ScreenFade_t;
class qboolean;
class CEngineSprite;
class CViewSetup;

class ClientClass
{
public:
	CreateClientClassFn      m_pCreateFn;
	CreateEventFn            m_pCreateEventFn;
	char*                    m_pNetworkName;
	RecvTable*               m_pRecvTable;
	ClientClass*             m_pNext;
	EClassIds                m_ClassID;
};

class IBaseClientDLL
{
public:
	virtual int              Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals) = 0;
	virtual int              Disconnect(void) = 0;
	virtual int              Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals) = 0;
	virtual void             PostInit() = 0;
	virtual void             Shutdown(void) = 0;
	virtual void             LevelInitPreEntity(char const* pMapName) = 0;
	virtual void             LevelInitPostEntity() = 0;
	virtual void             LevelShutdown(void) = 0;
	virtual ClientClass*     GetAllClasses(void) = 0;
	virtual void					unkn0() = 0;
	virtual int						HudVidInit(void);
	virtual void					HudProcessInput(bool bActive);
	virtual void					HudUpdate(bool bActive);
	virtual void					HudReset(void);
	virtual void					HudText(const char * message);
	virtual void					IN_ActivateMouse(void);
	virtual void					IN_DeactivateMouse(void);
	virtual void					IN_Accumulate(void);
	virtual void					IN_ClearStates(void);
	virtual bool					IN_IsKeyDown(const char *name, bool& isdown);
	virtual void					IN_OnMouseWheeled(int nDelta);
	virtual int						IN_KeyEvent(int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding);
	virtual void					IN_SetSampleTime(float frametime);
	virtual void					CreateMove(int sequence_number, float input_sample_frametime, bool active);
	virtual void					ExtraMouseSample(float frametime, bool active);
	virtual bool					WriteUsercmdDeltaToBuffer(bf_write *buf, int from, int to, bool isnewcommand);
	virtual void					EncodeUserCmdToBuffer(bf_write& buf, int slot);
	virtual void					DecodeUserCmdFromBuffer(bf_read& buf, int slot);
	virtual void					View_Render(vrect_t *rect);
	virtual void					RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw);
	virtual void					View_Fade(ScreenFade_t *pSF);
	virtual void					SetCrosshairAngle(const QAngle& angle);
	virtual void					InitSprite(CEngineSprite *pSprite, const char *loadname);
	virtual void					ShutdownSprite(CEngineSprite *pSprite);
	virtual int						GetSpriteSize(void) const;
	virtual void					VoiceStatus(int entindex, qboolean bTalking);
	virtual void					InstallStringTableCallback(const char *tableName);
	virtual void					FrameStageNotify(ClientFrameStage_t curStage);
	virtual bool					DispatchUserMessage(int type, unsigned int a3, unsigned int length, const void *msg_data);
};

class INetChannel;
class CClockDriftMgr
{
public:
	float m_ClockOffsets[17];   //0x0000
	uint32_t m_cur_clock_offset; //0x0044
	uint32_t m_server_tick;     //0x0048
	uint32_t m_client_tick;     //0x004C
}; //Size: 0x0050

class ClientState {
public:
public:
	void force_full_update() {
		if(*reinterpret_cast<int*>(uintptr_t(this) + Offsets::clientstate_delta_ticks) != -1)
			*reinterpret_cast<int*>(uintptr_t(this) + Offsets::clientstate_delta_ticks) = -1;
	};

	char pad_0000[156];             //0x0000
	INetChannel* m_net_channel;          //0x009C
	uint32_t m_challenge_nr;        //0x00A0
	char pad_00A4[100];             //0x00A4
	uint32_t m_signon_state;        //0x0108
	char pad_010C[8];               //0x010C
	float m_next_cmd_time;          //0x0114
	uint32_t m_server_count;        //0x0118
	uint32_t m_current_sequence;    //0x011C
	char pad_0120[8];               //0x0120
	CClockDriftMgr m_clock_drift_mgr; //0x0128
	uint32_t m_delta_tick;          //0x0178
	bool m_paused;                 //0x017C
	char pad_017D[3];               //0x017D
	uint32_t m_view_entity;         //0x0180
	uint32_t m_player_slot;         //0x0184
	char m_level_name[260];        //0x0188
	char m_level_name_short[80];    //0x028C
	char m_group_name[80];         //0x02DC
	char pad_032С[92];				//0x032С
	uint32_t m_max_clients;         //0x0388
	char pad_0314[18820];           //0x0314
	float m_last_server_tick_time;   //0x4C98
	bool m_insimulation;              //0x4C9C
	char pad_4C9D[3];               //0x4C9D
	uint32_t m_oldtickcount;          //0x4CA0
	float m_tick_remainder;          //0x4CA4
	float m_frame_time;              //0x4CA8
	uint32_t m_last_outgoing_command;//0x4CAC
	uint32_t m_choked_commands;     //0x4CB0
	uint32_t m_last_command_ack;     //0x4CB4
	uint32_t m_command_ack;			//0x4CB8
	uint32_t m_sound_sequence;      //0x4CBC
	char pad_4CC0[80];              //0x4CC0
	QAngle viewangles;              //0x4D10
};

