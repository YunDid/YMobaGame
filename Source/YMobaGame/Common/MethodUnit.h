// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../YMobaGameState.h"

namespace MethodUnit
{
	AYMobaGameState* GetYMobaGameState_Unit(UWorld* CurrentWorld);

	const FCharacterTable* GetFCharaterTableByID_Unit(UWorld* CurrentWorld, const int64& CharaterID);
}
