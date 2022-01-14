// Fill out your copyright notice in the Description page of Project Settings.


#include "Link.h"

#include "GameFramework/CharacterMovementComponent.h"

ALink::ALink()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->AddLocalOffset(FVector(0.0f,0.0f,100.0f));
	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	Stamina = 100.0f;
	MaxStamina = 100.0f;
	HP = 12;
	MaxHP = 12;
	
	bRunning = false;
	bStaminaRegen = false;
	StaminaState = EStaminaState::DEFAULT;
}

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


	//디버그
	PlayerInputComponent->BindAction(TEXT("Debug1"),EInputEvent::IE_Pressed,this,&ALink::Debug1);
	PlayerInputComponent->BindAction(TEXT("Debug2"),EInputEvent::IE_Pressed,this,&ALink::Debug2);
}

void ALink::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bStaminaRegen == true)
	{
		Stamina += 70 * DeltaSeconds;
		if(Stamina>= MaxStamina)
		{
			Stamina = MaxStamina;
			bStaminaRegen = false;
			if(GetStaminaState() == EStaminaState::DEPLETION) SetStaminaState(EStaminaState::DEFAULT);
		}
	}
	else if(GetStaminaState() == EStaminaState::USING)
	{
		Stamina -= 35 * DeltaSeconds;
		if(Stamina <= 0)
		{
			Stamina = 0;
			SetStaminaState(EStaminaState::DEPLETION);
		}
	}
	
}

void ALink::MoveForward(float Axis)
{
	AddMovementInput(GetActorForwardVector(),Axis);
}

void ALink::MoveRight(float Axis)
{
	AddMovementInput(GetActorRightVector(),Axis);
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
	if(StaminaState == EStaminaState::DEPLETION) return;
	
	bRunning = true;
	SetStaminaState(EStaminaState::USING);
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

void ALink::RunStop()
{
	if(StaminaState == EStaminaState::DEPLETION) return;
	
	bRunning = false;
	SetStaminaState(EStaminaState::DEFAULT);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

float ALink::GetStamina() const
{
	return Stamina;
}
float ALink::GetMaxStamina() const
{
	return MaxStamina;
}

int32 ALink::GetHP() const
{
	return HP;
}

int32 ALink::GetMaxHP() const
{
	return MaxHP;
}

EStaminaState ALink::GetStaminaState() const
{
	return StaminaState;
}

void ALink::SetStaminaState(EStaminaState NewState)
{
	StaminaState = NewState;
	
	switch (StaminaState)
	{
	case EStaminaState::DEFAULT :
		DoRegen();
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		break;
	case EStaminaState::USING :
		StopRegen();
		break;
	case EStaminaState::DEPLETION:
		DoRegen();
		bRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		break;
	}

	StaminaStateChanged.Broadcast();
}

void ALink::DoRegen()
{
	GetWorld()->GetTimerManager().SetTimer(WaitStaminaRegenTimer,
			FTimerDelegate::CreateLambda([this]() -> void {bStaminaRegen = true;}), 0.5f, false);
}
void ALink::StopRegen()
{
	bStaminaRegen = false;
	GetWorld()->GetTimerManager().ClearTimer(WaitStaminaRegenTimer);
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


void ALink::Debug1()
{
	SetHP(GetHP() + 1);
}

void ALink::Debug2()
{
	SetHP(GetHP() - 1);
}