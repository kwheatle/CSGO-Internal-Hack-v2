#pragma once
#pragma warning( disable : 4244 ) // warning C4244: '=': conversion possible loss of data
#include "Definitions\Definitions.h"
#include "Math\Vector.h"
#include "Math\VMatrix.h"
#include "Misc\CUserCmd.h"
#include "Misc\Color.h"
#include "Misc\EngineTraceStructs.h"
#include "Misc\player_info.h"
#include "Interfaces\Client.h"
#include "Interfaces\ClientMode.h"
#include "Interfaces\Engine.h"
#include "Interfaces\EntityList.h"
#include "Interfaces\EngineTrace.h"
#include "Interfaces\Panel.h"
#include "Interfaces\Surface.h"
#include "Interfaces\InputSystem.h"
#include "Interfaces\InputInternal.h"
#include "Interfaces\GameEvent.h"
#include "Interfaces\Globals.h"
#include "Math\Math.h"
//#include "Misc\CBaseCombatWeapon.h"
#include "Misc\CBaseEntity.h"
#include "Misc\Studio.h"
#include "Misc\player_info.h"
#include "Misc\IVModelInfoClient.h"
#include "Misc\IRenderView.h"
#include "Misc\IClientRenderable.h"
#include "Misc\IMaterialSystem.h"
#include "Misc\IVDebugOverlay.h"
#include "Misc\ConVar.h"
#include "Misc\INetChannelInfo.h"
#include "Interfaces\CPrediction.h"
#include "Misc\CInput.h"

#include <map>

#define TICK_INTERVAL			(gInterface.globals->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

typedef unsigned short mdl_handle_t;

enum
{
	MDLHANDLE_INVALID = (mdl_handle_t)~0
};

enum MDLCacheDataType_t
{
	// Callbacks to get called when data is loaded or unloaded for these:
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,

	// Callbacks NOT called when data is loaded or unloaded for these:
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK,
};

class i_mdl_cache_notify
{
public:
	// Called right after the data is loaded
	virtual void on_data_loaded(MDLCacheDataType_t type, mdl_handle_t handle) = 0;

	// Called right before the data is unloaded
	virtual void on_data_unloaded(MDLCacheDataType_t type, mdl_handle_t handle) = 0;
};

enum MDLCacheFlush_t
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,

	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;
struct studiohwdata_t;

class IMdlCache
{
public:
	// Used to install callbacks for when data is loaded + unloaded
	// Returns the prior notify
	virtual void set_cache_notify(i_mdl_cache_notify *pNotify) = 0;

	// NOTE: This assumes the "GAME" path if you don't use
	// the UNC method of specifying files. This will also increment
	// the reference count of the MDL
	virtual mdl_handle_t find_mdl(const char *pMDLRelativePath) = 0;

	// Reference counting
	virtual int add_ref(mdl_handle_t handle) = 0;
	virtual int release(mdl_handle_t handle) = 0;
	virtual int get_ref(mdl_handle_t handle) = 0;

	// Gets at the various data associated with a MDL
	virtual studiohdr_t *get_studio_hdr(mdl_handle_t handle) = 0;
	virtual studiohwdata_t *get_hardware_data(mdl_handle_t handle) = 0;
	virtual vcollide_t *get_v_collide(mdl_handle_t handle) = 0;
	virtual unsigned char *get_anim_block(mdl_handle_t handle, int nBlock) = 0;
	virtual virtualmodel_t *get_virtual_model(mdl_handle_t handle) = 0;
	virtual int get_autoplay_list(mdl_handle_t handle, unsigned short **pOut) = 0;
	virtual vertexFileHeader_t *get_vertex_data(mdl_handle_t handle) = 0;

	// Brings all data associated with an MDL into memory
	virtual void touch_all_data(mdl_handle_t handle) = 0;

	// Gets/sets user data associated with the MDL
	virtual void set_user_data(mdl_handle_t handle, void* pData) = 0;
	virtual void *get_user_data(mdl_handle_t handle) = 0;

	// Is this MDL using the error model?
	virtual bool is_error_model(mdl_handle_t handle) = 0;

