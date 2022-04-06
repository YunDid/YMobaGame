// Fill out your copyright notice in the Description page of Project Settings.

#include "MethodUnit.h"

namespace MethodUnit
{
	AMobaGameState* GetMobaGameState(UWorld* CurrentWorld) 
	{
		if (CurrentWorld) {
			return CurrentWorld->GetGameState<AMobaGameState>();
		}

		return nullptr;
	}
}