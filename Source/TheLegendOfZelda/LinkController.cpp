// Fill out your copyright notice in the Description page of Project Settings.


#include "LinkController.h"

#include "MainHud.h"
#include "Blueprint/UserWidget.h"

ALinkController::ALinkController()
{

	static ConstructorHelpers::FClassFinder<UMainHud> HUD_CLASS(TEXT("WidgetBlueprint'/Game/TheLegendOfZeldaBP/System/HUD/MainHud.MainHud_C'"));

	if(HUD_CLASS.Succeeded())
	{
		MainHudClass = HUD_CLASS.Class;
	}
	
	
}

void ALinkController::BeginPlay()
{
	Super::BeginPlay();
	if(MainHudClass != nullptr)
	{
		MainHud = CreateWidget<UMainHud>(this,MainHudClass,TEXT("MainHud"));
		MainHud->AddToViewport();
	}

}

