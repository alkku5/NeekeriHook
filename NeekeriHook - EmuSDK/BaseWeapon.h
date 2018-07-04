#pragma once
#include "SDK.h"

#define NETVAR_PTR(funcname, type, netvarname) type* funcname() \
{ \
	constexpr fnv_t hash = FnvHash(netvarname); \
	static uint16_t offset = 0; \
	if(!offset) offset = CNetVarManager::GetOffsetByHash(hash); \
	return reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define NETVAR(funcname, type, netvarname) type& funcname() \
{ \
	constexpr fnv_t hash = FnvHash(netvarname); \
	static uint16_t offset = 0; \
	if(!offset) offset = CNetVarManager::GetOffsetByHash(hash); \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

class C_BaseEntity;
class C_BaseWeapon
{

};