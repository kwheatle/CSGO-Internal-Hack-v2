#pragma once

class CBaseEntity;

class IClientEntityList
{
public:
	inline CBaseEntity* GetClientEntity(int _entityIndex);
	inline CBaseEntity* GetClientEntityFromHandle(DWORD _handle);
};

inline CBaseEntity* IClientEntityList::GetClientEntity(int _entityIndex)
{
	return gInterface.CallVirtualFunction<CBaseEntity*>(this, Indexes::ClientEntityList::GetClientEntity, _entityIndex);
}

inline CBaseEntity* IClientEntityList::GetClientEntityFromHandle(DWORD _handle)
{
	return gInterface.CallVirtualFunction<CBaseEntity*>(this, Indexes::ClientEntityList::GetClientEntityFromHandle, _handle);
}