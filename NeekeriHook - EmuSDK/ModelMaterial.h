#pragma once
#include "SDK.h"

class IVModelRender
{
public:
	virtual int                     DrawModel(int flags, void *pRenderable, ModelInstanceHandle_t instance, int entity_index, const model_t *model, Vector const& origin, QAngle const& angles, int skin, int body, int hitboxset, const matrix3x4_t *modelToWorld = NULL, const matrix3x4_t *pLightingOffset = NULL) = 0;
	virtual void                    ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = 0, int nOverrides = 0) = 0;
	virtual bool                    IsForcedMaterialOverride(void) = 0;
	virtual void                    SetViewTarget(const void *pStudioHdr, int nBodyIndex, const Vector& target) = 0;
	virtual ModelInstanceHandle_t   CreateInstance(void *pRenderable, LightCacheHandle_t *pCache = NULL) = 0;
	virtual void                    DestroyInstance(ModelInstanceHandle_t handle) = 0;
	virtual void                    SetStaticLighting(ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle) = 0;
	virtual LightCacheHandle_t      GetStaticLighting(ModelInstanceHandle_t handle) = 0;
	virtual bool                    ChangeInstance(ModelInstanceHandle_t handle, void *pRenderable) = 0;
	virtual void                    AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal) = 0;
	virtual void                    RemoveAllDecals(ModelInstanceHandle_t handle) = 0;
	virtual bool                    ModelHasDecals(ModelInstanceHandle_t handle) = 0;
	virtual void                    RemoveAllDecalsFromAllModels() = 0;
	virtual matrix3x4_t*            DrawModelShadowSetup(void *pRenderable, int body, int skin, void *pInfo, matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
	virtual void                    DrawModelShadow(void *pRenderable, const float &info, matrix3x4_t *pCustomBoneToWorld = NULL) = 0;
	virtual bool                    RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;
	virtual void                    ReleaseAllStaticPropColorData(void) = 0;
	virtual void                    RestoreAllStaticPropColorData(void) = 0;
	virtual int                     DrawModelEx(ModelRenderInfo_t &pInfo) = 0;
	virtual int                     DrawModelExStaticProp(ModelRenderInfo_t &pInfo) = 0;
	virtual bool                    DrawModelSetup(ModelRenderInfo_t &pInfo, DrawModelState_t *pState, matrix3x4_t **ppBoneToWorldOut) = 0;
	virtual void                    DrawModelExecute(void* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld = NULL) = 0;
	virtual void                    SetupLighting(const Vector &vecCenter) = 0;
	virtual int                     DrawStaticPropArrayFast(StaticPropRenderInfo_t *pProps, int count, bool bShadowDepth) = 0;
	virtual void                    SuppressEngineLighting(bool bSuppress) = 0;
	virtual void                    SetupColorMeshes(int nTotalVerts) = 0;
	virtual void                    SetupLightingEx(const Vector &vecCenter, ModelInstanceHandle_t handle) = 0;
	virtual bool                    GetBrightestShadowingLightSource(const Vector &vecCenter, Vector& lightPos, Vector& lightBrightness, bool bAllowNonTaggedLights) = 0;
	virtual void                    ComputeLightingState(int nCount, const LightingQuery_t *pQuery, void *pState, void **ppEnvCubemapTexture) = 0;
	virtual void                    GetModelDecalHandles(StudioDecalHandle_t *pDecals, int nDecalStride, int nCount, const ModelInstanceHandle_t *pHandles) = 0;
	virtual void                    ComputeStaticLightingState(int nCount, const StaticLightingQuery_t *pQuery, void *pState, void *pDecalState, void **ppStaticLighting, void **ppEnvCubemapTexture, void *pColorMeshHandles) = 0;
	virtual void                    CleanupStaticLightingState(int nCount, void *pColorMeshHandles) = 0;
}; extern IVModelRender* g_pModelRender;

