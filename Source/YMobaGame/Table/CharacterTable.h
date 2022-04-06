// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//#include "GameFramework/Pawn.h"
#include "CharacterTable.generated.h"

USTRUCT(BluePrintType)
struct FCharacterTable : public FTableRowBase 
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterTable();

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	int64 CharacterID;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class")
	TSubclassOf<APawn> CharacterClass;
};
