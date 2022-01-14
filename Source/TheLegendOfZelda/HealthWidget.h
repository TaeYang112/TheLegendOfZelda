// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class THELEGENDOFZELDA_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	void SetHeartLevel(float Level);
private:
	UPROPERTY( Meta = (BindWidget ))
	class UImage *Heart_Image;

	UPROPERTY()
	UMaterialInstanceDynamic* M_Dynamic;
	
};
