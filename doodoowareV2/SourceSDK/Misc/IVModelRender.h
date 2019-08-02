#pragma once

#include "..\Math\Vector.h"
#include "..\sdk.h"
#include "Studio.h"
#include "..\Misc\IClientRenderable.h"

enum PreviewImageRetVal_t
{
	MATERIAL_PREVIEW_IMAGE_BAD = 0,
	MATERIAL_PREVIEW_IMAGE_OK,
	MATERIAL_NO_PREVIEW_IMAGE,
};



typedef unsigned short ModelInstanceHandle_t;
typedef void* LightCacheHandle_t;
typedef void* StudioDecalHandle_t;
typedef int OverrideType_t;
typedef int ImageFormat;
typedef int VertexFormat_t;
typedef int MaterialPropertyTypes_t;

//class IClientRenderable;
class IMaterial;
class CStudioHdr;

class DataCacheHandle_t;
class ITexture;
class IMaterialVar;
class KeyValues
{
public:
	void *operator new(size_t iAllocSize)
	{
		static PVOID pKeyValuesSystem;
		if (!pKeyValuesSystem)
		{
			typedef PVOID(__cdecl* oKeyValuesSystem)();
			oKeyValuesSystem KeyValuesSystem = (oKeyValuesSystem)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem");

			pKeyValuesSystem = KeyValuesSystem();
		}

		typedef PVOID(__thiscall* oAllocKeyValuesMemory)(PVOID, size_t);
		return gInterface.CallVirtualFunction<oAllocKeyValuesMemory>(pKeyValuesSystem, 1, pKeyValuesSystem, iAllocSize);
	}

	void operator delete(void * pMem)
	{
		static PVOID pKeyValuesSystem;
		if (!pKeyValuesSystem)
		{
			typedef PVOID(__cdecl* oKeyValuesSystem)();
			oKeyValuesSystem KeyValuesSystem = (oKeyValuesSystem)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "KeyValuesSystem");

			pKeyValuesSystem = KeyValuesSystem();
		}

		typedef void(__thiscall* oFreeKeyValuesMemory)(PVOID, PVOID);
		gInterface.CallVirtualFunction<oFreeKeyValuesMemory>(pKeyValuesSystem, 2, pKeyValuesSystem, pMem);
	}
	KeyValues() {}
	char pad[0x20];
};
struct mstudioanimdesc_t;
struct mstudioseqdesc_t;
struct Ray_t;
struct DrawModelInfo_t;
struct studiohwdata_t;
struct MaterialLightingState_t;
struct ColorMeshInfo_t;

struct model_t
{
	char name[255];
};

struct DrawModelState_t
{
	studiohdr_t*            m_pStudioHdr;
	studiohwdata_t*         m_pStudioHWData;
	IClientRenderable*      m_pRenderable;
	const matrix3x4_t*      m_pModelToWorld;
	StudioDecalHandle_t     m_decals;
	int                     m_drawFlags;
	int                     m_lod;
};

enum DrawModelFlags_t
{
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	// Not a studio flag, but used to flag model as a non-sorting brush model
	STUDIO_TRANSPARENCY = 0x80000000,
	// Not a studio flag, but used to flag model as using shadow depth material override
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SKIP_DECALS = 0x10000000,
};

struct StaticPropRenderInfo_t
{
	const matrix3x4_t*      pModelToWorld;
	const model_t*          pModel;
	IClientRenderable*      pRenderable;
	Vector*                 pLightingOrigin;
	short                   skin;
	ModelInstanceHandle_t   instance;
};

struct ModelRenderInfo_t
{
	Vector origin;
	QAngle angles;
	char _padding[0x4];
	void *pRenderable;
	const model_t *pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
	ModelRenderInfo_t()
	{
		pModelToWorld = nullptr;
		pLightingOffset = nullptr;
		pLightingOrigin = nullptr;
	}
};

struct LightingQuery_t
{
	Vector                  m_LightingOrigin;
	ModelInstanceHandle_t   m_InstanceHandle;
	bool                    m_bAmbientBoost;
};

struct StaticLightingQuery_t : public LightingQuery_t
{
	IClientRenderable*        m_pRenderable;
};



class IVModelRender {
public:
	virtual int DrawModel(int flags, IClientRenderable *pRenderable, ModelInstanceHandle_t instance, int entity_index,
		const model_t *model, vec3_t const &origin, vec3_t const &angles, int skin, int body,
		int hitboxset, const matrix3x4_t *modelToWorld = NULL,
		const matrix3x4_t *pLightingOffset = NULL) = 0;
	virtual void ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = 0,
		int nOverrides = 0) = 0;
	virtual bool IsForcedMaterialOverride(void) = 0;
	virtual void SetViewTarget(const int *pStudioHdr, int nBodyIndex, const vec3_t &target) = 0;
	virtual ModelInstanceHandle_t CreateInstance(IClientRenderable *pRenderable, LightCacheHandle_t *pCache = NULL) = 0;
	virtual void DestroyInstance(ModelInstanceHandle_t handle) = 0;
};
