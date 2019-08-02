#pragma once
#include "../sdk.h"
#include "../Misc/IVModelRender.h"

typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);
typedef void* (*InstantiateInterfaceFn)();

class IMaterial;
typedef int ImageFormat;

class IAppSystem
{
public:
	virtual bool                            Connect(CreateInterfaceFn factory) = 0;                                     // 0
	virtual void                            Disconnect() = 0;                                                           // 1
	virtual void*                           QueryInterface(const char *pInterfaceName) = 0;                             // 2
	virtual int /*InitReturnVal_t*/         Init() = 0;                                                                 // 3
	virtual void                            Shutdown() = 0;                                                             // 4
	virtual const void* /*AppSystemInfo_t*/ GetDependencies() = 0;                                                      // 5
	virtual int /*AppSystemTier_t*/         GetTier() = 0;                                                              // 6
	virtual void                            Reconnect(CreateInterfaceFn factory, const char *pInterfaceName) = 0;       // 7
	virtual bool							IsSingleton() = 0;                                                          // 8
};


enum CompiledVtfFlags
{
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_PWL_CORRECTED = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,
	TEXTUREFLAGS_PRE_SRGB = 0x00080000,
	TEXTUREFLAGS_UNUSED_00100000 = 0x00100000,
	TEXTUREFLAGS_UNUSED_00200000 = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,
	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,
	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,
	TEXTUREFLAGS_CLAMPU = 0x02000000,
	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,
	TEXTUREFLAGS_SSBUMP = 0x08000000,
	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,
	TEXTUREFLAGS_BORDER = 0x20000000,
	TEXTUREFLAGS_UNUSED_40000000 = 0x40000000,
	TEXTUREFLAGS_UNUSED_80000000 = 0x80000000
};

enum StandardLightmap_t
{
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE = -1,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE_BUMP = -2,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_USER_DEFINED = -3
};


struct MaterialSystem_SortInfo_t
{
	IMaterial	*material;
	int			lightmapPageID;
};

enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};

enum MaterialContextType_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};

enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};

struct MaterialTextureInfo_t
{
	int iExcludeInformation;
};

struct ApplicationPerformanceCountersInfo_t
{
	float msMain;
	float msMST;
	float msGPU;
	float msFlip;
	float msTotal;
};

struct ApplicationInstantCountersInfo_t
{
	uint32_t m_nCpuActivityMask;
	uint32_t m_nDeferredWordsAllocated;
};
struct MaterialAdapterInfo_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;			// This is the *preferred* dx support level
	int m_nMinDXSupportLevel;
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
};

struct MaterialVideoMode_t
{
	int m_Width;			// if width and height are 0 and you select 
	int m_Height;			// windowed mode, it'll use the window size
	ImageFormat m_Format;	// use ImageFormats (ignored for windowed mode)
	int m_RefreshRate;		// 0 == default (ignored for windowed mode)
};
enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT,
};

enum RestoreChangeFlags_t
{
	MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED = 0x1,
	MATERIAL_RESTORE_RELEASE_MANAGED_RESOURCES = 0x2,
};

enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,
	RT_SIZE_DEFAULT = 1,
	RT_SIZE_PICMIP = 2,
	RT_SIZE_HDR = 3,
	RT_SIZE_FULL_FRAME_BUFFER = 4,
	RT_SIZE_OFFSCREEN = 5,
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6
};

enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3,
};

struct ShaderStencilState_t {
	bool m_bEnable;
	int m_FailOp;
	int m_ZFailOp;
	int m_PassOp;
	int m_CompareFunc;
	int m_nReferenceValue;
	uint32_t m_nTestMask;
	uint32_t m_nWriteMask;
};

typedef void(*MaterialBufferReleaseFunc_t)(int nChangeFlags);	// see RestoreChangeFlags_t
typedef void(*MaterialBufferRestoreFunc_t)(int nChangeFlags);	// see RestoreChangeFlags_t
typedef void(*ModeChangeCallbackFunc_t)(void);
typedef void(*EndFrameCleanupFunc_t)(void);
typedef bool(*EndFramePriorToNextContextFunc_t)(void);
typedef void(*OnLevelShutdownFunc_t)(void *pUserData);

typedef unsigned short MaterialHandle_t;
#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
DECLARE_POINTER_HANDLE(MaterialLock_t);

// These are given to FindMaterial to reference the texture groups that Show up on the 
#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		  "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"

class IMaterialProxyFactory;
class IMaterialSystemHardwareConfig;
class IShader;
struct AspectRatioInfo_t;
struct MaterialSystem_Config_t;
struct MaterialSystemHWID_t;

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),
	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	//MATERIAL_VAR_UNUSED = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17), // OBSOLETE
	MATERIAL_VAR_UNUSED = (1 << 18), // UNUSED
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = (1 << 30),
	MATERIAL_VAR_VERTEXFOG = (1 << 31),
};

struct MaterialSystemHardwareIdentifier_t;
class IFileList;
class IMatRenderContext;

class IMaterialSystem : public IAppSystem
{
public:

