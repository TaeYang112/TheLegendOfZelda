// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaBar.h"
#include "Components/Image.h"


void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();

	// �ʱ�ȭ
	DepletionColor = FLinearColor::Red;
	bDepletionColor_incr = true;
	
	M_Dynamic = Progress_Image->GetDynamicMaterial();
	M_Dynamic->SetVectorParameterValue(TEXT("Color"), FLinearColor::Green);	
	M_Dynamic->SetVectorParameterValue(TEXT("Color2"), FLinearColor::Green);
	
	Link = Cast<ALink>(GetOwningPlayerPawn());
	StaminaSystem = Link->GetStaminaSystem();
	
	Progress_Image->SetOpacity(0);


	
	// ��ũ�� ���׹̳� ���°� ����Ǹ� ȣ��
	StaminaSystem->StaminaStateChanged.AddLambda([this]()->void
	{
		StaminaState = StaminaSystem->GetStaminaState();
			
		if(StaminaState == EStaminaState::FULL || StaminaState == EStaminaState::REGEN)
		{
			// ���°� �븻�ϰ�� ���׹̳� ���� �ʷϻ����� ����
			M_Dynamic->SetVectorParameterValue(TEXT("Color"), FLinearColor::Green);	// ���׹̳� �������� �κ�
			M_Dynamic->SetVectorParameterValue(TEXT("Color2"), FLinearColor::Green);	// ���׹̳� ���κ�
			
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);				// ���� �ʷϻ� �����̹Ƿ� ���󺯰� Ÿ�̸Ӹ� ����

			
			// ���� ���׹̳��� ��á�ٸ�
			if(StaminaState == EStaminaState::FULL)
			{
				// ���� �ð��� ��ٸ� ��,
				GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle,FTimerDelegate::CreateUObject(this,&UStaminaBar::MakeDisappear),0.8f,false);
			}
		}
		else
		{
			// �븻�� �ƴҰ�� ChangeStaminaColor�� �ֱ������� ȣ���� ��Ȳ ~ �������� ǥ��
			GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, FTimerDelegate::CreateUObject(this, &UStaminaBar::UpdateStaminaColor), 0.02f, true);

			// ������ ���¿��� ������ ������ �������ϰ� �ٲ�
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
	// ���׹̳� �Ҹ����̰ų� �������϶� ������ �� ( ��Ȳ ~ ���� ) ����
	if (bDepletionColor_incr)
	{
		// ���� -> ��Ȳ
		DepletionColor.G += 0.02;
		if (DepletionColor.G >= 0.3) bDepletionColor_incr = false;
	}
	else
	{
		// ��Ȳ -> ����
		DepletionColor.G -= 0.02;
		if (DepletionColor.G <= 0) bDepletionColor_incr = true;
	}
	
	if (StaminaState == EStaminaState::DEPLETION)
	{
		M_Dynamic->SetVectorParameterValue(TEXT("Color"), DepletionColor);
		M_Dynamic->SetVectorParameterValue(TEXT("Color2"), DepletionColor);	// ���׹̳� ���κ�
	}
	else if (StaminaState == EStaminaState::USING)
	{
		M_Dynamic->SetVectorParameterValue(TEXT("Color2"), DepletionColor); // ���׹̳� ���κ�
	}

	
}

void UStaminaBar::MakeAppear()
{
	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle,FTimerDelegate::CreateLambda([this]
	{
		widgetOpacity = FMath::Clamp(widgetOpacity+0.15,0,1);
		Progress_Image->SetOpacity(widgetOpacity);

		// ������������ Ÿ�̸� ����
		if(widgetOpacity == 1)
			GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
	}),0.02,true);
}

void UStaminaBar::MakeDisappear()
{
	// ������ ���ݾ� �����ϰ� ����
	GetWorld()->GetTimerManager().SetTimer(DisappearTimerHandle,FTimerDelegate::CreateLambda([this]
	{
		widgetOpacity = FMath::Clamp(widgetOpacity-0.15,0,1);
		Progress_Image->SetOpacity(widgetOpacity);
						
		// ���������� Ÿ�̸� ����
		if(widgetOpacity == 0)
			GetWorld()->GetTimerManager().ClearTimer(DisappearTimerHandle);
	}),0.02,true);
}