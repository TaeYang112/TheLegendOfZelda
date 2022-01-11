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
	
	FLinearColor Color;					// 스테미나 끝부분 제외
	FLinearColor Color2;				// 스테미나 끝부분 ( 스테미나 사용중 DepletionColor으로 색 변경 )
	FLinearColor DepletionColor;		// 스테미나 고갈 혹은 사용중일때 표시되는 색 ( 빨간색 ~ 주황색 으로 천천히 변화 )
	bool bDepletionColor_incr;

	EStaminaState StaminaState;

	UPROPERTY()
	ALink *Link;
	
	public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

	
	
};
