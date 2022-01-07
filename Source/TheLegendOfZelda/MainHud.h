// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

/**
 * 
 */
UCLASS()
class THELEGENDOFZELDA_API UMainHud : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY( Meta = (BindWidget ))
	class UStaminaBar* StaminaWidget;
	
};
