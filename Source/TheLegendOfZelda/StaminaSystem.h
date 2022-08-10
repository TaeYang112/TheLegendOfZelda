// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StaminaSystem.generated.h"

UENUM(BlueprintType)
enum class EStaminaState : uint8
{
	FULL,
	REGEN,
	USING,
	DEPLETION
};

DECLARE_MULTICAST_DELEGATE(FOnStaminaStateChanged);

/**
 * 
 */
UCLASS()
class THELEGENDOFZELDA_API UStaminaSystem : public UActorComponent
{
	GENERATED_BODY()


private:
	// 스테마나 변수
	EStaminaState StaminaState;											// 스테미나 미사용중 : NORMAL / 사용중 : USING / 고갈상태 : DEPLETION
	float Stamina;														// 현재 스테미나	
	float MaxStamina;													// 최대 스테미나
	
	// 타이머 핸들
	FTimerHandle WaitStaminaRegenTimer;									// 스테미나 사용이 끝나고 스테미나 재생까지 텀을 주는 타이머
	FTimerHandle StaminaRegenLoopTimer;									// Tick대신 사용하는 스테미나 회복 타이머
	FTimerHandle StaminaUsingLoopTimer;									// Tick대신 사용하는 스테미나 소모 타이머

public:
	UStaminaSystem();
	virtual void BeginPlay() override;
	
	// 델리게이트
	FOnStaminaStateChanged StaminaStateChanged;							// 스테미나 상태가 변경됨		
	
	// 스태미나 관련 함수
	void SetStaminaState(EStaminaState NewState);						// 현재 스테미나 상태를 설정 (NORMAL, USING, DEPLETION)
	EStaminaState GetStaminaState() const;
	
	void DoStaminaRegen();												// 호출 후 일정 시간 뒤에 StaminaRegenLoop 반복 호출
	void StaminaRegenLoop();											// 스테미나를 회복시킴
	void StopStaminaRegen();											// 스테미나 재생 종료
	
	void DoStaminaUsing();												// StaminaUsingLoop를 반복 호출
	void StaminaUsingLoop();											// 스테미나를 소모시킴
	void StopStaminaUsing();											// 스테미나 소모 종료
	
	float GetStamina() const;
	float GetMaxStamina() const;
	

	
};
