// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "YMobaGameTableBase.generated.h"

USTRUCT(BlueprintType)
struct FYMobaGameTableBase :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FYMobaGameTableBase();

	UPROPERTY(EditDefaultsOnly, Category = "Moba Game Table")
	int32 CharacterID;
};