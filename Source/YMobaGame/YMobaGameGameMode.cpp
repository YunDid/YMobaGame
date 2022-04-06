// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGameGameMode.h"
#include "YMobaGamePlayerController.h"
#include "YMobaGameCharacter.h"
#include "MobaGameState.h"
#include "UObject/ConstructorHelpers.h"

AYMobaGameGameMode::AYMobaGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AYMobaGamePlayerController::StaticClass();

	GameStateClass = AMobaGameState::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/MobaPawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}