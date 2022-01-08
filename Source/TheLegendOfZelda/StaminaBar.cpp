// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaBar.h"
#include "Link.h"
#include "Components/Image.h"

void UStaminaBar::NativeTick(const FGeometry& MovieSceneBlends, float InDeltaTime)
{
	Super::NativeTick(MovieSceneBlends, InDeltaTime);
	
	ALink* Link = Cast<ALink>(GetOwningPlayerPawn());
	if(Link != nullptr)
	{
		UMaterialInstanceDynamic* M_Dynamic = Progress_Image->GetDynamicMaterial();
		/*
		// 스태미나가 고갈상태라면 빨간색으로 표시
		if(Link->IsStaminaDepletion() == true)
		{
			Color = FLinearColor::Red;
			Color2 = FLinearColor::Red;
		}
		// 스태미나를 사용중이라면 끝에 일부분을 빨간색으로 표시
		else if(Link->IsUseStamina() == true)
			Color2 = FLinearColor::Red;
		*/
		M_Dynamic->SetVectorParameterValue(TEXT("Color"),Color);
		M_Dynamic->SetVectorParameterValue(TEXT("Color2"),Color2);
		M_Dynamic->SetScalarParameterValue(TEXT("Decimal"),Link->GetStamina() / Link->GetMaxStamina());
	}
}

void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();
	ALink* sdf = Cast<ALink>(GetOwningPlayerPawn());
	if(sdf)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("%s"),ToCStr(sdf->GetName())));
	}
	

}
