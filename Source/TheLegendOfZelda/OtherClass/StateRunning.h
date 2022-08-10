#pragma once
#include "ZeldaState.h"

class StateRunning : ZeldaState
{
public:
	virtual void Run() override;
	virtual void Stop() override;
};
