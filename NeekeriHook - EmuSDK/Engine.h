#pragma once
#include "SDK.h"

class IVEngineClient
{
public:
	void GetScreenSize(int& Width, int& Height)
	{
		typedef void(__thiscall* _GetScreenSize)(void*, int&, int&);
		g_pUtils->Emulate< _GetScreenSize >(this, 5)(this, Width, Height);
	}

	void GetPlayerInfo(int Index, player_info_t* _Info)
	{
		typedef void(__thiscall* _GetPlayerInfo)(void*, int, player_info_t*);
		g_pUtils->Emulate<_GetPlayerInfo>(this, 8)(this, Index, _Info);
	}

	int GetLocalPlayer()
	{
		typedef int(__thiscall* _GetLocalPlayer)(void*);
		return g_pUtils->Emulate<_GetLocalPlayer>(this, 12)(this);
	}

	bool IsInGame()
	{
		typedef bool(__thiscall* _IsInGame)(void*);
		return g_pUtils->Emulate<_IsInGame>(this, 26)(this);
	}

	bool IsConnected()
	{
		typedef bool(__thiscall* _IsConnected)(void*);
		return g_pUtils->Emulate<_IsConnected>(this, 27)(this);
	}

	void SetViewAngles(Vector& vAngles)
	{
		typedef void(__thiscall* fnVA)(void*, Vector&);
		g_pUtils->Emulate<fnVA>(this, 19)(this, vAngles);
	}

	float GetLastTimeStamp(void)
	{
		typedef float(__thiscall* OriginalFn)(PVOID);
		return  g_pUtils->Emulate< OriginalFn >(this, 14)(this);
	}

	void GetViewAngles(QAngle& qViewAngles)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, QAngle&);
		g_pUtils->Emulate< OriginalFn >(this, 18)(this, qViewAngles);
	}

	void SetViewAngles(QAngle& qViewAngles)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, QAngle&);
		g_pUtils->Emulate< OriginalFn >(this, 19)(this, qViewAngles);
	}

	void ExecuteClientCmd(const char* pszCmd)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const char*);
		g_pUtils->Emulate< OriginalFn >(this, 108)(this, pszCmd);
	}
}; extern IVEngineClient* g_pEngine;

/* conveniently placed client.dll interface don't mind me */
class IClientEntityList
{
private:
	virtual void*				  GetClientNetworkable(int entnum) = 0;
	virtual void*                 vtablepad0x1(void) = 0;
	virtual void*                 vtablepad0x2(void) = 0;
public:
	virtual C_BaseEntity*         GetClientEntity(int entNum) = 0;
	virtual C_BaseEntity*         GetClientEntityFromHandle(HANDLE hEnt) = 0;
	virtual int                   NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int                   GetHighestEntityIndex(void) = 0;
private:
	virtual void                  SetMaxEntities(int maxEnts) = 0;
	virtual int                   GetMaxEntities() = 0;
}; extern IClientEntityList* g_pEntList;

class IVRenderView
{
public:
	void SetColorModulation(float const* blend)
	{
		typedef void(__thiscall *OrigFn)(void*, float const*);
		g_pUtils->Emulate<OrigFn>(this, 6)(this, blend);
	}

	void SetAlphaModulation(float blend)
	{
		typedef void(__thiscall* OrigFn)(void*, float);
		g_pUtils->Emulate<OrigFn>(this, 4)(this, blend);
	}

}; extern IVRenderView* g_pRenderView;