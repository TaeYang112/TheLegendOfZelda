// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Link.h"
#include "Blueprint/UserWidget.h"
#include "StaminaBar.generated.h"

/**
 * 
 */
UCLASS()
class THELEGENDOFZELDA_API UStaminaBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY( Meta = (BindWidget ))
	class UImage *Progress_Image;
	
	FTimerHandle StaminaTimerHandle;			// 스테미나 색상변경 타이머
	FLinearColor DepletionColor;				// 스테미나 고갈 혹은 사용중일때 표시되는 색 ( 빨간색 ~ 주황색 으로 천천히 변화 )
	bool bDepletionColor_incr;					// true : 빨강 -> 주황    false : 주황 ~ 빨강

	UPROPERTY()
	UMaterialInstanceDynamic* M_Dynamic;
	
	EStaminaState StaminaState;

	UPROPERTY()
	ALink *Link;								// 위젯 소유 액터

	UPROPERTY()
	UStaminaSystem *StaminaSystem;

	FTimerHandle DisappearDelayTimerHandle;		// 위젯을 천천히 사라지게, 나타내게 하는 타이머를 호출하기전 딜레이를 주는 타이머
	FTimerHandle DisappearTimerHandle;			// 위젯을 천천히 사라지게, 나타나게 하는 타이머
	float widgetOpacity;						// 위젯 투명도
	
	
public:
	virtual void NativeConstruct() override;
	void UpdateStaminaColor();
	void MakeAppear();
	void MakeDisappear();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	
	
};
