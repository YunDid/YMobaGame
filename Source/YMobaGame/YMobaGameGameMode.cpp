// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGameGameMode.h"
#include "YMobaGamePlayerController.h"
#include "YMobaGameState.h"
#include "UObject/ConstructorHelpers.h"

AYMobaGameGameMode::AYMobaGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AYMobaGamePlayerController::StaticClass();

	GameStateClass = AYMobaGameState::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/YMobaGamePawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}