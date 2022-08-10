#pragma once

class ZeldaState
{
public:
	ZeldaState(ACharacter* Owner);

	virtual void Run() = 0;
	virtual void Stop() = 0;
private:
	UPROPERTY()
	ACharacter* Owner;
};
