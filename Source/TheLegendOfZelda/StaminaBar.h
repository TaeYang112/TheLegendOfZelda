// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
