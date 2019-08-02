#include "netvars.h"
#include "..\sdk.h"
//#include "../../Hooks/Hooks.h"
NetVarManager::NetVarManager()
{
	for (auto pClass = gInterface.client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
		if (pClass->m_pRecvTable)
			DumpRecursive(pClass->m_pRecvTable);
}

void NetVarManager::DumpRecursive(RecvTable* pTable)
{
	for (auto i = 0; i < pTable->m_nProps; ++i)
	{
		auto pProperty = &pTable->m_pProps[i];

		//Skip trash array items
		if (!pProperty || isdigit(pProperty->m_pVarName[0]))
			continue;

		//We dont care about the base class, we already know that
		if (strcmp(pProperty->m_pVarName, "baseclass") == 0)
			continue;

		if (pProperty->m_RecvType == DPT_DataTable &&
			pProperty->m_pDataTable != nullptr &&
			pProperty->m_pDataTable->m_pNetTableName[0] == 'D') // Skip shitty tables
		{
			DumpRecursive(pProperty->m_pDataTable);
		}

		std::string strHash = pTable->m_pNetTableName;
		strHash += "->";
		strHash += pProperty->m_pVarName;

		m_mapOffsets[FnvHash(strHash.c_str())] = pProperty;
	}
}