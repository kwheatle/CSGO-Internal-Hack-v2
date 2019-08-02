#pragma once


#include "../Interfaces/Recv.h"
#include <vector>
#include <map>


#pragma warning( disable : 4307 )
class FnvHash
{
	static const unsigned int FNV_PRIME = 16777619u;
	static const unsigned int OFFSET_BASIS = 2166136261u;

	template <unsigned int N>
	static constexpr unsigned int fnvHashConst(const char(&str)[N], unsigned int I = N)
	{
		return I == 1 ? (OFFSET_BASIS ^ str[0]) * FNV_PRIME : (fnvHashConst(str, I - 1) ^ str[I - 1]) * FNV_PRIME;
	}

	static unsigned int fnvHash(const char* str)
	{
		const size_t length = strlen(str) + 1;
		unsigned int hash = OFFSET_BASIS;
		for (size_t i = 0; i < length; ++i)
		{
			hash ^= *str++;
			hash *= FNV_PRIME;
		}
		return hash;
	}

	struct Wrapper
	{
		Wrapper(const char* str) : str(str) { }
		const char* str;
	};

	unsigned int hash_value;
public:
	FnvHash(Wrapper wrapper) : hash_value(fnvHash(wrapper.str)) { }

	template <unsigned int N>
	constexpr FnvHash(const char(&str)[N]) : hash_value(fnvHashConst(str)) { }

	constexpr operator unsigned int() const { return this->hash_value; }
};

class NetVarManager
{
public:
	static NetVarManager& Get()
	{
		static NetVarManager Instance;

		return Instance;
	}

	unsigned short GetOffset(unsigned int wHash)
	{
		return m_mapOffsets[wHash]->m_Offset;
	}

	RecvProp* GetPropPtr(unsigned int wHash)
	{
		return m_mapOffsets[wHash];
	}

private:
	NetVarManager();
	void DumpRecursive(RecvTable* pTable);

private:
	std::map<unsigned int, RecvProp*> m_mapOffsets;
};

#define PNETVAR(funcname, type, netvarname) type* funcname() \
{ \
	constexpr auto hash = FnvHash(netvarname); \
	static uintptr_t offset = NetVarManager::Get().GetOffset(hash); \
	return reinterpret_cast<type*>(uintptr_t(this) + offset); \
}

#define NETVAR(funcname, type, netvarname) type& funcname() \
{ \
	constexpr auto hash = FnvHash(netvarname); \
	static uintptr_t offset = NetVarManager::Get().GetOffset(hash); \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset); \
}