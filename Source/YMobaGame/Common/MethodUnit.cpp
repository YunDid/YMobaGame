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

	const FCharacterTable* GetFCharaterTableByID_Unit(UWorld* CurrentWorld, const int32& CharacterID) {
			
		//获取 GameState.
		AYMobaGameState* YMobaGameState_Ins = GetYMobaGameState_Unit(CurrentWorld);
		//获取 FCharacterTable.
		if (YMobaGameState_Ins) {
			return YMobaGameState_Ins->GetFCharaterTableByID(CharacterID);
		}

		return nullptr;
	}

	const TArray<FCharacterTable*>* GetFCharaterTable_Cache_Unit(UWorld* CurrentWorld) {
		//获取 GameState.
		AYMobaGameState* YMobaGameState_Ins = GetYMobaGameState_Unit(CurrentWorld);

		return YMobaGameState_Ins->GetFCharaterTableCache_Template();
	}

	int32 GetCurrentCharacterID(UWorld* CurrentWorld, int64 PlayerID)
	{
		if (AYMobaGameState* GameState_Ins = GetYMobaGameState_Unit(CurrentWorld))
		{
			return GameState_Ins->GetCurrentCharacterID(PlayerID);
		}

		return INDEX_NONE;
	}

}