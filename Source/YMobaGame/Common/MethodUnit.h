// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../YMobaGameState.h"

namespace MethodUnit
{
	AYMobaGameState* GetYMobaGameState_Unit(UWorld* CurrentWorld);

	const FCharacterTable* GetFCharaterTableByID_Unit(UWorld* CurrentWorld, const int64& CharaterID);

	//获取所有配置表缓存.
	const TArray<FCharacterTable*>* GetFCharaterTable_Cache_Unit(UWorld* CurrentWorld);
}
