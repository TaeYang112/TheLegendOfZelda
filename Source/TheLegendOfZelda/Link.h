// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Link.generated.h"

/**
 * 
 */
UCLASS()
class THELEGENDOFZELDA_API ALink : public ACharacterBase
{
	GENERATED_BODY()

public:
	float Stamina;
	float MaxStamina;
	bool bStaminaDepletion;
	bool bRunning;
	bool bUseStamina;
	bool bStaminaRegen;
	FTimerHandle WaitStaminaRegenTimer = {};

private:
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
	bool IsStaminaDepletion() const;
	void SetUseStamina(bool bUse);
	bool IsUseStamina() const;
	void SetStaminaDepletion(bool bDepletion);
};
