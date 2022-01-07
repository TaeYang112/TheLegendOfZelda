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
	bStaminaDepletion = false;
	bRunning = false;
	bStaminaRegen = false;
	bUseStamina = false;
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
			if(bStaminaDepletion) SetStaminaDepletion(false);
		}
	}
	else if(bUseStamina)
	{
		Stamina -= 35 * DeltaSeconds;
		if(Stamina < 0)
		{
			SetStaminaDepletion(true);
			bUseStamina = false;
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
	if(bStaminaDepletion) return;
	
	bRunning = true;
	SetUseStamina(true);
	GetCharacterMovement()->MaxWalkSpeed = 800.0f;
}

void ALink::RunStop()
{
	if(bStaminaDepletion) return;
	
	bRunning = false;
	SetUseStamina(false);
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

bool ALink::IsStaminaDepletion() const
{
	return bStaminaDepletion;
}

void ALink::SetUseStamina(bool bUse)
{
	if(bUse)
	{
		bUseStamina = true;
		bStaminaRegen = false;
		GetWorld()->GetTimerManager().ClearTimer(WaitStaminaRegenTimer);
	}
	else
	{
		bUseStamina = false;
		GetWorld()->GetTimerManager().SetTimer(WaitStaminaRegenTimer, FTimerDelegate::CreateLambda([this]() -> void {
			bStaminaRegen = true;}), 
	0.5f, false);
	}
}

bool ALink::IsUseStamina() const
{
	return bUseStamina;
}

void ALink::SetStaminaDepletion(bool bDepletion)
{
	if(bDepletion)
	{
		bRunning = false;
		SetUseStamina(false);
		bStaminaDepletion = true;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	else
	{
		bStaminaDepletion = false;
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}


