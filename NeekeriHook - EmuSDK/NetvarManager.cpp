#include "SDK.h"
#pragma warning(disable: 4101) //some shitty warning about "char hash_name[256]"

CNetVarManager::CNetVarManager()
{
	for (auto clazz = g_pClient->GetAllClasses(); clazz; clazz = clazz->m_pNext)
		if (clazz->m_pRecvTable)
			DumpRecursive(clazz->m_pRecvTable);
}
 
void CNetVarManager::DumpRecursive(RecvTable* table)
{
	for (auto i = 0; i < table->m_nProps; ++i)
	{
		auto prop_ptr = &table->m_pProps[i];
 
		//Skip trash array items
		if (!prop_ptr || isdigit(prop_ptr->m_pVarName[0]))
			continue;
 
		//We dont care about the base class, we already know that
		if (strcmp(prop_ptr->m_pVarName, "baseclass") == 0)
			continue;
 
		if (prop_ptr->m_RecvType == DPT_DataTable &&
			prop_ptr->m_pDataTable != nullptr  &&
			prop_ptr->m_pDataTable->m_pNetTableName[0] == 'D') // Skip shitty tables
		{
			DumpRecursive(prop_ptr->m_pDataTable);
		}
 
		char hash_name[256];
 
		std::string strHash = table->m_pNetTableName;
		strHash += "->";
		strHash += prop_ptr->m_pVarName;
		// Need to cast it to prevent FnvHash using the recursive hasher
		// which would hash all 256 bytes
		
		m_props[FnvHash(strHash.c_str())] = prop_ptr;
	}
}
