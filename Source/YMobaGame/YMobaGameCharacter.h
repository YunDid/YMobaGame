// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YMobaGameCharacter.generated.h"

UCLASS(Blueprintable)
class AYMobaGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AYMobaGameCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};

