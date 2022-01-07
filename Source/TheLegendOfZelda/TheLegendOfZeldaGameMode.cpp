// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheLegendOfZeldaGameMode.h"
#include "Link.h"
#include "LinkController.h"

ATheLegendOfZeldaGameMode::ATheLegendOfZeldaGameMode()
{
	// set default pawn class to our Blueprinted character

	DefaultPawnClass = ALink::StaticClass();
	PlayerControllerClass = ALinkController::StaticClass();
}
