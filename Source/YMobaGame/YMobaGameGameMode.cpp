// Copyright Epic Games, Inc. All Rights Reserved.

#include "YMobaGameGameMode.h"
#include "YMobaGamePlayerController.h"
#include "YMobaGameState.h"
#include "YMobaGameHUD.h"
#include "UObject/ConstructorHelpers.h"

AYMobaGameGameMode::AYMobaGameGameMode()
{
	//∆Ù”√ Tick.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	// use our custom PlayerController class
	PlayerControllerClass = AYMobaGamePlayerController::StaticClass();

	HUDClass = AYMobaGameHUD::StaticClass();

	GameStateClass = AYMobaGameState::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/YMobaGamePawn"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

// Called when the game starts or when spawned
void AYMobaGameGameMode::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AYMobaGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
