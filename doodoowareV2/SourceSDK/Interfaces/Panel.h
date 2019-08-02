#pragma once
#include "../../Interfaces.h"

class IPanel
{
public:
	inline const char* GetName(VPANEL _vguiPanel);
};

inline const char* IPanel::GetName(VPANEL _vguiPanel)
{
	return gInterface.CallVirtualFunction<const char*>(this, Indexes::Panel::GetName, _vguiPanel);
}