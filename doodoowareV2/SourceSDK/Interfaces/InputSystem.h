#pragma once

class IInputSystem
{
public:
	inline bool IsButtonDown(ButtonCode_t _code);
};

inline bool IInputSystem::IsButtonDown(ButtonCode_t _code)
{
	return gInterface.CallVirtualFunction<bool>(this, Indexes::InputSystem::IsButtonDown, _code);
}