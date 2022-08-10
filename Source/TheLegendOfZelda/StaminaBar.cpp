// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaBar.h"
#include "Components/Image.h"


void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();

	// 초기화
	DepletionColor = FLinearColor::Red;
	bDepletionColor_incr = true;
	
	M_Dynamic = Progress_Image->GetDynamicMaterial();
	M_Dynamic->SetVectorParameterValue(TEXT("Color"), FLinearColor::Green);	
	M_Dynamic->SetVectorParameterValue(TEXT("Color2"), FLinearColor::Green);
	
	Link = Cast<ALink>(GetOwningPlayerPawn());
	StaminaSystem = Link->GetStaminaSystem();
	
	Progress_Image->SetOpacity(0);


	
	// 링크의 스테미나 상태가 변경되면 호출
	StaminaSystem->StaminaStateChanged.AddLambda([this]()->void
	{
		StaminaState = StaminaSystem->GetStaminaState();
			
		if(StaminaState == EStaminaState::FULL || StaminaState == EStaminaState::REGEN)
		{
			// 상태가 노말일경우 스테미나 색을 초록색으로 변경
			M_Dynamic->SetVectorParameterValue(TEXT("Color"), FLinearColor::Green);	// 스테미나 전반적인 부분
			M_Dynamic->SetVectorParameterValue(TEXT("Color2"), FLinearColor::Green);	// 스테미나 끝부분
			
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);				// 색이 초록색 고정이므로 색상변경 타이머를 종료

			
			// 만약 스테미나가 꽉찼다면
			if(StaminaState == EStaminaState::FULL)
			{
				// 일정 시간을 기다린 뒤,
				GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle,FTimerDelegate::CreateUObject(this,&UStaminaBar::MakeDisappear),0.8f,false);
			}
		}
		else
		{
			// 노말이 아닐경우 ChangeStaminaColor를 주기적으로 호출해 주황 ~ 빨강색을 표시
			GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, FTimerDelegate::CreateUObject(this, &UStaminaBar::UpdateStaminaColor), 0.02f, true);

			// 투명한 상태였던 위젯을 서서히 불투명하게 바꿈
			MakeAppear();
		}
		
	});

}


void UStaminaBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	M_Dynamic->SetScalarParameterValue(TEXT("Decimal"), StaminaSystem->GetStamina() / StaminaSystem->GetMaxStamina());
}



void UStaminaBar::UpdateStaminaColor()
{
	// 스테미나 소모중이거나 고갈상태일때 나오는 색 ( 주황 ~ 빨강 ) 결정
	if (bDepletionColor_incr)
	{
		// 빨강 -> 주황
		DepletionColor.G += 0.02;
		if (DepletionColor.G >= 0.3) bDepletionColor_incr = false;
	}
	else
	{
		// 주황 -> 빨강
		DepletionColor.G -= 0.02;
		if (DepletionColor.G <= 0) bDepletionColor_incr = true;
	}
	
	if (StaminaState == EStaminaState::DEPLETION)
	{
		M_Dynamic->SetVectorParameterValue(TEXT("Color"), DepletionColor);
		M_Dynamic->SetVectorParameterValue(TEXT("Color2"), DepletionColor);	// 스테미나 끝부분
	}
	else if (StaminaState == EStaminaState::USING)
	{
		M_Dynamic->SetVectorParameterValue(TEXT("Color2"), DepletionColor); // 스테미나 끝부분
	}

	
}

void UStaminaBar::MakeAppear()
{
	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle,FTimerDelegate::CreateLambda([this]
	{
		widgetOpacity = FMath::Clamp(widgetOpacity+0.15,0,1);
		Progress_Image->SetOpacity(widgetOpacity);

		// 불투명해지면 타이머 종료
		if(widgetOpacity == 1)
			GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
	}),0.02,true);
}

void UStaminaBar::MakeDisappear()
{
	// 위젯을 조금씩 투명하게 만듬
	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle,FTimerDelegate::CreateLambda([this]
	{
		widgetOpacity = FMath::Clamp(widgetOpacity-0.15,0,1);
		Progress_Image->SetOpacity(widgetOpacity);
						
		// 투명해지면 타이머 종료
		if(widgetOpacity == 0)
			GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
	}),0.02,true);
}