class IVModelInfo
{
public:
	virtual                                 ~IVModelInfo(void) {}
	virtual const model_t*                  GetModel(int modelindex) const = 0;
	virtual int                             GetModelIndex(const char *name) const = 0;
	virtual const char*                     GetModelName(const model_t *model) const = 0;
	virtual vcollide_t*                     GetVCollide(const model_t *model) const = 0;
	virtual vcollide_t*                     GetVCollide(int modelindex) const = 0;
	virtual void                            GetModelBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual void                            GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual int                             GetModelFrameCount(const model_t *model) const = 0;
	virtual int                             GetModelType(const model_t *model) const = 0;
	virtual void*                           GetModelExtraData(const model_t *model) = 0;
	virtual bool                            ModelHasMaterialProxy(const model_t *model) const = 0;
	virtual bool                            IsTranslucent(model_t const* model) const = 0;
	virtual bool                            IsTranslucentTwoPass(const model_t *model) const = 0;
	virtual void                            Unused0() {};
	virtual RenderableTranslucencyType_t    ComputeTranslucencyType(const model_t *model, int nSkin, int nBody) = 0;
	virtual int                             GetModelMaterialCount(const model_t* model) const = 0;
	virtual void                            GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial) = 0;
	virtual bool                            IsModelVertexLit(const model_t *model) const = 0;
	virtual const char*                     GetModelKeyValueText(const model_t *model) = 0;
	virtual bool                            GetModelKeyValue(const model_t *model, float &buf) = 0;
	virtual float                           GetModelRadius(const model_t *model) = 0;
	virtual void*                     GetStudioHdr(MDLHandle_t handle) = 0;
	virtual const studiohdr_t*              FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t*              FindModel(void *cache) const = 0;
	virtual void*                 GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
	virtual uint8_t*                        GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;
	virtual void                            GetModelMaterialColorAndLighting(const model_t *model, Vector const& origin, QAngle const& angles, trace_t* pTrace, Vector& lighting, Vector& matColor) = 0;
	virtual void                            GetIlluminationPoint(const model_t *model, void *pRenderable, Vector const& origin, QAngle const& angles, Vector* pLightingCenter) = 0;
	virtual int                             GetModelContents(int modelIndex) const = 0;
	virtual studiohdr_t*                    GetStudiomodel(const model_t *mod) = 0;
	virtual int                             GetModelSpriteWidth(const model_t *model) const = 0;
	virtual int                             GetModelSpriteHeight(const model_t *model) const = 0;
	virtual void                            SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void                            GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;
	virtual void                            SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual unsigned char                   ComputeLevelScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char                   ComputeViewScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual int                             GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;
	virtual CPhysCollide*                   GetCollideForVirtualTerrain(int index) = 0;
	virtual bool                            IsUsingFBTexture(const model_t *model, int nSkin, int nBody, void* *pClientRenderable) const = 0;
	virtual const model_t*                  FindOrLoadModel(const char *name) const = 0;
	virtual MDLHandle_t                     GetCacheHandle(const model_t *model) const = 0;
	virtual int                             GetBrushModelPlaneCount(const model_t *model) const = 0;
	virtual void                            GetBrushModelPlane(const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin) const = 0;
	virtual int                             GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool                            UsesEnvCubemap(const model_t *model) const = 0;
	virtual bool                            UsesStaticLighting(const model_t *model) const = 0;
}; extern IVModelInfo* g_pModelInfo;

class IMaterialSystem
{
public:
	IMaterial* IMaterialSystem::CreateMaterial(bool flat, bool ignorez, bool wireframed) // credit: A5
	{
		std::string type = (flat) ? "UnlitGeneric" : "VertexLitGeneric";

		std::string matdata = "\"" + type + "\"\n{\n\t\"$basetexture\" \"vgui/white_additive\"\n\t\"$envmap\"  \"\"\n\t\"$model\" \"1\"\n\t\"$flat\" \"1\"\n\t\"$nocull\"  \"0\"\n\t\"$selfillum\" \"1\"\n\t\"$halflambert\" \"1\"\n\t\"$nofog\"  \"0\"\n\t\"$znearer\" \"0\"\n\t\"$wireframe\" \"" + std::to_string(wireframed) + "\"\n\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n}\n";

		static int created = 0;
		std::string matname = "custom_" + std::to_string(created);
		++created;

		KeyValues* pKeyValues = new KeyValues(matname.c_str());
		g_pUtils->InitKeyValues(pKeyValues, type.c_str());
		g_pUtils->LoadFromBuffer(pKeyValues, matname.c_str(), matdata.c_str());

		typedef IMaterial*(__thiscall* OriginalFn)(void*, const char *pMaterialName, KeyValues *pVMTKeyValues);
		IMaterial* createdMaterial = g_pUtils->Emulate<OriginalFn>(this, 83)(this, matname.c_str(), pKeyValues);

		createdMaterial->IncrementReferenceCount();

		return createdMaterial;
	}

	IMaterial* IMaterialSystem::FindMaterial(const char* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix)
	{
		typedef IMaterial*(__thiscall* OriginalFn)(void*, char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix);
		return g_pUtils->Emulate<OriginalFn>(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}

	MaterialHandle_t IMaterialSystem::FirstMaterial()
	{
		return g_pUtils->Emulate<MaterialHandle_t(__thiscall*)(void*)>(this, 86)(this);
	}

	MaterialHandle_t IMaterialSystem::NextMaterial(MaterialHandle_t h)
	{
		return g_pUtils->Emulate<MaterialHandle_t(__thiscall*)(void*, MaterialHandle_t)>(this, 87)(this, h);
	}

	MaterialHandle_t IMaterialSystem::InvalidMaterial()
	{
		return g_pUtils->Emulate<MaterialHandle_t(__thiscall*)(void*)>(this, 88)(this);
	}

	IMaterial* IMaterialSystem::GetMaterial(MaterialHandle_t h)
	{
		return g_pUtils->Emulate<IMaterial*(__thiscall*)(void*, MaterialHandle_t)>(this, 89)(this, h);
	}
}; extern IMaterialSystem* g_pMaterialSystem;