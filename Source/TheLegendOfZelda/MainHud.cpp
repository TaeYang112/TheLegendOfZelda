// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHud.h"

#include "HealthWidget.h"
#include "Link.h"
#include "Components/WrapBox.h"

void UMainHud::NativePreConstruct()
{
	Super::NativePreConstruct();
	static UBlueprintGeneratedClass* BP_HealthBarClass = LoadObject<UBlueprintGeneratedClass>(nullptr,
		TEXT("WidgetBlueprint'/Game/TheLegendOfZeldaBP/System/HUD/HealthBarWidget.HealthBarWidget_C'"));

	HealthBarClass = Cast<UClass>(BP_HealthBarClass);

	
}

void UMainHud::NativeConstruct()
{
	Super::NativeConstruct();

	ALink* Link = Cast<ALink>(GetOwningPlayerPawn());
	Link = Cast<ALink>(GetOwningPlayerPawn());
	if(Link != nullptr)
	{
		Link->OnHPChanged.AddLambda([this]()->void
		{
			UpdateWidget();
		});
	}
	
	UpdateWidget();
}

void UMainHud::UpdateWidget()
{
	ALink* Link = Cast<ALink>(GetOwningPlayerPawn());
	if(Link != nullptr)
	{
		
		float HeartLevel = Link->GetHP() / 4.0f;
		int32 MaxHP = Link->GetMaxHP();

		for(int32 i=0; i<MaxHP / 4; i++)
		{
			
			if(HealthArray.Num() <= i)	// 만약 아직 HP위젯이 만들어져있지 않으면 위젯 생성
			{
				UUserWidget* NewWidget = CreateWidget(GetOwningPlayer(),HealthBarClass,FName("HealthWidget_" + i));
				HealthWrapBox->AddChild(NewWidget);
				HealthArray.Add(Cast<UHealthWidget>(NewWidget));
				
			}

			// 하트가 어느정도 만큼 표시될지 설정
			if(HeartLevel >= 1)
			{
				HealthArray[i]->SetHeartLevel(1);
				HeartLevel-= 1;
			}
			else
			{
				HealthArray[i]->SetHeartLevel(HeartLevel);
				HeartLevel= 0;
			}
		}

		// 필요없는 하트 위젯을 제거 ( MaxHP 보다 위젯 개수가 더 많을경우)
		int count = HealthArray.Num() - MaxHP/4;
		for(int i=count - 1; i>= 0; i--)
		{
			HealthArray[i]->RemoveFromParent();
			HealthArray.RemoveAt(i);
		}
		
	}
}
