// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Link.generated.h"

UENUM(BlueprintType)
enum class EStaminaState : uint8
{
	DEFAULT,
	USING,
	DEPLETION
};

DECLARE_MULTICAST_DELEGATE(FOnStaminaStateChanged);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedged);

UCLASS()
class THELEGENDOFZELDA_API ALink : public ACharacterBase
{
	GENERATED_BODY()

public:
	
	FOnStaminaStateChanged StaminaStateChanged;
	FOnHPChangedged OnHPChanged;
private:
	float Stamina;
	float MaxStamina;
	int32 HP;
	int32 MaxHP;
	
	bool bRunning;
	bool bStaminaRegen;
	FTimerHandle WaitStaminaRegenTimer = {};
	EStaminaState StaminaState;
	
	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY()
	UCameraComponent* Camera;
	
public:
	ALink();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void LookUp(float Axis);
	void Turn(float Axis);
	void Run();
	void RunStop();

	
	float GetStamina() const;
	float GetMaxStamina() const;
	int32 GetHP() const;
	int32 GetMaxHP() const;
	EStaminaState GetStaminaState() const;
	void SetStaminaState(EStaminaState NewState);
	void DoRegen();
	void StopRegen();
	void SetMaxHP(int32 newMaxHP);
	void SetHP(int newHP);
	void Debug1();
	void Debug2();
};
