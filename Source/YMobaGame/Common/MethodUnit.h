// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../YMobaGameState.h"

namespace MethodUnit
{
	AYMobaGameState* GetYMobaGameState_Unit(UWorld* CurrentWorld);

	const FCharacterTable* GetFCharaterTableByID_Unit(UWorld* CurrentWorld, const int32& CharaterID);

	//��ȡ�������ñ���.
	const TArray<FCharacterTable*>* GetFCharaterTable_Cache_Unit(UWorld* CurrentWorld);

	int32 GetCurrentCharacterID(UWorld* CurrentWorld, int64 PlayerID);
}