	virtual CreateInterfaceFn               Init(char const* pShaderAPIDLL, IMaterialProxyFactory *pMaterialProxyFactory, CreateInterfaceFn fileSystemFactory, CreateInterfaceFn cvarFactory = NULL) = 0;
	virtual void                            SetShaderAPI(char const *pShaderAPIDLL) = 0;
	virtual void                            SetAdapter(int nAdapter, int nFlags) = 0;
	virtual void                            ModInit() = 0;
	virtual void                            ModShutdown() = 0;
	virtual void                            SetThreadMode(MaterialThreadMode_t mode, int nServiceThread = -1) = 0;
	virtual MaterialThreadMode_t            GetThreadMode() = 0;
	virtual void                            ExecuteQueued() = 0;
	virtual void                            OnDebugEvent(const char *pEvent) = 0;
	virtual IMaterialSystemHardwareConfig*  GetHardwareConfig(const char *pVersion, int *returnCode) = 0;
	virtual void                            __unknown() = 0;
	virtual bool                            UpdateConfig(bool bForceUpdate) = 0; //20
	virtual bool                            OverrideConfig(const MaterialSystem_Config_t &config, bool bForceUpdate) = 0;
	virtual const MaterialSystem_Config_t&  GetCurrentConfigForVideoCard() const = 0;
	virtual bool                            GetRecommendedConfigurationInfo(int nDXLevel, KeyValues * pKeyValues) = 0;
	virtual int                             GetDisplayAdapterCount() const = 0;
	virtual int                             GetCurrentAdapter() const = 0;
	virtual void                            GetDisplayAdapterInfo(int adapter, MaterialAdapterInfo_t& info) const = 0;
	virtual int                             GetModeCount(int adapter) const = 0;
	virtual void                            GetModeInfo(int adapter, int mode, MaterialVideoMode_t& info) const = 0;
	virtual void                            AddModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;
	virtual void                            GetDisplayMode(MaterialVideoMode_t& mode) const = 0; //30
	virtual bool                            SetMode(void* hwnd, const MaterialSystem_Config_t &config) = 0;
	virtual bool                            SupportsMSAAMode(int nMSAAMode) = 0;
	virtual const MaterialSystemHWID_t&     GetVideoCardIdentifier(void) const = 0;
	virtual void                            SpewDriverInfo() const = 0;
	virtual void                            GetBackBufferDimensions(int &width, int &height) const = 0;
	virtual ImageFormat                     GetBackBufferFormat() const = 0;
	virtual const AspectRatioInfo_t&        GetAspectRatioInfo() const = 0;
	virtual bool                            SupportsHDRMode(HDRType_t nHDRModede) = 0;
	virtual bool                            AddView(void* hwnd) = 0;
	virtual void                            RemoveView(void* hwnd) = 0; //40
	virtual void                            SetView(void* hwnd) = 0;
	virtual void                            BeginFrame(float frameTime) = 0;
	virtual void                            EndFrame() = 0;
	virtual void                            Flush(bool flushHardware = false) = 0;
	virtual unsigned int                    GetCurrentFrameCount() = 0;
	virtual void                            SwapBuffers() = 0;
	virtual void                            EvictManagedResources() = 0;
	virtual void                            ReleaseResources(void) = 0;
	virtual void                            ReacquireResources(void) = 0;
	virtual void                            AddReleaseFunc(MaterialBufferReleaseFunc_t func) = 0; //50
	virtual void                            RemoveReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
	virtual void                            AddRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void                            RemoveRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void                            AddEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;
	virtual void                            RemoveEndFrameCleanupFunc(EndFrameCleanupFunc_t func) = 0;
	virtual void                            OnLevelShutdown() = 0;
	virtual bool                            AddOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void *pUserData) = 0;
	virtual bool                            RemoveOnLevelShutdownFunc(OnLevelShutdownFunc_t func, void *pUserData) = 0;
	virtual void                            OnLevelLoadingComplete() = 0;
	virtual void                            ResetTempHWMemory(bool bExitingLevel = false) = 0; //60
	virtual void                            HandleDeviceLost() = 0;
	virtual int                             ShaderCount() const = 0;
	virtual int                             GetShaders(int nFirstShader, int nMaxCount, IShader **ppShaderList) const = 0;
	virtual int                             ShaderFlagCount() const = 0;
	virtual const char*                     ShaderFlagName(int nIndex) const = 0;
	virtual void                            GetShaderFallback(const char *pShaderName, char *pFallbackShader, int nFallbackLength) = 0;
	virtual IMaterialProxyFactory*          GetMaterialProxyFactory() = 0;
	virtual void                            SetMaterialProxyFactory(IMaterialProxyFactory* pFactory) = 0;
	virtual void                            EnableEditorMaterials() = 0;
	virtual void                            EnableGBuffers() = 0; //70
	virtual void                            SetInStubMode(bool bInStubMode) = 0;
	virtual void                            DebugPrintUsedMaterials(const char *pSearchSubString, bool bVerbose) = 0;
	virtual void                            DebugPrintUsedTextures(void) = 0;
	virtual void                            ToggleSuppressMaterial(char const* pMaterialName) = 0;
	virtual void                            ToggleDebugMaterial(char const* pMaterialName) = 0;
	virtual bool                            UsingFastClipping(void) = 0;
	virtual int                             StencilBufferBits(void) = 0; //number of bits per pixel in the stencil buffer
	virtual void                            UncacheAllMaterials() = 0;
	virtual void                            UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;
	virtual void                            CacheUsedMaterials() = 0; //80
	virtual void                            ReloadTextures() = 0;
	virtual void                            ReloadMaterials(const char *pSubString = NULL) = 0;
	virtual IMaterial*                      CreateMaterial(const char *pMaterialName, KeyValues *pVMTKeyValues) = 0;
	virtual IMaterial*                      FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL) = 0;
	virtual void							unk0() = 0;
	virtual MaterialHandle_t                FirstMaterial() const = 0;
	virtual MaterialHandle_t                NextMaterial(MaterialHandle_t h) const = 0;
	virtual MaterialHandle_t                InvalidMaterial() const = 0;
	virtual IMaterial*                      GetMaterial(MaterialHandle_t h) const = 0;
	virtual int                             GetNumMaterials() const = 0;
	virtual ITexture*                       FindTexture(char const* pTextureName, const char *pTextureGroupName, bool complain = true) = 0;
	virtual bool                            IsTextureLoaded(char const* pTextureName) const = 0;
	virtual ITexture*                       CreateProceduralTexture(const char	*pTextureName, const char *pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags) = 0;
	virtual void                            BeginRenderTargetAllocation() = 0;
	virtual void                            EndRenderTargetAllocation() = 0; // Simulate an Alt-Tab in here, which causes a release/restore of all resources
	virtual ITexture*                       unk1() = 0;
	virtual ITexture*                       unk2() = 0;
	virtual ITexture*                       unk3() = 0;
	virtual ITexture*						unk4() = 0;

};


