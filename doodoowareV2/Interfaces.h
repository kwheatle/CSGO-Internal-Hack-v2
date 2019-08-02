#pragma once
#include "SourceSDK\sdk.h"
#include <iostream>
#include <string>
#include <sstream>
#include <Psapi.h>
#include <iomanip>

#include <ctime>
#include <chrono>

class Interface
{

	friend class IBaseClientDLL;
	friend class ClientModeShared;
	friend class IVEngineClient;
	friend class IClientEntityList;
	friend class ISurface;
	friend class CGlobalVarsBase;
	friend class IVDebugOverlay;
	friend class IPanel;
	friend class IVModelInfo;
	friend class IEngineTrace;
	friend class ICVar;
	friend class IMaterialSystem;
	friend class IVModelRender;
	friend class CInput;

	friend class CPrediction;
	friend class IGameMovement;
	friend class IMoveHelper;
	friend class IMdlCache;
	friend class ClientState;
	friend class IPhysicsProps;

public:

	IBaseClientDLL* client;
	ClientModeShared* clientMode;
	IVEngineClient* engine = nullptr;
	IClientEntityList* entityList;
	ISurface* surface;
	CGlobalVarsBase* globals;
	IVDebugOverlay* debug;
	IVModelRender* mdlRender;
	IPanel* panel;
	IVModelInfo* modelInfo;
	IEngineTrace* engineTrace;
	ICVar* cvar;
	IMaterialSystem* matsystem;
	IMdlCache* mdlCache;
	ClientState* clientState;

	CInput* input;

	CPrediction* prediction;
	IMoveHelper* moveHelper;
	IGameMovement* movement;

	IPhysicsProps* physicsProps;

	//QAngle fakeang;

	QAngle fakeang = QAngle(0,0,0);
	QAngle realang = QAngle(0, 0, 0);
	CUserCmd* oCmd;
	matrix3x4_t localMatrix[128];
	byte bSendPacket;
	CBaseEntity* local = nullptr;
	int lastWeapon;

	template<typename T>
	T* FindInterface(const std::string _moduleName, const std::string &_interfaceName)
	{
		typedef void* (*CreateInterfaceFn)(const char*, int*);
		CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandle(_moduleName.c_str()), "CreateInterface");

		void* interfacePtr = nullptr;

		for (int i = 1; i < 100; ++i)
		{
			std::ostringstream interfaceNameStream;
			interfaceNameStream << _interfaceName << std::setw(3) << std::setfill('0') << i;

			interfacePtr = (void*)CreateInterface(interfaceNameStream.str().c_str(), NULL);

			if (interfacePtr)
			{
				std::cout << interfaceNameStream.str() << std::endl;
				break;
			}
		}

		return (T*)interfacePtr;
	}

	typedef void* (*CreateInterfaceFn)(const char *pName, int *pReturnCode);

	CreateInterfaceFn get_module_factory(HMODULE module)
	{
		return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
	}

	template<typename T>
	T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
	{
		auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

		if (!result) {
			throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
		}

		return result;
	}

	#define INRANGE(x,a,b)    (x >= a && x <= b) 
	#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
	#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	DWORD FindPattern(std::string moduleName, std::string pattern)
	{
		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
		MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;    //one ?
			}
			else
			{
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}
public:
	template<typename functionType, typename ...typesOfFunctionParameters, class C>
	functionType __forceinline CallVirtualFunction(C* _ECX, int _functionIndex, typesOfFunctionParameters... _functionParameters)
	{
		typedef functionType(__thiscall* functionPrototype)(void*, typesOfFunctionParameters...);
		functionPrototype originalFunction = (functionPrototype)this->GetVirtualFunction(_ECX, _functionIndex);
		return originalFunction(_ECX, _functionParameters...);
	}


	template <std::size_t Index, typename ReturnType, typename... Args>
	__forceinline ReturnType call_virtual(void* instance, Args... args)
	{
		using Fn = ReturnType(__thiscall*)(void*, Args...);

		auto function = (*reinterpret_cast<Fn**>(instance))[Index];
		return function(instance, args...);
	}

	template< typename Function > Function CallVFunction(PVOID Base, DWORD Index)
	{
		PDWORD* VTablePointer = (PDWORD*)Base;
		PDWORD VTableFunctionBase = *VTablePointer;
		DWORD dwAddress = VTableFunctionBase[Index];
		return (Function)(dwAddress);
	}

private:
	__forceinline void* GetVirtualFunction(void* _base, int _index)
	{
		void** VTable = *(void***)_base;
		return VTable[_index];
	}
};
extern Interface gInterface;