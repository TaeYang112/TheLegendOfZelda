// Fill out your copyright notice in the Description page of Project Settings.


#include "Link.h"

#include "GameFramework/CharacterMovementComponent.h"

ALink::ALink()
{
	// 컴포넌트 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	StaminaSystem = CreateDefaultSubobject<UStaminaSystem>(TEXT("StaminaSystem"));
	
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->AddLocalOffset(FVector(0.0f,0.0f,100.0f));
	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	// 스텟 초기화
	HP = 12;
	MaxHP = 12;

	// 그외 멤버 변수 초기화
	bRunning = false;
	ActionMode = EActionType::MAGNET;

	// 스테미나 상태 변경시 동작 수행
	StaminaSystem->StaminaStateChanged.AddLambda([this]
	{
		switch (StaminaSystem->GetStaminaState())
		{
		case EStaminaState::FULL : // 스테미나 꽉찬상태
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
			break;
		case EStaminaState::REGEN : // 스테미나 재생중 상태
			GetCharacterMovement()->MaxWalkSpeed = 500.0f;
			break;	
		case EStaminaState::USING : // 스테미나 사용 상태
			break;
		case EStaminaState::DEPLETION: // 스테미나 고갈 상태
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			break;
		}
	});
}

void ALink::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALink::BeginPlay()
{
	Super::BeginPlay();
}

#pragma region Tool

void ALink::ToolChange()
{
	switch(ActionMode)
	{
	case EActionType::NORMAL:
		break;
	case EActionType::SWORD:
		break;
	case EActionType::BOW:
		break;
	case EActionType::BOMB:
		break;
	case EActionType::MAGNET:
		
		break;
	default:
		break;
	}
}

void ALink::Magnet()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 500.0f,
		ECollisionChannel::ECC_GameTraceChannel2);
}

#pragma endregion Tool

#pragma region Input Process

void ALink::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ALink::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ALink::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&ALink::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ALink::Turn);
	
	
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this,&ALink::Jump);
	PlayerInputComponent->BindAction(TEXT("Run"),EInputEvent::IE_Pressed,this,&ALink::Run);
	PlayerInputComponent->BindAction(TEXT("Run"),EInputEvent::IE_Released,this,&ALink::RunStop);

	//PlayerInputComponent->BindAction(TEXT("RightClick"),IE_Pressed,this,&ALink::ToolChange);

	//디버그
	PlayerInputComponent->BindAction(TEXT("Debug1"),EInputEvent::IE_Pressed,this,&ALink::Debug1);
	PlayerInputComponent->BindAction(TEXT("Debug2"),EInputEvent::IE_Pressed,this,&ALink::Debug2);
}

void ALink::MoveForward(float Axis)
{
	AddMovementInput(Camera->GetForwardVector(),Axis);
}

void ALink::MoveRight(float Axis)
{
	AddMovementInput(Camera->GetRightVector(),Axis);
}

void ALink::LookUp(float Axis)
{
	AddControllerPitchInput(Axis);
}

void ALink::Turn(float Axis)
{
	AddControllerYawInput(Axis);
}

void ALink::Run()
{
	if(StaminaSystem->GetStaminaState() == EStaminaState::DEPLETION) return;
	
	bRunning = true;
	StaminaSystem->SetStaminaState(EStaminaState::USING);
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

void ALink::RunStop()
{
	if(StaminaSystem->GetStaminaState() == EStaminaState::DEPLETION) return;
	
	bRunning = false;
	StaminaSystem->SetStaminaState(EStaminaState::REGEN);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

#pragma  endregion Input Process

#pragma region Getter Setter



int32 ALink::GetHP() const
{
	return HP;
}

int32 ALink::GetMaxHP() const
{
	return MaxHP;
}

void ALink::SetMaxHP(int32 newMaxHP)
{
	if(newMaxHP > 0) MaxHP = newMaxHP;
	if(HP > MaxHP) HP = MaxHP;

	if(OnHPChanged.IsBound()) OnHPChanged.Broadcast();
}


void ALink::SetHP(int newHP)
{
	HP = FMath::Clamp(newHP,0,GetMaxHP());
	if(OnHPChanged.IsBound()) OnHPChanged.Broadcast();
}

UStaminaSystem* ALink::GetStaminaSystem()
{
	return StaminaSystem;
}


#pragma endregion  Getter Setter

void ALink::Debug1()
{
	SetHP(GetHP() + 1);
}

void ALink::Debug2()
{
	SetHP(GetHP() - 1);
}