class IMaterial {
public:
	// Get the name of the material.  This is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char *GetName() const = 0;
	virtual const char *GetTextureGroupName() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual int /*PreviewImageRetVal_t*/ GetPreviewImageProperties(int *width, int *height,
		ImageFormat *imageFormat,
		bool *isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual int /*PreviewImageRetVal_t*/ GetPreviewImage(unsigned char *data,
		int width, int height,
		ImageFormat imageFormat) const = 0;
	// 
	virtual int GetMappingWidth() = 0;
	virtual int GetMappingHeight() = 0;

	virtual int GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool InMaterialPage(void) = 0;
	virtual void GetMaterialOffset(float *pOffset) = 0;
	virtual void GetMaterialScale(float *pScale) = 0;
	virtual IMaterial *GetMaterialPage(void) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual IMaterialVar *FindVar(const char *varName, bool *found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void IncrementReferenceCount(void) = 0;
	virtual void DecrementReferenceCount(void) = 0;

	inline void AddRef() {
		IncrementReferenceCount();
	}

	inline void Release1() {
		DecrementReferenceCount();
	}

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int GetEnumerationID(void) const = 0;

	virtual void GetLowResColorSample(float s, float t, float *color) const = 0;

	// This computes the state snapshots for this material
	virtual void RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool IsVertexLit() = 0;

	// Gets the vertex format
	virtual void GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool HasProxy(void) const = 0;

	virtual bool UsesEnvCubemap(void) = 0;

	virtual bool NeedsTangentSpace(void) = 0;

	virtual bool NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool NeedsSoftwareSkinning(void) = 0;

	// Apply constant color or alpha modulation
	virtual void AlphaModulate(float alpha) = 0;
	virtual void ColorModulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;

	// Gets material reflectivity
	virtual void GetReflectivity(vec3_t &reflect) = 0;

	// Gets material property flags
	virtual bool GetPropertyFlag(int /*MaterialPropertyTypes_t*/ type) = 0;

	// Is the material visible from both sides?
	virtual bool IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void SetShader(const char *pShaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int GetNumPasses(void) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int GetTextureMemoryBytes(void) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool NeedsLightmapBlendAlpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool NeedsSoftwareLighting(void) = 0;

	// Gets at the shader parameters
	virtual int ShaderParamCount() const = 0;
	virtual IMaterialVar **GetShaderParams(void) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool IsErrorMaterial() const = 0;

	virtual void Unused() = 0;

	// Gets the current alpha modulation
	virtual float GetAlphaModulation() = 0;
	virtual void GetColorModulation(float *r, float *g, float *b) = 0;

	// Is this translucent given a particular alpha modulation?
	virtual bool IsTranslucentUnderModulation(float fAlphaModulation = 1.0f) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual IMaterialVar *FindVarFast(char const *pVarName, unsigned int *pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void SetShaderAndParams(KeyValues *pKeyValues) = 0;
	virtual const char *GetShaderName() const = 0;

	virtual void DeleteIfUnreferenced() = 0;

	virtual bool IsSpriteCard() = 0;

	virtual void CallBindProxy(void *proxyData) = 0;

	virtual void RefreshPreservingMaterialVars() = 0;

	virtual bool WasReloadedFromWhitelist() = 0;

	virtual bool SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;

	virtual int GetReferenceCount() const = 0;
};

struct LightDesc_t;
class Vector4D;
struct MaterialMatrixMode_t;
struct MaterialCullMode_t;
struct MaterialFogMode_t;
struct MaterialHeightClipMode_t;
class IMesh;
struct MaterialIndexFormat_t;
class IVertexBuffer;
class IIndexBuffer;
struct OcclusionQueryObjectHandle_t;
struct MaterialPrimitiveType_t;
struct FlashlightState_t;
struct MorphFormat_t;
struct MorphWeight_t;
struct Rect_t;
class IMorph;
class ICallQueue;
struct DeformationBase_t;
struct ColorCorrectionHandle_t;
struct MaterialNonInteractiveMode_t;

typedef unsigned long ulong_t;

class IMatRenderContext {
public:
	virtual void BeginRender() = 0;
	virtual void EndRender() = 0;

	virtual void Flush(bool flushHardware = false) = 0;

	virtual void BindLocalCubemap(ITexture *pTexture) = 0;

	// pass in an ITexture (that is build with "rendertarget" "1") or
	// pass in NULL for the regular backbuffer.
	virtual void SetRenderTarget(ITexture *pTexture) = 0;
	virtual ITexture *GetRenderTarget(void) = 0;

	virtual void GetRenderTargetDimensions(int &width, int &height) const = 0;

	// Bind a material is current for rendering.
	virtual void Bind(IMaterial *material, void *proxyData = 0) = 0;
	// Bind a lightmap page current for rendering.  You only have to 
	// do this for materials that require lightmaps.
	virtual void BindLightmapPage(int lightmapPageID) = 0;

	// inputs are between 0 and 1
	virtual void DepthRange(float zNear, float zFar) = 0;

	virtual void ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;

	// read to a unsigned char rgb image.
	virtual void ReadPixels(int x, int y, int width, int height, unsigned char *data, ImageFormat dstFormat) = 0;

	// Sets lighting
	virtual void SetAmbientLight(float r, float g, float b) = 0;
	virtual void SetLight(int lightNum, const LightDesc_t &desc) = 0;

	// The faces of the cube are specified in the same order as cubemap textures
	virtual void SetAmbientLightCube(Vector4D cube[6]) = 0;

	// Blit the backbuffer to the framebuffer texture
	virtual void CopyRenderTargetToTexture(ITexture *pTexture) = 0;

	// Set the current texture that is a copy of the framebuffer.
	virtual void SetFrameBufferCopyTexture(ITexture *pTexture, int textureIndex = 0) = 0;
	virtual ITexture *GetFrameBufferCopyTexture(int textureIndex) = 0;

	//
	// end vertex array api
	//

	// matrix api
	virtual void MatrixMode(MaterialMatrixMode_t matrixMode) = 0;
	virtual void PushMatrix(void) = 0;
	virtual void PopMatrix(void) = 0;
	virtual void LoadMatrix(float const &matrix) = 0;
	virtual void LoadMatrix(matrix3x4_t const &matrix) = 0;
	virtual void MultMatrix(float const matrix) = 0;
	virtual void MultMatrix(matrix3x4_t const &matrix) = 0;
	virtual void MultMatrixLocal(float const &matrix) = 0;
	virtual void MultMatrixLocal(matrix3x4_t const &matrix) = 0;
	virtual void GetMatrix(MaterialMatrixMode_t matrixMode, float *matrix) = 0;
	virtual void GetMatrix(MaterialMatrixMode_t matrixMode, matrix3x4_t *matrix) = 0;
	virtual void LoadIdentity(void) = 0;
	virtual void Ortho(double left, double top, double right, double bottom, double zNear, double zFar) = 0;
	virtual void PerspectiveX(double fovx, double aspect, double zNear, double zFar) = 0;
	virtual void PickMatrix(int x, int y, int width, int height) = 0;
	virtual void Rotate(float angle, float x, float y, float z) = 0;
	virtual void Translate(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	// end matrix api

	// Sets/gets the viewport
	virtual void Viewport(int x, int y, int width, int height) = 0;
	virtual void GetViewport(int &x, int &y, int &width, int &height) const = 0;

	// The cull mode
	virtual void CullMode(MaterialCullMode_t cullMode) = 0;

	// end matrix api

	// This could easily be extended to a general user clip plane
	virtual void SetHeightClipMode(MaterialHeightClipMode_t nHeightClipMode) = 0;
	// garymcthack : fog z is always used for heightclipz for now.
	virtual void SetHeightClipZ(float z) = 0;

	// Fog methods...
	virtual void FogMode(MaterialFogMode_t fogMode) = 0;
	virtual void FogStart(float fStart) = 0;
	virtual void FogEnd(float fEnd) = 0;
	virtual void SetFogZ(float fogZ) = 0;
	virtual MaterialFogMode_t GetFogMode(void) = 0;

	virtual void FogColor3f(float r, float g, float b) = 0;
	virtual void FogColor3fv(float const *rgb) = 0;
	virtual void FogColor3ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void FogColor3ubv(unsigned char const *rgb) = 0;

	virtual void GetFogColor(unsigned char *rgb) = 0;

	// Sets the number of bones for skinning
	virtual void SetNumBoneWeights(int numBones) = 0;

	// Creates/destroys Mesh
	virtual IMesh *CreateStaticMesh(VertexFormat_t fmt, const char *pTextureBudgetGroup, IMaterial *pMaterial = NULL) =
		0;
	virtual void DestroyStaticMesh(IMesh *mesh) = 0;

	// Gets the dynamic mesh associated with the currently bound material
	// note that you've got to render the mesh before calling this function 
	// a second time. Clients should *not* call DestroyStaticMesh on the mesh 
	// returned by this call.
	// Use buffered = false if you want to not have the mesh be buffered,
	// but use it instead in the following pattern:
	//		meshBuilder.Begin
	//		meshBuilder.End
	//		Draw partial
	//		Draw partial
	//		Draw partial
	//		meshBuilder.Begin
	//		meshBuilder.End
	//		etc
	// Use Vertex or Index Override to supply a static vertex or index buffer
	// to use in place of the dynamic buffers.
	//
	// If you pass in a material in pAutoBind, it will automatically bind the
	// material. This can be helpful since you must bind the material you're
	// going to use BEFORE calling GetDynamicMesh.
	virtual IMesh *GetDynamicMesh(
		bool buffered = true,
		IMesh *pVertexOverride = 0,
		IMesh *pIndexOverride = 0,
		IMaterial *pAutoBind = 0) = 0;

	// ------------ New Vertex/Index Buffer interface ----------------------------
	// Do we need support for bForceTempMesh and bSoftwareVertexShader?
	// I don't think we use bSoftwareVertexShader anymore. .need to look into bForceTempMesh.
	virtual IVertexBuffer *CreateStaticVertexBuffer(VertexFormat_t fmt, int nVertexCount,
		const char *pTextureBudgetGroup) = 0;
	virtual IIndexBuffer *CreateStaticIndexBuffer(MaterialIndexFormat_t fmt, int nIndexCount,
		const char *pTextureBudgetGroup) = 0;
	virtual void DestroyVertexBuffer(IVertexBuffer *) = 0;
	virtual void DestroyIndexBuffer(IIndexBuffer *) = 0;
	// Do we need to specify the stream here in the case of locking multiple dynamic VBs on different streams?
	virtual IVertexBuffer *GetDynamicVertexBuffer(int streamID, VertexFormat_t vertexFormat, bool bBuffered = true) = 0;
	virtual IIndexBuffer *GetDynamicIndexBuffer(MaterialIndexFormat_t fmt, bool bBuffered = true) = 0;
	virtual void BindVertexBuffer(int streamID, IVertexBuffer *pVertexBuffer, int nOffsetInBytes, int nFirstVertex,
		int nVertexCount, VertexFormat_t fmt, int nRepetitions = 1) = 0;
	virtual void BindIndexBuffer(IIndexBuffer *pIndexBuffer, int nOffsetInBytes) = 0;
	virtual void Draw(MaterialPrimitiveType_t primitiveType, int firstIndex, int numIndices) = 0;
	// ------------ End ----------------------------

	// Selection mode methods
	virtual int SelectionMode(bool selectionMode) = 0;
	virtual void SelectionBuffer(unsigned int *pBuffer, int size) = 0;
	virtual void ClearSelectionNames() = 0;
	virtual void LoadSelectionName(int name) = 0;
	virtual void PushSelectionName(int name) = 0;
	virtual void PopSelectionName() = 0;

	// Sets the Clear Color for ClearBuffer....
	virtual void ClearColor3ub(unsigned char r, unsigned char g, unsigned char b) = 0;
	virtual void ClearColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;

	// Allows us to override the depth buffer setting of a material
	virtual void OverrideDepthEnable(bool bEnable, bool bDepthEnable) = 0;

	// FIXME: This is a hack required for NVidia/XBox, can they fix in drivers?
	virtual void DrawScreenSpaceQuad(IMaterial *pMaterial) = 0;

	// For debugging and building recording files. This will stuff a token into the recording file,
	// then someone doing a playback can watch for the token.
	virtual void SyncToken(const char *pToken) = 0;

	// FIXME: REMOVE THIS FUNCTION!
	// The only reason why it's not gone is because we're a week from ship when I found the bug in it
	// and everything's tuned to use it.
	// It's returning values which are 2x too big (it's returning sphere diameter x2)
	// Use ComputePixelDiameterOfSphere below in all new code instead.
	virtual float ComputePixelWidthOfSphere(const vec3_t &origin, float flRadius) = 0;

	//
	// Occlusion query support
	//

	// Allocate and delete query objects.
	virtual OcclusionQueryObjectHandle_t CreateOcclusionQueryObject(void) = 0;
	virtual void DestroyOcclusionQueryObject(OcclusionQueryObjectHandle_t) = 0;

	// Bracket drawing with begin and end so that we can get counts next frame.
	virtual void BeginOcclusionQueryDrawing(OcclusionQueryObjectHandle_t) = 0;
	virtual void EndOcclusionQueryDrawing(OcclusionQueryObjectHandle_t) = 0;

	// Get the number of pixels rendered between begin and end on an earlier frame.
	// Calling this in the same frame is a huge perf hit!
	virtual int OcclusionQuery_GetNumPixelsRendered(OcclusionQueryObjectHandle_t) = 0;

	virtual void SetFlashlightMode(bool bEnable) = 0;

	virtual void SetFlashlightState(const FlashlightState_t &state, const matrix3x4_t &worldToTexture) = 0;

	// Gets the current height clip mode
	virtual MaterialHeightClipMode_t GetHeightClipMode() = 0;

	// This returns the diameter of the sphere in pixels based on 
	// the current model, view, + projection matrices and viewport.
	virtual float ComputePixelDiameterOfSphere(const vec3_t &vecAbsOrigin, float flRadius) = 0;

	// By default, the material system applies the VIEW and PROJECTION matrices	to the user clip
	// planes (which are specified in world space) to generate projection-space user clip planes
	// Occasionally (for the particle system in hl2, for example), we want to override that
	// behavior and explictly specify a ViewProj transform for user clip planes
	virtual void EnableUserClipTransformOverride(bool bEnable) = 0;
	virtual void UserClipTransform(const matrix3x4_t &worldToView) = 0;

	virtual bool GetFlashlightMode() const = 0;

	// Used to make the handle think it's never had a successful query before
	virtual void ResetOcclusionQueryObject(OcclusionQueryObjectHandle_t) = 0;

	// FIXME: Remove
	virtual void Unused3() {}

	// Creates/destroys morph data associated w/ a particular material
	virtual IMorph *CreateMorph(MorphFormat_t format, const char *pDebugName) = 0;
	virtual void DestroyMorph(IMorph *pMorph) = 0;

	// Binds the morph data for use in rendering
	virtual void BindMorph(IMorph *pMorph) = 0;

	// Sets flexweights for rendering
	virtual void SetFlexWeights(int nFirstWeight, int nCount, const MorphWeight_t *pWeights) = 0;

	// FIXME: Remove
	virtual void Unused4() {};
	virtual void Unused5() {};
	virtual void Unused6() {};
	virtual void Unused7() {};
	virtual void Unused8() {};

	// Read w/ stretch to a host-memory buffer
	virtual void ReadPixelsAndStretch(Rect_t *pSrcRect, Rect_t *pDstRect, unsigned char *pBuffer, ImageFormat dstFormat,
		int nDstStride) = 0;

	// Gets the window size
	virtual void GetWindowSize(int &width, int &height) const = 0;

	// This function performs a texture map from one texture map to the render destination, doing
	// all the necessary pixel/texel coordinate fix ups. fractional values can be used for the
	// src_texture coordinates to get linear sampling - integer values should produce 1:1 mappings
	// for non-scaled operations.
	virtual void DrawScreenSpaceRectangle(
		IMaterial *pMaterial,
		int destx, int desty,
		int width, int height,
		float src_texture_x0, float src_texture_y0, // which texel you want to appear at
													// destx/y
		float src_texture_x1, float src_texture_y1, // which texel you want to appear at
													// destx+width-1, desty+height-1
		int src_texture_width, int src_texture_height, // needed for fixup
		void *pClientRenderable = NULL,
		int nXDice = 1,
		int nYDice = 1) = 0;

	virtual void LoadBoneMatrix(int boneIndex, const matrix3x4_t &matrix) = 0;

	// This version will push the current rendertarget + current viewport onto the stack
	virtual void PushRenderTargetAndViewport() = 0;

	// This version will push a new rendertarget + a maximal viewport for that rendertarget onto the stack
	virtual void PushRenderTargetAndViewport(ITexture *pTexture) = 0;

	// This version will push a new rendertarget + a specified viewport onto the stack
	virtual void PushRenderTargetAndViewport(ITexture *pTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0;

	// This version will push a new rendertarget + a specified viewport onto the stack
	virtual void PushRenderTargetAndViewport(ITexture *pTexture, ITexture *pDepthTexture, int nViewX, int nViewY,
		int nViewW, int nViewH) = 0;

	// This will pop a rendertarget + viewport
	virtual void PopRenderTargetAndViewport(void) = 0;

	// Binds a particular texture as the current lightmap
	virtual void BindLightmapTexture(ITexture *pLightmapTexture) = 0;

	// Blit a subrect of the current render target to another texture
	virtual void CopyRenderTargetToTextureEx(ITexture *pTexture, int nRenderTargetID, Rect_t *pSrcRect,
		Rect_t *pDstRect = NULL) = 0;
	virtual void CopyTextureToRenderTargetEx(int nRenderTargetID, ITexture *pTexture, Rect_t *pSrcRect,
		Rect_t *pDstRect = NULL) = 0;

	// Special off-center perspective matrix for DoF, MSAA jitter and poster rendering
	virtual void PerspectiveOffCenterX(double fovx, double aspect, double zNear, double zFar, double bottom, double top,
		double left, double right) = 0;

	// Rendering parameters control special drawing modes withing the material system, shader
	// system, shaders, and engine. renderparm.h has their definitions.
	virtual void SetFloatRenderingParameter(int parm_number, float value) = 0;
	virtual void SetIntRenderingParameter(int parm_number, int value) = 0;
	virtual void SetVectorRenderingParameter(int parm_number, vec3_t const &value) = 0;

	// stencil buffer operations.
	virtual void SetStencilEnable(bool onoff) = 0;
	virtual void SetStencilFailOperation(int op) = 0;
	virtual void SetStencilZFailOperation(int op) = 0;
	virtual void SetStencilPassOperation(int op) = 0;
	virtual void SetStencilCompareFunction(int cmpfn) = 0;
	virtual void SetStencilReferenceValue(int ref) = 0;
	virtual void SetStencilTestMask(ulong_t msk) = 0;
	virtual void SetStencilWriteMask(ulong_t msk) = 0;
	virtual void ClearStencilBufferRectangle(int xmin, int ymin, int xmax, int ymax, int value) = 0;

	virtual void SetRenderTargetEx(int nRenderTargetID, ITexture *pTexture) = 0;

	// rendering clip planes, beware that only the most recently pushed plane will actually be used in a sizeable chunk of hardware configurations
	// and that changes to the clip planes mid-frame while UsingFastClipping() is true will result unresolvable depth inconsistencies
	virtual void PushCustomClipPlane(const float *pPlane) = 0;
	virtual void PopCustomClipPlane(void) = 0;

	// Returns the number of vertices + indices we can render using the dynamic mesh
	// Passing true in the second parameter will return the max # of vertices + indices
	// we can use before a flush is provoked and may return different values 
	// if called multiple times in succession. 
	// Passing false into the second parameter will return
	// the maximum possible vertices + indices that can be rendered in a single batch
	virtual void GetMaxToRender(IMesh *pMesh, bool bMaxUntilFlush, int *pMaxVerts, int *pMaxIndices) = 0;

	// Returns the max possible vertices + indices to render in a single draw call
	virtual int GetMaxVerticesToRender(IMaterial *pMaterial) = 0;
	virtual int GetMaxIndicesToRender() = 0;
	virtual void DisableAllLocalLights() = 0;
	virtual int CompareMaterialCombos(IMaterial *pMaterial1, IMaterial *pMaterial2, int lightMapID1, int lightMapID2) =
		0;

	virtual IMesh *GetFlexMesh() = 0;

	virtual void SetFlashlightStateEx(const FlashlightState_t &state, const matrix3x4_t &worldToTexture,
		ITexture *pFlashlightDepthTexture) = 0;

	// Returns the currently bound local cubemap
	virtual ITexture *GetLocalCubemap() = 0;

	// This is a version of clear buffers which will only clear the buffer at pixels which pass the stencil test
	virtual void ClearBuffersObeyStencil(bool bClearColor, bool bClearDepth) = 0;

	//enables/disables all entered clipping planes, returns the input from the last time it was called.
	virtual bool EnableClipping(bool bEnable) = 0;

	//get fog distances entered with FogStart(), FogEnd(), and SetFogZ()
	virtual void GetFogDistances(float *fStart, float *fEnd, float *fFogZ) = 0;

	// Hooks for firing PIX events from outside the Material System...
	virtual void BeginPIXEvent(unsigned long color, const char *szName) = 0;
	virtual void EndPIXEvent() = 0;
	virtual void SetPIXMarker(unsigned long color, const char *szName) = 0;

	// Batch API
	// from changelist 166623:
	// - replaced obtuse material system batch usage with an explicit and easier to thread API
	virtual void BeginBatch(IMesh *pIndices) = 0;
	virtual void BindBatch(IMesh *pVertices, IMaterial *pAutoBind = NULL) = 0;
	virtual void DrawBatch(int firstIndex, int numIndices) = 0;
	virtual void EndBatch() = 0;

	// Raw access to the call queue, which can be NULL if not in a queued mode
	virtual ICallQueue *GetCallQueue() = 0;

	// Returns the world-space camera position
	virtual void GetWorldSpaceCameraPosition(vec3_t *pCameraPos) = 0;
	virtual void GetWorldSpaceCameraVectors(vec3_t *pVecForward, vec3_t *pVecRight, vec3_t *pVecUp) = 0;

	// Tone mapping
	virtual void ResetToneMappingScale(float monoscale) = 0; // set scale to monoscale instantly with no chasing
	virtual void SetGoalToneMappingScale(float monoscale) = 0; // set scale to monoscale instantly with no chasing

															   // call TurnOnToneMapping before drawing the 3d scene to get the proper interpolated brightness
															   // value set.
	virtual void TurnOnToneMapping() = 0;

	// Set a linear vector color scale for all 3D rendering.
	// A value of [1.0f, 1.0f, 1.0f] should match non-tone-mapped rendering.
	virtual void SetToneMappingScaleLinear(const vec3_t &scale) = 0;

	virtual vec3_t GetToneMappingScaleLinear(void) = 0;
	virtual void SetShadowDepthBiasFactors(float fSlopeScaleDepthBias, float fDepthBias) = 0;

	// Apply stencil operations to every pixel on the screen without disturbing depth or color buffers
	virtual void PerformFullScreenStencilOperation(void) = 0;

	// Sets lighting origin for the current model (needed to convert directional lights to points)
	virtual void SetLightingOrigin(vec3_t vLightingOrigin) = 0;

	// Set scissor rect for rendering
	virtual void SetScissorRect(const int nLeft, const int nTop, const int nRight, const int nBottom,
		const bool bEnableScissor) = 0;

	// Methods used to build the morph accumulator that is read from when HW morph<ing is enabled.
	virtual void BeginMorphAccumulation() = 0;
	virtual void EndMorphAccumulation() = 0;
	virtual void AccumulateMorph(IMorph *pMorph, int nMorphCount, const MorphWeight_t *pWeights) = 0;

	virtual void PushDeformation(DeformationBase_t const *Deformation) = 0;
	virtual void PopDeformation() = 0;
	virtual int GetNumActiveDeformations() const = 0;

	virtual bool GetMorphAccumulatorTexCoord(vec3_t *pTexCoord, IMorph *pMorph, int nVertex) = 0;

	// Version of get dynamic mesh that specifies a specific vertex format
	virtual IMesh *GetDynamicMeshEx(VertexFormat_t vertexFormat, bool bBuffered = true,
		IMesh *pVertexOverride = 0, IMesh *pIndexOverride = 0, IMaterial *pAutoBind = 0) = 0;

	virtual void FogMaxDensity(float flMaxDensity) = 0;

	virtual IMaterial *GetCurrentMaterial() = 0;
	virtual int GetCurrentNumBones() const = 0;
	virtual void *GetCurrentProxy() = 0;

	// Color correction related methods..
	// Client cannot call IColorCorrectionSystem directly because it is not thread-safe
	// FIXME: Make IColorCorrectionSystem threadsafe?
	virtual void EnableColorCorrection(bool bEnable) = 0;
	virtual ColorCorrectionHandle_t AddLookup(const char *pName) = 0;
	virtual bool RemoveLookup(ColorCorrectionHandle_t handle) = 0;
	virtual void LockLookup(ColorCorrectionHandle_t handle) = 0;
	virtual void LoadLookup(ColorCorrectionHandle_t handle, const char *pLookupName) = 0;
	virtual void UnlockLookup(ColorCorrectionHandle_t handle) = 0;
	virtual void SetLookupWeight(ColorCorrectionHandle_t handle, float flWeight) = 0;
	virtual void ResetLookupWeights() = 0;
	virtual void SetResetable(ColorCorrectionHandle_t handle, bool bResetable) = 0;

	//There are some cases where it's simply not reasonable to update the full screen depth texture (mostly on PC).
	//Use this to mark it as invalid and use a dummy texture for depth reads.
	virtual void SetFullScreenDepthTextureValidityFlag(bool bIsValid) = 0;

	// A special path used to tick the front buffer while loading on the 360
	virtual void SetNonInteractivePacifierTexture(ITexture *pTexture, float flNormalizedX, float flNormalizedY,
		float flNormalizedSize) = 0;
	virtual void SetNonInteractiveTempFullscreenBuffer(ITexture *pTexture, MaterialNonInteractiveMode_t mode) = 0;
	virtual void EnableNonInteractiveMode(MaterialNonInteractiveMode_t mode) = 0;
	virtual void RefreshFrontBufferNonInteractive() = 0;
	// Allocates temp render data. Renderdata goes out of scope at frame end in multicore
	// Renderdata goes out of scope after refcount goes to zero in singlecore.
	// Locking/unlocking increases + decreases refcount
	virtual void *LockRenderData(int nSizeInBytes) = 0;
	virtual void UnlockRenderData(void *pData) = 0;

	// Typed version. If specified, pSrcData is copied into the locked memory.
	template< class E >
	E *LockRenderDataTyped(int nCount, const E *pSrcData = NULL);

	// Temp render data gets immediately freed after it's all unlocked in single core.
	// This prevents it from being freed
	virtual void AddRefRenderData() = 0;
	virtual void ReleaseRenderData() = 0;

	// Returns whether a pointer is render data. NOTE: passing NULL returns true
	virtual bool IsRenderData(const void *pData) const = 0;
	virtual void PrintfVA(char *fmt, va_list vargs) = 0;
	virtual void Printf(const char *fmt, ...) = 0;
	virtual float Knob(char *knobname, float *setvalue = NULL) = 0;
	// Allows us to override the alpha write setting of a material
	virtual void OverrideAlphaWriteEnable(bool bEnable, bool bAlphaWriteEnable) = 0;
	virtual void OverrideColorWriteEnable(bool bOverrideEnable, bool bColorWriteEnable) = 0;

	virtual void ClearBuffersObeyStencilEx(bool bClearColor, bool bClearAlpha, bool bClearDepth) = 0;

	void SetStencilState(ShaderStencilState_t stencil) {
		SetStencilEnable(stencil.m_bEnable);
		SetStencilFailOperation(stencil.m_FailOp);
		SetStencilZFailOperation(stencil.m_ZFailOp);
		SetStencilPassOperation(stencil.m_PassOp);
		SetStencilCompareFunction(stencil.m_CompareFunc);
		SetStencilReferenceValue(stencil.m_nReferenceValue);
		SetStencilTestMask(stencil.m_nTestMask);
		SetStencilWriteMask(stencil.m_nWriteMask);
	}
};