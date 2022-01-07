// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LinkController.generated.h"

/**
 * 
 */
UCLASS()
class THELEGENDOFZELDA_API ALinkController : public APlayerController
{
	GENERATED_BODY()

	ALinkController();
	
private:
	UPROPERTY()
	class UUserWidget* MainHud;

	TSubclassOf<class UMainHud> MainHudClass;

public:
	virtual void BeginPlay() override;
	
};
