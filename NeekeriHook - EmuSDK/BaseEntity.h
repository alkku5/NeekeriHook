#pragma once
#include "SDK.h"
#include "Collideable.h"
#include "PlayerInfo.h"
 
#define	FL_ONGROUND				(1<<0)// At rest / on the ground
#define FL_DUCKING				(1<<1)// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<2)// player jumping out of water
#define FL_ONTRAIN				(1<<3)// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<4)// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<5)// Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<6)// Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<7)// Is a player
#define FL_FAKECLIENT			(1<<8)// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<9)// In water
 
enum LifeState : unsigned char
{
	LIFE_ALIVE = 0,// alive
	LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
	LIFE_DEAD = 2, // dead. lying still.
	MAX_LIFESTATE
};
 
enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
	MAX_MOVETYPE
};
 
#define NETVAR1(funcname, type, netvarname) type& funcname() \
{ \
	constexpr fnv_t hash = FnvHash(netvarname); \
	static uint16_t offset = 0; \
	if(!offset) offset = CNetVarManager::Get().GetOffset(hash); \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

 
class C_BaseWeapon;
struct matrix3x4_t;
struct model_t;
class C_BaseEntity
{
public:
	/* functions */
	bool IsAlive()
	{
		FUNCTION_GUARD;
		if (LifeState() == LIFE_ALIVE)
			return true;
		//else
			//return false;
	}
	bool IsPlayer()
	{
		FUNCTION_GUARD;
		return g_pUtils->Emulate<bool(__thiscall*)(C_BaseEntity*)>(this, 152)(this);
	}
	
	bool IsWeapon()
	{
		FUNCTION_GUARD;
		return g_pUtils->Emulate<bool(__thiscall*)(C_BaseEntity*)>(this, 160)(this);
	}
 
	ICollideable* GetCollideable()
	{
		FUNCTION_GUARD;
		typedef ICollideable*(__thiscall *OrigFn)(void*);
		return g_pUtils->Emulate<OrigFn>(this, 3)(this);
	}
 
	const model_t* GetModel()
	{
		FUNCTION_GUARD;
		void *pRenderable = (void*)(this + 0x4);
		typedef const model_t*(__thiscall *OrigFn)(void*);
		return g_pUtils->Emulate<OrigFn>(pRenderable, 8)(pRenderable);
	}
 
	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		FUNCTION_GUARD;
		void *pRenderable = (void*)(this + 0x4);
		typedef bool(__thiscall *OrigFn)(void*, matrix3x4_t*, int, int, float);
		return g_pUtils->Emulate<OrigFn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
 
	ClientClass* GetClientClass() 
	{
		FUNCTION_GUARD;
		void *pNetworked = (void*)(this + 0x8);
		typedef ClientClass*(__thiscall *OrigFn)(void*);
		return g_pUtils->Emulate<OrigFn>(pNetworked, 2)(pNetworked);
	}
 
	int GetIndex()
	{
		return *reinterpret_cast<int*>((uintptr_t)this + 0x64);
	}
	/* netvars */ 
	//using "NETVAR1" instead of NETVAR name because it would throw redefinition warning
 
 
	NETVAR1(m_fFlags, int, "DT_BasePlayer->m_fFlags") // working flags netvar

};