	// Flushes the cache, force a full discard
	virtual void flush(MDLCacheFlush_t nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Flushes a particular model out of memory
	virtual void flush(mdl_handle_t handle, int nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Returns the name of the model (its relative path)
	virtual const char *get_model_name(mdl_handle_t handle) = 0;

	// faster access when you already have the studiohdr
	virtual virtualmodel_t *get_virtual_model_fast(const studiohdr_t *pStudioHdr, mdl_handle_t handle) = 0;

	// all cache entries that subsequently allocated or successfully checked
	// are considered "locked" and will not be freed when additional memory is needed
	virtual void begin_lock() = 0;

	// reset all protected blocks to normal
	virtual void end_lock() = 0;

	// returns a pointer to a counter that is incremented every time the cache has been out of the locked state (EVIL)
	virtual int *get_frame_unlock_counter_ptr_old() = 0;

	// Finish all pending async operations
	virtual void finish_pending_loads() = 0;

	virtual vcollide_t *get_v_collide_ex(mdl_handle_t handle, bool synchronousLoad = true) = 0;
	virtual bool get_v_collide_size(mdl_handle_t handle, int *pVCollideSize) = 0;

	virtual bool get_async_load(MDLCacheDataType_t type) = 0;
	virtual bool set_async_load(MDLCacheDataType_t type, bool bAsync) = 0;

	virtual void begin_map_load() = 0;
	virtual void end_map_load() = 0;
	virtual void mark_as_loaded(mdl_handle_t handle) = 0;

	virtual void init_preload_data(bool rebuild) = 0;
	virtual void shutdown_preload_data() = 0;

	virtual bool is_data_loaded(mdl_handle_t handle, MDLCacheDataType_t type) = 0;

	virtual int *get_frame_unlock_counter_ptr(MDLCacheDataType_t type) = 0;

	virtual studiohdr_t *lock_studio_hdr(mdl_handle_t handle) = 0;
	virtual void unlock_studio_hdr(mdl_handle_t handle) = 0;

	virtual bool preload_model(mdl_handle_t handle) = 0;

	// Hammer uses this. If a model has an error loading in GetStudioHdr, then it is flagged
	// as an error model and any further attempts to load it will just get the error model.
	// That is, until you call this function. Then it will load the correct model.
	virtual void reset_error_model_status(mdl_handle_t handle) = 0;

	virtual void mark_frame() = 0;
};

enum knifeDefinitionIndex               // id
{
	WEAPON_KNIFE = 42,
	WEAPON_KNIFE_T = 59,
	WEAPON_KNIFE_BAYONET = 500,         // 0
	WEAPON_KNIFE_FLIP = 505,            // 1
	WEAPON_KNIFE_GUT = 506,             // 2
	WEAPON_KNIFE_KARAMBIT = 507,        // 3
	WEAPON_KNIFE_M9_BAYONET = 508,      // 4
	WEAPON_KNIFE_TACTICAL = 509,        // 5
	WEAPON_KNIFE_FALCHION = 512,        // 6
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,  // 7
	WEAPON_KNIFE_BUTTERFLY = 515,       // 8
	WEAPON_KNIFE_PUSH = 516,            // 9
	WEAPON_KNIFE_URSUS = 519,           // 10
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520, // 11
	WEAPON_KNIFE_STILETTO = 522,        // 12
	WEAPON_KNIFE_WIDOWMAKER = 523       // 13
};

struct weapon_info {
	char* mdlName;
	char* knifeName;
};

const static std::map<int, char*> info =
{
	{ WEAPON_KNIFE,"models/weapons/v_knife_default_ct.mdl" },
	{ WEAPON_KNIFE_T,"models/weapons/v_knife_default_t.mdl"},
	{ WEAPON_KNIFE_BAYONET,"models/weapons/v_knife_bayonet.mdl"},
	{ WEAPON_KNIFE_FLIP,"models/weapons/v_knife_flip.mdl"},
	{ WEAPON_KNIFE_GUT,"models/weapons/v_knife_gut.mdl"},
	{ WEAPON_KNIFE_KARAMBIT, "models/weapons/v_knife_karam.mdl"},
	{ WEAPON_KNIFE_M9_BAYONET, "models/weapons/v_knife_m9_bay.mdl"},
	{ WEAPON_KNIFE_TACTICAL, "models/weapons/v_knife_tactical.mdl"},
	{ WEAPON_KNIFE_FALCHION, "models/weapons/v_knife_falchion_advanced.mdl"},
	{ WEAPON_KNIFE_SURVIVAL_BOWIE,"models/weapons/v_knife_survival_bowie.mdl"},
	{ WEAPON_KNIFE_BUTTERFLY, "models/weapons/v_knife_butterfly.mdl"},
	{ WEAPON_KNIFE_PUSH, "models/weapons/v_knife_push.mdl"},
	{ WEAPON_KNIFE_URSUS, "models/weapons/v_knife_ursus.mdl"},
	{ WEAPON_KNIFE_GYPSY_JACKKNIFE, "models/weapons/v_knife_gypsy_jackknife.mdl"},
	{ WEAPON_KNIFE_STILETTO, "models/weapons/v_knife_stiletto.mdl"},
	{ WEAPON_KNIFE_WIDOWMAKER,"models/weapons/v_knife_widowmaker.mdl"},
};
/*
#include "Definitions\Definitions.h"
#include "Math\Vector.h"
#include "Math\VMatrix.h"
#include "Misc\CUserCmd.h"
#include "Misc\Color.h"
#include "Misc\EngineTraceStructs.h"
#include "Misc\player_info.h"
#include "Interfaces\Interfaces.h"
#include "Interfaces\Client.h"
#include "Interfaces\ClientMode.h"
#include "Interfaces\Engine.h"
#include "Interfaces\EntityList.h"
#include "Interfaces\EngineTrace.h"
#include "Interfaces\Panel.h"
#include "Interfaces\Surface.h"
#include "Interfaces\InputSystem.h"
#include "Interfaces\InputInternal.h"
#include "Interfaces\GameEvent.h"
#include "Interfaces\Globals.h"
#include "Math\Math.h"
#include "Utils\Utils.h"
#include "Misc\CBaseCombatWeapon.h"
#include "Misc\CBaseEntity.h"
#include "Misc\Studio.h"
#include "Misc\player_info.h"
#include "Misc\IVModelInfoClient.h"
#include "Misc\IRenderView.h"
#include "Misc\IClientRenderable.h"
#include "Misc\IMaterialSystem.h"
#include "Misc\IVDebugOverlay.h"
*/


