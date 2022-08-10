// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaSystem.h"


UStaminaSystem::UStaminaSystem()
{
	Stamina = 100.0f;
	MaxStamina = 100.0f;
	StaminaState = EStaminaState::FULL;

	//
}

void UStaminaSystem::BeginPlay()
{
	Super::BeginPlay();
	SetStaminaState(EStaminaState::FULL);
}

void UStaminaSystem::SetStaminaState(EStaminaState NewState)
{
	StaminaState = NewState;
	
	switch (StaminaState)
	{
	case EStaminaState::FULL : // 스테미나 꽉찬상태
		StopStaminaRegen();
		break;
	case EStaminaState::REGEN : // 스테미나 재생중 상태
		DoStaminaRegen();
		break;	
	case EStaminaState::USING : // 스테미나 사용 상태
		StopStaminaRegen();
		DoStaminaUsing();
		break;
	case EStaminaState::DEPLETION: // 스테미나 고갈 상태
		StopStaminaUsing();
		DoStaminaRegen();
		break;
	}

	if(StaminaStateChanged.IsBound()) StaminaStateChanged.Broadcast();
}

// 0.5초 뒤에 0.02초 간격으로 StaminaRegenLoop 호출
void UStaminaSystem::DoStaminaRegen()
{
	GetWorld()->GetTimerManager().SetTimer(WaitStaminaRegenTimer,
			FTimerDelegate::CreateLambda([this]
				{
					GetWorld()->GetTimerManager().SetTimer(StaminaRegenLoopTimer,
						FTimerDelegate::CreateUObject(this,&UStaminaSystem::StaminaRegenLoop),0.02,true);
				}),0.5f, false);
}

// 스테미나를 회복시키는 함수
void UStaminaSystem::StaminaRegenLoop()
{
	Stamina += 1.4f;
	if(Stamina>= MaxStamina)
	{
		Stamina = MaxStamina;
		SetStaminaState(EStaminaState::FULL);
	}
}

// 스테미나 회복 중지
void UStaminaSystem::StopStaminaRegen()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitStaminaRegenTimer);
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenLoopTimer);
}

// StaminaUsingLoop를 반복 호출하여 스테미나 소모 시킴
void UStaminaSystem::DoStaminaUsing()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenLoopTimer,
		FTimerDelegate::CreateUObject(this,&UStaminaSystem::StaminaUsingLoop),0.02f,true);
}

// 스테미나를 소모시키는 함수
void UStaminaSystem::StaminaUsingLoop()
{
	if(GetStaminaState() == EStaminaState::USING)
	{
		Stamina -= 35 * 0.02f;
		if(Stamina <= 0)
		{
			Stamina = 0;
			SetStaminaState(EStaminaState::DEPLETION);
		}
	}
}

// 스테미나 소모 중지
void UStaminaSystem::StopStaminaUsing()
{
	GetWorld()->GetTimerManager().ClearTimer(StaminaUsingLoopTimer);
}

float UStaminaSystem::GetStamina() const
{
	return Stamina;
}
float UStaminaSystem::GetMaxStamina() const
{
	return MaxStamina;
}

EStaminaState UStaminaSystem::GetStaminaState() const
{
	return StaminaState;
}