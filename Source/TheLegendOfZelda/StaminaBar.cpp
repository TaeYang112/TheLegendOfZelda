// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaBar.h"
#include "Components/Image.h"


void UStaminaBar::NativePreConstruct()
{
	Super::NativePreConstruct();
	Color = FLinearColor::Green;
	Color2 = FLinearColor::Green;
	DepletionColor = FLinearColor::Red;
	bDepletionColor_incr = true;
	Link = nullptr;

	M_Dynamic = Progress_Image->GetDynamicMaterial();
	
}

void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();


	Link = Cast<ALink>(GetOwningPlayerPawn());
	if(Link != nullptr)
	{
		Link->StaminaStateChanged.AddLambda([this]()->void
		{
			StaminaState = Link->GetStaminaState();
			
			if(StaminaState == EStaminaState::DEFAULT)
			{
				Color = FLinearColor::Green;
				Color2 = FLinearColor::Green;
			}
		});
	}
	
}


void UStaminaBar::NativeTick(const FGeometry& MovieSceneBlends, float InDeltaTime)
{
	Super::NativeTick(MovieSceneBlends, InDeltaTime);
	
	if(Link != nullptr)
	{
		
		
		if (StaminaState == EStaminaState::DEPLETION)
		{
			Color = DepletionColor;
			Color2 = DepletionColor;
		}
		else if(StaminaState == EStaminaState::USING)
		{
        	Color2 = DepletionColor;
        }

		if(bDepletionColor_incr)
		{
			DepletionColor.G += 0.02;
			if(DepletionColor.G >= 0.3) bDepletionColor_incr = false;
		}
		else
		{
			DepletionColor.G -= 0.02;
			if(DepletionColor.G <= 0) bDepletionColor_incr = true;
		}
		
		M_Dynamic->SetVectorParameterValue(TEXT("Color"),Color);
		M_Dynamic->SetVectorParameterValue(TEXT("Color2"),Color2);
		M_Dynamic->SetScalarParameterValue(TEXT("Decimal"),Link->GetStamina() / Link->GetMaxStamina());
	}
}

