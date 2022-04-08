// Fill out your copyright notice in the Description page of Project Settings.

#include "MethodUnit.h"

namespace MethodUnit
{
	AYMobaGameState* GetYMobaGameState(UWorld* CurrentWorld) 
	{
		if (CurrentWorld) {
			return CurrentWorld->GetGameState<AYMobaGameState>();
		}

		return nullptr;
	}
}