// Fill out your copyright notice in the Description page of Project Settings.

#include "MethodUnit.h"

namespace MethodUnit
{
	AYMobaGameState* GetYMobaGameState_Unit(UWorld* CurrentWorld)
	{
		if (CurrentWorld) {
			return CurrentWorld->GetGameState<AYMobaGameState>();
		}

		return nullptr;
	}

	const FCharacterTable* GetFCharaterTableByID_Unit(UWorld* CurrentWorld, const int64& CharaterID) {
		//��ȡ GameState.
		AYMobaGameState* YMobaGameState_Ins = GetYMobaGameState_Unit(CurrentWorld);
		//��ȡ FCharacterTable.
		if (YMobaGameState_Ins) {
			return YMobaGameState_Ins->GetFCharaterTableByID(CharaterID);
		}

		return nullptr;
	}

	const TArray<FCharacterTable*>* GetFCharaterTable_Cache_Unit(UWorld* CurrentWorld) {
		//��ȡ GameState.
		AYMobaGameState* YMobaGameState_Ins = GetYMobaGameState_Unit(CurrentWorld);

		return YMobaGameState_Ins->GetFCharaterTable_Cache();
	}

}