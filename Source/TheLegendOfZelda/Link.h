// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "StaminaSystem.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Link.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	NORMAL,
	SWORD,
	BOW,
	BOMB,
	MAGNET
};

DECLARE_MULTICAST_DELEGATE(FOnHPChangedged);

UCLASS()
class THELEGENDOFZELDA_API ALink : public ACharacterBase
{
	GENERATED_BODY()

private:
	// 캐릭터 스텟
	int32 HP;															// 현재 체력
	int32 MaxHP;														// 최대 체력

	
	// 캐릭터 상태관련 변수
	bool bRunning;														// 달리는중 TRUE 아니면 FALSE
	EActionType ActionMode;												// 들고있는 도구를 나타냄 ( NORMAL, SWORD, BOMB 등 )

	
	// 컴포넌트
	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	UStaminaSystem* StaminaSystem;
	
public:
	// 기본 함수
	ALink();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	// 도구 관련 함수
	void ToolChange();													// 도구 변경키 ( 무기, 자석, 폭탄 등)
	void Magnet();														// 자석(임시)
	
	// 조작 관련 함수
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;		// 사용자의 입력을 받아옴
	void MoveForward(float Axis);										// 앞, 뒤 이동
	void MoveRight(float Axis);											// 좌, 우 이동
	void LookUp(float Axis);											// 위, 아래 카메라 회전
	void Turn(float Axis);												// 좌, 우 카메라 회전
	void Run();															// 달리기
	void RunStop();														// 달리기 해제

	
	// Getter, Setter
	int32 GetHP() const;
	void SetHP(int newHP);
	int32 GetMaxHP() const;
	void SetMaxHP(int32 newMaxHP);
	UStaminaSystem* GetStaminaSystem();

	// 델리게이트
	FOnHPChangedged OnHPChanged;										// 체력이 변경됨
	
	// 디버그
	void Debug1();
	void Debug2();
};
